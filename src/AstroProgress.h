/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroProgress.h
	Last generated: Sat Nov 6 15:32:47 1999

 *********************************************************************/

#ifndef AstroProgress_included
#define AstroProgress_included

#include "AstroProgressData.h"

extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
}

class AstroProgress : public AstroProgressData
{
    Q_OBJECT

public:

    AstroProgress
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~AstroProgress();
    bool prog;
public slots:

    void timeslot();
    void okslot();
    void nowslot();
    void progsetslot(int);
    void dayslot();
    void cancelslot();
    void degperdayslot(int);
    void yearslot();

private:

    CI myci;
    int degperday;
    _bool doprog, solarc;

};
#endif // AstroProgress_included
