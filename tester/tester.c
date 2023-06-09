#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <time.h>

#include "tester.h"

int main() {
	srand(time(NULL));
	CU_initialize_registry();
	CU_SuiteInfo suites[] = {
	    card_stack_suite,
	    r7_suite,
	    CU_SUITE_INFO_NULL,
	};
	CU_register_suites(suites);
	CU_pSuite s = CU_get_suite(suites[1].pName);
	CU_basic_run_suite(s);

	CU_get_run_summary();
	CU_cleanup_registry();
	return 0;
}