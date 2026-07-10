#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
namespace stdlib {
#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
int64_t abs(int64_t a)  {
    if((a > 0)) {
        return a;
    };
    return -a;
}

const double PI=3.140000;
double sqrt(double a)  {
    double x=1.000000;
     for(int32_t i=0;(i <= 6);i++) {
        x=(0.500000 * (x + (a / x)));
    };
    return x;
}

double floor_helper(double a)  {
    int64_t res=a;
    if(((a < 0) && (a != res))) {
        return (res - 1);
    };
    return res;
}

double floor(double a)  {
    if(((a - floor_helper(a)) < 0.500000)) {
        return floor_helper(a);
    };
    return floor_helper((a + 1));
}

double fmod(double a, double b)  {
    double res=(a - (b * floor((a / b))));
    return res;
}

double pow(double a, int32_t b)  {
    double res=a;
     for(int32_t i=1;(i < b);i++) {
        res *= a;
    };
    return res;
}

double fshiftr(double a, int8_t n)  {
    return (a / pow(2, n));
}

double fshiftl(double a, int8_t n)  {
    return (a * pow(2, n));
}

void println()  {
    std::cout << "\n";
    return;
}

}
namespace strings {
#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
uint32_t len(std::string s)  {
    uint32_t len=0;
    while((s[len] != 0))  {
        len++;
    };
    return len;
}

std::string concat(std::string s1, std::string s2)  {
    std::string s3=s1;
    s3 += s2;
    return s3;
}

std::string reversed(std::string s)  {
    uint32_t size=len(s);
    std::string rev="";
     for(int32_t i=(size - 1);(i >= 0);i--) {
        rev += s[i];
    };
    return rev;
}

std::string substr(std::string s, int64_t start, int64_t length)  {
    uint32_t size=len(s);
    if((((start < 0) || (length < 0)) || ((start + length) > size))) {
        return "";
    };
    if(((start == 0) && (length == size))) {
        return s;
    };
    std::string sub="";
     for(uint64_t i=start;(i < (start + length));i++) {
        sub += s[i];
    };
    return sub;
}

int64_t indexOf(std::string s, std::string sub)  {
    uint32_t size=len(s);
    uint32_t ssize=len(sub);
    if(((size == 0) || (size < ssize))) {
        return -1;
    };
     for(uint64_t i=0;(i < size);i++) {
        bool match=1;
         for(uint64_t j=0;(j < ssize);j++) {
            if((s[i] != sub[j])) {
                match=0;
                break;
            };
        };
        if(match) {
            return i;
        };
    };
    return -1;
}

bool contains(std::string s, std::string sub)  {
    bool cont=(indexOf(s, sub) != -1);
    return cont;
}

}
void test(std::string s)  {
    s += "a";
}

int32_t main()  {
    std::string ass="Hello world";
    std::cout << strings::contains(ass, "world");
    std::cout << (ass + 3);
    return 0;
}

