#include <stdio.h>
#include <stdlib.h>
#include "libtcc.h"

void error_callback(void *opaque, const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

int check_file_syntax(const char *filename)
{
    TCCState *s = tcc_new(16 * 1024 * 1024);
    if (!s) {
        fprintf(stderr, "Could not create tcc state\n");
        return -1;
    }

    tcc_set_lib_path(s, ".");
    tcc_set_error_func(s, NULL, error_callback);
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    //tcc_set_output_type(s, TCC_OUTPUT_PREPROCESS);

    int result = tcc_add_file(s, filename);

    printf("Watermark %d\n", tcc_arena_watermark());
    tcc_delete(s);

    return result;
}

int check_syntax(const char *content)
{
    TCCState *s = tcc_new(16 * 1024 * 1024);
    if (!s) {
        fprintf(stderr, "Could not create tcc state\n");
        return -1;
    }

    tcc_set_lib_path(s, ".");
    tcc_set_error_func(s, NULL, error_callback);
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    int result = tcc_compile_string(s, content);

    tcc_delete(s);

    return result;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file.c>\n", argv[0]);
        return 1;
    }

    for(int i =0; i<100; i++) {
	    int result = check_file_syntax(argv[1]);

	    if (result == 0) {
		    //printf("Syntax OK: %s\n", argv[1]);
	    } else {
		    printf("Syntax errors in: %s\n", argv[1]);
	    }
    }
}
