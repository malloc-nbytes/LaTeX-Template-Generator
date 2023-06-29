# LaTeX-Template-Generator
A simple program to create a starter LaTeX document in a given directory written in OCaml.

## Usage
A basic example is:
```
make template_gen
./template_gen template=research compiler=pdflatex filepath=~/dev/papers/research_paper.tex
```
This will create a `LaTeX` document with the `research` template as well as a Makefile with the given compiler.

The templates available consist of:
1. article
2. homework
3. research
4. notes

The flags can be specified in any order, but the flag and argument must be split by `=`.

If needed, the program can be run with the `--help` flag for more information.
```
./template_gen --help
```
