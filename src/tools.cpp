#include "tools.hpp"
#include <cstddef>
#include <cstdint>

/**

  @p '__s'
  @brief function that @return's len of a 'const char *'
 
 */
size_t slen(const char *__s)
{
    size_t i(0);
    for(; __s[i]; ++i){}
    return i;
}

int explicit_strcmp(const char *__s1, const char *__s2)
{
    while (*__s1 && (*__s1 == *__s2))
    {
        ++__s1;
        ++__s2;
    }
    return (uint8_t)(*__s1) - (uint8_t)(*__s2);
}

uint8_t scmp(const char *__s, const char *__s2)
{
    uint8_t result = true;
    for (size_t i = 0; *__s; ++i)
    {
        if (__s[i] != __s2[i])
        {
            result = false;
            break;
        }
    }
    return result;
}
