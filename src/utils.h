#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/* macros */
#define FREE(ptr) \
	do { \
		free(ptr); \
		ptr = NULL; \
	} while (0)

#define ALLOC(ptr, size, ctx) \
	do { \
		ptr = _calloc(size, sizeof(ptr[0])); \
		if (!ptr) { \
			nu_free(ctx); \
			panic("Failed to allocate memory at %s, line %d", __FILE__, __LINE__); \
		} \
	} while (0)

#define RESET(ptr) \
	do { \
		size_t i; \
		for (i = 0; ptr[i]; ++i) \
			ptr[i] = '\0'; \
	} while (0)

/* function prototypes */
void panic(const char *fmt, ...);

size_t _strlen(const char *s);
int _strcmp(const char *s1, const char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);
char *_strchr(const char *s, char c);
char *_strstr(const char *haystack, const char *needle);
char *_strcat(char *dest, const char *src);
char *_strncat(char *dest, const char *src, size_t n);
char *_strcpy(char *dest, const char *src);
char *_strdup(const char *s);
char **split(const char *s, const char *delim);

void free_each(char **ptr);
void *_calloc(size_t nmemb, size_t size);
void *_realloc(void *ptr, size_t old_size, size_t new_size);

int _atoi(const char *s);

#endif /* UTILS_H */
