#include "yagi/app/application.h"

namespace yagi {

Application::Application() {
  window = std::make_unique<Window>();
}

void Application::initialize() {}
void Application::update(double dt) {}
void Application::draw() {}

} // namespace yagi
