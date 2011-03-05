/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: SetAstroColor.h
	Last generated: Thu Oct 14 15:27:48 1999

 *********************************************************************/

#ifndef SetAstroColor_included
#define SetAstroColor_included

#include "SetAstroColorData.h"
#include "AstroColor.h"

class SetAstroColor : public SetAstroColorData
{
    Q_OBJECT

public:

    SetAstroColor
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~SetAstroColor();
    
    //inline QRadioButton* getRb(int i) { return cv->rb(i); };
    //inline QLabel*      getLb(int i) { return cv->lb(i); };

public slots:

    void tableslot(int);
    void colorlistslot(int);
    void okslot();
    void cancelslot();

private:

    AstroColor *LocalTable[16]; // A localcopy of Acl

};
#endif // SetAstroColor_included
