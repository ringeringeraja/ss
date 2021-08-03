#include "doc.h"

const char *test_document = 
"Este e um titulo\n"
"artigo,programacao,c,parser,fortran\n\n"
"Lorem ipsum dolor...";

int main()
{
	TAG *root = doc_compile(test_document);
	char *doc = tag_to_string(root);
	tag_destroy(&root);

	puts(doc);
	free(doc);

	return EXIT_SUCCESS;
}
