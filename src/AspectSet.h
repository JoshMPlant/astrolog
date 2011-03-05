/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AspectSet.h
	Last generated: Tue Oct 19 15:02:59 1999

 *********************************************************************/

#ifndef AspectSet_included
#define AspectSet_included

#include "AspectSetData.h"

#define real double

class AspectSet : public AspectSetData
{
    Q_OBJECT

public:

    AspectSet
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );
    void Set(bool, const char*, real, real, real);  
    void Get(bool*, real*, real*, real*);
    inline void Toggle(bool b) { AspName->setChecked(b); }
    inline bool isToggled() { return AspName->isChecked(); }; //On(); };
    virtual ~AspectSet();

signals:

    void assignal(AspectSet*, bool);

public slots:
    
    void orbslot();
    void influenceslot();
    void angleslot();
    void aspnameslot();

private:

    real orb, angle, influence;
};
#endif // AspectSet_included




