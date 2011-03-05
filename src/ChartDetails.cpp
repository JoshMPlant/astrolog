/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: ChartDetails.cpp
	Last generated: Sat Nov 13 16:01:10 1999

 *********************************************************************/

#include "ChartDetails.h"
#include "AstroWidget.h"

#define Inherited ChartDetailsData

extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
}

ChartDetails::ChartDetails(QWidget* parent, const char* name) : Inherited( parent, name )
{
  QString s;
  int i;
  setCaption(transl("Chart Setting Details"));
  Velocit->setChecked(us.fVelocity ? TRUE : FALSE);
  s.sprintf("%d", us.nWheelRows);
  wheelrows = us.nWheelRows;
  emit WheelRows->setText(s.data());
  Reverse->setChecked(us.fWheelReverse ? TRUE : FALSE);
  ShowConf->setChecked(us.fGridConfig ? TRUE : FALSE);
  ShowsSummary->setChecked(us.fAspSummary ? TRUE : FALSE);
  MidpointSummary->setChecked(us.fMidSummary ? TRUE : FALSE);
  IncludesAsp->setChecked(us.fMidAspect ? TRUE : FALSE);
  PolarCenter->setChecked(us.fPrimeVert ? TRUE : FALSE);
  ApproxPlac->setChecked(us.fSectorApprox ? TRUE : FALSE);
  SignInfluence->setChecked(us.fInfluenceSign ? TRUE : FALSE);
  s.sprintf("%d", us.nAstroGraphStep);
  astrographstep = us.nAstroGraphStep;
  emit StepRate->setText(s.data());
  LatCross->setChecked(us.fLatitudeCross ? TRUE : FALSE);
  EntireYear->setChecked(us.fCalendarYear ? TRUE : FALSE);
#ifdef ARABIC
  s.sprintf("%d", us.nArabicParts);
  arabicparts = us.nArabicParts;
  emit NbArabParts->setText(s.data());
  TermRevers->setChecked(us.fArabicFlip ? TRUE : FALSE);
#endif
  switch(us.nStar)
  {
    case 'z' : i = 1; break;
    case 'l' : i = 2; break;
    case 'n' : i = 3; break;
    case 'b' : i = 4; break;
    default : i = 0;
  }
  StarsOrder->setButton(i);
  starsorder = i;
#ifdef ARABIC
  switch(us.nArabic)
  {
    case 'z' : i = 1; break;
    case 'n' : i = 2; break;
    case 'f' : i = 3; break;
    default : i = 0;
  }  
  ArabPartOrder->setButton(i);
  arabpartorder = i;
#endif
}


ChartDetails::~ChartDetails()
{
}

void ChartDetails::steprateslot()
{
  NCheck(StepRate, 1, 160, "step rate", &astrographstep);
}

void ChartDetails::nbarabpartslot()
{
  NCheck(NbArabParts, 1, cPart, "numer of parts", &arabicparts);
}

void ChartDetails::okslot()
{
  close(FALSE);
  us.fVelocity       = Velocit->isChecked();
  wheelrowslot();
  us.nWheelRows      = wheelrows;
  us.fWheelReverse   = Reverse->isChecked();
  us.fGridConfig     = ShowConf->isChecked();
  us.fAspSummary     = ShowsSummary->isChecked();
  us.fMidSummary     = MidpointSummary->isChecked();
  us.fMidAspect      = IncludesAsp->isChecked();
  us.fPrimeVert      = PolarCenter->isChecked();
  us.fSectorApprox   = ApproxPlac->isChecked();
  us.fInfluenceSign  = SignInfluence->isChecked();
  steprateslot();
  us.nAstroGraphStep = astrographstep;
  us.fLatitudeCross  = LatCross->isChecked();
  us.fCalendarYear   = EntireYear->isChecked();
#ifdef ARABIC
  nbarabpartslot();
  us.nArabicParts    = arabicparts;
  us.fArabicFlip     = TermRevers->isChecked();
#endif
  switch(starsorder)
    {
      case 1  : us.nStar = 'z'; break;
      case 2  : us.nStar = 'l'; break;
      case 3  : us.nStar = 'n'; break;
      case 4  : us.nStar = 'b'; break;
      default : us.nStar = 0;
    }
#ifdef ARABIC
  switch(arabpartorder)
  {
      case 1  : us.nArabic = 'z'; break;
      case 2  : us.nArabic = 'n'; break;
      case 3  : us.nArabic = 'f'; break;
      default : us.nArabic = 0;
  }
#endif        
}

void ChartDetails::arabpartorderslot(int i)
{
  arabpartorder = i;
}

void ChartDetails::starorderslot(int i)
{
  starsorder = i;
}

void ChartDetails::wheelrowslot()
{
  NCheck(WheelRows, 1, 11, "numer of rows", &wheelrows);
}

void ChartDetails::cancelslot()
{
  close(FALSE);
}













