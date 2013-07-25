#include <iostream>
#include <array>
#include <type_traits>

template<unsigned int... ints> struct Ints{};

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

template<typename intList, bool>
struct Remove;

template<typename intList>
struct PrimeHelper;

template<unsigned int... intList, unsigned int head>
struct Remove<Ints<head, intList...>, false> : public
    PrimeHelper<Ints<head-1, intList...>>{};

template<unsigned int... intList, unsigned int head>
struct Remove<Ints<head, intList...>, true> : public
    PrimeHelper<Ints<head-1, head, intList...>>{};

template<unsigned int... intList>
struct PrimeHelper< Ints<2, intList...> > {
    typedef Ints<2, intList...> value;
    static const std::array<unsigned int, sizeof...(intList)+1> values;
};

template<unsigned int... intList>
const std::array<unsigned int, sizeof...(intList)+1>
    PrimeHelper< Ints<2, intList...> >::values = {{2, intList...}};


template<unsigned int... intList, unsigned int head>
struct PrimeHelper<Ints<head, intList...>> : 
    public Remove<Ints<head, intList...>,
		  std::is_same<typename Factors<head>::value, Ints<>>::value>{};



template<unsigned int bound>
struct Primes : public PrimeHelper<Ints<bound>>{};

int main() {
    for( auto value : Primes<20>::values) {
	std::cout << value << " "; //prints 2 3 5 7 11 13 17 19
    }
}
