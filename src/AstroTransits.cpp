/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroTransits.cpp
	Last generated: Wed Nov 3 14:34:27 1999

 *********************************************************************/

#include "AstroWidget.h"
#include "AstroTransits.h"

#define Inherited AstroTransitsData

AstroTransits::AstroTransits(QWidget* parent, const char* name) : Inherited( parent, name )
{
  int i;
  QString s;
  setCaption(transl("Transits"));
  for(i = 1; i < 13; i++)
    Month->insertItem(szMonth[i]);
  if(us.fInDay)           { TransType->setButton(1); transtype = 1; }
  else if(us.fInDayInf)   { TransType->setButton(2); transtype = 2; }
  else if(us.fTransit)    { TransType->setButton(3); transtype = 3; }
  else if(us.fTransitInf) { TransType->setButton(4); transtype = 4; }
  else                    { TransType->setButton(0); transtype = 0; }
  if(is.fProgress) Progress->setChecked(TRUE);
  myci = ciTran;
  if(myci.mon < 1) myci.mon = 1;
  Month->setCurrentItem(myci.mon-1);
  if(myci.day < 1) myci.day = 1;
  s.sprintf("%d", myci.day);
  emit TheDay->setText(s.data());
  s.sprintf("%d", myci.yea);
  emit Year->setText(s.data());
  emit Time->setText(SzTim(myci.tim));
  SearchForHits->setButton(0);
  searchforhits = 0;
  s.sprintf("%d", us.nEphemYears);
  nbyears = us.nEphemYears;
  emit NbYears->setText(s.data());
  if(is.fReturn) DisplayReturns->setChecked(TRUE);
  s.sprintf("%d", us.nDivision);
  emit Divisions->setText(s.data());
  divisions = us.nDivision;
  print = FALSE;
}


AstroTransits::~AstroTransits()
{
}

void AstroTransits::okslot()
{
  close(FALSE);
  us.fListing = us.fWheel = us.fGrid = us.fAspList = us.fMidpoint = us.fHorizon = us.fOrbit = us.fSector = us.fCalendar = us.fInfluence = us.fAstroGraph = us.fEphemeris = us.fInDay = us.fHorizonSearch = us.fTransit = us.fTransitInf = us.fSign = us.fObject = us.fAspect = us.fConstel = us.fOrbitData = us.fMeaning = us.fKeyGraph = us.fSwitch = us.fSwitchRare = us.fCredit = fFalse;
  us.nArabic = 0;
  switch(transtype)
  {
    case 1 : us.fInDay = fTrue; break;
    case 2 : us.fInDayInf = fTrue; break;
    case 3 : us.fTransit  = fTrue; break;
    case 4 : us.fTransitInf = fTrue; break;
    default :;
  }
  is.fProgress = Progress->isChecked() ? fTrue : fFalse;
  myci.mon = Month->currentItem() + 1;
  is.fReturn = DisplayReturns->isChecked() ? fTrue : fFalse;
  divisionslot();
  us.nDivision = divisions;
  us.fInDayMonth = (searchforhits) ? fTrue : fFalse;
  nbyearsslot();
  dayslot();
  yearslot();
  timeslot();
  if(searchforhits == 2)
  {
    myci.mon = 0;
    nbyears = 1;
  }
  if(searchforhits == 3)
  {
    if(us.fTransit || us.fTransitInf)
    {
      myci.mon = -1;
      myci.day = -nbyears;
    }
    else
      myci.mon = 0;
    //myci.mon = (us.fTransit || us.fTransitInf) ? -1 : 0;
    if(nbyears < 1)
      nbyears = 1;
  }
  us.nEphemYears = nbyears;
  print = TRUE;
  ciTran = myci;
}

void AstroTransits::cancelslot()
{
  close(FALSE);
}

void AstroTransits::timeslot()
{
  char *ptr = NULL;
  QString s;
  real f = RParseSz(Time->text(), pmTim);
  myci.tim = f;
}

void AstroTransits::divisionslot()
{
  NCheck(Divisions, 2, 512 /*32*/, "year value", &divisions);
}

void AstroTransits::transtypeslot(int i)
{
  transtype = i;
}

void AstroTransits::searchforhitsslot(int i)
{
  searchforhits = i;
}

void AstroTransits::nbyearsslot()
{
  NCheck(NbYears, 0, 5000, "number of years", &nbyears);
}

void AstroTransits::yearslot()
{
  NCheck(Year, -5000, 5000, "year value", &myci.yea);
}

void AstroTransits::nowslot()
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

void AstroTransits::dayslot()
{
  NCheck(TheDay, 1, 31, "day value", &myci.day);
}

