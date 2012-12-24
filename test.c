#include "doc.h"

int main(int argc, char **argv) {
	int i;
	FILE *file;
	Doc *doc;
	for (i = 1; i < argc; i++) {
		file = fopen(argv[i], "r");
		if (!file) {
			perror(argv[i]);
			return -1;
		}
		doc = new_doc(file);
		printdoc(doc);
	}
	return 0;
}
