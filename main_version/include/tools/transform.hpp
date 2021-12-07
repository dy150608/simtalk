#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

namespace simtalk
{

namespace tools
{

/*
 * convert integer to dec-string
 * [in 1]: storage buffer
 * [in 2]: integer need to convert
 * [out]: length of dec-string
 */
size_t convert(char buff[], int64_t value);

/*
 * convert unsigned integer to HEX-string
 * params like: size_t convert(char buff[], int64_t value);
 */
size_t convertHex(char buff[], uint64_t value);

}// namespace:tools
}// namespace:simtalk
#endif// TRANSFORM_HPP
