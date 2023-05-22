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
		if (scan(ctx.lineptr, &ctx) == NULL) {
			nu_reset(&ctx);
			continue;
		}
#if 1
		printf("getline: [%s]\n", ctx.lineptr);
		print_list(ctx.tokens);
		printf("-> %lu elements\n", list_length(ctx.tokens));
#endif
		command_run(&ctx);
		if (!ctx.exit_loop)
			nu_reset(&ctx);
	}
	nu_free(&ctx);
	return ctx.exit_status;
}
