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

void attrib_parse(Node *n, char *s) {
	if (!n || !s || !strlen(s))
		return;

	char *p = s;
	char *name;
	char *value;
	while (*p) {
		p += strspn(p, "\n\t ");
		if (!strchr(p, '='))
			return;

		p[strcspn(p, "=")] = '\0';
		name = strdup(p);
		p += strlen(p) + 1;
			
		p[strcspn(p, "\n\t /")] = '\0';
		value = strdup(p);
		p += strlen(p) + 1;

		node_add_attrib(n, name, value);
	}
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
					return -1;
				}
			}
			r = cptr - stateptr;
			strncpy(buf, stateptr, r);
			buf[r] = '\0';
			buf[strcspn(buf, "\n\t /")] = '\0';
			stateptr += r;
			tmp = new_node(buf);

			cptr = buf + strlen(buf) + 1;
			attrib_parse(tmp, cptr);

			doc_new_root(doc, tmp);
			if (*(stateptr - 1) == '/')
				tmp = NULL;
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
					return -1;
				}
			}
			stateptr += 1;
			r = cptr - stateptr;
			strncpy(buf, stateptr, r);
			buf[r] = '\0';
			if (strcasecmp(buf, tmp->name)) {
				fprintf(stderr, "malformed tag: <%s></%s>\n", tmp->name, buf);
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
				return -1;
			}
		}
		r = cptr - stateptr;
		strncpy(buf, stateptr, r);
		buf[r] = '\0';
		buf[strcspn(buf, "\n\t /")] = '\0';
		stateptr += r;

		r = (*(stateptr - 1) == '/');
		tmp = node_add_child(tmp, new_node(buf), r);

		cptr = buf + strlen(buf) + 1;
		attrib_parse(tmp, cptr);

		if (r) {
			tmp = tmp->parent;
		}
	}
	return 0;
}
