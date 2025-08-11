#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define GB_IMPLEMENTATION
#include "gb.h"


#define FOR(idx, start, end) for(i32 idx = (start);idx < (end);idx += 1)
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

i64 pow_i64(i64 base, i64 exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    i64 res = 0;
    i64 half_exp = exp / 2;
    b32 odd_exp = exp & 1;
    i64 half_pow = pow_i64(base, half_exp);

    return half_pow * half_pow * ((odd_exp) ? base : 1);
}

gbString read_entire_file(FILE* f, gbAllocator a) {
    fseek(f, 0, SEEK_END);
    i64 file_len = ftell(f);
    fseek(f, 0, SEEK_SET);

    gbString data = gb_string_make_length(a, NULL, file_len);
    fread(data, 1, file_len, f);

    return data;
}

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

void DAY(11, f, a, part1) {
    gbArray(u64) stones;
    gbArray(u64) next_stones;
    gb_array_init(stones, a);
    gb_array_init(next_stones, a);

    while(!feof(f)) {
        i32 num;
        fscanf(f, "%i", &num);
        gb_array_append(stones, num);
    }

    // FOR(i, 0, gb_array_count(stones)) printf("%i ", stones[i]);
    i32 num_blinks = part1 ? 25 : 75;

    FOR(time, 0, num_blinks) {
        printf("time %i, num stones %i\n", time, gb_array_count(stones));

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

    printf("%i", gb_array_count(stones));
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
        FILE* f = fopen(datapath, "r"); \
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
    default:
        puts("Bad exercise number");
        return 1;
    }
    #undef CASE_DAY

    putchar('\n');
    return 0;
}
