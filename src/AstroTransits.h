/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroTransits.h
	Last generated: Wed Nov 3 14:34:27 1999

 *********************************************************************/

#ifndef AstroTransits_included
#define AstroTransits_included

#include "AstroTransitsData.h"

extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
}

class AstroTransits : public AstroTransitsData
{
    Q_OBJECT

public:

    AstroTransits
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~AstroTransits();

    bool print; // test if we must print the corresponding chart

public slots:

    void okslot();
    void cancelslot();
    void timeslot();
    void nowslot();
    void dayslot();
    void divisionslot();
    void transtypeslot(int);
    void searchforhitsslot(int);
    void nbyearsslot();
    void yearslot();

private:

    int transtype, searchforhits, nbyears, divisions;
    CI myci;
};
#endif // AstroTransits_included
