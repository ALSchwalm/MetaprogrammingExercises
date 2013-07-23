#include <iostream>
#include <array>

template<int, int, bool>
struct lesserHelper;

template<int left, int right>
struct lesserHelper<left,  right, true> {
    static const int value = left;
};

template<int left, int right>
struct lesserHelper<left, right, false> {
    static const int value = right;
};

template<int left, int right>
struct lesser : public lesserHelper<left, right, (left<right) >{};

template<int... T>
struct Least;

template<int x>
struct Least<x>{
    static const int value = x;
};

template<int x1, int x2, int... xs>
struct Least<x1, x2, xs...> : public Least<lesser<x1, x2>::value, xs...>{};

template<int...> struct Ints{};

template<typename intList, int target>
struct Remove;

template<typename unchecked, typename checked, int target, bool found>
struct Check;

template<int... ints, int target>
struct Remove<Ints<ints...>, target> : Check<Ints<ints...>, Ints<>, target, false> {};

template<typename unchecked, typename checked, int target, bool found>
struct Move;

template<int... unchecked, int... checked, int head, int target>
struct Move <Ints<head, unchecked...>, Ints<checked...>, target, true> :
    public Check< Ints<>, Ints<checked..., unchecked...>, target, true>{};

template<int... unchecked, int... checked, int head, int target>
struct Move <Ints<head, unchecked...>, Ints<checked...>, target, false> :
    public Check< Ints<unchecked...>, Ints<checked..., head>, target, false>{};

template<int... checked, int target, bool found>
struct Check <Ints<>, Ints<checked...>, target, found> {
    static const int len = sizeof...(checked);
    typedef Ints<checked...> value;
};

template<int... unchecked, int head, int... checked, int target, bool found>
struct Check <Ints<head, unchecked...>, Ints<checked...>, target, found> :
    public Move< Ints<head, unchecked...>, Ints<checked...>, target, (head==target) >{};

template<typename unsorted, typename sorted=Ints<>> struct Sort;

template<int... ints> 
struct Sort<Ints<>, Ints<ints...>>{
    typedef Ints<ints...> value;
    static const std::array<int, sizeof...(ints)> values;
};

template<int... ints> const std::array<int, sizeof...(ints)>
 Sort<Ints<>, Ints<ints...>>::values = {{ints...}};

template<int... unsortedInts, int... sortedInts, int head>
struct Sort<Ints<head, unsortedInts...>, Ints<sortedInts...>> : 
    public Sort< typename Remove< Ints<head, unsortedInts...>,
				  Least<head, unsortedInts...>::value>::value, 
		 Ints<sortedInts..., Least<head, unsortedInts...>::value>>{};


int main() {
    for (auto value: Sort< Ints<1, 3, 19, 3, -2>>::values) {
	std::cout << value << " "; //prints -2 1 3 3 19
    }
    return 0;
}
