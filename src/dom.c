#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "dom.h"
#include "utils.h"

TAG_ATTR*
tag_attr_create(const char *name, const char *value)
{
	TAG_ATTR *attr = (TAG_ATTR*)malloc(sizeof(TAG_ATTR));
	assert( attr );

	attr->name = (char*)malloc(sizeof(char)*strlen(name)+1); 
	attr->value = (char*)malloc(sizeof(char)*strlen(value)+1);
	attr->index = 0;
	attr->last = 0;

	strcpy(attr->name, name);
	strcpy(attr->value, value);
	
	return attr;
}

void
tag_attr_destroy(TAG_ATTR **attr)
{
	if( !*attr )
		return;

	safe_free((*attr)->name);
	safe_free((*attr)->value);
	safe_free(*attr);
}

TAG_ATTRLIST
tag_attrlist_create()
{
	TAG_ATTRLIST attrlist = (TAG_ATTRLIST)malloc(sizeof(TAG_ATTR*));
	*attrlist = NULL;
	
	return attrlist;
}

TAG_ATTR*
tag_attrlist_append(TAG_ATTRLIST *list, TAG_ATTR *attr)
{
	TAG_ATTR *lastattr = NULL;
	if( list )
		lastattr = **list;

	if( !lastattr )
		lastattr = **list = attr;
	else
	{
		int index = lastattr->last + 1;
		*list = (TAG_ATTRLIST)realloc(*list, sizeof(TAG_ATTR*)*(index+1));

		(*list)[index] = attr;
		(*list)[index]->index = index;

		lastattr = (*list)[index];
		(**list)->last = index;
	}

	return lastattr;
}

void
tag_attrlist_destroy(TAG_ATTRLIST *list)
{
	TAG_ATTR *attr = NULL;
	int i = 0;

	for( ; i<(**list)->last, attr; i++ )	
	{
		attr = (*list)[i];
		tag_attr_destroy(&attr);
	}

	safe_free(*list);
}

TAG*
tag_create(const char *name, TAG_ATTRLIST attr_list)
{
	TAG *tag = (TAG*)malloc(sizeof(TAG));
	tag->name = (char *)malloc(sizeof(char)*strlen(name));
	tag->text = NULL;
	tag->attr_list = attr_list;
	tag->children = NULL;
	tag->children_count = 0;

	strcpy(tag->name, name); 

	return tag;
}

TAG*
tag_child_append(TAG *parent, TAG *child)
{
	if( !parent->children )
		parent->children = (TAG**)malloc(sizeof(TAG*));
	else
		parent->children = (TAG**)realloc(parent->children, sizeof(TAG*)*parent->children_count+1);

	parent->children[parent->children_count] = child;
	parent->children_count += 1;

	return child;

}

void
tag_text_set(TAG *tag, const char *text)
{
	if( tag->text )
		free(tag->text);

	tag->text = (char*)malloc(sizeof(char)*strlen(text)+1);
	strcpy(tag->text, text);
}

void
tag_destroy(TAG **tag)
{
	safe_free((*tag)->name);
	safe_free((*tag)->text);
	if( (*tag)->attr_list )
		tag_attrlist_destroy(&(*tag)->attr_list);

	if( (*tag)->children_count > 0 )
	{
		TAG *child = NULL;
		int i = 0;

		for( ; i<(*tag)->children_count; i++ )
		{
			child = (*tag)->children[i];
			tag_destroy(&child);
		}
	}

	if(*tag)
		safe_free(*tag);
}

char*
_tag_to_string(TAG *tag, char c, int depth, int width)
{
	char *str = NULL, *tmp = NULL;
	char
		*pad1 = string_pad(c, depth, width),
		*pad2 = string_pad(c, depth-1, width);
	
	TAG_ATTR *attr = NULL;
	TAG *child = NULL;

	size_t attr_len = 0;
	int i = 0,
		attr_count = (tag->attr_list ? tag->attr_list[0]->last + 1: 0),
		children_count = tag->children_count;
	
	string_append(&str, "<%s", tag->name);

	if( attr_count > 0 )
	for( i=0; i<attr_count; i++ )
	{
		attr = tag->attr_list[i];
		string_append(&str, " %s=\"%s\"", attr->name, attr->value);
	}

	if( children_count == 0 && !tag->text )
		string_append(&str, " />\n");

	else
	{
		string_append(&str, ">");
		if( !tag->text )
			string_append(&str, "\r\n");

		if( children_count > 0 )
		for( i=0; i<children_count; i++ )
		{
			child = tag->children[i];
			tmp = _tag_to_string(child, c, depth+1, width);
			string_append(&str, "%s%s", pad1, tmp); 

			free(tmp);
		}

		if( tag->text )
			string_append(&str, tag->text);

		else if( depth > 1 )
			string_append(&str, pad2);

		string_append(&str, "</%s>\n", tag->name);
	}

	if( pad1 ) free(pad1);
	if( pad2 ) free(pad2);

	return str;	
}

char*
tag_to_string(TAG *tag)
{
	return _tag_to_string(tag, ' ', 1, 4);
}
