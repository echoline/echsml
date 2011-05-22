#include "doctree.h"

Node *new_node(char *n) {
	Node *ret = (Node*)calloc(1, sizeof(Node));
	ret->name = strdup(n);
	ret->numchildren = 0;
	ret->children = (Node**)calloc(1, sizeof(Node*));
	return ret;
}

Node *node_add_child(Node *parent, Node *child) {
	if (!parent || !child)
		return NULL;

	parent->children[parent->numchildren] = child;
	parent->numchildren++;
	parent->children = realloc(parent->children,
				(parent->numchildren + 1) * sizeof(Node*));
	parent->children[parent->numchildren] = NULL;

	child->parent = parent;
	return child;
}
