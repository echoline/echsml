#include "doc.h"

Doc *new_doc(FILE *docf) {
	assert(docf != NULL);

	Doc *ret = (Doc*)calloc(1, sizeof(Doc));
	if (!ret) {
		perror("malloc");
		return NULL;
	}
	ret->file = docf;
	if (doc_parse(ret)) {
		perror("doc_parse");
		return NULL;
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
