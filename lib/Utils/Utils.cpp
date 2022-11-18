#include <sstream>
#include <string>

void reset(std::stringstream& stream)
{
    const static std::stringstream initial;
    
    stream.str(std::string());
    stream.clear();
    stream.copyfmt(initial);
}