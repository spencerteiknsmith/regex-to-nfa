# Regex to NFA
A tool for translating regex generators to equivalent Nondeterministic Finite State Automata (NFA) acceptors.

## Inspiration
For my Intro to Computational Theory course, one set of homeworks involved creating NFAs that accept the same language that a given regex generates.
This of course is tedious, and automatable.
Taking the time to write a program to do it for me was clearly easier than just doing the work manually.

## Input
An input regex is provided as a command line argument.
The tool supports regex `*` and `|` operators, parenthesis `()` for grouping, and implicit concatenation. These work as expected.

## Output
A representation of an NFA is printed to stdout in [DOT language](https://graphviz.org/doc/info/lang.html) format.
I like to use [webgraphviz](www.webgraphviz.com) for visualizing the output.
