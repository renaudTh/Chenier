#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <time.h>

#include "tester.h"

typedef enum ParseStatus {
	PARSING_OK,
	UNKNOWN_COMMAND,
	MISSING_VALUE,
} ParseStatus;

#define CHECK_ARGS(id, argc)                                                                                           \
	if (id + 1 >= argc) return MISSING_VALUE;

void tester_parse_log(ParseStatus status, char *value) {
	switch (status) {
		case MISSING_VALUE:
			printf("Missing value for option '%s'\n", value);
			break;
		case UNKNOWN_COMMAND:
			printf("error : unknown command '%s'\n", value);
			break;
		default:
			break;
	}
}
typedef struct ChenierTester {

	char *suite_name;
	char *test_name;
	CU_SuiteInfo *suites_list;
	int nb_suites;

} ChenierTester;

ParseStatus tester_parse_arg(int argc, char *argv[], int *i, ChenierTester *tester) {

	if (strcmp(argv[*i], "--suite") == 0) {
		CHECK_ARGS(*i, argc);
		*i += 1;
		tester->suite_name = argv[*i];
	} else if (strcmp(argv[*i], "--test") == 0) {
		CHECK_ARGS(*i, argc);
		*i += 1;
		tester->test_name = argv[*i];
	} else {
		return UNKNOWN_COMMAND;
	}
	return PARSING_OK;
}
ParseStatus tester_parse_all(ChenierTester *tester, int argc, char *argv[]) {

	ParseStatus ret;
	int i = 1;
	if (i + 1 > argc) return PARSING_OK;
	do {
		ret = tester_parse_arg(argc, argv, &i, tester);
		tester_parse_log(ret, argv[i]);
		i++;
	} while (i < argc && ret == PARSING_OK);
	return ret;
}

ChenierTester *chenier_tester_new(CU_SuiteInfo *suites_list) {
	ChenierTester *tester = malloc(sizeof(ChenierTester));
	tester->suite_name = NULL;
	tester->test_name = NULL;
	tester->suites_list = suites_list;
	return tester;
}

void chenier_tester_print(ChenierTester *tester) {
	printf("suite : %s\ntest : %s\n", tester->suite_name, tester->test_name);
}
void chenier_tester_list_suites(ChenierTester *tester) {

	CU_SuiteInfo *p = &tester->suites_list[0];
	while (p->pName != NULL) {
		printf("%s\n", p->pName);
		p++;
	}
}
void chenier_tester_run(ChenierTester *tester) {

	if (tester->suite_name == NULL) {
		CU_basic_run_tests();

	} else {

		CU_pSuite s = CU_get_suite(tester->suite_name);
		if (!s) {
			printf("Unable to find suite. Available suites are : \n");
			chenier_tester_list_suites(tester);
			return;
		}
		if (tester->test_name != NULL) {

			CU_pTest t = CU_get_test_by_name(tester->test_name, s);
			if (!t) {
				printf("Unable to find test. Available tests are : \n");
				return;
			}
			CU_basic_run_test(s, t);
		} else {
			CU_basic_run_suite(s);
		}
	}
	CU_get_run_summary();
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	CU_initialize_registry();
	CU_SuiteInfo suites[] = {
	    card_stack_suite,
	    r7_suite,
	    CU_SUITE_INFO_NULL,
	};
	CU_register_suites(suites);
	ChenierTester *tester = chenier_tester_new(suites);
	tester_parse_all(tester, argc, argv);
	chenier_tester_run(tester);
	CU_cleanup_registry();
	return 0;
}