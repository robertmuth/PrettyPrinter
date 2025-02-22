// (c) Robert Muth - see LICENSE for more info
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "pretty.h"

using namespace PP;

void SplitAndGroup(std::vector<Token>* out, std::string_view s,
                   BreakType break_type = BreakType::INCONSISTENT) {
  out->push_back(Beg(break_type, 2));
  size_t num_non_ws = 0;
  size_t i;
  for (i = 0; i < s.size(); ++i) {
    if (s[i] == ' ') {
      if (num_non_ws > 0) {
        out->push_back(Str(s.substr(i - num_non_ws, num_non_ws)));
        out->push_back(Brk());
        num_non_ws = 0;
      }
    } else {
      num_non_ws++;
    }
  }
  assert(num_non_ws > 0);
  out->push_back(Str(s.substr(i - num_non_ws, num_non_ws)));

  out->push_back(End());
}

int Test() {
  std::vector<Token> example0 = {Beg(BreakType::FORCE_LINE_BREAK, 0),
                                 Str("procedure test(x, y: Integer);"),
                                 Brk(),
                                 Str("begin"),
                                 Beg(BreakType::FORCE_LINE_BREAK, 2),
                                 Str("x:=1;"),
                                 Brk(),
                                 Str("y:=200;"),
                                 Brk(),
                                 Str("for z:= 1 to 100 do"),
                                 Brk(),
                                 Str("begin"),
                                 Beg(BreakType::FORCE_LINE_BREAK, 2),
                                 Str("x := x + z;"),
                                 End(),
                                 Brk(),
                                 Str("end;"),
                                 Brk(),
                                 Str("y:=x;"),
                                 End(),
                                 Brk(),
                                 Str("end;"),
                                 End()};

  std::vector<Token> example6 = {{Beg(BreakType::INCONSISTENT, 6)}};
  SplitAndGroup(&example6, "cases 1 : XXXXX");
  example6.push_back(LineBreak());
  SplitAndGroup(&example6, "2 : YYYYY");
  example6.push_back(LineBreak());
  SplitAndGroup(&example6, "3 : ZZZZZ");
  example6.push_back(End());

  struct {
    size_t line_width;
    std::vector<Token> tokens;
    std::string expected;
  } test_suite[] = {{75, example6, R"(cases 1 : XXXXX
      2 : YYYYY
      3 : ZZZZZ)"},
                    {75, example0, R"(procedure test(x, y: Integer);
begin
  x:=1;
  y:=200;
  for z:= 1 to 100 do
  begin
    x := x + z;
  end;
  y:=x;
end;)"}};

  for (const auto& test : test_suite) {
    std::string result = PrettyPrint(test.tokens, test.line_width);
    std::cout << "\n"
              << "EXPECTED-START\n"
              << test.expected << "\nEXPECTED-END\n";
    if (result != test.expected) {
      std::cout << "\n"
                << "FAILED_START\n"
                << result << "\nFAILED_END\n";

      return 1;
    }
  }
  return 0;
}

int main() { return Test(); }