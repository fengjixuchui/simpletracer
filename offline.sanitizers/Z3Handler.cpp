#include "Z3Handler.h"

#include <stdio.h>
#include <stdlib.h>


void exitf(const char* message);
void error_handler(Z3_context c, Z3_error_code e);
Z3_context mk_context_custom(Z3_config cfg, Z3_error_handler err);

void exitf(const char* message) {
	fprintf(stderr,"BUG: %s.\n", message);
	exit(1);
}

void error_handler(Z3_context c, Z3_error_code e) {

	printf("Error code: %d\n", e);
	exitf("incorrect use of Z3");
}

Z3_context mk_context() {
	Z3_config  cfg;
	cfg = Z3_mk_config();
	Z3_context context = mk_context_custom(cfg, error_handler);
	Z3_del_config(cfg);
	return context;
}

Z3_context mk_context_custom(Z3_config cfg, Z3_error_handler err) {

	Z3_set_param_value(cfg, "model", "true");
	Z3_context context = Z3_mk_context(cfg);
	//Z3_set_error_handler(context, err);

	return context;
}

Z3Handler::Z3Handler() {
	context = mk_context();
}

Z3Handler::~Z3Handler() {
	Z3_del_context(context);
}

Z3_ast Z3Handler::toAst(char *smt, size_t size) {

	// max lex bitvec[32]
	Z3_symbol symbols[MAX_LEN];
	for (int i = 0; i < MAX_LEN; ++i) {
		char s[MAX_SYMBOL_NAME];
		sprintf(s, "s[%d]", i);
		symbols[i] = Z3_mk_string_symbol(context, s);
	}

	Z3_sort sorts[MAX_LEN];
	for (int i = 0; i < MAX_LEN; ++i) {
		sorts[i] = Z3_mk_bv_sort(context, 8);
	}

	printf("%s\n", smt);
	Z3_set_ast_print_mode(context, Z3_PRINT_SMTLIB2_COMPLIANT);
	Z3_ast res = Z3_parse_smtlib2_string(context, smt,
	//		0, 0, 0, 0, 0, 0);
			MAX_LEN, symbols, sorts, 0, 0, 0);

	Z3_error_code e = Z3_get_error_code(context);
	if (e != Z3_OK) {
		return 0;
	} else {
		Z3_set_ast_print_mode(context, Z3_PRINT_SMTLIB2_COMPLIANT);

		printf("%s\n", Z3_ast_to_string(context, res));
		return res;
	}

	return 0;
}
