#include "nushell.h"

int main(void)
{
	context_t ctx;

	nu_init(&ctx);
	while (!ctx.done) {
		if (ctx.is_interactive)
			prompt_display();
		if (getline(&ctx.lineptr, &ctx.line_len, stdin) == -1) {
			nu_free(&ctx);
			if (feof(stdin))
				exit(EXIT_SUCCESS);
			exit(EXIT_FAILURE);
		}
	}
	nu_free(&ctx);
	return ctx.exit_status;
}
