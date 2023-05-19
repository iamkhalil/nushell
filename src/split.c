#include "utils.h"

static int isdelim(char c, const char *delim)
{
	while (*delim) {
		if (c == *delim)
			return 1;
		++delim;
	}
	return 0;
}

static size_t count_tokens(const char *s, const char *delim)
{
	size_t ntokens = 0;

	while (*s) {
		while (*s && isdelim(*s, delim))
			++s;
		if (*s == '\0')
			break;
		while (*s && !isdelim(*s, delim))
			++s;
		++ntokens;
	}
	return ntokens;
}

static size_t length_token(const char *s, const char *delim)
{
	size_t len;

	for (len = 0; *s && !isdelim(*s, delim); ++s, ++len)
		;
	return len;
}

static void free_backward(char **arr, size_t idx)
{
	if (!arr)
		return;
	while (idx--)
		free(*(arr + idx));
	free(arr);
	arr = NULL;
}

char **split(const char *s, const char *delim)
{
	char **arr;
	size_t ntokens, i, j;

	if (!s)
		return NULL;
	if (!delim)
		delim = " ";

	ntokens = count_tokens(s, delim);
	arr = malloc(sizeof(char *) * (ntokens + 1)); /* +1 for NULL */
	if (!arr)
		return NULL;

	arr[ntokens] = NULL;
	i = 0;
	for (; *s; ++s) {
		if (isdelim(*s, delim))
			continue;
		arr[i] = malloc(length_token(s, delim) + 1); /* +1 for '\0' */
		if (!arr[i]) {
			free_backward(arr, i);
			return NULL;
		}
		j = 0;
		while (*s && !isdelim(*s, delim))
			arr[i][j++] = *s++;
		arr[i++][j] = '\0';
		--s;
	}
	return arr;
}
