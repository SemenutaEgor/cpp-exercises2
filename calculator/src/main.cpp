#include <cstdio>

#include "cli/cli.h"
#include "context.h"
#include "core/calculator.h"

int main(int argc, char* argv[]) {
  Context ctx{};

  if (!parse_arguments(argc, argv, ctx)) {
    print_help();
    return 1;
  }

  if (!check(ctx)) return 1;
  if (!calculate(ctx)) return 1;

  printf("%lld\n", ctx.result);
  return 0;
}
