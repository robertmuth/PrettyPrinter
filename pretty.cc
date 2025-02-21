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

void UpdatesSizesForNoBreaks(const std::vector<Token>& tokens,
                             std::vector<ssize_t>& sizes) {
  ssize_t total = INFINITE_WIDTH;
  for (size_t j = 0; j < tokens.size(); j++) {
    size_t i = tokens.size() - 1 - j;
    const Token& token = tokens[i];
    switch (token.type) {
      case TokenType::BEG:
        if (token.beg.break_type == BreakType::FORCE_LINE_BREAK) {
          total = INFINITE_WIDTH;
        }
        break;
      case TokenType::END:
        total += INFINITE_WIDTH;
        break;
      case TokenType::STR:
        total += token.str.size();
        break;
      case TokenType::BRK:
        if (token.brk.nobreak) {
          if (total < sizes[i]) {
            sizes[i] = total;
            total += token.brk.num_space;
          } else {
            total = sizes[i];
          }
        } else {
          total = 0;
        }
        break;

      default:
        assert(false);  // unreachable
        break;
    }
  }
  //
  total = 0;
  for (size_t j = 0; j < tokens.size(); j++) {
    size_t i = tokens.size() - 1 - j;
    const Token& token = tokens[i];
    switch (token.type) {
      case TokenType::BEG:
        if (token.beg.break_type == BreakType::FORCE_LINE_BREAK) {
          total = 0;
        }
        break;
      case TokenType::END:
        //
        break;
      case TokenType::STR:
        total += token.str.size();
        break;
      case TokenType::BRK:
        total += token.brk.num_space;
        if (!token.brk.nobreak) {
          if (total > sizes[i]) {
            sizes[i] = total;
          } else {
            total = 0;
          }
        }
        break;

      default:
        assert(false);  // unreachable
        break;
    }
  }
}

std::string PrettyPrint(const std::vector<Token>& tokens, size_t line_width) {
  std::vector<ssize_t> sizes = ComputeSizes(tokens);
  UpdatesSizesForNoBreaks(tokens, sizes);
  line_width = line_width;
  return "";
}

}  // namespace PrettyPrint
