#include "Parser.h"

#include <istream>
#include <nlohmann/json.hpp>

#include "Errors.h"

using nlohmann::json;

Request Parser::parse(std::istream& in) {
  try {
    json j;
    in >> j;

    if (!j.contains("operation") || !j["operation"].is_string()) {
      throw ParseError("field 'operation' is missing or not a string");
    }

    if (!j.contains("args") || !j["args"].is_array()) {
      throw ParseError("field 'args' is missing or not an array");
    }

    std::string operation = j["operation"];

    std::vector<int> args;
    for (const auto& el : j["args"]) {
      if (!el.is_number_integer()) {
        throw ParseError("all elements of 'args' must be integers");
      }
      args.push_back(el.get<int>());
    }

    return Request(operation, args);
  } catch (const json::exception& e) {
    throw ParseError(e.what());
  }
}
