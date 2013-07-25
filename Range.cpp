#include <array>
#include <iostream>

template<unsigned int... ints> struct Ints{};

template<typename intList, bool one>
struct RangeHelper;

template<unsigned int... ints>
struct RangeHelper<Ints<ints...>, true>{
    typedef Ints<ints...> value;
    static const std::array<unsigned int, sizeof...(ints)> values; 
};

template<unsigned int... ints> 
const std::array<unsigned int, sizeof...(ints)> 
RangeHelper<Ints<ints...>, true>::values = {{ints...}};

template<unsigned int... ints, unsigned int head>
struct RangeHelper<Ints<head, ints...>, false> :
    public RangeHelper<Ints<head-1, head, ints...>, head-1==1>{};

template<unsigned int bound>
struct Range : public RangeHelper<Ints<bound>, bound==1>{};

int main() {
    for (auto value : Range<10>::values) {
	std::cout << value << " ";
    }
    return 0;
}
