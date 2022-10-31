#ifndef YAGI_GFX_GL_STATIC_BUFFER_H
#define YAGI_GFX_GL_STATIC_BUFFER_H

#include "yagi/gfx/gl/buffer_base.h"
#include "yagi/util/enum_bitmask_ops.h"
#include <concepts>
#include <vector>

namespace yagi {

template <typename T>
concept Numeric = requires(T) { std::integral<T> || std::floating_point<T>; };

template <Numeric T>
class StaticBuffer : public BufferBase {
public:
  StaticBuffer(
      std::unique_ptr<GladGLContext> &gl,
      const BufTarget &target,
      const BufUsage &usage,
      const std::vector<T> &data
  );
  ~StaticBuffer() override = default;

  // Copy constructors don't make sense for OpenGL objects
  StaticBuffer(const StaticBuffer &) = delete;
  StaticBuffer &operator=(const StaticBuffer &) = delete;

  StaticBuffer(StaticBuffer &&other) noexcept;
  StaticBuffer &operator=(StaticBuffer &&other) noexcept;

  void write(
      const BufTarget &target,
      const BufUsage &usage,
      const std::vector<T> &data
  );
};

template <Numeric T>
StaticBuffer<T>::StaticBuffer(
    std::unique_ptr<GladGLContext> &gl,
    const BufTarget &target,
    const BufUsage &usage,
    const std::vector<T> &data
) : BufferBase(gl) {
  write(target, usage, data);
}

template <Numeric T>
StaticBuffer<T>::StaticBuffer(StaticBuffer &&other) noexcept : BufferBase(std::move(other)) {

}

template <Numeric T>
void StaticBuffer<T>::write(
    const BufTarget &target,
    const BufUsage &usage,
    const std::vector<T> &data
) {
  bind(target);
  gl_->BufferData(unwrap(target), sizeof(T) * data.size(), &data[0], unwrap(usage));
  unbind(target);
}

template <Numeric T>
StaticBuffer<T> &StaticBuffer<T>::operator=(StaticBuffer &&other) noexcept {
  if (this != &other) {
    BufferBase::operator=(std::move(other));
  }
  return *this;
}

} // namespace yagi

#endif //YAGI_GFX_GL_STATIC_BUFFER_H
