/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: CalcSettings.cpp
	Last generated: Mon Oct 25 15:06:02 1999

 *********************************************************************/

#include "AstroWidget.h"
#include "CalcSettings.h"

#define Inherited CalcSettingsData

static real AyaValues[] = { 0.0, 0.883333, 0.983333, 2.333333 };

CalcSettings::CalcSettings(QWidget* parent, const char* name) : Inherited( parent, name )
{
  int i;
  real offset;
  QString s;
  emit setCaption(transl("Calculation Settings"));
  for(i = 0; i < 4; i++)
  {
    s.sprintf("%1.5f", AyaValues[i]);
    Ayanamsa->insertItem(s.data());
    if(AyaValues[i] == us.rZodiacOffset)
    {
      Ayanamsa->setCurrentItem(i);
      zodoffset = i;
    }
  }
  s.sprintf("%d", us.nAsp);
  nbasp = us.nAsp;
  emit NbAspects->setText(s.data());
  s.sprintf("%d", us.nHarmonic);
  nbharmonic = us.nHarmonic;
  emit HarmChartFact->setText(s.data());
  FillPlanets(*CentPlan);
  CentPlan->setCurrentItem(us.objCenter);
  centplan = us.objCenter;
  if(us.objOnAsc == 0)
    SolChartSett->setButton(0);
  else if(us.objOnAsc > 0)
    SolChartSett->setButton(1);
  else if(us.objOnAsc < 0)
    SolChartSett->setButton(2);
  solchartset = us.objOnAsc;
  FillPlanets(*UsePlan);
  UsePlan->setCurrentItem(abs(us.objOnAsc));
  s.sprintf("%d", us.nScreenWidth);
  screenwidth = us.nScreenWidth;
  emit TextCol->setText(s.data());
#ifdef PLACALC
  if(us.fPlacalc) Ephem->setChecked(TRUE);
#endif
  DispForm->setButton(us.nDegForm);
  dispform = us.nDegForm;
  if(us.fEquator) EquaPos->setChecked(TRUE);
}

void CalcSettings::FillPlanets(QComboBox& qcb)
{
  int i;
  for(i = 0; i <= 10; i++)
    qcb.insertItem(szObjName[i]);
}

CalcSettings::~CalcSettings()
{
}

void CalcSettings::okslot()
{
  close(FALSE);
  us.rZodiacOffset = AyaValues[zodoffset];
  nbaspslot();
  us.nAsp = nbasp;
  harmchartfactslot();
  us.nHarmonic = nbharmonic;
  us.objCenter = centplan;
  us.objOnAsc = solchartset;
  textcolslot();
  us.nScreenWidth = screenwidth;
#ifdef PLACLAC
  us.fPlacalc = Ephem->isChecked() ? fTrue : fFalse;
#endif
  us.nDegForm = dispform;
  us.fEquator = EquaPos->isChecked() ? fTrue : fFalse;
}

void CalcSettings::cancelslot()
{
  close(FALSE);
}

void CalcSettings::nbaspslot()
{
  NCheck(NbAspects, 0, 18, "number of aspects", &nbasp);
}

void CalcSettings::harmchartfactslot()
{
  NCheck(HarmChartFact, 1, 30000, "Harmonic chart fact", &nbharmonic);
}

void CalcSettings::textcolslot()
{
  NCheck(TextCol, 20, 200, "number of columns", &screenwidth);
}

void CalcSettings::solchartsettslot(int i)
{
  if(i == 0)
  {
    solchartset = 0;
    UsePlan->setCurrentItem(0);
  }
  else if(i == 1)
  {
    if(solchartset == 0)
    {
      solchartset = 1;
      UsePlan->setCurrentItem(1);
    }
    solchartset = abs(solchartset);
  }
  else
  {
    if(solchartset == 0)
    {
      solchartset = -1;
      UsePlan->setCurrentItem(1);
    }
    solchartset = -abs(solchartset);
  }
}

void CalcSettings::useplanslot(int i)
{
  solchartset = ((solchartset < 0) ? -1 : 1) * i;
}

void CalcSettings::displayformatslot(int i)
{
  dispform = i;
}

void CalcSettings::ayanamsaslot(int i)
{
  zodoffset = i;
}

void CalcSettings::centplanslot(int i)
{
  centplan = i;
}


