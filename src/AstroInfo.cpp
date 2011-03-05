/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :         kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroInfo.cpp
	Last generated: Tue Sep 28 14:38:42 1999

 *********************************************************************/

#include <kfiledialog.h>
#include "AstroInfo.h"

#define NUMERROR 100000
#define Inherited AstroInfoData

extern "C"
{
#include <ctype.h>

void gettz(CI *ci)
{
  AstroTZ atz(ci->tzfile, ci->tim, ci->day, ci->mon, ci->yea);
  ci->zon = -atz.setztime();
  ci->dst = atz.isdst();
}

}

AstroInfo::AstroInfo(QWidget* parent, const char* name) : Inherited(parent, name)
{
}


AstroInfo::~AstroInfo()
{
}

const char* AstroInfo::SzLatLong(real angle, bool latitude, bool lhms)
{
  static char szLoc[10];
  int i, j, k, m, s;
  real sign, x, y;
  char ch;
  sign = angle < 0.0 ? -1.0 : 1.0;
  if(lhms)
  {
    x = RAbs(angle);
    y = RFloor((x - RFloor(x) +0.00005) * 10000.0) * 100.0 / 60.0;
    x = (RFloor(x) + (y / 10000.0)) * 86400.0 / 360.0;
    s = (int)x;
    k = s / 3600;
    angle = ((real)(k) + 0.01) * sign;
    m = (s - (k * 3600));
    i = m / 60;
    j = m - (i * 60);
  }
  else
    i = (int)(RFract(RAbs(angle))*100.0+rRound);
  ch = us.fAnsiChar > 1 ? 128 : chDeg1;
  if (us.fAnsiChar != 3)
  {
    if(lhms)
      sprintf(szLoc, "%3.0f%c%02d%c%02d",
	      RFloor(RAbs(angle)), ch, i, sign < 0.0 ? (latitude ? 'S' : 'E') : (latitude ? 'N' : 'W'), j);
    else
      sprintf(szLoc, "%3.0f%c%02d%c",
	      RFloor(RAbs(angle)), ch, i, sign < 0.0 ? (latitude ? 'S' : 'E') : (latitude ? 'N' : 'W'));
  }
  else
  {
    if(gi.longhms)
      sprintf(szLoc, "%3.0f%c%02d%3.0f%c%02d'%02d",
	      RFloor(RAbs(angle)), ch, i, sign < 0.0 ? (latitude ? 'S' : 'E') : (latitude ? 'N' : 'W'), j);
    else
      sprintf(szLoc, "%3.0f%c%02d%3.0f%c%02d",
	      RFloor(RAbs(angle)), ch, i, sign < 0.0 ? (latitude ? 'S' : 'E') : (latitude ? 'N' : 'W'));
  }
  return (char *)szLoc;
}
 
AstroInfo::AstroInfo(CI* ci)
{
  emit setCaption(transl("Enter Chart Info"));
  SetAstroInfo(ci, TRUE);
}

AstroInfo::AstroInfo(CI* ci, bool b)
{
  emit setCaption(transl("Default Info"));
  SetAstroInfo(ci, b);
}

