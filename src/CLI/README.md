# How to generate the lexer and parser from their definitions

The lexer and parser for the USBGuard CLI is generated from a lexer and parser definition.

## Lexer

The lexer is generated using [Quex](http://quex.sourceforge.net/). The lexer definition files are located at
src/CLI/CLILine\_Lexer.qx and src/CLI/CLIArgv\_Lexer.qx. If your distribution doesn't ship Quex
as an installable package, you'll have to download it from the Quex website.

## Parser

The parser is generated using the [lemon parser generator](http://www.hwaci.com/sw/lemon/). To be able to generate
the parser, you'll need two files. The source code of [the generator](http://www.sqlite.org/src/artifact?ci=trunk&filename=tool/lemon.c),
and the [parser template](http://www.sqlite.org/src/artifact?ci=trunk&filename=tool/lempar.c) -- `lempar.c`.
To compile the parser:

    $ gcc -O2 -Wall -o lemon lemon.c

## Generating the lexer and parser

Assuming that you have the lemon parser generator (along with the parser template) and the Quex lexer generator installed,
these are commands needed to generate the C/C++ source files of the rule language lexer and parser:

    $ cd src/CLI/
    $ lemon T=/path/to/lempar.c CLIParser.y
    $ quex --source-package . --foreign-token-id-file CLIParser.h --token-id-prefix CLI_TOKEN_ -i CLILine_Lexer.qx -o CLILine::Lexer --fes pp
    $ quex --source-package . --foreign-token-id-file CLIParser.h --token-id-prefix CLI_TOKEN_ -i CLIArgv_Lexer.qx -o CLIArgv::Lexer --fes pp
