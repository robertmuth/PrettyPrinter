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

std::vector<Token> MakeTokensFromString(
    std::string_view s, BreakType break_type = BreakType::INCONSISTENT) {
  std::vector<Token> out;
  SplitAndGroup(&out, s, break_type);
  return out;
}

template <typename T>
void AppendAll(T& dst, const T& src) {
  dst.insert(end(dst), begin(src), end(src));
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

  std::vector<Token> example3a{Beg(BreakType::INCONSISTENT, 2),
                               Str("XXXXXXXXXX"),
                               Brk(),
                               Str("+"),
                               Brk(),
                               Str("YYYYYYYYYY"),
                               Brk(),
                               Str("+"),
                               Brk(),
                               Str("ZZZZZZZZZZ"),
                               End()};

  std::vector<Token> example3b = {Beg(BreakType::INCONSISTENT, 4),
                                  Str("XXXXXXXXXX"),
                                  Brk(),
                                  Str("+"),
                                  Brk(),
                                  Str("YYYYYYYYYY"),
                                  Brk(),
                                  Str("+"),
                                  Brk(),
                                  Str("ZZZZZZZZZZ"),
                                  End()};

  std::vector<Token> example4 = {Beg(BreakType::INCONSISTENT, 2),
                                 Str("begin"),
                                 Brk(),
                                 Str("x"),
                                 Brk(),
                                 Str(":="),
                                 Brk(),
                                 Str("40"),
                                 Brk(),
                                 Str("+"),
                                 Brk(),
                                 Str("2"),
                                 Brk(),
                                 Str("end"),
                                 End()};

  std::vector<Token> example5 = {
      Beg(BreakType::INCONSISTENT, 2),
      Str("let x = "),
      Beg(BreakType::INCONSISTENT, 2),
      Str("{[10]uint:"),
      LineBreak(),
#
      Str("0,"),
      Brk(),
      Str("1,"),
      Brk(),
      Str("2,"),
      Brk(),
      Str("3,"),
      Brk(),
      Str("4,"),
      Brk(),
      Str("5,"),
      Brk(),
      Str("6,"),
      //
      LineBreak(),
      Str("# comment"),
      LineBreak(),
      //
      Str("0,"),
      Brk(),
      Str("1,"),
      Brk(),
      Str("2,"),
      Brk(),
      Str("3,"),
      Brk(),
      Str("4,"),
      Brk(),
      Str("5,"),
      Brk(),
      Str("6,"),
      Brk(),
      Str("7,"),
      Brk(),
      Str("8"),
      //
      Str("}"),
      End(),
      End(),
  };

  std::vector<Token> example6 = {{Beg(BreakType::INCONSISTENT, 6)}};
  SplitAndGroup(&example6, "cases 1 : XXXXX");
  example6.push_back(LineBreak());
  SplitAndGroup(&example6, "2 : YYYYY");
  example6.push_back(LineBreak());
  SplitAndGroup(&example6, "3 : ZZZZZ");
  example6.push_back(End());

  std::vector<Token> example_abcd = {Beg(BreakType::CONSISTENT, 4), Str("("),
                                     NoBreak(0), Str("a"), Brk(), Str("b"),
                                     Brk(), Str("c"), Brk(), Str("d"),
                                     // note missing break
                                     Str(")"), End()};

  std::vector<Token> example_abcd_x4 = {Beg(BreakType::CONSISTENT, 1), Str("("),
                                        NoBreak(0)};
  AppendAll(example_abcd_x4, example_abcd);
  example_abcd_x4.push_back(Brk(0));
  AppendAll(example_abcd_x4, example_abcd);
  example_abcd_x4.push_back(Brk(0));
  AppendAll(example_abcd_x4, example_abcd);
  example_abcd_x4.push_back(Brk(0));
  AppendAll(example_abcd_x4, example_abcd);
  example_abcd_x4.push_back(Str(")"));
  example_abcd_x4.push_back(End());

  std::vector<Token> example_abcdefgh = {Beg(BreakType::CONSISTENT, 1),
                                         Str("("),
                                         NoBreak(0),
                                         Beg(BreakType::CONSISTENT, 4),
                                         Str("("),
                                         NoBreak(0),
                                         Str("abcde"),
                                         NoBreak(1)};
  AppendAll(example_abcdefgh, example_abcd_x4);
  AppendAll(example_abcdefgh, std::vector<Token>{
                                  Str(")"),
                                  End(),
                                  Str(")"),
                                  Brk(1),
                                  Beg(BreakType::CONSISTENT, 4),
                                  Str("("),
                                  NoBreak(0),
                                  Str("abcdefgh"),
                                  NoBreak(1),
                              });
  AppendAll(example_abcdefgh, example_abcd_x4);
  AppendAll(example_abcdefgh,
            std::vector<Token>{Str(")"), End(), Str(")"), End()});

  std::vector<Token> example_while = {Beg(BreakType::INCONSISTENT, 0),
                                      Str("while"),
                                      Brk(),
                                      Str("True"),
                                      Brk(0),
                                      Str(":"),
                                      End()};

  std::vector<Token> example_print = {Beg(BreakType::INCONSISTENT, 0),
                                      Str("print("),
                                      Brk(0),
                                      Str("'hello world'"),
                                      Str(")"),
                                      End()};

  std::vector<Token> example_python;
  example_python.push_back(Beg(BreakType::FORCE_LINE_BREAK, 4));
  AppendAll(example_python, example_while);
  example_python.push_back(Beg(BreakType::FORCE_LINE_BREAK, 4));
  AppendAll(example_python, example_while);
  example_python.push_back(Beg(BreakType::FORCE_LINE_BREAK, 4));
  AppendAll(example_python, example_while);
  example_python.push_back(Beg(BreakType::FORCE_LINE_BREAK, 4));
  AppendAll(example_python, example_print);
  example_python.push_back(End());
  example_python.push_back(End());
  example_python.push_back(End());
  example_python.push_back(End());

  struct {
    size_t line_width;
    std::vector<Token> tokens;
    std::string expected;
  } test_suite[] = {
      {75, example6, R"(cases 1 : XXXXX
      2 : YYYYY
      3 : ZZZZZ)"},
      //
      {75, example0,
       R"(procedure test(x, y: Integer);
begin
  x:=1;
  y:=200;
  for z:= 1 to 100 do
  begin
    x := x + z;
  end;
  y:=x;
end;)"},
      //
      {75,
       {Beg(BreakType::INCONSISTENT, 2), Str("XXXXXXX"), Brk(), Str("YYYYYYY"),
        End()},
       "XXXXXXX YYYYYYY"},
      //
      {75, example3a,
       "XXXXXXXXXX + YYYYYYYYYY + "
       "ZZZZZZZZZZ"},
      {25, example3a,
       "XXXXXXXXXX + YYYYYYYYYY +\n  "
       "ZZZZZZZZZZ"},
      {20, example3a,
       "XXXXXXXXXX +\n  YYYYYYYYYY +\n  "
       "ZZZZZZZZZZ"},
      {20, example3b,
       "XXXXXXXXXX +\n    YYYYYYYYYY +\n    "
       "ZZZZZZZZZZ"},
      //
      {75, example4, "begin x := 40 + 2 end"},
      //
      {20, MakeTokensFromString("locals a, b, c, d, e, f, g, h;"),
       "locals a, b, c, d,\n  e, f, g, h;"},
      {20,
       MakeTokensFromString("locals a, b, c, d, e, f, g, h;",
                            BreakType::CONSISTENT),
       "locals\n  a,\n  b,\n  c,\n  d,\n  "
       "e,\n  f,\n  g,\n  h;"},
      //
      {20, example5, R"(let x = {[10]uint:
          0, 1, 2,
          3, 4, 5,
          6,
          # comment
          0, 1, 2,
          3, 4, 5,
          6, 7, 8})"},
      {20, example_abcd, "(a b c d)"},
      {5, example_abcd, "(a\n    b\n    c\n    d)"},
      //
      {25, example_abcdefgh, R"(((abcde ((a b c d)
         (a b c d)
         (a b c d)
         (a b c d))))
 (abcdefgh ((a b c d)
            (a b c d)
            (a b c d)
            (a b c d)))))"},
      //
      {40, example_python, R"(while True:
    while True:
        while True:
            print('hello world'))"},
  };

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