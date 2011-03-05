/*
** Astrolog (Version 5.42J) File: riyal.c
**
** IMPORTANT NOTICE: The graphics database and chart display routines
** used in this program are Copyright (C) 1991-1998 by Walter D. Pullen
** (Astara@msn.com, http://www.magitech.com/~cruiser1/astrolog.htm).
** Permission is granted to freely use and distribute these routines
** provided one doesn't sell, restrict, or profit from them in any way.
** Modification is allowed provided these notices remain with any
** altered or edited versions of the program.
**
** The main planetary calculation routines used in this program have
** been Copyrighted and the core of this program is basically a
** conversion to C of the routines created by James Neely as listed in
** Michael Erlewine's 'Manual of Computer Programming for Astrologers',
** available from Matrix Software. The copyright gives us permission to
** use the routines for personal use but not to sell them or profit from
** them in any way.
**
** The PostScript code within the core graphics routines are programmed
** and Copyright (C) 1992-1993 by Brian D. Willoughby
** (brianw@sounds.wa.com). Conditions are identical to those above.
**
** The extended accurate ephemeris databases and formulas are from the
** calculation routines in the library SWISS EPHEMERIS and are programmed and
** copyright 1998 by Astrodienst AG.
** The use of that source code is subject to
** the Swiss Ephemeris Public License, available at
** http://www.astro.ch/swisseph. This copyright notice must not be
** changed or removed by any user of this program.
**
** Modifications from version 5.41 to 5.42 are by Nicolas Scharnagl

*/
#include "astrolog.h"
#include "config.h"
#ifdef ASTEROIDS
#include "swephexp.h"
#include "sweph.h"
#include "swephlib.h"

/* Global */
char *riyalObjName[cRiya]={"1999RZ215","1999CF119","1999CY118","1999CZ118","2000PF30","2000PJ30",
                           "2000OM67","2008SJ236","2008QD4","1999RD215","2000EE173","2007VK305",
                           "2007UM126", "1995SM55",
                           "1999TC36","2000CR105","2000OO67","2007RH283","2007RW10","2005VJ119","1995TL8",
                           "2000PH30","2005RO43","2001FZ173","2001FP185","2002KX14","2002MS4","2003AZ84","2002TC302","Ceto", "2004PG115","2004TF282","2003YQ179", /* tnpso.fle */
                           "1998XY95","1999RU214","1999RZ214","1999RJ215","2000CQ105",

                           "TA94","2000PE30","2000SR331","Pelion","2000YY1","Cyllarus","2001FM194","2001KG76","2001KG77","Bienor","1999XX143","Amycus","Crantor","1999OX3","2001BL41","2000QB243","2000FZ53",
                           "2001OM109","2001QX322", "2002AW197","2002TX300","2002UX25","2003LA7","Quaoar","2001XQ254","2002GA32", "Okyrhoe","Echeclus","2000GM137","2000CO104","2002PN34","2001SQ73","2002DH5","2003CO1","2002GZ32","2001XZ255","2001XA255","2001KF77","1999HD12",
                           "1999JV127","1995SN55","1996RX33","1996AR20","1996AS20","2002CA249","2002KY14","2002PQ152","2002FY36","2002TK301","2002VG131","2002VR130","2002QX47","2000SN331",/* centaurs.fle */
                           "2004NN8","2005SB223","2006LM1","2005TJ50","2005VD","2006RG1","2006BZ8","1997MD10","1998WU24","1999XS35","2000KP65","2006EX52",
                           "1999LE31","2003UY283","2003WN188","1998QJ1","2000DG8","1998HO121","2004CM111","2000AB229","2000HE46","2002RP120","2001QF6","2002CE10","2001YK61","2003CC22","2006RJ2",  /*asteroid.fle */
                           "2008ST291","2005QU182","2006SQ372","Ulysses","Cassini","Rosetta","VoyagerI","VoyagerII","Pioneer10","Pioneer11" /* .sx1 */
                          };


/* Added by N.Scharnagl ,using Riyal code and ephemerides */
/* See www.expreso.co.cr/centaurs */

void Everett(n,p,x,f,d)
double *f,*d,p;
int n,x[6];
{
  double  q,v2,v4,e2,e4,fy,fx,f0,f1,f2,f3,d20,d21,d40,d41,md=10.0,n2=12.0,n5=15.0;
  q=1.0-p;
  v2=(q+1.0)*q*(q-1.0)/6.0;
  e2=(p+1.0)*p*(p-1.0)/6.0;
  v4=v2*(q+2.0)*(q-2.0)/2.0/md;
  e4=e2*(p+2)*(p-2)/2.0/md;
  fy=(double)x[n-3];
  fx=(double)x[n-2];
  f0=(double)x[n-1];
  f1=(double)x[n];
  f2=(double)x[n+1];
  f3=(double)x[n+2];
  d20=f1-2.0*f0+fx;
  d21=f2-2.0*f1+f0;
  d40=f2-4.0*f1+6.0*f0-4.0*fx+fy;
  d41=f3-4.0*f2+6.0*f1-4.0*f0+fx;
  *f= q*f0+p*f1+v2*d20+e2*d21+v4*d40+e4*d41;
  v2=(3.0*q*q-1.0)/6.0;
  e2=(3.0*p*p-1.0)/6.0;
  v4=(5.0*q*q*q*q-n5*q*q+4.0)/n2/md;
  e4=(5.0*p*p*p*p-n5*p*p+4.0)/n2/md;
  *d= f1-f0-v2*d20+e2*d21-v4*d40+e4*d41;
  return;
}
void REverett(n,p,x,f,d)
double *f,*d,p,x[6];
int n;
{
  double  q,v2,v4,e2,e4,fy,fx,f0,f1,f2,f3,d20,d21,d40,d41,md=10.0,n2=12.0,n5=15.0;
  q=1.0-p;
  v2=(q+1.0)*q*(q-1.0)/6.0;
  e2=(p+1.0)*p*(p-1.0)/6.0;
  v4=v2*(q+2.0)*(q-2.0)/2.0/md;
  e4=e2*(p+2)*(p-2)/2.0/md;
  fy=(double)x[n-3];
  fx=(double)x[n-2];
  f0=(double)x[n-1];
  f1=(double)x[n];
  f2=(double)x[n+1];
  f3=(double)x[n+2];
  d20=f1-2.0*f0+fx;
  d21=f2-2.0*f1+f0;
  d40=f2-4.0*f1+6.0*f0-4.0*fx+fy;
  d41=f3-4.0*f2+6.0*f1-4.0*f0+fx;
  *f= q*f0+p*f1+v2*d20+e2*d21+v4*d40+e4*d41;
  v2=(3.0*q*q-1.0)/6.0;
  e2=(3.0*p*p-1.0)/6.0;
  v4=(5.0*q*q*q*q-n5*q*q+4.0)/n2/md;
  e4=(5.0*p*p*p*p-n5*p*p+4.0)/n2/md;
  *d= f1-f0-v2*d20+e2*d21-v4*d40+e4*d41;
  return;
}


