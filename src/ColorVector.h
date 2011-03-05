/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: ColorVector.h
	Last generated: Thu Oct 14 14:53:36 1999

 *********************************************************************/

#ifndef ColorVector_included
#define ColorVector_included

#include "ColorVectorData.h"
#include "AstroColor.h"

class ColorVector : public ColorVectorData
{
    Q_OBJECT

public:

    ColorVector(QWidget* parent = NULL, const char* name = NULL);
    virtual ~ColorVector();
    ColorVector& operator << (int); // Ckecks the QRadioButton n to 1
    void init(AstroColor&, int); // Initializes the color table n to th given Astro Color
    int checked(); // Gives the checked button, or -1
    inline QButtonGroup* t()       { return Table; }; // JUst the the connect instruction 

public slots:

   void tableslot(int);

};
#endif // ColorVector_included
