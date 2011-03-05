/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AspectSet.cpp
	Last generated: Tue Oct 19 15:02:59 1999

 *********************************************************************/

#include "AstroWidget.h"
#include "AspectSet.h"
#include <qstring.h>

extern "C"
{
#include <stdlib.h>
}

#define Inherited AspectSetData

AspectSet::AspectSet
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
}

void AspectSet::Set(bool checked, const char *name, real oRb, real anGle, real inFluence)
{
  QString buf;
  AspName->setChecked(checked);
  AspName->setText(name);
  buf.sprintf("%2.2f", oRb);
  orb = oRb;
  emit Orb->setText(buf.data());
  buf.sprintf("%3.2f", anGle);
  angle = anGle;
  emit Angle->setText(buf.data());
  buf.sprintf("%1.2f", inFluence);
  influence = inFluence;
  emit Influence->setText(buf.data());
}


AspectSet::~AspectSet()
{
}

void AspectSet::Get(bool* checked, real* oRb, real* anGle, real* inFluence)
{
  char buf[8];
  *checked   = AspName->isChecked();
  *oRb       = orb;
  *anGle     = angle;
  *inFluence = influence;
}
	      
void AspectSet::orbslot()
{
  FCheck(Orb, -30.0, 30.0, "orb value", &orb);
}

void AspectSet::influenceslot()
{
  FCheck(Influence, 0.0, 1.0, "influence value", &influence);
}

void AspectSet::angleslot()
{
  FCheck(Angle, 0.0, 360.0, "angle value", &angle);
}

void AspectSet::aspnameslot()
{
  emit assignal(this, AspName->isChecked());
}




