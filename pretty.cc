// (c) Robert Muth - see LICENSE for more info

#include "pretty.h"

namespace PrettyPrint {
std::vector<size_t> CompureSizes(std::vector<Token> tokens) {
  std::vector<size_t> out;
  out.reserve(tokens.size());
  return out;
}

std::string PrettyPrint(std::vector<Token> tokens, size_t line_width) {
  std::vector<size_t> sizes = CompureSizes(tokens);
  line_width = line_width;
  return "";
}

}  // namespace PrettyPrint
