#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <iostream>
#include <iomanip>

char *itobBuf;

template< typename T >
std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << "0x"
         << std::setfill ('0') << std::setw(sizeof(T)*2)
         << std::hex << i;
  return stream.str();
}

char *int2bin(int a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = (buf_size - 1); i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

char *int_to_bin(int a, int buf_size) {
    if(itobBuf > 0) free(itobBuf);
    itobBuf = (char *)malloc((sizeof(char)*buf_size)+sizeof(char));
    itobBuf[buf_size] = '\0';

    int2bin(a, itobBuf, buf_size);

    return itobBuf;
}

#endif // UTIL_H
