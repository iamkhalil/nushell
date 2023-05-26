#include "nushell.h"

void (*get_builtin_func(char *prog))(context_t *, char **)
{
	static const builtin_t map[] = {
		{"exit", builtin_exit},
		{"env", builtin_env},
		{"setenv", builtin_setenv},
		{"unsetenv", builtin_unsetenv},
		{"cd", builtin_cd},
		{"history", builtin_history},
		{"help", builtin_help},
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

	if (new_dir[0] != '/') { /* to get the absolute path when using env builtin */
		free(new_dir);
		new_dir = nu_getcwd();
	}
	env_set(ctx, "OLDPWD", cur_dir);
	env_set(ctx, "PWD", new_dir);
	free(cur_dir);
	free(new_dir);
}

void builtin_history(context_t *ctx, char **command)
{
	size_t i;

	(void)command;
	for (i = 0; i < ctx->history_size; ++i)
		printf("%4lu  %s\n", i, ctx->history[i]);
}

void builtin_help(context_t *ctx, char **command)
{
	if (size_array(command) != 2) {
		fprintf(stderr, "Usage: help BUILTIN\n");
		ctx->exit_status = EINVAL;
		return;
	}

	if (_strcmp(command[1], "exit") == 0) {
		puts("exit: exit [n]\n\tExit the shell.\n\n\tExits the shell"
			"with a status of N. If N is omitted, the exit"
			"status is that of the last command executed.\n");
	} else if (_strcmp(command[1], "env") == 0) {
		puts("env: env\n\tprint the current environement");
	} else if (_strcmp(command[1], "setenv") == 0) {
		puts("setenv: setenv [VARIABLE] [VALUE]\n\tChange or add an"
			"environment variable.\n");
	} else if (_strcmp(command[1], "unsetenv") == 0) {
		puts("unsetenv: unsetenv [VARIABLE]\n\tRemove an environment variable");
	} else if (_strcmp(command[1], "cd") == 0) {
		puts("cd: cd [- | directory]\n\tchange the working directory");
	} else if (_strcmp(command[1], "history") == 0) {
		puts("history: history\n\tDisplay the history list");
	} else if (_strcmp(command[1], "help") == 0) {
		puts("help: help [BUILTIN]\n\tDisplay information about builtin commands");
	} else {
		fprintf(stderr, "nushell: help: no help topics match '%s'\n", command[1]);
		ctx->exit_status = 1;
	}
}
