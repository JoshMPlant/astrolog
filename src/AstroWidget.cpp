/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :         kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


#include <qcursor.h>
#include <qtimer.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <kprocess.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qmessagebox.h>
#include <qaccel.h>
#include <qimage.h>
#include <qpaintdevicemetrics.h>
#include <kapp.h>
#include <kfiledialog.h>
#ifdef KDE1
#include <kmsgbox.h>
#include <kfm.h>
#else
#include <kmessagebox.h>
#include <qdragobject.h>
#include <qstringlist.h>
#include <qvaluelist.h>
#include <kurldrag.h>
#endif
#include "qregexp.h"
#include <kurl.h>
#include "AstroMenu.h"
#include "AstroInfo.h"
#include "AstroCommand.h"
#include "AstroColor.h"
#include "SetColors.h"
#include "SetAstroColor.h"
#include "AstroSetAspects.h"
#include "AstroObjectSet.h"
#include "Restrictions.h"
#include "StarRestrict.h"
#include "CalcSettings.h"
#include "ObscureSettings.h"
#include "AstroTransits.h"
#include "AstroProgress.h"
#include "ChartDetails.h"
#include "GraphSettings.h"
#include "tr.h"

extern "C"
{
#include <ctype.h>
#include <unistd.h>
}

#define FILT transl("*.kst|kastrolog files\n|All")
#define action gi.fiskde = fFalse; Action(); gi.fiskde = fTrue
#define C(fl, mo) if(us.fl) gi.nMode = mo; else
#define NEWHEIGHT gi.newHeight = -1

// MARGINS = 2 cm / 2.54 inches * 72 dpi
#define MARGINS  57
#define COLCHAR '"'
#define BOLDLINE 4

