// edaMacros.h -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_EDAMACROS_H_
#define EDA_EDAMACROS_H_

#include <iostream>

#define Data unsigned int

//define a debug print macro here

#define THIS_FILE ((strrchr(__FILE__, 0x5C) ?: __FILE__ - 1) + 1)
#define debug if (0) ; else std::cerr << THIS_FILE << "--" << __PRETTY_FUNCTION__ << ": "
#define info if (0) ; else std::cout << std::hex << THIS_FILE << ": "

#define u32 unsigned int
#define u8 unsigned char

#define rol(x,n) (x<<n | (x>>(32-n)))
#define ror(x,n) (x>>n | (x<<(32-n)))

#endif /* EDA_EDAMACROS_H_ */
