#ifndef __DOC_H
#define __DOC_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "doctree.h"

typedef struct {
	Node **roots;
	int numroots;
	FILE *file;
} Doc;

Doc *new_doc();
Node *doc_new_root(Doc *doc, Node *tmp);

#endif // __DOC_H