void FRiyalUnnumbered(jd,helio)
int helio;
double jd;
{
  char sz[cchSzDef];
  int i,ii,j,xyz[6][3],d[6],k,ind;
  real a,b,g[6],x[6],p,dd,jde,xobs[6],xearth[6],nutlo[12],lighttime,sineps,coseps,jdestart,stepsize;
  int verylongmax=39,donelighttime; /* also for below +1*/
  int verylong[40]={CentLo+1,CentLo+2,CentLo+3,CentLo+4,CentLo+5,CentLo+6,CentLo+7,CentLo+8,CentLo+9,CentLo+10,CentLo+11,TrneLo,TrneLo+1,TrneLo+2,TrneLo+3,TrneLo+4,TrneLo+5,TrneLo+6,TrneLo+7,TrneLo+8,TrneLo+9,TrneLo+10,TrneLo+11,TrneLo+12,TrneLo+13,TrneLo+14,TrneLo+15,TrneLo+16,TrneLo+17,TrneLo+18,TrneLo+19,TrneLo+20,TrneLo+21,TrneLo+22,TrneLo+23,TrneLo+24,TrneLo+25,TrneLo+26,TrneLo+27,0};
  /*  struct save_positions *sd; */
  FILE *file;
  char *filename="tnpsdo.fle\0         ";
  char serr[AS_MAXCH];
#ifdef _BIG_ENDIAN
  byte wo[4];
#endif
  init_ephe_path();
  if (!FBetween(jd,-2e6,6e6))
    return;
  if (us.objNode)
    ignore[us.objNode]=fFalse;
  if (us.fSidereal)
    is.rSid=-swe_get_ayanamsa(jd);
  if (!fdeltaT)
    deltaT=swe_deltat(jd);
  jde = jd + deltaT;
  swe_calc(jde,SE_EARTH,(fUseJPL ? SEFLG_JPLEPH : 0) + SEFLG_NONUT+SEFLG_J2000+SEFLG_SPEED+SEFLG_XYZ+SEFLG_HELCTR+SEFLG_TRUEPOS,xearth,serr);
  sineps=swed.oec2000.seps;
  coseps=swed.oec2000.ceps;
  jdestart=2341972.5;
  stepsize=200.0;
  dd=(jde-jdestart)/stepsize;
  ind=RiyaLo;
  file=FileOpen(filename,3);
  if (file == NULL)
    {
      sprintf(sz, "File '%s' not found.", filename);
      PrintError(sz);
    }
  donelighttime=fFalse;
lightrecalcT:
  p=RFract(dd);
  if (dd<2.0||dd>580)
    goto longrange;
  for (k=0; k<cRiyaT; k++)
    if (!FIgnore(k+RiyaLo))
      /* if (k!=8&&k!=7&&k!=11&&k!=12&&!FBetween(k,17,19)&&k!=22) */
      {
        ind=k+RiyaLo;
        i=fseek(file,4*(3*585*k+3*NFloor(dd)-6),0);
        for (i=0; i<=5; i++)
          {
            for (j=0; j<=2; j++)
              {
#ifdef _BIG_ENDIAN
                wo[3] =getc(file);
                wo[2] =getc(file);
                wo[1] =getc(file);
                wo[0] =getc(file);
                memcpy(&xyz[i][j],&wo[0],4);
#else
                xyz[i][j]=getw(file);
#endif
              }
          }
        for (i=0; i<=2; i++)
          {
            for (j=0; j<=5; j++)
              {
                d[j]=xyz[j][i];
              }
            Everett(3,p,d,&a,&b);
            g[i]=a*1e-7;
            g[i+3]=b*1e-7/200;
          }
        if (!helio)
          {
            for (i = 0; i <= 5; i++)
              {
                x[i] = -xearth[i] + g[i];
                if (!us.fTopocentric)
                  xobs[i]=0;
              }
            /* rough estimation of lighttime */
            lighttime=us.fTruePos ? 0 : (sqrt(square_sum(x))*AUNIT/CLIGHT/86400.0);
            if (!us.fTruePos && !fUseJ2000&&!donelighttime)
              {
                dd=(jde-jdestart-lighttime)/stepsize;
                donelighttime=fTrue;
                goto lightrecalcT;
              }

            if (us.fTopocentric&&!us.objCenter)
              {
                if (swi_get_observer(jd, (fUseJPL ? SEFLG_JPLEPH : 0 ) + SEFLG_NONUT+SEFLG_J2000+SEFLG_SPEED+SEFLG_XYZ+SEFLG_TRUEPOS, 0, xobs, serr) != OK)
                  {
                    sprintf(sz, "%s\n","Problem calculating observer position");
                    PrintError(sz);
                  }
              }
            for (i=0; i<=5; i++)
              g[i]-=xobs[i]+xearth[i];
          }

        swi_coortrf2(g, g, -sineps, coseps);
        swi_coortrf2(g+3, g+3, -sineps, coseps);
        if (!helio && !fUseJ2000 && !fUseICRS)
          {
            if (!us.fTruePos && !us.fNoGravityDeflection )
              swi_deflect_light(g, lighttime, SEFLG_SPEED);
            if (!us.fTruePos && !us.fNoAberration )
              swi_aberr_light(g, xearth, SEFLG_SPEED);
          }
        if ( !fUseJ2000 && !fUseICRS)
          {
            swi_precess(g,jde,J2000_TO_J);
            swi_precess_speed(g,jde,J2000_TO_J);
          }
        if (!us.fNoNutation&&!us.fSidereal&&!fUseJ2000 && !fUseICRS)
          {
            swi_nutation(jde,nutlo);
            swi_nutate(g,SEFLG_XYZ+SEFLG_EQUATORIAL+SEFLG_TRUEPOS+SEFLG_SPEED,FALSE);
          }
        if (!us.fEquator)
          {
            if ( !fUseJ2000 && !fUseICRS )
              {
                swi_coortrf2(g, g, swed.oec.seps, swed.oec.ceps);
                swi_coortrf2(g+3, g+3, swed.oec.seps, swed.oec.ceps);
              }
            else
              {
                swi_coortrf2(g, g, swed.oec2000.seps, swed.oec2000.ceps);
                swi_coortrf2(g+3, g+3, swed.oec2000.seps, swed.oec2000.ceps);
              }
            if (!us.fNoNutation&&!us.fSidereal&& !fUseJ2000 &&!fUseICRS)
              {
                swi_coortrf2(g, g, swed.nut.snut, swed.nut.cnut);
                swi_coortrf2(g+3, g+3, swed.nut.snut, swed.nut.cnut);
              }
          }
        spacex[ind]=g[0];
        spacey[ind]=g[1];
        spacez[ind]=g[2];
        spacedx[ind]=g[3];
        spacedy[ind]=g[4];
        spacedz[ind]=g[5];
        swi_cartpol_sp(g,g);
        planet[ind]=Mod((us.fSidereal ? is.rSid : 0) +rDegRad*g[0]);
        planetalt[ind]=rDegRad*g[1];
        planetdis[ind]=g[2];
        ret[ind]=g[3];
        altret[i]=g[4];
        disret[i]=g[5];
        ind++;
      }
  fclose(file);
  filename="centaurs.fle";
  file=FileOpen(filename,3);
  if (file == NULL)
    {
      sprintf(sz, "File '%s' not found.", filename);
      PrintError(sz);
    }
  dd=(jde-jdestart)/stepsize;
  donelighttime=fFalse;
lightrecalcC:
  p=RFract(dd);
  for (k=0; k<cRiyaC; k++)
    if (!FIgnore(k+cRiyaT+RiyaLo))
      /* if (k!=22&&k!=23&&k!=29&&k!=30&&k!=6&&k!=7&&k!=9&&!FBetween(k,11,13)) */
      {
        ind=k+RiyaLo+cRiyaT;
        i=fseek(file,4*(3*585*k+3*NFloor(dd)-6),0);
        for (i=0; i<=5; i++)
          {
            for (j=0; j<=2; j++)
              {
#ifdef _BIG_ENDIAN
                wo[3] =getc(file);
                wo[2] =getc(file);
                wo[1] =getc(file);
                wo[0] =getc(file);
                memcpy(&xyz[i][j],&wo[0],4);
#else
                xyz[i][j]=getw(file);
#endif
              }
          }
        for (i=0; i<=2; i++)
          {
            for (j=0; j<=5; j++)
              {
                d[j]=xyz[j][i];
              }
            Everett(3,p,d,&a,&b);
            g[i]=a*1e-7;
            g[i+3]=b*1e-7/200;
          }
        if (!helio)
          {
            for (i = 0; i <= 5; i++)
              {
                x[i] = -xearth[i] + g[i];
                if (!us.fTopocentric)
                  xobs[i]=0;
              }
            /* rough estimation of lighttime */
            lighttime=us.fTruePos ? 0 : (sqrt(square_sum(x))*AUNIT/CLIGHT/86400.0);
            if (!us.fTruePos && !fUseJ2000&&!donelighttime)
              {
                dd=(jde-jdestart-lighttime)/stepsize;
                donelighttime=fTrue;
                goto lightrecalcC;
              }

            if (us.fTopocentric&&!us.objCenter)
              {
                if (swi_get_observer(jd, (fUseJPL ? SEFLG_JPLEPH : 0 ) + SEFLG_NONUT+SEFLG_J2000+SEFLG_SPEED+SEFLG_XYZ+SEFLG_TRUEPOS, 0, xobs, serr) != OK)
                  {
                    sprintf(sz, "%s\n","Problem calculating observer position");
                    PrintError(sz);
                  }
              }
            for (i=0; i<=5; i++)
              g[i]-=xobs[i]+xearth[i];
          }

        swi_coortrf2(g, g, -sineps, coseps);
        swi_coortrf2(g+3, g+3, -sineps, coseps);
        if (!helio && !fUseJ2000 && !fUseICRS)
          {
            if (!us.fTruePos && !us.fNoGravityDeflection)
              swi_deflect_light(g, lighttime, SEFLG_SPEED);
            if (!us.fTruePos && !us.fNoAberration)
              swi_aberr_light(g, xearth, SEFLG_SPEED);
          }
        if (/*!us.fSidereal &&*/ !fUseJ2000 && !fUseICRS)
          {
            swi_precess(g,jde,J2000_TO_J);
            swi_precess_speed(g,jde,J2000_TO_J);
          }
        if (!us.fNoNutation&&!us.fSidereal&&!fUseJ2000 && !fUseICRS)
          {
            swi_nutation(jde,nutlo);
            swi_nutate(g,SEFLG_SPEED,FALSE);
          }
        if (!us.fEquator)
          {
            if ( !fUseJ2000 && !fUseICRS )
              {
                swi_coortrf2(g, g, swed.oec.seps, swed.oec.ceps);
                swi_coortrf2(g+3, g+3, swed.oec.seps, swed.oec.ceps);
              }
            else
              {
                swi_coortrf2(g, g, swed.oec2000.seps, swed.oec2000.ceps);
                swi_coortrf2(g+3, g+3, swed.oec2000.seps, swed.oec2000.ceps);
              }
            if (!us.fNoNutation&&!us.fSidereal&& !fUseJ2000 &&!fUseICRS)
              {
                swi_coortrf2(g, g, swed.nut.snut, swed.nut.cnut);
                swi_coortrf2(g+3, g+3, swed.nut.snut, swed.nut.cnut);
              }
          }
        spacex[ind]=g[0];
        spacey[ind]=g[1];
        spacez[ind]=g[2];
        swi_cartpol_sp(g,g);
        g[0]*=rDegRad;
        g[1]*=rDegRad;
        planet[ind]=Mod((us.fSidereal ? is.rSid : 0) +g[0]);
        planetalt[ind]=g[1];
        planetdis[ind]=g[2];
        ret[ind]=g[3];
        altret[ind]=g[4];
        disret[ind]=g[5];
        ind++;
      }
  fclose(file);

  filename="asteroid.fle";
  file=FileOpen(filename,3);
  if (file == NULL)
    {
      sprintf(sz, "File '%s' not found.", filename);
      PrintError(sz);
    }
  stepsize=80.0;
  dd=(jde-jdestart)/stepsize;
  if (dd<2.0||dd>1455)
    goto longrange;
  donelighttime=fFalse;
lightrecalcA:
  p=RFract(dd);
  for (k=0; k<cRiyaA; k++)
    if (!FIgnore(k+cRiyaT+cRiyaC+RiyaLo))
      /* if (k>11&&k!=13&&k!=14&&k!=18) */
      {
        ind=k+RiyaLo+cRiyaT+cRiyaC;
        i=fseek(file,4*(3*1461*k+3*NFloor(dd)-6),0);
        for (i=0; i<=5; i++)
          {
            for (j=0; j<=2; j++)
              {
#ifdef _BIG_ENDIAN
                wo[3] =getc(file);
                wo[2] =getc(file);
                wo[1] =getc(file);
                wo[0] =getc(file);
                memcpy(&xyz[i][j],&wo[0],4);
#else
                xyz[i][j]=getw(file);
#endif
              }
          }
        for (i=0; i<=2; i++)
          {
            for (j=0; j<=5; j++)
              {
                d[j]=xyz[j][i];
              }
            Everett(3,p,d,&a,&b);
            g[i]=a*1e-7;
            g[i+3]=b*1e-7/80;
            if (k==26 /*|| FBetween(k,19,21) */)
              {
                g[i]*=0.1;
                g[i+3]*=0.1;
              }

          }
        if (!helio)
          {
            for (i = 0; i <= 5; i++)
              {
                x[i] = -xearth[i] + g[i];
                if (!us.fTopocentric)
                  xobs[i]=0;
              }
            /* rough estimation of lighttime */
            lighttime=us.fTruePos ? 0 : (sqrt(square_sum(x))*AUNIT/CLIGHT/86400.0);
            if (!us.fTruePos && !fUseJ2000&&!donelighttime)
              {
                dd=(jde-jdestart-lighttime)/stepsize;
                donelighttime=fTrue;
                goto lightrecalcA;
              }

            if (us.fTopocentric&&!us.objCenter)
              {
                if (swi_get_observer(jd, (fUseJPL ? SEFLG_JPLEPH : 0 ) + SEFLG_NONUT+SEFLG_J2000+SEFLG_SPEED+SEFLG_XYZ+SEFLG_TRUEPOS, 0, xobs, serr) != OK)
                  {
                    sprintf(sz, "%s\n","Problem calculating observer position");
                    PrintError(sz);
                  }
                for (i=0; i<=5; i++)
                  g[i]-=xobs[i]+xearth[i];
              }
              }
            swi_coortrf2(g, g, -sineps, coseps);
            swi_coortrf2(g+3, g+3, -sineps, coseps);
            if (!helio && !fUseJ2000 && fUseICRS)
              {
                if (!us.fTruePos && !us.fNoGravityDeflection)
                  swi_deflect_light(g, lighttime, SEFLG_SPEED);
                if (!us.fTruePos && !us.fNoAberration)
                  swi_aberr_light(g, xearth, SEFLG_SPEED);
              }

            if (/*!us.fSidereal &&*/ !fUseJ2000 && !fUseICRS )
              {
                swi_precess(g,jde,J2000_TO_J);
                swi_precess_speed(g,jde,J2000_TO_J);
              }
            if (!us.fNoNutation&&!us.fSidereal&&!fUseJ2000 && !fUseICRS)
              {
                swi_nutation(jde,nutlo);
                swi_nutate(g,SEFLG_SPEED,FALSE);
              }
            if (!us.fEquator)
              {
                if ( !fUseJ2000 && !fUseICRS )
                  {
                    swi_coortrf2(g, g, swed.oec.seps, swed.oec.ceps);
                    swi_coortrf2(g+3, g+3, swed.oec.seps, swed.oec.ceps);
                  }
                else
                  {
                    swi_coortrf2(g, g, swed.oec2000.seps, swed.oec2000.ceps);
                    swi_coortrf2(g+3, g+3, swed.oec2000.seps, swed.oec2000.ceps);
                  }
                if (!us.fNoNutation&&!us.fSidereal&& !fUseJ2000 &&!fUseICRS)
                  {
                    swi_coortrf2(g, g, swed.nut.snut, swed.nut.cnut);
                    swi_coortrf2(g+3, g+3, swed.nut.snut, swed.nut.cnut);
                  }
              }
            spacex[ind]=g[0];
            spacey[ind]=g[1];
            spacez[ind]=g[2];
            swi_cartpol_sp(g,g);
            planet[ind]=Mod((us.fSidereal ? is.rSid : 0) +rDegRad*g[0]);
            planetalt[ind]=rDegRad*g[1];
            planetdis[ind]=g[2];
            ret[ind]=g[3];
            altret[ind]=g[4];
            disret[ind]=g[5];
            ind++;
          }
        goto verylong;
longrange:
        fclose(file);
        jdestart=1940507.5; /* Oct 27,600 */
        stepsize=200;
        filename="long.fle";
        file=FileOpen(filename,3);
        if (file == NULL)
          {
            sprintf(sz, "File '%s' not found.", filename);
            PrintError(sz);
          }
        dd=(jde-jdestart)/stepsize; /* Oct 27,600 */
        donelighttime=fFalse;
lightrecalcL:
        if (dd<2.0||dd>2587)
          goto verylong;
        p=RFract(dd);
        for (k=0; k<=17; k++) if (k!=4 && k!=12 && k!=13)
            {
              i=fseek(file,4*(3*2592*k+3*NFloor(dd)-6),0);
              for (i=0; i<=5; i++)
                {
                  for (j=0; j<=2; j++)
                    {
#ifdef _BIG_ENDIAN
                      wo[3] =getc(file);
                      wo[2] =getc(file);
                      wo[1] =getc(file);
                      wo[0] =getc(file);
                      memcpy(&xyz[i][j],&wo[0],4);
#else
                      xyz[i][j]=getw(file);
#endif
                    }
                }
              for (i=0; i<=2; i++)
                {
                  for (j=0; j<=5; j++)
                    {
                      d[j]=xyz[j][i];
                    }
                  Everett(3,p,d,&a,&b);
                  g[i]=a*1e-7;
                  g[i+3]=b*1e-7/200;
                }
              if (!helio)
                {
                  for (i = 0; i <= 5; i++)
                    {
                      x[i] = -xearth[i] + g[i];
                      if (!us.fTopocentric)
                        xobs[i]=0;
                    }
                  /* rough estimation of lighttime */
                  lighttime=us.fTruePos ? 0 : (sqrt(square_sum(x))*AUNIT/CLIGHT/86400.0);
                  if (!us.fTruePos && !fUseJ2000&&!donelighttime)
                    {
                      dd=(jde-jdestart-lighttime)/stepsize;
                      donelighttime=fTrue;
                      goto lightrecalcL;
                    }
                  if (us.fTopocentric&&!us.objCenter)
                    {
                      if (swi_get_observer(jd, (fUseJPL ? SEFLG_JPLEPH : 0 ) + SEFLG_NONUT+SEFLG_J2000+SEFLG_SPEED+SEFLG_XYZ+SEFLG_TRUEPOS, 0, xobs, serr) != OK)
                        {
                          sprintf(sz, "%s\n","Problem calculating observer position");
                          PrintError(sz);
                        }
                    }

                  for (i=0; i<=5; i++)
                    g[i]-=xobs[i]+xearth[i];
                }
              swi_coortrf2(g, g, -sineps, coseps);
              swi_coortrf2(g+3, g+3, -sineps, coseps);
              if (!helio && !fUseJ2000 && !fUseICRS)
                {
                  if (!us.fTruePos && !us.fNoGravityDeflection)
                    swi_deflect_light(g, lighttime, SEFLG_SPEED);
                  if (!us.fTruePos && !us.fNoAberration)
                    swi_aberr_light(g, xearth, SEFLG_SPEED);
                }
              if (!fUseJ2000 && !fUseICRS )
                {
                  swi_precess(g,jde,J2000_TO_J);
                  swi_precess_speed(g,jde,J2000_TO_J);
                }
              if (!us.fNoNutation&&!us.fSidereal && !fUseJ2000 && !fUseICRS)
                {
                  swi_nutation(jde,nutlo);
                  swi_nutate(g,SEFLG_SPEED,FALSE);
                }
              if (!us.fEquator)
                {
                  if ( !fUseJ2000 && !fUseICRS )
                    {
                      swi_coortrf2(g, g, swed.oec.seps, swed.oec.ceps);
                      swi_coortrf2(g+3, g+3, swed.oec.seps, swed.oec.ceps);
                    }
                  else
                    {
                      swi_coortrf2(g, g, swed.oec2000.seps, swed.oec2000.ceps);
                      swi_coortrf2(g+3, g+3, swed.oec2000.seps, swed.oec2000.ceps);
                    }
                  if (!us.fNoNutation&&!us.fSidereal&& !fUseJ2000 &&!fUseICRS)
                    {
                      swi_coortrf2(g, g, swed.nut.snut, swed.nut.cnut);
                      swi_coortrf2(g+3, g+3, swed.nut.snut, swed.nut.cnut);
                    }
                }
              if (k==3  )
                ind=oNes;
              else if (k==0  )
                ind=CentHi; /* Typhon */
              else if (k==1  )
                ind=oCha;
              else if (k==2  )
                ind=oHyl;
              /* else if (k==4  )
                ind=oAsb; no good with MPCORB 200d steps */
              else if (k==5  )
                ind=RiyaLo+cRiyaT-4;
              else if (k==6  )
                ind=TrneLo+25; /* Rhadamantus */
              else if (k==7  )
                ind=TrneLo+26; /* Huya */
              else if (k==8  )
                ind=RiyaLo+cRiyaT+5;
              else if (k==9  )
                ind=CentHi-1; /*BU 48 */
              else if (k==10  )
                ind=RiyaLo+cRiyaT+8;
              else if (k==11  )
                ind=RiyaLo+cRiyaT+10;
              /*  else if (k==12  )
                  ind=CentHi-2; Thereus no good
                else if (k==13  )
                  ind=CentHi-3; Elatus no good with MPCORB 200d steps */
              else if (k==14  )
                ind=TrneLo+1; /* Varuna */
              else if (k==15  )
                ind=TrneLo; /* Ixion */
              else if (k==16  )
                ind=CentHi-4; /* TD10 */
              else if (k==17  )
                ind=CentHi-7; /* TL66 */
              else continue;
              if (planetdis[ind]<0.01)
                {
                  spacex[ind]=g[0];
                  spacey[ind]=g[1];
                  spacez[ind]=g[2];
                  swi_cartpol_sp(g,g);
                  planet[ind]=Mod((us.fSidereal ? is.rSid : 0)+rDegRad*g[0]);
                  planetalt[ind]=rDegRad*g[1];
                  planetdis[ind]=g[2];
                  ret[ind]=g[3];
                  altret[ind]=g[4];
                  disret[ind]=g[5];
                }
            }

verylong:

        fclose(file);
        for (ii=0; ii<=verylongmax+cRiyaX; ii++) if (! (ii>=verylongmax ? ignore[ii-verylongmax+RiyaHi-cRiyaX] : ignore[verylong[ii]]))
            {
              if (ii>=verylongmax)
                {
                  ind=ii-verylongmax+RiyaHi-cRiyaX;
                }
              else ind=verylong[ii];
              if (FRiyalNumbered(jd,ind,us.objCenter!= oEar))
                continue;
              else if (us.fDebug)
                {
                  sprintf(sz,"SX1 Error %d \n",ind);
                  PrintSz(sz);
                }
            }
        return;
      }


  int FRiyalNumbered(jd,ind,helio)
  int helio,ind;
  double jd;
  {
    char sz[cchSzDef];
    bool donelighttime;
    int i,j,xyz[6][3],d[6],k,iobj,intstepsize,len,prec;
    real rxyz[6][3],a,b,g[6],x[6],p=0.0,dd,maxdd,jde,xobs[6],xearth[6],nutlo[12],jdestart,stepsize,scale=0.0,lighttime=0.0;
    /*  struct save_positions *sd; */
    FILE *file;
#ifdef PC
    char *filename="tnpsdo.fle\0         ";
#else
    char *filename="tnpsdo.fle\0         ";
#endif
    char name[cchSzMax];
    char serr[AS_MAXCH];
#ifdef _BIG_ENDIAN
    byte wo[4];
#endif
    if (!FBetween(jd,-2e6,6e6))
      return 0;
    if (us.objNode && FBetween(ind,RiyaLo,RiyaHi-cRiyaX-1))
      {
        FRiyalUnnumbered(jd,helio);
        return fTrue;
      }
    init_ephe_path();
    if (us.fSidereal)
      is.rSid=-swe_get_ayanamsa(jd);
    iobj=AlToSweObj(ind);
    if (FBetween(iobj,SE_COMET_OFFSET,SE_COMET_OFFSET+MaxNumberedComet))
      {
        filename="sc";
        sprintf(sz,"%s%05d.sx1",filename,iobj-SE_COMET_OFFSET);
      }
    else if (iobj > SE_AST_OFFSET )
      {
        filename="sx";
        sprintf(sz,"%s%05d.sx1",filename,iobj-SE_AST_OFFSET);
      }
    else if (iobj==SE_WHITE_MOON)
      {
        filename="sx";
        sprintf(sz,"%s%s.sx1",filename,szObjName[ind]);
      }
    else return 0 ;
    filename=strdup(sz);
    file=FileOpen(filename,3);
    if (file == NULL)
      {
        if (us.fDebug)
          {
            sprintf(sz, "File '%s' not found.", filename);
            PrintWarning(sz);
          }
        return 0;
      }
    k=fseek(file,0,0);
    i=0;
    for (j=0; j<=2; j++)
      {
#ifdef _BIG_ENDIAN
        wo[3] =getc(file);
        wo[2] =getc(file);
        wo[1] =getc(file);
        wo[0] =getc(file);
        memcpy(&xyz[i][j],&wo[0],4);
#else
        xyz[i][j]=getw(file);
#endif
      }
    k=fseek(file,0,SEEK_END);
    i=ftell(file);

    /* chose default for backward compat of old .sx1 files */
    jdestart=(double)xyz[0][0]+ 0.5;
    /*if (RAbs(RFract((jdestart-0.5)))> .001)*/
    intstepsize=xyz[0][1];
    if (!FBetween(intstepsize,1,1000))
      intstepsize=200;
    prec=xyz[0][2];
    scale=(double)xyz[0][2];
    if (RAbs(RFract(scale/1000))> .001)
      {
        scale=1e7;
        jdestart=452400.5;
      }
    if (intstepsize==73||intstepsize==183||intstepsize==146||intstepsize==37||intstepsize==365)
      stepsize= 36.525* (double)(intstepsize/36);
    else
      stepsize = (double)intstepsize;
    if (prec)
      len = sizeof (int);
    else
      len = sizeof (real);
    maxdd=(i-3*sizeof(int))/(3*len)-2;

    if (!FBetween(ind,RiyaLo,RiyaHi))
      {
        if (FBetween(iobj,SE_COMET_OFFSET,SE_COMET_OFFSET+MaxComet))
          szObjName[ind]=cometName[iobj-SE_COMET_OFFSET];
        else if (ind > RiyaLo )
          {
            sx_get_planet_name(iobj,name);
            if (!us.fDiscoveryYear &&(name[0]=='2'||name[0]=='1'))
              szObjName[ind]=strdup(name+5);
            else if (!us.fDiscoveryYear &&(name[0]=='?'))
              szObjName[ind]=strdup(name+7);
            else if (us.fDiscoveryYear &&(name[0]=='?'))
              szObjName[ind]=strdup(name+2);
            else
              szObjName[ind]=strdup(name);
          }
      }
    if (!fdeltaT)
      deltaT=swe_deltat(jd);
    jde = jd + deltaT;
    dd=(jde-jdestart)/stepsize+1;
    donelighttime=fFalse;
lighttimerecalc:
    if (us.fDebug > 1)
      {
        sprintf(sz,"head %d %d %d %f %f %f %f %f \n",xyz[0][0],xyz[0][1],xyz[0][2],dd,maxdd,jdestart,stepsize,scale);
        PrintSz(sz);
      }
    if (dd<1.0||dd>maxdd+2)
      goto finish2;
    p=RFract(dd);
    if (FBetween(dd,3.0,maxdd))
      {
        i=fseek(file,12+len*(3*NFloor(dd-3)),0);
        if (prec)
          {
            for (i=0; i<=5; i++)
              {
                for (j=0; j<=2; j++)
                  {
#ifdef _BIG_ENDIAN
                    wo[3] =getc(file);
                    wo[2] =getc(file);
                    wo[1] =getc(file);
                    wo[0] =getc(file);
                    memcpy(&xyz[i][j],&wo[0],4);
#else
                    xyz[i][j]=getw(file);
#endif
                  }
              }
          }
        else  fread((void *) &rxyz[0][0],sizeof(real),18,file);
      }
    else
      {
        if (dd < 3.0) /* dd 1-3 */
          i=fseek(file,12+len*(3*NFloor(dd-1)),0);
        if (dd > maxdd) /* dd end */
          i=fseek(file,12+len*(3*NFloor(dd-3)),0);
        if (prec)
          {
            for (i=2; i<=3; i++)
              {
                for (j=0; j<=2; j++)
                  {
#ifdef _BIG_ENDIAN
                    wo[3] =getc(file);
                    wo[2] =getc(file);
                    wo[1] =getc(file);
                    wo[0] =getc(file);
                    memcpy(&xyz[i][j],&wo[0],4);
#else
                    xyz[i][j]=getw(file);
#endif
                  }
              }
            for (j=0; j<=2; j++)
              {
                a=rxyz[3][j]-rxyz[2][j];
                if (dd < 3.0)
                  {
                    rxyz[1][j]=rxyz[2][j]-a;
                    rxyz[0][j]=rxyz[2][j]-2.0*a;
                  }
                else
                  {
                    rxyz[4][j]=rxyz[3][j]+a;
                    rxyz[5][j]=rxyz[3][j]+2.0*a;
                  }
              }
          }
        else
          {
            if (dd < 3.0)
              fread((void *) &rxyz[2][0],sizeof(real),12,file);
            else
              fread((void *) &rxyz[0][0],sizeof(real),12,file);
            for (j=0; j<=2; j++)
              {
                a=rxyz[3][j]-rxyz[2][j];
                if (dd < 3.0)
                  {
                    rxyz[1][j]=rxyz[2][j]-a;
                    rxyz[1][j]=rxyz[2][j]-2*a;
                  }
                else
                  {
                    rxyz[4][j]=rxyz[3][j]+a;
                    rxyz[5][j]=rxyz[3][j]+2.0*a;
                  }
              }
          }
      }
    if (us.fDebug > 3 )
      {
        sprintf(sz,"dd p %f %f %f \n",dd,p,rxyz[3][0]);
        PrintSz(sz);
        sprintf(sz,"dd p %f %f %f \n",dd,p,rxyz[4][0]);
        PrintSz(sz);
        sprintf(sz,"dd p %f %f %f \n",dd,p,rxyz[5][0]);
        PrintSz(sz);
      }
    if (prec)
      {
        for (i=0; i<=2; i++)
          {
            for (j=0; j<=5; j++)
              {
                d[j]=xyz[j][i];
              }
            Everett(3,p,d,&a,&b);
            g[i]=a/scale;
            g[i+3]=b/scale/stepsize;
          }
      }
    else
      {
        for (i=0; i<=2; i++)
          {
            for (j=0; j<=5; j++)
              {
                x[j]=rxyz[j][i];
                if (us.fDebug > 1 && !prec )
                  {
                    sprintf(sz," %f ",x[j]);
                    PrintSz(sz);
                  }
              }
            if (us.fDebug > 1 && !prec )
              {
                PrintL();
              }
            REverett(3,p,x,&a,&b);
            g[i]=a;
            g[i+3]=b/stepsize;
          }
      }

    if (us.fDebug > 3)
      {
        sprintf(sz,"\nread %f %f %f \n",g[0],g[1],g[2]);
        PrintSz(sz);
        sprintf(sz,"read hel ecl J2000 %f %f %f \n",g[0],g[1],g[2]);
        PrintSz(sz);
      }
    if (!helio)
      {
        swe_calc(jde,SE_EARTH,(fUseJPL ? SEFLG_JPLEPH : 0) + SEFLG_NONUT+SEFLG_J2000+SEFLG_SPEED+SEFLG_XYZ+SEFLG_HELCTR+SEFLG_TRUEPOS+SEFLG_NOABERR+SEFLG_NOGDEFL+SEFLG_NONUT ,xearth,serr);
        for (i = 0; i <= 5; i++)
          {
            x[i] = -xearth[i] + g[i];
            xobs[i]=0;
          }
        /* rough estimation of lighttime */
        lighttime=us.fTruePos ? 0 : (sqrt(square_sum(x))*AUNIT/CLIGHT/86400.0);
        if (!us.fTruePos && !donelighttime)
          {
            dd=(jde-jdestart-lighttime)/stepsize+1;
            donelighttime=fTrue;
            goto lighttimerecalc;
          }
        /*    position of observer */
        if (us.fTopocentric&&!us.objCenter)
          {
            if (swi_get_observer(jd, (fUseJPL ? SEFLG_JPLEPH : 0 ) + SEFLG_NONUT+SEFLG_J2000+SEFLG_SPEED+SEFLG_XYZ+SEFLG_TRUEPOS, 0, xobs, serr) != OK)
              {
                sprintf(sz, "%s\n","Problem calculating observer position");
                PrintError(sz);
              }
          }
      }
    for (i=0; i<=5; i++)
      if (!helio)
        g[i]-=xobs[i]+xearth[i];
    if (us.fDebug > 1)
      {
        sprintf(sz,"read obs %.12f %.12f %.12f \n",xobs[0],xobs[1],xobs[2]);
        PrintSz(sz);
        sprintf(sz,"read earth %.12f %.12f %.12f \n",xearth[0],xearth[1],xearth[2]);
        PrintSz(sz);
        sprintf(sz,"read geo ecl J2000 %f %f %f \n",g[0],g[1],g[2]);
        PrintSz(sz);
      }
    swi_coortrf2(g, g, -swed.oec2000.seps,swed.oec2000.ceps);
    swi_coortrf2(g+3, g+3, -swed.oec2000.seps,swed.oec2000.ceps);
    if (us.fDebug > 1)
      {
        sprintf(sz,"read equ geo J2000 %f %f %f \n",g[0],g[1],g[2]);
        PrintSz(sz);
      }
    if ( !helio && !fUseJ2000 && !fUseICRS )
      {
        if (!us.fTruePos && !us.fNoGravityDeflection)
          swi_deflect_light(g, lighttime, SEFLG_SPEED);
        if (!us.fTruePos && !us.fNoAberration)
          swi_aberr_light(g, xearth, SEFLG_SPEED);
      }
    if ( !fUseJ2000 && !fUseICRS)
      {
        swi_precess(g,jde,J2000_TO_J);
        swi_precess_speed(g,jde,J2000_TO_J);
      }
    if (us.fDebug > 1)
      {
        sprintf(sz,"read equ geo mean equinox %f %f %f \n",g[0],g[1],g[2]);
        PrintSz(sz);
      }
    if ((!us.fNoNutation&&!us.fSidereal && !fUseJ2000 &&!fUseICRS))
      {
        swi_nutation(jde,nutlo);
        swi_nutate(g,SEFLG_XYZ+SEFLG_SPEED+SEFLG_EQUATORIAL+SEFLG_TRUEPOS,FALSE);
      }
    if (us.fDebug > 1)
      {
        sprintf(sz,"read geo equ true equinox %f %f %f \n",g[0],g[1],g[2]);
        PrintSz(sz);
      }

    if (!us.fEquator)
      {
        if ( !fUseJ2000 && !fUseICRS )
          {
            swi_coortrf2(g, g, swed.oec.seps, swed.oec.ceps);
            swi_coortrf2(g+3, g+3, swed.oec.seps, swed.oec.ceps);
          }
        else
          {
            swi_coortrf2(g, g, swed.oec2000.seps, swed.oec2000.ceps);
            swi_coortrf2(g+3, g+3, swed.oec2000.seps, swed.oec2000.ceps);
          }
        if (!us.fNoNutation&&!us.fSidereal&& !fUseJ2000 &&!fUseICRS)
          {
            swi_coortrf2(g, g, swed.nut.snut, swed.nut.cnut);
            swi_coortrf2(g+3, g+3, swed.nut.snut, swed.nut.cnut);
          }
        if (us.fDebug > 1)
          {
            sprintf(sz,"final geo ecl %f %f %f \n",g[0],g[1],g[2]);
            PrintSz(sz);
          }
      }

    spacex[ind]=g[0];
    spacey[ind]=g[1];
    spacez[ind]=g[2];
    spacedx[ind]=g[3];
    spacedy[ind]=g[4];
    spacedz[ind]=g[5];
    swi_cartpol_sp(g,g);
    planet[ind]=Mod((us.fSidereal ? is.rSid : 0)+rDegRad*g[0]);
    planetalt[ind]=rDegRad*g[1];
    planetdis[ind]=g[2];
    ret[ind]=g[3];
    altret[ind]=g[4];
    disret[ind]=g[5];
    fclose(file);
    return fTrue;
finish2:
    if (!fRestrictClean)
      ignore[ind]=ignore2[ind]=ignore3[ind]=ignoreS[ind]=1;
    fclose(file);
    return 0;
  }
  /* test for solex output,heliocentric xyz mean J2000, first 2 lines removed*/
