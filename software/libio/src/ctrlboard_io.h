
#ifndef __CTRLBOARD_IO_H_
#define __CTRLBOARD_IO_H_

#include "lowlevel_io.h"
#include "config.h"
#include <string>

class CtrlboardIo: public LowlevelIo
{
public:
    typedef std::basic_string< unsigned char > TIo;

    CtrlboardIo();
    ~CtrlboardIo();

    int version( std::string & ver );
    int firmware( std::string & fir );

};



#endif


