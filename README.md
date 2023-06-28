# LaTeX-Template-Generator
A simple program to create a starter LaTeX document in a given directory.

## Usage
A basic example is:
```
python3 main.py template=research compiler=pdflatex filepath=~/dev/papers/research_paper.tex
```
This will create a `LaTeX` document with the `research` template as well as a Makefile with the given compiler.

You can also just use the first letter for each flag.
```
python3 main.py t=research c=pdflatex f=~/dev/papers/research_paper.tex
```

The templates available consist of:
1. article
2. homework
3. research
4. notes

The flags can be specified in any order, but the flag and argument must be split by `=`.

If the filepath provided does not exist, then the program will try to create it for you.

If needed, the program can be run with the `--help` flag for more information.
```
python3 main.py --help
```

## Adding More Templates

To add more templates, edit this dictionary:
```py
FILES = {
    "article": "../templates/article.template",
    "research": "../templates/research.template",
    "homework": "../templates/homework.template",
    "notes": "../templates/notes.template",
}
```
Put the name on the left, and put the actual file in the `templates` directory.
```py
FILES = {
    "article": "../templates/article.template",
    "research": "../templates/research.template",
    "homework": "../templates/homework.template",
    "notes": "../templates/notes.template",
    "my_template": "../templates/my_template.template",
}
```
