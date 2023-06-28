#!/bin/python3

import sys
import os

NOTES_TYPE = "notes"
HOMEWORK_TYPE = "homework"

FILES = {
    "article": "../templates/article.template",
    "research": "../templates/research.template",
    "homework": "../templates/homework.template",
    "notes": "../templates/notes.template",
}

COMPILER = None
DIRECTORY = None
FILENAME = None
TEMPLATE_FILE = None

def validate():
    global COMPILER, DIRECTORY, FILENAME, TEMPLATE_FILE
    error_msgs = [
        '(ERR) Compiler is not specified',
        '(ERR) Filepath is not specified',
        '(ERR) Template is not specified',
        '(ERR) Invalid file extension. File extension should be .tex'
    ]

    isvalid = True

    if COMPILER is None:
        print(error_msgs[0])
        isvalid = False
    if DIRECTORY is None:
        print(error_msgs[1])
        isvalid = False
    if TEMPLATE_FILE is None:
        print(error_msgs[2])
        isvalid = False

    if FILENAME is not None:
        parts = FILENAME.split('.')
        if len(parts) == 1 or parts[-1] != 'tex':
            print(error_msgs[3])
            isvalid = False
    else:
        isvalid = False

    if not isvalid:
        usage()

def usage(msg=None):
    if msg is not None:
        print("(ERR)", msg)
    print("(USAGE) python3 main.py template=<template> filepath=~/path/to/destination/main.tex compiler=<compiler>")
    print("(HELP) Use the --help flag for descriptions")
    exit(1)

def help():
    print("(HELP) Flags can be specified in any order")
    print("(HELP) template=<template>")
    print("(HELP)   `article`")
    print("(HELP)     - Creates a template in `article` format")
    print("(HELP)   `research`")
    print("(HELP)     - Creates a template in `research` format")
    print("(HELP)   `homework`")
    print("(HELP)     - Creates a template in `homework` format")
    print("(HELP)   `notes`")
    print("(HELP)     - Creates a template in `notes` format")
    print("(HELP) filepath=<path/to/destination/name.tex>")
    print("(HELP)   - Provide a path to create the files")
    print("(HELP)   - If the directory does not exist, it will create it")
    print("(HELP)   - The filename must end in `.tex`")
    print("(HELP) compiler=<compiler>")
    print("(HELP)   - Specify the compiler that the Makefile will use")
    print("(HELP)   - An example one is `pdflatex`")

def set_compiler(value):
    global COMPILER
    COMPILER = value
    print(f"Set compiler to {value}")

def set_filepath(location):
    global DIRECTORY, FILENAME
    DIRECTORY, FILENAME = os.path.split(location)
    print(f"Set directory to {DIRECTORY}")
    print(f"Set filename to {FILENAME}")
    print(f"Set filepath to {location}")

def set_template(value):
    global FILES, TEMPLATE_FILE
    try:
        TEMPLATE_FILE = open(FILES[value], "r")
    except:
        print(f"(ERR) Invalid template {value}")
        return
    print(f"Set template to {value}")

def create_dir(directory, filepath, content):
    try:
        os.makedirs(directory, exist_ok=True)
        with open(filepath, "w") as f:
            f.write(content)
            print(f"{filepath} created successfully")
    except OSError as e:
        print("(ERR) An error occurred while creating the directory:", str(e))
    except Exception as e:
        print("(ERR) An error occurred while creating the Makefile:", str(e))

def create_makefile(directory, filename, compiler):
    filepath = os.path.join(directory, "Makefile")
    content = filename.split('.')[0] + ': ' + filename + '\n\t' + compiler + ' ' + filename
    create_dir(directory, filepath, content)

def create_template(directory, filename, template_file):
    filepath = os.path.join(directory, filename)
    content = template_file.read()
    create_dir(directory, filepath, content)

set_flag = {
    "compiler": set_compiler,
    "filepath": set_filepath,
    "template": set_template,
}

argv = sys.argv[1:]
argc = len(sys.argv) - 1

if argc == 0:
    usage()

if argv[0] == "--help":
    help()
    exit(1)

for i in range(0, argc):
    try:
        flag, value = argv[i].split('=')
        set_flag[flag](value)
    except:
        usage(f"Invalid flag: {argv[i]}")

validate()

create_makefile(DIRECTORY, FILENAME, COMPILER)
create_template(DIRECTORY, FILENAME, TEMPLATE_FILE)
print("Finished template creation")
