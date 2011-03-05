/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: ColorVector.cpp
	Last generated: Thu Oct 14 14:53:36 1999

 *********************************************************************/

#include "ColorVector.h"
#include "AstroColor.h"

extern AstroColorList *Acl;

#define Inherited ColorVectorData

ColorVector::ColorVector(QWidget* parent, const char* name) : Inherited( parent, name )
{
}

ColorVector& ColorVector::operator << (int i)
{
  b[i]->setChecked(TRUE);
}


ColorVector::~ColorVector()
{
}

void ColorVector::init(AstroColor& ac, int i)
{
  l[i]->setBackgroundColor(ac);
  b[i]->setText(ac.name);
}

int ColorVector::checked()
{
  int i;
  for(i = 0; i <= 15; i++)
    if(b[i]->isChecked())
      return i;
  return -1;
}
void ColorVector::tableslot(int i)
{
}
