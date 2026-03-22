#include <iostream>
#include <cstdint>
#include <array>
int32_t add(int32_t a, int32_t b)  {
    return ((a + b));
}
int32_t add_(std::array<int32_t ,3>a)  {
    return ((a[0] + a[1]));
}
int32_t main()  {
    std::array<int32_t ,3>i={1, 2, 3};
    std::cout << add_(i) << std::endl;
    std::cout << "Hi ^^" << std::endl;
    return (0);
}
