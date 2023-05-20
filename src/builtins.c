#include "nushell.h"

void (*get_builtin_func(char *prog))(context_t *, char **)
{
	static const builtin_t map[] = {
		{"exit", builtin_exit},
		{"env", builtin_env},
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
		ctx->exit_status = (unsigned) _atoi(command[1]);
	ctx->exit_loop = true;
	puts("exit");
}

void builtin_env(context_t *ctx, char **command)
{
	size_t i;
	(void)command;

	for (i = 0; ctx->envp[i]; ++i)
		puts(ctx->envp[i]);
}
