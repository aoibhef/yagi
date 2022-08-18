#ifndef YAGI_OVERLOADED_H
#define YAGI_OVERLOADED_H

namespace yagi {

template<class ...Ts>
struct overloaded : Ts ... {
  using Ts::operator()...;
};
template<class ...Ts> overloaded(Ts...) -> overloaded<Ts...>;

} // namespace yagi

#endif //YAGI_OVERLOADED_H
