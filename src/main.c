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
	history_load(&ctx);
	while (!ctx.exit_loop) {
		if (ctx.is_interactive)
			prompt_display();
		if (_getline(ctx.fd, &ctx) <= -1)
			break;
		if (_strcmp(ctx.lineptr, ""))
			history_add(&ctx, ctx.lineptr);
		if (scan(ctx.lineptr, &ctx) == NULL) {
			nu_reset(&ctx);
			continue;
		}
#if 0
		printf("getline: [%s]\n", ctx.lineptr);
		print_list(ctx.tokens);
		printf("-> %lu elements\n", list_length(ctx.tokens));
#endif
		command_run(&ctx);
		if (!ctx.exit_loop)
			nu_reset(&ctx);
	}
	history_save(&ctx);
	nu_free(&ctx);
	return ctx.exit_status;
}
