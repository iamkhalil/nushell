#include "utils.h"

/**
 * panic - Print an error message and exit
 * @fmt: format string
 */
void panic(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
	exit(EXIT_FAILURE);
}

/**
 * free_each - free an array of strings
 * @ptr: pointer to an array of strings
 */
void free_each(char **ptr)
{
	size_t idx;

	if (!ptr)
		return;
	for (idx = 0; ptr[idx]; ++idx)
		free(*(ptr + idx));
	free(ptr);
	ptr = NULL;
}

/**
 * _calloc - Allocate a zero-initialized array in the heap.
 * @nmemb: number of elements
 * @size: size of an element
 *
 * Return: On success, a pointer to the newly allocated array is returned.
 * NULL otherwise.
 */
void *_calloc(size_t nmemb, size_t size)
{
	void *ptr;
	size_t i;

	if (nmemb == 0 || size == 0)
		return NULL;
	ptr = malloc(size * nmemb);
	if (!ptr)
		return NULL;
	for (i = 0; i < (nmemb * size); ++i)
		((char *)ptr)[i] = 0;
	return ptr;
}

/**
 * _realloc - Reallocate a memory block.
 * @ptr: pointer to previously allocated memory area
 * @old_size: actual size of memory
 * @new_size: new size of memory
 *
 * Return: On success, a pointer to the newly allocated memory area is returned.
 * NULL otherwise.
 */
void *_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void *new;
	size_t min, i;

	if (old_size == new_size)
		return ptr;
	if (!ptr)
		return malloc(new_size);
	if (!new_size) {
		FREE(ptr);
		return NULL;
	}
	new = malloc(new_size);
	if (!new)
		return NULL;
	min = (old_size < new_size) ? old_size : new_size;
	for (i = 0; i < min; ++i)
		((char *)new)[i] = ((char *)ptr)[i];
	free(ptr);
	return new;
}

/**
 * _strlen - Calculate the length of a string
 * @s: the given string
 *
 * Return: number of bytes in the string s
 */
size_t _strlen(const char *s)
{
	const char *sp = s;

	while (*sp)
		++sp;
	return sp - s;
}

/**
 * _strcmp - Compare two strings
 * @s1: first string
 * @s2: second string
 *
 * Return: 0 if s1 and s2 are equal. non-zero otherwise
 */
int _strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2 && *s1) {
		++s1;
		++s2;
	}
	return *s1 - *s2;
}

/**
 * _strncmp - Compare at most n bytes of two strings
 * @s1: first string
 * @s2: second string
 * @n: number of bytes
 *
 * Return: 0 if s1 and s2 are equal. non-zero otherwise.
 */
int _strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return 0;
	while (*s1 == *s2 && *s1 && --n) {
		++s1;
		++s2;
	}
	return *s1 - *s2;
}

/**
 * _strchr - locate character in string
 * @s: the given string
 * @c: the given character
 *
 * Return: On success, a pointer to the first occurrence of the character c in
 * the string s is returned. NULL otherwise.
 */
char *_strchr(const char *s, char c)
{
	if (!s)
		return NULL;
	while (*s && (*s != c))
		++s;
	if (*s == c)
		return (char *)s;
	return NULL;
}

/**
 * _strstr - Find the first occurrence of the substring needle in the string haystack
 * @haystack: string to examine
 * @needle: substring to search for
 *
 * Return: On success, a pointer to the beginning of the located substring is
 * returned. NULL, if the substring is not found.
 */
char *_strstr(const char *haystack, const char *needle)
{
	unsigned int i, j;

	for (i = 0; haystack[i]; ++i) {
		j = 0;
		while (haystack[i + j] == needle[j] && needle[j])
			++j;
		if (needle[j] == '\0')
			return ((char *)&haystack[i]);
	}
	return NULL;
}

/**
 * _strcat - Concatenate two strings
 * @dest: pointer to destination string
 * @src: pointer to source string
 *
 * Return: pointer to the resulting string dest
 */
char *_strcat(char *dest, const char *src)
{
	char *sp = dest;

	while (*sp)
		++sp;
	while ((*sp++ = *src++) != '\0')
		;
	return dest;
}

/**
 * _strncat - Concatenate two strings using at most N bytes from the source string
 * @dest: pointer to destination string
 * @src: pointer to source string
 * @n: number of bytes to append
 *
 * Return: pointer to the resulting string dest
 */
char *_strncat(char *dest, const char *src, size_t n)
{
	char *sp = dest;

	if (n == 0)
		return sp;
	sp += _strlen(dest);
	while (*src && n--)
		*sp++ = *src++;
	*sp = '\0';
	return dest;
}

/**
 * _strcpy - Copy the string pointed to by src, to the buffer pointed to by dest
 * @src: pointer to the source string
 * @dest: pointer to the destination string
 *
 * Return: pointer to dest
 */
char *_strcpy(char *dest, const char *src)
{
	char *sp = dest;

	while ((*sp++ = *src++) != '\0')
		;
	return dest;
}

/**
 * _strdup - Duplicate a string
 * @s: the string to duplicate
 *
 * Return: On success, a pointer to the newly allocated string is returned.
 * On error, NULL is returned.
 */
char *_strdup(const char *s)
{
	char *sp;

	if (!s)
		return NULL;
	sp = malloc(_strlen(s) + 1);
	if (!sp)
		return NULL;
	return _strcpy(sp, s);
}
