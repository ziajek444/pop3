#ifndef BASE64_H_INCLUDED
#define BASE64_H_INCLUDED
//Marcin Ziajkowski
#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_encode(std::string bytes_to_encode);
std::string base64_decode(std::string const& s);

#endif // BASE64_H_INCLUDED


