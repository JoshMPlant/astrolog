/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: Restrictions.h
	Last generated: Sat Oct 23 15:23:22 1999

 *********************************************************************/

#ifndef Restrictions_included
#define Restrictions_included

#undef bool
#undef tr
#include "RestrictionsData.h"
extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
};

typedef byte ib[88];

class Restrictions : public RestrictionsData
{
    Q_OBJECT

public:

    Restrictions(const char *, byte*, byte*);
    virtual ~Restrictions();

public slots:

   void toglecuspslot();
   void okslot();
   void toggleminorslot();
   void copyslot();
   void toggleuranslot();
   void restrictallslot();
   void cancelslot();
   void unrestrictallslot(); 
   
private:
   byte *ib1, *ib2;
};
#endif // Restrictions_included




