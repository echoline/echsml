#ifndef __DOCTREE_H
#define __DOCTREE_H
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *name;
	char *value;
} Attrib;

typedef struct node Node;
struct node {
	char *name;
	Node *parent;
	Node **children;
	int numchildren;
	char *innards;
	Attrib *attribs;
};

Node *new_node(char *);
Node *node_add_child(Node *parent, Node *child);

#endif // __DOCTREE_H
