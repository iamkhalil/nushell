#include "nushell.h"

void print_list(const listtoken_t *head)
{
	while (head) {
		printf("%d, %s\n", head->token.type, head->token.value);
		head = head->next;
	}
}

listtoken_t *add_node_end(listtoken_t **head, const token_t *tok)
{
	listtoken_t *tail, *current;

	if (!head || !tok)
		return NULL;

	tail = malloc(sizeof(listtoken_t));
	if (!tail)
		return NULL;
	tail->token.value = _strdup(tok->value);
	if (!tail->token.value) {
		free(tail);
		return NULL;
	}
	tail->token.type = tok->type;
	tail->next = NULL;

	if (*head == NULL) {
		*head = tail;
		return *head;
	}

	current = *head;
	while (current->next)
		current = current->next;
	current->next = tail;
	return tail;
}

void free_list(listtoken_t **head)
{
	listtoken_t *node;

	if (!head)
		return;
	while ((node = *head)) {
		*head = (*head)->next;
		free(node->token.value);
		free(node);
	}
}

size_t list_length(const listtoken_t *head)
{
	size_t nelem = 0;

	while (head) {
		++nelem;
		head = head->next;
	}
	return nelem;
}
