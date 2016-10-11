/* 
 * File:   utils.h
 * Author: kris
 *
 * Created on 02 November 2014, 17:02
 */

#ifndef UTILS_H
#define	UTILS_H

/**
@file Utils.h
*/

//https://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor/
//The foolowing PRNG implementation is written by whoever wrote the above article, not myself
///Define generator for intel fastrand seeder
#define FASTSRAND(GSEED) static inline void fast_srand( int seed ) { \
GSEED = seed;\
}

///Define generator for intel fastrand PRNG computer
#define FASTRAND(GSEED) static inline unsigned int fast_rand(){\
GSEED = (214013*GSEED+2531011);\
return GSEED;\
}
//End of PRNG from Intel

#endif	/* UTILS_H */

