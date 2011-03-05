/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: GraphSettings.cpp
	Last generated: Tue Nov 16 15:08:26 1999

 *********************************************************************/

#include "AstroWidget.h"
#include "GraphSettings.h"

extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
#undef bool
}

#define Inherited GraphSettingsData

GraphSettings::GraphSettings(QWidget* parent, const char* name) : Inherited( parent, name )
{
  QString s;
  int i = 0;
  emit setCaption(transl("Graphic Settings"));
  s.sprintf("%d", gs.xWin);
  xWin = gs.xWin;
  emit HChartSize->setText(s.data());
  yWin = gs.yWin;
  s.sprintf("%d", gs.yWin);
  emit VChartSize->setText(s.data());
  Rot = gs.nRot;
  s.sprintf("%d", gs.nRot);
  emit MapDegRot->setText(s.data());
  Tilt = gs.rTilt;
  s.sprintf("%2.1f", gs.rTilt);
  emit GlobeTilt->setText(s.data());
  s.sprintf("%d", gi.nTimerDelay);
  Delay = gi.nTimerDelay;
  emit AnimDel->setText(s.data());
  
  WorldMollewide->setChecked(gs.fMollewide ? TRUE : FALSE);
  if(gs.objLeft > 0)
    i = 1;
  else if(gs.objLeft < 0)
    i = 2;
  WheelChart->setButton(i);
  ObjLeft = gs.objLeft;
  for(i = 0; i <= 10; i++)
    Planet->insertItem(szObjName[i]); 
  Planet->setCurrentItem(gs.objLeft == 0 ? oSun : abs(gs.objLeft));
}

GraphSettings::~GraphSettings()
{
}

void GraphSettings::vchartsizeslot()
{
  NCheck(VChartSize, BITMAPY1, BITMAPY, transl("vertical value"), &yWin);
}

void GraphSettings::mapdegrotslot()
{
  NCheck(MapDegRot, 0, nDegMax-1, transl("degree of rotation"), &Rot);
}

void GraphSettings::globetiltslot()
{
  FCheck(GlobeTilt, -rDegQuad, rDegQuad, transl("degree of tilt"), &Tilt);
}

void GraphSettings::okslot()
{
  close(FALSE);
  hchartsizeslot();
  gs.xWin = xWin;
  vchartsizeslot();
  gs.yWin = yWin;
  mapdegrotslot();
  gs.nRot = Rot;
  globetiltslot();
  gs.rTilt = Tilt;
  gs.fMollewide = WorldMollewide->isChecked() ? fTrue : fFalse;
  gs.objLeft = ObjLeft;
  animdelslot();
  gi.nTimerDelay = Delay;
  central->setPaint(' ');
  emit central->resizeContents(gs.xWin, gs.yWin + OFFSET);
}

void GraphSettings::hchartsizeslot()
{
  NCheck(HChartSize, BITMAPX1, BITMAPX, transl("horizontal value"), &xWin);
}

void GraphSettings::animdelslot()
{
  NCheck(AnimDel, 1, 20, transl("animation delay"), &Delay);
}

void GraphSettings::cancelslot()
{
  close(FALSE);
}

void GraphSettings::wheelchartslot(int i)
{
  if(i == 0)
    ObjLeft = 0;
  else if(i == 1)
  {
    if(!ObjLeft)
    {
      ObjLeft = 1;
      Planet->setCurrentItem(1);
      }
    ObjLeft = abs(ObjLeft);
  }
  else if(i == 2)
  {
    if(!ObjLeft)
    {
      ObjLeft = 1;
      Planet->setCurrentItem(1);
      }
    ObjLeft = -abs(ObjLeft);
  }
}

void GraphSettings::planetslot(int i)
{
  int j = (ObjLeft < 0) ? -1 : 1;
  ObjLeft = j * i;
}


