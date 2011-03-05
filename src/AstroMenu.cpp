/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 

#include "AstroMenu.h"
#include "SetAstroColor.h"
#include <qkeycode.h>

extern "C"
{
#include <ctype.h>
#include <sys/types.h>
#include <signal.h>
}

extern AstroApp AstroApp;

#define NewMenu(popup, text, accel) popup->insertItem(text, accel); \
popup->setAccel(accel, accel);
#define iftog(flag, key)  if(flag) Toggle(key)
#define action gi.fiskde = fFalse; Action(); gi.fiskde = fTrue
#define SwitchAction(flag) Toggle(i); not2(flag); central->setPaint(' ');
#define pop(var) *var = new QPopupMenu
#define seths(key, no) case Qt::CTRL + Qt::SHIFT + key : us.nHouseSystem = no; ToggleHousSys(); central->setPaint(); break;
#define chtype(i, f, c) ToggleType(i); central->ChartType(&us.f, c); break;
#define MenuMac if(szMacroName[i-1]) strcpy(buf, szMacroName[i-1]); else \
sprintf(buf, transl("Macro %d"), i);

extern icons icons[];

char *ChartTitle[Keyboard+1];

#ifdef KDE1
static const int ChartKeys[] = { SHIFT+Key_V, ALT+SHIFT+Key_V, SHIFT+Key_A, ALT+Key_L, ALT+Key_M, SHIFT+Key_Z, SHIFT+Key_S, SHIFT+Key_M, SHIFT+Key_K, SHIFT+Key_I, SHIFT+Key_L, SHIFT+Key_E, ALT+Key_P, ALT+SHIFT+Key_L, ALT+SHIFT+Key_T, ALT+SHIFT+Key_P,  Key_C, ALT+Key_Y, ALT+SHIFT+Key_Y, ALT+SHIFT+Key_M, ALT+SHIFT+Key_D, SHIFT+Key_Y, ALT+Key_N, ALT+SHIFT+Key_N, SHIFT+Key_W, SHIFT+Key_G, SHIFT+Key_P, ALT+Key_2, ALT+Key_3, ALT+Key_4, ALT+Key_5, ALT+Key_6, ALT+Key_7, ALT+Key_8, ALT+Key_9, Key_Question },

  HousSysKeys[] = { Key_P, Key_K, Key_E, Key_C, Key_M, Key_R, Key_Y, Key_U, \
  Key_T, Key_A, Key_Q, Key_O, Key_W, Key_V, Key_N },
  nScaleType[] = { CTRL+Key_1, CTRL+Key_2, CTRL+Key_3, CTRL+Key_4 },
  JRateKeys[] = { Key_Exclam, Key_At, Key_NumberSign, Key_Dollar, Key_Percent,\
  Key_AsciiCircum, Key_Ampersand, Key_Apostrophe,Key_ParenLeft },
  JFactKeys[] = { Key_1, Key_2, Key_3, Key_4, Key_5, Key_6, Key_7, Key_8, Key_9 };
#else
static const int ChartKeys[] = { Qt::SHIFT+Qt::Key_V, Qt::ALT+Qt::SHIFT+Qt::Key_V, Qt::SHIFT+Qt::Key_A, Qt::ALT+Qt::Key_L, Qt::ALT+Qt::Key_M, Qt::SHIFT+Qt::Key_Z, Qt::SHIFT+Qt::Key_S, Qt::SHIFT+Qt::Key_M, Qt::SHIFT+Qt::Key_K, Qt::SHIFT+Qt::Key_I, Qt::SHIFT+Qt::Key_L, Qt::SHIFT+Qt::Key_E, Qt::ALT+Qt::Key_P, Qt::ALT+Qt::SHIFT+Qt::Key_L, Qt::ALT+Qt::SHIFT+Qt::Key_T, Qt::ALT+Qt::SHIFT+Qt::Key_P,  Qt::Key_C, Qt::ALT+Qt::Key_Y, Qt::ALT+Qt::SHIFT+Qt::Key_Y, Qt::ALT+Qt::SHIFT+Qt::Key_M, Qt::ALT+Qt::SHIFT+Qt::Key_D, Qt::SHIFT+Qt::Key_Y, Qt::ALT+Qt::Key_N, Qt::ALT+Qt::SHIFT+Qt::Key_N, Qt::SHIFT+Qt::Key_W, Qt::SHIFT+Qt::Key_G, Qt::SHIFT+Qt::Key_P, Qt::ALT+Qt::Key_2, Qt::ALT+Qt::Key_3, Qt::ALT+Qt::Key_4, Qt::ALT+Qt::Key_5, Qt::ALT+Qt::Key_6, Qt::ALT+Qt::Key_7, Qt::ALT+Qt::Key_8, Qt::ALT+Qt::Key_9, Qt::Key_Question },

  HousSysKeys[] = { Qt::Key_P, Qt::Key_K, Qt::Key_E, Qt::Key_C, Qt::Key_M, Qt::Key_R, Qt::Key_Y, Qt::Key_U, \
  Qt::Key_T, Qt::Key_A, Qt::Key_Q, Qt::Key_O, Qt::Key_W, Qt::Key_V, Qt::Key_N },
  nScaleType[] = { Qt::CTRL+Qt::Key_1, Qt::CTRL+Qt::Key_2, Qt::CTRL+Qt::Key_3, Qt::CTRL+Qt::Key_4 },
  JRateKeys[] = { Qt::Key_Exclam, Qt::Key_At, Qt::Key_NumberSign, Qt::Key_Dollar, Qt::Key_Percent,\
  Qt::Key_AsciiCircum, Qt::Key_Ampersand, Qt::Key_Apostrophe,Qt::Key_ParenLeft },
  JFactKeys[] = { Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4, Qt::Key_5, Qt::Key_6, Qt::Key_7, Qt::Key_8, Qt::Key_9 };
#endif

#ifdef KDE1
bool KMenuBar::isItemChecked(int id)
{     
  return menu->isItemChecked(id);
}
#endif

void AstroMenu::VoidChart()
{
  int c;
  for(c = List; c <= Keyboard; c++)
    if(isItemChecked(ChartKeys[c]))
      setItemChecked(ChartKeys[c], FALSE);
}

void AstroMenu::ToggleChart(ChartType ct)
{
  int c;
  VoidChart();
  if(!us.fWheel)
  {
    ToggleType(Wheel);
    central->ChartType(&us.fWheel, 'V');
  }
  if(ct == Comparison)
  {
#ifdef KDE1
    Toggle(Key_C);
#else
    Toggle(Qt::Key_C);
#endif
  }
  else
  {
    for(c = Comparison; c <= Progressed_Natal; c++)
      if(c == ct)
	setItemChecked(ChartKeys[c], TRUE);
  }
  setItemChecked(ChartKeys[Wheel], TRUE);
  Ksb->changeItem(ChartTitle[ct], 1);
}

void AstroMenu::ToggleType(ChartType ct)
{
  int c;
  VoidChart();
  for(c = List; c <= Progress; c++)
    if(c == ct)
      setItemChecked(ChartKeys[c], TRUE);
  Ksb->changeItem(ChartTitle[ct], 0);
}

void AstroMenu::ToggleGraph(ChartType ct)
{
  int c;
  VoidChart();
#ifdef KDE1
  setItemChecked(Key_V, TRUE);
#else
   setItemChecked(Qt::Key_V, TRUE);
#endif 
  for(c = Map; c <= Polar; c++)
    if(c == ct)
      setItemChecked(ChartKeys[c], TRUE);
  Ksb->changeItem("", 1);
  Ksb->changeItem(ChartTitle[ct], 0);
}

void AstroMenu::TogglenScale(int i)
{
  int j;
  gs.nScale = i;
  for(j = Small; j <= Huge; j++)
    setItemChecked(nScaleType[j], FALSE);
  switch(i)
  {
    case 100 : setItemChecked(nScaleType[Small], TRUE); break;
    case 200 : setItemChecked(nScaleType[Medium], TRUE); break;
    case 300 : setItemChecked(nScaleType[Large], TRUE); break;
    case 400 : setItemChecked(nScaleType[Huge], TRUE); break;
  }
  central->setPaint(' ');
}

void AstroMenu::ToggleJRate(JRateType jr)
{
 int c;
 for(c = Seconds; c <= Millenia; c++)
 {
      setItemChecked(JRateKeys[c], c == jr ? TRUE : FALSE);
      unijmp->setItemChecked(c, c == jr ? TRUE : FALSE);
 }
}

void AstroMenu::ToggleJFact(JFact jf, int i)
{
  int c;
  central->SetDir(i);
  for(c = Unit_1; c <= Unit_9; c++)
  {
      setItemChecked(JFactKeys[c], c == jf ? TRUE : FALSE);
      nbjmp->setItemChecked(c, c == jf ? TRUE : FALSE);
  }
}

void AstroMenu::ToggleHelp(ChartType ct)
{
  int c;
  VoidChart();
  for(c = Signs; c <= Keyboard; c++)
    if(c == ct)
      setItemChecked(ChartKeys[c], TRUE);
  Ksb->changeItem("", 1);
  Ksb->changeItem(ChartTitle[ct], 0);
}

