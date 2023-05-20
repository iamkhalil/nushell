#include "nushell.h"

void (*get_builtin_func(char *prog))(context_t *, char **)
{
	static const builtin_t map[] = {
		{"exit", nu_exit},
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

void nu_exit(context_t *ctx, char **command)
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
