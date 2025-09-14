#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define GB_IMPLEMENTATION
#include "gb.h"

#define MYLIB_IMPLEMENTATION
#include "mylib.h"


// utils

u32 read_u32(FILE* f) {
    i8 c;
    u32 res = 0;
    while (c = fgetc(f), gb_char_is_digit(c)) {
        res *= 10;
        res += (c - '0');
    }
    if (c != EOF)
        ungetc(c, f);
    return res;
}


gbString read_entire_file(FILE* f, gbAllocator a) {
    fseek(f, 0, SEEK_END);
    i64 file_len = ftell(f);
    fseek(f, 0, SEEK_SET);

    gbString data = gb_string_make_length(a, NULL, file_len);
    fread(data, 1, file_len, f);

    return data;
}



#define gb_array_last(array) array[gb_array_count(array) - 1]

#define DAY(number, file, alloc, part) day##number(FILE * file, gbAllocator a, b32 part)

void DAY(1, f, a, part1) {
    gbArray(i32) array1; gb_array_init(array1, a);
    gbArray(i32) array2; gb_array_init(array2, a);
    i32 num1, num2;
    while (!feof(f)) {
        fscanf(f, "%i %i", &num1, &num2);
        gb_array_append(array1, num1);
        gb_array_append(array2, num2);
    }

    i32 sum = 0;

    if (part1) {
        gb_sort(array1, gb_array_count(array1), sizeof(i32), gb_i32_cmp(0));
        gb_sort(array2, gb_array_count(array1), sizeof(i32), gb_i32_cmp(0));
        for (int i = 0;i < gb_array_count(array1);i++) {
            // printf("%i %i\n", array1[i], array2[i]);
            sum += gb_abs(array1[i] - array2[i]);
        }
    } else {
        gbArray(i32) counts; gb_array_init(counts, a);
        gb_array_set_capacity(counts, 100000);
        for (int i = 0;i < gb_array_count(array2);i++) {
            counts[array2[i]]++;
        }
        for (int i = 0;i < gb_array_count(array1);i++) {
            sum += array1[i] * counts[array1[i]];
        }
    }

    printf("%i", sum);
}

b32 day2_is_safe(gbArray(i32) nums, gbAllocator a) {
    b32 is_increasing, is_decreasing, is_smooth;

    gbArray(i32) diff; gb_array_init(diff, a);
    gb_array_set_capacity(diff, 8);

    for(int i = 0;i < gb_array_count(nums) - 1;i++) {
        gb_array_append(diff, nums[i+1] - nums[i]);
    }

    is_increasing = true;
    for(int i = 0;i < gb_array_count(diff);i++) {
        is_increasing = is_increasing && diff[i] > 0;
    }

    is_decreasing = true;
    for(int i = 0;i < gb_array_count(diff);i++) {
        is_decreasing = is_decreasing && diff[i] < 0;
    }

    is_smooth = true;
    for(int i = 0;i < gb_array_count(diff);i++) {
        is_smooth = is_smooth && diff[i] != 0 && gb_abs(diff[i]) <= 3;
    }

    gb_array_free(diff);
    return (is_increasing || is_decreasing) && is_smooth;
}

void DAY(2, f, a, part1) {
    gbArray(i32) array; gb_array_init(array, a);
    gb_array_set_capacity(array, 8);

    i32 num;
    i8 c;

    i32 sum_safe = 0;

    while (!feof(f)) {
        gb_array_clear(array);
        while (true) {
            c = fgetc(f);
            if (c == '\n' || c == EOF)
                break;
            ungetc(c, f);

            fscanf(f, "%i", &num);
            gb_array_append(array, num);
        }
        if (gb_array_count(array) == 0) break;

        if (part1) {
            sum_safe += day2_is_safe(array, a) ? 1 : 0;
        } else {
            gbArray(i32) new_nums; gb_array_init(new_nums, a);
            gb_array_set_capacity(array, 8);
            for(int skip = 0;skip < gb_array_count(array);skip++) {
                gb_array_clear(new_nums);

                for(int i = 0;i < skip;i++) {
                    gb_array_append(new_nums, array[i]);
                }

                for(int i = skip+1;i < gb_array_count(array);i++) {
                    gb_array_append(new_nums, array[i]);
                }

                if (day2_is_safe(new_nums, a)) {
                    sum_safe++;
                    break;
                }
            }
        }
    }
    printf("%i", sum_safe);
}

