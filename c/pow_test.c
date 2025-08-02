#include "stdio.h"

#define i64 long long
#define i32 int
#define b32 int


i64 pow_i64(i64 base, i64 exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    i64 res = 0;
    i64 half_exp = exp / 2;
    b32 odd_exp = exp & 1;
    i64 half_pow = pow_i64(base, half_exp);

    return half_pow * half_pow * ((odd_exp) ? base : 1);
}


int main() {
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

    return 0;
}