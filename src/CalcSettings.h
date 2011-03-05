/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: CalcSettings.h
	Last generated: Mon Oct 25 15:06:02 1999

 *********************************************************************/

#ifndef CalcSettings_included
#define CalcSettings_included

#include "CalcSettingsData.h"
extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
}

class CalcSettings : public CalcSettingsData
{
    Q_OBJECT

public:

    CalcSettings
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CalcSettings();

private:
    
    int scs, zodoffset, nbasp, centplan, nbharmonic, screenwidth, solchartset, dispform;
    void FillPlanets(QComboBox&);

public slots:

    void okslot();
    void cancelslot();
    void nbaspslot();
    void harmchartfactslot();
    void textcolslot();
    void solchartsettslot(int);
    void useplanslot(int);
    void displayformatslot(int);
    void ayanamsaslot(int);
    void centplanslot(int);
};
#endif // CalcSettings_included





