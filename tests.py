#!/usr/bin/python3
"""
Tests and sample usage of the Pretty Printer.
"""
import pretty as PP


def SplitAndGroup(s: str,  break_type=PP.BreakType.INCONSISTENT):
    token: list[PP.Token] = [PP.String(w) for w in s.split(' ')]
    out: list[PP.Token] = [PP.Break()] * (len(token) * 2 - 1)
    out[0::2] = token
    return [PP.Begin(break_type, 2)] + out + [PP.End()]


example0 = [
    PP.Begin(PP.BreakType.FORCE_LINE_BREAK, 0),
    PP.String('procedure test(x, y: Integer);'),
    PP.Break(),
    PP.String("begin"),
    PP.Begin(PP.BreakType.FORCE_LINE_BREAK, 2),
    PP.String("x:=1;"),
    PP.Break(),
    PP.String("y:=200;"),
    PP.Break(),
    PP.String("for z:= 1 to 100 do"),
    PP.Break(),
    PP.String("begin"),
    PP.Begin(PP.BreakType.FORCE_LINE_BREAK, 2),
    PP.String("x := x + z;"),
    PP.End(),
    PP.String("end;"),
    PP.Break(),
    PP.String("y:=x;"),
    PP.End(),
    PP.String("end;"),
    PP.End()]


example1 = [
    *SplitAndGroup('begin'), PP.LineBreak(),
    *SplitAndGroup('x := f(x);'), PP.Break(),
    *SplitAndGroup('y := f(y);'), PP.Break(),
    *SplitAndGroup('z := f(z);'), PP.Break(),
    *SplitAndGroup('w := f(w);'), PP.LineBreak(),
    *SplitAndGroup('end;'),
    PP.End()
]

example2 = [
    *SplitAndGroup('begin'), PP.LineBreak(),
    PP.Begin(PP.BreakType.CONSISTENT, 0),
    *SplitAndGroup('x := f(x);'), PP.Break(),
    *SplitAndGroup('y := f(y);'), PP.Break(),
    *SplitAndGroup('z := f(z);'), PP.Break(),
    *SplitAndGroup('w := f(w);'), PP.LineBreak(),
    PP.End(),
    *SplitAndGroup('end;'),
    PP.End()

]

example3 = [
    PP.String("XXXXXXXXXX"),
    PP.Break(), PP.String("+"), PP.Break(),
    PP.String("YYYYYYYYYY"),
    PP.Break(), PP.String("+"), PP.Break(),
    PP.String("ZZZZZZZZZZ"),
    PP.End()]

example4 = [
    PP.Begin(PP.BreakType.INCONSISTENT, 2),
    PP.String("begin"), PP.Break(),
    PP.String("x"),
    PP.Break(), PP.String(":="), PP.Break(),
    PP.String("40"),
    PP.Break(), PP.String("+"), PP.Break(),
    PP.String("2"), PP.Break(),
    PP.String("end"),
    PP.End()
]

example5 = [
    PP.Begin(PP.BreakType.INCONSISTENT, 2),
    PP.String("let x = "),
    PP.Begin(PP.BreakType.INCONSISTENT, 2),
    PP.String("{[10]uint:"),
    PP.LineBreak(),
    #
    PP.String("0,"),
    PP.Break(),
    PP.String("1,"),
    PP.Break(),
    PP.String("2,"),
    PP.Break(),
    PP.String("3,"),
    PP.Break(),
    PP.String("4,"),
    PP.Break(),
    PP.String("5,"),
    PP.Break(),
    PP.String("6,"),
    #
    PP.LineBreak(),
    PP.String("# comment"),
    PP.LineBreak(),
    #
    PP.String("0,"),
    PP.Break(),
    PP.String("1,"),
    PP.Break(),
    PP.String("2,"),
    PP.Break(),
    PP.String("3,"),
    PP.Break(),
    PP.String("4,"),
    PP.Break(),
    PP.String("5,"),
    PP.Break(),
    PP.String("6,"),
    PP.Break(),
    PP.String("7,"),
    PP.Break(),
    PP.String("8"),
    #
    PP.String("}"),
    PP.End(),
    PP.End(),
]

TESTS = [
    (75,
     [
         PP.Begin(PP.BreakType.INCONSISTENT, 6),
         *SplitAndGroup('cases 1 : XXXXX'), PP.LineBreak(),
         *SplitAndGroup('2 : YYYYY'), PP.LineBreak(),
         *SplitAndGroup('3 : ZZZZZ'),
         PP.End()],
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
end;
"""),
    (75, [PP.Begin(PP.BreakType.INCONSISTENT, 2)] + example1,
     """begin
  x := f(x); y := f(y); z := f(z); w := f(w);
  end;"""),
    (24, [PP.Begin(PP.BreakType.INCONSISTENT, 2)] + example1,
     """begin
  x := f(x); y := f(y);
  z := f(z); w := f(w);
  end;"""),
    (75, [PP.Begin(PP.BreakType.CONSISTENT, 2)] + example1,
        """begin
  x := f(x);
  y := f(y);
  z := f(z);
  w := f(w);
  end;"""),
    (75, [PP.Begin(PP.BreakType.INCONSISTENT, 2), PP.String("XXXXXXX"), PP.Break(), PP.String("YYYYYYY"), PP.End()],
     "XXXXXXX YYYYYYY"),
    (75, [PP.Begin(PP.BreakType.INCONSISTENT, 2)] + example3,
     'XXXXXXXXXX + YYYYYYYYYY + ZZZZZZZZZZ'),
    (25, [PP.Begin(PP.BreakType.INCONSISTENT, 2)] + example3,
     """XXXXXXXXXX + YYYYYYYYYY +
  ZZZZZZZZZZ"""),
    (20, [PP.Begin(PP.BreakType.INCONSISTENT, 2)] + example3,
     'XXXXXXXXXX +\n  YYYYYYYYYY +\n  ZZZZZZZZZZ'),
    (20, [PP.Begin(PP.BreakType.INCONSISTENT, 4)] + example3,
     """XXXXXXXXXX +
    YYYYYYYYYY +
    ZZZZZZZZZZ"""),
    (75, example4, "begin x := 40 + 2 end"),
    (20, SplitAndGroup("locals a, b, c, d, e, f, g, h;"),
     """locals a, b, c, d,
  e, f, g, h;"""),
    (20, SplitAndGroup("locals a, b, c, d, e, f, g, h;",
                       break_type=PP.BreakType.CONSISTENT),
     """locals
  a,
  b,
  c,
  d,
  e,
  f,
  g,
  h;"""),
    (20, example5,
     """let x = {[10]uint:
          0, 1, 2,
          3, 4, 5,
          6,
          # comment
          0, 1, 2,
          3, 4, 5,
          6, 7, 8}"""),
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
