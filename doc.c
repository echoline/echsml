#include "doc.h"

Doc *new_doc(FILE *docf) {
	if (docf == NULL) {
		fprintf(stderr, "docf == NULL\n");
		return NULL;
	}

	Doc *ret = (Doc*)calloc(1, sizeof(Doc));
	if (!ret) {
		perror("malloc");
		return NULL;
	}
	ret->file = docf;
	if (doc_parse(ret)) {
		perror("doc_parse");
	}
	return ret;
}

Node *doc_new_root(Doc *doc, Node *tmp) {
	doc->roots[doc->numroots] = tmp;
	doc->numroots++;
	doc->roots = (Node**)realloc(doc->roots,
			sizeof(Node*) * (doc->numroots + 1));
	doc->roots[doc->numroots] = NULL;
	return doc->roots[0];
}

Node *new_node(char *n) {
	Node *ret = (Node*)calloc(1, sizeof(Node));
	ret->name = strdup(n);
	ret->numchildren = 0;
	ret->children = (Child**)calloc(1, sizeof(Child*));
	ret->numattribs = 0;
	ret->attribs = (Attrib**)calloc(1, sizeof(Attrib*));
	return ret;
}

Node *node_add_child(Node *parent, Node *child, char single) {
	if (!parent || !child)
		return NULL;

	child->single = single;

	parent->children[parent->numchildren] = (Child*)
						calloc(1, sizeof(Child));
	parent->children[parent->numchildren]->u.n = child;
	parent->children[parent->numchildren]->type = NODE;
	parent->numchildren++;
	parent->children = realloc(parent->children,
				(parent->numchildren + 1) * sizeof(Child*));
	parent->children[parent->numchildren] = NULL;

	child->parent = parent;
	return child;
}

Node *node_add_attrib(Node *n, char *name, char *value) {
	if (!n || !name || !value)
		return n;

	n->attribs[n->numattribs] = (Attrib*)calloc(1, sizeof(Attrib*));
	n->attribs[n->numattribs]->name = name;
	n->attribs[n->numattribs]->value = value;
	n->numattribs++;
	n->attribs = realloc(n->attribs, (n->numattribs + 1) * sizeof(Attrib*));
	n->attribs[n->numattribs] = NULL;

	return n;
}

Node *node_add_text(Node *parent, char *txt) {
	if (!parent || !txt || !strlen(txt))
		return parent;

	parent->children[parent->numchildren] = (Child*)
						calloc(1, sizeof(Child));
	parent->children[parent->numchildren]->u.c = strdup(txt);
	parent->children[parent->numchildren]->type = CSTR;
	parent->numchildren++;
	parent->children = realloc(parent->children,
				(parent->numchildren + 1) * sizeof(Child*));
	parent->children[parent->numchildren] = NULL;

	return parent;
}

void printnode(Node *n) {
	int i;
	if (n->single) {
		printf("<%s", n->name);
		for (i = 0; n->attribs[i]; i++) {
			printf(" %s=%s", n->attribs[i]->name, n->attribs[i]->value);
		}
		if (n->single == 1)
			printf("/");
		printf(">");
		return;
	}
	printf("<%s", n->name);
	for (i = 0; n->attribs[i]; i++) {
		printf(" %s=%s", n->attribs[i]->name, n->attribs[i]->value);
	}
	printf(">");
	for (i = 0; n->children[i]; i++) {
		if (n->children[i]->type == NODE)
			printnode(n->children[i]->u.n);
		else if (n->children[i]->type == CSTR)
			printf("%s", n->children[i]->u.c);
		else
			exit(-1);
	}
	printf("</%s>", n->name);
}

void printdoc(Doc *doc) {
	int j;
	if (!doc)
		return;

	for (j = 0; doc->roots[j]; j++) {
		printnode(doc->roots[j]);
	}
	printf("\n");
}
