#ifndef BASECONVERTER_H
#define BASECONVERTER_H

#include <string>
#include "bigfraction.h"

class BaseConverter
{
public:
    static bool convert(const std::string& input, int p, int q,
                        std::string& output, std::string& error);
};

#endif // BASECONVERTER_H
