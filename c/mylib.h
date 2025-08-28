#ifndef MYLIB_H
#define MYLIB_H

#include "gb.h" // header only
#include "stdio.h"

#define FOR(idx, start, end) for(i32 idx = (start);idx < (end);idx += 1)

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
