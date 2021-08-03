#include "doc.h"

#define EXPECTED(x) \
	if( !*(c - 1) ) \
	{ \
		fprintf(stderr, "%s expected\n", x); \
		exit(1); \
	}

int
doc_title_get(char *c, char **title)
{
	int i = 0;
	*title = (char *)malloc(sizeof(char)*TITLE_MAX+1);
		
	for(; *c && *c != '\n' && i < TITLE_MAX; c++, i++ )
		(*title)[i] = *c;	

	(*title)[i++] = '\0';
	return i;
}

int
doc_categories_get(char *c, char **text, char ***list)
{
	int i = 0, j = 0, k = 0;
	
	char category[CATEGORY_MAX+1];
	char *categories = (char*)malloc(sizeof(char)*CATEGORIES_TEXT_MAX+1), *cp = NULL;

	*list = (char**)malloc(sizeof(char**));	
	**list = NULL;

	for(; *c && *c != '\n' && i < CATEGORIES_TEXT_MAX; c++, i++ )
	{
		for(j=0; *c && *c != ',' && *c != '\n'; c++, j++, k++ )
		{
			category[j] = *c;
			categories[k] = *c; 
		}

		category[j++] = '\0';	
		categories[k++] = ',';

		if( !**list )
			cp = **list = (char*)malloc(sizeof(char)*strlen(category)+1);
		else
		{
			*list = (char**)realloc(*list, sizeof(char**)*i+1);
			cp = (*list)[i] = (char*)malloc(sizeof(char)*strlen(category)+1);
		}

		strcpy(cp, category);
		memset(category, 0, CATEGORY_MAX+1);
	}
	
	categories[k-1] = '\0';

	*text = (char*)malloc(sizeof(char)*strlen(categories)+1);
	strcpy(*text, categories);

	return k+1;
}

int
doc_paragraph_get(char *c, char **paragraph)
{
	int i = 0;
	char text[PARAGRAPH_MAX+1];

	for(; *c && !(*c == '\n' && *(c+1) == '\n') && i<PARAGRAPH_MAX; c++, i++)
		text[i] = *c;

	if( i == 0 )
		return 0;

	text[i+1] = '\0';
	if( *c == '\n' ) i+=2;

	*paragraph = (char *)malloc(sizeof(char)*strlen(text)+1);
	strcpy(*paragraph, text);

	return i;
}

TAG*
doc_compile(const char *text)
{
	char *title_text = NULL, *categories_text = NULL, *category = NULL, *paragraph = NULL;
	char **categories = NULL;

	int i = 0, j = 0, k = 0;
	char *tmp = NULL, *c = (char*)text;

	c += doc_title_get(c, &title_text);
	EXPECTED("categories");

	c += doc_categories_get(c, &categories_text, &categories);
	EXPECTED("paragraph");

	TAG_ATTRLIST root_attrl = tag_attrlist_create();
	tag_attrlist_append(&root_attrl, tag_attr_create("lang", "pt-br"));

	TAG *root = tag_create("html", root_attrl);
	TAG *head = tag_create("head", NULL);
	TAG *body = tag_create("body", NULL);

	/* head */
	TAG_ATTRLIST meta_charset_attrl = tag_attrlist_create();
	tag_attrlist_append(&meta_charset_attrl, tag_attr_create("charset", "UTF-8"));

	TAG_ATTRLIST meta_keywords_attrl = tag_attrlist_create();

	TAG_ATTRLIST link_css_main_attrl = tag_attrlist_create();
	tag_attrlist_append(&link_css_main_attrl, tag_attr_create("rel", "stylesheet"));
	tag_attrlist_append(&link_css_main_attrl, tag_attr_create("href", "/main.css"));

	TAG *meta_charset = tag_create("meta", meta_charset_attrl);
	TAG *meta_keywords = tag_create("meta", meta_keywords_attrl);
	TAG *link_css_main = tag_create("link", link_css_main_attrl);
	TAG *title = tag_create("title", NULL);

	tag_child_append(head, meta_charset);
	tag_child_append(head, meta_keywords);
	tag_child_append(head, link_css_main);
	tag_child_append(head, title);
	/* end of head */

	/* body */
	TAG *h2_title = tag_create("h2", NULL);
	TAG *sec_categories = tag_create("i", NULL);

	tag_child_append(body, h2_title);
	tag_child_append(body, sec_categories);
	/* end of body */
	
	tag_child_append(root, head);
	tag_child_append(root, body);

	for( i=0; category = categories[i]; i++ ) 
	{
		asprintf(&tmp, "/category/%s.html", category);

		TAG_ATTRLIST category_attrl = tag_attrlist_create();
		tag_attrlist_append(&category_attrl, tag_attr_create("href", tmp));

		TAG *a_category = tag_create("a", category_attrl);
		tag_text_set(a_category, category);
		tag_child_append(sec_categories, a_category);

		free(tmp);
	}
	
	/* paragraphs logic */
	while( (i = doc_paragraph_get(c, &paragraph)) )
	{
		TAG *p = tag_create("p", NULL);
		tag_text_set(p, paragraph);
		tag_child_append(body, p);

		c += i;
		free(paragraph);
	}
	/* end of paragraphs logic*/

	tag_text_set(title, title_text);
	tag_text_set(h2_title, title_text);
	tag_attrlist_append(&meta_keywords_attrl, tag_attr_create("name", "keywords"));
	tag_attrlist_append(&meta_keywords_attrl, tag_attr_create("content", categories_text));

	free(title_text);
	free(categories_text);

	for( i=0; category = categories[i]; i++ )
		free(category);

	free(categories);

	return root;
}
