#include "nushell.h"

listtoken_t *scan(const char *line)
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
			if (cur.type == TOK_COMMAND)
				_strncat(sp, line, 1);
			break;

		case ';':
			if (cur.type == TOK_COMMAND) {
				cur.value = sp;
				add_node_end(&head, &cur);
				RESET(sp);
			}
			_strncat(sp, line, 1);
			cur.type = TOK_SEMICOLON;
			cur.value = sp;
			add_node_end(&head, &cur);
			RESET(sp);
			break;

		default:
			if (cur.type == TOK_EMPTY || cur.type == TOK_SEMICOLON)
				cur.type = TOK_COMMAND;
			if (cur.type == TOK_COMMAND)
				_strncat(sp, line, 1);
			if (*(line + 1) == '\0') {
				cur.value = sp;
				add_node_end(&head, &cur);
			}
			break;
		}
	}
	return head;
}
