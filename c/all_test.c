// #pragma GCC diagnostic ignore "-Wformat"


#include "stdio.h"
#define GB_IMPLEMENTATION
#define MYLIB_IMPLEMENTATION
#include "mylib.h"

#define i64 long long
#define i32 int
#define b32 int

void test_pow_i64() {
    printf("%lli\n", pow_i64(10, 0));
    printf("%lli\n", pow_i64(10, 1));
    printf("%lli\n", pow_i64(10, 2));
    printf("%lli\n", pow_i64(10, 3));

    printf("%lli\n", pow_i64(2, 0));
    printf("%lli\n", pow_i64(2, 1));
    printf("%lli\n", pow_i64(2, 2));
    printf("%lli\n", pow_i64(2, 3));

    printf("%lli\n", pow_i64(1, 3));

    printf("%lli\n", pow_i64(7, 19));

    printf("%lli\n", pow_i64(16, 16)); // overflow

}

void test_minheap() {
    gbAllocator a = gb_heap_allocator();

    gbArray(i32) test; gb_array_init(test, a);
    gb_array_append(test, 2);
    gb_array_append(test, 1);
    gb_array_append(test, 5);
    gb_array_append(test, 3);

    gb_array_append(test, 4);
    gb_array_append(test, 8);
    gb_array_append(test, 7);
    gb_array_append(test, 6);

    heapsort(test, gb__i32_cmp);

    FOR(i, 0, gb_array_count(test))
        printf("%i, ", test[i]);
    puts("");

    gb_array_clear(test);
    gbRandom r;
    gb_random_init(&r);
    FOR(i, 0, 1000) {
        i32 rng = gb_random_range_i64(&r, 1, 500);
        gb_array_append(test, rng);
    }
    heapsort(test, gb__i32_cmp);

    FOR(i, 0, gb_array_count(test))
        printf("%i, ", test[i]);
    puts("");
}

void test_map() {
    map m_data = map_make(gb_heap_allocator());
    map* m = &m_data;
    u64* res;

    FOR(i, 200, 300) {
        map_put(m, i * 45, i * 23);
    }

    FOR(i, 200, 300) {
        res = map_get(m, i * 45);
        if (res == NULL)
            GB_ASSERT(false);
        else
            GB_ASSERT(*res == i * 23);

        *res += 1;
    }

    FOR(i, 200, 300) {
        res = map_get(m, i * 45);
        GB_ASSERT(*res == i * 23 + 1);
    }
    // del some keys
    FOR(i, 200, 300) {
        map_del(m, i * 45);
        i += 1;
    }

    FOR(i, 200, 300) {
        res = map_get(m, i * 45);
        b8 correct =
            i%2 == 0 && res == NULL ||
            i%2 == 1 && res != NULL;
        if (!correct) DBG_ASSERT(false);
    }
    DBG_ASSERT(false);

    map_free(m);
}

int main() {
    // test_pow_i64();

    // test_minheap();

    //printf("%i", GB_DEFAULT_MEMORY_ALIGNMENT);

    test_map();

    return 0;
}