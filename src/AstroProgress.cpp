/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroProgress.cpp
	Last generated: Sat Nov 6 15:32:47 1999

 *********************************************************************/

#include "AstroWidget.h"
#include "AstroProgress.h"

#define Inherited AstroProgressData
static real dpd[] = { 365.25, 27.321661, 29.530588 };

AstroProgress::AstroProgress(QWidget* parent, const char* name) : Inherited( parent, name )
{
  int i;
  QString s;  
  setCaption(transl("Progressions"));
  if(is.fProgress) DoProg->setChecked(TRUE);
  doprog = is.fProgress ? fTrue : fFalse;
  prog = FALSE;
  ProgSet->setButton(us.fSolarArc ? 1 : 0);
  solarc = us.fSolarArc ? fTrue : fFalse;
  for(i = 1; i < 13; i++)
    Month->insertItem(szMonth[i]); 
  myci = ciTran;
  Month->setCurrentItem(myci.mon-1);
  s.sprintf("%d", myci.day);
  emit TheDay->setText(s.data());
  s.sprintf("%d", myci.yea);
  emit Year->setText(s.data());
  emit Time->setText(SzTim(myci.tim));
  for(i = 0; i < 3 ; i++)
    if(us.rProgDay == dpd[i])
    {
      degperday = i;
      DegPerDay->setCurrentItem(degperday);
    }
}


AstroProgress::~AstroProgress()
{
}

void AstroProgress::timeslot()
{
  char *ptr = NULL;
  QString s;
  real f = RParseSz(Time->text(), pmTim);
  myci.tim = f;  
}

void AstroProgress::okslot()
{
  close(FALSE);
  us.fListing = us.fWheel = us.fGrid = us.fAspList = us.fMidpoint = us.fHorizon = us.fOrbit = us.fSector = us.fCalendar = us.fInfluence = us.fAstroGraph = us.fEphemeris = us.fInDay = us.fHorizonSearch = us.fTransitInf = us.fSign = us.fObject = us.fAspect = us.fConstel = us.fOrbitData = us.fMeaning = us.fKeyGraph = us.fSwitch = us.fSwitchRare = us.fCredit = fFalse;
  us.nArabic = 0;
  is.fProgress = DoProg->isChecked();
  us.fSolarArc = solarc;
  us.rProgDay = dpd[degperday];
  myci.mon = Month->currentItem() + 1;
  dayslot();
  yearslot();
  timeslot();
  ciTran = myci;
  prog = TRUE;
  us.fTransit = fTrue;
}

void AstroProgress::nowslot()
{
  QString s;
  CI tempci = ciCore;
  FInputData(szNowCore);
  myci = ciCore;
  ciCore = tempci;
  Month->setCurrentItem(myci.mon-1);
  s.sprintf("%d", myci.day);
  emit TheDay->setText(s.data());
  s.sprintf("%d", myci.yea);
  emit Year->setText(s.data());
  emit Time->setText(SzTim(myci.tim));
}

void AstroProgress::progsetslot(int i)
{
  solarc = (i == 1) ? fTrue : fFalse;
}

void AstroProgress::dayslot()
{
  NCheck(TheDay, 1, 31, "day value", &myci.day);
}

void AstroProgress::cancelslot()
{
  close(FALSE);
}

void AstroProgress::degperdayslot(int i)
{
  degperday = i;
}

void AstroProgress::yearslot()
{
  NCheck(Year, -5000, 5000, "year value", &myci.yea);
}
