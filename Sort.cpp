#include <iostream>
#include <array>

template<int, int, bool>
struct lesserHelper;

//If left is less than right
template<int left, int right>
struct lesserHelper<left,  right, true> {
    static const int value = left;
};

//If left is not less than right
template<int left, int right>
struct lesserHelper<left, right, false> {
    static const int value = right;
};

//Compare left and right, lesserHelper statically initialize the correct value
template<int left, int right>
struct lesser : public lesserHelper<left, right, (left<right) >{};

template<int... T>
struct Least;

//Base of Least. 
template<int x>
struct Least<x>{
    static const int value = x;
};

//The least value in a set is the Least of the lesser of the first two and the rest of the set
template<int x1, int x2, int... xs>
struct Least<x1, x2, xs...> : public Least<lesser<x1, x2>::value, xs...>{};

//Ints is used as a container for sets of ints
template<int...> struct Ints{};

template<typename intList, int target>
struct Remove;

template<typename unchecked, typename checked, int target, bool found>
struct Check;


//Interface for Remove. 
template<int... ints, int target>
struct Remove<Ints<ints...>, target> : Check<Ints<ints...>, Ints<>, target, false> {};


template<typename unchecked, typename checked, int target, bool found>
struct Move;


//If the target was found by Check, drop it.
template<int... unchecked, int... checked, int head, int target>
struct Move <Ints<head, unchecked...>, Ints<checked...>, target, true> :
    public Check< Ints<>, Ints<checked..., unchecked...>, target, true>{};


//If the target was no found by Check, just move the head to the checked set
template<int... unchecked, int... checked, int head, int target>
struct Move <Ints<head, unchecked...>, Ints<checked...>, target, false> :
    public Check< Ints<unchecked...>, Ints<checked..., head>, target, false>{};


//Base case of check. Stop checking for the target when the unchecked set is empty
template<int... checked, int target, bool found>
struct Check <Ints<>, Ints<checked...>, target, found> {
    static const int len = sizeof...(checked);
    typedef Ints<checked...> value;
};

//Check to see if the head is the target. Call Move with a corresponding bool
template<int... unchecked, int head, int... checked, int target, bool found>
struct Check <Ints<head, unchecked...>, Ints<checked...>, target, found> :
    public Move< Ints<head, unchecked...>, Ints<checked...>, target, (head==target) >{};


template<typename unsorted, typename sorted=Ints<>> struct SortHelper;

//Base case. Stop sorting when the unsorted set is empty
template<int... ints> 
struct SortHelper<Ints<>, Ints<ints...>>{
    typedef Ints<ints...> value;
    static const std::array<int, sizeof...(ints)> values;
};

template<int... ints> const std::array<int, sizeof...(ints)>
 SortHelper<Ints<>, Ints<ints...>>::values = {{ints...}};


/* 
 * Perform the sorting algorithm. It works as follows.
 * 
 * UnsortedList = [1, 3, 18, 3, -2]; SortedList = []
 * SortedList += Least(UnsortedList)
 * Remove(UnsortedList, Least(UnsortedList))
 * Repeat while UnsortedList.size > 0
 */
template<int... unsortedInts, int... sortedInts, int head>
struct SortHelper<Ints<head, unsortedInts...>, Ints<sortedInts...>> : 
    public SortHelper< typename Remove< Ints<head, unsortedInts...>,
				  Least<head, unsortedInts...>::value>::value, 
		 Ints<sortedInts..., Least<head, unsortedInts...>::value>>{};

//Interface for sort, prevents the user from having to call Sort< Ints<...>>
template<int... intList>
struct Sort : SortHelper< Ints<intList...> >{};


int main() {
    for (auto value: Sort< 1, 3, 19, 3, -2>::values) {
	std::cout << value << " "; //prints -2 1 3 3 19
    }
    return 0;
}
