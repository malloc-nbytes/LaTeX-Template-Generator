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
        'Compiler is not specified',
        'Filepath is not specified',
        'Template is not specified',
        'Invalid file extension. File extension should be .tex'
    ]

    if COMPILER is None:
        usage(error_msgs[0])
    if DIRECTORY is None:
        usage(error_msgs[1])
    if TEMPLATE_FILE is None:
        usage(error_msgs[2])

    parts = FILENAME.split('.')
    if len(parts) != 2 or parts[1] != 'tex':
        usage(error_msgs[3])

def usage(msg=None):
    if msg is not None:
        print("(!)", msg)
    print("(!) Usage: python3 main.py template=<template> filepath=~/path/to/destination/main.tex compiler=<compiler>")
    print("(!) Use the --help flag for descriptions")
    exit(1)

def help():
    print("(+) template=<template>")
    print("(+)   `article`")
    print("(+)     - Creates a template in `article` format")
    print("(+)   `research`")
    print("(+)     - Creates a template in `research` format")
    print("(+)   `homework`")
    print("(+)     - Creates a template in `homework` format")
    print("(+)   `notes`")
    print("(+)     - Creates a template in `notes` format")
    print("(+) filepath=<path/to/destination/name.tex>")
    print("(+)   - Provide a path to create the files")
    print("(+)   - If the directory does not exist, it will create it")
    print("(+)   - The filename must end in `.tex`")
    print("(+) compiler=<compiler>")
    print("(+)   - Specify the compiler that the Makefile will use")
    print("(+)   - An example one is `pdflatex`")

def set_compiler(value):
    global COMPILER
    COMPILER = value
    print(f"(+) Set compiler to {value}")

def set_filepath(location):
    global DIRECTORY, FILENAME
    DIRECTORY, FILENAME = os.path.split(location)
    print(f"(+) Set filepath to {location}")

def set_template(value):
    global FILES, TEMPLATE_FILE
    try:
        TEMPLATE_FILE = open(FILES[value], "r")
    except:
        print(f"(!) Invalid template {value}")
        return
    print(f"(+) Set template to {value}")

def create_dir(directory, filepath, content, name):
    try:
        os.makedirs(directory, exist_ok=True)
        with open(filepath, "w") as f:
            f.write(content)
            print(f"(+) {name} created successfully")
    except OSError as e:
        print("(!) An error occurred while creating the directory:", str(e))
    except Exception as e:
        print("(!) An error occurred while creating the Makefile:", str(e))

def create_makefile(directory, filename, compiler):
    filepath = os.path.join(directory, "Makefile")
    content = filename.split('.')[0] + ': ' + filename + '\n\t' + compiler + ' ' + filename
    create_dir(directory, filepath, content, "Makefile")

def create_template(directory, filename, template_file):
    filepath = os.path.join(directory, filename)
    content = template_file.read()
    create_dir(directory, filepath, content, filename)

set_flag = {
    "compiler": set_compiler,
    "filepath": set_filepath,
    "template": set_template,
}

argv = sys.argv[1:]
argc = len(sys.argv) - 1

for i in range(0, argc):
    try:
        if argv[i] == "--help":
            help()
            break
        flag, value = argv[i].split('=')
        set_flag[flag](value)
    except:
        usage(f"Invalid flag: {argv[i]}")

validate()

create_makefile(DIRECTORY, FILENAME, COMPILER)
create_template(DIRECTORY, FILENAME, TEMPLATE_FILE)
