#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define STRCMP_SUCCESS 0

const char *glbl_templates[] = {"homework", "article"};
const char *glbl_homework_template[] = {
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
  "\\title{Assignment \\#: Chapter}\n"                                                               ,
  "\\date{\\today}\n"                                                                                ,
  "\n"                                                                                               ,
  "\\begin{document}\n"                                                                              ,
  "\\maketitle\n"                                                                                    ,
  "\\end{document}\n"
};
const char *glbl_article_template[] = {
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
  "\\date{Month 20XX}\n"                                                                             ,
  "\n"                                                                                               ,
  "\\begin{document}\n"                                                                              ,
  "\\maketitle\n"                                                                                    ,
  "\\end{document}\n"
};

// Sizes related to templates.
#define GLBL_TEMPLATES_SZ 2
#define GLBL_HOMEWORK_TEMPLATE_SZ 21
#define GLBL_ARTICLE_TEMPLATE_SZ 21
#define MAKEFILE_SZ 6

// Caps.
#define PATH_CAP 64
#define NAME_CAP 64

typedef enum {
  TEMPLATETYPE_HOMEWORK = 0,
  TEMPLATETYPE_ARTICLE  = 1
} TemplateType;

typedef struct {
  TemplateType type;
  char path[PATH_CAP];
  char file[NAME_CAP];
} UserIn;

void write_files(UserIn *userin) {
  FILE *fp = fopen(userin->path, "w");

  if (!fp) {
    fprintf(stderr, "ERROR: failed to create directory %s\nReason: %s\n", userin->path, strerror(errno));
    exit(EXIT_FAILURE);
  }

   switch (userin->type) {
   case TEMPLATETYPE_HOMEWORK:
     for (int i = 0; i < GLBL_HOMEWORK_TEMPLATE_SZ; i++) {
       fprintf(fp, "%s", glbl_homework_template[i]);
     }
     break;
   case TEMPLATETYPE_ARTICLE:
     for (int i = 0; i < GLBL_ARTICLE_TEMPLATE_SZ; i++) {
       fprintf(fp, "%s", glbl_article_template[i]);
     }
     break;
   default:
     assert(false && "UNREACHABLE");
   }

  fclose(fp);

  // Create Makefile.
  size_t path_sz = strlen(userin->path);
  for (int i = path_sz-1; i >= 0; i--) {
    if (userin->path[i] == '/') {
      break;
    }
    userin->path[i] = '\0';
    path_sz -= 1;
  }

  // + 8 for Makefile and + 1 for '\0'.
  char *makefile_dir = (char *)malloc(sizeof(char) * (path_sz + 8) + 1);
  strcpy(makefile_dir, userin->path);
  strcat(makefile_dir, "Makefile");

  char *makefile_contents[] = {
    "main: ",
    userin->file,
    "\n",
    "	pdflatex ",
    userin->file,
    "\n"
  };

  fp = fopen(makefile_dir, "w");

  if (!fp) {
    fprintf(stderr, "ERROR: failed to create directory %s\nReason: %s\n", userin->path, strerror(errno));
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < MAKEFILE_SZ; i++) {
    fprintf(fp, "%s", makefile_contents[i]);
  }

  fclose(fp);
}

void parse_path(UserIn *userin, char *path) {

  memset(userin->path, '\0', sizeof(userin->path[0]) * PATH_CAP);
  memset(userin->file, '\0', sizeof(userin->file[0]) * NAME_CAP);

  size_t path_sz = strlen(path);
  size_t name_sz = 0;
  int slash = 0;

  for (int i = path_sz-1; i >= 0; i--) {
    char c = *(path + i);
    if (c == '/') {
      slash = i + 1;
      break;
    }
    userin->file[path_sz - 1 - i] = c;
    name_sz += 1;
  }

  // Reverse userin->file.
  for (int i = 0; i < name_sz / 2; i++) {
    char tmp = userin->file[i];
    userin->file[i] = userin->file[name_sz - 1 - i];
    userin->file[name_sz - 1 - i] = tmp;
  }

  memcpy(userin->path, path, sizeof(path[0]) * path_sz);
}

void usage(FILE *stream) {
  fprintf(stream, "usage: ./template <template type [article | homework]> <filepath/filename>\n");
  exit(EXIT_FAILURE);
}

bool assign_type(UserIn *userin, const char *template) {
  for (int i = 0; i < GLBL_TEMPLATES_SZ; i++) {
    if (strcmp(template, glbl_templates[i]) == STRCMP_SUCCESS) {
      userin->type = i;
      return true;
    }
  }
  return false;
}

int main(int argc, char **argv) {

  if (argc != 3) {
    usage(stderr);
  }

  UserIn userin;

  if (!assign_type(&userin, argv[1])) {
    usage(stderr);
  }

  parse_path(&userin, argv[2]);
  write_files(&userin);

  return 0;
}
