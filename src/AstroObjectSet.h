/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroObjectSet.h
	Last generated: Sat Oct 23 13:49:55 1999

 *********************************************************************/

#ifndef AstroObjectSet_included
#define AstroObjectSet_included

#include "AstroObjectSetData.h"

class AstroObjectSet : public AstroObjectSetData
{
    Q_OBJECT

public:

    AstroObjectSet(const char *, int, int);
    virtual ~AstroObjectSet();

public slots:

    void okslot();
    void cancelslot();

private:

    int obj1, obj2;

};
#endif // AstroObjectSet_included
