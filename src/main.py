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
    if COMPILER is None or DIRECTORY is None or FILENAME is None or TEMPLATE_FILE is None:
        usage()
    if FILENAME.split('.')[1] != 'tex':
        usage()

def usage():
    print("usage: python3 main.py template=<template> filepath=~/path/to/destination/main.tex compiler=<compiler>")
    exit(1)

def set_compiler(value):
    global COMPILER
    COMPILER = value

def set_filepath(location):
    global DIRECTORY, FILENAME
    DIRECTORY, FILENAME = os.path.split(location)

def set_template(value):
    global FILES, TEMPLATE_FILE
    TEMPLATE_FILE = open(FILES[value], "r")

def create_makefile(directory, filename, compiler):
    filepath = directory + '/' + "Makefile"
    with open(filepath, "w") as f:
        f.write(filename.split('.')[0] + ': ' + filename + '\n\t' + compiler + ' ' + filename)

def create_template(directory, filename, template_file):
    filepath = directory + '/' + filename
    with open(filepath, "w") as f:
        f.write(template_file.read())

set_flag = {
    "compiler": set_compiler,
    "filepath": set_filepath,
    "template": set_template,
}

argv = sys.argv[1:]
argc = len(sys.argv) - 1

try:
    for i in range(0, argc):
       flag, value = argv[i].split('=')
       set_flag[flag](value)
except:
    usage()
validate()

create_makefile(DIRECTORY, FILENAME, COMPILER)
create_template(DIRECTORY, FILENAME, TEMPLATE_FILE)
