#ifndef YAGI_GFX_GL_VERTEX_ARRAY_H
#define YAGI_GFX_GL_VERTEX_ARRAY_H

#include "yagi/gfx/gl/shader.h"
#include "yagi/gfx/gl/buffer_base.h"
#include "yagi/util/enum_bitmask_ops.h"
#include "fmt/ostream.h"
#include "glad/gl.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace yagi {

enum class AttrType {
  b = GL_BYTE,
  ub = GL_UNSIGNED_BYTE,
  s = GL_SHORT,
  us = GL_UNSIGNED_SHORT,
  i = GL_INT,
  ui = GL_UNSIGNED_INT,
  hf = GL_HALF_FLOAT,
  f = GL_FLOAT,
  d = GL_DOUBLE,
  fix = GL_FIXED,
  i_2_10_10_10_rev = GL_INT_2_10_10_10_REV,
  ui_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV,
  ui_10f_11f_11f_rev = GL_UNSIGNED_INT_10F_11F_11F_REV
};

struct AttrPointer {
  std::string name;
  GLint size;
  AttrType type;
  bool normalized;
  GLsizei stride;
  GLint offset;
  GLsizei type_size{-1};  // Override AttrType size (multiplied into stride and offset)
};

using BufAttrs = std::pair<const BufferBase &, const std::vector<AttrPointer>>;

class VertexArray {
public:
  GLuint id{};

  VertexArray(
      std::unique_ptr<GladGLContext> &gl,
      Shader &shader,
      const std::vector<BufAttrs> &buf_attrs
  );
  ~VertexArray();

  // Copy constructors don't make sense for OpenGL objects
  VertexArray(const VertexArray &) = delete;
  VertexArray &operator=(const VertexArray &) = delete;

  VertexArray(VertexArray &&other) noexcept;
  VertexArray &operator=(VertexArray &&other) noexcept;

  void bind() const;
  void unbind() const;

private:
  std::unique_ptr<GladGLContext> &gl_;

  void gen_id_();
  void del_id_();

  void process_buf_attrs_(Shader &shader, const std::vector<BufAttrs> &buf_attrs);

  static std::unordered_map<AttrType, GLsizei> attr_type_size;
};

} // namespace yagi

inline std::ostream &operator<<(std::ostream &out, yagi::AttrType value) {
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
    STRINGIFY(fix)
    STRINGIFY(i_2_10_10_10_rev)
    STRINGIFY(ui_2_10_10_10_rev)
    STRINGIFY(ui_10f_11f_11f_rev)
  }
#undef STRINGIFY
}

#endif //YAGI_GFX_GL_VERTEX_ARRAY_H