void AstroInfo::SetAstroInfo(CI* ci, bool normal)
{
  char buf[80];
  bool ansi = TRUE;
  ok = FALSE;
  ciptr = ci;
  myci = *ciptr;
  tzfile[0] = 0;
  w = width();
  h = height();
  cw = Comment->width();
  ch = Comment->height();

  if(normal)
  {
    Month->setCurrentItem(myci.mon - 1);
    sprintf(buf, "%d", myci.day);
    emit TheDay->setText(buf);
    sprintf(buf, "%d", myci.yea);
    emit Year->setText(buf);
    emit Time->setText(SzTim(myci.tim));
  }
  else
  {
    emit Month->setEnabled(FALSE);
    emit TheDay->setEnabled(FALSE);
    emit Year->setEnabled(FALSE);
    emit Time->setEnabled(FALSE);
    emit monthlabel->setEnabled(FALSE);
    emit daylabel->setEnabled(FALSE);
    emit yearlabel->setEnabled(FALSE);
    emit timelabel->setEnabled(FALSE);
    emit namelabel->setEnabled(FALSE);
    emit localtionlabel->setEnabled(FALSE);
    emit nowlabel->setEnabled(FALSE);
    emit previouslabel->setEnabled(FALSE);
    emit commentlabel->setEnabled(FALSE);
    emit Comment->setEnabled(FALSE);
  }
  if(myci.dst)
    Daylight->setChecked(TRUE);
  if(gi.longhms)
    LongHMS->setChecked(TRUE);
  emit Timezone->setText(SzZone(-myci.zon));
  if(us.fAnsiChar)
  {
    us.fAnsiChar = fFalse;
    ansi = FALSE;
  }
  emit Longitude->setText(SzLatLong(myci.lon, FALSE, gi.longhms));
  emit Latitude->setText(SzLatLong(myci.lat, TRUE, FALSE));
  if(!ansi)
  {
    us.fAnsiChar = fTrue;
    ansi = TRUE;
  }
  if(normal)
  {
    strncpy(namebuf, ciptr->nam, 80);
    myci.nam = namebuf;
    emit Name->setText(myci.nam);
    strncpy(location, ciptr->loc, 80);
    myci.loc = location;
    emit Location->setText(myci.loc);
  }
  else
  {
    emit Name->setEnabled(FALSE);
    emit Location->setEnabled(FALSE);
  }
  if(ciptr->tzfile)
    strncpy(tzfile, ciptr->tzfile, 80);
  myci.tzfile = tzfile;
  if(myci.tzfile && myci.tzfile[0])
    settz(myci.tzfile, FALSE);
  myci.comment = ciptr->comment;
  emit Comment->setText(ciptr->comment);
  sprintf(buf, transl("TZ file: %s"), myci.tzfile);
  WriteError(buf);
  connect(TheDay, SIGNAL(focusoutsignal()), this, SLOT(dayslot()));
  connect(Year, SIGNAL(focusoutsignal()), this, SLOT(yearslot()));
  connect(Time, SIGNAL(focusoutsignal()), this, SLOT(timeslot()));
  connect(Timezone, SIGNAL(focusoutsignal()), this, SLOT(timezoneslot()));
  connect(Longitude, SIGNAL(focusoutsignal()), this, SLOT(longitudeslot()));
  connect(Latitude, SIGNAL(focusoutsignal()), this, SLOT(latitudeslot()));
}

void AstroInfo::previousslot()
{
  SetAstroInfo(ciptr, TRUE);
}

void AstroInfo::okslot()
{
  close(FALSE);
  myci.mon = Month->currentItem() + 1;
  real d = Daylight->isChecked() ? 1.0 : 0.0;
  dayslot();
  yearslot();
  timeslot();
  timezoneslot();
  latitudeslot();
  longitudeslot();
  nameslot();
  if(ciptr->nam != nullnamloc && ciptr->nam)
    DeallocateNear(ciptr->nam);
  locationslot();
  if(ciptr->loc != nullnamloc && ciptr->loc)
    DeallocateNear(ciptr->loc);
  if(ciptr->tzfile != nullnamloc && ciptr->tzfile)
    DeallocateNear(ciptr->tzfile);
  if(ciptr->comment != nullnamloc && ciptr->comment)
    DeallocateNear(ciptr->comment);
  myci.dst = d;
  *ciptr = myci;
  ciptr->nam = SzPersist(namebuf);
  ciptr->loc = SzPersist(location);
  ciptr->tzfile = SzPersist(tzfile);
  ciptr->comment = SzPersist((char *)Comment->text().data());
  ok = TRUE;
}

void AstroInfo::cancelslot()
{
  close(FALSE);
}

void AstroInfo::dayslot()
{
  int i = atoi(TheDay->text());
  if(!FBetween(i, 1, 31))
    WriteError(transl("Illegal day value"));
  else
    {
      myci.day = i;
      setdst();
      WriteError(NULL);
    }
}

void AstroInfo::yearslot()
{
  char *errptr = (char *)-1;
  int i = strtol(Year->text(), &errptr, 10);
  if(*errptr != 0 || errptr == Year->text())
    {
      WriteError(transl("Illegal year value"));
      return;
    }
    i = NParseSz(Year->text(), pmYea);
  if(!FBetween(i, -5000, 5000))
    WriteError(transl("Illegal year value"));
  else
    {
      myci.yea = i;
      setdst();
      WriteError(NULL);
    }
}

static bool in(char c, const char *s)
{
  const char *p;
  if(!s)
    return FALSE;
  for(p = s; *p; p++)
    if(*p == c)
      return TRUE;
  return FALSE;
}

static int GetNum(char **s, int i1, int i2, bool allow, const char *seps = NULL)
{
  char buf[4], *ptr = *s, *s1 = *s, c;
  int i = 0, j = 0, result;
  if(allow && seps)
    while(c = *s1++)
      if(!isdigit(c) && !in(c, seps) && j++ < 2)
	return NUMERROR;
  while(*ptr && isdigit(*ptr) && i < 3)
    buf[i++] = *ptr++;
  buf[i] = 0;
  result = atoi(buf);
  if(result < i1 || result > i2)
    return NUMERROR;
  else
  {
    *s = ptr;
    return result;
  }
}

