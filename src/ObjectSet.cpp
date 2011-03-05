/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: ObjectSet.cpp
	Last generated: Fri Oct 22 14:44:46 1999

 *********************************************************************/

#include "AstroWidget.h"
#include "ObjectSet.h"

#define Inherited ObjectSetData

#include <qstring.h>

extern "C"
{
#include <stdlib.h>
}

ObjectSet::ObjectSet(QWidget* parent, const char* name) : Inherited( parent, name )
{
}

void ObjectSet::Set(const char *c, real Maxorb, real inFluence, real addi)
{
  QString buf;
  ObjSet->setText(c);
  buf.sprintf("%3.1f", Maxorb);
  maxorb = Maxorb;
  emit MaxOrb->setText(buf.data());
  buf.sprintf("%1.1f", addi);
  add = addi;
  emit Add->setText(buf);
  buf.sprintf("%2.1f", inFluence);
  influence = inFluence;
  emit Influence->setText(buf);
}

void ObjectSet::Get(real* Maxorb, real* inFluence, real* addi)
{
  *Maxorb    = maxorb;
  *inFluence = influence;
  *addi      = add;
}

void ObjectSet::Disable()
{
  emit MaxOrb->setEnabled(FALSE);
  emit Add->setEnabled(FALSE);
  emit Influence->setEnabled(FALSE);
}

void ObjectSet::Influenceslot()
{
  FCheck(Influence, 0.0, 70.0, "influence value", &influence);
}

void ObjectSet::maxorbslot()
{
  FCheck(MaxOrb, 0.0, 360.0, "max orb value", &maxorb);
}

void ObjectSet::addslot()
{
  FCheck(Add, 0.0, 9.0, "add value", &add);
}

ObjectSet::~ObjectSet()
{
}
