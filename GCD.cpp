#include <iostream>

// Generic template, undefined because it should never be used
template <unsigned int one, unsigned int two, bool base = false>
struct GCD;

// Base case, the GCD has been found
template <unsigned int one, unsigned int two>
struct GCD<one, two, true> {
    enum { value = two + 1 };
};

// Core of the algorithm, count down from either one of the numbers
// trying to divide it into the other each time, the inheritance loop
// is broken when a factor is found.
template <unsigned int one, unsigned int two>
struct GCD<one, two, false> : public GCD<one, two - 1, (one % two == 0)> {};

int main() {
    std::cout << GCD<30, 3>::value << std::endl; // prints 3
    std::cout << GCD<2, 1>::value << std::endl;  // prints 1
    return 0;
}
