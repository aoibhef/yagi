#include "yagi/util/log.h"
#include "yagi/gfx/gl/vertex_array.h"

namespace yagi {

VertexArray::VertexArray(
    GladGLContext &gl,
    Shader &shader,
    const std::vector<BufAttrs> &buf_attrs
) : gl_(gl) {
  gen_id_();
  process_buf_attrs_(shader, buf_attrs);
}

VertexArray::~VertexArray() {
  del_id_();
}

VertexArray::VertexArray(VertexArray &&other) noexcept : gl_(other.gl_) {
  std::swap(id, other.id);
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
  if (this != &other) {
    del_id_();

    std::swap(gl_, other.gl_);
    std::swap(id, other.id);
  }
  return *this;
}

void VertexArray::bind() const {
  gl_.BindVertexArray(id);
}

void VertexArray::unbind() const {
  gl_.BindVertexArray(0);
}

void VertexArray::draw_arrays(const DrawMode &mode, GLint first, GLsizei count) {
  bind();
  gl_.DrawArrays(unwrap(mode), first, count);
  unbind();
}

void VertexArray::gen_id_() {
  gl_.GenVertexArrays(1, &id);
  YAGI_LOG_DEBUG("Generated vertex array ({})", id);
}

void VertexArray::del_id_() {
  if (id != 0) {
    gl_.DeleteVertexArrays(1, &id);
    YAGI_LOG_DEBUG("Deleted vertex array ({})", id);
  }
}

void VertexArray::process_buf_attrs_(Shader &shader, const std::vector<BufAttrs> &buf_attrs) {
  bind();

  for (const auto &buf_attr : buf_attrs) {
    const auto &buf = buf_attr.first;
    const auto &attrs = buf_attr.second;

    buf.bind(BufTarget::array);
    for (const auto &attr : attrs) {
      GLint loc = shader.get_attrib_loc(attr.name);
      GLsizei type_size = (attr.type_size != -1) ? attr.type_size : attr_type_size[attr.type];

      gl_.VertexAttribPointer(
          loc,
          attr.size,
          unwrap(attr.type),
          attr.normalized ? GL_TRUE : GL_FALSE,
          attr.stride * type_size,
          reinterpret_cast<void *>(attr.offset * type_size)
      );
      gl_.EnableVertexAttribArray(loc);

      YAGI_LOG_DEBUG(
          "Vertex attribute for buf {}: {} {} {} {} {}{}{} {}",
          buf.id,
          attr.name,
          loc,
          attr.size,
          attr.type,
          attr.normalized,
          (attr.type_size != -1) ? " overridden " : " ",
          attr.stride * type_size,
          attr.offset * type_size
      );
    }
    buf.unbind(BufTarget::array);
  }

  unbind();
}

std::unordered_map<AttrType, GLsizei> VertexArray::attr_type_size = {
    {AttrType::b, sizeof(char)},
    {AttrType::ub, sizeof(unsigned char)},
    {AttrType::s, sizeof(short)},
    {AttrType::us, sizeof(unsigned short)},
    {AttrType::i, sizeof(int)},
    {AttrType::ui, sizeof(unsigned int)},
    {AttrType::hf, sizeof(float) / 2},  // FIXME: No way this is right (wait for C++23 native support?)
    {AttrType::f, sizeof(float)},
    {AttrType::d, sizeof(double)},
    {AttrType::fixed, 32},
    {AttrType::i_2_10_10_10_rev, 32},
    {AttrType::ui_2_10_10_10_rev, 32},
    {AttrType::ui_10f_11f_11f_rev, 32}
};

} // namespace yagi

std::ostream &operator<<(std::ostream &out, yagi::AttrType value) {
#define STRINGIFY(p) case (p): return out << #p;
  switch (value) {
    using enum yagi::AttrType;
    STRINGIFY(b)
    STRINGIFY(ub)
    STRINGIFY(s)
    STRINGIFY(us)
    STRINGIFY(i)
    STRINGIFY(ui)
    STRINGIFY(hf)
    STRINGIFY(f)
    STRINGIFY(d)
    STRINGIFY(fixed)
    STRINGIFY(i_2_10_10_10_rev)
    STRINGIFY(ui_2_10_10_10_rev)
    STRINGIFY(ui_10f_11f_11f_rev)
  }
#undef STRINGIFY
}

