# 1 "a.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "a.cpp"
# 1 "a.hpp" 1

# 1 "b.hpp" 1



# 1 "a.hpp" 1
# 9 "a.hpp"
class a
{
public:
 void init();
private:
 b _b;
};
# 5 "b.hpp" 2

class b
{
public:
 friend class a;
};
# 3 "a.hpp" 2
class b;
# 2 "a.cpp" 2


int main()
{
 a _a;
 return 0;
}
