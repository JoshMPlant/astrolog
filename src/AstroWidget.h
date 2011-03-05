/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 

#ifdef KDE1
#include <drag.h>
#endif
#include <qscrollview.h>
#include <qpicture.h>
#include <qpainter.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qlist.h>
#include <kstatusbar.h>
#include <ktoolbar.h>
#include <ktmainwindow.h>
#include <kiconloader.h>
#include "myLineEdit.h"
#include "AstroInfo.h"

extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
#include <unistd.h>
#include <signal.h>
#undef bool
}

#define OFFSET FONTY

enum savetype { SaveInfo, SaveOutput, SaveText, SavePostscript, SaveBitmap, SavePicture, SaveSettings };

enum GraphMode { GMGraph, GMText, GMDblbuf };

void NCheck(QLineEdit*, int, int, const char *, int*);
void FCheck(QLineEdit*, real, real, const char *, real*);

class AstroStringList: public QList<QString>
{
public:
  typedef QString Iterator;
  AstroStringList();
  void clear();
};

//inline char *unconst(char *c, ...) {char **ptr = &c; return *(++ptr); }

#define UNCONST(c) (char*)(c)
#ifdef KDE1
#define latin1 data
#endif
#define INC(s, sl) s= sl->next()

class AstroApp
{
public:

  AstroApp();
  void init(int, char **);
  int run(int, int);
  inline void newtask() { kill(mainpid, SIGCONT); }
  int nbchilds, thecounter;
  void bye(bool);
  inline void add(pid_t pid) { mypidList.add(pid); }
  inline void killall() { mypidList.killall(); }
  void checkdst();
  void getTZ(real *r);
private:

  class pidList: public QList<pid_t>
  {
    public:
  
    void add(pid_t);
    void killall();
  }
  mypidList;

  int theargc;
  char** theargv;
  pid_t mainpid;
  AstroTZ tz;
  char *tzfile;
};

enum Direction { DirUp, DirDown, DirTop, DirBottom };

#ifdef KDE1
class MyDropZone : public KDNDDropZone
{
 public:
  MyDropZone(QWidget* _parent) : KDNDDropZone(_parent, DndURL) {}
  void drop( char *_data, int _size, int _type, int _x, int _y );
};
#endif

class AstroWidget : public QScrollView
{

  Q_OBJECT

public:

  AstroWidget(QWidget *, const char *);
  void drawContentsOffset ( QPainter *, int ox, int oy, int cx, int cy, int cw, int ch );
  inline QPainter* GetP() const { return pain; }
  inline char GetKey()    const { return key; }
  inline int GetScale()   const { return scale; }
  inline int GetNumber()  const { return number; }
  inline QFont* GetFont(bool ast = FALSE) const { return ast ? astro : cour; }
  void resizeEvent(QResizeEvent*);
  void keyPressEvent(QKeyEvent*);
  void setPaint(char c = 0);
  void Inputfile(CI& ci = ciCore);
  void OutputFile(savetype st = SaveInfo);
  void PrintDialog();
  void Print();
  void RunLine(const char *);
  void Command();
  void RunMacro(int);
  void SetChartInfo(CI&);
  void SetChartDefaultInfo();
  void SetRelation(int);
  void ResizeChart() { not2(Resize); };
  void SetPalette();
  void SetEAColors();
  void AspectSettings();
  void ObjectSettings(const char*, int, int);
  void Restrict(const char*, byte*, byte*);
  void ToggleObj(int, int, bool);
  void RestrictStar();
  void CalculationSettings();
  void ObscureSetting();
  void ChartType(void*, char);
  void Anim(int);
  bool Transits();
  bool Progressions();
  void ChartSettings();
  void GraphicSettings();
  void SetDir(int);
  inline void SetPause() { not2(Pause); }
  void TimedExpos();
  void ListFile(const char*);
  void About();
  GraphMode GetGm();
  void SetGm(GraphMode g); 
  void PutText(const char*, bool);
  void ColoredText();
  void Scroll(Direction);
  void GraphicsText();
  void ShowGlobe(int);
  void ShowInterp();
  void PlusMinus(char);
  inline bool isPrinting() { return Printing; };
#ifndef KDE1
  void viewportDropEvent(QDropEvent*);
  void viewportDragEnterEvent(QDragEnterEvent*);
#endif

public slots:

  void timerslot();

private:

  void PrintTheChart(_bool, _bool);
  void NewPage(int, int, int);
  KI LastCol(int);

  QPicture *qpic;
  QPainter *pain;
  QPrinter *print;
  QTimer *tim;
  QPixmap *dblbuf;
  AstroStringList *sl;
  QFont *cour, *astro;
  int jrate, scale, number, color;
  char key;
  bool Pause, Exposed, Resize, Printing;
  GraphMode gm;
};

class AstroMenu;

class AstroWindow : public KTMainWindow
{
public:

  AstroWindow();
  inline void init() { central->setPaint(' '); show(); }
  AstroMenu *am;
  AstroWidget *central;
  KStatusBar *Ksb;
  KToolBar *Ktb;
  KIconLoader* Kil;
};

#ifndef ASTROWINDOW
#define ASTROWINDOW
extern AstroWindow *asw;
#define am      asw->am
#define central asw->central
#define Ksb     asw->Ksb
#define Ktb     asw->Ktb
#define Kil     asw->Kil
#endif









