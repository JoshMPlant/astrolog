/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroSetAspects.cpp
	Last generated: Wed Oct 20 14:37:06 1999

 *********************************************************************/

#include "AstroSetAspects.h"

#define Inherited AstroSetAspectsData


AstroSetAspects::AstroSetAspects(QWidget* parent, const char* name) : Inherited( parent, name )
{
  int i;
  refresh = TRUE;
  setCaption(transl("Aspects Settings"));
  for(i = 1; i <= cAspect ; i++)
  {
    a[i-1]->Set( i >= us.nAsp ? FALSE : TRUE, szAspectName[i], rAspOrb[i], rAspAngle[i], rAspInf[i]);
    connect(a[i-1], SIGNAL(assignal(AspectSet*, bool)), this, SLOT(asslot(AspectSet*, bool)));
  }
}


AstroSetAspects::~AstroSetAspects()
{
}

void AstroSetAspects::okslot()
{
  int i, j = -1;
  close(FALSE);
  bool b;
  for(i = 1; i <= cAspect; i++)
  {
    a[i-1]->Get(&b, &rAspOrb[i], &rAspAngle[i], &rAspInf[i]);
    if(b) j++;
  }
  us.nAsp = j + 2;
}

void AstroSetAspects::cancelslot()
{
  close(FALSE);
}

void AstroSetAspects::restrictallslot()
{
  int i;
  refresh = FALSE;
  for(i = 0; i < cAspect; i++)
    a[i]->Toggle(FALSE);
  refresh = TRUE;
}

void AstroSetAspects::unrestrictallslot()
{
  int i;
  for(i = 0; i < cAspect; i++)
    a[i]->Toggle(TRUE);
}

void AstroSetAspects::togglemajorslot()
{
  int i;
  for(i = 0; i < 5; i++)
    a[i]->Toggle(a[i]->isToggled() ? FALSE : TRUE);
}

void AstroSetAspects::asslot(AspectSet* as, bool b)
{
  int i, j;
  for(i = 0; i < cAspect; i++)
    if(a[i] == as)
    {
      // Tricky zone. Dpn't modify unless you know what you do !
      for(j = 0; j < i; j++)
      {
	  a[j]->Toggle(TRUE);
      }
      if(++j < cAspect)
	a[j]->Toggle(b);
      for(; j < cAspect; j++)
	a[j]->Toggle(FALSE);
      break;
    }
}
