/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: SetColors.h
	Last generated: Mon Oct 18 13:01:08 1999

 *********************************************************************/

#ifndef SetColors_included
#define SetColors_included

#include "SetColorsData.h"
extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
#undef bool
}

class SetColors : public SetColorsData
{
    Q_OBJECT

public:

    SetColors
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~SetColors();
    inline ColorVector& thecv() { return *cv; }

public slots:

    void okslot();
    void cancelslot(); 
    void elementsslot(int);
    void aspectsslot(int);
    void tableslot(int);

private:

    KI LocalElemB[4], LocalAspB[cAspect+1];
};
#endif // SetColors_included
