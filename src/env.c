#include "nushell.h"

void env_add(context_t *ctx, const char *env)
{
	char **new = NULL;
	size_t i;

	if (ctx->env_size >= ctx->env_capacity) {
		ctx->env_capacity += ENV_BUFFER_CAPACITY;
		ALLOC(new, ctx->env_capacity, ctx);
		for (i = 0; i < ctx->env_size; ++i)
			new[i] = ctx->envp[i];
		free(ctx->envp);
		ctx->envp = new;
	}
	ctx->envp[ctx->env_size++] = _strdup(env);
}

void env_build(context_t *ctx, char **environ)
{
	if (!ctx || !environ)
		return;
	ALLOC(ctx->envp, ctx->env_capacity, ctx);
	while (*environ)
		env_add(ctx, *environ++);
}

int env_find(context_t *ctx, const char *key)
{
	size_t idx, len;

	len = _strlen(key);
	idx = 0;
	while (ctx->envp[idx]) {
		if (_strncmp(key, ctx->envp[idx], len) == 0)
			return idx;
		++idx;
	}
	return -1;
}

void env_delete(context_t *ctx, const char *key)
{
	int idx, i;

	if ((idx = env_find(ctx, key)) == -1)
		return;
	if (idx == (int)ctx->env_size - 1) { /* pop */
		free(ctx->envp[--ctx->env_size]);
		ctx->envp[ctx->env_size] = NULL;
		return;
	}
	free(ctx->envp[idx]);
	for (i = idx; i <= (int)ctx->env_size - 1; ++i)
		ctx->envp[i] = ctx->envp[i + 1];
	free(ctx->envp[--ctx->env_size]);
	ctx->envp[ctx->env_size] = NULL;
}

void env_set(context_t *ctx, const char *key, const char *value)
{
	char *env;
	int idx;

	ALLOC(env, _strlen(key) + _strlen(value) + 2, ctx);
	_strcpy(env, key);
	_strcat(env, "=");
	_strcat(env, value);
	if ((idx = env_find(ctx, key)) == -1) {
		env_add(ctx, env);
		free(env);
	} else {
		free(ctx->envp[idx]);
		ctx->envp[idx] = env;
	}
}

char *_getenv(const context_t *ctx, const char *key)
{
	size_t i, len;

	i = 0;
	len = _strlen(key);
	while (ctx->envp[i]) {
		if (_strncmp(key, ctx->envp[i], len) == 0)
			return &ctx->envp[i][len + 1];
		++i;
	}
	return NULL;
}

char *which(context_t *ctx, const char *prog)
{
	char *pathname = NULL;
	size_t capacity = 0, i = 0, len = _strlen(prog);
	bool found = false;

	if (!prog || !ctx)
		return NULL;

	if (prog[0] == '/' || prog[0] == '.') {
		pathname = _strdup(prog);
		if (!pathname || access(pathname, F_OK) == -1)
			return NULL;
		return pathname;
	}

	while (ctx->paths[i] && !found) {
		while (len + _strlen(ctx->paths[i]) + 2 >= capacity) {
			capacity += PATHNAME_CAPACITY;
			free(pathname);
			ALLOC(pathname, capacity, ctx);
		}
		_strcpy(pathname, ctx->paths[i]);
		_strcat(pathname, "/");
		_strcat(pathname, prog);
		if (access(pathname, F_OK) == 0)
			found = true;
		++i;
	}
	if (!found) {
		free(pathname);
		pathname = NULL;
	}
	return pathname;
}
