#include "nushell.h"

void nu_init(context_t *ctx)
{
	ctx->lineptr = NULL;
	ctx->line_len = 0;
	ctx->exit_status = 0;
	ctx->is_interactive = isatty(STDIN_FILENO);
	ctx->done = 0;
	ctx->env_buffer_size = 0;
	ctx->env_buffer_capacity = ENV_BUFFER_CAPACITY;
	env_build(ctx, environ);
}

void nu_free(context_t *ctx)
{
	FREE(ctx->lineptr);
	free_each(ctx->envp);
}
