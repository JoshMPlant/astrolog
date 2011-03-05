/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 

// This file just define the transl() macros for the *Data.cpp file

#ifndef TRANSL
#define TRANSL
extern "C"
{
char *getlocale(const char*);
#define transl(c) getlocale(c) 
};
#endif
