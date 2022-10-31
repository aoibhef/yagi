#ifndef YAGI_EXAMPLES_INDEV_H
#define YAGI_EXAMPLES_INDEV_H

#include "yagi/core/oracle.h"

#include "yagi/util/enum_bitmask_ops.h"
#include "yagi/util/log.h"
#include "fmt/format.h"

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

  BufferBase(std::unique_ptr<GladGLContext> &gl);
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

namespace yagi {

BufferBase::BufferBase(std::unique_ptr<GladGLContext> &gl) : gl_(gl) {
  gen_id_();
}

BufferBase::~BufferBase() {
  del_id_();
}

BufferBase::BufferBase(BufferBase &&other) noexcept : gl_(other.gl_) {
  std::swap(id, other.id);
}

BufferBase &BufferBase::operator=(BufferBase &&other) noexcept {
  if (this != &other) {
    del_id_();

    gl_.swap(other.gl_);
    std::swap(id, other.id);
  }
  return *this;
}

void BufferBase::bind(const BufTarget &target) const {
  gl_->BindBuffer(unwrap(target), id);
}

void BufferBase::unbind(const BufTarget &target) const {
  gl_->BindBuffer(unwrap(target), 0);
}

void BufferBase::gen_id_() {
  gl_->GenBuffers(1, &id);
  YAGI_LOG_TRACE("Generated buffer ({})", id);
}

void BufferBase::del_id_() {
  if (id != 0) {
    gl_->DeleteBuffers(1, &id);
    YAGI_LOG_TRACE("Deleted buffer ({})", id);
  }
}

} // namespace yagi

#endif //YAGI_EXAMPLES_INDEV_H
