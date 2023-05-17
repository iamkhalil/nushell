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
		ctx.tokens = scan(ctx.lineptr);
		if (!ctx.tokens) {
			ctx.exit_status = ERROR_LEXER;
			break;
		}
#if 1
		print_list(ctx.tokens);
		printf("-> %lu elements\n", list_length(ctx.tokens));
#endif

		if (!ctx.exit_loop)
			nu_reset(&ctx);
	}
	nu_free(&ctx);
	return ctx.exit_status;
}
