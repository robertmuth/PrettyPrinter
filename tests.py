#!/usr/bin/python3
"""
Tests and sample usage of the Pretty Printer.
"""
import pretty as PP


def SplitAndGroup(s: str,  break_type=PP.BreakType.INCONSISTENT) -> list[PP.Token]:
    token: list[PP.Token] = [PP.Str(w) for w in s.split(' ')]
    out: list[PP.Token] = [PP.Brk()] * (len(token) * 2 - 1)
    out[0::2] = token
    return [PP.Beg(break_type, 2)] + out + [PP.End()]


example0: list[PP.Token] = [
    PP.Beg(PP.BreakType.FORCE_LINE_BREAK, 0),
    PP.Str('procedure test(x, y: Integer);'),
    PP.Brk(),
    PP.Str("begin"),
    PP.Beg(PP.BreakType.FORCE_LINE_BREAK, 2),
    PP.Str("x:=1;"),
    PP.Brk(),
    PP.Str("y:=200;"),
    PP.Brk(),
    PP.Str("for z:= 1 to 100 do"),
    PP.Brk(),
    PP.Str("begin"),
    PP.Beg(PP.BreakType.FORCE_LINE_BREAK, 2),
    PP.Str("x := x + z;"),
    PP.End(),
    PP.Brk(),
    PP.Str("end;"),
    PP.Brk(),
    PP.Str("y:=x;"),
    PP.End(),
    PP.Brk(),
    PP.Str("end;"),
    PP.End()]


example1: list[PP.Token] = [
    *SplitAndGroup('begin'), PP.LineBreak(),
    *SplitAndGroup('x := f(x);'), PP.Brk(),
    *SplitAndGroup('y := f(y);'), PP.Brk(),
    *SplitAndGroup('z := f(z);'), PP.Brk(),
    *SplitAndGroup('w := f(w);'), PP.LineBreak(),
    *SplitAndGroup('end;'),
    PP.End()
]

example2: list[PP.Token] = [
    *SplitAndGroup('begin'), PP.LineBreak(),
    PP.Beg(PP.BreakType.CONSISTENT, 0),
    *SplitAndGroup('x := f(x);'), PP.Brk(),
    *SplitAndGroup('y := f(y);'), PP.Brk(),
    *SplitAndGroup('z := f(z);'), PP.Brk(),
    *SplitAndGroup('w := f(w);'), PP.LineBreak(),
    PP.End(),
    *SplitAndGroup('end;'),
    PP.End()

]

example3a: list[PP.Token] = [
    PP.Beg(PP.BreakType.INCONSISTENT, 2),
    PP.Str("XXXXXXXXXX"),
    PP.Brk(), PP.Str("+"), PP.Brk(),
    PP.Str("YYYYYYYYYY"),
    PP.Brk(), PP.Str("+"), PP.Brk(),
    PP.Str("ZZZZZZZZZZ"),
    PP.End()]

example3b: list[PP.Token] = [
    PP.Beg(PP.BreakType.INCONSISTENT, 4),
    PP.Str("XXXXXXXXXX"),
    PP.Brk(), PP.Str("+"), PP.Brk(),
    PP.Str("YYYYYYYYYY"),
    PP.Brk(), PP.Str("+"), PP.Brk(),
    PP.Str("ZZZZZZZZZZ"),
    PP.End()]

example4: list[PP.Token] = [
    PP.Beg(PP.BreakType.INCONSISTENT, 2),
    PP.Str("begin"), PP.Brk(),
    PP.Str("x"),
    PP.Brk(), PP.Str(":="), PP.Brk(),
    PP.Str("40"),
    PP.Brk(), PP.Str("+"), PP.Brk(),
    PP.Str("2"), PP.Brk(),
    PP.Str("end"),
    PP.End()
]

