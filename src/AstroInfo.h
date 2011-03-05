/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :         kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroInfo.h
	Last generated: Tue Sep 28 14:38:42 1999

 *********************************************************************/

#ifndef AstroInfo_included
#define AstroInfo_included

#undef bool
// Just for a weird behavior during compilation

#include "AstroInfoData.h"

extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
}
class AstroTZ
{
 public:
  
  inline AstroTZ()                   { tzsave[0] = 0; }
  inline AstroTZ(const char *f)      { settz(f); }
  inline AstroTZ(const char *f, real tim, int day, int mon, int yea)
                                     { settz(f); settime(tim, day, mon, yea); }
  ~AstroTZ();

  void   settime(real, int, int, int); // Set local time
  double setztime();    // return time zone ofset
  int    isdst();       // return daylisght offset
  void   settz(const char *, bool set = TRUE); // Set 

 private:

  time_t tt;
  char tzsave[80];
};


class AstroInfo : public AstroInfoData, AstroTZ
{
    Q_OBJECT

public:

    AstroInfo(QWidget* parent = NULL, const char* name = NULL);
    AstroInfo(CI*);
    AstroInfo(CI*, bool);
    virtual ~AstroInfo();
    bool ok;
public slots:

 void okslot();
 void cancelslot();
 void monthslot(int);
 void dayslot();
 void yearslot();
 void timeslot();
 void daylightslot(bool);
 void longhmsslot(bool);
 void timezoneslot();
 void latitudeslot();
 void longitudeslot();
 void nameslot();
 void locationslot();
 void timezonefileslot();
 void nowslot();
 void previousslot();
 void resizeEvent(QResizeEvent*);
 void settzslot();

private:

 CI myci, *ciptr;
 char namebuf[80], location[80], tzfile[80];
 int w, h, cw, ch;

 const char *SzLatLong(real, bool, bool);
 bool GetTime(const char*, real*);
 bool GetLatLong(const char*, real*, bool latitude = TRUE);
 void SetAstroInfo(CI*, bool);
 void WriteError(const char *);
 void setdst();
};
#endif // AstroInfo_included