AstroMenu::AstroMenu(QWidget* parent) : KMenuBar(parent, NULL)
{
  int i = 1, j;
  char buf[20], uni[2] = " ";
  minors = FALSE;
  asw = (AstroWindow *)parent;
#ifdef KDE1
  QString anim2 = "anim2.xpm";
#else
  const QString anim2 = "anim2";
#endif
  QPopupMenu pop(fi), pop(ed), pop(m1), pop(m2), pop(m3), pop(m4), pop(view), \
    pop(ws), pop(info), pop(ch3), pop(ch4), pop(sett), pop(house), pop(hset),\
    pop(cha), pop(gra), pop(chsc), pop(glo), pop(anim), pop(jrate), \
    pop(jfact), pop(hlp), pop(doc);
  nbjmp  = new QPopupMenu;
  unijmp = new QPopupMenu;
#ifdef KDE1
  Ksb->setInsertOrder(KStatusBar::RightToLeft);
  Ksb->insertItem("                                     ", 0);
  Ksb->insertItem("                             ", 1);
#else
  Ksb->insertFixedItem("                                     ", 0, TRUE);
  Ksb->insertFixedItem("                                                     ", 1);
#endif
  for(j = 0; j <= LASTICON; j++)
#ifdef KDE1
	Ktb->insertButton(Kil->loadIcon(icons[j].s), icons[j].id, TRUE, icons[j].tip);
#else
	Ktb->insertButton(Kil->loadIcon(icons[j].s, KIcon::MainToolbar), icons[j].id, TRUE, icons[j].tip);
#endif

  for(j =1; j <= 9; j++)
  {
    uni[0] = j + '0';
    nbjmp->insertItem(uni, j -1);
  }
#ifdef KDE1
  Ktb->insertButton(Kil->loadIcon(anim2.data()), 10, nbjmp, TRUE, transl("Jump factor"));
#else
  Ktb->insertButton(Kil->loadIcon(anim2, KIcon::MainToolbar), 10, nbjmp, TRUE, transl("Jump factor"));
#endif
  connect(nbjmp, SIGNAL(activated(int)), this, SLOT(nbsslot(int)));
  unijmp->insertItem(transl("Seconds"), Seconds);
  unijmp->insertItem(transl("Minutes"), Minutes);
  unijmp->insertItem(transl("Hours"), Hours);
  unijmp->insertItem(transl("Days"), Days);
  unijmp->insertItem(transl("Months"), Months);
  unijmp->insertItem(transl("Years"), Years);
  unijmp->insertItem(transl("Decades"), Decades);
  unijmp->insertItem(transl("Centuries"), Centuries);
  unijmp->insertItem(transl("Millenia"), Millenia);
#ifdef KDE1
  Ktb->insertButton(Kil->loadIcon(anim2.data()), 11, unijmp, TRUE, transl("Jump Rate"));
#else
  Ktb->insertButton(Kil->loadIcon(anim2, KIcon::MainToolbar), 11, unijmp, TRUE, transl("Jump Rate"));
#endif
  connect(unijmp, SIGNAL(activated(int)), this, SLOT(unitsslot(int)));
  connect(Ktb, SIGNAL(clicked(int)), SLOT(iconsslot(int)));
#ifdef KDE1
  NewMenu(fi, transl("New Window..."), ALT + Key_Exclam);
  NewMenu(fi, transl("Open Chart..."), ALT + Key_O);
  NewMenu(fi, transl("Open Chart #2..."), ALT + SHIFT + Key_O);
  fi->insertSeparator();
  NewMenu(fi, transl("Save Chart Info..."), ALT + Key_W);
  NewMenu(fi, transl("Save Chart Positions..."), ALT + SHIFT + Key_W);
  NewMenu(fi, transl("Save Chart Text Output..."), CTRL + Key_5);
  NewMenu(fi, transl("Save Chart Bitmap..."), CTRL + Key_6);
#ifdef META
  NewMenu(fi, transl("Save Chart Picture..."), CTRL + Key_7);
#endif
#ifdef PS
  NewMenu(fi, transl("Save Chart Postscript..."), CTRL + Key_8);
#endif
  fi->insertSeparator();
  NewMenu(fi, transl("Save Settings..."), CTRL + Key_9);
  fi->insertSeparator();
  NewMenu(fi, transl("Print..."), CTRL + Key_P);
  NewMenu(fi, transl("Print Setup..."), CTRL + Key_Q);
  if(AstroApp.nbchilds > 1)
    NewMenu(fi, transl("Close"), ALT + Key_C);
  NewMenu(fi, transl("Quit..."), Key_Q);
  insertItem(transl("File"), fi);
  NewMenu(ed, transl("Command Line..."), Key_Return);
  ed->insertSeparator();
  while(i <= 12)
    {
      MenuMac;
      NewMenu(m1, buf, Key_F1 + i - 1);
      i++;
    }
  ed->insertItem(transl("Run Macros (Normal Set)"), m1);
  while(i <= 24)
    {
      MenuMac;
      NewMenu(m2, buf, SHIFT + Key_F1 + i - 13);
      i++;
    }
  ed->insertItem(transl("Run Macros (Shift Set)"), m2);
  while(i <= 36)
    {
      MenuMac;
      NewMenu(m3, buf, CTRL + Key_F1 + i - 25);
      i++;
    }
  ed->insertItem(transl("Run Macros (Control Set)"), m3);
  while(i <= 48)
    {
      MenuMac;
      NewMenu(m4, buf, ALT + Key_F1 + i - 37);
      i++;
    }
  ed->insertItem(transl("Run Macros (Alt Set)"), m4);
  //ed->insertSeparator();
  insertItem(transl("Edit"), ed);
  NewMenu(view, transl("Show Graphic..."), Key_V);
  view->insertItem(transl("Window Settings"), ws);
  NewMenu(ws, transl("Buffer Redraws"), Key_Tab);
  NewMenu(ws, transl("Redraw Screen"), Key_Space);
  NewMenu(ws, transl("Clear Screen"), Key_Delete);
  ws->insertSeparator();
  NewMenu(ws, transl("Window Resizes Chart"), ALT + Key_Q);
  NewMenu(ws, transl("Page Up"), Key_PageUp);
  NewMenu(ws, transl("Page Down"), Key_PageDown);
  NewMenu(ws, transl("Home"), Key_Home);
  NewMenu(ws, transl("End"), Key_End);
  NewMenu(view, transl("Colored Text"), ALT + Key_K);
  NewMenu(view, transl("Set Palette..."), ALT +SHIFT + Key_K);
  NewMenu(view, transl("Set Colors..."), ALT + SHIFT + Key_Percent);
#ifdef INTERPRET
  NewMenu(view, transl("Show Interpretation"), ALT + SHIFT + Key_I);
#endif
  NewMenu(view, transl("Print Nearest second"), ALT + Key_B);
  NewMenu(view, transl("Applying Aspects"), ALT + Key_X);
  NewMenu(view, transl("Parallel Aspects"), ALT + SHIFT + Key_X);
  insertItem(transl("View"), view);
  NewMenu(info, transl("Set Chart Info..."), ALT + Key_Percent);
  NewMenu(info, transl("Chart For Now"), Key_N);
  NewMenu(info, transl("Default Chart Info..."), ALT +Key_D);
  info->insertSeparator();
  NewMenu(info, transl("Set Chart #2 Info..."), ALT + SHIFT +Key_Z);
  NewMenu(ch3, transl("Set Chart #3 Info..."), SHIFT +Key_3);
  NewMenu(ch3, transl("Open Chart #4 ..."), ALT + SHIFT + Key_3);
  info->insertItem(transl("Chart #3"), ch3);
  NewMenu(ch4, transl("Set Chart #4 Info..."), SHIFT + Key_4);
  NewMenu(ch4, transl("Open Chart #4 ..."), ALT + SHIFT + Key_4);
  info->insertItem(transl("Chart #4"), ch4);
  info->insertSeparator();
  NewMenu(info, ChartTitle[Comparison], Key_C);
  NewMenu(info, ChartTitle[Synastry], ALT + Key_Y);
  NewMenu(info, ChartTitle[Composite], ALT + SHIFT + Key_Y);
  NewMenu(info, ChartTitle[Time_Space], ALT + SHIFT +Key_M);
  info->insertSeparator();
  NewMenu(info, ChartTitle[Date_Diff], ALT + SHIFT + Key_D);
#ifdef BIORHYTHM
  NewMenu(info, ChartTitle[Biorhythm], SHIFT + Key_Y);
#endif
  NewMenu(info, ChartTitle[Transit_Natal], ALT + Key_N);
  NewMenu(info, ChartTitle[Progressed_Natal], ALT + SHIFT + Key_N);
  insertItem(transl("Info"), info);
  NewMenu(sett, transl("Sidereal Zodiac"), Key_S);
  NewMenu(sett, transl("Heliocentric"), Key_H);
  NewMenu(house, szSystem[0], CTRL+ SHIFT+ Key_P);
  NewMenu(house, szSystem[1], CTRL + SHIFT+ Key_K);
  NewMenu(house, szSystem[2], CTRL + SHIFT+ Key_E);
  NewMenu(house, szSystem[3], CTRL + SHIFT+ Key_C);
  NewMenu(house, szSystem[4], CTRL + SHIFT+ Key_M);
  NewMenu(house, szSystem[5], CTRL + SHIFT+ Key_R);
  NewMenu(house, szSystem[6], CTRL + SHIFT+ Key_Y);
  NewMenu(house, szSystem[7], CTRL + SHIFT+ Key_U);
  NewMenu(house, szSystem[8], CTRL + SHIFT+ Key_T);
  NewMenu(house, szSystem[9], CTRL + SHIFT+ Key_A);
  NewMenu(house, szSystem[10], CTRL + SHIFT+ Key_Q);
  NewMenu(house, szSystem[11], CTRL + SHIFT+ Key_O);
  NewMenu(house, szSystem[12], CTRL + SHIFT+ Key_W);
  NewMenu(house, szSystem[13], CTRL + SHIFT+ Key_V);
  NewMenu(house, szSystem[14], CTRL + SHIFT+ Key_N);
  sett->insertItem(transl("House System"), house);
  NewMenu(hset, transl("Solar Chart"), ALT + Key_1);
  NewMenu(hset, transl("Show Decan"), Key_G);
  NewMenu(hset, transl("Flip Signs & Houses"), Key_F);
  NewMenu(hset, transl("Geodetic Houses"), ALT + SHIFT + Key_H);
  hset->insertSeparator();
  NewMenu(hset, transl("Vedic Wheel Display"), Key_Z);
  NewMenu(hset, transl("Show Navamsas"), Key_Y);
  sett->insertItem(transl("House Settings"), hset);
  NewMenu(sett, transl("Aspects Settings..."), ALT + SHIFT + Key_A);
  NewMenu(sett, transl("Objects Settings..."), ALT + Key_J);
  NewMenu(sett, transl("21-40 Objects Settings..."), ALT + SHIFT + Key_J);
  NewMenu(sett, transl("41-60 Objects Settings..."), CTRL + ALT + SHIFT + Key_J);
  NewMenu(sett, transl("61-80 Objects Settings..."), CTRL + SHIFT + Key_J);
  sett->insertSeparator();
  NewMenu(sett, transl("Restrictions..."), ALT + Key_R);
  NewMenu(sett, transl("Include Minors"), SHIFT + Key_R);
  NewMenu(sett, transl("Include Cusps"), SHIFT + Key_C);
  NewMenu(sett, transl("Include Uranians"), Key_U);
  NewMenu(sett, transl("Include Fixed Stars"), SHIFT + Key_U);
  NewMenu(sett, transl("Star Restrictions..."), ALT + SHIFT + Key_R);
  NewMenu(sett, transl("Transit Restrictions..."), ALT + SHIFT + Key_F);
  sett->insertSeparator();
  NewMenu(sett, transl("Calculation Settings..."), ALT + SHIFT + Key_S);
  NewMenu(sett, transl("Obscure Settings..."), ALT + SHIFT + Key_B);
  insertItem(transl("Settings"), sett);
  NewMenu(cha, ChartTitle[List], ChartKeys[List]);
  NewMenu(cha, ChartTitle[Wheel], ChartKeys[Wheel]);
  NewMenu(cha, ChartTitle[Grid], ChartKeys[Grid]);
  NewMenu(cha, ChartTitle[Aspect], ChartKeys[Aspect]);
  NewMenu(cha, ChartTitle[MidPoint], ChartKeys[MidPoint]);
  NewMenu(cha, ChartTitle[Horizon], ChartKeys[Horizon]);
  NewMenu(cha, ChartTitle[Orbit], ChartKeys[Orbit]);
  NewMenu(cha, ChartTitle[Sector], ChartKeys[Sector]);
  NewMenu(cha, ChartTitle[Calendar], ChartKeys[Calendar]);
  NewMenu(cha, ChartTitle[Influence], ChartKeys[Influence]);
  NewMenu(cha, ChartTitle[AstroGraph], ChartKeys[AstroGraph]);
  NewMenu(cha, ChartTitle[Ephemeris], ChartKeys[Ephemeris]);
#ifdef ARABIC
  NewMenu(cha, ChartTitle[Arabic], ChartKeys[Arabic]);
#endif
  NewMenu(cha, ChartTitle[Rising], ChartKeys[Rising]);
  cha->insertSeparator();
  NewMenu(cha, ChartTitle[Transit], ChartKeys[Transit]);
  NewMenu(cha, ChartTitle[Progress], ChartKeys[Progress]);
  cha->insertSeparator();
  NewMenu(cha, transl("Chart Settings..."), ALT + SHIFT + Key_C);
  insertItem(transl("Chart"), cha);
  NewMenu(gra, ChartTitle[Map], ChartKeys[Map]);
  NewMenu(gra, ChartTitle[Globe], ChartKeys[Globe]);
  NewMenu(gra, ChartTitle[Polar], ChartKeys[Polar]);
#ifdef CONSTEL
  NewMenu(gra, transl("Show Constellations"), SHIFT + Key_F);
#endif
  gra->insertSeparator();
  NewMenu(gra, transl("Reverse Background"), Key_X);
  NewMenu(gra, transl("Monochrome"), Key_M);
  NewMenu(gra, transl("Show Border"), Key_B);
  NewMenu(gra, transl("Show Chart Info"), Key_T);
  NewMenu(gra, transl("Show Info SideBar"), ALT + Key_T);
  NewMenu(gra, transl("Show Glyph Labels"), Key_L);
  NewMenu(gra, transl("Square Screen"), CTRL + ALT + Key_Q);
  NewMenu(chsc, transl("Small"), nScaleType[Small]);
  NewMenu(chsc, transl("Medium"), nScaleType[Medium]);
  NewMenu(chsc, transl("Large"), nScaleType[Large]);
  NewMenu(chsc, transl("Huge"), nScaleType[Huge]);
  chsc->insertSeparator();
  NewMenu(chsc, transl("Decrease"), Key_Less);
  NewMenu(chsc, transl("Increase"), Key_Greater);
  gra->insertItem(transl("Character Scale"), chsc);
  NewMenu(glo, transl("Set to Zero"), ALT + SHIFT + Key_E);
  glo->insertSeparator();
  NewMenu(glo, transl("Decrease"), Key_BracketLeft);
  NewMenu(glo, transl("Increase"), Key_BracketRight);
  gra->insertItem(transl("Globe Tilt"), glo);
  gra->insertSeparator();
  NewMenu(gra, transl("Modify Display"), Key_I);
  NewMenu(gra, transl("Modify Chart"), SHIFT + Key_O);
  //NewMenu(gra, transl("Scribble Color"), CTRL + Key_Z); This function is not clear for the moment
  NewMenu(gra, transl("Graphic Settings..."), ALT + SHIFT + Key_G);
  insertItem(transl("Graphics"), gra);
  NewMenu(anim, transl("Stop Animation"), SHIFT + Key_N);
  NewMenu(jrate, transl("Update to Now"), CTRL + ALT + Key_N);
  jrate->insertSeparator();
  NewMenu(jrate, transl("Seconds"), JRateKeys[Seconds]);
  NewMenu(jrate, transl("Minutes"), JRateKeys[Minutes]);
  NewMenu(jrate, transl("Hours"), JRateKeys[Hours]);
  NewMenu(jrate, transl("Days"), JRateKeys[Days]);
  NewMenu(jrate, transl("Months"), JRateKeys[Months]);
  NewMenu(jrate, transl("Years"), JRateKeys[Years]);
  NewMenu(jrate, transl("Decades"), JRateKeys[Decades]);
  NewMenu(jrate, transl("Centuries"), JRateKeys[Centuries]);
  NewMenu(jrate, transl("Millenia"), JRateKeys[Millenia]);
  anim->insertItem(transl("Jump Rate"), jrate);
  NewMenu(jfact, transl("1 Unit"), JFactKeys[Unit_1]);
  NewMenu(jfact, transl("2 Units"), JFactKeys[Unit_2]);
  NewMenu(jfact, transl("3 Units"), JFactKeys[Unit_3]);
  NewMenu(jfact, transl("4 Units"), JFactKeys[Unit_4]);
  NewMenu(jfact, transl("5 Units"), JFactKeys[Unit_5]);
  NewMenu(jfact, transl("6 Units"), JFactKeys[Unit_6]);
  NewMenu(jfact, transl("7 Units"), JFactKeys[Unit_7]);
  NewMenu(jfact, transl("8 Units"), JFactKeys[Unit_8]);
  NewMenu(jfact, transl("9 Units"), JFactKeys[Unit_9]);
  anim->insertItem(transl("Jump Factor"), jfact);
  NewMenu(anim, transl("Reverse Direction"), Key_R);
  NewMenu(anim, transl("Pause Animation"), Key_P);
  NewMenu(anim, transl("Timed Exposure"), Key_J);
  anim->insertSeparator();
  NewMenu(anim, transl("Step Forward"), Key_Plus);
  NewMenu(anim, transl("Step Backward"), Key_Minus);
  NewMenu(anim, transl("Store Chart Info"), Key_O);
  NewMenu(anim, transl("Recall Chart Info"), SHIFT + Key_O);
  insertItem(transl("Animation"), anim);
  NewMenu(doc, transl("Open Defaults"), CTRL + Key_ParenRight);
  NewMenu(doc, transl("Open Summary"), CTRL + Key_Exclam);
  NewMenu(doc, transl("Open Readme"), CTRL + Key_At);
  NewMenu(doc, transl("Open Update"), CTRL + Key_Percent);
  NewMenu(doc, transl("Helpfile"), CTRL + Key_Dollar);
  NewMenu(doc, transl("Kastrolog Specific Help"), CTRL + Key_Ampersand);
  doc->insertSeparator();
  NewMenu(doc, transl("Open HomePage"), Key_ParenRight);
  hlp->insertItem(transl("Documentation"), doc);
  NewMenu(hlp, ChartTitle[Signs], ChartKeys[Signs]);
  NewMenu(hlp, ChartTitle[Objects], ChartKeys[Objects]);
  NewMenu(hlp, ChartTitle[Aspects], ChartKeys[Aspects]);
  NewMenu(hlp, ChartTitle[Constell], ChartKeys[Constell]);
  NewMenu(hlp, ChartTitle[Planets], ChartKeys[Planets]);
  NewMenu(hlp, ChartTitle[GenMean], ChartKeys[GenMean]);
  NewMenu(hlp, ChartTitle[Flags], ChartKeys[Flags]);
  NewMenu(hlp, ChartTitle[SecFlags], ChartKeys[SecFlags]);
  NewMenu(hlp, ChartTitle[Keyboard], ChartKeys[Keyboard]);
  hlp->insertSeparator();
  NewMenu(hlp, transl("About KAstrolog..."), CTRL + Key_O);
  insertItem(transl("help"), hlp);
  connect(this, SIGNAL(activated(int)), this, SLOT(menuslot(int)));
  iftog(us.fGraphics, Key_V);
  iftog(us.fAnsiColor, ALT + Key_K);
#ifdef INTERPRET
  iftog(us.fInterpret, ALT + SHIFT + Key_I);
#endif
  iftog(us.fSeconds, ALT + Key_B);
  iftog(us.fAppSep, ALT + Key_X);
  iftog(us.fParallel, ALT + SHIFT + Key_X);
  iftog(us.fSidereal, Key_S);
  iftog(us.objCenter == oSun, Key_H); 
  iftog(us.fSolarArc, ALT +Key_1);
  iftog(us.fDecan, Key_G);
  iftog(us.fFlip, Key_F);
  iftog(us.fGeodetic, ALT + SHIFT + Key_H);
  iftog(us.fVedic, Key_Z);
  iftog(us.fNavamsa, Key_Y);
  iftog(minors, Key_R);
  iftog(us.fCusp, SHIFT + Key_C);
  iftog(us.fUranian, Key_U);
  iftog(us.nStar, SHIFT + Key_U);
#else // KDE2
  NewMenu(fi, transl("New Window..."), Qt::ALT + Qt::Key_Exclam);
  NewMenu(fi, transl("Open Chart..."), Qt::ALT + Qt::Key_O);
  NewMenu(fi, transl("Open Chart #2..."), Qt::ALT + Qt::SHIFT + Qt::Key_O);
  fi->insertSeparator();
  NewMenu(fi, transl("Save Chart Info..."), Qt::ALT + Qt::Key_W);
  NewMenu(fi, transl("Save Chart Positions..."), Qt::ALT + Qt::SHIFT + Qt::Key_W);
  NewMenu(fi, transl("Save Chart Text Output..."), Qt::CTRL + Qt::Key_5);
  NewMenu(fi, transl("Save Chart Bitmap..."), Qt::CTRL + Qt::Key_6);
#ifdef META
  NewMenu(fi, transl("Save Chart Picture..."), Qt::CTRL + Qt::Key_7);
#endif
#ifdef PS
  NewMenu(fi, transl("Save Chart Postscript..."), Qt::CTRL + Qt::Key_8);
#endif
  fi->insertSeparator();
  NewMenu(fi, transl("Save Settings..."), Qt::CTRL + Qt::Key_9);
  fi->insertSeparator();
  NewMenu(fi, transl("Print..."), Qt::CTRL + Qt::Key_P);
  NewMenu(fi, transl("Print Setup..."), Qt::CTRL + Qt::Key_Q);
  if(AstroApp.nbchilds > 1)
    NewMenu(fi, transl("Close"), Qt::ALT + Qt::Key_C);
  NewMenu(fi, transl("Quit..."), Qt::Key_Q);
  insertItem(transl("File"), fi);
  NewMenu(ed, transl("Command Line..."), Qt::Key_Return);
  ed->insertSeparator();
  while(i <= 12)
    {
      MenuMac;
      NewMenu(m1, buf, Qt::Key_F1 + i - 1);
      i++;
    }
  ed->insertItem(transl("Run Macros (Normal Set)"), m1);
  while(i <= 24)
    {
      MenuMac;
      NewMenu(m2, buf, Qt::SHIFT + Qt::Key_F1 + i - 13);
      i++;
    }
  ed->insertItem(transl("Run Macros (Shift Set)"), m2);
  while(i <= 36)
    {
      MenuMac;
      NewMenu(m3, buf, Qt::CTRL + Qt::Key_F1 + i - 25);
      i++;
    }
  ed->insertItem(transl("Run Macros (Control Set)"), m3);
  while(i <= 48)
    {
      MenuMac;
      NewMenu(m4, buf, Qt::ALT + Qt::Key_F1 + i - 37);
      i++;
    }
  ed->insertItem(transl("Run Macros (Alt Set)"), m4);
  //ed->insertSeparator();
  insertItem(transl("Edit"), ed);
  NewMenu(view, transl("Show Graphic..."), Qt::Key_V);
  view->insertItem(transl("Window Settings"), ws);
  NewMenu(ws, transl("Buffer Redraws"), Qt::Key_Tab);
  NewMenu(ws, transl("Redraw Screen"), Qt::Key_Space);
  NewMenu(ws, transl("Clear Screen"), Qt::Key_Delete);
  ws->insertSeparator();
  NewMenu(ws, transl("Window Resizes Chart"), Qt::ALT + Qt::Key_Q);
  NewMenu(ws, transl("Page Up"), Qt::Key_PageUp);
  NewMenu(ws, transl("Page Down"), Qt::Key_PageDown);
  NewMenu(ws, transl("Home"), Qt::Key_Home);
  NewMenu(ws, transl("End"), Qt::Key_End);
  NewMenu(view, transl("Colored Text"), Qt::ALT + Qt::Key_K);
  NewMenu(view, transl("Set Palette..."), Qt::ALT +Qt::SHIFT + Qt::Key_K);
  NewMenu(view, transl("Set Colors..."), Qt::ALT + Qt::SHIFT + Qt::Key_Percent);
#ifdef INTERPRET
  NewMenu(view, transl("Show Interpretation"), Qt::ALT + Qt::SHIFT + Qt::Key_I);
#endif
  NewMenu(view, transl("Print Nearest second"), Qt::ALT + Qt::Key_B);
  NewMenu(view, transl("Applying Aspects"), Qt::ALT + Qt::Key_X);
  NewMenu(view, transl("Parallel Aspects"), Qt::ALT + Qt::SHIFT + Qt::Key_X);
  insertItem(transl("View"), view);
  NewMenu(info, transl("Set Chart Info..."), Qt::ALT + Qt::Key_Percent);
  NewMenu(info, transl("Chart For Now"), Qt::Key_N);
  NewMenu(info, transl("Default Chart Info..."), Qt::ALT +Qt::Key_D);
  info->insertSeparator();
  NewMenu(info, transl("Set Chart #2 Info..."), Qt::ALT + Qt::SHIFT +Qt::Key_Z);
  NewMenu(ch3, transl("Set Chart #3 Info..."), Qt::SHIFT +Qt::Key_3);
  NewMenu(ch3, transl("Open Chart #4 ..."), Qt::ALT + Qt::SHIFT + Qt::Key_3);
  info->insertItem(transl("Chart #3"), ch3);
  NewMenu(ch4, transl("Set Chart #4 Info..."), Qt::SHIFT + Qt::Key_4);
  NewMenu(ch4, transl("Open Chart #4 ..."), Qt::ALT + Qt::SHIFT + Qt::Key_4);
  info->insertItem(transl("Chart #4"), ch4);
  info->insertSeparator();
  NewMenu(info, ChartTitle[Comparison], Qt::Key_C);
  NewMenu(info, ChartTitle[Synastry], Qt::ALT + Qt::Key_Y);
  NewMenu(info, ChartTitle[Composite], Qt::ALT + Qt::SHIFT + Qt::Key_Y);
  NewMenu(info, ChartTitle[Time_Space], Qt::ALT + Qt::SHIFT +Qt::Key_M);
  info->insertSeparator();
  NewMenu(info, ChartTitle[Date_Diff], Qt::ALT + Qt::SHIFT + Qt::Key_D);
#ifdef BIORHYTHM
  NewMenu(info, ChartTitle[Biorhythm], Qt::SHIFT + Qt::Key_Y);
#endif
  NewMenu(info, ChartTitle[Transit_Natal], Qt::ALT + Qt::Key_N);
  NewMenu(info, ChartTitle[Progressed_Natal], Qt::ALT + Qt::SHIFT + Qt::Key_N);
  insertItem(transl("Info"), info);
  NewMenu(sett, transl("Sidereal Zodiac"), Qt::Key_S);
  NewMenu(sett, transl("Heliocentric"), Qt::Key_H);
  NewMenu(house, szSystem[0], Qt::CTRL+ Qt::SHIFT+ Qt::Key_P);
  NewMenu(house, szSystem[1], Qt::CTRL + Qt::SHIFT+ Qt::Key_K);
  NewMenu(house, szSystem[2], Qt::CTRL + Qt::SHIFT+ Qt::Key_E);
  NewMenu(house, szSystem[3], Qt::CTRL + Qt::SHIFT+ Qt::Key_C);
  NewMenu(house, szSystem[4], Qt::CTRL + Qt::SHIFT+ Qt::Key_M);
  NewMenu(house, szSystem[5], Qt::CTRL + Qt::SHIFT+ Qt::Key_R);
  NewMenu(house, szSystem[6], Qt::CTRL + Qt::SHIFT+ Qt::Key_Y);
  NewMenu(house, szSystem[7], Qt::CTRL + Qt::SHIFT+ Qt::Key_U);
  NewMenu(house, szSystem[8], Qt::CTRL + Qt::SHIFT+ Qt::Key_T);
  NewMenu(house, szSystem[9], Qt::CTRL + Qt::SHIFT+ Qt::Key_A);
  NewMenu(house, szSystem[10], Qt::CTRL + Qt::SHIFT+ Qt::Key_Q);
  NewMenu(house, szSystem[11], Qt::CTRL + Qt::SHIFT+ Qt::Key_O);
  NewMenu(house, szSystem[12], Qt::CTRL + Qt::SHIFT+ Qt::Key_W);
  NewMenu(house, szSystem[13], Qt::CTRL + Qt::SHIFT+ Qt::Key_V);
  NewMenu(house, szSystem[14], Qt::CTRL + Qt::SHIFT+ Qt::Key_N);
  sett->insertItem(transl("House System"), house);
  NewMenu(hset, transl("Solar Chart"), Qt::ALT + Qt::Key_1);
  NewMenu(hset, transl("Show Decan"), Qt::Key_G);
  NewMenu(hset, transl("Flip Signs & Houses"), Qt::Key_F);
  NewMenu(hset, transl("Geodetic Houses"), Qt::ALT + Qt::SHIFT + Qt::Key_H);
  hset->insertSeparator();
  NewMenu(hset, transl("Vedic Wheel Display"), Qt::Key_Z);
  NewMenu(hset, transl("Show Navamsas"), Qt::Key_Y);
  sett->insertItem(transl("House Settings"), hset);
  NewMenu(sett, transl("Aspects Settings..."), Qt::ALT + Qt::SHIFT + Qt::Key_A);
  NewMenu(sett, transl("Objects Settings..."), Qt::ALT + Qt::Key_J);
  NewMenu(sett, transl("21-40 Objects Settings..."), Qt::ALT + Qt::SHIFT + Qt::Key_J);
  NewMenu(sett, transl("41-60 Objects Settings..."), Qt::CTRL + Qt::ALT + Qt::SHIFT + Qt::Key_J);
  NewMenu(sett, transl("61-80 Objects Settings..."), Qt::CTRL + Qt::SHIFT + Qt::Key_J);
  sett->insertSeparator();
  NewMenu(sett, transl("Restrictions..."), Qt::ALT + Qt::Key_R);
  NewMenu(sett, transl("Include Minors"), Qt::SHIFT + Qt::Key_R);
  NewMenu(sett, transl("Include Cusps"), Qt::SHIFT + Qt::Key_C);
  NewMenu(sett, transl("Include Uranians"), Qt::Key_U);
  NewMenu(sett, transl("Include Fixed Stars"), Qt::SHIFT + Qt::Key_U);
  NewMenu(sett, transl("Star Restrictions..."), Qt::ALT + Qt::SHIFT + Qt::Key_R);
  NewMenu(sett, transl("Transit Restrictions..."), Qt::ALT + Qt::SHIFT + Qt::Key_F);
  sett->insertSeparator();
  NewMenu(sett, transl("Calculation Settings..."), Qt::ALT + Qt::SHIFT + Qt::Key_S);
  NewMenu(sett, transl("Obscure Settings..."), Qt::ALT + Qt::SHIFT + Qt::Key_B);
  insertItem(transl("Settings"), sett);
  NewMenu(cha, ChartTitle[List], ChartKeys[List]);
  NewMenu(cha, ChartTitle[Wheel], ChartKeys[Wheel]);
  NewMenu(cha, ChartTitle[Grid], ChartKeys[Grid]);
  NewMenu(cha, ChartTitle[Aspect], ChartKeys[Aspect]);
  NewMenu(cha, ChartTitle[MidPoint], ChartKeys[MidPoint]);
  NewMenu(cha, ChartTitle[Horizon], ChartKeys[Horizon]);
  NewMenu(cha, ChartTitle[Orbit], ChartKeys[Orbit]);
  NewMenu(cha, ChartTitle[Sector], ChartKeys[Sector]);
  NewMenu(cha, ChartTitle[Calendar], ChartKeys[Calendar]);
  NewMenu(cha, ChartTitle[Influence], ChartKeys[Influence]);
  NewMenu(cha, ChartTitle[AstroGraph], ChartKeys[AstroGraph]);
  NewMenu(cha, ChartTitle[Ephemeris], ChartKeys[Ephemeris]);
#ifdef ARABIC
  NewMenu(cha, ChartTitle[Arabic], ChartKeys[Arabic]);
#endif
  NewMenu(cha, ChartTitle[Rising], ChartKeys[Rising]);
  cha->insertSeparator();
  NewMenu(cha, ChartTitle[Transit], ChartKeys[Transit]);
  NewMenu(cha, ChartTitle[Progress], ChartKeys[Progress]);
  cha->insertSeparator();
  NewMenu(cha, transl("Chart Settings..."), Qt::ALT + Qt::SHIFT + Qt::Key_C);
  insertItem(transl("Chart"), cha);
  NewMenu(gra, ChartTitle[Map], ChartKeys[Map]);
  NewMenu(gra, ChartTitle[Globe], ChartKeys[Globe]);
  NewMenu(gra, ChartTitle[Polar], ChartKeys[Polar]);
#ifdef CONSTEL
  NewMenu(gra, transl("Show Constellations"), Qt::SHIFT + Qt::Key_F);
#endif
  gra->insertSeparator();
  NewMenu(gra, transl("Reverse Background"), Qt::Key_X);
  NewMenu(gra, transl("Monochrome"), Qt::Key_M);
  NewMenu(gra, transl("Show Border"), Qt::Key_B);
  NewMenu(gra, transl("Show Chart Info"), Qt::Key_T);
  NewMenu(gra, transl("Show Info SideBar"), Qt::ALT + Qt::Key_T);
  NewMenu(gra, transl("Show Glyph Labels"), Qt::Key_L);
  NewMenu(gra, transl("Square Screen"), Qt::CTRL + Qt::ALT + Qt::Key_Q);
  NewMenu(chsc, transl("Small"), nScaleType[Small]);
  NewMenu(chsc, transl("Medium"), nScaleType[Medium]);
  NewMenu(chsc, transl("Large"), nScaleType[Large]);
  NewMenu(chsc, transl("Huge"), nScaleType[Huge]);
  chsc->insertSeparator();
  NewMenu(chsc, transl("Decrease"), Qt::Key_Less);
  NewMenu(chsc, transl("Increase"), Qt::Key_Greater);
  gra->insertItem(transl("Character Scale"), chsc);
  NewMenu(glo, transl("Set to Zero"), Qt::ALT + Qt::SHIFT + Qt::Key_E);
  glo->insertSeparator();
  NewMenu(glo, transl("Decrease"), Qt::Key_BracketLeft);
  NewMenu(glo, transl("Increase"), Qt::Key_BracketRight);
  gra->insertItem(transl("Globe Tilt"), glo);
  gra->insertSeparator();
  NewMenu(gra, transl("Modify Display"), Qt::Key_I);
  NewMenu(gra, transl("Modify Chart"), Qt::SHIFT + Qt::Key_O);
  //NewMenu(gra, transl("Scribble Color"), Qt::CTRL + Qt::Key_Z); This function is not clear for the moment
  NewMenu(gra, transl("Graphic Settings..."), Qt::ALT + Qt::SHIFT + Qt::Key_G);
  insertItem(transl("Graphics"), gra);
  NewMenu(anim, transl("Stop Animation"), Qt::SHIFT + Qt::Key_N);
  NewMenu(jrate, transl("Update to Now"), Qt::CTRL + Qt::ALT + Qt::Key_N);
  jrate->insertSeparator();
  NewMenu(jrate, transl("Seconds"), JRateKeys[Seconds]);
  NewMenu(jrate, transl("Minutes"), JRateKeys[Minutes]);
  NewMenu(jrate, transl("Hours"), JRateKeys[Hours]);
  NewMenu(jrate, transl("Days"), JRateKeys[Days]);
  NewMenu(jrate, transl("Months"), JRateKeys[Months]);
  NewMenu(jrate, transl("Years"), JRateKeys[Years]);
  NewMenu(jrate, transl("Decades"), JRateKeys[Decades]);
  NewMenu(jrate, transl("Centuries"), JRateKeys[Centuries]);
  NewMenu(jrate, transl("Millenia"), JRateKeys[Millenia]);
  anim->insertItem(transl("Jump Rate"), jrate);
  NewMenu(jfact, transl("1 Unit"), JFactKeys[Unit_1]);
  NewMenu(jfact, transl("2 Units"), JFactKeys[Unit_2]);
  NewMenu(jfact, transl("3 Units"), JFactKeys[Unit_3]);
  NewMenu(jfact, transl("4 Units"), JFactKeys[Unit_4]);
  NewMenu(jfact, transl("5 Units"), JFactKeys[Unit_5]);
  NewMenu(jfact, transl("6 Units"), JFactKeys[Unit_6]);
  NewMenu(jfact, transl("7 Units"), JFactKeys[Unit_7]);
  NewMenu(jfact, transl("8 Units"), JFactKeys[Unit_8]);
  NewMenu(jfact, transl("9 Units"), JFactKeys[Unit_9]);
  anim->insertItem(transl("Jump Factor"), jfact);
  NewMenu(anim, transl("Reverse Direction"), Qt::Key_R);
  NewMenu(anim, transl("Pause Animation"), Qt::Key_P);
  NewMenu(anim, transl("Timed Exposure"), Qt::Key_J);
  anim->insertSeparator();
  NewMenu(anim, transl("Step Forward"), Qt::Key_Plus);
  NewMenu(anim, transl("Step Backward"), Qt::Key_Minus);
  NewMenu(anim, transl("Store Chart Info"), Qt::Key_O);
  NewMenu(anim, transl("Recall Chart Info"), Qt::SHIFT + Qt::Key_O);
  insertItem(transl("Animation"), anim);
  NewMenu(doc, transl("Open Defaults"), Qt::CTRL + Qt::Key_ParenRight);
  NewMenu(doc, transl("Open Summary"), Qt::CTRL + Qt::Key_Exclam);
  NewMenu(doc, transl("Open Readme"), Qt::CTRL + Qt::Key_At);
  NewMenu(doc, transl("Open Update"), Qt::CTRL + Qt::Key_Percent);
  NewMenu(doc, transl("Helpfile"), Qt::CTRL + Qt::Key_Dollar);
  NewMenu(doc, transl("Kastrolog Specific Help"), Qt::CTRL + Qt::Key_Ampersand);
  doc->insertSeparator();
  NewMenu(doc, transl("Open HomePage"), Qt::Key_ParenRight);
  hlp->insertItem(transl("Documentation"), doc);
  NewMenu(hlp, ChartTitle[Signs], ChartKeys[Signs]);
  NewMenu(hlp, ChartTitle[Objects], ChartKeys[Objects]);
  NewMenu(hlp, ChartTitle[Aspects], ChartKeys[Aspects]);
  NewMenu(hlp, ChartTitle[Constell], ChartKeys[Constell]);
  NewMenu(hlp, ChartTitle[Planets], ChartKeys[Planets]);
  NewMenu(hlp, ChartTitle[GenMean], ChartKeys[GenMean]);
  NewMenu(hlp, ChartTitle[Flags], ChartKeys[Flags]);
  NewMenu(hlp, ChartTitle[SecFlags], ChartKeys[SecFlags]);
  NewMenu(hlp, ChartTitle[Keyboard], ChartKeys[Keyboard]);
  hlp->insertSeparator();
  NewMenu(hlp, transl("About KAstrolog..."), Qt::CTRL + Qt::Key_O);
  insertItem(transl("help"), hlp);
  connect(this, SIGNAL(activated(int)), this, SLOT(menuslot(int)));
  iftog(us.fGraphics, Qt::Key_V);
  iftog(us.fAnsiColor, Qt::ALT + Qt::Key_K);
#ifdef INTERPRET
  iftog(us.fInterpret, Qt::ALT + Qt::SHIFT + Qt::Key_I);
#endif
  iftog(us.fSeconds, Qt::ALT + Qt::Key_B);
  iftog(us.fAppSep, Qt::ALT + Qt::Key_X);
  iftog(us.fParallel, Qt::ALT + Qt::SHIFT + Qt::Key_X);
  iftog(us.fSidereal, Qt::Key_S);
  iftog(us.objCenter == oSun, Qt::Key_H); 
  iftog(us.fSolarArc, Qt::ALT +Qt::Key_1);
  iftog(us.fDecan, Qt::Key_G);
  iftog(us.fFlip, Qt::Key_F);
  iftog(us.fGeodetic, Qt::ALT + Qt::SHIFT + Qt::Key_H);
  iftog(us.fVedic, Qt::Key_Z);
  iftog(us.fNavamsa, Qt::Key_Y);
  iftog(minors, Qt::Key_R);
  iftog(us.fCusp, Qt::SHIFT + Qt::Key_C);
  iftog(us.fUranian, Qt::Key_U);
  iftog(us.nStar, Qt::SHIFT + Qt::Key_U);
#endif // KDE1
  
  if(us.fListing) ToggleType(List);
  else if(us.fWheel) ToggleType(Wheel);
  else if(us.fGrid) ToggleType(Grid);
  else if(us.fAspList) ToggleType(Aspect);
  else if(us.fMidpoint) ToggleType(MidPoint);
  else if(us.fHorizon && !us.fHorizonSearch) ToggleType(Horizon);
  else if(us.fOrbit) ToggleType(Orbit);
  else if(us.fSector) ToggleType(Sector);
  else if(us.fCalendar) ToggleType(Calendar);
  else if(us.fInfluence) ToggleType(Influence);
  else if(us.fAstroGraph) ToggleType(AstroGraph);
  else if(us.fEphemeris) ToggleType(Ephemeris);
#ifdef ARABIC
  else if(us.nArabic) ToggleType(Arabic);
#endif
  else if(us.fHorizon && us.fHorizonSearch) ToggleType(Rising);
  else if(us.fTransit) ToggleType(Transit);
  else
  {
    us.fWheel = fTrue; ToggleType(Wheel);
  }

  switch(us.nRel)
  {
    //case 0           : ToggleChart(Comparison); break;
    case '1'          : ToggleChart(Comparison); break;
    case rcSynastry   : ToggleChart(Synastry); break;
    case rcComposite  : ToggleChart(Composite); break;
    case rcMidpoint   : ToggleChart(Time_Space); break;
    case rcDifference : ToggleChart(Date_Diff); break;
    case rcBiorhythm  : ToggleChart(Biorhythm); break;
    case rcTransit    : ToggleChart(Transit_Natal); break;
    case rcProgress   : ToggleChart(Progressed_Natal); break;
    default           : ;
  }
  ToggleHousSys();
#ifdef KDE1
  iftog(gi.nMode == gWorldMap, SHIFT + Key_W);
  iftog(gi.nMode == gGlobe, SHIFT + Key_G);
  iftog(gi.nMode == gPolar, SHIFT + Key_P);
#ifdef CONSTEL
  iftog(gs.fConstel, SHIFT + Key_F);
#endif
  iftog(gs.fInverse, Key_X);
  iftog(!gs.fColor, Key_M);
  iftog(fDrawBorder, Key_B);
  iftog(!fDrawText, Key_T);
  iftog(fSidebar, ALT + Key_T);
  iftog(gs.fLabel, Key_L);
  iftog(fSquare, SHIFT + Key_Q);
  iftog(gs.nScale == 100, nScaleType[Small]);
  iftog(gs.nScale == 200, nScaleType[Medium]);
  iftog(gs.nScale == 300, nScaleType[Large]);
  iftog(gs.nScale == 400, nScaleType[Huge]);
  Toggle(SHIFT + Key_N);
  Toggle(Key_At);
  Toggle(Key_1);
  Toggle(Key_J);
#else //KDE2
iftog(gi.nMode == gWorldMap, Qt::SHIFT + Qt::Key_W);
  iftog(gi.nMode == gGlobe, Qt::SHIFT + Qt::Key_G);
  iftog(gi.nMode == gPolar, Qt::SHIFT + Qt::Key_P);
#ifdef CONSTEL
  iftog(gs.fConstel, Qt::SHIFT + Qt::Key_F);
#endif
  iftog(gs.fInverse, Qt::Key_X);
  iftog(!gs.fColor, Qt::Key_M);
  iftog(fDrawBorder, Qt::Key_B);
  iftog(!fDrawText, Qt::Key_T);
  iftog(fSidebar, Qt::ALT + Qt::Key_T);
  iftog(gs.fLabel, Qt::Key_L);
  iftog(fSquare, Qt::SHIFT + Qt::Key_Q);
  iftog(gs.nScale == 100, nScaleType[Small]);
  iftog(gs.nScale == 200, nScaleType[Medium]);
  iftog(gs.nScale == 300, nScaleType[Large]);
  iftog(gs.nScale == 400, nScaleType[Huge]);
  Toggle(Qt::SHIFT + Qt::Key_N);
  Toggle(Qt::Key_At);
  Toggle(Qt::Key_1);
  Toggle(Qt::Key_J);
#endif // KDE2
  ToggleJRate(Minutes);
  setItemChecked(JFactKeys[Unit_1], TRUE);
  nbjmp->setItemChecked(Unit_1, TRUE);
}

