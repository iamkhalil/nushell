#include "nushell.h"

static void set_file_descriptor(context_t *ctx, int argc, char **argv)
{
	ctx->fd = STDIN_FILENO;
	if (argc > 1) {
		ctx->is_interactive = 0;
		if ((ctx->fd = open(argv[1], O_RDONLY)) == -1) {
			dprintf(STDERR_FILENO, "Error: Can't open file %s\n", argv[1]);
			ctx->exit_status = errno;
			ctx->exit_loop = 1;
		}
	}
}

void nu_init(context_t *ctx, int argc, char **argv)
{
	ALLOC(ctx->lineptr, LINE_BUFFER_CAPACITY, ctx);
	ctx->line_size = 0;
	ctx->line_capacity= LINE_BUFFER_CAPACITY;
	ctx->exit_status = 0;
	ctx->exit_loop = 0;
	ctx->is_interactive = isatty(STDIN_FILENO);
	set_file_descriptor(ctx, argc, argv);
	ctx->env_size = 0;
	ctx->env_capacity = ENV_BUFFER_CAPACITY;
	env_build(ctx, environ);
}

void nu_free(context_t *ctx)
{
	FREE(ctx->lineptr);
	free_each(ctx->envp);
	if (ctx->fd > 2)
		close(ctx->fd);
}