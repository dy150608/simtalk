#include "include/tools/transform.hpp"

const char digits[] = "9876543210123456789";
const char digitsHex[] = "0123456789ABCDEF";
const char* zero = digits + 9;

// Efficient Integer to String Conversions, by Matthew Wilson.
// and I'm copy from chenshuo/muduo.git
size_t convert(char buff[], int64_t value)
{
	char* p = buff;
	int64_t i = value;
	do
	{
		int64_t lsd = static_cast<int>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	} while (i != 0);

	if (value < 0)
	{
		*p++ = '-';
	}
	*p = '\0';
	std::reverse(buff, p);

	return p - buff;
}

size_t convertHex(char buff[], uint64_t value)
{
  uint64_t i = value;
  char* p = buff;

  do
  {
    uint64_t lsd = static_cast<uint64_t>(i % 16);
    i /= 16;
    *p++ = digitsHex[lsd];
  } while (i != 0);

  *p = '\0';
  std::reverse(buff, p);

  return p - buff;
}
