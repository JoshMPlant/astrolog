/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: Restrictions.cpp
	Last generated: Sat Oct 23 15:23:22 1999

 *********************************************************************/

#include "Restrictions.h"
extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
}


#define Inherited RestrictionsData

Restrictions::Restrictions(const char *s, byte *i1, byte *i2) : Inherited(NULL, NULL)
{
  int i;
  setCaption(s);
  ib1 = i1; ib2 = i2;
  for(i = 0, i1++; i < uranHi; i++, i1++)
  {
    r[i]->setText(szObjName[i+1]);
     if(*i1)
	emit r[i]->setChecked(TRUE);
  }
  

}


Restrictions::~Restrictions()
{
}

void Restrictions::toglecuspslot()
{
  int i;
  for(i = cuspLo - 1; i < cuspHi; i++)
    emit r[i]->setChecked(r[i]->isChecked() ? FALSE : TRUE);
}
void Restrictions::okslot()
{
  int i;
  close(FALSE);
  for(i = 0, ib1++; i < uranHi; i++, ib1++)
  {
    *ib1 = r[i]->isChecked() ? fTrue : fFalse;
  }
}
void Restrictions::toggleminorslot()
{
  int i;
  for(i = oMain; i < oCore; i++)
    emit r[i]->setChecked(r[i]->isChecked() ? FALSE : TRUE);
}
void Restrictions::copyslot()
{
  int i;
  byte *i1 = ++ib2;
  for(i = 0; i < uranHi; i++, i1++)
    emit r[i]->setChecked( *i1 ? TRUE : FALSE);
}
void Restrictions::toggleuranslot()
{
  int i;
  for(i = uranLo -1; i < uranHi; i++)
    emit r[i]->setChecked(r[i]->isChecked() ? FALSE : TRUE);
}
void Restrictions::restrictallslot()
{
  int i;
  for(i = 0; i < uranHi; i++)
    emit r[i]->setChecked(TRUE);
}
void Restrictions::cancelslot()
{
  close(FALSE);
}
void Restrictions::unrestrictallslot()
{
  int i;
  for(i = 0; i < uranHi; i++)
    emit r[i]->setChecked(FALSE); 
}
