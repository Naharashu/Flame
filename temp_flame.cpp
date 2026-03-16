#include <iostream>
#include <cstdint>
#include <iostream>
#include <cstdint>
int64_t abs(int64_t a)  {
    if((a > 0)) {
        return (a);
    };
    return (-a);
}
double sqrt(double a)  {
    double x=1.000000;
     for(int32_t i=0;(i <= 6);i++) {
         x=(0.500000 * (x + (a / x)));
    };
    return (x);
}
double floor(double a)  {
    int64_t res=a;
    if(((a < 0) && (a != res))) {
        return ((res - 1));
    };
    return (res);
}
double fmod(double a, double b)  {
    double res=(a - (b * floor((a / b))));
    return (res);
}
double pow(double a, int32_t b)  {
    double res=a;
     for(int32_t i=1;(i < b);i++) {
        res *= a;
    };
    return (res);
}
int32_t add(int32_t a, int32_t b)  {
    return ((a + b));
}
int32_t main()  {
    int32_t a[3];
    int32_t i=2;
    std::cout << a[i] << std::endl;
    return (0);
}
