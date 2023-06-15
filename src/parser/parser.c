#include "parser.h"

void chenier_parser_log(ChenierParseStatus status, char *value) {
	switch (status) {
		case ChenierParseStatus_MISSING_VALUE:
			printf("Missing value for option '%s'\n", value);
			break;
		case ChenierParseStatus_UNKNOWN_COMMAND:
			printf("error : unknown command '%s'\n", value);
			break;
		default:
			break;
	}
}
ChenierParseStatus chenier_parser_parse(ChenierParser *cp) {
	ChenierParseStatus ret;
	int i = 1;
	if (i + 1 > cp->argc) return ChenierParseStatus_PARSING_OK;
	do {
		ret = cp->parse(cp->argc, cp->argv, &i, cp->user_data);
		chenier_parser_log(ret, cp->argv[i]);
		i++;
	} while (i < cp->argc && ret == ChenierParseStatus_PARSING_OK);
	return ret;
}