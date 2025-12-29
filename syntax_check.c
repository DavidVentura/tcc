#include <stdio.h>
#include <stdlib.h>
#include "libtcc.h"

TCCBufWriter buf_writer = { NULL, 0, 0, 0 };
TCCBufWriter debug_buf_writer = { NULL, 0, 0, 0 };

void error_callback(void *opaque, const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

TCCState* init_tcc() {
    TCCState *s = tcc_new(16*1024*1024);
    tcc_set_lib_path(s, ".");
    tcc_add_sysinclude_path(s, ".");
    tcc_set_error_func(s, NULL, error_callback);
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    tcc_set_options(s, "-Wall");
    tcc_set_options(s, "-Werror");
    return s;
}
int check_syntax(const char *source, int with_type_info) {

    TCCState *s = init_tcc();
    if (!s) {
        fprintf(stderr, "Could not create tcc state\n");
        return -1;
    }

    int result;

    if (with_type_info) {
        if (buf_writer.buf == NULL) {
             buf_writer.buf = malloc(1 * 1024 * 1024);
             buf_writer.size = 1 * 1024 * 1024;
        }
        do {
            buf_writer.full = 0;
            buf_writer.pos = 0;
            result = tcc_compile_string_ex(s, source, &buf_writer);
            if (buf_writer.full) {
                buf_writer.size *= 2;
                buf_writer.buf = realloc(buf_writer.buf, buf_writer.size);
                tcc_delete(s);
                s = init_tcc();
            }
        } while (buf_writer.full);

        /* Get debug calls */
        if (debug_buf_writer.buf == NULL) {
            debug_buf_writer.buf = malloc(1 * 1024 * 1024);
            debug_buf_writer.size = 1 * 1024 * 1024;
        }
        debug_buf_writer.full = 0;
        debug_buf_writer.pos = 0;
        tcc_get_debug_calls(s, &debug_buf_writer);
    } else {
        result = tcc_compile_string_ex(s, source, NULL);
    }

    tcc_delete(s);
    return result;
}

const char* get_type_info_buffer() {
    return buf_writer.buf;
}

int get_type_info_length() {
    return buf_writer.pos;
}

const char* get_debug_calls_buffer() {
    return debug_buf_writer.buf;
}

int get_debug_calls_length() {
    return debug_buf_writer.pos;
}
