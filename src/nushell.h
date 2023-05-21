#ifndef NUSHELL_H
#define NUSHELL_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#include "utils.h"

/* macros */
#define ENV_BUFFER_CAPACITY	128
#define LINE_BUFFER_CAPACITY	256
#define PATHNAME_CAPACITY	128

#define END_OF_FILE	-2
#define ERROR_SPLIT	-3

#define DELIM " \t\a\r\v\f"

/* variables */
extern char **environ;

/* enums */
typedef enum { false, true } bool;

enum token_type {
	TOK_EMPTY,
	TOK_COMMAND,
	TOK_SEMICOLON
};

/* structs */
typedef struct token_s {
	enum token_type type;
	char *value;
} token_t;

typedef struct listtoken_s {
	token_t token;
	struct listtoken_s *next;
} listtoken_t;

typedef struct context_s {
	char **envp;
	size_t env_size;
	size_t env_capacity;
	char *lineptr;
	size_t line_size;
	size_t line_capacity;
	listtoken_t *tokens;
	char **paths;
	int exit_status;
	int fd;
	unsigned int is_interactive :1;
	unsigned int exit_loop :1;
} context_t;

typedef struct builtin_s {
	char *prog;
	void (*func)(context_t *, char **);
} builtin_t;

/* nu.c */
void nu_init(context_t *ctx, int argc, char **argv);
void nu_free(context_t *ctx);
void nu_reset(context_t *ctx);

/* env.c */
void env_build(context_t *ctx, char **environ);
void env_add(context_t *ctx, const char *env);
int env_find(context_t *ctx, const char *key);
void env_delete(context_t *ctx, const char *key);
char *_getenv(const context_t *ctx, const char *key);
char *which(context_t *ctx, const char *prog);

/* prompt.c */
void prompt_display(void);

/* getline.c */
int _getline(int fd, context_t *ctx);

/* lexer.c */
listtoken_t *scan(const char *line, context_t *ctx);

/* lists.c */
void print_list(const listtoken_t *head);
size_t list_length(const listtoken_t *head);
listtoken_t *add_node_end(listtoken_t **head, const token_t *tok);
listtoken_t *add_node(listtoken_t **head, const token_t *tok);
listtoken_t *reverse_list(listtoken_t **head);
void free_list(listtoken_t **head);

/* command.c */
void command_run(context_t *ctx);
int command_builtin(context_t *ctx, char **command);

/* builtins.c */
void (*get_builtin_func(char *))(context_t *, char **);
void builtin_exit(context_t *ctx, char **command);
void builtin_env(context_t *ctx, char **command);
void builtin_setenv(context_t *ctx, char **command);
void builtin_unsetenv(context_t *ctx, char **command);

#endif /* NUSHELL_H */