static real GetHM(char **str, int i1, int i2, char *sep, bool allow, const char *seps = NULL)
{
  int h, m;
  h = GetNum(str, i1, i2, allow, seps);
  if(h == NUMERROR)
    return (real)NUMERROR;
  if(! in(**str, seps))
  {
    *sep = ':';
    return (real)h;
  }
  *sep = **str;
  (*str)++;
  m = GetNum(str, 0, 59, allow, seps);
  if(m == NUMERROR)
    return (real)NUMERROR;
  return (real)h + (real)m / 100.0;
}

static void GetHMSLong(real *r, char **c, const char *sb)
{
  char *sptr;
  int h = (int)*r, m, s;
  real x = (*r - (real)h) * 100.0, y;
  m = (int)(x + 0.005);
  *r = (real)(h * 3600 + m * 60);
  sptr = (*c)++;
  s = GetNum(c, 0, 59, FALSE);
  if(s == NUMERROR)
  {
    *r = (real)NUMERROR;
    return;
  }
  *c = sptr;
  x =  (*r + (real)s) * 360.0 / 86400.0;
  y = RFloor((x - RFloor(x) + 0.00005) * 10000.0);
  y *= 60.0 / 100.0;
  *r = RFloor(x) + (y / 10000.0);
  
}

static void PackString(char *s)
{
  char *c;
  for(c = s; *s; s++)
  {
    if(*s != ' ')
      *c++ = tolower(*s);
  }
  *c = 0;
}

bool AstroInfo::GetTime(const char *s, real *t)
{
  char buf[80], *c = buf, sep;
  strncpy(buf, s, 80);
  PackString(buf);
  real hm = GetHM(&c, 0, 23, &sep, FALSE, ":");
  if(hm == (real)NUMERROR)
  {
    WriteError(transl("Hour syntax : [0..23]:[0..59]"));
    return FALSE;
  }
  if(hm >= 12.00 && strcmp(c, "am") == 0)
    hm -= 12.00;
  if(hm < 12.00 && strcmp(c, "pm") == 0)
    hm += 12.00;
  if(*c && strcmp(c, "am") && strcmp(c, "pm"))
  {
    WriteError(transl("The hour can only be followed by 'am' or 'pm'"));
    return FALSE;
  }
  *t = hm;
  return TRUE;
}

bool AstroInfo::GetLatLong(const char *s, real *t, bool latitude = TRUE)
{
  char buf[80], sb[5] = ":°", *c = buf, sep;
  real hm;
  strncpy(buf, s, 80);
  PackString(buf);
  strcat(sb, latitude ? "ns" : "ew");
  hm = GetHM(&c, 0, latitude ? 89 : 179, &sep, TRUE, sb);
  if(hm == (real)NUMERROR)
  {
    WriteError(latitude ? transl("Latitude syntax: [0..89]x[0..59]  x= ':°ns'") :
     gi.longhms ? transl("Long. syntax: [0..179]x[0..59]y[0..59] x,y= ':°ew'") : transl("Longitude syntax: [0..179]x[0..59]  x= ':°ew'"));
    return FALSE;
  }
  if(!latitude && gi.longhms)
    GetHMSLong(&hm, &c, sb);
  if(hm == (real)NUMERROR)
  {
    WriteError(latitude ? transl("Latitude syntax: [0..89]x[0..59]  x= ':°ns'") :
     gi.longhms ? transl("Long. syntax: [0..179]x[0..59]y[0..59] x,y= ':°ew'") : transl("Longitude syntax: [0..179]x[0..59]  x= ':°ew'"));
    return FALSE;
  }
  if(sep ==  (latitude ? 's' : 'e'))
    hm *= -1.00;
  if(in(sep, ":°"))
  {
    if(!in(*c, latitude ? "ns" : "ew"))
    {
      WriteError(latitude ? transl("The latitude must be followed by 'n' or 's'") :
	       gi.longhms ? transl("You must provide a 'e' or 'w' after the minute") : transl("The longitude must be followed by 'e' or 'w'"));
      return FALSE;
    }
    if(*c ==  (latitude ? 's' : 'e'))
      hm *= -1.00;
  }
  *t = hm;
  return TRUE;
}

void AstroInfo::WriteError(const char *c)
{
  emit Errors->setText(c ? c : "");
}

void AstroInfo::monthslot(int i)
{
   myci.mon = Month->currentItem() + 1;
   setdst();
}

void AstroInfo::timeslot()
{
  real r;
  if(GetTime(Time->text(), &r))
  {
    WriteError(NULL);
    myci.tim = r;
    setdst();
  }
}

