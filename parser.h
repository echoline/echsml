#ifndef __PARSER_H
#define __PARSER_H
#include "doc.h"

#ifdef __cplusplus
namespace echsml {
	extern "C" {
#endif

int doc_parse(Doc *);

#ifdef __cplusplus
	}
}
#endif

#endif // __PARSER_H
