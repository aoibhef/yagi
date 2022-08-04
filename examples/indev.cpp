#include "yagi/util/log.h"
#include "yagi/util/rnd.h"

int main(int, char *[]) {
  LOG_TRACE("Test trace");
  LOG_DEBUG("Test debug");
  LOG_INFO("Test info");
  LOG_ERROR("Test error");
  LOG_CRITICAL("Test critical");

  rnd::debug_show_seed();
}