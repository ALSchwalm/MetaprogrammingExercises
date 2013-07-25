#include <iostream>
#include <array>

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

//------------------------------------

template<typename factorList, unsigned int, unsigned int>
struct FactorsHelper;

template<typename valueList, unsigned int, unsigned int, bool>
struct Move;

template<unsigned int... valueList, unsigned int begin, unsigned int possibleFactor>
struct Move<Ints<valueList...>, begin, possibleFactor, true> :
  FactorsHelper<Ints<valueList..., possibleFactor>, possibleFactor-1, begin>{};

template<unsigned int... valueList, unsigned int begin, unsigned int possibleFactor>
struct Move<Ints<valueList...>, begin, possibleFactor, false> :
  FactorsHelper<Ints<valueList...>, possibleFactor-1, begin>{};


template<unsigned int... factorList, unsigned int begin>
struct FactorsHelper<Ints<factorList...>, 1, begin>{
  typedef Ints<factorList...> value;
  static const std::array<unsigned int, sizeof...(factorList)> values;
};

template<unsigned int... factorList, unsigned int begin>
const std::array<unsigned int, sizeof...(factorList)> 
FactorsHelper<Ints<factorList...>, 1, begin>::values = {{factorList...}};


template<unsigned int... factorList, unsigned int possibleFactor, unsigned int begin>
struct FactorsHelper<Ints<factorList...>, possibleFactor, begin> :
  Move<Ints<factorList...>, begin, possibleFactor, (begin%possibleFactor == 0)>{};

//determine the factors of a number other than itself and 1
template<unsigned int begin>
struct Factors : FactorsHelper<Ints<>, begin/2, begin>{};

//-------------------------------------------------

/*
template<unsigned int bound>
struct Primes : public PrimeHelper< Range<bound>::value>{};
*/
int main() {
    for( auto value : Factors<20>::values) {
	std::cout << value << " "; //prints 1 2 3 4 5 6 7 8 9 10
    }
}
