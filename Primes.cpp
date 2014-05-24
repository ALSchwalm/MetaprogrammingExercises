#include <iostream>
#include <array>
#include <type_traits>

//wrapper for ints
template<unsigned int... ints> struct Ints{};

//FactorsHelper generic
template<typename factorList, unsigned int, unsigned int>
struct FactorsHelper;

//Generic Move
template<typename valueList, unsigned int, unsigned int, bool>
struct Move;

//The value is a factor, so append it to the list
//and proceed to the next value
template<unsigned int... valueList, unsigned int begin, unsigned int possibleFactor>
struct Move<Ints<valueList...>, begin, possibleFactor, true> :
    FactorsHelper<Ints<valueList..., possibleFactor>, possibleFactor-1, begin>{};

//The value is not a factor so drop it
//and proceed to the next value
template<unsigned int... valueList, unsigned int begin, unsigned int possibleFactor>
struct Move<Ints<valueList...>, begin, possibleFactor, false> :
    FactorsHelper<Ints<valueList...>, possibleFactor-1, begin>{};

//Base case of factor checking. If the current value is 1, drop it and stop
template<unsigned int... factorList, unsigned int begin>
struct FactorsHelper<Ints<factorList...>, 1, begin>{
    using value = Ints<factorList...>;
    static constexpr std::array<unsigned int, sizeof...(factorList)> values = {{factorList...}};
};

//Performs the bulk of the Factor work. Moves the current value to the factorList
//if the start value is divisible by it (i.e. if it is a factor)
template<unsigned int... factorList, unsigned int possibleFactor, unsigned int begin>
struct FactorsHelper<Ints<factorList...>, possibleFactor, begin> :
    Move<Ints<factorList...>, begin, possibleFactor, (begin%possibleFactor == 0)>{};

//determine the factors of a number other than itself and 1
template<unsigned int begin>
struct Factors : FactorsHelper<Ints<>, begin/2, begin>{};


//Generic case of remove
template<typename intList, bool>
struct Remove;

//Generic case of PrimeHelper
template<typename intList>
struct PrimeHelper;

//If the bool is false, then the head of the list is non-prime,
//so we drop it an check the head-1
template<unsigned int... intList, unsigned int head>
struct Remove<Ints<head, intList...>, false> : public
PrimeHelper<Ints<head-1, intList...>>{};

//The head of the list is prime, so keep it and check the next value
template<unsigned int... intList, unsigned int head>
struct Remove<Ints<head, intList...>, true> : public
PrimeHelper<Ints<head-1, head, intList...>>{};

//special case for 0
template<>
struct PrimeHelper<Ints<0>> {
    using value = Ints<>;
    static constexpr std::array<unsigned int, 0> values = {};
};

//special case for 1
template<>
struct PrimeHelper<Ints<1>> {
    using value = Ints<>;
    static constexpr std::array<unsigned int, 0> values = {};
};

//Base case of PrimeHelper, if the head is two, stop
template<unsigned int... intList>
struct PrimeHelper< Ints<2, intList...> > {
    using value = Ints<2, intList...>;
    static constexpr std::array<unsigned int, sizeof...(intList)+1> values = {{2, intList...}};
};

//Bulk of the prime work. Remove the head from the prime list if its factors are non-empty
template<unsigned int... intList, unsigned int head>
struct PrimeHelper<Ints<head, intList...>> :
    public Remove<Ints<head, intList...>,
                  std::is_same<typename Factors<head>::value, Ints<>>::value>{};

//Interaface for primes
template<unsigned int upperBound>
struct Primes : public PrimeHelper<Ints<upperBound>>{};

int main() {
    constexpr auto primes = Primes<20>::values;
    static_assert(primes.size() == 8, "Incorrect number of primes.");
    static_assert(primes[0] == 2  &&
                  primes[1] == 3  &&
                  primes[2] == 5  &&
                  primes[3] == 7  &&
                  primes[4] == 11 &&
                  primes[5] == 13 &&
                  primes[6] == 17 &&
                  primes[7] == 19, "Incorrect value of primes");
}
