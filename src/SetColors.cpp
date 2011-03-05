/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: SetColors.cpp
	Last generated: Mon Oct 18 13:01:08 1999

 *********************************************************************/

#include "SetColors.h"
#include "AstroColor.h"

#define Inherited SetColorsData

extern AstroColor *colorTable[16];

SetColors::SetColors(QWidget* parent, const char* name) : Inherited( parent, name )
{
  int i;
  setCaption(transl("Set the colors for the elements and the aspects"));
  connect(cv->t(), SIGNAL(clicked(int)), this, SLOT(tableslot(int)));
  for(i = 0; i <= 3; i++)
    LocalElemB[i] = kElemB[i];
  for(i = 0; i <= cAspect; i++)
    LocalAspB[i] = kAspB[i];
  for(i = 0; i <= 15; i++)
    cv->init(*colorTable[i], i);
}


SetColors::~SetColors()
{
}

void SetColors::okslot()
{
  int i;
  close(FALSE);
  for(i = 0; i <= 3; i++)
    kElemA[i] = kElemB[i] = LocalElemB[i];
  for(i = 0; i <= cAspect; i++)
    kAspA[i] = kAspB[i] = LocalAspB[i];
}

void SetColors::cancelslot()
{
  close(FALSE);
}

void SetColors::elementsslot(int i)
{
  int j;
  for(j = 0; j < cAspect; j++)
    if(a[j]->isChecked())
      emit a[j]->setChecked(FALSE);
  thecv() << LocalElemB[i];
}

void SetColors::aspectsslot(int i)
{
  int j;
  for(j = 0; j <= 3; j++)
    if(e[j]->isChecked())
      emit e[j]->setChecked(FALSE);
  thecv() << LocalAspB[i+1];
}

void SetColors::tableslot(int i)
{
  int j;
  for(j = 0; j <= 3; j++)
    if(e[j]->isChecked())
    {
      LocalElemB[j] = i;
      return;
    }
  for(j = 0; j < cAspect; j++)
    if(a[j]->isChecked())
      LocalAspB[j+1] = i;
}



