#include "parser.h"

char *pushtxt(FILE *file, char *txt, char *stateptr) {
	int len;
	if (!stateptr) {
		return fgets(txt, BUFSIZ-1, file);
	}
	if ((stateptr - txt) > strlen(txt)) {
		fprintf(stderr, "state pointer out of bounds\n");
		return NULL;
	}
	len = strlen(stateptr) + 1;
	memmove(txt, stateptr, len);
	return fgets(&txt[len], BUFSIZ-len-1, file);
}

int doc_parse(Doc *doc) {
	char txt[BUFSIZ];
	char buf[BUFSIZ];
	char *cptr = NULL;
	char *stateptr = NULL;
	int r;
	Node *tmp = NULL;

	doc->numroots = 0;
	doc->roots = (Node**)calloc(1, sizeof(Node*) * (doc->numroots + 1));
	
	while (1) {
		if (!stateptr) {
			if (!pushtxt(doc->file, txt, stateptr))
				return 0;
		}
		if (!tmp) {
			stateptr = strstr(stateptr? stateptr: txt, "<");
			if (!stateptr)
				continue;
			stateptr += 1;
			cptr = strstr(stateptr, ">");
			if (!cptr) {
				if (!pushtxt(doc->file, txt, stateptr))
					return 0;
				cptr = strstr(stateptr, ">");
				if (!cptr) {
					return ERANGE;
				}
			}
			r = cptr - stateptr;
			strncpy(buf, stateptr, r);
			buf[r] = '\0';
			tmp = new_node(buf);
			stateptr += r;

			doc_new_root(doc, tmp);
			continue;
		}
		
		if (stateptr) {
			stateptr++;
			r = strcspn(stateptr, "<");
			strncpy(buf, stateptr, r);
			buf[r] = '\0';

			tmp = node_add_text(tmp, buf);
		}
		stateptr = strstr(stateptr? stateptr: txt, "<");
		if (!stateptr)
			continue;
		stateptr += 1;
		if (stateptr[0] == '/') {
			cptr = strstr(stateptr, ">");
			if (!cptr) {
				if (!pushtxt(doc->file, txt, stateptr))
					return 0;
				cptr = strstr(stateptr, ">");
				if (!cptr) {
					return ERANGE;
				}
			}
			stateptr += 1;
			r = cptr - stateptr;
			strncpy(buf, stateptr, r);
			buf[r] = '\0';
			if (strcasecmp(buf, tmp->name)) {
				return EILSEQ;
			}
			tmp = tmp->parent;
			stateptr += r;
			continue;
		}
		cptr = strstr(stateptr, ">");
		if (!cptr) {
			if (!pushtxt(doc->file, txt, stateptr))
				return 0;
			cptr = strstr(stateptr, ">");
			if (!cptr) {
				return ERANGE;
			}
		}
		r = cptr - stateptr;
		strncpy(buf, stateptr, r);
		buf[r] = '\0';
		stateptr += r;

		tmp = node_add_child(tmp, new_node(buf));
	}
	return 0;
}
