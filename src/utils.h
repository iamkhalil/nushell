#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/* macros */
#define FREE(ptr)		\
	do {			\
		free(ptr);	\
		ptr = NULL;	\
	} while (0)

#define ALLOC(ptr, size, ctx)							\
	do {									\
		ptr = malloc(sizeof(ptr[0]) * size);				\
		if (!ptr) {							\
			nu_free(ctx);						\
			panic("Failed to allocate memory at %s, line %d",	\
					__FILE__, __LINE__);			\
		}								\
	} while (0)

/* function prototypes */
size_t _strlen(const char *s);
int _strcmp(const char *s1, const char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);
char *_strstr(const char *haystack, const char *needle);
char *_strcat(char *dest, const char *src);
char *_strcpy(char *dest, const char *src);
char *_strdup(const char *s);

void panic(const char *fmt, ...);
void free_each(char **ptr);

#endif /* UTILS_H */
