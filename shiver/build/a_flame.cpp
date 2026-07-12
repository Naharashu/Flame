#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
#include <memory>
struct point {
    int32_t x=1;
    int32_t y=1;
};
float test(const float &a)  {
    return a;
}

int32_t main()  {
    std::unique_ptr<std::string> ass=std::make_unique<std::string>("hi");
    return 0;
}

