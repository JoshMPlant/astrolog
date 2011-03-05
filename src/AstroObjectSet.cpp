/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroObjectSet.cpp
	Last generated: Sat Oct 23 13:49:55 1999

 *********************************************************************/

#include "AstroObjectSet.h"
extern "C"
{
#include "astrolog.h"
}

#define Inherited AstroObjectSetData

AstroObjectSet::AstroObjectSet(const char *title, int o1, int o2) : Inherited( NULL, NULL )
{
  int i;
  emit setCaption(title);
  obj1 = o1;
  for(i = 0; i < 20; i++)
  {
    o[i]->Set(szObjName[i+obj1], rObjOrb[i+obj1], rObjInf[i+obj1], rObjAdd[i+obj1]);
  }
}


AstroObjectSet::~AstroObjectSet()
{
}

void AstroObjectSet::okslot()
{
  int i;
  close(FALSE);
  for(i = 0; i < 20; i++)
  {
    o[i]->Get(&rObjOrb[i+obj1], &rObjInf[i+obj1], &rObjAdd[i+obj1]);
  }
}

void AstroObjectSet::cancelslot()
{
  close(FALSE);
}
