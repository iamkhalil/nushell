#include "nushell.h"

/**
 * main - Entry point
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: On success, 0 is returned. On error, a non-zero value is returned.
 */
int main(int argc, char *argv[])
{
	context_t ctx;

	nu_init(&ctx, argc, argv);
	while (!ctx.exit_loop) {
		if (ctx.is_interactive)
			prompt_display();
		if (_getline(ctx.fd, &ctx) <= -1)
			break;

		/* Reset line */
		if (!ctx.exit_loop) {
			FREE((&ctx)->lineptr);
			ctx.line_size = 0, ctx.line_capacity = LINE_BUFFER_CAPACITY;
			ALLOC((&ctx)->lineptr, LINE_BUFFER_CAPACITY, &ctx);
		}
	}
	nu_free(&ctx);
	return ctx.exit_status;
}