example5: list[PP.Token] = [
    PP.Beg(PP.BreakType.INCONSISTENT, 2),
    PP.Str("let x = "),
    PP.Beg(PP.BreakType.INCONSISTENT, 2),
    PP.Str("{[10]uint:"),
    PP.LineBreak(),
    #
    PP.Str("0,"),
    PP.Brk(),
    PP.Str("1,"),
    PP.Brk(),
    PP.Str("2,"),
    PP.Brk(),
    PP.Str("3,"),
    PP.Brk(),
    PP.Str("4,"),
    PP.Brk(),
    PP.Str("5,"),
    PP.Brk(),
    PP.Str("6,"),
    #
    PP.LineBreak(),
    PP.Str("# comment"),
    PP.LineBreak(),
    #
    PP.Str("0,"),
    PP.Brk(),
    PP.Str("1,"),
    PP.Brk(),
    PP.Str("2,"),
    PP.Brk(),
    PP.Str("3,"),
    PP.Brk(),
    PP.Str("4,"),
    PP.Brk(),
    PP.Str("5,"),
    PP.Brk(),
    PP.Str("6,"),
    PP.Brk(),
    PP.Str("7,"),
    PP.Brk(),
    PP.Str("8"),
    #
    PP.Str("}"),
    PP.End(),
    PP.End(),
]


example_abcd: list[PP.Token] = [
    PP.Beg(PP.BreakType.CONSISTENT, 4),
    PP.Str("("),
    PP.NoBreak(0),
    PP.Str("a"),
    PP.Brk(),
    PP.Str("b"),
    PP.Brk(),
    PP.Str("c"),
    PP.Brk(),
    PP.Str("d"),
    # note missing break
    PP.Str(")"),
    PP.End()
]

example_abcdefgh: list[PP.Token] = [
    PP.Beg(PP.BreakType.CONSISTENT, 1),
    PP.Str("("),
    PP.NoBreak(0),
    PP.Beg(PP.BreakType.CONSISTENT, 4),
    PP.Str("("),
    PP.NoBreak(0),
    PP.Str("abcde"),
    PP.NoBreak(1),
    PP.Beg(PP.BreakType.CONSISTENT, 1),
    PP.Str("("),
    PP.NoBreak(0),
    *example_abcd,
    PP.Brk(0),
    *example_abcd,
    PP.Brk(0),
    *example_abcd,
    PP.Brk(0),
    *example_abcd,
    PP.Str(")"),
    PP.End(),
    PP.Str(")"),
    PP.End(),
    PP.Str(")"),
    #
    PP.Brk(1),
    #
    PP.Beg(PP.BreakType.CONSISTENT, 4),
    PP.Str("("),
    PP.NoBreak(0),
    PP.Str("abcdefgh"),
    PP.NoBreak(1),
    PP.Beg(PP.BreakType.CONSISTENT, 1),
    PP.Str("("),
    PP.NoBreak(0),
    *example_abcd,
    PP.Brk(0),
    *example_abcd,
    PP.Brk(0),
    *example_abcd,
    PP.Brk(0),
    *example_abcd,
    PP.Str(")"),
    PP.End(),
    PP.Str(")"),
    PP.End(),
    PP.Str(")"),
    PP.End(),
]


example_while: list[PP.Token] = [
    PP.Beg(PP.BreakType.INCONSISTENT, 0),
    PP.Str("while"),
    PP.Brk(),
    PP.Str("True"),
    PP.Brk(0),
    PP.Str(":"),
    PP.End(),
]

example_print: list[PP.Token] = [
    PP.Beg(PP.BreakType.INCONSISTENT, 0),
    PP.Str("print("), PP.Brk(0), PP.Str("'hello world'"),
    PP.Str(")"),
    PP.End(),
]
example_python: list[PP.Token] = [
    PP.Beg(PP.BreakType.FORCE_LINE_BREAK, 0),
    *example_while,
    PP.Beg(PP.BreakType.FORCE_LINE_BREAK, 4),
    *example_while,
    PP.Beg(PP.BreakType.FORCE_LINE_BREAK, 4),
    *example_while,
    PP.Beg(PP.BreakType.FORCE_LINE_BREAK, 4),
    *example_print,
    PP.End(),
    PP.End(),
    PP.End(),
    PP.End(),
]

