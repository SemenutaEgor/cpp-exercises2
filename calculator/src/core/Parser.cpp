#include "Parser.h"

#include "Errors.h"
#include "Request.h"

Request Parser::parse(std::istream&) {
  throw ParseError("parsing not implemented");
}
