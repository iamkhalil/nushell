#include "nushell.h"

/**
 * _getline - read a line from fd
 * @fd: file descriptor
 * @ctx: pointer to a context structure
 *
 * Return: On success, 0 is returned. On error, a negative value is returned
 * that indicates an EOF was detected or an error occurred.
 */
int _getline(int fd, context_t *ctx)
{
	static char buf[LINE_BUFFER_CAPACITY];
	ssize_t rbytes;
	bool newline = false;
	char *loc;

	while (!newline && (rbytes = read(fd, buf, LINE_BUFFER_CAPACITY)) > 0) {
		if (ctx->line_size + rbytes >= ctx->line_capacity) {
			ctx->line_capacity += LINE_BUFFER_CAPACITY;
			ctx->lineptr = _realloc(ctx->lineptr,
						ctx->line_size + 1,
						ctx->line_capacity);
			if (!ctx->lineptr) {
				nu_free(ctx);
				panic("Failed to reallocate memory at %s, line %d",
						__FILE__, __LINE__);
			}
		}
		while ((loc = _strchr(buf, '\n')) != NULL) {
			*loc = (ctx->is_interactive) ? '\0' : ';' ;
			newline = true;
		}
		_strcat(ctx->lineptr, buf);
		ctx->line_size += rbytes;
	}

	if (rbytes == 0 && newline == false) {
		ctx->exit_loop = true;
		return END_OF_FILE;
	}
	if (rbytes == -1) {
		ctx->exit_loop = true;
		ctx->exit_status = errno;
	}
	return rbytes;
}
