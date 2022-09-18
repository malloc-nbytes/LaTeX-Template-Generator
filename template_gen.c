#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEMPLATE_SZ 22

void create_tex_template(const char *filepath) {
  char *template[TEMPLATE_SZ] = {
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

  return 0;
}
