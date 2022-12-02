#ifndef _UTILS_H
#define _UTILS_H

#include <sstream>
#include <string>

void reset(std::stringstream &stream);

template<typename ... Args>
std::string formatString(const std::string &format, Args ... args);

#endif /* _UTILS_H */
