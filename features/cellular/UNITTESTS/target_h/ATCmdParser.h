#ifndef __AT_CMD_PARSER_H__
#define __AT_CMD_PARSER_H__

#include "mbed.h"
#include <cstdarg>
#include "FileHandle.h"

class ATCmdParser
{
public:
    ATCmdParser(mbed::FileHandle *fh, const char *output_delimiter = "\r",
                 int buffer_size = 256, int timeout = 8000, bool debug = false){}

    ~ATCmdParser(){}
};

#endif //__AT_CMD_PARSER_H__

