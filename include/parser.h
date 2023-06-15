
#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>

typedef enum ChenierParseStatus {
	ChenierParseStatus_PARSING_OK = 0,
	ChenierParseStatus_UNKNOWN_COMMAND = 1,
	ChenierParseStatus_MISSING_VALUE = 2,
} ChenierParseStatus;

#define CHECK_ARGS(id, argc)                                                                                           \
	if (id + 1 >= argc) return ChenierParseStatus_MISSING_VALUE;

typedef ChenierParseStatus (*parse_arg_func)(int, char **, int *, void *);

typedef struct ChenierParser {
	void *user_data;
	int argc;
	char **argv;
	parse_arg_func parse;
} ChenierParser;

void chenier_parser_log(ChenierParseStatus status, char *value);
ChenierParseStatus chenier_parser_parse(ChenierParser *cp);

#endif // PARSER_H