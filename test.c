#include "parser.h"

void printnode(Node *n) {
	int i;
	printf("<%s>", n->name);
	for (i = 0; n->children[i]; i++) {
		printnode(n->children[i]);
	}
	printf("</%s>\n", n->name);
}

int main(int argc, char **argv) {
	int i, j;
	FILE *file;
	Doc *doc;
	for (i = 1; i < argc; i++) {
		file = fopen(argv[i], "r");
		if (!file) {
			perror(argv[i]);
			return -1;
		}
		printf("loaded %s to (Doc*)%p\n", argv[i], &doc);
		doc = new_doc(file);
		for (j = 0; doc->roots[j]; j++) {
			printnode(doc->roots[j]);
		}
	}
	return 0;
}
