#ifndef YAGI_GFX_GL_BUFFER_BASE_H
#define YAGI_GFX_GL_BUFFER_BASE_H

#include "glad/gl.h"
#include <memory>

namespace yagi {

enum class BufTarget {
  none = 0,
  array = GL_ARRAY_BUFFER,
  atomic_counter = GL_ATOMIC_COUNTER_BUFFER,
  copy_read = GL_COPY_READ_BUFFER,
  copy_write = GL_COPY_WRITE_BUFFER,
  dispatch_indirect = GL_DISPATCH_INDIRECT_BUFFER,
  draw_indirect = GL_DRAW_INDIRECT_BUFFER,
  element_array = GL_ELEMENT_ARRAY_BUFFER,
  pixel_pack = GL_PIXEL_PACK_BUFFER,
  pixel_unpack = GL_PIXEL_UNPACK_BUFFER,
  query = GL_QUERY_BUFFER,
  shader_storage = GL_SHADER_STORAGE_BUFFER,
  texture = GL_TEXTURE_BUFFER,
  transform_feedback = GL_TRANSFORM_FEEDBACK_BUFFER,
  uniform = GL_UNIFORM_BUFFER,
};

enum class BufUsage {
  none = 0,
  stream_draw = GL_STREAM_DRAW,
  stream_read = GL_STREAM_READ,
  stream_copy = GL_STREAM_COPY,
  static_draw = GL_STATIC_DRAW,
  static_read = GL_STATIC_READ,
  static_copy = GL_STATIC_COPY,
  dynamic_draw = GL_DYNAMIC_DRAW,
  dynamic_read = GL_DYNAMIC_READ,
  dynamic_copy = GL_DYNAMIC_COPY,
};

class BufferBase {
protected:
  std::unique_ptr<GladGLContext> &gl_;

  void gen_id_();
  void del_id_();

public:
  GLuint id{};

  explicit BufferBase(std::unique_ptr<GladGLContext> &gl);
  virtual ~BufferBase();

  // Copy constructors don't make sense for OpenGL objects
  BufferBase(const BufferBase &) = delete;
  BufferBase &operator=(const BufferBase &) = delete;

  BufferBase(BufferBase &&other) noexcept;
  BufferBase &operator=(BufferBase &&other) noexcept;

  void bind(const BufTarget &target) const;
  void unbind(const BufTarget &target) const;
};

} // namespace yagi

#endif //YAGI_GFX_GL_BUFFER_BASE_H