const char *ColorName[16] =
#ifdef PC
{ "gray1", "RosyBrown1", "LightGreen", "OrangeRed1", "DarkBlue", "purple", "DarkCyan", "gray77", "gray31", "red", "SpringGreen1", "yellow1", "CadetBlue1", "HotPink1", "cyan1", "snow" };
#else
{"black", "maroon", "dark green", "orange", "dark blue", "dark red", "dark cyan", "light grey", "dark grey", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
#endif
extern char *ColorTbl[];
extern AstroColorList *Acl;
extern KApplication *Ka;
extern const QString kdepath;
extern const QPixmap *kdeicon;
extern AstroApp AstroApp;
AstroColor *colorTable[16];

extern "C"
{
  
  char getkey() { return central->GetKey(); }

  int getdir() { return central->GetNumber(); }

  void KColor(KI col)
  {
    if(!gi.fiskde || !gi.painter) return;
    if(col == -1)
      col = 0;
    if(central->GetGm() != GMText)
      central->GetP()->setPen(*colorTable[col]);
    else
    {
      char buf[3] = "  ";
      buf[0] = COLCHAR;
      buf[1] = '0' + col;
      central->PutText(buf, FALSE);
    }
  };

  void KPoint(int x, int y)
  {
    if(gi.fiskde)
      central->GetP()->drawPoint(x, y);
  };

  void KLine(int x, int y, int x1, int y1)
  {
    if(gi.fiskde)
      central->GetP()->drawLine(x, y, x1, y1);
  }

  void KDash(int x, int y, int x1, int y1, int skip)
  {
    if(gi.fiskde)
    {
#ifdef KDE1
      PenStyle ps;
#else
      Qt::PenStyle ps;
#endif
      switch(skip)
      {
        case 0  :
#ifdef KDE1
        case 1  : ps = SolidLine;   break;
        case 2  : ps = DashLine;    break;
        case 3  : ps = DashDotLine; break;
        default : ps = DotLine;
#else
        case 1  : ps = Qt::SolidLine;   break;
        case 2  : ps = Qt::DashLine;    break;
        case 3  : ps = Qt::DashDotLine; break;
        default : ps = Qt::DotLine;
#endif
      }
      QPen qp(*colorTable[gi.kiCur], central->isPrinting() ? BOLDLINE : 1, ps);
      central->GetP()->setPen(qp);
      central->GetP()->drawLine(x, y, x1, y1);
#ifdef KDE1
      qp.setStyle(SolidLine);
#else
      qp.setStyle(Qt::SolidLine);
#endif
      central->GetP()->setPen(qp);
    }
  }

  void KBlock(int x, int y, int x1, int y1)
  {
    if(gi.fiskde)
      central->GetP()->drawRect(x, y, x1, y1);
  }

  void KEllipse(int x, int y, int w, int h)
  {
    if(gi.fiskde)
      central->GetP()->drawArc(x, y, w, h, 0, 5760);
  }

  void KSetCour(int fact)
  {
    QFont *cour = central->GetFont();
    if(!gi.fiskde && central->GetGm() == GMText || !central->GetP()) return;
    cour->setPointSize(gi.nScale*fact);
    central->GetP()->setFont(*cour);
    gi.w = central->GetP()->fontMetrics().width(' ');
  }

  void KTextLine(int fact, char *str, int x, int y)
  {
    char *s, buf[240];
    int h, i;
    if(!gi.fiskde) return;
    if(!gi.fixcour)
      KSetCour(fact);
    for(s = str, i = 0; *s; s++, i++)
    {
      buf[i] = *s;
      if(*s == '\n' || (us.fClip80 && is.cchCol + i >= us.nScreenWidth))
      {
         buf[i] = 0;
         switch(central->GetGm())
         {
            case GMText   :
              central->PutText(buf, TRUE);
              break;
            case GMGraph  :
            case GMDblbuf :
              if(i > 0) central->GetP()->drawText(x, y, buf, i);
              break;
         }
         is.cchRow++;
         is.cchCol = 0;
         x = gi.w;
         i = -1;
         y += FONTY;
         if (y >= central->height())
         gi.newHeight = y;  
      }
    }
     buf[i] = 0;
     switch(central->GetGm())
     {
       case GMText   :
         central->PutText(buf, FALSE);
         break;
       case GMGraph  :
       case GMDblbuf :
         if(i > 0) central->GetP()->drawText(x, y, buf, i);
         break;
     }
     is.cchCol += i;
     h = central->height();
     if (y >= h)
       gi.newHeight = y;
  }

  void KNumber(int i, int x, int y)
  {
    int w;
    QString qs;
    QFont* cour = central->GetFont();
    if(!gi.fiskde && central->GetGm() == GMText) return;
    qs.sprintf("%d", i);
    cour->setPointSize(gi.nScale*9);
    central->GetP()->setFont(*cour);
    w = central->GetP()->fontMetrics().width(qs);
    central->GetP()->drawText(x-w/2, y+3*gi.nScale, qs); 
  }

  void KDrawSign(int i, int x, int y)
  {
    char buf[2] = " ";
    char legend[13] = "xc^bnmXCVZNM";
    int w;
    QFont* astro = central->GetFont(TRUE);
    if(!gi.fiskde) return;
    // Capric glyph selection
    if(i == 10)
     /* buf[0] = (gs.nGlyphs / 1000 == 2) ? 'k' : 'J';*/
      buf[0] = (gs.nGlyphs / 1000 == 2) ? 'Z' : 'B';
    else
    /*  buf[0] = '@' + i;*/
      buf[0] = legend[i-1];
    astro->setPointSize(gi.nScale*11);
    central->GetP()->setFont(*astro);
    w = central->GetP()->fontMetrics().width(buf);
    central->GetP()->drawText(x-w/2, y+3*gi.nScale, buf);
  }

_bool KObj(int obj, int x, int y)
{
    char buf[2] = " ";
    int w;
    QFont* astro = central->GetFont(TRUE);
    if(!gi.fiskde && central->GetGm() == GMText) return fTrue;
    if(obj == 22 || obj == 23 || obj == 25 || obj == 26 || obj == 28 || obj == 29 || obj == 31 || obj == 32)
    {
      KNumber(obj - 20, x, y + 4);
      return fTrue;
    }
    if(szObjectFont[obj] == ' ')
    {
      if ((gi.nScale & 1) || !szDrawObject2[obj][0])
        DrawTurtle(szDrawObject[obj], x, y);
      else
      {
        gi.nScale >>= 1;
        DrawTurtle(szDrawObject2[obj], x, y); /* Special hi-res object glyphs. */
        gi.nScale <<= 1;
      }              
      return fTrue;
    }
    buf[0] = szObjectFont[obj];
    astro->setPointSize(gi.nScale*13);
    central->GetP()->setFont(*astro);
    w = central->GetP()->fontMetrics().width(buf[0]);
    central->GetP()->drawText(x-w/2, y+(3*gi.nScale)+4, buf);
    return fTrue;
  }

  void KBackground(KI col)
  {
    if(gi.fiskde && central->GetGm() != GMText)
      central->GetP()->setBackgroundColor(*colorTable[col]);
  }

  void KMessage(char *s, int type)
  {
#ifndef KDE1
    QString Error = transl("Error"), str = s, Warning = transl("Warning");
#endif
    switch(type)
      {
#ifdef KDE1
        case 1 : 
          KMsgBox::message(NULL, transl("Error"), s, KMsgBox::STOP); break;
        case 2 :
          KMsgBox::message(NULL, transl("Warning"), s, KMsgBox::EXCLAMATION); break;
#else
        case 1 :
	  KMessageBox::error(NULL, s, Error); break;
        case 2:
	  KMessageBox::information(NULL, s, Warning); break;
#endif
        case 3 :
          Ksb->message(s, 2000); break;
        default:;
      }
  }

  char *CheckColor(int i)
  {
    return (strcmp(colorTable[i]->name, ColorName[i])) ? colorTable[i]->name : NULL;
  }

  void KCheckDst()
  {
    AstroApp.checkdst();
  }

  void KGetTZ(real *r)
  {
    AstroApp.getTZ(r);
  }
};

AstroStringList::AstroStringList()
{
  setAutoDelete(TRUE);
}

void AstroStringList::clear()
{
  QList<QString>::clear();
  append(new QString);
}

AstroWidget::AstroWidget(QWidget *w, const char *s): QScrollView(w, s)
{
  int i;
  emit setUpdatesEnabled(TRUE);
  scale = gi.nScale;
  jrate = -2;
  number = 1;
  Pause = Resize = Printing = FALSE;
  Exposed = TRUE;
  tim = NULL;
  qpic = NULL;
  is.fMult = fTrue;
  SetGm(GMGraph);
  pain = new QPainter;
  CHECK_PTR(pain);
  gi.painter = fTrue;
  print = new QPrinter;
  CHECK_PTR(print);
  cour = new QFont("courier", 10);
  CHECK_PTR(cour);
  QFontMetrics qfm = *cour;
#ifdef ASTRORAWNAME
  astro = new QFont;
  CHECK_PTR(astro);
  astro->setRawName(ASTRORAWNAME);
  astro->setPointSize(10);
#else
/*  astro = new QFont("astrolog", 10);*/
  astro = new QFont("starfontsans", 10);
  astro->setCharSet(QFont::Latin1);
  CHECK_PTR(astro);
#endif
  sl = new AstroStringList;
  CHECK_PTR(sl);
#ifdef KDE1
  MyDropZone *mdz = new MyDropZone(this);
  CHECK_PTR(mdz);
  Ka->addDropZone(mdz);
#endif
  for(i = 0; i < 16; i++)
    colorTable[i] = &Acl->GetColor(ColorTbl[i] ? ColorTbl[i] : ColorName[i]);
  gi.w = qfm.width(' ');
  emit resizeContents(gs.xWin, gs.yWin + OFFSET);
  emit resize(gs.xWin, gs.yWin + OFFSET);
  viewport()->setFocusPolicy(ClickFocus);
  viewport()->setBackgroundColor(*colorTable[gi.kiOff]);
#ifndef KDE1
  viewport()->setAcceptDrops(TRUE);
#endif
}

KI AstroWidget::LastCol(int i)
{
  AstroStringList::Iterator *s = sl->at(i > 0 ? i - 1 : 0);
  if(i > 0)
    (void)sl->next();
  char j = '0', *c;
  KI col;
  for(c = UNCONST(s->data()); *c; c++)
    if(*c == COLCHAR)
      j = *(++c);
  j -= '0';
  switch(j)
  {
    case  0 : col = gi.kiOn; break;
    case 15 : col = gi.kiOff; break;
    case  8 : col = gi.kiGray; break;
    case  7 : col = gi.kiLite; break;
    default : col = j;
  }
  return col;
}

void AstroWidget::drawContentsOffset(QPainter *p, int ox, int oy, int cx, int cy, int cw, int ch )
{
  /*QWMatrix m;
  m.translate((float)-ox, (float)(-oy + OFFSET));
  m.scale(1.28, 1.28);*/
  switch(gm)
  {
    case GMGraph :
      p->translate((float)-ox, (float)(-oy + OFFSET));
      //p->setWorldMatrix(m);
      p->drawPicture(*qpic);
      break;
    case GMText  :
      int toprow = cy/FONTY, bottomrow = (cy+ch+FONTY)/FONTY, i, x;
      char col, *c, *c1;
      if(toprow >= sl->count()) break;
      AstroStringList::Iterator *s = sl->at(toprow), s1;
      p->setClipRect(cx-ox, cy-oy, cw, ch);
      cour->setPointSize(FONTY);
      p->setFont(*cour);
      const QFontMetrics& qfm = p->fontMetrics();
      if(toprow > 0)
	p->setPen(*colorTable[LastCol(toprow)]);
      for(i = toprow; i <= bottomrow && s; i++, INC(s, sl))
      {
	x = 0;
	for(c = UNCONST(s->data()), c1 = c; *c; c++)
	{
	  if(*c == COLCHAR)
	  {
	    *c = 0;
	    p->drawText(-ox+x, -oy+(i*FONTY)+OFFSET, c1);
	    x +=  qfm.width(c1);
	    *c = COLCHAR;
	    col = *(++c) - '0';
	    switch(col)
	    {
	      case  0 : col = gi.kiOn; break;
	      case 15 : col = gi.kiOff; break;
              case  8 : col = gi.kiGray; break;
              case  7 : col = gi.kiLite; break;
              default:;
            }
	    p->setPen(*colorTable[col]);
	    c1 = c+1;
	  }
	}
	p->drawText(-ox+x, -oy+(i*FONTY)+OFFSET, c1);
	x += qfm.width(c1);
      }
  };
}

void AstroWidget::resizeEvent(QResizeEvent *qe)
{
  if(Resize)
  {
    gs.xWin = qe->size().width();
    gs.yWin = qe->size().height() -20;
    emit resizeContents(gs.xWin -20 , gs.yWin -20 + OFFSET);
    setPaint(' ');
  }
  QScrollView::resizeEvent(qe);
}

void AstroWidget::keyPressEvent(QKeyEvent* e)
{
  char c = e->ascii();
  if(c)
    setPaint(c);
}

void AstroWidget::setPaint(char c = 0)
{
  if(!us.fGraphics)
  {
    if(us.fAspList || us.fMidpoint || us.nArabic || us.fHorizon || us.fTransit || us.fTransitInf || us.fListing || us.fWheel || us.fGrid || us.fOrbit || us.fSector || us.fInDay || us.fInDayInf)
    PrintTheChart(is.fProgress, fFalse);
  else if(us.fSign || us.fObject || us.fAspect || us.fConstel || us.fOrbitData || us.fMeaning || us.fKeyGraph || us.fCredit)
    PrintTheChart(fFalse, fTrue);
    return;
  }
  if(!c)
  {
    action;
    c = ' ';
  }
  key = c;
  is.cchRow = 0;
  switch(gm)
  {
     case GMGraph:
       delete qpic;
       qpic = new QPicture();
       pain->begin(qpic);
       InteractX();
       pain->end();
       if(gi.newHeight)
         {
           emit resizeContents(gs.xWin, (gi.newHeight == -1) ? gs.yWin +OFFSET:
                gi.newHeight);
           gi.newHeight = 0;
         }
       break;
     case GMText:
       switch(c)
       {
         case 'x' : not2(gs.fInverse); InitColorsX(); break;
         case 'q' : exit(0); break;
       }
       break;
     case GMDblbuf:
       dblbuf->fill(*colorTable[gi.kiOff]);
       pain->begin(dblbuf);
       InteractX();
       pain->end();
       bitBlt(viewport(), 0, OFFSET, dblbuf, 0, 0, viewport()->width(),
	      viewport()->height(), CopyROP, TRUE);
       break;
  }
  if(c == 'x')
    viewport()->setBackgroundColor(*colorTable[gi.kiOff]);
  if(gm != GMDblbuf)
    viewport()->update();
}

void AstroWidget::PutText(const char *c, bool nl)
{
  if(sl->isEmpty()) return;
  *sl->current() += c;
  if(nl)
      sl->append(new QString);
}

void AstroWidget::ColoredText()
{
  not2(us.fAnsiColor);
  not2(us.fAnsiChar);
  setPaint(' ');
}

GraphMode AstroWidget::GetGm()
{
  return gm;
}

void AstroWidget::SetGm(GraphMode g)
{
  gm = g; 
  if(g != GMGraph)
    sl->clear();
}

void NCheck(QLineEdit* qle, int i1, int i2, const char *name, int* var)
{
  char *ptr = NULL;
  QString s;
  long l = strtol(qle->text(), &ptr, 10);
  if(l < i1 || l > i2 || *ptr)
  {
    s.sprintf((const char *)transl("The %s must be set within the interval %d..%d"), name, i1, i2);
    PrintError((char *)s.data());
    s.sprintf("%d", *var);
    emit qle->setText(s.data());
  }
  else
   *var  = l; 
}

void FCheck(QLineEdit* qle, real f1, real f2, const char *name, real* var)
{
  char *ptr = NULL;
  QString s;
  real l = strtod(qle->text(), &ptr);
  if(l < f1 || l > f2 || *ptr)
  {
    s.sprintf((const char *)transl("The %s must be set within the interval %f..%f"), name, f1, f2);
    PrintError((char *)s.data());
    s.sprintf("%f", *var);
    emit qle->setText(s.data());
  }
  else
   *var  = l; 
}

void AstroWidget::Scroll(Direction d)
{
  int curline = contentsY() / FONTY, nb2 =  viewport()->height() / (2 *FONTY), bottom = sl->count();
  switch(d)
  {
     case DirTop    : emit setContentsPos(contentsX(), 0); break;
     case DirBottom : emit ensureVisible(contentsX(), bottom * FONTY); break;
     case DirUp     : if(curline > 0) emit scrollBy(0, - nb2 * FONTY); break;
     case DirDown   : if(curline < bottom - nb2) emit scrollBy(0,  nb2 * FONTY); break;
  }
}

void AstroWidget::GraphicsText()
{
  if(us.fGraphics)
  {
    C(fListing,    gList)
    C(fWheel,      gWheel)
    C(fGrid,       gGrid)
    C(fHorizon,    gHorizon)
    C(fOrbit,      gOrbit)
    C(fSector,     gSector)
    C(fCalendar,   gCalendar)
    C(fInfluence,  gDisposit)
    C(fAstroGraph, gAstroGraph)
    C(fEphemeris,  gEphemeris);
    SetGm(GMGraph);  
  }
  setPaint(' ');
}

void AstroWidget::Inputfile(CI& ci)
{
  CI ciSave = ciCore;
  bool b = memcmp(&ci, &ciSave, sizeof(CI)) ? TRUE : FALSE;
  QString s = getenv("HOME");
  s += KASTDATADIR;
  s = KFileDialog::getOpenFileName(s.data(), FILT, NULL, NULL);
  if(s.data())
    {
      FInputData(UNCONST(s.data()));
      if(is.bufComment)
      CastChart(fTrue);
      ci = ciCore;
      if(b)
	ciCore = ciSave;
      setPaint();
    }
}

static void KPrintChart(_bool *flag, const char *s)
{
  int x = gs.xWin, y = gs.yWin, ns = gi.nScale, nst = gi.nScaleT, gns = gs.nScale;
  *flag = 1;
  if(!gi.szFileOut)
    {
      gi.szFileOut = UNCONST(s);
      FActionX();
      gi.szFileOut = NULL;
      gi.fFile = fFalse;
      gs.xWin = x;
      gs.yWin = y;
      gi.newHeight = 0;
      gi.nScale = ns;
      gi.nScaleT = nst;
      gs.nScale = gns;
    }
  *flag = 0;
}

void AstroWidget::OutputFile(savetype st)
{
  QString s = getenv("HOME"), filt, filt2, filt3;
  QRegExp KstFilt;
  QPainter pr;
  void *a;

  switch(st)
  {
#ifdef PS
    case SavePostscript: filt2= ".ps"; filt = transl("*.ps|Postscript\n|All"); break;
#endif
    case SaveBitmap    : filt2 = ".png"; filt = transl("*.png|PNG Bitmap\n|All"); break;
#ifdef META
    case SavePicture   : filt2 = ".wmf"; filt = transl("*.wmf|Windows Meta File\n|All"); break;
#endif
    case SaveInfo      :
    case SaveOutput    : filt2 = ".kst"; filt = transl("*.kst|Kastrolog files\n|All"); s += KASTDATADIR; break;
    case SaveSettings  : filt2 = ".dat"; filt = transl("*.dat|Astrolog data file\n|All"); s += KASTDATADIR; break;
    default            : filt2 = "";     filt = transl("|All");
  }
  s = KFileDialog::getSaveFileName(s, filt, NULL, NULL);
  FILE *save;
  gi.fiskde = fFalse;
  us.fWritePos = (st == SaveOutput) ? fTrue : fFalse;
  if(s.data())
    {
      filt3 = filt2;
      filt3.insert(0, ".\\");
      filt3.append("$");
      KstFilt = filt3;
      if(KstFilt.match(s.data()) == -1)
	s.append(filt2);
      switch(st)
      {
#ifdef PS
	case SavePostscript:
	  KPrintChart(&gs.fPS, s.data());  break;
#endif
	case SaveBitmap:
	  viewport()->update();
	  setCursor(waitCursor);
	  dblbuf = new QPixmap(gs.xWin, gs.yWin);
	  CHECK_PTR(dblbuf);
	  dblbuf->fill(*colorTable[gi.kiOff]);
	  pr.begin(dblbuf);
	  pr.drawPicture(*qpic);
	  dblbuf->save(s.data(), "PNG");
	  pr.end();
	  delete dblbuf;
	  setCursor(arrowCursor);
	  break;
#ifdef META
	case SavePicture:
	  KPrintChart(&gs.fMeta, s.data()); break;
#endif
	case SaveText:
	  save = is.S;
	  is.S = fopen(s.data(), "w");
	  if(!is.S)
	    {
	      KMessage(transl("Can't create file"), 1);
	      is.S = save;
	      gi.fiskde = fTrue;
	      return;
	    }
	  PrintChart(fFalse);
	  fclose(is.S);
	  is.S = save;
	  break;
	case SaveInfo:
	case SaveOutput:
	  is.szFileOut = UNCONST(s.data());
	  us.fWriteFile = fTrue;
	  CastChart(fTrue);
	  FOutputData();
	  us.fWriteFile = fFalse;
	  break;
	case SaveSettings:
	  is.szFileOut = UNCONST(s.data());
	  FOutputSettings();
	  break;
	default: break;
	};
    }
  gi.fiskde = fTrue;
}

void AstroWidget::PrintDialog()
{
  QPrintDialog qpd(print);
  qpd.getPrinterSetup(print);
}

void AstroWidget::NewPage(int page, int x, int i)
{
  QString pagestr;
  QFontMetrics qfm(*cour);
  int r;
  pagestr.sprintf(transl("Page %d"), page);
  pain->setPen(*colorTable[gi.kiOn]);
  if(page % 2)
  {
    r = qfm.size(SingleLine, pagestr.latin1()).width();
    r = (int)((float)(x - r) * (float)pain->window().width() / (float)x);
  }
  else
    r = 0;
  pain->drawText(r, ((i+2) * FONTY), pagestr.latin1());
}

void AstroWidget::Print()
{
#ifdef KDE1
  if(KMsgBox::yesNo(NULL, transl("Printing"), transl("Do you really want to print ?"), KMsgBox::QUESTION) == 1)
#else
    QString text = transl("Do you really want to print ?"), caption = transl("Printing");
    if(KMessageBox::questionYesNo(NULL, text, caption) == KMessageBox::Yes)
#endif
  {
    QPainter pr, *p = pain;
    QPaintDeviceMetrics qpdm(print);
    int x = qpdm.width() - 2 * MARGINS, y = qpdm.height() - 2 * MARGINS;
    key = ' ';
    is.cchRow = 0;
    setCursor(waitCursor);
    pain = &pr;
    Printing = TRUE;
    pain->begin(print);
    QPen& pn = (QPen&)pain->pen();
    pn.setWidth(BOLDLINE);
    if(gm != GMText)
    {
      if(print->orientation() == QPrinter::Portrait)
	pain->setViewport(MARGINS + gs.portx, MARGINS + gs.porty, x, (int)((float)x * (float)gs.yWin / (float)gs.xWin));
      else
	pain->setViewport(MARGINS + gs.landx, MARGINS + gs.landy, (int)((float)gs.xWin * (float)y / (float)gs.yWin), y);
      gi.nScaleT *= FACT;
      gs.xWin *= FACT;
      gs.yWin *= FACT;
      gs.nScale *= FACT;
      pain->setWindow(0, 0, gs.xWin, gs.yWin);
      InteractX();
      gs.xWin /= FACT;
      gs.yWin /= FACT;
      gi.nScaleT /= FACT;
      gs.nScale /= FACT;
    }
    else
    {
      int i, j, x1, page = 1, h = y + 2 * MARGINS;
      char col, *c, *c1;
      KI lc;
      AstroStringList::Iterator *s = sl->first(), s1;
      pain->setViewport(MARGINS, MARGINS, x, y);
      pain->setWindow(0, 0, viewport()->width(), pain->window().height());
      cour->setPointSize(FONTY);
      pain->setFont(*cour);
      const QFontMetrics& qfm = pain->fontMetrics();
      for(i = 0, j = 0; s; i++, j++, INC(s, sl))
      {
	if((i * FONTY) > h)
	{
	  lc = LastCol(j);
	  NewPage(page, viewport()->width(), i);
	  pain->setPen(*colorTable[lc]);
	  i = 0;
	  page++;
	  print->newPage();
	}
	x1 = 0;
	for(c = UNCONST(s->data()), c1 = c; *c; c++)
	{
	  if(*c == COLCHAR)
	  {
	    *c = 0;
	    pain->drawText(x1, i * FONTY, c1);
	    x1 +=  qfm.width(c1);
	    *c = COLCHAR;
	    col = *(++c) - '0';
	    switch(col)
	    {
	      case  0 : col = gi.kiOn; break;
	      case 15 : col = gi.kiOff; break;
              case  8 : col = gi.kiGray; break;
              case  7 : col = gi.kiLite; break;
              default:;
            }
	    pain->setPen(*colorTable[col]);
	    c1 = c+1;
	  }
	}
	pain->drawText(x1, i * FONTY, c1);
	x1 += qfm.width(c1);
      }
      NewPage(page, viewport()->width(), h / FONTY);
    }
    pain->end();
    pain = p;
    gi.newHeight = -1;
    Printing = FALSE;
    setPaint(' ');
    setCursor(arrowCursor);
  }
}
  
void AstroWidget::Command()
{
  AstroCommand ac;
  ac.show();
}

void AstroWidget::RunLine(const char *s)
{
if(FProcessCommandLine((char *)s))
  setPaint();
}

void AstroWidget::RunMacro(int i)
{
  if (FValidMacro(i+1))
    RunLine(szMacro[i]); 
}

void AstroWidget::SetChartInfo(CI& ci)
{
  CI* ciptr = &ci;
  AstroInfo ai(&ci);
  ai.show();
  if(ai.ok)
  {
    if(ciptr == &ciMain)
      ciCore = ci;
    setPaint();
  }
}

void AstroWidget::SetChartDefaultInfo()
{
  CI localci;
  localci.dst = us.dstDef;
  localci.zon = us.zonDef;
  localci.lon = us.lonDef;
  localci.lat = us.latDef;
  localci.nam = nullnamloc;
  localci.loc = nullnamloc;
  localci.comment = nullnamloc;
  localci.tzfile = us.tzfile;
  AstroInfo ai(&localci, FALSE);
  ai.show();
  us.dstDef = localci.dst;
  us.zonDef = localci.zon;
  us.lonDef = localci.lon;
  us.latDef = localci.lat;
  us.tzfile = localci.tzfile;
}

void AstroWidget::SetRelation(int i)
{
  if(us.nRel == 0 && i == 0)
    us.nRel = rcDual;
  else if(us.nRel == rcDual && i == 0)
    us.nRel = 0;
  else
    us.nRel = i;
  if(us.nRel == rcTransit || us.nRel == rcProgress)
  {
    CI ciTemp = ciCore;
    FInputData(szNowCore);
    ciTwin = ciCore;
    ciCore = ciTemp;
  }
  else
    ciCore = ciMain;
  setPaint();
}

void AstroWidget::SetPalette()
{
  SetAstroColor sac; sac.show(); setPaint(' ');
}

void AstroWidget::SetEAColors()
{
 SetColors sc; sc.show(); setPaint();
}

void AstroWidget::AspectSettings()
{
  AstroSetAspects asa; asa.show(); setPaint();
}

void AstroWidget::ObjectSettings(const char *s, int o1, int o2)
{
  AstroObjectSet aos(s, o1, o2); aos.show(); setPaint();
}

void AstroWidget::Restrict(const char *s, byte* i1, byte* i2)
{
  Restrictions r(s, i1, i2); r.show(); setPaint();
}

void AstroWidget::ToggleObj(int i1, int i2, bool b)
{
  int i;
  for(i = i1; i < i2; i++)
    ignore[i+1] = b;
  setPaint();
}

void AstroWidget::RestrictStar()
{
  StarRestrict sr; sr.show(); setPaint();
}

void AstroWidget::CalculationSettings()
{
  CalcSettings cs; cs.show(); setPaint();
}

void AstroWidget::ObscureSetting()
{
  ObscureSettings os; os.show(); setPaint();
}

void AstroWidget::ChartType(void* ptr, char c)
{
  us.fListing = us.fWheel = us.fGrid = us.fAspList = us.fMidpoint = us.fHorizon = us.fOrbit = us.fSector = us.fCalendar = us.fInfluence = us.fAstroGraph = us.fEphemeris = us.fInDay = us.fHorizonSearch = us.fTransit = us.fTransitInf = us.fSign = us.fObject = us.fAspect = us.fConstel = us.fOrbitData = us.fMeaning = us.fKeyGraph = us.fSwitch = us.fSwitchRare = us.fCredit = fFalse;  
  us.nArabic = 0;
  if(ptr == &us.nArabic)
    *((int*)ptr) = 1;
  else
    *((_bool*)ptr) = fTrue;
  if(ptr == &us.fHorizon)
      us.fHorizonSearch = (c == ' ') ? fTrue : fFalse;
  if(!us.fWheel)
    Ksb->changeItem("", 1);
  if(ptr == &us.fAspList || ptr == &us.fMidpoint || ptr == &us.nArabic || ptr == &us.fHorizon)
    {
      gi.nMode = gList;
      PrintTheChart(is.fProgress, fFalse);
      return;
    }
  else if(ptr == &us.fSign || ptr == &us.fObject || ptr == &us.fAspect || ptr == &us.fConstel || ptr == &us.fOrbitData || ptr == &us.fMeaning || ptr == &us.fKeyGraph || ptr == &us.fSwitch || ptr == &us.fSwitchRare || ptr == &us.fCredit)
  {
    gi.nMode = gList;
    PrintTheChart(fFalse, fTrue);
    return;
  }
  if(ptr != &us.fListing && us.fGraphics)
  {
    NEWHEIGHT;
    SetGm(GMGraph);
    setPaint(c);
  }
  else
  {
    gi.nMode = gList;
    PrintTheChart(fFalse, fFalse);
  }
}

void AstroWidget::PrintTheChart(_bool b, _bool table)
{
  viewport()->update();
  emit resizeContents(gs.xWin, gs.yWin + OFFSET);
  setCursor(waitCursor);
  gi.newHeight = 0;
  gi.fixcour = fTrue;
  is.cchCol = 1;
  is.cchRow = 0;
  SetGm(GMText);
  if(table)
    FPrintTables();
  else
    PrintChart(b);
  PrintSz("\n");
  gi.fixcour = fFalse;
  setCursor(arrowCursor);
  if(gi.newHeight)
  {
    emit resizeContents(width(), (gi.newHeight == -1) ? gs.yWin + OFFSET: gi.newHeight);
    gi.newHeight = 0;
  }
  viewport()->update();
}

void AstroWidget::ShowGlobe(int g)
{
  us.fListing = us.fWheel = us.fGrid = us.fAspList = us.fMidpoint = us.fHorizon = us.fOrbit = us.fSector = us.fCalendar = us.fInfluence = us.fAstroGraph = us.fEphemeris = us.fInDay = us.fHorizonSearch = us.fTransit = us.fTransitInf = us.fSign = us.fObject = us.fAspect = us.fConstel = us.fOrbitData = us.fMeaning = us.fKeyGraph = us.fSwitch = us.fSwitchRare = us.fCredit = fFalse;  
  us.nArabic = 0;
  gi.nMode = g;
  us.fGraphics = fTrue;
  SetGm(GMGraph);
  NEWHEIGHT;
  setPaint();
}

void AstroWidget::ShowInterp()
{
  if(us.fListing)
    ChartType(&us.fListing, List);
  else if(us.fGrid)
    ChartType(&us.fGrid, Grid);
  else if(us.fAspList)
    ChartType(&us.fAspList, Aspect);
  else if(us.fMidpoint)
    ChartType(&us.fMidpoint, MidPoint);
  else if(us.fTransit || us.fTransitInf || us.fInDay || us.fInDayInf)
    PrintTheChart(is.fProgress, fFalse);
}

bool AstroWidget::Transits()
{
  AstroTransits at; at.show();
  if(at.print)
  {
    gi.nMode = gList;
    PrintTheChart(is.fProgress, fFalse);
    return TRUE;
  }
  return FALSE;
}

bool AstroWidget::Progressions()
{
  AstroProgress ap; ap.show();
  if(ap.prog)
  {
    gi.nMode = gList;
    PrintTheChart(is.fProgress, fFalse);
    return TRUE;
  }
  return FALSE;
}

void AstroWidget::ChartSettings()
{
  ChartDetails cd; cd.show(); setPaint();
}

void AstroWidget::GraphicSettings()
{
  GraphSettings gs; gs.show(); setPaint(' ');
}
 
void AstroWidget::timerslot()
{
  if(!Pause)
    setPaint(' ');
}

void AstroWidget::SetDir(int c)
{
  int i = (number >= 0) ? 1 : -1;
  if(c == -1)
    number = -number;
  else
   number = i * c;
}

void AstroWidget::TimedExpos()
{
  not2(Exposed);
  if(tim)
    tim->changeInterval(Exposed ? gi.nTimerDelay : 0);
}

void AstroWidget::PlusMinus(char c)
{
  number = abs(number);
  int nb = number;
  gs.nAnim = jrate;
  if(c == '-')  
    SetDir(-1);
  setPaint(' ');
  gs.nAnim = 0;
  number = nb;
}

void AstroWidget::Anim(int rate)
{
  if(gm == GMText)
    return;
  switch(rate)
  {
     case 0 : if(!gs.nAnim)
              {
		gs.nAnim = jrate;
		gm = GMDblbuf;
		dblbuf = new QPixmap(viewport()->width(), viewport()->height());
		CHECK_PTR(dblbuf);
		dblbuf->setOptimization(QPixmap::BestOptim);
		tim = new QTimer;
		CHECK_PTR(tim);
		connect(tim, SIGNAL(timeout()), this, SLOT(timerslot()));
		tim->start(Exposed ? gi.nTimerDelay : 0, FALSE);
	      }
              else
	      {
		gs.nAnim = 0;
		tim->stop();
		gm = GMGraph;
		delete dblbuf;
		disconnect(tim, SIGNAL(timeout()), this, SLOT(timerslot()));
		delete tim;
		tim = NULL;
	      } break;
     case -1:
     case -2:
     case -3:
     case -4:
     case -5:
     case -6:
     case -7:
     case -8:
     case -9: jrate = rate; if(gs.nAnim) gs.nAnim = jrate; break;
     case 10: gs.nAnim = 10; setPaint(); jrate = rate;
  }
}

void AstroWidget::ListFile(const char *c)
{

#ifdef KDE1
  KFM kfm;
  QRegExp http("^http:");
  QString file = kdepath + c;
  kfm.openURL((http.match(c) == -1) ? file.data() : c);
#else
  QString s = "file:" + kdepath + c;
  Ka->invokeBrowser(s);
#endif
}

void AstroWidget::About()
{
  QString credit;
  credit.sprintf(transl("%s %s, %s\nKDE version: Christophe Gros: %s\nOriginal programming: Walter D. Pullen %s\n"), szAppNameCore, szVersionCore, szDateCore, MyAddressCore, szAddressCore);
  credit += transl("Main planetary calculation formulas were converted from routines by James Neely\n as listed in 'Manual of Computer Programming  for Astrologers' by Michael Erlewine, available from Matrix Software.\nPostScript graphics routines by Brian D. Willoughby.\nExtended ephemeris calculation and formulas are by Alois Treindl, as in the package 'Placalc', available from Astrodienst AG.\nIMPORTANT: Astrolog is 'freeware', but is copyrighted and not in public domain.\nPermission is granted to freely use and distribute these routines provided one does not sell, restrict, or profit from the program or its output in any way.\nModification is allowed provided these exact notices remain with any altered or edited versions of the program.\nThese conditions are true of both the program in whole and of all parts by any individual author.\nViolators are subject to copyright law penalties, and negative karmic debts to aforementioned contributors.\nSpecial thanks to all those unmentioned, seen and unseen, who have pointed out problems, suggested features, & sent many positive vibes! :)");      
  QMessageBox about(NULL, credit.data(), QMessageBox::Information, QMessageBox::Ok, 0, 0);
  about.setIconPixmap(*kdeicon);
  emit about.setCaption(transl("About"));
  about.show();
}
#ifdef KDE1
void MyDropZone::drop( char *_data, int _size, int _type, int _x, int _y )
{
  QString s = _data, target;
  KURL ku(s);
  s = ku.path();
  CI ciSave = ciCore;
  if(s.data())
    {
      FInputData(s.data());
      CastChart(fTrue);
      central->setPaint();
      ciCore = ciSave;
    }
}
#else
void AstroWidget::viewportDropEvent(QDropEvent* de)
{
  QString s, reg = "\\.kst$";
  QRegExp qre(reg);
  KURL::List kl;
  KURL::List::Iterator it;
  KURLDrag::decode(de, kl);
  for(it = kl.begin(); it != kl.end(); it++)
  {
    s = KURL::decode_string((*it).path());
    if(qre.match(s.latin1()) != -1)
    {
      CI ciSave = ciCore;
      FInputData((char *)s.latin1());
      CastChart(fTrue);
      central->setPaint();
      ciCore = ciSave;
      break;
    }
    else
    KMessageBox::error(NULL, transl("Illegal file type"), transl("Error"));
  }
}

void AstroWidget::viewportDragEnterEvent(QDragEnterEvent* qdev)
{
  qdev->accept(QTextDrag::canDecode(qdev));
}

#endif
