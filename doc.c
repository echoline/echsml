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
	return ret;
}

Node *node_add_child(Node *parent, Node *child) {
	if (!parent || !child)
		return NULL;

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
