#include <iostream>

template<int one, int two, bool base>
struct GCDHelper;

template<int one, int two>
struct GCDHelper<one,two,true>
{
  static const int value = two+1;
};

template<int one, int two>
struct GCDHelper<one, two, false> : 
  public GCDHelper<one, two-1, (one%two==0)>{};

template<int one, int two>
struct GCD : public GCDHelper<one ,two, false>{};

int main()
{
  std::cout << GCD<30, 3>::value << std::endl;   //prints 3
  std::cout << GCD<17, 19>::value << std::endl;  //prints 1
  return 0;
}

