#include "yagi/gfx/gl/buffer_base.h"
#include "yagi/util/enum_bitmask_ops.h"
#include "yagi/util/log.h"

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
