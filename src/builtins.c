#include "nushell.h"

void (*get_builtin_func(char *prog))(context_t *, char **)
{
	static const builtin_t map[] = {
		{"exit", builtin_exit},
		{"env", builtin_env},
		{"setenv", builtin_setenv},
		{"unsetenv", builtin_unsetenv},
		{"cd", builtin_cd},
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
		ctx->exit_status = 1;
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
	if (size_array(command) != 3) {
		fprintf(stderr, "Usage: setenv VARIABLE NAME\n");
		ctx->exit_status = EINVAL;
		return;
	}
	env_set(ctx, command[1], command[2]);
}

void builtin_unsetenv(context_t *ctx, char **command)
{
	if (size_array(command) != 2) {
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		ctx->exit_status = EINVAL;
		return;
	}
	env_delete(ctx, command[1]);
}

void builtin_cd(context_t *ctx, char **command)
{
	size_t ac;
	char *cur_dir, *new_dir;

	ac = size_array(command);
	if (ac > 2) {
		fprintf(stderr, "nushell: cd: too many arguments\n");
		ctx->exit_status = 1;
		return;
	}

	cur_dir = nu_getcwd();
	if (ac == 2) {
		if (_strcmp(command[1], cur_dir) == 0) {
			free(cur_dir);
			return;
		} else if (_strcmp(command[1], "-") == 0) {
			new_dir = _strdup(_getenv(ctx, "OLDPWD"));
			if (!new_dir) {
				free(cur_dir);
				fprintf(stderr, "nushell: cd: OLDPWD not set\n");
				ctx->exit_status = 1;
				return;
			}
			puts(new_dir);
		} else {
			new_dir = _strdup(command[1]);
		}

	} else {
		new_dir = _strdup(_getenv(ctx, "HOME"));
	}

	if (chdir(new_dir) == -1) {
		perror("nushell: cd");
		ctx->exit_status = errno;
		free(cur_dir);
		free(new_dir);
		return;
	}

	env_set(ctx, "OLDPWD", cur_dir);
	env_set(ctx, "PWD", new_dir);
	free(cur_dir);
	free(new_dir);
}
