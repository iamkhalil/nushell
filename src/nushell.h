#ifndef NUSHELL_H
#define NUSHELL_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "utils.h"

#define ENV_BUFFER_CAPACITY	128
#define LINE_BUFFER_CAPACITY	256

#define END_OF_FILE -2

extern char **environ;

typedef enum { false, true } bool;

typedef struct context_s {
	char **envp;
	size_t env_size;
	size_t env_capacity;
	char *lineptr;
	size_t line_size;
	size_t line_capacity;
	int exit_status;
	int fd;
	unsigned int is_interactive :1;
	unsigned int exit_loop :1;
} context_t;

/* nu.c */
void nu_init(context_t *ctx, int argc, char **argv);
void nu_free(context_t *ctx);

/* env.c */
void env_build(context_t *ctx, char **environ);
void env_add(context_t *ctx, const char *env);

/* prompt.c */
void prompt_display(void);

/* getline.c */
int _getline(int fd, context_t *ctx);

#endif /* NUSHELL_H */
