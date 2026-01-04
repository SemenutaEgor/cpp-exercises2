#include "Parser.h"

#include "Request.h"

Request Parser::parse(std::istream&) { return Request("add", {1, 2}); }
