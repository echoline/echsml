#ifndef __DOC_H
#define __DOC_H

#ifdef __cplusplus
namespace echsml {
	extern "C" {
#endif

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
	Attrib **attribs;
	int numattribs;
	char single;
};

Node *new_node(char *);
Node *node_add_child(Node *parent, Node *child, char single);
Node *node_add_attrib(Node *n, char *name, char *value);
Node *node_add_text(Node *parent, char *);
Doc *new_doc();
Node *doc_new_root(Doc *doc, Node *tmp);
void printnode(Node*);
void printdoc(Doc*);
int doc_parse(Doc *);

#ifdef __cplusplus
	}
}
#endif

#endif // __DOC_H
