#ifndef YAGI_YAGI_H
#define YAGI_YAGI_H

#include "yagi/core/application.h"
#include "yagi/core/oracle.h"
#include "yagi/core/window.h"

#include "yagi/gfx/color.h"
#include "yagi/gfx/context.h"

#include "fmt/format.h"

#define YAGI_CONSULT_ORACLE(A)           \
  int main(int, char *[]) {              \
    yagi::Oracle().run_application<A>(); \
  }

#endif //YAGI_YAGI_H
