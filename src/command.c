#include "nushell.h"

static void exec_child_proc(char *pathname, char **args, context_t *ctx)
{
	pid_t pid;
	int status;

	if ((pid = fork()) == -1) {
		perror("fork");

	} else if (pid == 0) {
		if (execve(pathname, args, ctx->envp) == -1) {
			perror("execve");
			_exit(EXIT_FAILURE);
		}

	} else {
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			ctx->exit_status = WEXITSTATUS(status);
	}
}

static void command_exec(char **command, context_t *ctx)
{
	char *pathname;

	/* if (command_builtin(command, ctx)) */
	/* 	return; */

	pathname = which(ctx, command[0]);
	if (!pathname) {
		fprintf(stderr, "nushell: %s : command not found\n", command[0]);
		ctx->exit_status = errno;
		return;
	}
	exec_child_proc(pathname, command, ctx);
	FREE(pathname);
}

void command_run(context_t *ctx)
{
	listtoken_t *head;
	char **command;

	head = ctx->tokens;
	while (head) {
		if (head->token.type == TOK_COMMAND) {
			command = split(head->token.value, DELIM);
			if (!command) {
				ctx->exit_loop = true;
				ctx->exit_status = ERROR_SPLIT;
				return;
			}
			command_exec(command, ctx);
			free_each(command);
		}
		head = head->next;
	}
}
