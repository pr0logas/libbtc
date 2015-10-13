/**********************************************************************
 * Copyright (c) 2015 Jonas Schnelli                                  *
 * Distributed under the MIT software license, see the accompanying   *
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.*
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "utils.h"
#include "vector.h"

struct teststruct {
    void *dummy1;
    void *dummy2;
};

void free_dummy(void *data)
{
    free(((struct teststruct *)data)->dummy1);
    free(((struct teststruct *)data)->dummy2);
    free((struct teststruct *)data);
}

void test_vector()
{
    bool res;
    char str0[] = "string";
    char str1[] = "rumba";

    vector *vec = vector_new(10, NULL);
    assert(vec != NULL);
    assert(vec->len == 0);
    assert(vec->alloc > 0);

    res = vector_add(vec, str0);
    assert(res == true);
    assert(vec->len == 1);

    res = vector_add(vec, str1);
    assert(res == true);
    assert(vec->len == 2);

    assert(vector_find(vec, str0) == 0);
    assert(vector_find(vec, str1) == 1);

    assert(strcmp(vector_idx(vec, 0), "string") == 0);
    assert(strcmp(vector_idx(vec, 1), "rumba") == 0);

    vector_remove_idx(vec,0);
    assert(res == true);
    assert(strcmp(vector_idx(vec, 0), "rumba") == 0);

    vector_free(vec, true);

    vec = vector_new(10, free);
    res = vector_add(vec, strdup("TEST0")); assert(res == true);
    res = vector_add(vec, strdup("TEST1")); assert(res == true);
    assert(vec->len == 2);
    vector_free(vec, true);


    // test resize
    vec = vector_new(10, free);
    char str[80];
    int i;
    for (i=0;i<20;i++)
    {
        sprintf(str, "TEST%d", i);
        res = vector_add(vec, strdup(str)); assert(res == true);
        assert(vec->len == i+1);
    }

    res = vector_resize(vec, 5); assert(res == true);
    assert(strcmp(vector_idx(vec, 0), "TEST0") == 0);
    assert(strcmp(vector_idx(vec, 4), "TEST4") == 0);
    assert(vector_idx(vec, 5) == NULL);
    vector_free(vec, true);


    // test custom free callback handler
    struct teststruct *some_data = calloc(1,sizeof(struct teststruct));
    some_data->dummy1 = calloc(1,10);
    some_data->dummy2 = calloc(1,10);

    vec = vector_new(1, free_dummy);
    vector_add(vec, some_data);
    vector_free(vec, true);
}