void AstroInfo::daylightslot(bool b)
{
  myci.dst = b ? 1.0 : 0.0;
}

void AstroInfo::settzslot()
{
  setdst();
  myci.zon = -setztime();
  emit Timezone->setText(SzZone(-myci.zon));
}

void AstroInfo::longhmsslot(bool b)
{
  longitudeslot();
  gi.longhms = b;
  emit Longitude->setText(SzLatLong(myci.lon, FALSE, b));
}
  
void AstroInfo::timezoneslot()
{
  real r = RParseSz(Timezone->text(), pmZon);
  if(!FBetween(r, -12.0, 12.0))
    WriteError(transl("Illegal time zone value"));
  else
  {
    WriteError(NULL);
    myci.zon = r;
    setdst();
  }
}

void AstroInfo::latitudeslot()
{
  real r;
  if(GetLatLong(Latitude->text(), &r))
  {
    WriteError(NULL);
    myci.lat = r;
  }
}

void AstroInfo::longitudeslot()
{
  real r;
  if(GetLatLong(Longitude->text(), &r, FALSE))
  {
    WriteError(NULL);
    myci.lon = r;
  }
}

void AstroInfo::nameslot()
{
  strncpy(namebuf, Name->text(), 80);
}

void AstroInfo::locationslot()
{
  strncpy(location, Location->text(), 80);
}

void AstroInfo::setdst()
{
  if(!*tzfile) return;
  settime(myci.tim, myci.day, myci.mon, myci.yea);
  if(isdst() > 0)
    {
      myci.dst = 1.0;
      Daylight->setChecked(TRUE);
    }
  else
    {
      myci.dst = 0.0;
      Daylight->setChecked(FALSE);
    }
}


void AstroInfo::timezonefileslot()
{
  QString buf;
  int i;
  buf.sprintf("%s", strlen(tzfile) ? tzfile : ZoneInfoPath);
  QString s = KFileDialog::getOpenFileName(buf.data());
  if(s.data())
    {
      emit dayslot();
      emit yearslot();
      emit timeslot();
      strncpy(tzfile, s.data(), 80);
      settz(s.data());
      setdst();
      myci.zon = setztime();
      emit Timezone->setText(SzZone(myci.zon));
      buf.sprintf(transl("TZ file : %s"), s.data());
      WriteError(buf.data());
    }
}

void AstroInfo::nowslot()
{
  char buf[7];
  GetTimeNow(&myci.mon, &myci.day, &myci.yea, &myci.tim, myci.zon);
  settime(myci.tim, myci.day, myci.mon, myci.yea);
  Month->setCurrentItem(myci.mon - 1);
  sprintf(buf, "%d", myci.day);
  emit TheDay->setText(buf);
  sprintf(buf, "%d", myci.yea);
  emit Year->setText(buf);
  emit Time->setText(SzTim(myci.tim));
  setdst();
}

void AstroInfo::resizeEvent(QResizeEvent *qrse)
{
  int iw = qrse->size().width() - w, ih = qrse->size().height() - h;
  Comment->resize(cw + iw, ch + ih);
}

void AstroTZ::settz(const char *f, bool set)
{
  char *c, varbuf[60];
  if(f)
    {
      c = getenv("TZ");
      if(c)
	strncpy(tzsave, c, 80);
      varbuf[0] = ':';
      varbuf[1] = 0;
      strncat(varbuf, f, 80);
      setenv("TZ", varbuf, 1);
      if(set)
	tzset();
    }
}

void AstroTZ::settime(real tim, int day, int mon, int yea)
{
  struct tm tm;
  int h, m;
  h = (int)NFloor(tim);
  m =  (int)(RFract(RAbs(tim))*100.0+rRound/600.0);
  tm.tm_sec = 0;
  tm.tm_min = m;
  tm.tm_hour = h;
   tm.tm_mday = day;
  tm.tm_mon = mon - 1;
  tm.tm_year = yea-1900;
  tt = mktime(&tm);
}

double AstroTZ::setztime()
{
  struct tm *tmptrg;
  double dt, zheure, zminute;
  //tmptrl = localtime(&tt);
  tmptrg =  gmtime(&tt);
  dt = difftime(mktime(tmptrg), tt);
  zminute = floor(dt/60.0);
  zheure = ceil(zminute / 60.0);
  zminute = floor(zminute - (zheure * 60.0));
  return -(zheure + zminute / 100.0);
}

int AstroTZ::isdst()
{
  struct tm *tmptrl;
  tmptrl = localtime(&tt);
 return tmptrl->tm_isdst;
}

AstroTZ::~AstroTZ()
{
  if(tzsave[0])
    setenv("TZ", tzsave, 1);
  else
    unsetenv("TZ");
}









