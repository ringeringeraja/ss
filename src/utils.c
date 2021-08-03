#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

char*
string_append(char **dst, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char *tmp = NULL, *ret = NULL;
	assert( vasprintf(&tmp, fmt, args) > 0);
	va_end(args);

	if( *dst )
	{
		*dst = (char*)realloc(*dst, sizeof(char)*(strlen(*dst)+strlen(tmp))+1);
		strcat(*dst, tmp);
	}
	else
	{
		*dst = (char*)malloc(sizeof(char)*strlen(tmp)+1);
		strcpy(*dst, tmp);
	}

	free(tmp);
	return ret;
}

char*
string_pad(char c, int size, int m)
{
	char *ret = (char*)malloc(sizeof(char)*size*m+1);
	int i = 0;

	for( ; i<size*m; i++ )
		*(ret+i) = c;
	
	*(ret+i++) = '\0';
	return ret;
}
