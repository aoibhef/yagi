#include "yagi/core/application.h"
#include "yagi/util/log.h"
#include "yagi/util/overloaded.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

namespace yagi {

Application::Application() {
  msg_endpoint_id_ = Bus::register_endpoint([&](const Msg &msg) { received_msg_(msg); });
  Bus::subscribe(msg_endpoint_id_, MsgType::ImguiLog);

  window = std::make_unique<Window>();
  input = std::make_unique<InputMgr>();
}

void Application::initialize() {}
void Application::deinitialize() {}
void Application::update(double dt) {}
void Application::draw() {}

void Application::app_debug_overlay_enabled(bool enabled) {
  overlay.enabled = enabled;
}

void Application::received_msg_(const Msg &msg) {
  std::visit(overloaded {
      [&](const ImguiLog &m) { overlay.log.add("%s", m.msg.c_str()); },
      [&](const auto &) { YAGI_LOG_WARN("Unhandled event {}", msg.type); }
  }, msg.data);
}

void Application::update_() {
  framecounter_.update();

  update(framecounter_.dt());
}

void Application::start_frame_() {
  imgui_start_frame_();
}

void Application::draw_() {
  start_frame_();

  draw();

  end_frame_();
}

void Application::draw_debug_overlay_() {
  ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
  if (ImGui::Begin("Overlay_Perf", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("%s", fmt::format("FPS: {:.2f}", framecounter_.fps()).c_str());

    auto dts = framecounter_.dts_vec();
    auto [dt_min, dt_max] = std::minmax_element(dts.begin(), dts.end());
    double mid = (*dt_max + *dt_min) / 2.0;

    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, {0, 0});
    if (ImPlot::BeginPlot("FPS_History", {100,35}, ImPlotFlags_CanvasOnly | ImPlotFlags_NoFrame | ImPlotFlags_NoInputs)) {
      ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
      ImPlot::SetupAxisLimits(ImAxis_X1, 0, dts.size(), ImPlotCond_Always);
      ImPlot::SetupAxisLimits(ImAxis_Y1, mid - 1.5 * (*dt_max - mid), mid + 1.5 * (*dt_max - mid), ImPlotCond_Always);
      ImPlot::PlotLine("History", &dts[0], dts.size());
      ImPlot::EndPlot();
    }
    ImPlot::PopStyleVar();
  }
  ImGui::End();

  ImGui::SetNextWindowPos({0.0f, ImGui::GetIO().DisplaySize.y}, 0, {0.0f, 1.0f});
  ImGui::SetNextWindowSize({ImGui::GetIO().DisplaySize.x * 0.4f, ImGui::GetIO().DisplaySize.y * 0.25f}, ImGuiCond_Once);
  overlay.log.draw("Debug Log");
}

void Application::end_frame_() {
  imgui_end_frame_();

  window->swap();
}

void Application::post_draw_() {
  input->update(framecounter_.dt());
  
  glfwPollEvents();
}

void Application::imgui_start_frame_() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Application::imgui_end_frame_() {
  if (overlay.enabled)
    draw_debug_overlay_();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImguiLogWindow::ImguiLogWindow() {
  autoscroll_ = true;
  clear();
}

void ImguiLogWindow::clear() {
  buf_.clear();
  line_offsets_.clear();
  line_offsets_.push_back(0);
}

void ImguiLogWindow::add(const char *fmt, ...) {
  int old_size = buf_.size();
  va_list args;
      va_start(args, fmt);
  buf_.appendfv(fmt, args);
      va_end(args);
  for (int new_size = buf_.size(); old_size < new_size; old_size++)
    if (buf_[old_size] == '\n')
      line_offsets_.push_back(old_size + 1);
}

void ImguiLogWindow::draw(const char *title, bool *p_open) {
  if (!ImGui::Begin(title, p_open)) {
    ImGui::End();
    return;
  }

  // Options menu
  if (ImGui::BeginPopup("Options")) {
    ImGui::Checkbox("Auto-scroll", &autoscroll_);
    ImGui::EndPopup();
  }

  // Main window
  if (ImGui::Button("Options"))
    ImGui::OpenPopup("Options");
  ImGui::SameLine();
  if (ImGui::Button("Clear"))
    clear();
  ImGui::SameLine();
  if (ImGui::Button("Copy"))
    ImGui::LogToClipboard();
  ImGui::SameLine();
  filter_.Draw("Filter", -100.0f);

  ImGui::Separator();
  ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  const char* buf = buf_.begin();
  const char* buf_end = buf_.end();
  if (filter_.IsActive()) {
    for (int line_no = 0; line_no < line_offsets_.Size; line_no++) {
      const char* line_start = buf + line_offsets_[line_no];
      const char* line_end = (line_no + 1 < line_offsets_.Size) ? (buf + line_offsets_[line_no + 1] - 1) : buf_end;
      if (filter_.PassFilter(line_start, line_end))
        ImGui::TextUnformatted(line_start, line_end);
    }
  } else {
    ImGuiListClipper clipper;
    clipper.Begin(line_offsets_.Size);
    while (clipper.Step()) {
      for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
        const char* line_start = buf + line_offsets_[line_no];
        const char* line_end = (line_no + 1 < line_offsets_.Size) ? (buf + line_offsets_[line_no + 1] - 1) : buf_end;
        ImGui::TextUnformatted(line_start, line_end);
      }
    }
    clipper.End();
  }
  ImGui::PopStyleVar();

  if (autoscroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    ImGui::SetScrollHereY(1.0f);

  ImGui::EndChild();
  ImGui::End();
}

} // namespace yagi
