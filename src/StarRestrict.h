/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: StarRestrict.h
	Last generated: Mon Oct 25 13:54:25 1999

 *********************************************************************/

#ifndef StarRestrict_included
#define StarRestrict_included

#include "StarRestrictData.h"

class StarRestrict : public StarRestrictData
{
    Q_OBJECT

public:

    StarRestrict
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~StarRestrict();

public slots:

void okslot();
void restrictslot();
void unrestrictslot();
void cancelslot(); 

};
#endif // StarRestrict_included
