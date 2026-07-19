#include <stdio.h>
#include <stdint.h>
#include <oxygen_runtime.h>
int64_t stdlib_flame__abs(int64_t a)  {
    if((a > 0)) {
        return a;
    };
    return -a;
}

const double PI=3.140000;
double stdlib_flame__sqrt(double a)  {
    double x=1.000000;
     for(int32_t i=0;(i <= 6);i++) {
        x=(0.500000 * (x + (a / x)));
    };
    return x;
}

double stdlib_flame__floor_helper(double a)  {
    double res=a;
    if(((a < 0) && (a != res))) {
        return (res - 1);
    };
    return res;
}

double stdlib_flame__floor(double a)  {
    if(((a - stdlib_flame_._floor_helper(a)) < 0.500000)) {
        return stdlib_flame_._floor_helper(a);
    };
    return stdlib_flame_._floor_helper((a + 1));
}

double stdlib_flame__fmod(double a, double b)  {
    double res=(a - (b * stdlib_flame_._floor((a / b))));
    return res;
}

double stdlib_flame__pow(double a, int8_t b)  {
    double res=a;
     for(int32_t i=1;(i < b);i++) {
        res *= a;
    };
    return res;
}

double stdlib_flame__fshiftr(double a, int8_t n)  {
    return (a / stdlib_flame_._pow(2, n));
}

double stdlib_flame__fshiftl(double a, int8_t n)  {
    return (a * stdlib_flame_._pow(2, n));
}

void stdlib_flame__println(string s)  {
    return;
}

typedef struct point {
    int32_t x;
    int32_t y;
} point;
point point_flame_def_init() {
    point Temp;
    Temp.x = 1;
    Temp.y = 1;
    return Temp;
};
float test(float * a)  {
    return *a;
}

int32_t main()  {
    int32_t* t = malloc(sizeof(int32_t));
    *t = 3;
    point p = point_flame_def_init();
    int32_t x[3] = {1, 2, 3};
    string s;
    oxygen_new_string( &s, "hi");
    int32_t a=10;
    a ^= 20;
    oxygen_string_destroy(&s);
    free(t);
    return a;
}