#ifdef PLACALC
  void PrintSolex()
  {
    char ch=chNull,sz[cchSzDef],szLine[cchSzMax];
    int i,j,xyz[6][3],d[6],k,argc;
    real a,b,g[6],p,dd,jd,jde,xobs[6],start;
    FILE *file;
    char *filename="eris.out";
    char serr[AS_MAXCH],*argv[MAXSWITCHES];

    file=fopen(filename,BFILE_R_ACCESS);
    if (file == NULL)
      {
        sprintf(sz, "File '%s' not found.", filename);
        PrintError(sz);
      }
    jd=julday(Mon,Day,Yea,DegToDec(Tim),1);
    jde=jd+swe_deltat(jd);
    swe_calc(jde,SE_EARTH,SEFLG_SPEED+SEFLG_XYZ+SEFLG_HELCTR,xobs,serr);
    for (szLine[0] = ch, j = 1; j < cchSzMax && !feof(file) &&
         (szLine[j] = getc(file)) >= ' '; j++)
      ;
    szLine[j] = chNull;
    argc = NParseCommandLine(szLine, argv);
    start=atof(argv[2]);
    dd=(jde-start)/200;
    if (dd<2.0)
      return;
    for (k=0; k<=0; k++)
      {
        p=RFract(dd);
        sprintf(sz,"hihi %f %f \n",dd,p);
        PrintSz(sz);
        for (i=0; i<=5; i++)
          {
            j=fseek(file,63*NFloor(dd)-126+63*i,0);
            for (szLine[0] = ch, j = 1; j < cchSzMax && !feof(file) &&
                 (szLine[j] = getc(file)) >= ' '; j++)
              ;
            szLine[j] = chNull;
            argc = NParseCommandLine(szLine, argv);
            sprintf(sz,"hihi %f %f %f %f \n",atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]));
            PrintSz(sz);
            for (j=0; j<=2; j++)
              {
                xyz[i][j]=(int)10000000*atof(argv[3+j]);
              }
          }
        for (i=0; i<=2; i++)
          {
            for (j=0; j<=5; j++)
              {
                d[j]=xyz[j][i];
              }
            Everett(3,p,d,&a,&b);
            g[i]=a*1e-6;
            g[i+3]=b*1e-6/80;
          }
        sprintf(sz,"final %f %f %f \n",g[0],g[1],g[2]);
        PrintSz(sz);
        for (i=0; i<=5; i++)
          g[i]-=xobs[i];
        swi_precess(g,jd,J2000_TO_J);
        if (!us.fNoNutation&&!us.fSidereal)
          swi_nutate(g,SEFLG_SPEED,FALSE);
        swi_cartpol_sp(g,g);
        sprintf(sz,"%s %s","Eris",SzZodiac(rDegRad*g[0]));
        PrintSz(sz);
        PrintAltitude(rDegRad*g[1]);
        sprintf(sz,"%f %f \n",g[2],rDegRad*g[3]);
        PrintSz(sz);
      }
    fclose(file);
    return;
  }

  /* Expects XYZ Cartesian Ecliptic Heliocentric coordinates in Mean Equinox J2000.0 from Solex */
  /* Starting (Julian) Sat Aug 10 -3474=3475BC JD 452400.50 in 11000 steps of 200 days */
  /* Starting (Julian) Sat Dec 30 1497 JD 226820000.50 in 5000 steps of 80 days */

  void ConvertSolex(filename,filenameout,stepsize,scale,sxheader)
  char *filename,*filenameout;
  int stepsize;
  int scale;
  bool sxheader;
  {
    char ch=chNull,sz[cchSzDef],outmode[3],mode[3],szLine[cchSzMax],done=fFalse;
    int i,j,argc,l=0;
    real s,jdestart;
    FILE *file,*outfile;
    /*  char serr[AS_MAXCH],*/
    char *argv[MAXSWITCHES];
    sprintf(mode,"r%s","");
    sprintf(outmode,"w%s","b");
    file=fopen(filename,mode);
    if (file == NULL)
      {
        sprintf(sz, "File '%s' not found.", filename);
        PrintError(sz);
      }
    /*outfile=fopen(filenameout,mode);
    if (outfile != NULL && stepsize!=73 ){
        sprintf(sz, "File '%s' already exists.", filenameout);
        PrintError(sz);
    }
    fclose(outfile);*/
    outfile=fopen(filenameout,outmode);
    if (outfile == NULL)
      {
        sprintf(sz, "File '%s' cannot be opened for writing.", filenameout);
        PrintError(sz);
      }
    loop
    {
      while (!feof(file)&& (ch = getc(file)) < ' ')
        ;
      if (feof(file))
        break;
      for (szLine[0]=ch, j = 1; j < cchSzMax && !feof(file) &&
           (szLine[j] = getc(file)) >= ' '; j++)
        ;
      szLine[j] = chNull;
      /*sprintf(sz,"%s\n",szLine);PrintSz(sz);*/
      argc = NParseCommandLine(szLine, argv);
      /*   header=fFalse; */
      if (fUseICRS)
        scale=0;
      if (l==0)
        {
          jdestart=atof(argv[1]);
          sprintf(sz,"Starting at JD %f \n",jdestart);
          PrintSz(sz);
          if (sxheader)
            {
              i=NFloor(jdestart);
              putw(i,outfile);
              putw(stepsize,outfile);
              putw(scale,outfile);
              sxheader=fTrue;
            }
        }
      if (l>=0)
        {
          for (j=0; j<=2; j++)
            {
              if (!fUseICRS)
                {
                  s= scale*atof(argv[2+j]);
                  i=(int)s;
                  if (RAbs(s)>=2147483648.0 && !done)
                    {
                      sprintf(sz,"Writing %s to %s \n32bit limitation %f \n",filename,filenameout,s/scale);
                      PrintError(sz);
                      done=fTrue;
                      return;
                    }
                  putw(i,outfile);
                }
              else
                {
                  s= atof(argv[2+j]);
                  fwrite ((void *)&s, sizeof(real), 1, outfile);
                }
            }
        }
      l++;
    }
    fclose(outfile);
    fclose(file);
    sprintf(sz,"%d records written to %s",l+1,filenameout);
    PrintError(sz);
    return;
  }
#endif
#endif



