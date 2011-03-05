/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: ObscureSettings.cpp
	Last generated: Thu Oct 28 13:40:55 1999

 *********************************************************************/

#include "AstroWidget.h"
#include "ObscureSettings.h"

#define Inherited ObscureSettingsData
#define CHECK(f, o) if(us.f) o->setChecked(TRUE)
#define ISCHK(f, o) us.f = o->isChecked() ? fTrue : fFalse;

ObscureSettings::ObscureSettings(QWidget* parent, const char* name) : Inherited( parent, name )
{
  int i;
  QString s;
  setCaption(transl("Obscure Settings"));
  CHECK(fTrueNode, TrueNode);
  CHECK(fEuroDate, D24H);
  CHECK(fEuroTime, AmPm);
  CHECK(fSmartCusp, InsigCuspAsp);
  CHECK(fClip80, RightMost);
  CHECK(fWriteOld, OldStyle);
  CHECK(fHouseAngle, CuspPosi);
  CHECK(fIgnoreSign, SignChange);
  CHECK(fIgnoreDir, DirectChange);
  CHECK(fTz4chart, ZoneLess);
  s.sprintf("%d", gs.nGridCell);
  nbcells = gs.nGridCell;
  emit NbCells->setText(s.data());
  if(gs.fFont) RealFonts->setChecked(TRUE);
  switch(gs.nOrient)
  {
    case  0 : i = 0; break;
    case  1 : i = 1; break;
    case -1 : i = 2;
  }
  PostOrient->setButton(i);
  glyph = gs.nGlyphs;
  orient = gs.nOrient;
  igno = ignorez;
  CaprGlyph->setButton((gs.nGlyphs / 1000 == 2) ? 1 : 0);
  UraGlyph->setButton(((gs.nGlyphs / 100) % 10 == 2) ? 1 : 0);
  PlutGlyph->setButton(((gs.nGlyphs / 10) % 10 == 2) ? 1 : 0);
  LiliGlyph->setButton((gs.nGlyphs % 10 == 2) ? 1 : 0);
  RisSetRestr->setExclusive(FALSE);
  for(i = 0; i < 4 ; i++)
    if(ignorez[i]) RisSetRestr->setButton(i);
}


ObscureSettings::~ObscureSettings()
{
}

void ObscureSettings::okslot()
{
  close(FALSE);
  ISCHK(fTrueNode, TrueNode);
  ISCHK(fEuroDate, D24H);
  ISCHK(fEuroTime, AmPm);
  ISCHK(fSmartCusp, InsigCuspAsp);
  ISCHK(fClip80, RightMost);
  ISCHK(fWriteOld, OldStyle);
  ISCHK(fHouseAngle, CuspPosi);
  ISCHK(fIgnoreSign, SignChange);
  ISCHK(fIgnoreDir, DirectChange);
  ISCHK(fTz4chart, ZoneLess);
  gs.nGridCell = nbcells;
  gs.fFont = RealFonts->isChecked() ? fTrue : fFalse;
  gs.nOrient = orient;
  gs.nGlyphs = glyph;
  // Lilith
  //szObjectFont[17] = (gs.nGlyphs % 10 == 2) ? 'e' : 'i';
  szObjectFont[17] = (gs.nGlyphs % 10 == 2) ? '?' : 'a';
  // Pluto
  //szObjectFont[10] = ((gs.nGlyphs / 10) % 10 == 2) ? 'l' : 'Z';
  szObjectFont[10] = ((gs.nGlyphs / 10) % 10 == 2) ? 'H' : 'J';
  // Uranus
  //szObjectFont[8]  = ((gs.nGlyphs / 100) % 10 == 2) ? 'm' : 'X';
  szObjectFont[8]  = ((gs.nGlyphs / 100) % 10 == 2) ? 'F' : 'A';
  ignorez = igno;
}

void ObscureSettings::cancelslot()
{
  close(FALSE);
}

void ObscureSettings::caprslot(int i)
{
  int offset = (glyph / 1000) * 1000, base = glyph - offset;
  glyph = (i == 1) ? base + 2000 : base;
}

void ObscureSettings::uraslot(int i)
{
  int offset = ((glyph / 100) % 10) * 100, base = glyph - offset;
  glyph = (i == 1) ? base + 200 : base;
}


void ObscureSettings::plutslot(int i)
{
  int offset = ((glyph / 10) % 10) * 10, base = glyph - offset;
  glyph = (i == 1) ? base + 20 : base;
}

void ObscureSettings::lilislot(int i)
{
  int offset = glyph % 10, base = glyph - offset;
  glyph = (i == 1) ? base + 2 : base;
}

void ObscureSettings::nbcellsslot()
{
  NCheck(NbCells, 1, cObj, "number of cells", &nbcells);  
}

void ObscureSettings::orientslot(int i)
{
  switch(i)
  {
    case 0 : orient = 0; break;
    case 1 : orient = 1; break;
    case 2 : orient = -1;
  }
}

void ObscureSettings::restslot(int i)
{
  igno[i] = !igno[i];
}
