// edaMacros.h -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_MACROS_H_
#define EDA_MACROS_H_

#include <iostream>
#include <cstring>

#define DATA_PATH "Z:\\EDA\\eda-reversing\\data\\"
//#define DATA_PATH "D:\\eworkspace\\eda\\Debug\\data\\"

#define Data unsigned int

//define a debug print macro here

#define THIS_FILE ((strrchr(__FILE__, 0x5C) ?: __FILE__ - 1) + 1)
#define debug if (0) ; else std::cerr << THIS_FILE << "--" << __PRETTY_FUNCTION__ << ": "
#define info if (0) ; else std::cout << std::hex << THIS_FILE << ": "

#define u32 unsigned int
#define u8 unsigned char

#define rol(x,n) (x<<n | (x>>(32-n)))
#define ror(x,n) (x>>n | (x<<(32-n)))


#endif /* EDA_MACROS_H_ */
