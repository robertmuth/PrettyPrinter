// (c) Robert Muth - see LICENSE for more info

#include "pretty.h"

#include <cassert>

namespace PP {

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
        total += token.brk.num_spaces;
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

      case TokenType::BRK:
        if (token.brk.nobreak) {
          if (total < sizes[i]) {
            sizes[i] = total;
            total += token.brk.num_spaces;
          } else {
            total = sizes[i];
          }
        } else {
          total = 0;
        }
        break;
      case TokenType::STR:
        total += token.str.size();
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

      case TokenType::BRK:
        total += token.brk.num_spaces;
        if (!token.brk.nobreak) {
          if (total > sizes[i]) {
            sizes[i] = total;
          } else {
            total = 0;
          }
        }
        break;
      case TokenType::STR:
        total += token.str.size();
        break;
      default:
        assert(false);  // unreachable
        break;
    }
  }
}

class Output {
 public:
  Output(size_t line_width, size_t approx_output_length)
      : line_width_(line_width), remaining_(line_width) {
    buffer_.reserve(approx_output_length);
  }

  void Append(std::string_view str) { buffer_.append(str); }

  void AppendWithSpaceUpdate(std::string_view str) {
    buffer_ += str;
    remaining_ -= str.size();
  }

  void IndentWithSpaceUpdate(size_t num_spaces) {
    for (size_t i = 0; i < num_spaces; i++) {
      buffer_ += ' ';
    }
    remaining_ -= num_spaces;
  }

  size_t CurrentIndent() { return line_width_ - remaining_; }

  size_t LineWidth() { return line_width_; }

  size_t Remaining() { return remaining_; }

  bool FitsInCurrentLine(size_t size) { return size <= remaining_; }

  std::string Get() { return buffer_; }

  void SetOffsetAndLineBreak(size_t offset) {
    remaining_ = offset;
    buffer_ += '\n';
    size_t ci = CurrentIndent();
    for (size_t i = 0; i < ci; i++) {
      buffer_ += ' ';
    }
  }

 private:
  size_t line_width_;
  size_t remaining_;
  std::string buffer_;
};

struct Entry {
  size_t offset;
  BreakType break_type;
};

void Render(const std::vector<Token>& tokens, const std::vector<ssize_t>& sizes,
            Output* output) {
  std::vector<Entry> stack;
  for (size_t i = 0; i < tokens.size(); i++) {
    const Token& token = tokens[i];
    const size_t size = sizes[i];
    switch (token.type) {
      case TokenType::BEG: {
        Entry entry;
        if (token.beg.break_type == BreakType::FORCE_LINE_BREAK) {
          size_t offset;
          if (!stack.empty()) {
            offset = stack.back().offset;
            output->SetOffsetAndLineBreak(offset);
          } else {
            offset = output->LineWidth();
          }
        } else if (output->FitsInCurrentLine(size)) {
          entry = {0, BreakType::FITS};
        } else {
          entry = {
              output->Remaining(),
              token.beg.break_type == BreakType::CONSISTENT
                  ? BreakType::CONSISTENT
                  : BreakType::INCONSISTENT,
          };
        }
        stack.push_back(entry);
      } break;
      case TokenType::END:
        stack.pop_back();
        break;
      case TokenType::BRK: {
        const Entry& top = stack.back();
        BreakType break_type = top.break_type;
        size_t offset = top.offset;
        stack.pop_back();
        if ((token.brk.nobreak && output->FitsInCurrentLine(size)) ||
            break_type == BreakType::FITS) {
          output->IndentWithSpaceUpdate(token.brk.num_spaces);
        } else if (top.break_type == BreakType::CONSISTENT ||
                   top.break_type == BreakType::FORCE_LINE_BREAK) {
          output->SetOffsetAndLineBreak(offset - token.brk.offset);
        } else if (top.break_type == BreakType::INCONSISTENT) {
          if (token.brk.nobreak && output->FitsInCurrentLine(size)) {
            output->IndentWithSpaceUpdate(token.brk.num_spaces);
          } else {
            output->SetOffsetAndLineBreak(offset - token.brk.offset);
          }
        }
      } break;

      case TokenType::STR:
        output->AppendWithSpaceUpdate(token.str);
        break;
      case TokenType::INVALID:
        assert(false);  // unreachable
        break;
    }
  }
}

size_t ApproxOutputLength(const std::vector<Token>& tokens) {
  size_t total = 0;
  for (const Token& token : tokens) {
    switch (token.type) {
      case TokenType::BEG:
      case TokenType::END:
        break;
      case TokenType::STR:
        total += token.str.size();
        break;
      case TokenType::BRK:
        total += token.brk.num_spaces;
        break;
      default:
        assert(false);  // unreachable
        break;
    }
  }
  return total;
}

std::string PrettyPrint(const std::vector<Token>& tokens, size_t line_width) {
  std::vector<ssize_t> sizes = ComputeSizes(tokens);
  UpdatesSizesForNoBreaks(tokens, sizes);
  Output output(line_width, ApproxOutputLength(tokens));
  Render(tokens, sizes, &output);
  return output.Get();
}

}  // namespace PP
