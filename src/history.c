#include "nushell.h"

char *history_get_file(context_t *ctx, const char *filename)
{
	char *pathname, *home;

	home = _getenv(ctx, "HOME");
	if (!home)
		return NULL;
	ALLOC(pathname, _strlen(home) + _strlen(filename) + 2, ctx);
	_strcpy(pathname, home);
	_strcat(pathname, "/");
	_strcat(pathname, filename);
	return pathname;
}

char *read_file(context_t *ctx, const char *pathname)
{
	int fd;
	char *content;
	struct stat statbuf;

	if ((fd = open(pathname, O_RDONLY)) == -1)
		return NULL;
	if (fstat(fd, &statbuf) == -1) {
		perror("fstat");
		return NULL;
	}
	ALLOC(content, statbuf.st_size + 1, ctx);
	if (read(fd, content, statbuf.st_size) == -1) {
		perror("read");
		free(content);
		return NULL;
	}
	return content;
}

void history_add(context_t *ctx, const char *line)
{
	size_t i;

	if (ctx->history_size == ctx->history_capacity) { /* TODO: freeing by chunks */
		free(ctx->history[0]);
		for (i = 1; i < ctx->history_capacity; ++i)
			ctx->history[i - 1] = ctx->history[i];
		--ctx->history_size;
	}
	ctx->history[ctx->history_size++] = _strdup(line);
}

void history_load(context_t *ctx)
{
	char *pathname, *content;
	char **lines, **linesptr;

	pathname = history_get_file(ctx, HISTORY_FILE);
	if (pathname) {
		content = read_file(ctx, pathname);
		if (content) {
			lines = linesptr = split(content, "\n");
			while (*lines)
				history_add(ctx, *lines++);
			free_each(linesptr);
			free(content);
		}
		free(pathname);
	}
}

void history_save(context_t *ctx)
{
	char *pathname;
	int fd;
	size_t i;

	if (ctx->history_size) {
		pathname = history_get_file(ctx, HISTORY_FILE);
		if (pathname) {
			fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd != -1) {
				for (i = 0; i < ctx->history_size; ++i) {
					write(fd, ctx->history[i], _strlen(ctx->history[i]));
					write(fd, "\n", 1);
				}
				close(fd);
			}
			free(pathname);
		}
	}
}
