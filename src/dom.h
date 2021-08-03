#ifndef DOM_H
#define DOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define safe_free(x) \
	if( x ) \
	{ \
		free(x); \
		x = NULL; \
	}

struct TAG_ATTR
{
	int index;
	int last;
	char *name;
	char *value;
};

struct TAG;
typedef struct TAG TAG;
typedef struct TAG_ATTR TAG_ATTR;
typedef TAG_ATTR** TAG_ATTRLIST;

struct TAG
{
	char *name;
	char *text;
	TAG_ATTRLIST attr_list;
	TAG **children;
	int children_count;
};

TAG_ATTR* tag_attr_create(const char*, const char*);
void tag_attr_destroy(TAG_ATTR**);

TAG_ATTRLIST tag_attrlist_create();
TAG_ATTR* tag_attrlist_append(TAG_ATTRLIST*, TAG_ATTR*);
void tag_attrlist_destroy(TAG_ATTRLIST*);

TAG* tag_create(const char*, TAG_ATTRLIST);
TAG* tag_child_append(TAG*, TAG*);
void tag_text_set(TAG*, const char*); 
void tag_destroy(TAG**);

char* _tag_to_string(TAG*, char, int, int);
inline char* tag_to_string(TAG*);

#endif
