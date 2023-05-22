#include "nushell.h"

listtoken_t *scan(const char *line, context_t *ctx)
{
	char sp[256] = { '\0' }; /* TODO: we need a dynamic string */
	token_t cur = { TOK_EMPTY, "" };
	listtoken_t *head = NULL;

	if (!line || !*line)
		return NULL;

	for (; *line; ++line) {
		switch (*line) {
		case ' ':
		case '\t':
		case '\a':
		case '\r':
		case '\v':
		case '\f':
			if (cur.type == TOK_COMMAND)
				_strncat(sp, line, 1);
			break;

		case ';':
			if (cur.type == TOK_COMMAND) {
				cur.value = sp;
				add_node(&head, &cur);
				RESET(sp);
			}
			_strncat(sp, line, 1);
			cur.type = TOK_SEMICOLON;
			cur.value = sp;
			add_node(&head, &cur);
			RESET(sp);
			break;

		case '&':
			if (cur.type == TOK_COMMAND) {
				cur.value = sp;
				add_node(&head, &cur);
				RESET(sp);
			}
			if (*(line + 1) == '&') {
				++line;
				_strncat(sp, "&&", 2);
				cur.type = TOK_OP_AND;
			} else {
				_strncat(sp, line, 1);
				cur.type = TOK_BACKGROUND;
			}
			cur.value = sp;
			add_node(&head, &cur);
			RESET(sp);
			break;

		case '|':
			if (cur.type == TOK_COMMAND) {
				cur.value = sp;
				add_node(&head, &cur);
				RESET(sp);
			}
			if (*(line + 1) == '|') {
				++line;
				_strncat(sp, "||", 2);
				cur.type = TOK_OP_OR;
			} else {
				_strncat(sp, line, 1);
				cur.type = TOK_PIPE;
			}
			cur.value = sp;
			add_node(&head, &cur);
			RESET(sp);
			break;

		case '#':
			if (cur.type == TOK_COMMAND) {
				cur.value = sp;
				add_node(&head, &cur);
				RESET(sp);
			}
			_strncat(sp, line, 1);
			cur.type = TOK_COMMENT;
			cur.value = sp;
			add_node(&head, &cur);
			RESET(sp);
			break;

		default:
			if (cur.type != TOK_COMMAND)
				cur.type = TOK_COMMAND;
			if (cur.type == TOK_COMMAND)
				_strncat(sp, line, 1);
			if (*(line + 1) == '\0') {
				cur.value = sp;
				add_node(&head, &cur);
			}
			break;
		}
	}
	reverse_list(&head);
	ctx->tokens = head;
	return head;
}
