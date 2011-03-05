/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroSetAspects.h
	Last generated: Wed Oct 20 14:37:06 1999

 *********************************************************************/

#ifndef AstroSetAspects_included
#define AstroSetAspects_included

#include "AstroSetAspectsData.h"
extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
}

#undef bool

class AstroSetAspects : public AstroSetAspectsData
{
    Q_OBJECT

public:

    AstroSetAspects
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~AstroSetAspects();

public slots:

    void okslot();
    void cancelslot();
    void restrictallslot();
    void unrestrictallslot(); 
    void togglemajorslot();
    void asslot(AspectSet*, bool);

private:

    bool refresh;

};
#endif // AstroSetAspects_included
