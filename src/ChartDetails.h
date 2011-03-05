/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: ChartDetails.h
	Last generated: Sat Nov 13 16:01:10 1999

 *********************************************************************/

#ifndef ChartDetails_included
#define ChartDetails_included

#include "ChartDetailsData.h"

class ChartDetails : public ChartDetailsData
{
    Q_OBJECT

public:

    ChartDetails
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ChartDetails();

public slots:

    void steprateslot();
    void nbarabpartslot();
    void okslot();
    void arabpartorderslot(int);
    void starorderslot(int);
    void wheelrowslot();
    void cancelslot();

private:

    int wheelrows, astrographstep, arabicparts, starsorder, arabpartorder;

};
#endif // ChartDetails_included
