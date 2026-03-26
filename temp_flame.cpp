#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
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
double floor_helper(double a)  {
    int64_t res=a;
    if(((a < 0) && (a != res))) {
        return ((res - 1));
    };
    return (res);
}
double floor(double a)  {
    if(((a - floor_helper(a)) < 0.500000)) {
        return (floor_helper(a));
    };
    return (floor_helper((a + 1)));
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
int32_t add_(std::array<int32_t ,3>a)  {
    return ((a[0] + a[1]));
}
std::array<int32_t ,3>array_()  {
    std::array<int32_t ,3>a={1, 2, 3};
    return (a);
}
int32_t main()  {
    std::array<int32_t ,3>i=array_();
    std::array<int32_t ,2>j={1, 2};
    int32_t f=-3;
    std::cout << floor(3.900000) << std::endl;
    std::vector<int32_t >a={1, 2, 3};
    a.emplace_back(4);
    a.pop_back();
    std::cout << a.at(3) << std::endl;
    return (0);
}