example6 = [
    PP.Beg(PP.BreakType.INCONSISTENT, 6),
    *SplitAndGroup('cases 1 : XXXXX'), PP.LineBreak(),
    *SplitAndGroup('2 : YYYYY'), PP.LineBreak(),
    *SplitAndGroup('3 : ZZZZZ'),
    PP.End()]

TESTS = [
    (75,
     example6,
     """cases 1 : XXXXX
      2 : YYYYY
      3 : ZZZZZ"""),
    (75, example0,
     """procedure test(x, y: Integer);
begin
  x:=1;
  y:=200;
  for z:= 1 to 100 do
  begin
    x := x + z;
  end;
  y:=x;
end;"""),
    (75, [PP.Beg(PP.BreakType.INCONSISTENT, 2)] + example1,
     """begin
  x := f(x); y := f(y); z := f(z); w := f(w);
  end;"""),
    (24, [PP.Beg(PP.BreakType.INCONSISTENT, 2)] + example1,
     """begin
  x := f(x); y := f(y);
  z := f(z); w := f(w);
  end;"""),
    (75, [PP.Beg(PP.BreakType.CONSISTENT, 2)] + example1,
        """begin
  x := f(x);
  y := f(y);
  z := f(z);
  w := f(w);
  end;"""),
    (75, [PP.Beg(PP.BreakType.INCONSISTENT, 2), PP.Str("XXXXXXX"), PP.Brk(), PP.Str("YYYYYYY"), PP.End()],
     "XXXXXXX YYYYYYY"),
    (75,  example3a,
     'XXXXXXXXXX + YYYYYYYYYY + ZZZZZZZZZZ'),
    (25,  example3a,
     "XXXXXXXXXX + YYYYYYYYYY +\n  ZZZZZZZZZZ"),
    (20, example3a,
     'XXXXXXXXXX +\n  YYYYYYYYYY +\n  ZZZZZZZZZZ'),
    (20, example3b,
     "XXXXXXXXXX +\n    YYYYYYYYYY +\n    ZZZZZZZZZZ"),
    (75, example4, "begin x := 40 + 2 end"),
    (20, SplitAndGroup("locals a, b, c, d, e, f, g, h;"),
     "locals a, b, c, d,\n  e, f, g, h;"),
    (20, SplitAndGroup("locals a, b, c, d, e, f, g, h;",
                       break_type=PP.BreakType.CONSISTENT),
     "locals\n  a,\n  b,\n  c,\n  d,\n  e,\n  f,\n  g,\n  h;"),
    (20, example5,
     """let x = {[10]uint:
          0, 1, 2,
          3, 4, 5,
          6,
          # comment
          0, 1, 2,
          3, 4, 5,
          6, 7, 8}"""),
    (20, example_abcd, "(a b c d)"),
    (5, example_abcd, """(a
    b
    c
    d)"""),
    (25, example_abcdefgh, """((abcde ((a b c d)
         (a b c d)
         (a b c d)
         (a b c d))))
 (abcdefgh ((a b c d)
            (a b c d)
            (a b c d)
            (a b c d))))"""),
    (40, example_python, """while True:
    while True:
        while True:
            print('hello world')"""),

]


def RunAllTests():
    for line_width, token, expected in TESTS:
        print()
        print(f"[width={line_width}]")
        print("EXPECTED-START")
        print(expected)
        print("EXPECTED-END")
        result = PP.PrettyPrint(token, line_width)
        if expected != result:
            print("FAILURE-START")
            print(result)
            print("FAILURE-END")
            assert False


if __name__ == "__main__":
    RunAllTests()
