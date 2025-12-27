#include "cli.h"

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

static bool parse_flags(int argc, char* argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "h")) != -1) {
    if (opt == 'h') return false;
  }
  return true;
}

static Operation parse_operation(const char* op) {
  if (strcmp(op, "+") == 0) return Operation::Add;
  if (strcmp(op, "-") == 0) return Operation::Sub;
  if (strcmp(op, "x") == 0) return Operation::Mul;
  if (strcmp(op, "/") == 0) return Operation::Div;
  if (strcmp(op, "^") == 0) return Operation::Pow;
  if (strcmp(op, "!") == 0) return Operation::Fact;
  return Operation::Unknown;
}

bool parse_arguments(int argc, char* argv[], Context& ctx) {
  if (!parse_flags(argc, argv)) return false;

  int remaining = argc - optind;
  if (remaining != 2 && remaining != 3) return false;

  ctx.first = atoi(argv[optind]);
  ctx.operation = parse_operation(argv[optind + 1]);

  if (ctx.operation == Operation::Fact) {
    return remaining == 2;
  }

  ctx.second = atoi(argv[optind + 2]);
  return true;
}

void print_help() {
  printf("Usage:\n");
  printf("    calcucator <a> <op> <b>\n");
  printf("    or\n");
  printf("    calculator <n> !\n");
  printf("Operations:\n");
  printf("    + addition\n");
  printf("    - subtraction\n");
  printf("    x multiplication\n");
  printf("    / division\n");
  printf("    ^ power\n");
  printf("    ! factorial\n");
}
