#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
void swap(int32_t  &a, int32_t  &b)  {
    int32_t c=b;
     b=a;
     a=c;
}

struct cool {
    int32_t x=0;
    std::vector<int32_t >a={1};
    void inc(int32_t i)  {
        x += i;
        return;
    }
;
};
int32_t main()  {
    int32_t f=-3;
    int32_t d=-2;
    swap(f, d);
    std::cout << f << std::endl;
    cool a;
    a.inc(1);
    std::cout << a.a[0] << std::endl;
    int32_t* c=new int32_t(1);
    std::cout << *c << std::endl;
    delete c;
    return 0;
}

