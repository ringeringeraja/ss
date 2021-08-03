#ifndef DOC_H
#define DOC_H

#include "dom.h"

#define TITLE_MAX 120
#define CATEGORY_MAX 120
#define CATEGORIES_MAX 15
#define PARAGRAPH_MAX 10000
#define CATEGORIES_TEXT_MAX CATEGORY_MAX*CATEGORIES_MAX+CATEGORIES_MAX

int doc_title_get(char *, char **);
int doc_categories_get(char *, char **, char ***);
int doc_paragraph_get(char *, char **);

TAG* doc_compile(const char*);

#endif
