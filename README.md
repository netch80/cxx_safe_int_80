Synopsis:

A small header-only library for safe arithmetic in C++.

This variant uses operations on standard types and intended, respectively,
to replace specific operations in critical places. (This opposes to most
alternatives which provide own types.)

Requirements: GCC or Clang because relies on builtin overflows.
(Impatiently apprehending standard functions in C++23.)
For other compilers, waiting for contributions.

History: recreation from scratch what was used in an old proprietary
project, mainly for own desire not to lose the previous results.

Status: beta-before-public for existing functions. Some functions
are planned but missed yet.

License: public domain.

TODO:
-> Documentation where not obvious.
-> Finish with tests for existing functions.
-> More functions from plan.
-> Use standard overflow checkers where possible (newer standards).
-> Variation of some parts on compilation standard.
