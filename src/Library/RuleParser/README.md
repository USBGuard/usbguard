# How to generate the lexer and parser from their definitions

The lexer and parser for the USBGuard rule language is generated from a lexer and parser definition.

## Lexer

The lexer is generated using [Quex](http://quex.sourceforge.net/). The lexer definition file is located in src/Library/RuleParser/Lexer.qx.
If your distribution doesn't ship Quex as an installable package, you'll have to download it from the Quex website.

## Parser

The parser is generated using the [lemon parser generator](http://www.hwaci.com/sw/lemon/). To be able to generate
the parser, you'll need two files. The source code of [the generator](http://www.sqlite.org/src/artifact?ci=trunk&filename=tool/lemon.c),
and the [parser template](http://www.sqlite.org/src/artifact?ci=trunk&filename=tool/lempar.c) -- `lempar.c`.
To compile the parser:

> $ gcc -O2 -Wall -o lemon lemon.c

## Generating the lexer and parser

Assuming that you have the lemon parser generator (along with the parser template) and the Quex lexer generator installed,
these are commands needed to generate the C/C++ source files of the rule language lexer and parser:

> cd src/Library/RuleParser/
> lemon T=/path/to/lempar.c Parser.y
> quex --source-package . --foreign-token-id-file Parser.h --token-id-prefix RULE_TOKEN_ -i Lexer.qx -o Lexer --fes pp
