#include "doc.h"

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

char stralpha (char *str) {
	char *p = str;

	while (*p) {
		if (!isalpha(*p))
			return 0;
		p++;
	}

	return 1;
}

void attrib_parse(Node *n, char *s) {
	if (!n || !s || !strlen(s))
		return;

	char *p = s;
	char *name;
	char *value;

	int r = strlen(s)-1;
	if (s[r] == '/')
		s[r] = '\0';

	while (*p) {
		p += strspn(p, "\n\t ");
		if (!strchr(p, '='))
			return;

		p[strcspn(p, "=")] = '\0';
		name = strdup(p);
		p += strlen(p) + 1;
			
		p[strcspn(p, "\n\t ")] = '\0';
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
	int s;
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
			buf[strcspn(buf, "\n\t ")] = '\0';
		
			s = (*(stateptr-1+r) == '/');
			if (s) {
				s = strlen(buf);
				if (s && (buf[s-1] == '/')) 
					buf[s-1] = '\0';
				s = 1;
	 		}

			if (!stralpha(buf)) {
				s = 2;
				r = cptr - stateptr;
				strncpy(buf, stateptr, r);
				buf[r] = '\0';
			} else {
				cptr++;
			}
			tmp = new_node(buf);
			tmp->single = s;

			stateptr += r;

			cptr = buf + strlen(buf);
			attrib_parse(tmp, cptr);

			doc_new_root(doc, tmp);
			if (s)
				tmp = NULL;
			continue;
		}
		
		if (stateptr) {
			stateptr++;
			r = strcspn(stateptr, "<");
			strncpy(buf, stateptr, r);
			buf[r] = '\0';

			tmp = node_add_text(tmp, buf);
			stateptr = strstr(stateptr, "<");
		} else {
			r = strcspn(txt, "<");
			strncpy(buf, txt, r);
			buf[r] = '\0';

			tmp = node_add_text(tmp, buf);
			stateptr = strstr(txt, "<");
		}
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
		buf[strcspn(buf, "\n\t ")] = '\0';
		stateptr += r;

		s = (*(stateptr - 1) == '/');
		if (s) {
			r = strlen(buf);
			if (r && (buf[r-1] == '/')) 
				buf[r-1] = '\0';
	 	}
		tmp = node_add_child(tmp, new_node(buf), s);

		cptr = buf + strlen(buf) + 1;
		attrib_parse(tmp, cptr);

		if (s) {
			tmp = tmp->parent;
		}
	}
	return 0;
}
