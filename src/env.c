#include "nushell.h"

void env_add(context_t *ctx, const char *env)
{
	char **new = NULL;
	size_t i;

	if (ctx->env_buffer_size >= ctx->env_buffer_capacity) {
		ctx->env_buffer_capacity += ENV_BUFFER_CAPACITY;
		ALLOC(new, ctx->env_buffer_capacity, ctx);
		for (i = 0; i < ctx->env_buffer_size; ++i)
			new[i] = ctx->envp[i];
		free(ctx->envp);
		ctx->envp = new;
	}
	ctx->envp[ctx->env_buffer_size++] = _strdup(env);
}

void env_build(context_t *ctx, char **environ)
{
	if (!ctx || !environ)
		return;
	ALLOC(ctx->envp, ctx->env_buffer_capacity, ctx);
	while (*environ)
		env_add(ctx, *environ++);
	env_add(ctx, NULL);
}
