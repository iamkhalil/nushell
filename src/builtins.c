#include "nushell.h"

void (*get_builtin_func(char *prog))(context_t *, char **)
{
	static const builtin_t map[] = {
		{"exit", builtin_exit},
		{"env", builtin_env},
		{"setenv", builtin_setenv},
		{NULL, NULL}
	};
	size_t i;

	i = 0;
	while (map[i].prog) {
		if (_strcmp(map[i].prog, prog) == 0)
			return map[i].func;
		++i;
	}
	return NULL;
}

static size_t size_array(char **arr)
{
	size_t len;

	for (len = 0; arr[len]; ++len)
		;
	return len;
}

void builtin_exit(context_t *ctx, char **command)
{
	size_t ac;

	ac = size_array(command);
	if (ac > 2) {
		fprintf(stderr, "nushell: %s: too many arguments\n", command[0]);
		ctx->exit_status = EINVAL;
		return;
	}
	if (ac == 2)
		ctx->exit_status = _atoi(command[1]);
	ctx->exit_loop = true;
	puts("exit");
}

void builtin_env(context_t *ctx, char **command)
{
	size_t i;
	(void)command;

	for (i = 0; i < ctx->env_size; ++i)
		puts(ctx->envp[i]);
}

void builtin_setenv(context_t *ctx, char **command)
{
	char *new;
	int idx;

	if (size_array(command) != 3) {
		fprintf(stderr, "Usage: setenv VARIABLE NAME\n");
		ctx->exit_status = EINVAL;
		return;
	}
	ALLOC(new, _strlen(command[1]) + _strlen(command[2]) + 2, ctx);
	_strcpy(new, command[1]);
	_strcat(new, "=");
	_strcat(new, command[2]);
	if ((idx = env_find(ctx, command[1])) == -1) {
		env_add(ctx, new);
		free(new);
	} else {
		free(ctx->envp[idx]);
		ctx->envp[idx] = new;
	}
}
