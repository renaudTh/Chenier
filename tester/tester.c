#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <time.h>

#include "tester.h"

typedef struct ChenierTester {

	char *suite_name;
	char *test_name;
	CU_SuiteInfo *suites_list;
	int nb_suites;

} ChenierTester;

ChenierParseStatus tester_parse_arg(int argc, char *argv[], int *i, void *user_data) {
	ChenierTester *tester = (ChenierTester *)user_data;
	if (strcmp(argv[*i], "--suite") == 0) {
		CHECK_ARGS(*i, argc);
		*i += 1;
		tester->suite_name = argv[*i];
	} else if (strcmp(argv[*i], "--test") == 0) {
		CHECK_ARGS(*i, argc);
		*i += 1;
		tester->test_name = argv[*i];
	} else {
		return ChenierParseStatus_UNKNOWN_COMMAND;
	}
	return ChenierParseStatus_PARSING_OK;
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
void chenier_tester_list_tests(CU_pSuite suite) {
	if (!suite) return;
	CU_pTest test = suite->pTest;
	while (test != NULL) {
		printf("%s\n", test->pName);
		test = test->pNext;
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
				chenier_tester_list_tests(s);
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
	    klondike_suite,
	    CU_SUITE_INFO_NULL,
	};
	CU_register_suites(suites);
	ChenierTester *tester = chenier_tester_new(suites);
	ChenierParser parser = {.argc = argc, .argv = argv, .parse = tester_parse_arg, .user_data = tester};
	ChenierParseStatus s = chenier_parser_parse(&parser);
	if (s == ChenierParseStatus_PARSING_OK) chenier_tester_run(tester);
	CU_cleanup_registry();
	return 0;
}