#ifndef NUSHELL_H
#define NUSHELL_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "utils.h"

#define ENV_BUFFER_CAPACITY 128

extern char **environ;

typedef struct context_s {
	char **envp;
	size_t env_buffer_size;
	size_t env_buffer_capacity;
	char *lineptr;
	size_t line_len;
	int exit_status;
	unsigned int is_interactive :1;
	unsigned int done :1;
} context_t;

/* nu.c */
void nu_init(context_t *ctx);
void nu_free(context_t *ctx);

/* env.c */
void env_build(context_t *ctx, char **environ);
void env_add(context_t *ctx, const char *env);

/* prompt.c */
void prompt_display(void);

#endif /* NUSHELL_H */
