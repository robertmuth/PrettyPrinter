// (c) Robert Muth - see LICENSE for more info

#include "pretty.h"

#include <cassert>

namespace PrettyPrint {

std::vector<ssize_t> ComputeSizes(const std::vector<Token>& tokens) {
  std::vector<ssize_t> sizes;
  sizes.reserve(tokens.size());
  ssize_t total = 0;
  std::vector<size_t> scan_stack;
  size_t x;

  for (size_t i = 0; i < tokens.size(); i++) {
    const Token& token = tokens[i];
    switch (token.type) {
      case TokenType::BEG:
        scan_stack.push_back(i);
        sizes.push_back(-total);
        break;
      case TokenType::END:
        sizes.push_back(1);
        x = scan_stack.back();
        sizes[x] += total;
        scan_stack.pop_back();
        if (tokens[x].type == TokenType::BRK) {
          x = scan_stack.back();
          sizes[x] += total;
          scan_stack.pop_back();
        }
        break;
      case TokenType::BRK:
        sizes.push_back(-total);
        // maybe close sizes a preceeding BRK
        x = scan_stack.back();
        if (tokens[x].type == TokenType::BRK) {
          scan_stack.pop_back();
          sizes[x] += total;
        }
        scan_stack.push_back(i);
        total += token.brk.num_space;
        break;
      case TokenType::STR:
        total += token.str.size();
        break;
      default:
        assert(false);  // unreachable
        break;
    }
    sizes.push_back(total);
  }
  return sizes;
}

#if 0
void UpdatesSizesForNoBreaks(const std::vector<Token>& tokens,
                        const std::vector<ssize_t>& sizes) {}
#endif

                        std::string PrettyPrint(const std::vector<Token>& tokens, size_t line_width) {
  std::vector<ssize_t> sizes = ComputeSizes(tokens);
  line_width = line_width;
  return "";
}

}  // namespace PrettyPrint
