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
  fixed = GL_FIXED,
  i_2_10_10_10_rev = GL_INT_2_10_10_10_REV,
  ui_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV,
  ui_10f_11f_11f_rev = GL_UNSIGNED_INT_10F_11F_11F_REV
};

enum class DrawMode {
  points = GL_POINTS,
  line_strip = GL_LINE_STRIP,
  line_loop = GL_LINE_LOOP,
  lines = GL_LINES,
  line_strip_adjacency = GL_LINE_STRIP_ADJACENCY,
  lines_adjacency = GL_LINES_ADJACENCY,
  triangle_strip = GL_TRIANGLE_STRIP,
  triangle_fan = GL_TRIANGLE_FAN,
  triangles = GL_TRIANGLES,
  triangle_strip_adjacency = GL_TRIANGLE_STRIP_ADJACENCY,
  triangles_adjacency = GL_TRIANGLES_ADJACENCY,
  patches = GL_PATCHES
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
      GladGLContext &gl,
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

  void draw_arrays(const DrawMode &mode, GLint first, GLsizei count);

private:
  GladGLContext &gl_;

  void gen_id_();
  void del_id_();

  void process_buf_attrs_(Shader &shader, const std::vector<BufAttrs> &buf_attrs);

  static std::unordered_map<AttrType, GLsizei> attr_type_size;
};

} // namespace yagi

std::ostream &operator<<(std::ostream &out, yagi::AttrType value);

#endif //YAGI_GFX_GL_VERTEX_ARRAY_H