void DAY(3, f, a, part1) {
    i8 c = 0;
    u32 num1, num2;
    u32 sum = 0;
    b32 mul_enable = true;

    while (c != EOF) {
        /*

        if (match(f, "mul(")) {
            num1 = read_u32(f);
            c = fgetc(f);
            if (c == ',') {
                num2 = read_u32(f);
            }
            c = fgetc(f);
            if (c == ')' && (mul_enable || part1)) {
                sum += num1 * num2;
            }
        }


        */
        c = fgetc(f);
        if (c == 'm') {
            c = fgetc(f);
            if (c == 'u') {
                c = fgetc(f);
                if (c == 'l') {
                    c = fgetc(f);
                    if (c == '(') {
                        num1 = read_u32(f);
                        c = fgetc(f);
                        if (c == ',') {
                            num2 = read_u32(f);
                            c = fgetc(f);
                            if (c == ')' && (mul_enable || part1)) {
                                sum += num1 * num2;
                            }
                        }
                    }
                }
            }
        }

        if (c == 'd') {
            c = fgetc(f);
            if (c == 'o') {
                c = fgetc(f);
                if (c == '(') {
                    c = fgetc(f);
                    if (c == ')') {
                        mul_enable = true;
                    }
                }

                if (c == 'n') {
                    c = fgetc(f);
                    if (c == '\'') {
                        c = fgetc(f);
                        if (c == 't') {
                            c = fgetc(f);
                            if (c == '(') {
                                c = fgetc(f);
                                if (c == ')') {
                                    mul_enable = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    printf("%i", sum);
}

void DAY(4, f, a, part1) {
    fseek(f, 0, SEEK_END);
    i64 file_len = ftell(f);
    fseek(f, 0, SEEK_SET);

    i8* data = malloc(file_len);
    fread(data, 1, file_len, f);

    i32 idx = 0;
    for(;idx < file_len;idx++)
        if (data[idx] == '\n') break;

    i32 w, h;
    w = idx;
    h = file_len / idx;
    #define DATA(i, j) data[((i)*(w+1) + (j))]

    if (!part1) {
        const char * MAS = "MAS";
        const char * SAM = "SAM";
        char tmp[3];
        i32 x_count = 0;
        b32 diag_ok, cdiag_ok;
        for(int i = 0;i < h - 2;i++) {
            for(int j = 0;j < w - 2;j++) {
                tmp[0] = DATA(i, j);
                tmp[1] = DATA(i+1, j+1);
                tmp[2] = DATA(i+2, j+2);

                diag_ok = gb_memcompare(tmp, MAS, 3) == 0 ||
                    gb_memcompare(tmp, SAM, 3) == 0;

                tmp[0] = DATA(i, j+2);
                tmp[1] = DATA(i+1, j+1);
                tmp[2] = DATA(i+2, j);

                cdiag_ok = gb_memcompare(tmp, MAS, 3) == 0 ||
                    gb_memcompare(tmp, SAM, 3) == 0;

                if (cdiag_ok && diag_ok)
                    x_count++;
            }
        }
        printf("%i", x_count);
        return;

    }

    i32 xmas_count = 0;

    const char * XMAS = "XMAS";
    const char * SAMX = "SAMX";
    // horizonal
    for(int i = 0;i < h;i++) {
        for(int j = 0;j < w - 3;j++) {
            if (gb_memcompare(&DATA(i, j), XMAS, 4) == 0)
                xmas_count++;
            if (gb_memcompare(&DATA(i, j), SAMX, 4) == 0)
                xmas_count++;
        }
    }
    // vertical
    char tmp[4];
    for(int i = 0;i < h - 3;i++) {
        for(int j = 0;j < w;j++) {
            tmp[0] = DATA(i, j);
            tmp[1] = DATA(i+1, j);
            tmp[2] = DATA(i+2, j);
            tmp[3] = DATA(i+3, j);
            if (gb_memcompare(tmp, XMAS, 4) == 0)
                xmas_count++;
            if (gb_memcompare(tmp, SAMX, 4) == 0)
                xmas_count++;
        }
    }
    // diag
    for(int i = 0;i < h - 3;i++) {
        for(int j = -h;j < w - 3;j++) {
            if (j < 0 || j+3 >= w) continue;
            tmp[0] = DATA(i, j);
            tmp[1] = DATA(i+1, j+1);
            tmp[2] = DATA(i+2, j+2);
            tmp[3] = DATA(i+3, j+3);
            if (gb_memcompare(tmp, XMAS, 4) == 0)
                xmas_count++;
            if (gb_memcompare(tmp, SAMX, 4) == 0)
                xmas_count++;
        }
    }
    // counter diag
    for(int i = 0;i < h - 3;i++) {
        for(int j = 3;j < w + h;j++) {
            if (j-3 < 0 || j >= w) continue;
            tmp[0] = DATA(i, j);
            tmp[1] = DATA(i+1, j-1);
            tmp[2] = DATA(i+2, j-2);
            tmp[3] = DATA(i+3, j-3);
            if (gb_memcompare(tmp, XMAS, 4) == 0)
                xmas_count++;
            if (gb_memcompare(tmp, SAMX, 4) == 0)
                xmas_count++;
        }
    }

    printf("%i", xmas_count);

    #undef DATA
}

gbArray(gbArray(i32)) order;

int order_cmp(const void* a, const void* b) {
    const i32 ia = *(const i32*)a;
    const i32 ib = *(const i32*)b;

    // if b in nums_after_a -> a < b
    gbArray(i32) nums_after_a = order[ia];
    for (int k = 0;k < gb_array_count(nums_after_a);k++)
        if (ib == nums_after_a[k])
            return -1;

    // if a in nums_after_b -> b < a
    gbArray(i32) nums_after_b = order[ib];
    for (int k = 0;k < gb_array_count(nums_after_b);k++)
        if (ia == nums_after_b[k])
            return 1;

    return 0;
}


void DAY(5, f, a, part1) {
    i32 num1, num2;
    gb_array_init(order, a);
    gb_array_set_capacity(order, 100);
    for(int i = 0;i < 100;i++)
        gb_array_init(order[i], a);

    while (true) {
        i8 c = fgetc(f);
        if (c == '\n')
            break;
        // bad in theory
        ungetc(c, f);

        num1 = read_u32(f);
        fgetc(f); // '|'
        num2 = read_u32(f);
        fgetc(f); // '\n'

        gb_array_append(order[num1], num2);
    }
    i32 mid_elem_sum = 0;
    gbArray(i32) update; gb_array_init(update, a);
    while (!feof(f)) {
        i8 c;
        gb_array_clear(update);
        while (true) {
            num1 = read_u32(f);
            gb_array_append(update, num1);

            c = fgetc(f); // ',' or '\n' at eol
            if (c == '\n' || c == EOF) break;
        }
        b32 is_correct = true;

        for (int i = 1;i < gb_array_count(update);i++) {
            // update[i] after update[0:i]
            gbArray(i32) nums_after_i = order[update[i]];
            b32 is_after = true;
            for (int j = 0;j < i;j++) {
                // is update[j] contained in nums_after_i ?
                for (int k = 0;k < gb_array_count(nums_after_i);k++) {
                    if (update[j] == nums_after_i[k])
                        is_after = false;
                }

            }
            is_correct = is_correct && is_after;
        }
        if (part1 && is_correct)
            mid_elem_sum += update[gb_array_count(update) / 2];

        if (!part1 && !is_correct) {
            gb_sort(update, gb_array_count(update), sizeof(i32), order_cmp);
            mid_elem_sum += update[gb_array_count(update) / 2];
        }
    }
end:
    printf("%i", mid_elem_sum);
}

b32 day6_loop_test(i8* data, i32 w, i32 h, i32 y, i32 x, i32 rot, i32 old_y, i32 old_x, gbAllocator a) {
    #define DATA(i, j) data[((i)*(w+1) + (j))]
    #define ROT_BUF(i, j) rot_buf[((i)*(w+1) + (j))]

    i8 backup = DATA(y, x);
    DATA(y, x) = '#';

    i32* rot_buf = gb_alloc(a, (w+1) * h * sizeof(i32));
    for(int i = 0;i < (w+1) * h;i++) {
        rot_buf[i] = -1;
    }

    b32 is_looping = false;
    i32 test_y = old_y, test_x = old_x;
    i32 test_rot = (rot + 1) % 4;
    while (true) {
        if (ROT_BUF(test_y, test_x) == test_rot) {
            is_looping = true; // return to place, loop
            break;
        }

        if (ROT_BUF(test_y, test_x) == -1)
            ROT_BUF(test_y, test_x) = test_rot;

        i32 new_test_x = test_x, new_test_y = test_y;
        switch (test_rot) {
        case 0: // up
            new_test_y -= 1;
            break;
        case 1: // right
            new_test_x += 1;
            break;
        case 2: // down
            new_test_y += 1;
            break;
        case 3: // left
            new_test_x -= 1;
            break;
        default:
            GB_ASSERT(false);
        }
        if (new_test_x < 0 || new_test_x >= w ||
            new_test_y < 0 || new_test_y >= h) {
            is_looping = false;
            break; // leave map, no loop
        }
        if (DATA(new_test_y, new_test_x) == '#') {
            test_rot = (test_rot + 1) % 4;
        } else { // move
            test_y = new_test_y;
            test_x = new_test_x;
        }
    }

    gb_free(a, rot_buf);

    DATA(y, x) = backup;
    #undef DATA
    #undef ROT_BUF
    return is_looping;
}

void DAY(6, f, a, part1) {
    fseek(f, 0, SEEK_END);
    i64 file_len = ftell(f);
    fseek(f, 0, SEEK_SET);

    i8* data = malloc(file_len);
    fread(data, 1, file_len, f);

    i32 idx = 0;
    for(;idx < file_len;idx++)
        if (data[idx] == '\n') break;

    i32 w, h;
    w = idx;
    h = file_len / idx;
    #define DATA(i, j) data[((i)*(w+1) + (j))]

    // find guard
    i32 guard_init_pos = 0;
    for(;guard_init_pos < file_len;guard_init_pos++)
        if (data[guard_init_pos] == '^') break;

    i32 guard_x = guard_init_pos % (w+1);
    i32 guard_y = guard_init_pos / (w+1);
    i32 guard_rot = 0;

    i32 x_count = 0;
    i32 loop_count = 0;

    while (true) {
        if (part1) {
            if (DATA(guard_y, guard_x) != 'X') {
                DATA(guard_y, guard_x) = 'X';
                x_count += 1;
            }
        }

        i32 new_guard_x = guard_x, new_guard_y = guard_y;
        switch (guard_rot) {
        case 0: // up
            new_guard_y -= 1;
            break;
        case 1: // right
            new_guard_x += 1;
            break;
        case 2: // down
            new_guard_y += 1;
            break;
        case 3: // left
            new_guard_x -= 1;
            break;
        default:
            GB_ASSERT(false);
        }
        if (new_guard_x < 0 || new_guard_x >= w ||
            new_guard_y < 0 || new_guard_y >= h){
                break;
        }
        if (!part1) {
            if (day6_loop_test(data, w, h, new_guard_y, new_guard_x, guard_rot, guard_y, guard_x, a))
                loop_count += 1;
        }

        if (DATA(new_guard_y, new_guard_x) == '#') {
            guard_rot = (guard_rot + 1) % 4;
        } else { // move

            guard_y = new_guard_y;
            guard_x = new_guard_x;
        }
    }

    // fwrite(data, 1, file_len, stdout);

    #undef DATA

    if (part1)
        printf("%i", x_count);
    else
        printf("%i", loop_count);
}

void DAY(7, f, a, part1) {
    gbArray(i32) nums; gb_array_init(nums, a);
    i64 true_test_num_sum = 0;

    while (!feof(f)) {
        gb_array_clear(nums);
        i64 test_num;
        fscanf(f, "%lli:", &test_num);
        while(true) {
            i32 num;
            fscanf(f, "%i", &num);
            gb_array_append(nums, num);
            i8 c = fgetc(f);
            if (c == '\n' || c == EOF)
                break;
        }
        i32 nums_count = gb_array_count(nums);
        if (part1) {
            i32 num_variations = 1 << (nums_count - 1);
            for (i32 variation = 0;variation < num_variations;variation++) {
                i64 res = nums[0];
                for (i32 bit = 1;bit < nums_count;bit++) {
                    if ((1 << (bit-1)) & variation) {
                        res = res * nums[bit];
                    } else {
                        res = res + nums[bit];
                    }
                }
                if (res == test_num) {
                    true_test_num_sum += test_num;
                    break;
                }
            }
        } else {
            i32 num_variations = pow_i64(3, (nums_count - 1));
            for (i32 variation = 0;variation < num_variations;variation++) {
                i64 res = nums[0];
                for (i32 j = 1;j < nums_count;j++) {
                    i32 operator = (variation / pow_i64(3, j-1)) % 3;
                    switch (operator) {
                    case 0:  res = res * nums[j]; break;
                    case 1:  res = res + nums[j]; break;
                    case 2: {
                        i32 num = nums[j];
                        i32 num_digits = 0;
                        while (num > 0) {
                            num /= 10;
                            num_digits += 1;
                        }
                        res = pow_i64(10, num_digits) * res + nums[j];
                    } break;
                    }

                }
                if (res == test_num) {
                    // printf("found %lli, var %i\n", test_num, variation);
                    true_test_num_sum += test_num;
                    break;
                }
            }

        }
    }
    printf("%lli", true_test_num_sum);
}

void DAY(8, f, a, part1) {
    fseek(f, 0, SEEK_END);
    i64 file_len = ftell(f);
    fseek(f, 0, SEEK_SET);

    i8* data = malloc(file_len);
    fread(data, 1, file_len, f);

    i32 idx = 0;
    for(;idx < file_len;idx++)
        if (data[idx] == '\n') break;

    i32 w, h;
    w = idx;
    h = file_len / idx;

    #define NUM_FREQS (10 + 26 + 26)
    gbArray(i32) antennas[NUM_FREQS];
    FOR(i, 0, NUM_FREQS) {
        gb_array_init(antennas[i], a);
    }

    FOR(pos, 0, file_len) {
        if (gb_char_is_digit(data[pos])) {
            int freq_idx = data[pos] - '0';
            // GB_ASSERT(freq_idx >= 0 && freq_idx < NUM_FREQS);
            gb_array_append(antennas[freq_idx], pos);
        }
        if (gb_char_is_alpha(data[pos])) {
            int freq_idx;
            if (data[pos] >= 'a')
                freq_idx = data[pos] - 'a' + 10 + 26;
            if (data[pos] <= 'Z')
                freq_idx = data[pos] - 'A' + 10;
            GB_ASSERT(freq_idx >= 0 && freq_idx < NUM_FREQS);
            gb_array_append(antennas[freq_idx], pos);
        }
    }


    i8* antinodes = malloc(file_len);
    gb_memcopy(antinodes, data, file_len);


    int multiplier_max = gb_max(h, w);

    FOR(freq, 0, NUM_FREQS) {
        gbArray(i32) positions = antennas[freq];
        i32 num_positions = gb_array_count(positions);
        FOR(i, 0, num_positions) {
            i32 ix = positions[i] % (w+1);
            i32 iy = positions[i] / (w+1);
            FOR(j, (i+1), num_positions) {
                i32 jx = positions[j] % (w+1);
                i32 jy = positions[j] / (w+1);

                i32 diffx = ix - jx;
                i32 diffy = iy - jy;

                // k * diff + i = anode for k for -inf to +inf
                // rather than inf take (+-)max(w, h)
                if (!part1) {
                    FOR(k, -multiplier_max, multiplier_max) {
                        i32 locx = ix + k * diffx;
                        i32 locy = iy + k * diffy;

                        b32 loc_in_bounds = !(locx < 0 || locx >= w ||
                            locy < 0 || locy >= h);

                        if (loc_in_bounds) {
                            antinodes[locy * (w+1) + locx] = '#';
                        }
                    }
                } else {
                    i32 loc1x = ix + diffx;
                    i32 loc1y = iy + diffy;

                    i32 loc2x = jx - diffx;
                    i32 loc2y = jy - diffy;

                    b32 loc1_in_bounds = !(loc1x < 0 || loc1x >= w ||
                        loc1y < 0 || loc1y >= h);

                    if (loc1_in_bounds) {
                        antinodes[loc1y * (w+1) + loc1x] = '#';
                    }

                    b32 loc2_in_bounds = !(loc2x < 0 || loc2x >= w ||
                        loc2y < 0 || loc2y >= h);

                    if (loc2_in_bounds) {
                        antinodes[loc2y * (w+1) + loc2x] = '#';
                    }
                }
            }
        }
    }
    i32 num_anodes = 0;
    FOR(i, 0, file_len) {
        if (antinodes[i] == '#')
            num_anodes += 1;
    }

    fwrite(antinodes, 1, file_len, stdout);

    printf("%i", num_anodes);
}

void DAY(9, f, a, part1) {
    gbString data = read_entire_file(f, a);
    i32 data_len = gb_string_length(data);

    i64 num_file_blocks = 0;
    i64 num_blocks = 0;
    FOR(i, 0, data_len) {
        i8 num_this_block = data[i] - '0';
        data[i] = num_this_block;

        num_blocks += num_this_block;
        if (i%2 == 0)
            num_file_blocks += num_this_block;
    }

    i32* blocks = gb_alloc(a, num_blocks * sizeof(i32));

    i32 block_pos = 0;
    FOR(i, 0, data_len) {
        i32 file_id = (i % 2 == 0) ? (i / 2) : -1;
        FOR(j, 0, data[i]) {
            blocks[block_pos] = file_id;
            block_pos += 1;
        }
    }

    if (part1) {
        i32 first_free_block = 0;
        i32 last_file_block = num_blocks - 1;

        while (first_free_block < last_file_block) {
            while(blocks[last_file_block] == -1) last_file_block--;
            while(blocks[first_free_block] != -1) first_free_block++;

            if (first_free_block >= last_file_block)
                break;

            blocks[first_free_block] = blocks[last_file_block];
            blocks[last_file_block] = -1;
        }
    } else {
        i32 first_free_idx = 1;
        i32 last_file_idx = data_len - ((data_len % 2 == 0) ? 2 : 1);

        i8* data_after = gb_alloc(a, data_len);
        gb_memcopy(data_after, data, data_len);

        for(i32 file_idx = last_file_idx;file_idx > 0;file_idx -= 2) {
            i8 file_size = data[file_idx];

            for(i32 free_idx = 1;free_idx < file_idx;free_idx += 2) {
                if (data_after[free_idx] >= file_size) {

                    i32 free_block = 0;
                    for(i32 i = 0;i < free_idx;i += 1) {
                        free_block += data_after[i];
                    }
                    i32 file_block = 0;
                    for(i32 i = 0;i < file_idx;i += 1) {
                        file_block += data[i];
                    }

                    data_after[free_idx] -= file_size;
                    data_after[free_idx - 1] += file_size;

                    data_after[file_idx] = 0;
                    data_after[file_idx - 1] += file_size;

                    FOR(k, 0, file_size) {
                        blocks[k + free_block] = blocks[k + file_block];
                        GB_ASSERT(blocks[k + file_block] != -1);
                        blocks[k + file_block] = -1;
                    }
                    break;
                }
            }
        }

    }

    i64 checksum = 0;
    FOR(i, 0, num_blocks) {
        if (blocks[i] == -1) continue;
        checksum += i * cast(i64)blocks[i];
    }

    printf("%lli", checksum);
}

void DAY(10, f, a, part1) {
    fseek(f, 0, SEEK_END);
    i64 file_len = ftell(f);
    fseek(f, 0, SEEK_SET);

    i8* data = malloc(file_len);
    fread(data, 1, file_len, f);

    i32 idx = 0;
    for(;idx < file_len;idx++)
        if (data[idx] == '\n') break;

    i32 w, h;
    w = idx;
    h = file_len / idx;

    i32 total_score = 0;

    i8* nine_visited = malloc(file_len);
    FOR(i, 0, file_len) data[i] -= '0';

    FOR(i, 0, file_len) {
        if (data[i] == 0) {
            // printf("start dfs %i \n", i);

            // implicit procedural DFS
            i32 pos_stack[10];
            i32 move_stack[10];
            i32 stack_idx = -1;

            FOR(i, 0, file_len) nine_visited[i] = false;

            stack_idx += 1;
            pos_stack[stack_idx] = i;
            move_stack[stack_idx] = 0;

            while (stack_idx >= 0) {
                i32 pos = pos_stack[stack_idx];
                i32 move = move_stack[stack_idx];

                i32 iw = pos % (w + 1), ih = pos / (w+1);

                // printf("dfs %i %i \n", ih, iw);

                i32 wmove[4] = {0, 0, -1, 1};
                i32 hmove[4] = {-1, 1, 0, 0};

                for(;move < 4;move++) {
                    i32 neww = iw + wmove[move];
                    i32 newh = ih + hmove[move];

                    if (neww < 0 || neww >= w) continue;
                    if (newh < 0 || newh >= h) continue;

                    i32 newpos = newh * (w+1) + neww;

                    if (data[newpos] == 9 && data[pos] == 8) {
                        // printf("dfs 9 at  %i %i \n", newh, neww);

                        if (part1) {
                            if (!nine_visited[newpos]) {
                                total_score += 1;
                                nine_visited[newpos] = true;
                            }
                            continue;
                        } else {
                            total_score += 1;
                        }
                    }


                    if (data[newpos] != data[pos] + 1) continue;

                    stack_idx += 1;
                    pos_stack[stack_idx] = newpos;
                    move_stack[stack_idx] = 0;

                    move_stack[stack_idx - 1] = move + 1;
                    goto lbl_next_dfs_loop;
                }

                stack_idx -= 1;

lbl_next_dfs_loop:
;
            }

        }
    }

    // printf("%i %i \n", h, w);
    printf("%i \n", total_score);
}

void day11_expand25(gbArray(u64)* ptr_stones, gbArray(u64)* ptr_next_stones) {
    gbArray(u64) stones = *ptr_stones;
    gbArray(u64) next_stones = *ptr_next_stones;

    FOR(time, 0, 25) {
        //printf("time %i, num stones %i\n", time, gb_array_count(stones));

        FOR(idx, 0, gb_array_count(stones)) {
            u64 stone = stones[idx];
            if (stone == 0) {
                gb_array_append(next_stones, 1);
                continue;
            }

            u64 pow_of_ten = 10;
            u64 num_digits = 1;
            while (stone >= pow_of_ten) {
                pow_of_ten *= 10;
                num_digits += 1;
            }

            if (num_digits % 2 == 0) {
                u64 div = pow_i64(10, num_digits / 2);

                u64 left = stone / div;
                u64 right = stone % div;

                gb_array_append(next_stones, left);
                gb_array_append(next_stones, right);

                continue;
            }

            gb_array_append(next_stones, stone * 2024);
        }

        gb_swap(gbArray(u64), stones, next_stones);
        gb_array_clear(next_stones);
    }

    *ptr_stones = stones;
    *ptr_next_stones = next_stones;
}

u64 day11_expand_to_3digits(gbArray(u64)* ptr_stones, gbArray(u64)* ptr_next_stones) {
    gbArray(u64) stones = *ptr_stones;
    gbArray(u64) next_stones = *ptr_next_stones;

    u64 depth = 0;
    while (true) {
        b32 all_digits = true;
        FOR(idx, 0, gb_array_count(stones)) {
            u64 stone = stones[idx];

            u64 pow_of_ten = 10;
            u64 num_digits = 1;
            while (stone >= pow_of_ten) {
                pow_of_ten *= 10;
                num_digits += 1;
            }

            if (num_digits <= 3 && depth != 0)
                continue;
            else
                all_digits = false;

            if (num_digits % 2 == 0) {
                u64 div = pow_i64(10, num_digits / 2);

                u64 left = stone / div;
                u64 right = stone % div;

                gb_array_append(next_stones, left);
                gb_array_append(next_stones, right);

                continue;
            }

            gb_array_append(next_stones, stone * 2024);
        }

        gb_swap(gbArray(u64), stones, next_stones);
        gb_array_clear(next_stones);

        depth += 1;
        if (all_digits)
            break;
    }

    *ptr_stones = stones;
    *ptr_next_stones = next_stones;

    return depth;
}

void DAY(11, f, a, part1) {
    gbArray(u64) input_stones;
    gb_array_init(input_stones, a);

    gbArray(u64) stones;
    gb_array_init(stones, a);

    gbArray(u64) next_stones;
    gb_array_init(next_stones, a);

    while(!feof(f)) {
        i32 num;
        fscanf(f, "%i", &num);
        gb_array_append(input_stones, num);
    }

    u64 num_count = 0;
    if (part1) {
        FOR(i, 0, gb_array_count(input_stones)) {
            gb_array_clear(stones);
            gb_array_clear(next_stones);

            gb_array_append(stones, input_stones[i]);

            day11_expand25(&stones, &next_stones);

            num_count += gb_array_count(stones);
        }
    } else {
        FOR(i, 0, gb_array_count(input_stones)) {
            gb_array_clear(stones);
            gb_array_clear(next_stones);

            gb_array_append(stones, input_stones[i]);

            u64 depth = day11_expand_to_3digits(&stones, &next_stones);
            printf("depth %lli\n", depth);
        }
    }
    printf("%lli", num_count);
}

typedef struct _day12side {
    i32 pos1, pos2;
    i32 move;
} Day12Side;

Day12Side make_side(i32 y, i32 x, i32 move, i32 data_w) {
    i32 pos1, pos2;
    i32 sides_w = data_w + 1; // side positions are between data positions, so there is one more
    i32 pos = y * sides_w + x;
    switch (move) {
    case 0: // down
        pos1 = pos;
        pos2 = pos + 1;
        break;
    case 1: // up
        pos1 = pos + sides_w;
        pos2 = pos + sides_w + 1;
        break;
    case 2: // left
        pos1 = pos;
        pos2 = pos + sides_w;
        break;
    case 3: // right
        pos1 = pos + 1;
        pos2 = pos + sides_w + 1;
        break;
    }
    Day12Side s = {pos1, pos2, move};
    return s;
}

void DAY(12, f, a, part1) {
    matrix(i8) data = matrix_load_from_file(f, a);
    i32 dh = matrix_rows(data);
    i32 dw = matrix_cols(data);

    printf("h %i, w %i\n", dh, dw);

    matrix_make_like(visited, b8, a, data);
    FOR(i, 0, dh) FOR(j, 0, dw) matrix_at(visited, i, j) = false;

    i64 price = 0;

    gbArray(i32) pos_stack; gb_array_init(pos_stack, a);
    gbArray(i32) move_stack; gb_array_init(move_stack, a);
    gbArray(Day12Side) sides; gb_array_init(sides, a);

    FOR(row, 0, dh) FOR(col, 0, dw) {
        if (matrix_at(visited, row, col)) continue;

        i32 area = 0;
        i32 perimeter = 0;
        i32 num_sides = 0;

        i8 type = matrix_at(data, row, col);


        // implicit procedural DFS
        gb_array_clear(pos_stack);
        gb_array_clear(move_stack);
        gb_array_clear(sides);

        gb_array_append(pos_stack, matrix_idx(data, row, col));
        gb_array_append(move_stack, 0);

        while (gb_array_count(pos_stack) > 0) {

            i32 pos = gb_array_last(pos_stack);
            i32 move = gb_array_last(move_stack);

            if (!visited[pos]) {
                visited[pos] = true;
                area += 1;
            }

            i32 iw = pos % dw, ih = pos / dw;

            // printf("dfs %i %i \n", ih, iw);

            i32 wmove[4] = {0, 0, -1, 1};
            i32 hmove[4] = {-1, 1, 0, 0};

            for(;move < 4;move++) {
                i32 neww = iw + wmove[move];
                i32 newh = ih + hmove[move];

                if (neww < 0 || neww >= dw ||
                    newh < 0 || newh >= dh) {

                    perimeter += 1;
                    if (!part1) {
                        gb_array_append(sides, make_side(ih, iw, move, dw));
                    }
                    continue;
                }

                i32 newpos = matrix_idx(data, newh, neww);

                if (data[newpos] != type) {
                    perimeter += 1;
                    if (!part1) {
                        gb_array_append(sides, make_side(ih, iw, move, dw));
                    }
                    continue;
                }
                if (visited[newpos]) continue;


                gb_array_last(move_stack) = move + 1;

                gb_array_append(pos_stack, newpos);
                gb_array_append(move_stack, 0);

                goto lbl_next_dfs_loop;
            }

            gb_array_pop(pos_stack);
            gb_array_pop(move_stack);

lbl_next_dfs_loop:
;
        }

        b8* sides_visited = gb_alloc(a, gb_array_count(sides));
        FOR(i, 0, gb_array_count(sides)) sides_visited[i] = false;

        b8 do_print = false; //  (type == 'R') && (area == 139);

        FOR(i, 0, gb_array_count(sides)) {
            if (sides_visited[i]) continue;
            i32 curr = i;
            i32 curr_pos = sides[curr].pos2;

            i32 start_pos = sides[curr].pos1;

            i32 curr_move = sides[curr].move;
            num_sides += 1;
            if (do_print) {
                printf("start and inc\n");
            }

            while (!sides_visited[curr]) {
                sides_visited[curr] = true;

                if (do_print) {
                    printf("\tpos1 %i, pos2 %i, move %i\n", sides[curr].pos1, sides[curr].pos2, curr_move);
                }


                FOR(j, 0, gb_array_count(sides)) {
                    if (sides_visited[j]) continue;


                    // sides are not compatible with the opposite
                    #define IsNotCompatible(move1, move2) \
                        ((move1 == 0 && move2 == 1) || \
                        (move1 == 1 && move2 == 0) || \
                        (move1 == 2 && move2 == 3) || \
                        (move1 == 3 && move2 == 2)) \



                    #define IsNextSide(pos_to_check, pos_to_go_next) \
                        if (curr_pos == pos_to_check) { \
                            if (IsNotCompatible(curr_move, sides[j].move)) { \
                                printf("not compat\n"); \
                                continue; \
                            } \
                            curr = j; \
                            curr_pos = pos_to_go_next; \
                            if (curr_move != sides[j].move) { \
                                num_sides += 1; \
                                if (do_print) { \
                                    printf("inc\n"); \
                                } \
                            } \
                            curr_move = sides[j].move; \
                            break; \
                        } \

                    IsNextSide(sides[j].pos1, sides[j].pos2);
                    IsNextSide(sides[j].pos2, sides[j].pos1);

                    #undef IsNextSide
                    #undef IsNotCompatible

                }
            }
            b8 end_is_start = curr_pos == start_pos;
            if (!end_is_start)
                printf("end not start");

        }
        gb_free(a, sides_visited);

        if (part1) {
            printf("type %c, area %i, perimeter %i\n", type, area, perimeter);
            price += area * perimeter;
        } else {
            printf("type %c, area %i, nsides %i\n", type, area, num_sides);
            price += area * num_sides;
        }
    }
    printf("%lli", price);
}

void DAY(13, f, a, part1) {
    i64 total_price = 0;
    while (!feof(f)) {
        i64 lowest_price = (1LL << 60);

        i64 ax, ay;
        fscanf(f, "Button A: X+%lli, Y+%lli\n", &ax, &ay);
        i64 bx, by;
        fscanf(f, "Button B: X+%lli, Y+%lli\n", &bx, &by);
        i64 px, py;
        fscanf(f, "Prize: X=%lli, Y=%lli\n", &px, &py);
        if (!part1){
            px += 10000000000000;
            py += 10000000000000;
        }

        f64 k1 =    cast(f64)(by * px - bx * py) /
                    cast(f64)(ax * by - bx * ay);

        f64 k2 =    cast(f64)(ax * py - ay * px) /
                    cast(f64)(ax * by - bx * ay);

        if (lowest_price == (1LL << 60)) {
            lowest_price = 0;
        }
        i64 ksolution = (k1 >= 0.0 && k2 >= 0.0 && (gb__floor64(k1) == k1) && (gb__floor64(k2) == k2)) ?
            cast(i64)(gb__floor64(3 * k1 + k2)) : 0;

        total_price += ksolution;
    }
    printf("%lli", total_price);
}

void day14_printstate(gbArray(i32) xpos, gbArray(i32) ypos, gbAllocator a) {
    matrix_make(data, i8, a, 103, 101 + 1);
    matrix_fill(data, '.');
    FOR(i, 0, matrix_rows(data)) matrix_at(data, i, 101) = '\n';

    FOR(i, 0, gb_array_count(xpos))
        matrix_at(data, ypos[i], xpos[i]) = 'x';

    fwrite(data, 103 * (101 + 1), 1, stdout);
    matrix_delete(data, a);
}

i32 day14_dfs(matrix(i8) data, i32 x, i32 y) {
    if (x < 0 || x >= matrix_rows(data) ||
        y < 0 || y >= matrix_cols(data) ||
        matrix_at(data, x, y) != 'x')
            return 0;
    matrix_at(data, x, y) = '_';

    return
        day14_dfs(data, x, y+1) +
        day14_dfs(data, x, y-1) +
        day14_dfs(data, x+1, y) +
        day14_dfs(data, x-1, y) +

        day14_dfs(data, x-1, y-1) +
        day14_dfs(data, x-1, y+1) +
        day14_dfs(data, x+1, y-1) +
        day14_dfs(data, x+1, y+1) +
        1;
}

i32 day14_largest_cluster(gbArray(i32) xpos, gbArray(i32) ypos, gbAllocator a) {
    matrix_make(data, i8, a, 103, 101);
    matrix_fill(data, '.');
    FOR(i, 0, gb_array_count(xpos))
        matrix_at(data, ypos[i], xpos[i]) = 'x';

    i32 largest_cluster = 0;

    FOR(i, 0, matrix_rows(data)) FOR(j, 0, matrix_cols(data)) {
        i32 this_cluster = day14_dfs(data, i, j);
        if (largest_cluster < this_cluster)
            largest_cluster = this_cluster;
    }

    matrix_delete(data, a);
    return largest_cluster;
}

void DAY(14, f, a, part1) {
    gbArray(i32) xpos; gb_array_init(xpos, a);
    gbArray(i32) ypos; gb_array_init(ypos, a);

    gbArray(i32) xvel; gb_array_init(xvel, a);
    gbArray(i32) yvel; gb_array_init(yvel, a);

    while (!feof(f)) {
        i32 px, py, vx, vy;
        fscanf(f, "p=%i,%i v=%i,%i\n", &px, &py, &vx, &vy);

        gb_array_append(xpos, px);
        gb_array_append(ypos, py);

        gb_array_append(xvel, vx);
        gb_array_append(yvel, vy);

    }

    i32 n_iter = (part1) ? 100 : 10000;
    FOR(i, 0, n_iter) {
        FOR(r, 0, gb_array_count(xpos)) {
            i32 newx = (xpos[r] + xvel[r]);
            if (newx < 0) newx += 101;
            if (newx >= 101) newx -= 101;
            xpos[r] = newx;

            i32 newy = (ypos[r] + yvel[r]);
            if (newy < 0) newy += 103;
            if (newy >= 103) newy -= 103;
            ypos[r] = newy;
        }

        if (!part1) {
            i32 lc = day14_largest_cluster(xpos, ypos, a);
            if (lc > 35) {

                printf("lc %i after iter: %i\n", lc, i+1);
                day14_printstate(xpos, ypos, a);
            }
        }
    }
    i32 q[4]; FOR(i, 0, 4) q[i] = 0;

    FOR(r, 0, gb_array_count(xpos)) {
        if (xpos[r] < 50 && ypos[r] < 51)
            q[0] += 1;
        if (xpos[r] > 50 && ypos[r] < 51)
            q[1] += 1;
        if (xpos[r] < 50 && ypos[r] > 51)
            q[2] += 1;
        if (xpos[r] > 50 && ypos[r] > 51)
            q[3] += 1;
    }
    i64 safety = 1;
    FOR(i, 0, 4) safety *= q[i];

    printf("%lli", safety);
}

void day15_print(matrix(i8) data, i32 ry, i32 rx) {
    i8 tmp = matrix_at(data, ry, rx);
    matrix_at(data, ry, rx) = '@';
    FOR(i, 0, matrix_rows(data)) {
        fwrite(&matrix_at(data, i, 0), matrix_cols(data), 1, stdout);
        puts("");
    }
    matrix_at(data, ry, rx) = tmp;
}

void op_step(i32 y, i32 x, i32* ny, i32* nx, i8 op) {
    *ny = y;
    *nx = x;
    switch (op) {
    case '^':
        *ny -= 1;
    break;
    case 'v':
        *ny += 1;
    break;
    case '<':
        *nx -= 1;
    break;
    case '>':
        *nx += 1;
    break;
    default:
        GB_ASSERT(false);
    }
}

typedef struct _day15_box {
    i32 y; i32 x;
} Day15Box;

b8 day15_all_affected_boxes(matrix(i8) data, i32 y, i32 x, i8 op, gbArray(Day15Box)* boxes) {
    // printf("(%i, %i)\n", y, x);
    FOR(i, 0, gb_array_count(*boxes))
        if ((*boxes)[i].y == y && (*boxes)[i].x == x)
            return true;

    b8 not_box = matrix_at(data, y, x) == '.' || matrix_at(data, y, x) == '#';

    if (not_box) {
        return matrix_at(data, y, x) == '.';
    }
    b8 wide_box = matrix_at(data, y, x) == '[' || matrix_at(data, y, x) == ']';
    b8 op_is_vert = op == '^' || op == 'v';
    i32 ny = y, nx = x;
    switch (op) {
    case '^':
        ny -= 1;
    break;
    case 'v':
        ny += 1;
    break;
    case '<':
        nx -= 1;
    break;
    case '>':
        nx += 1;
    break;
    default:
        GB_ASSERT(false);
    }
    b8 can_move = true;

    if (op_is_vert) {
        if (matrix_at(data, y, x) == '[') {
            can_move = can_move && day15_all_affected_boxes(data, ny, x+1, op, boxes);
            Day15Box b = {y, x+1};
            gb_array_append(*boxes, b);
        } else if (matrix_at(data, y, x) == ']') {
            can_move = can_move && day15_all_affected_boxes(data, ny, x-1, op, boxes);
            Day15Box b = {y, x-1};
            gb_array_append(*boxes, b);
        }
    }
    can_move = can_move && day15_all_affected_boxes(data, ny, nx, op, boxes);
    // put it after
    Day15Box b = {y, x};
    gb_array_append(*boxes, b);

    return can_move;
}

void DAY(15, f, a, part1) {
    gbString s = read_entire_file(f, a);
    i32 file_len = gb_string_length(s);
    i32 matrix_len;
    FOR(i, 0, file_len - 2) {
        if ((s[i] == '\n' && s[i+1] == '\n') ||
            (s[i] == '\n' && s[i+2] == '\n')) {
            matrix_len = i;
            break;
        }
    }
    matrix(i8) data = matrix_load_from_buf(s, matrix_len, a);

    i32 op_start = matrix_len;
    while(s[op_start] == '\n' || s[op_start] == '\r')
        op_start += 1;
    i32 op_count = file_len - op_start;

    if (!part1) {
        matrix_make(tmp, i8, a, matrix_rows(data), matrix_cols(data) * 2);
        FOR(i, 0, matrix_rows(data)) FOR(j, 0, matrix_cols(data)) {
            if (matrix_at(data, i, j) == '#') {
                matrix_at(tmp, i, 2 * j + 0) = '#';
                matrix_at(tmp, i, 2 * j + 1) = '#';
            }
            if (matrix_at(data, i, j) == 'O') {
                matrix_at(tmp, i, 2 * j + 0) = '[';
                matrix_at(tmp, i, 2 * j + 1) = ']';
            }
            if (matrix_at(data, i, j) == '.') {
                matrix_at(tmp, i, 2 * j + 0) = '.';
                matrix_at(tmp, i, 2 * j + 1) = '.';
            }
            if (matrix_at(data, i, j) == '@') {
                matrix_at(tmp, i, 2 * j + 0) = '@';
                matrix_at(tmp, i, 2 * j + 1) = '.';
            }
        }
        matrix_delete(data, a);
        data = tmp;
    }

    i32 rows = matrix_rows(data);
    i32 cols = matrix_cols(data);
    i32 ry, rx;
    FOR(i, 0, rows) FOR(j, 0, cols)
        if (matrix_at(data, i, j) == '@') {
            matrix_at(data, i, j) = '.';
            ry = i; rx = j;
            goto found_robot;
        }
found_robot:
    ;
//    day15_print(data, ry, rx);

    gbArray(Day15Box) boxes; gb_array_init(boxes, a);
    FOR(i, 0, op_count) {
        i8 op = s[op_start + i];
        if (op == '\n' || op == '\r') continue;
        i32 nexty, nextx;
        op_step(ry, rx, &nexty, &nextx, op);
        b8 robot_can_move = matrix_at(data, nexty, nextx) != '#';

        gb_array_clear(boxes);
        b8 can_move = day15_all_affected_boxes(data, nexty, nextx, op, &boxes);
/*
        FOR(i, 0, gb_array_count(boxes))
            printf("(%i, %i)\n", boxes[i].y, boxes[i].x);

        printf("can move: %i\n", can_move);
*/

        if (can_move && robot_can_move) {
            FOR(i, 0, gb_array_count(boxes)) {
                i32 bny, bnx;
                i32 by = boxes[i].y; i32 bx = boxes[i].x;
                op_step(by, bx, &bny, &bnx, op);
                matrix_at(data, bny, bnx) = matrix_at(data, by, bx);
                matrix_at(data, by, bx) = '.';
            }

            ry = nexty;
            rx = nextx;
        }
        /*
        printf("%c\n", op);
        day15_print(data, ry, rx);
        */
    }


    i64 gps_sum = 0;
    FOR(i, 0, rows) FOR(j, 0, cols) {
        if (matrix_at(data, i, j) == 'O' || matrix_at(data, i, j) == '[')
            gps_sum += 100 * i + j;

    }

    printf("%lli", gps_sum);
}

typedef struct _day16point {
    i32 y; i32 x;
    i32 distance;
    i32 from_move;
} Day16Point;

i8 day16_cmp_rev(const Day16Point* p1, const Day16Point* p2) {
    return -1 * (p1->distance < p2->distance ? -1 : p1->distance > p2->distance);
}

void day16_print(matrix(i8) data, matrix(i32) distance, gbAllocator a) {
    matrix_make_like(tmp, i8, a, data);

    matrix_for(i, j, tmp) {
        matrix_at(tmp, i, j) = matrix_at(data, i, j);

        if (matrix_at(distance, i, j) != (1 << 28)) {
            matrix_at(tmp, i, j) = '0' + (matrix_at(distance, i, j) % 10);
        }
    }


    FOR(i, 0, matrix_rows(tmp)) {
        fwrite(&matrix_at(tmp, i, 0),  matrix_cols(tmp), 1, stdout);
        printf("%i\n", i);
    }

    matrix_delete(tmp, a);
}

void DAY(16, f, a, part1) {
    matrix(i8) data = matrix_load_from_file(f, a);
    i32 rows =  matrix_rows(data);
    i32 cols =  matrix_cols(data);

    i32 sy, sx, ey, ex;
    FOR(i, 0, rows) FOR(j, 0, cols) {
        if (matrix_at(data, i, j) == 'S') {sy = i; sx = j;}
        if (matrix_at(data, i, j) == 'E') {ey = i; ex = j;}
    }

    matrix_make_like(distance, i32, a, data);
    FOR(i, 0, rows) FOR(j, 0, cols)
        matrix_at(distance, i, j) = (1 << 28);
    matrix_at(distance, sy, sx) = 0;

    maxheap(Day16Point) pos_heap; maxheap_init(pos_heap, a);

    Day16Point S = (Day16Point){sy, sx, 0, 3};
    maxheap_insert_with_comp(pos_heap, S, day16_cmp_rev);

    while(maxheap_count(pos_heap) > 0) {
        // pop front
        Day16Point p = maxheap_top(pos_heap);
        maxheap_pop_with_comp(pos_heap, day16_cmp_rev);

        // printf("curr: (%i, %i)\n", p.y, p.x);

        i32 ymove[] = {-1, 1, 0, 0};
        i32 xmove[] = {0, 0, -1, 1};
        FOR(i, 0, 4) {
            Day16Point np = {p.y + ymove[i], p.x + xmove[i], 0, i};

            if (matrix_at(data, np.y, np.x) == '#' || matrix_at(distance, np.y, np.x) != (1 << 28))
                continue;
            b8 is_turn = p.from_move != i;
            i32 dist_this_path = matrix_at(distance, p.y, p.x) + 1 + (is_turn ? 1000 : 0);
            matrix_at(distance, np.y, np.x) = dist_this_path;

            np.distance = matrix_at(distance, np.y, np.x);
            maxheap_insert_with_comp(pos_heap, np, day16_cmp_rev);
        }
    }

    //day16_print(data, distance, a);

    printf("dims: (%i, %i)\n", rows, cols);
    printf("S: (%i, %i)\n", sy, sx);
    printf("E: (%i, %i)\n", ey, ex);
    printf("dist E: %i\n", matrix_at(distance, ey, ex));

}

/*
B = A % 8

B = B xor 5

C = A >> B

B = B xor 6

A = A >> 3

B = B ^ C

out B % 8

jump to 0 if A != 0

*/

void run_program(i8* program, i32 prog_len, i64 regA, i64 regB, i64 regC, gbString* output) {
    while (regA != 0) {
        regB = regA % 8;
        regC = regA >> (regB ^ 5);

        regB = regB ^ (3 ^ regC);
        gb_string_append_rune(*output, (regB % 8) + '0');
        gb_string_append_rune(*output, ',');

        regA >>= 3;
    }
    return;

    i32 ip = 0;
    b8 inp_is_opcode = true;
    i32 op;
    while (ip < prog_len) {
        if (program[ip] == ',') {
            ip += 1;
            continue;
        }

        i32 inp = program[ip] - '0';

        if (inp_is_opcode) {
            op = inp;
        } else {
            i32 lit_operand = inp;
            i32 combo_operand;
            switch (inp) {
            case 0:
            case 1:
            case 2:
            case 3:
                combo_operand = inp;
            break;
            case 4:
                combo_operand = regA;
            break;
            case 5:
                combo_operand = regB;
            break;
            case 6:
                combo_operand = regC;
            break;
            case 7:
            default: GB_ASSERT(false);

            }

            switch (op) {
            case 0: // a divide
                regA = regA >> combo_operand;
            break;
            case 1: // b xor lit
                regB = regB ^ lit_operand;
            break;
            case 2: // combo mod 8
                regB = combo_operand % 8;
            break;
            case 3: // jnz
                if (regA != 0)
                    ip = lit_operand - 1;
            break;
            case 4: // b xor c
                regB = regB ^ regC;
            break;
            case 5: // out
                gb_string_append_rune(*output, (combo_operand % 8) + '0');
                gb_string_append_rune(*output, ',');
            break;
            case 6:
                regB = regA >> combo_operand;
            break;
            case 7:
                regC = regA >> combo_operand;
            break;
            default:
                GB_ASSERT(false);
            }
        }

        ip += 1;
        inp_is_opcode = !inp_is_opcode;
    }
    gb_string_trim(*output, ",");
}

void DAY(17, f, a, part1) {
    i64 regA, regB, regC;
    fscanf(f, "Register A: %lli\n", &regA);
    fscanf(f, "Register B: %lli\n", &regB);
    fscanf(f, "Register C: %lli\n", &regC);

    i8 program[1000];
    fscanf(f, "\nProgram: %s\n", program);
    i32 prog_len = gb_strlen(program);

    gbString output = gb_string_make_reserve(a, 16 * 100);
    if (part1) {
        run_program(program, prog_len, regA, regB, regC, &output);
        printf("Out: %s", output);
    }
    else {
        i32 mill = 1000000;
        FOR(new_regA, 35 * mill, 100 * mill) {
            printf("\r%i   ", new_regA);
            gb_string_clear(output);
            run_program(program, prog_len, new_regA, regB, regC, &output);

            if (gb_strncmp(output, program, prog_len) == 0) {
                printf("\n%s   \n", output);
                printf("Found eql at %i", new_regA);
                break;
            }
        }
    }
}

void day18_print(matrix(i8) space, gbAllocator a) {
    matrix_make_like(tmp, i8, a, space);
    matrix_for(x, y, space) {
        matrix_at(tmp, x, y) = matrix_at(space, x, y);
    }

    FOR(i, 0, matrix_rows(tmp)) {
        fwrite(&matrix_at(tmp, i, 0), 1, matrix_cols(tmp), stdout);
        puts("");
    }

    matrix_delete(tmp, a);
}

void DAY(18, f, a, part1) {
    matrix_make(space, i8, a, 71, 71);
    matrix_make_like(distance, i32, a, space);

    i32 max_distance = (1 << 30);
    matrix_fill(space, '.');
    matrix_fill(distance, max_distance);

    FOR(i, 0, 1024) {
        i32 x, y;
        fscanf(f, "%i,%i\n", &x, &y);

        matrix_at(space, x, y) = '#';
    }

    gbArray(i32) xpos; gb_array_init(xpos, a);
    gbArray(i32) ypos; gb_array_init(ypos, a);
    i32 queue_start = 0;

    matrix_at(distance, 0, 0) = 0;
    gb_array_append(xpos, 0);
    gb_array_append(ypos, 0);

    if (part1) {
        while (gb_array_count(xpos) - queue_start > 0) {
            i32 x = xpos[queue_start];
            i32 y = ypos[queue_start];
            queue_start += 1;

            matrix_at(space, x, y) = 'v';

            i32 xmove[] = {0, 0, -1, 1};
            i32 ymove[] = {-1, 1, 0, 0};
            FOR(move, 0, 4) {
                i32 nx = x + xmove[move];
                i32 ny = y + ymove[move];

                if (nx < 0 || nx > 70 || ny < 0 || ny > 70) continue;
                if (matrix_at(space, nx, ny) == '#') continue;
                if (matrix_at(distance, nx, ny) != max_distance) continue;

                matrix_at(distance, nx, ny) = matrix_at(distance, x, y) + 1;
                gb_array_append(xpos, nx);
                gb_array_append(ypos, ny);

            }
        }
        // day18_print(space, a);
        printf("%i", matrix_at(distance, 70, 70));
    } else {
        while (!feof(f)) {
            i32 x, y;
            fscanf(f, "%i,%i\n", &x, &y);
            matrix_at(space, x, y) = '#';

            // reset bfs
            matrix_fill(distance, max_distance);
            gb_array_clear(xpos);
            gb_array_clear(ypos);
            queue_start = 0;

            // init bfs
            matrix_at(distance, 0, 0) = 0;
            gb_array_append(xpos, 0);
            gb_array_append(ypos, 0);

            // run bfs
            while (gb_array_count(xpos) - queue_start > 0) {
                i32 x = xpos[queue_start];
                i32 y = ypos[queue_start];
                queue_start += 1;

                matrix_at(space, x, y) = 'v';

                i32 xmove[] = {0, 0, -1, 1};
                i32 ymove[] = {-1, 1, 0, 0};
                FOR(move, 0, 4) {
                    i32 nx = x + xmove[move];
                    i32 ny = y + ymove[move];

                    if (nx < 0 || nx > 70 || ny < 0 || ny > 70) continue;
                    if (matrix_at(space, nx, ny) == '#') continue;
                    if (matrix_at(distance, nx, ny) != max_distance) continue;

                    matrix_at(distance, nx, ny) = matrix_at(distance, x, y) + 1;
                    gb_array_append(xpos, nx);
                    gb_array_append(ypos, ny);

                }
            }
            if (matrix_at(distance, 70, 70) == max_distance) {
                printf("Unreachable after: %i,%i", x, y);
                break;
            }
        }
    }
}

typedef struct i8slice {
    i8* ptr; i32 len;
} i8slice;

void i8slice_print(i8slice s) {
    fwrite(s.ptr, s.len, 1, stdout);
}

i32 day19_lexical_cmp(const void* a, const void* b) {
    i8slice sa = *cast(i8slice*)a;
    i8slice sb = *cast(i8slice*)b;

    FOR(i, 0, gb_min(sa.len, sb.len)) {
        if (sa.ptr[i] < sb.ptr[i])
            return -1;
        if (sa.ptr[i] > sb.ptr[i])
            return 1;
    }
    if (sa.len < sb.len)
        return -1;
    if (sa.len > sb.len)
        return 1;
    return 0;
}

i64 day19_is_possible(gbArray(i8slice) patterns, i8slice target, gbAllocator a) {
    // 1. find all patterns in target (array of slices)
    // 2. dp array: i -> > 0 means that target[:i] can be matched
    // 3. one iteration, where for all i a match combined with a true entry in dp array
    gbArray(i8slice) matches; gb_array_init(matches, a);

    FOR(start, 0, target.len) {
        FOR(match_len, 1, 10) {
            if (start + match_len > target.len) continue;
            i8slice try = {target.ptr + start, match_len};
            i8slice* res = bsearch(&try, patterns, gb_array_count(patterns), sizeof(i8slice), day19_lexical_cmp);
            if (res == NULL) continue;

            gb_array_append(matches, try);
        }
    }
/*
    FOR(j, 0, gb_array_count(matches)) {
        i8slice_print(matches[j]); printf(" at pos %i\n", matches[j].ptr - target.ptr);
    }
*/

    i32 dplen = target.len + 10;
    i64* dp = gb_alloc(a, dplen * sizeof(i64));
    FOR(i, 0, dplen) dp[i] = 0;
    dp[0] = 1;

    FOR(i, 1, target.len+1) {
        if (dp[i-1] > 0) { // can solve target[:i-1]
            FOR(j, 0, gb_array_count(matches)) {
                if (matches[j].ptr - target.ptr == i - 1) {
                    dp[i-1 + matches[j].len] += dp[i-1];
                }
            }
        }
    }
    /*
    FOR(i, 0, target.len+1) {
        printf("%i, ", dp[i]);
    }
    puts("");
    */
    i64 res = dp[target.len];
    gb_free(a, dp);
    gb_array_free(matches);
    return res;
}

void DAY(19, f, a, part1) {
    gbString s = read_entire_file(f, a);
    i32 slen = gb_string_length(s);

    gbArray(i8slice) patterns; gb_array_init(patterns, a);

    i32 start = 0, end = 0;

    while (true) {
        start = end;
        while (end < slen && gb_char_is_alpha(s[end])) {
            end++;
        }
        gb_array_append(patterns, ((i8slice){&s[start], end-start}));
        if (s[end] != ',')
            break;
        while(end < slen && !gb_char_is_alpha(s[end]))
            end++;
    }

    while(end < slen && !gb_char_is_alpha(s[end]))
            end++;

    qsort(patterns, gb_array_count(patterns), sizeof(i8slice), day19_lexical_cmp);

    i64 n_possible = 0;
    while (true) {
        start = end;
        while (end < slen && gb_char_is_alpha(s[end])) {
            end++;
        }

        i8slice target = ((i8slice){&s[start], end-start});

        // i8slice_print(target); puts("");

        i64 n_matchings = day19_is_possible(patterns, target, a);
        if (n_matchings > 0)
            n_possible += part1 ? 1 : n_matchings;

        while(end < slen && !gb_char_is_alpha(s[end]))
            end++;
        if (end == slen)
            break;
    }
    printf("%lli", n_possible);
}

int main(int argc, char**argv) {
    if (argc < 2) {
        puts("Provide exercise number");
        return 1;
    }
    gbAllocator a = gb_heap_allocator();
    f64 ex_nr = gb_str_to_f64(argv[1], NULL);

    int ex_nr_int = (int)gb__floor64(ex_nr);
    b32 part = ex_nr > ex_nr_int ? false : true;

    #define CASE_DAY(n, datapath) \
        case n: { \
        FILE* f = fopen(datapath, "rb"); \
        if (f == NULL) { \
            puts("Data file not found"); \
            return 1; \
        } \
        day##n(f, a, part); \
        fclose(f); \
        } \
        break; \

    switch (ex_nr_int) {
    CASE_DAY(1, "../data/data1.txt")
    CASE_DAY(2, "../data/data2.txt")
    CASE_DAY(3, "../data/data3.txt")
    CASE_DAY(4, "../data/data4.txt")
    CASE_DAY(5, "../data/data5.txt")
    CASE_DAY(6, "../data/data6.txt")
    CASE_DAY(7, "../data/data7.txt")
    CASE_DAY(8, "../data/data8.txt")
    CASE_DAY(9, "../data/data9.txt")
    CASE_DAY(10, "../data/data10.txt")
    CASE_DAY(11, "../data/data11.txt")
    CASE_DAY(12, "../data/data12.txt")
    CASE_DAY(13, "../data/data13.txt")
    CASE_DAY(14, "../data/data14.txt")
    CASE_DAY(15, "../data/data15.txt")
    CASE_DAY(16, "../data/data16.txt")
    CASE_DAY(17, "../data/data17.txt")
    CASE_DAY(18, "../data/data18.txt")
    CASE_DAY(19, "../data/data19.txt")
    default:
        puts("Bad exercise number");
        return 1;
    }
    #undef CASE_DAY

    putchar('\n');
    return 0;
}
