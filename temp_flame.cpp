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
int32_t main()  {
    std::cout << sqrt(abs(-25)) << std::endl;
    int32_t a=sizeof(3);
    std::cout << a << std::endl;
    return (0);
}
