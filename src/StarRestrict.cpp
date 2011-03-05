/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: StarRestrict.cpp
	Last generated: Mon Oct 25 13:54:25 1999

 *********************************************************************/

#include "StarRestrict.h"

#define Inherited StarRestrictData
extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
}

StarRestrict::StarRestrict(QWidget* parent, const char* name) : Inherited( parent, name )
{
  int i;
  setCaption(transl("Fixed Star Restriction"));
  for(i = 0; i < cStar; i++)
  {
    s[i]->setText(szObjName[starLo + i]);
    if(ignore[starLo + i]) s[i]->setChecked(TRUE);
  }
    
}


StarRestrict::~StarRestrict()
{
}

void StarRestrict::okslot()
{
  int i;
  close(FALSE);
  for(i = 0; i < cStar; i++)
    ignore[starLo+i] = s[i]->isChecked() ? fTrue : fFalse;
}

void StarRestrict::cancelslot()
{
  close(FALSE);
}

void StarRestrict::restrictslot()
{
  int i;
  for(i = 0; i < cStar; i++)
    s[i]->setChecked(TRUE);
}

void StarRestrict::unrestrictslot()
{
  int i;
  for(i = 0; i < cStar; i++)
    s[i]->setChecked(FALSE);
}

