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

#define ALLOC(ptr, size, ctx)									\
	do {											\
		ptr = _calloc(size, sizeof(ptr[0]));						\
		if (!ptr) {									\
			nu_free(ctx);								\
			panic("Failed to allocate memory at %s, line %d", __FILE__, __LINE__);	\
		}										\
	} while (0)

/* function prototypes */
size_t _strlen(const char *s);
int _strcmp(const char *s1, const char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);
char *_strchr(const char *s, char c);
char *_strstr(const char *haystack, const char *needle);
char *_strcat(char *dest, const char *src);
char *_strcpy(char *dest, const char *src);
char *_strdup(const char *s);

void panic(const char *fmt, ...);
void free_each(char **ptr);
void *_calloc(size_t nmemb, size_t size);
void *_realloc(void *ptr, size_t old_size, size_t new_size);

#endif /* UTILS_H */