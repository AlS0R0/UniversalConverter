#ifndef BASECONVERTER_H
#define BASECONVERTER_H

#include <string>

class BaseConverter
{
public:
    static bool convert(const std::string& input, int p, int q,
                        std::string& output, std::string& warning);
};

#endif // BASECONVERTER_H
