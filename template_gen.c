#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEMPLATE_SZ 22
#define FILENAME_CAP 128
#define MAKEFILE_CONTENTS_SZ 6

char *get_filename_and_create_makefile(char *filepath) {
  size_t sz = strlen(filepath), slash = 0;

  int buff_idx = 0;
  static char filename[FILENAME_CAP];

  memset(filename, '\0', sizeof(filename[0]) * FILENAME_CAP);

  for (size_t i = 0; i < sz; i++) {
    if (*(filepath + i) == '/') {
      slash = i + 1;
    }
  }

  size_t backup = slash;

  for (int i = 0; i < FILENAME_CAP; i++) {
    if (slash > sz) break;
    filename[buff_idx++] = *(filepath + slash);
    *(filepath + slash) = '\0';
    slash += 1;
  }

  char new_filepath[FILENAME_CAP];
  memset(new_filepath, '\0', sizeof(new_filepath[0]) * FILENAME_CAP);

  strcat(new_filepath, filepath);
  strcat(new_filepath, "Makefile");

  FILE *fp = fopen(new_filepath, "w");

  if (!fp) {
    fprintf(stderr, "ERROR: could not create file %s\n", new_filepath);
    exit(EXIT_FAILURE);
  }

  char *makefile_contents[] = {
    "main: ",
    filename,
    "\n",
    "	pdflatex ",
    filename,
    "\n"
  };

  for (int i = 0; i < MAKEFILE_CONTENTS_SZ; i++) {
    fprintf(fp, "%s", makefile_contents[i]);
  }

  fclose(fp);

  return filename;
}

void create_tex_template(const char *filepath) {
  char *template[] = {
    "\\documentclass{article}\n"                                                                       ,
    "\n"                                                                                               ,
    "% Language setting\n"                                                                             ,
    "% Replace `english' with e.g. `spanish' to change the document language\n"                        ,
    "\n"                                                                                               ,
    "\\usepackage[english]{babel}\n"                                                                   ,
    "% Set page size and margins\n"                                                                    ,
    "% Replace `letterpaper' with `a4paper' for UK/EU standard size\n"                                 ,
    "\\usepackage[letterpaper,top=2cm,bottom=2cm,left=3cm,right=3cm,marginparwidth=1.75cm]{geometry}\n",
    "\n"                                                                                               ,
    "% Useful packages\n"                                                                              ,
    "\\usepackage{amsmath}\n"                                                                          ,
    "\\usepackage{graphicx}\n"                                                                         ,
    "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n"                                        ,
    "\n"                                                                                               ,
    "\\title{Title}\n"                                                                                 ,
    "\\author{Zachary Haskins}\n"                                                                      ,
    "\\date{Month 20XX}\n"                                                                             ,
    "\n"                                                                                               ,
    "\\begin{document}\n"                                                                              ,
    "\\maketitle\n"                                                                                    ,
    "\\end{document}\n"
  };

  FILE *fp = fopen(filepath, "w");

  if (!fp) {
    fprintf(stderr, "ERROR: could not create file %s\n", filepath);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < TEMPLATE_SZ; i++) {
    fprintf(fp, "%s", template[i]);
  }

  fclose(fp);
}

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "usage: ./template <filepath/filename>\n");
    exit(EXIT_FAILURE);
  }

  create_tex_template(argv[1]);
  get_filename_and_create_makefile(argv[1]);

  return 0;
}

