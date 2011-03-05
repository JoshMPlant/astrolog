/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: ObjectSet.h
	Last generated: Fri Oct 22 14:44:46 1999

 *********************************************************************/

#ifndef ObjectSet_included
#define ObjectSet_included

#include "ObjectSetData.h"

#define real double

class ObjectSet : public ObjectSetData
{
    Q_OBJECT

public:

    ObjectSet
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );
    void Set(const char *, real, real, real);
    void Get(real*, real*, real*);
    void Disable();
    virtual ~ObjectSet();

public slots:

    void Influenceslot();
    void maxorbslot();
    void addslot();

private:

    real influence, maxorb, add;
};
#endif // ObjectSet_included