void AstroMenu::Toggle(int i)
{
    setItemChecked(i, (isItemChecked(i)) ? FALSE : TRUE);
}

void AstroMenu::ToggleHousSys()
{
  int i;
  for(i = 0; i < 15; i++)
#ifdef KDE1
    setItemChecked(CTRL + SHIFT + HousSysKeys[i], (i == us.nHouseSystem) ? TRUE : FALSE);
#else
  setItemChecked(Qt::CTRL + Qt::SHIFT + HousSysKeys[i], (i == us.nHouseSystem) ? TRUE : FALSE);
#endif
}

void AstroMenu::menuslot(int i)
{
  switch(i)
  {
#ifdef KDE1
    case ALT + Key_Exclam    : AstroApp.newtask(); break;        // Open a new window
    case ALT + Key_O         : central->Inputfile(ciMain); break; // Open Chart
    case ALT + SHIFT + Key_O : central->Inputfile(ciTwin); break; // Open Chart #2
    case ALT + Key_W         : central->OutputFile(); break; // Save Chart Info
    case ALT + SHIFT + Key_W : central->OutputFile(SaveOutput); break; // Save Output pos
    case Qt::CTRL + Key_5        : central->OutputFile(SaveText); break; // Save Text
    case CTRL + Key_6        : central->OutputFile(SaveBitmap); break; // Save Bitmap
#ifdef META
    case CTRL + Key_7        : central->OutputFile(SavePicture); break; // Save WMF
#endif
#ifdef PS
    case CTRL + Key_8        : central->OutputFile(SavePostscript); break; // SAve PostScript
#endif
    case CTRL + Key_9        : central->OutputFile(SaveSettings); break; // Save settings
    case CTRL + Key_Q        : central->PrintDialog(); break; // Display a print dialog
    case CTRL + Key_P        : central->Print(); break; // Print current chart

    case ALT + Key_C         : AstroApp.bye(FALSE); break; // Quit the current task
    case Key_Q               : AstroApp.bye(TRUE); break; // exit from all
    case Key_Return          : central->Command(); break; // Wait for a line of commands and execute it
      // !!! gcc feature only !!!
  case Key_F1 ... Key_F12  : central->RunMacro(i-Key_F1); break; // execute macros from F1 to F12

    case SHIFT + Key_F1 ... SHIFT + Key_F12 : central->RunMacro(i-(SHIFT+Key_F1)+12); break; //execute macros from Shift F1 to Shift F12
   case CTRL + Key_F1 ... CTRL + Key_F12 : central->RunMacro(i-(CTRL+Key_F1)+24); break; // Run macros from Ctrl F1 to Ctrl F12 
   
   case ALT + Key_F1 ... ALT + Key_F12 : central->RunMacro(i-(ALT+Key_F1)+36); break; // Run macros from Alt F1 to Alt F12 
  case Key_V                : Toggle(i); not2(us.fGraphics); central->GraphicsText(); break; // Graphic <-> text modes selection
   case ALT + Key_Q          : Toggle(i); central->ResizeChart(); break;
   case Key_PageUp           : central->Scroll(DirUp); break;
   case Key_PageDown         : central->Scroll(DirDown); break;
   case Key_Home             : central->Scroll(DirTop); break;
   case Key_End              : central->Scroll(DirBottom); break;
   case ALT + Key_K          : Toggle(i); central->ColoredText(); break;
   case ALT + SHIFT + Key_K  : central->SetPalette(); break;
   case ALT + SHIFT + Key_Percent : central->SetEAColors(); break;
#ifdef INTERPRET
   case ALT + SHIFT +Key_I   : Toggle(i); not2(us.fInterpret);
                               if(us.fInterpret && us.fGraphics)
			       {
				 Toggle(Key_V);
				 not2(us.fGraphics);
				 central->GraphicsText();
			       }
			       central->ShowInterp();
			       break;
#endif
  case ALT + Key_B         : Toggle(i); not2(us.fSeconds); is.fSeconds = us.fSeconds; central->setPaint(' '); break;
   case ALT + Key_X         : Toggle(i); not2(us.fAppSep); central->setPaint(' ');  break;
   case ALT + Key_Percent   : central->SetChartInfo(ciMain); break;
   case ALT + SHIFT +Key_X  : SwitchAction(us.fParallel); break;
   case ALT +Key_D          : central->SetChartDefaultInfo(); break;
   case ALT + SHIFT + Key_Z : central->SetChartInfo(ciTwin); break;
   case SHIFT + Key_3       : central->SetChartInfo(ciThre); break;
   case ALT + SHIFT + Key_3 : central->Inputfile(ciThre); break;
   case SHIFT + Key_4       : central->SetChartInfo(ciFour); break;
   case ALT + SHIFT + Key_4 : central->Inputfile(ciFour); break;
   case Key_C               : ToggleChart(Comparison);  central->SetRelation(0); break;
   case ALT + Key_Y         : ToggleChart(Synastry); central->SetRelation(rcSynastry); break;
   case ALT + SHIFT + Key_Y : ToggleChart(Composite); central->SetRelation(rcComposite); break;
   case ALT + SHIFT + Key_M : ToggleChart(Time_Space); central->SetRelation(rcMidpoint); break;
   case ALT + SHIFT + Key_D : ToggleChart(Date_Diff); central->SetRelation(rcDifference); break;
#ifdef BIORHYTHM
   case SHIFT + Key_Y       : ToggleChart(Biorhythm); central->SetRelation(rcBiorhythm); break;
#endif
   case ALT + Key_N         : ToggleChart(Transit_Natal); central->SetRelation(rcTransit); break;
   case ALT + SHIFT + Key_N : ToggleChart(Progressed_Natal); central->SetRelation(rcProgress); break;
   case Key_S               : Toggle(Key_S); central->setPaint(tolower(i)); break;
   case Key_H               : Toggle(Key_H); central->setPaint(tolower(i)); break;
   seths(Key_P, 0);
   seths(Key_K, 1);
   seths(Key_E, 2);
   seths(Key_C, 3);
   seths(Key_M, 4);
   seths(Key_R, 5);
   seths(Key_Y, 6);
   seths(Key_U, 7);
   seths(Key_T, 8);
   seths(Key_A, 9);
   seths(Key_Q, 10);
   seths(Key_O, 11);
   seths(Key_W, 12);
   seths(Key_V, 13);
   seths(Key_N, 14);
   case ALT + Key_1         : Toggle(i); central->setPaint(ALT + Key_1); break;
   case Key_G               : Toggle(i); central->setPaint(tolower(i)); break;
   case Key_F               : Toggle(i); central->setPaint(tolower(i)); break;
   case ALT + SHIFT + Key_H : Toggle(i); central->setPaint(ALT + SHIFT + Key_H); break;
   case Key_Z               : Toggle(i); central->setPaint(tolower(i)); break;
   case Key_Y               : Toggle(i); central->setPaint(tolower(i)); break;
   case ALT + SHIFT + Key_A : central->AspectSettings(); break;
   case ALT + Key_J         : central->ObjectSettings(transl("Object Settings..."),1, 0); break;
   case ALT + SHIFT + Key_J : central->ObjectSettings(transl("Cusp & Uranian Settings"), 21, 0); break;
   case CTRL + ALT + SHIFT + Key_J : central->ObjectSettings(transl("Asteroid Settings"), 41, 0); break;
   case CTRL + SHIFT + Key_J : central->ObjectSettings(transl("Asteroid Settings"), 61, 0); break;
   case ALT + Key_R         : central->Restrict(transl("Object Restrictions"), ignore, ignore2); break;
   case SHIFT + Key_R       : Toggle(i); not2(minors); central->ToggleObj(oPlu, oEP, !minors); break;
   case SHIFT + Key_C       : Toggle(i); not2(us.fCusp); central->ToggleObj(cuspLo-1, cuspHi, !us.fCusp); break;
   case Key_U               : Toggle(i); not2(us.fUranian); central->ToggleObj(uranLo-1, uranHi, !us.fUranian); break;
   case SHIFT + Key_U       : Toggle(i); not2(us.nStar); central->setPaint();  break;
   case ALT + SHIFT + Key_R : central->RestrictStar(); break;
   case ALT + SHIFT + Key_F : central->Restrict(transl("Transit Object Restrictions"), ignore2, ignore); break;
   case ALT + SHIFT + Key_S : central->CalculationSettings(); break;
   case ALT + SHIFT + Key_B : central->ObscureSetting(); break;
   case SHIFT + Key_V       : chtype(List, fListing, 'v');
   case ALT + SHIFT + Key_V : chtype(Wheel, fWheel, 'V');
   case SHIFT + Key_A       : chtype(Grid, fGrid, 'A');
   case ALT + Key_L         : chtype(Aspect, fAspList, ' ');
   case ALT + Key_M         : chtype(MidPoint, fMidpoint, ' ');
   case SHIFT + Key_Z       : chtype(Horizon, fHorizon, 'Z');
   case SHIFT + Key_S       : chtype(Orbit, fOrbit, 'S');
   case SHIFT + Key_M       : chtype(Sector, fSector, 'M');
   case SHIFT + Key_K       : chtype(Calendar, fCalendar, 'K');
   case SHIFT + Key_I       : chtype(Influence, fInfluence, 'J');
   case SHIFT + Key_L       : chtype(AstroGraph, fAstroGraph, 'L');
   case SHIFT + Key_E       : chtype(Ephemeris, fEphemeris, 'E');
#ifdef ARABIC
   case ALT + Key_P         : chtype(Arabic, nArabic, ' ');
#endif
   case ALT + SHIFT + Key_L : chtype(Rising, fHorizon, ' ');
   case ALT + SHIFT + Key_T : if(central->Transits()) ToggleType(Transit); break;
   case ALT + SHIFT + Key_P : if(central->Progressions()) ToggleType(Progress); break;
   case ALT + SHIFT + Key_C : central->ChartSettings(); break;
   case SHIFT + Key_W       : ToggleGraph(Map); central->ShowGlobe(gWorldMap); break;
   case SHIFT + Key_G       : ToggleGraph(Globe); central->ShowGlobe(gGlobe); break;
   case SHIFT + Key_P       : ToggleGraph(Polar); central->ShowGlobe(gPolar); break;
#ifdef CONSTEL
   case SHIFT + Key_F       : SwitchAction(gs.fConstel); break;
#endif
   case Key_X               : Toggle(i); central->setPaint(tolower(i)); break;
   case Key_M               : Toggle(i); central->setPaint(tolower(i)); break;
   case Key_B               : Toggle(i); central->setPaint(tolower(i)); break;
   case Key_T               : Toggle(i); central->setPaint(tolower(i)); break;
   case Key_L               : Toggle(i); central->setPaint(tolower(i)); break;
   case ALT + Key_T         : not2(us.fVelocity); Toggle(i); central->setPaint('b'); break;
   case SHIFT + Key_Q       : Toggle(i); central->setPaint(tolower(i)); break;
   case CTRL + Key_1        : TogglenScale(100); break;
   case CTRL + Key_2        : TogglenScale(200); break;
   case CTRL + Key_3        : TogglenScale(300); break;
   case CTRL + Key_4        : TogglenScale(400); break;
   case Key_Less            : if(gs.nScale >= 200) { gs.nScale -= 100; TogglenScale(gs.nScale); } break;      
   case Key_Greater         : if(gs.nScale <= 300) { gs.nScale += 100; TogglenScale(gs.nScale); } break;
   case ALT + SHIFT + Key_E : if (gs.rTilt != 0.0) gs.rTilt = 0.0; central->setPaint(); break;
   case Key_BracketLeft     : if (gs.rTilt > -rDegQuad) gs.rTilt = gs.rTilt > -rDegQuad ? gs.rTilt-TILTSTEP : -rDegQuad; central->setPaint(' '); break;
   case Key_BracketRight    : if (gs.rTilt < rDegQuad) gs.rTilt = gs.rTilt < rDegQuad ? gs.rTilt+TILTSTEP  : rDegQuad; central->setPaint(' '); break;
   case Key_I               : not2(gs.fAlt); central->setPaint(' '); break;
   case CTRL + ALT + Key_O  :  not2(us.fPrimeVert); not2(us.fCalendarYear); not2(us.nEphemYears); not2(gs.fMollewide); gi.nMode = (gi.nMode == gWheel ? gHouse : (gi.nMode == gHouse ? gWheel : gi.nMode)); central->setPaint(' '); break;
   case ALT + SHIFT + Key_G : central->GraphicSettings(); break;
   case SHIFT + Key_N       : Toggle(SHIFT + Key_N); central->Anim(0); break;
   case CTRL + ALT + Key_N  : central->Anim(10); break;
   case Key_Exclam          : ToggleJRate(Seconds); central->Anim( -1); break;
   case Key_At              : ToggleJRate(Minutes); central->Anim(-2); break;
   case Key_NumberSign      : ToggleJRate(Hours); central->Anim(-3); break;
   case Key_Dollar          : ToggleJRate(Days); central->Anim(-4); break;
   case Key_Percent         : ToggleJRate(Months); central->Anim(-5); break;
   case Key_AsciiCircum     : ToggleJRate(Years); central->Anim(-6); break;
   case Key_Ampersand       : ToggleJRate(Decades); central->Anim(-7); break;
   case Key_Apostrophe      : ToggleJRate(Centuries); central->Anim(-8); break;
   case Key_ParenLeft       : ToggleJRate(Millenia); central->Anim(-9); break;
   case Key_1               : ToggleJFact(Unit_1, 1); break;
   case Key_2               : ToggleJFact(Unit_2, 2); break;
   case Key_3               : ToggleJFact(Unit_3, 3); break;
   case Key_4               : ToggleJFact(Unit_4, 4); break;
   case Key_5               : ToggleJFact(Unit_5, 5); break;
   case Key_6               : ToggleJFact(Unit_6, 6); break;
   case Key_7               : ToggleJFact(Unit_7, 7); break;
   case Key_8               : ToggleJFact(Unit_8, 8); break;
   case Key_9               : ToggleJFact(Unit_9, 9); break;
   case Key_R               : Toggle(i); central->SetDir(-1); break;
   case Key_P               : Toggle(i); central->SetPause(); break;
   case Key_J               : Toggle(i); central->TimedExpos(); break;
   case Key_Plus            : central->PlusMinus('+'); break;
   case Key_Minus           : central->PlusMinus('-'); break;
   case SHIFT + Key_O       : central->setPaint(i); break;
   case CTRL + Key_ParenRight : central->ListFile("astrolog.dat"); break;
   case CTRL + Key_Exclam   : central->ListFile("file_id.diz"); break;
   case CTRL + Key_At       : central->ListFile("readme.540"); break;
   case CTRL + Key_Percent  : central->ListFile("update.540"); break;
   case CTRL + Key_Dollar   : central->ListFile("helpfile.540"); break;
   case CTRL + Key_Ampersand   : central->ListFile("kastrolog.html"); break;
   case Key_ParenRight      : central->ListFile("http://devel-home.kde.org/~kastrolog/"); break;
   case ALT  + Key_2        : ToggleHelp(Signs); central->ChartType(&us.fSign, ' '); break;
   case ALT  + Key_3        : ToggleHelp(Objects); central->ChartType(&us.fObject, ' '); break;
   case ALT  + Key_4        : ToggleHelp(Aspects); central->ChartType(&us.fAspect, ' '); break;
   case ALT  + Key_5        : ToggleHelp(Constell); central->ChartType(&us.fConstel, ' '); break;
   case ALT  + Key_6        : ToggleHelp(Planets); central->ChartType(&us.fOrbitData, ' '); break;
   case ALT  + Key_7        : ToggleHelp(GenMean); central->ChartType(&us.fMeaning, ' '); break;
   case ALT  + Key_8        : ToggleHelp(Flags); central->ChartType(&us.fSwitch, ' '); break;
   case ALT  + Key_9        : ToggleHelp(SecFlags); central->ChartType(&us.fSwitchRare, ' ');break;
   case Key_Question        : ToggleHelp(Keyboard); central->ChartType(&us.fKeyGraph, ' '); break;
   case CTRL + Key_O        : central->About(); break;
   default                  : central->setPaint(tolower(i));
    }
#else //KDE2
    case Qt::ALT + Qt::Key_Exclam    : AstroApp.newtask(); break;        // Open a new window
    case Qt::ALT + Qt::Key_O         : central->Inputfile(ciMain); break; // Open Chart
    case Qt::ALT + Qt::SHIFT + Qt::Key_O : central->Inputfile(ciTwin); break; // Open Chart #2
    case Qt::ALT + Qt::Key_W         : central->OutputFile(); break; // Save Chart Info
    case Qt::ALT + Qt::SHIFT + Qt::Key_W : central->OutputFile(SaveOutput); break; // Save Output pos
    case Qt::Qt::CTRL + Qt::Key_5        : central->OutputFile(SaveText); break; // Save Text
    case Qt::CTRL + Qt::Key_6        : central->OutputFile(SaveBitmap); break; // Save Bitmap
#ifdef META
    case Qt::CTRL + Qt::Key_7        : central->OutputFile(SavePicture); break; // Save WMF
#endif
#ifdef PS
    case Qt::CTRL + Qt::Key_8        : central->OutputFile(SavePostscript); break; // SAve PostScript
#endif
    case Qt::CTRL + Qt::Key_9        : central->OutputFile(SaveSettings); break; // Save settings
    case Qt::CTRL + Qt::Key_Q        : central->PrintDialog(); break; // Display a print dialog
    case Qt::CTRL + Qt::Key_P        : central->Print(); break; // Print current chart

    case Qt::ALT + Qt::Key_C         : AstroApp.bye(FALSE); break; // Quit the current task
    case Qt::Key_Q               : AstroApp.bye(TRUE); break; // exit from all
    case Qt::Key_Return          : central->Command(); break; // Wait for a line of commands and execute it
      // !!! gcc feature only !!!
  case Qt::Key_F1 ... Qt::Key_F12  : central->RunMacro(i-Qt::Key_F1); break; // execute macros from F1 to F12

    case Qt::SHIFT + Qt::Key_F1 ... Qt::SHIFT + Qt::Key_F12 : central->RunMacro(i-(Qt::SHIFT+Qt::Key_F1)+12); break; //execute macros from Shift F1 to Shift F12
   case Qt::CTRL + Qt::Key_F1 ... Qt::CTRL + Qt::Key_F12 : central->RunMacro(i-(Qt::CTRL+Qt::Key_F1)+24); break; // Run macros from Ctrl F1 to Ctrl F12 
   
   case Qt::ALT + Qt::Key_F1 ... Qt::ALT + Qt::Key_F12 : central->RunMacro(i-(Qt::ALT+Qt::Key_F1)+36); break; // Run macros from Alt F1 to Alt F12 
  case Qt::Key_V                : Toggle(i); not2(us.fGraphics); central->GraphicsText(); break; // Graphic <-> text modes selection
   case Qt::ALT + Qt::Key_Q          : Toggle(i); central->ResizeChart(); break;
   case Qt::Key_PageUp           : central->Scroll(DirUp); break;
   case Qt::Key_PageDown         : central->Scroll(DirDown); break;
   case Qt::Key_Home             : central->Scroll(DirTop); break;
   case Qt::Key_End              : central->Scroll(DirBottom); break;
   case Qt::ALT + Qt::Key_K          : Toggle(i); central->ColoredText(); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_K  : central->SetPalette(); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_Percent : central->SetEAColors(); break;
#ifdef INTERPRET
   case Qt::ALT + Qt::SHIFT +Qt::Key_I   : Toggle(i); not2(us.fInterpret);
                               if(us.fInterpret && us.fGraphics)
			       {
				 Toggle(Qt::Key_V);
				 not2(us.fGraphics);
				 central->GraphicsText();
			       }
			       central->ShowInterp();
			       break;
#endif
  case Qt::ALT + Qt::Key_B         : Toggle(i); not2(us.fSeconds); is.fSeconds = us.fSeconds; central->setPaint(' '); break;
   case Qt::ALT + Qt::Key_X         : Toggle(i); not2(us.fAppSep); central->setPaint(' ');  break;
   case Qt::ALT + Qt::Key_Percent   : central->SetChartInfo(ciMain); break;
   case Qt::ALT + Qt::SHIFT +Qt::Key_X  : SwitchAction(us.fParallel); break;
   case Qt::ALT +Qt::Key_D          : central->SetChartDefaultInfo(); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_Z : central->SetChartInfo(ciTwin); break;
   case Qt::SHIFT + Qt::Key_3       : central->SetChartInfo(ciThre); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_3 : central->Inputfile(ciThre); break;
   case Qt::SHIFT + Qt::Key_4       : central->SetChartInfo(ciFour); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_4 : central->Inputfile(ciFour); break;
   case Qt::Key_C               : ToggleChart(Comparison);  central->SetRelation(0); break;
   case Qt::ALT + Qt::Key_Y         : ToggleChart(Synastry); central->SetRelation(rcSynastry); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_Y : ToggleChart(Composite); central->SetRelation(rcComposite); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_M : ToggleChart(Time_Space); central->SetRelation(rcMidpoint); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_D : ToggleChart(Date_Diff); central->SetRelation(rcDifference); break;
#ifdef BIORHYTHM
   case Qt::SHIFT + Qt::Key_Y       : ToggleChart(Biorhythm); central->SetRelation(rcBiorhythm); break;
#endif
   case Qt::ALT + Qt::Key_N         : ToggleChart(Transit_Natal); central->SetRelation(rcTransit); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_N : ToggleChart(Progressed_Natal); central->SetRelation(rcProgress); break;
   case Qt::Key_S               : Toggle(Qt::Key_S); central->setPaint(tolower(i)); break;
   case Qt::Key_H               : Toggle(Qt::Key_H); central->setPaint(tolower(i)); break;
   seths(Qt::Key_P, 0);
   seths(Qt::Key_K, 1);
   seths(Qt::Key_E, 2);
   seths(Qt::Key_C, 3);
   seths(Qt::Key_M, 4);
   seths(Qt::Key_R, 5);
   seths(Qt::Key_Y, 6);
   seths(Qt::Key_U, 7);
   seths(Qt::Key_T, 8);
   seths(Qt::Key_A, 9);
   seths(Qt::Key_Q, 10);
   seths(Qt::Key_O, 11);
   seths(Qt::Key_W, 12);
   seths(Qt::Key_V, 13);
   seths(Qt::Key_N, 14);
   case Qt::ALT + Qt::Key_1         : Toggle(i); central->setPaint(Qt::ALT + Qt::Key_1); break;
   case Qt::Key_G               : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::Key_F               : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_H : Toggle(i); central->setPaint(Qt::ALT + Qt::SHIFT + Qt::Key_H); break;
   case Qt::Key_Z               : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::Key_Y               : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_A : central->AspectSettings(); break;
   case Qt::ALT + Qt::Key_J         : central->ObjectSettings(transl("Object Settings..."),1, 0); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_J : central->ObjectSettings(transl("Cusp & Uranian Settings"), 21, 0); break;
   case Qt::CTRL + Qt::ALT + Qt::SHIFT + Qt::Key_J : central->ObjectSettings(transl("Asteroid Settings"), 41, 0); break;
   case Qt::CTRL + Qt::SHIFT + Qt::Key_J : central->ObjectSettings(transl("Asteroid Settings"), 61, 0); break;
   case Qt::ALT + Qt::Key_R         : central->Restrict(transl("Object Restrictions"), ignore, ignore2); break;
   case Qt::SHIFT + Qt::Key_R       : Toggle(i); not2(minors); central->ToggleObj(oPlu, oEP, !minors); break;
   case Qt::SHIFT + Qt::Key_C       : Toggle(i); not2(us.fCusp); central->ToggleObj(cuspLo-1, cuspHi, !us.fCusp); break;
   case Qt::Key_U               : Toggle(i); not2(us.fUranian); central->ToggleObj(uranLo-1, uranHi, !us.fUranian); break;
   case Qt::SHIFT + Qt::Key_U       : Toggle(i); not2(us.nStar); central->setPaint();  break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_R : central->RestrictStar(); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_F : central->Restrict(transl("Transit Object Restrictions"), ignore2, ignore); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_S : central->CalculationSettings(); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_B : central->ObscureSetting(); break;
   case Qt::SHIFT + Qt::Key_V       : chtype(List, fListing, 'v');
   case Qt::ALT + Qt::SHIFT + Qt::Key_V : chtype(Wheel, fWheel, 'V');
   case Qt::SHIFT + Qt::Key_A       : chtype(Grid, fGrid, 'A');
   case Qt::ALT + Qt::Key_L         : chtype(Aspect, fAspList, ' ');
   case Qt::ALT + Qt::Key_M         : chtype(MidPoint, fMidpoint, ' ');
   case Qt::SHIFT + Qt::Key_Z       : chtype(Horizon, fHorizon, 'Z');
   case Qt::SHIFT + Qt::Key_S       : chtype(Orbit, fOrbit, 'S');
   case Qt::SHIFT + Qt::Key_M       : chtype(Sector, fSector, 'M');
   case Qt::SHIFT + Qt::Key_K       : chtype(Calendar, fCalendar, 'K');
   case Qt::SHIFT + Qt::Key_I       : chtype(Influence, fInfluence, 'J');
   case Qt::SHIFT + Qt::Key_L       : chtype(AstroGraph, fAstroGraph, 'L');
   case Qt::SHIFT + Qt::Key_E       : chtype(Ephemeris, fEphemeris, 'E');
#ifdef ARABIC
   case Qt::ALT + Qt::Key_P         : chtype(Arabic, nArabic, ' ');
#endif
   case Qt::ALT + Qt::SHIFT + Qt::Key_L : chtype(Rising, fHorizon, ' ');
   case Qt::ALT + Qt::SHIFT + Qt::Key_T : if(central->Transits()) ToggleType(Transit); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_P : if(central->Progressions()) ToggleType(Progress); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_C : central->ChartSettings(); break;
   case Qt::SHIFT + Qt::Key_W       : ToggleGraph(Map); central->ShowGlobe(gWorldMap); break;
   case Qt::SHIFT + Qt::Key_G       : ToggleGraph(Globe); central->ShowGlobe(gGlobe); break;
   case Qt::SHIFT + Qt::Key_P       : ToggleGraph(Polar); central->ShowGlobe(gPolar); break;
#ifdef CONSTEL
   case Qt::SHIFT + Qt::Key_F       : SwitchAction(gs.fConstel); break;
#endif
   case Qt::Key_X               : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::Key_M               : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::Key_B               : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::Key_T               : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::Key_L               : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::ALT + Qt::Key_T         : not2(us.fVelocity); Toggle(i); central->setPaint('b'); break;
   case Qt::SHIFT + Qt::Key_Q       : Toggle(i); central->setPaint(tolower(i)); break;
   case Qt::CTRL + Qt::Key_1        : TogglenScale(100); break;
   case Qt::CTRL + Qt::Key_2        : TogglenScale(200); break;
   case Qt::CTRL + Qt::Key_3        : TogglenScale(300); break;
   case Qt::CTRL + Qt::Key_4        : TogglenScale(400); break;
   case Qt::Key_Less            : if(gs.nScale >= 200) { gs.nScale -= 100; TogglenScale(gs.nScale); } break;      
   case Qt::Key_Greater         : if(gs.nScale <= 300) { gs.nScale += 100; TogglenScale(gs.nScale); } break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_E : if (gs.rTilt != 0.0) gs.rTilt = 0.0; central->setPaint(); break;
   case Qt::Key_BracketLeft     : if (gs.rTilt > -rDegQuad) gs.rTilt = gs.rTilt > -rDegQuad ? gs.rTilt-TILTSTEP : -rDegQuad; central->setPaint(' '); break;
   case Qt::Key_BracketRight    : if (gs.rTilt < rDegQuad) gs.rTilt = gs.rTilt < rDegQuad ? gs.rTilt+TILTSTEP  : rDegQuad; central->setPaint(' '); break;
   case Qt::Key_I               : not2(gs.fAlt); central->setPaint(' '); break;
   case Qt::CTRL + Qt::ALT + Qt::Key_O  :  not2(us.fPrimeVert); not2(us.fCalendarYear); not2(us.nEphemYears); not2(gs.fMollewide); gi.nMode = (gi.nMode == gWheel ? gHouse : (gi.nMode == gHouse ? gWheel : gi.nMode)); central->setPaint(' '); break;
   case Qt::ALT + Qt::SHIFT + Qt::Key_G : central->GraphicSettings(); break;
   case Qt::SHIFT + Qt::Key_N       : Toggle(Qt::SHIFT + Qt::Key_N); central->Anim(0); break;
   case Qt::CTRL + Qt::ALT + Qt::Key_N  : central->Anim(10); break;
   case Qt::Key_Exclam          : ToggleJRate(Seconds); central->Anim( -1); break;
   case Qt::Key_At              : ToggleJRate(Minutes); central->Anim(-2); break;
   case Qt::Key_NumberSign      : ToggleJRate(Hours); central->Anim(-3); break;
   case Qt::Key_Dollar          : ToggleJRate(Days); central->Anim(-4); break;
   case Qt::Key_Percent         : ToggleJRate(Months); central->Anim(-5); break;
   case Qt::Key_AsciiCircum     : ToggleJRate(Years); central->Anim(-6); break;
   case Qt::Key_Ampersand       : ToggleJRate(Decades); central->Anim(-7); break;
   case Qt::Key_Apostrophe      : ToggleJRate(Centuries); central->Anim(-8); break;
   case Qt::Key_ParenLeft       : ToggleJRate(Millenia); central->Anim(-9); break;
   case Qt::Key_1               : ToggleJFact(Unit_1, 1); break;
   case Qt::Key_2               : ToggleJFact(Unit_2, 2); break;
   case Qt::Key_3               : ToggleJFact(Unit_3, 3); break;
   case Qt::Key_4               : ToggleJFact(Unit_4, 4); break;
   case Qt::Key_5               : ToggleJFact(Unit_5, 5); break;
   case Qt::Key_6               : ToggleJFact(Unit_6, 6); break;
   case Qt::Key_7               : ToggleJFact(Unit_7, 7); break;
   case Qt::Key_8               : ToggleJFact(Unit_8, 8); break;
   case Qt::Key_9               : ToggleJFact(Unit_9, 9); break;
   case Qt::Key_R               : Toggle(i); central->SetDir(-1); break;
   case Qt::Key_P               : Toggle(i); central->SetPause(); break;
   case Qt::Key_J               : Toggle(i); central->TimedExpos(); break;
   case Qt::Key_Plus            : central->PlusMinus('+'); break;
   case Qt::Key_Minus           : central->PlusMinus('-'); break;
   case Qt::SHIFT + Qt::Key_O       : central->setPaint(i); break;
   case Qt::CTRL + Qt::Key_ParenRight : central->ListFile("astrolog.dat"); break;
   case Qt::CTRL + Qt::Key_Exclam   : central->ListFile("file_id.diz"); break;
   case Qt::CTRL + Qt::Key_At       : central->ListFile("readme.540"); break;
   case Qt::CTRL + Qt::Key_Percent  : central->ListFile("update.540"); break;
   case Qt::CTRL + Qt::Key_Dollar   : central->ListFile("helpfile.540"); break;
   case Qt::CTRL + Qt::Key_Ampersand   : central->ListFile("kastrolog.html"); break;
   case Qt::Key_ParenRight      : central->ListFile("http://devel-home.kde.org/~kastrolog/"); break;
   case Qt::ALT  + Qt::Key_2        : ToggleHelp(Signs); central->ChartType(&us.fSign, ' '); break;
   case Qt::ALT  + Qt::Key_3        : ToggleHelp(Objects); central->ChartType(&us.fObject, ' '); break;
   case Qt::ALT  + Qt::Key_4        : ToggleHelp(Aspects); central->ChartType(&us.fAspect, ' '); break;
   case Qt::ALT  + Qt::Key_5        : ToggleHelp(Constell); central->ChartType(&us.fConstel, ' '); break;
   case Qt::ALT  + Qt::Key_6        : ToggleHelp(Planets); central->ChartType(&us.fOrbitData, ' '); break;
   case Qt::ALT  + Qt::Key_7        : ToggleHelp(GenMean); central->ChartType(&us.fMeaning, ' '); break;
   case Qt::ALT  + Qt::Key_8        : ToggleHelp(Flags); central->ChartType(&us.fSwitch, ' '); break;
   case Qt::ALT  + Qt::Key_9        : ToggleHelp(SecFlags); central->ChartType(&us.fSwitchRare, ' ');break;
   case Qt::Key_Question        : ToggleHelp(Keyboard); central->ChartType(&us.fKeyGraph, ' '); break;
   case Qt::CTRL + Qt::Key_O        : central->About(); break;
   default                  : central->setPaint(tolower(i));
    }
