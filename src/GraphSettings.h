/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: GraphSettings.h
	Last generated: Tue Nov 16 15:08:26 1999

 *********************************************************************/

#ifndef GraphSettings_included
#define GraphSettings_included

#define real double

#include "GraphSettingsData.h"

class GraphSettings : public GraphSettingsData
{
    Q_OBJECT

public:

    GraphSettings
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~GraphSettings();

public slots:

    void vchartsizeslot();
    void mapdegrotslot();
    void globetiltslot();
    void okslot();
    void hchartsizeslot();
    void animdelslot();
    void cancelslot();
    void wheelchartslot(int);
    void planetslot(int);

private:

    int xWin, yWin, Rot, ObjLeft, Delay;
    real Tilt;

};
#endif // GraphSettings_included
