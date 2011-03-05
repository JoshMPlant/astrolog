/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: SetAstroColor.cpp
	Last generated: Thu Oct 14 15:27:48 1999

 *********************************************************************/

#include "SetAstroColor.h"
#include "AstroColor.h"

extern AstroColorList *Acl;
extern AstroColor *colorTable[16];

#define Inherited SetAstroColorData

SetAstroColor::SetAstroColor(QWidget* parent, const char* name) : Inherited( parent, name )
{
  AstroColor *ac;
  int i = 0;
  char buf[20];
  setCaption(transl("Select a color"));
  for(i = 0; i <= 15; i++)
  {
    LocalTable[i] = colorTable[i];
  }
  connect(cv->t(), SIGNAL(clicked(int)), this, SLOT(tableslot(int)));
  for(i = 0, ac = Acl->first(); ac; ac = Acl->next(), i++)
    ColorList->insertItem(ac->name);
  for(i = 0; i <= 15; i++)
   cv->init(*LocalTable[i], i);
    
}


SetAstroColor::~SetAstroColor()
{
}

void SetAstroColor::tableslot(int i)
{
  ColorList->setCurrentItem(LocalTable[i]->index);
  ColorList->centerCurrentItem();
}

void SetAstroColor::okslot()
{
  int i;
  extern char *ColorTbl[];
  close(FALSE);
  
  for(i = 0; i <= 15; i++)
    colorTable[i] = LocalTable[i];
}

void SetAstroColor::cancelslot()
{
  close(FALSE);
}

void SetAstroColor::colorlistslot(int i)
{
  int j = cv->checked();
  if(j != -1)
  {
    AstroColor& ac = Acl->GetColor(i);
    LocalTable[j] = &ac;
    cv->init(ac, j);
  }
}
