#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ei.h"
#include "erl_test/utils.h"


void process_tuple(char* buff, int *index, int size) {
    // ei_term term;
    // int res = ei_decode_ei_term(buff, index, &term);
    // if(res) {
    for(int i = 0; i < size; i++) {
        ei_term term;
        int res = ei_decode_ei_term(buff, index, &term);
        fprintf(stderr, "Term decoding result: %d\n", res);
    }
    // }
}
