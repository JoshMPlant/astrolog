/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: ObscureSettings.h
	Last generated: Thu Oct 28 13:40:55 1999

 *********************************************************************/

#ifndef ObscureSettings_included
#define ObscureSettings_included


// must undef, otherwise getlocale isn't defined
#undef bool
#undef tr
#include "ObscureSettingsData.h"
extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
}

class ObscureSettings : public ObscureSettingsData
{
    Q_OBJECT

public:

    ObscureSettings
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~ObscureSettings();

public slots:

    void okslot();
    void cancelslot();
    void caprslot(int);
    void uraslot(int);
    void plutslot(int);
    void lilislot(int);
    void nbcellsslot();
    void orientslot(int);
    void restslot(int);

private:

    int glyph, orient, nbcells;
    byte igno[4];

};
#endif // ObscureSettings_included


