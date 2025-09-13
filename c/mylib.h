#ifndef MYLIB_H
#define MYLIB_H

#include "gb.h" // header only
#include "stdio.h"

#define FOR(idx, start, end) for(i32 idx = (start);idx < (end);idx += 1)

i64 pow_i64(i64 base, i64 exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    i64 res = 0;
    i64 half_exp = exp / 2;
    b32 odd_exp = exp & 1;
    i64 half_pow = pow_i64(base, half_exp);

    return half_pow * half_pow * ((odd_exp) ? base : 1);
}

// #define gb_array_print(x, a)

// note: matrix type is adapted from gb_array_*

typedef struct _matrixHeader {
	isize       rows, cols;
} matrixHeader;

#define matrix(Type) Type *

#define MATRIX_HEADER(x)    (cast(matrixHeader *)(x) - 1)

#define matrix_rows(x)     (MATRIX_HEADER(x)->rows)
#define matrix_cols(x)     (MATRIX_HEADER(x)->cols)
#define matrix_size(x)     (matrix_rows(x) * matrix_cols(x))

#define matrix_make(matrix_name, Type, alloc, num_rows, num_cols) \
matrix(Type) matrix_name; \
do { \
	matrixHeader *mh = cast(matrixHeader *)gb_alloc(alloc, gb_size_of(matrixHeader)+gb_size_of(Type)*((num_rows) * (num_cols))); \
	mh->rows = num_rows; \
	mh->cols = num_cols; \
	matrix_name = cast(matrix(Type))(mh+1); \
} while (0)

#define matrix_make_like(matrix_name, Type, alloc, other_matrix) \
    matrix_make(matrix_name, Type, alloc, matrix_rows(other_matrix), matrix_cols(other_matrix))

#define matrix_delete(x, alloc) do { \
	matrixHeader *mh = MATRIX_HEADER(x); \
	gb_free(alloc, mh); \
} while (0)

// note: does copy. maybe just invert indexing?
#define matrix_transpose(x) do { \
    matrixHeader *mh = MATRIX_HEADER(x); \
    for(i32 i = 0;i < mh->rows; i += 1) \
        for(i32 j = i;j < mh->cols; i += j) \
            GB_ASSERT(false) \
    gb_swap(mh->rows, mh->cols); \
} while (0)

#define matrix_reshape(x, new_rows, new_cols) do { \
    matrixHeader *mh = MATRIX_HEADER(x); \
    GB_ASSERT_MSG(mh->rows * mh->cols == new_rows * new_cols, "matrix must have same number of elems after reshape"); \
    mh->rows = new_rows; \
    mh->cols = new_cols; \
} while (0)

#define matrix_fill(x, value) \
    for(i32 i = 0;i < matrix_rows(x) * matrix_cols(x); i += 1) x[i] = value;

#define matrix_at(x, row, col) x[(row) * matrix_cols(x) + (col)]

#define matrix_idx(x, row, col) ((row) * matrix_cols(x) + (col))

#define matrix_for(it_rows, it_cols, x) FOR(it_rows, 0, matrix_rows(x)) FOR(it_cols, 0, matrix_cols(x))
// #define matrix_for_rows(it, x) for(__typeof__(x) it = x;it < x + matrix_rows(x); it += matrix_cols(x))

matrix(i8) matrix_load_from_file(FILE* f, gbAllocator a);
matrix(i8) matrix_load_from_file_full(FILE* f, i64 data_len, gbAllocator a);
matrix(i8) matrix_load_from_buf(i8* buf, i32 len, gbAllocator a);


// max heap (requires comp function)

#define maxheap(Type) gbArray(Type)
#define maxheap_init(x, a) gb_array_init(x, a)
#define maxheap_free(x) gb_array_free(x)

#define maxheap_count(x) gb_array_count(x)

#define maxheap_insert_with_comp(x, value, comp_fptr) do {               \
    gb_array_append(x, value);                                           \
    maxheap_restore_heap_with_comp(x, 0, gb_array_count(x), comp_fptr);  \
} while (0)

#define maxheap_top(x) x[0]

#define maxheap_pop_with_comp(x, comp_fptr) do {                         \
    typeof_swap(x[0], x[gb_array_count(x) - 1]);                         \
    gb_array_pop(x);                                                     \
    maxheap_restore_heap_with_comp(x, 0, gb_array_count(x), comp_fptr);  \
} while(0)

