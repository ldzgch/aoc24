#ifndef MYLIB_H
#define MYLIB_H

#include "gb.h" // header only
#include "stdio.h"

#define FOR(idx, start, end) for(i32 idx = (start);idx < (end);idx += 1)
#ifndef DEBUG
    #define DEBUG false
#endif
#define DBG(fmt, ...) if(DEBUG) printf(fmt, __VA_ARGS__)
#define DBG_ASSERT(cond) if (DEBUG) {if (!cond) { printf("Assert failed: %s\n", #cond); abort();}}

i64 pow_i64(i64 base, i64 exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    i64 res = 0;
    i64 half_exp = exp / 2;
    b32 odd_exp = exp & 1;
    i64 half_pow = pow_i64(base, half_exp);

    return half_pow * half_pow * ((odd_exp) ? base : 1);
}

#define gb_array_fill(x, value)  \
    FOR(i, 0, gb_array_count(x)) \
        x[i] = value;

#define gb_array_make(name, type, a, n_elem)    \
    gbArray(type) name;                         \
    gb_array_init(name, a);                     \
    if ((n_elem) > 0) gb_array_resize(name, (n_elem)); \

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

#define matrix_copy_from_array(x, arr, arr_len) \
    GB_ASSERT(matrix_size(x) >= arr_len);        \
    FOR(i, 0, arr_len)                          \
        x[i] = arr[i];


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


// graph

typedef struct _graph  {
    gbArray(gbArray(i8)) data;
    i32 edge_property_size;

}  graph;

#define graph_init(g, a) gb_array_init(g.data, a)
graph graph_make(i32 num_vertices, i32 edge_property_size, gbAllocator a);

#define graph_add_edge(g, u, v)  graph_add_edge_with_p(g, u, v, NULL)
void graph_add_edge_with_p(graph g, i32 u, i32 v, void* property);

#define graph_edges_at(g, u, idx) *cast(i32*)&(g.data[u][(4 + g.edge_property_size) * idx])
#define graph_edges_property(g, u, idx) \
    &(g.data[u][(4 + g.edge_property_size) * idx + 4])


void graph_add_all_reverse_edges(graph g);

typedef struct graph_bfs_args {
    i32 start_vertex;
    gbArray(i32) distances;
    gbArray(i32) predecessors;
} graph_bfs_args;

void graph_bfs_(graph g, graph_bfs_args args);

#define graph_bfs(g, ...) \
    graph_bfs_(g, (graph_bfs_args){.start_vertex = 0, __VA_ARGS__})

void graph_print_file(graph g, FILE* f);
#define graph_print(g) graph_print_file(g, stdout)

// hashes

u32 hash_fnv1_32(u8* data, u32 data_len);

// hashtable/map (u32 to u64 for now)

typedef struct {
    u8* keystates;
    u32* keys;
    u64* values;
    u32 count, cap;
    gbAllocator a;
} map;

inline map map_make(gbAllocator a);
void map_put(map* m, u32 key, u64 value);
u64* map_get(map* m, u32 key);
b8   map_del(map* m, u32 key);
void map_free(map* m);
void map_resize(map* m, u32 new_cap);

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

graph graph_make(i32 num_vertices, i32 edge_property_size, gbAllocator a) {
    graph g; graph_init(g, a);
    g.edge_property_size = edge_property_size;
    gb_array_resize(g.data, num_vertices);
    FOR(i, 0, num_vertices) gb_array_init(g.data[i], a);
    return g;
}

void graph_add_edge_with_p(graph g, i32 u, i32 v, void* property) {
    GB_ASSERT(u < gb_array_count(g.data) && u >= 0);
    GB_ASSERT(v < gb_array_count(g.data) && v >= 0);

    gb_array_appendv(g.data[u], cast(i8*)&v, sizeof(i32));
    if (property != NULL)
        gb_array_appendv(g.data[u], cast(i8*)property, g.edge_property_size);
}

// really no better solution?
void graph_add_all_reverse_edges(graph g) {
    gbArray(i32) edge_counts; gb_array_init(edge_counts, gb_heap_allocator());
    gbArray(i8) tmp; gb_array_init(tmp, gb_heap_allocator());
    gb_array_resize(edge_counts, gb_array_count(g.data));
    gb_array_resize(tmp, g.edge_property_size);
    FOR(u, 0, gb_array_count(g.data)) {
        edge_counts[u] = gb_array_count(g.data[u]);
    }
    FOR(u, 0, gb_array_count(g.data)) {
        FOR(idx, 0, edge_counts[u]) {
            i32 v = graph_edges_at(g, u, idx);
            if (g.edge_property_size > 0) {
                gb_memcopy(tmp, graph_edges_property(g, u, idx), g.edge_property_size);
                graph_add_edge_with_p(g, v, u, tmp);
            } else
                graph_add_edge_with_p(g, v, u, NULL);
        }
    }
    gb_array_free(edge_counts);
    gb_array_free(tmp);
}

void graph_bfs_(graph g, graph_bfs_args args) {
    GB_ASSERT(args.distances != NULL);
    i32 start_vertex = args.start_vertex;
    gbArray(i32) distances = args.distances;
    gbArray(i32) predecessors = args.predecessors;

    i32 max_distance = (1 << 31) - 1;
    i32 queue_start = 0;

    gb_array_fill(distances, max_distance);
    gbArray(i32) queue; gb_array_init(queue, gb_heap_allocator());

    gb_array_append(queue, start_vertex);
    distances[start_vertex] = 0;
    if (predecessors != NULL) predecessors[start_vertex] = -1;

    // run
    while (gb_array_count(queue) - queue_start > 0) {
        i32 curr = queue[queue_start];
        queue_start += 1;

        gbArray(i32) edges = cast(gbArray(i32))g.data[curr];
        FOR(edge_idx, 0, gb_array_count(edges)) {
            i32 other_vertex = graph_edges_at(g, curr, edge_idx);

            if (distances[other_vertex] != max_distance) continue;

            distances[other_vertex] = distances[curr] + 1;
            if (predecessors != NULL) predecessors[other_vertex] = curr;
            gb_array_append(queue, other_vertex);
        }
    }
    gb_array_free(queue);
}


