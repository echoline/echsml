#ifndef __DOC_H
#define __DOC_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node Node;

typedef struct {
	Node **roots;
	int numroots;
	FILE *file;
} Doc;

typedef struct {
	char *name;
	char *value;
} Attrib;

#define NODE 1
#define CSTR 2

typedef struct {
	union {
		Node *n;
		char *c;
	} u;
	char type;
} Child;

struct node {
	char *name;
	Node *parent;
	Child **children;
	int numchildren;
	char *innards;
	Attrib *attribs;
};

Node *new_node(char *);
Node *node_add_child(Node *parent, Node *child);
Doc *new_doc();
Node *doc_new_root(Doc *doc, Node *tmp);

#endif // __DOC_H
