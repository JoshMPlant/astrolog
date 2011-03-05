/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


#include <kapp.h>
#ifdef KDE1
#include <kpopmenu.h>
#else
#include <kpopupmenu.h>
#endif
#include <ktoolbar.h>
#include <kiconloader.h>
#ifdef KDE1
#include "Kmenubar.h"
#else
#include <kmenubar.h>
#endif
#include "AstroWidget.h"


#define LASTICON 8


enum ChartType { List, Wheel, Grid, Aspect, MidPoint, Horizon, Orbit, Sector, Calendar, Influence, AstroGraph, Ephemeris, Arabic, Rising, Transit, Progress, Comparison, Synastry, Composite, Time_Space , Date_Diff, Biorhythm, Transit_Natal, Progressed_Natal, Map, Globe, Polar, Signs, Objects, Aspects, Constell, Planets, GenMean, Flags, SecFlags, Keyboard }; 
enum nScaleType { Small, Medium, Large, Huge };
enum JRateType { Seconds, Minutes, Hours, Days, Months, Years, Decades, Centuries, Millenia };
enum JFact { Unit_1, Unit_2, Unit_3, Unit_4, Unit_5, Unit_6, Unit_7, Unit_8, Unit_9 };

struct icons
{
  QString s, tip;
  int id;
};

class AstroMenu : public KMenuBar
{
  Q_OBJECT

 public:
  
  AstroMenu(QWidget *parent);
  
public slots:

  void menuslot(int);
  void iconsslot(int);
  void nbsslot(int);
  void unitsslot(int);

 private:
  
  void Toggle(int);
  void VoidChart();
  void ToggleChart(ChartType); //Mode);
  void ToggleType(ChartType);
  void ToggleGraph(ChartType); //GraphType);
  void ToggleHousSys();
  void TogglenScale(int);
  void ToggleJRate(JRateType);
  void ToggleJFact(JFact, int);
  void ToggleHelp(ChartType);
  QPopupMenu *nbjmp, *unijmp;
  AstroWindow *asw;
  bool minors;
 
};









