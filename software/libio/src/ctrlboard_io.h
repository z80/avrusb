
#ifndef __CTRLBOARD_IO_H_
#define __CTRLBOARD_IO_H_

#include "lowlevel_io.h"
#include "ctrlboard_functions.h"
#include <string>

class CtrlboardIo: public LowlevelIo
{
public:
    typedef std::basic_string< unsigned char > TIo;

    CtrlboardIo();
    ~CtrlboardIo();

    int version();

};



#endif