#endif // KDE2
}

void AstroMenu::iconsslot(int i)
{
  switch(i)
  {
     case 0 :  central->Inputfile(ciMain); break;
     case 1 :  central->SetChartInfo(ciMain); break;
     case 2 :  central->setPaint('n'); break;
     case 3 :  central->Print(); break;
#ifdef KDE1
     case 4 :  Toggle(Key_X); central->setPaint('x'); break;
     case 5 :  Toggle(Key_M); central->setPaint('m'); break;
#else
     case 4 :  Toggle(Qt::Key_X); central->setPaint('x'); break;
     case 5 :  Toggle(Qt::Key_M); central->setPaint('m'); break;
#endif
     case 6 :  ToggleChart(Comparison); central->SetRelation(0); break;
#ifdef KDE1
     case 7 :  Toggle(SHIFT + Key_N); central->Anim(0); break;
#else
     case 7 :  Toggle(Qt::SHIFT + Qt::Key_N); central->Anim(0); break;
#endif
     case 8 :  Toggle(i); central->SetDir(-1); break;
     default:;
  }
}

void AstroMenu::nbsslot(int i)
{
  switch(i)
  {
     case 0: ToggleJFact(Unit_1, 1); break;
     case 1: ToggleJFact(Unit_2, 2); break;
     case 2: ToggleJFact(Unit_3, 3); break;
     case 3: ToggleJFact(Unit_4, 4); break;
     case 4: ToggleJFact(Unit_5, 5); break;
     case 5: ToggleJFact(Unit_6, 6); break;
     case 6: ToggleJFact(Unit_7, 7); break;
     case 7: ToggleJFact(Unit_8, 8); break;
     case 8: ToggleJFact(Unit_9, 9); break;
  };
}

void AstroMenu::unitsslot(int i)
{
  switch(i)
  {
    case 0 : ToggleJRate(Seconds); central->Anim( -1); break;
    case 1 : ToggleJRate(Minutes); central->Anim(-2); break;
    case 2 : ToggleJRate(Hours); central->Anim(-3); break;
    case 3 : ToggleJRate(Days); central->Anim(-4); break;
    case 4 : ToggleJRate(Months); central->Anim(-5); break;
    case 5 : ToggleJRate(Years); central->Anim(-6); break;
    case 6 : ToggleJRate(Decades); central->Anim(-7); break;
    case 7 : ToggleJRate(Centuries); central->Anim(-8); break;
    case 8 : ToggleJRate(Millenia); central->Anim(-9); break;
    default:;
  }
}






















