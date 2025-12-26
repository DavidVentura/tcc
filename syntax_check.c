#include <stdio.h>
#include <stdlib.h>
#include "libtcc.h"

void error_callback(void *opaque, const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

//static TCCState *s = NULL;
int init_tcc() {
	return 0;
}
int check_syntax(const char *source) {
    TCCState *s = tcc_new(16*1024*1024);
    if (!s) {
        fprintf(stderr, "Could not create tcc state\n");
        return -1;
    }
    tcc_set_lib_path(s, ".");
    tcc_add_sysinclude_path(s, ".");
    tcc_set_error_func(s, NULL, error_callback);
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    //int result = tcc_add_file(s, "prelude.c");
    //fprintf(stderr, "thing\n");
    // wanted to preload vmlinux.h parsing
    // (~3MB) but 

    int result = tcc_compile_string(s, source);
    tcc_delete(s);
    return result;
}