// note: m is one past the end
#define maxheap_restore_heap_with_comp(x, i, m, comp_fptr) do {                    \
    i32 start = i;                                                                 \
    i32 end = m;                                                                   \
    while(1) {                                                                     \
        b8 has_left_child = (start * 2 + 1) < end;                                 \
        if (!has_left_child)                                                       \
            break;                                                                 \
        i32 largest_child = start * 2 + 1;                                        \
        if (largest_child + 1 < end &&                                            \
            comp_fptr(&x[largest_child + 1], &x[largest_child]) == 1)           \
            largest_child += 1;                                                   \
        if (comp_fptr(&x[largest_child], &x[start]) == 1){                       \
            typeof_swap(x[start], x[largest_child]);                              \
            start = largest_child;                                                \
        } else {                                                                   \
            start = end - 1;                                                       \
            break;                                                                 \
        }                                                                          \
    }                                                                              \
} while (0)

#define maxheap_heapify_with_comp(x, comp_fptr)                                  \
    for(i32 i = gb_array_count(x) / 2;i >= 0; i -= 1) {                          \
        maxheap_restore_heap_with_comp(x, i, gb_array_count(x), comp_fptr);      \
    }

#define typeof_swap(x, y) do {      \
    __typeof__(x) tmp = (x);        \
    (x) = (y);                      \
    (y) = tmp;                      \
} while(0)

#define heapsort(x, comp_fptr) do {                             \
    maxheap_heapify_with_comp(x, comp_fptr);                    \
    for(i32 i = gb_array_count(x) - 1;i >= 0;i -= 1) {          \
        typeof_swap(x[0], x[i]);                                \
        maxheap_restore_heap_with_comp(x, 0, i-1, comp_fptr);   \
    }                                                           \
} while(0)

#endif // MYLIB_H

// impl

#ifdef MYLIB_IMPLEMENTATION
#define MYLIB_IMPLEMENTATION_DONE

matrix(i8) matrix_load_from_file(FILE* f, gbAllocator a) {
    fseek(f, 0, SEEK_END);
    i64 file_len = ftell(f);
    fseek(f, 0, SEEK_SET);

    return matrix_load_from_file_full(f, file_len, a);
}

// stride calc assumes file in binary mode!
matrix(i8) matrix_load_from_file_full(FILE* f, i64 data_len, gbAllocator a) {

    matrix_make(data, i8, a, data_len, 1);
    fread(data, 1, data_len, f);

    i32 w, h;

    b8 is_crlf = false;
    i32 stride = 0;
    for(;stride < data_len;stride++)
        if (data[stride] == 0xA) {
            if (stride > 0 && data[stride - 1] == 0xD) {
                w = stride - 1;
                is_crlf = true;
            }
            else
                w = stride;
            break;
        };
    stride += 1;

    h = (data[data_len - 1] != 0xA) ? // if end misses newline, pretend like it does exist
        (data_len + (is_crlf ? 2 : 1)) / (stride) :
        data_len / (stride);

    // remove line endings
    i32 dest = 0;
    for(i32 i = 0;i < data_len; i += 1) {
        if (data[i] == 0xA || data[i] == 0xD) continue;
        data[dest] = data[i];
        dest += 1;
    }
    matrix_rows(data) = h * w;
    matrix_reshape(data, h, w);
    return data;
}

matrix(i8) matrix_load_from_buf(i8* buf, i32 len, gbAllocator a) {
    i32 w, h;

    i32 stride = 0;
    for(;stride < len;stride++)
        if (buf[stride] == 0xA) {
            if (stride > 0 && buf[stride - 1] == 0xD)
                w = stride - 1;
            else
                w = stride;
            break;
        };
    stride += 1;

    i32 data_len = len;
    h = (buf[data_len - 1] != 0xA) ? // if end misses newline, pretend like it does exist
        (data_len+1) / (stride) :
        data_len / (stride);

    matrix_make(data, i8, a, h, w);
    // copy and remove line endings
    i32 dest = 0;
    for(i32 i = 0;i < data_len; i += 1) {
        if (buf[i] == 0xA || buf[i] == 0xD) continue;
        data[dest] = buf[i];
        dest += 1;
    }
    return data;
}


#endif
