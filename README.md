# PrettyPrinter
Pretty printer based on
```
Pretty Printing
Derek C. Oppen, 1979
[PDF](https://www.cs.tufts.edu/~nr/cs257/archive/derek-oppen/prettyprinting.pdf)
```

with some enhancements.

Currently, implementation are available in
* Python3 (the reference implenentation) - see [tests.py] for usage
* C++ (20) - see [tests.cc] for usage

PrettyPrinter is used by [Cwerg](cwerg.org) for formatting
Sexpr- and Python-style syntaxes.

For more recent developments in the field
of Pretty Printing this paper has a good historical overview:

```
The Final Pretty Printer
David Christiansen, David Darais, Weixi Ma
[PDF](https://davidchristiansen.dk/drafts/final-pretty-printer-draft.pdf)
```

Like most recent papers it is using a very functional style and may
be less accessible.
