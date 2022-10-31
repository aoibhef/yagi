#include "yagi/msg/bus.h"
#include "yagi/util/rnd.h"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

int main(int, char *[]) {
  spdlog::set_level(spdlog::level::debug);

  rnd::debug_show_seed();

  for (int i = 0; i < 100; i++)
    fmt::print("{}\n", rnd::base58(11));
}