void graph_print_file(graph g, FILE* f) {
    i32 edge_count = 0;
    FOR(u, 0, gb_array_count(g.data)) edge_count += gb_array_count(g.data[u]);
    fprintf(f, "Graph(%i), Num edges: %i\n", gb_array_count(g.data), edge_count);
    FOR(u, 0, gb_array_count(g.data)) {
        FOR(idx, 0, gb_array_count(g.data[u])) {
            fprintf(f, "%i, %i\n", u, g.data[u][idx]);
        }
    }
}

#define FNV1_32_BASIS 0x811c9dc5
#define FNV1_32_PRIME 0x01000193

u32 hash_fnv1_32(u8* data, u32 data_len) {
    DBG("num bytes %i\n", data_len);
    u32 hash = FNV1_32_BASIS;

    FOR(i, 0, data_len) {
        hash = hash * FNV1_32_PRIME;
        hash = hash ^ data[i];
    }
    return hash;
}

#define MAP_MIN_CAP 16
#define MAP_SHOULD_GROW(m) (m->count >= (4 * cast(u64)m->cap / 5))

#define MAP_STATE_EMPTY 0
#define MAP_STATE_FULL 1
#define MAP_STATE_DEL 2

#define MAP_HASH_FN(key) hash_fnv1_32(cast(u8*)&key, sizeof(key))


map map_make_with_cap(u32 init_cap, gbAllocator a) {
    map m;
    m.count = 0;
    m.cap = gb_max(init_cap, MAP_MIN_CAP);
    m.keystates = gb_alloc(a, sizeof(u8) * m.cap);
    m.keys = gb_alloc(a, sizeof(u32) * m.cap);
    m.values = gb_alloc(a, sizeof(u64) * m.cap);
    m.a = a;
    FOR(i, 0, m.cap) m.keystates[i] = MAP_STATE_EMPTY;
    // FOR(i, 0, m.cap) m.keys[i] = 0;
    return m;
}

inline map map_make(gbAllocator a) {return map_make_with_cap(MAP_MIN_CAP, a);}

void map_put(map* m, u32 key, u64 value) {
    if (MAP_SHOULD_GROW(m)) {
        DBG("resize to %i\n", m->cap * 2);
        map_resize(m, m->cap * 2);
    }
    // get hash mod (cap == 2^i)
    u32 hash = MAP_HASH_FN(key);
    u32 mask = m->cap - 1;
    u32 idx = hash & mask;
    // linear probing

    // case: state is empty -> put there
    // case: state is full and keys match -> replace value, i guess?
    // case: state is full and no match -> continue probing
    // case: state is del -> put there

    while (m->keystates[idx] == MAP_STATE_FULL) {
        if (m->keys[idx] == key) {
            DBG("update idx %i\n", idx);
            m->values[idx] = value;
            return;
        }
        idx = (idx + 1) & mask;
    }
    DBG("new idx %i\n", idx);
    m->keystates[idx] = MAP_STATE_FULL;
    m->keys[idx] = key;
    m->values[idx] = value;
    m->count += 1;
    return;
}

u64* map_get(map* m, u32 key) {
    // get hash mod (cap == 2^i)
    u32 hash = MAP_HASH_FN(key);
    u32 mask = m->cap - 1;
    u32 idx = hash & mask;
    // linear probing

    // case: state is empty -> ret NULL
    // case: state is full and keys match -> ret ptr
    // case: state is full and no match -> continue probing
    // case: state is del -> cont. probing

    while (m->keystates[idx] != MAP_STATE_EMPTY) {
        if (m->keystates[idx] == MAP_STATE_FULL && m->keys[idx] == key) {
            DBG("found at idx %i\n", idx);
            return &m->values[idx];
        }
        idx = (idx + 1) & mask;
    }
    DBG("not found\n", 0);
    return NULL;
}
b8   map_del(map* m, u32 key) {
    // get hash mod (cap == 2^i)
    u32 hash = MAP_HASH_FN(key);
    u32 mask = m->cap - 1;
    u32 idx = hash & mask;
    // linear probing

    // case: state is empty -> ret false
    // case: state is full and keys match -> del
    // case: state is full and no match -> continue probing
    // case: state is del -> cont. probing

    while (m->keystates[idx] != MAP_STATE_EMPTY) {
        if (m->keystates[idx] == MAP_STATE_FULL && m->keys[idx] == key) {
            m->keystates[idx] = MAP_STATE_DEL;
            return true;
        }
        idx = (idx + 1) & mask;
    }
    return false;
}
void map_free(map* m) {
    gb_free(m->a, m->keystates);
    gb_free(m->a, m->keys);
    gb_free(m->a, m->values);
}
void map_resize(map* m, u32 new_cap) {
    DBG_ASSERT(new_cap & (new_cap - 1) == 0); // is power of 2
    map new_m = map_make_with_cap(new_cap, m->a);
    FOR(idx, 0, m->cap) {
        if (m->keystates[idx] == MAP_STATE_FULL) {
            map_put(&new_m, m->keys[idx], m->values[idx]);
        }
    }
    map_free(m);
    *m = new_m;
}

#endif
