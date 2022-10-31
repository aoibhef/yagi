#include "yagi/msg/bus.h"
#include "fmt/format.h"

int main(int, char *[]) {
  fmt::print("2 + 2 = {}\n", yagi::add(2, 2));
}
