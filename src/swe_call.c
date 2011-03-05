/*
** Astrolog (Version 5.42J) File: swe_call.c
**
** Code changed by Valentin Abramov (valja@tartu.customs.ee)
**
** IMPORTANT NOTICE: The graphics database and chart display routines
** used in this program are Copyright (C) 1991-1996 by Walter D. Pullen
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
** calculation routines in the program "Swiss Ephemeris" and are
** Copyright (C) 1998 by Astrodienst AG.
** The use of that source code is subject to
** the Swiss Ephemeris Public License, available at
** http://www.astro.ch/swisseph.
** This copyright notice must not be changed or removed
** by any user of this program.
** by any user of this program.
**
** Initial programming 8/28,30, 9/10,13,16,20,23, 10/3,6,7, 11/7,10,21/1991.
** X Window graphics initially programmed 10/23-29/1991.
** PostScript graphics initially programmed 11/29-30/1992.
** Last code change made 9/22/1996.
** Modifications from version 5.41 to 5.42 are by Nicolas Scharnagl

*/

#include "astrolog.h"
#include "config.h"
#include "swephexp.h"
#include "sweph.h"
#include "swephlib.h"
#include "swehouse.h"
#ifdef PLACALC
#  define d2l(x)	swe_d2l(x)
#  define RMOON ((3476300.0 / AUNIT) / 2)
#  define RSUN    ((1391978489.9 / AUNIT)/2)
/* Given an object index and a Julian Day time, get        */
/* ecliptic longitude and latitude of the object  */
/* and its velocity and distance from the Earth or Sun. This basically    */
/* just calls the Placalc calculation function to actually do it, but as  */
/* this is the one routine called from Astrolog, this is the one routine  */
/* which has knowledge of and uses both Astrolog and Placalc definitions, */
/* and does things such as translation to Placalc indices and formats.    */

bool FPlacalcPlanet(ind, jd, helio, obj, objalt, dir, space, altdir,disdir)
int ind, helio;
double jd;
real *obj, *objalt, *dir, *space, *altdir,*disdir;

{
    static AS_BOOL sid_built = FALSE;
    int iobj;
    long flag,sflag;
    double jde, xx[6],nutlo[12],geopos[3];
    char serr[AS_MAXCH];

    init_ephe_path();
    iobj=AlToSweObj(ind);
    flag = GetFlag(helio);
    if (!fdeltaT)
        deltaT=swe_deltat(jd);
    jde = jd + deltaT;
    if (!fUseJ2000 /*&& !fUseICRS*/) {
        swe_calc(jde,SE_ECL_NUT,0,nutlo,serr);
        nut=us.fNoNutation ? 0 : nutlo[2];
        us.nEpsilon=us.fNoNutation ? nutlo[1] : nutlo[0];
    }
    else
        us.nEpsilon=RADTODEG*(swi_epsiln(J2000));
    if (us.fSidereal) {
        flag|= SEFLG_SIDEREAL;
        sflag=0;
        if (us.fSidEclDate)
            sflag|= SE_SIDBIT_ECL_T0;
        if (us.fSidSolSysPlane)
            sflag|= SE_SIDBIT_SSY_PLANE;
        sflag|= us.fSiderealMode;
        if (!sid_built&&us.fSiderealMode==255&&!us.nSidEclDate) {
            us.nSidEclDate=jde;
            /*sprintf(serr,"%d %f %d\n",sflag,us.nSidEclDate,sid_built);PrintSz(serr);*/
            sid_built=fTrue;
        } /* conflict with setastnames ???*/
        swe_set_sid_mode(sflag,us.nSidEclDate,us.rZodiacOffset);
        is.rSid=-(swe_get_ayanamsa(jde)+us.fNoNutation ? 0 : nut);
    }
    if (swe_calc(jde, iobj, flag, xx, serr) >= 0) {
        *obj    = xx[0];
        *objalt = xx[1];
        *space  = xx[2];
        *dir    = xx[3];
        *altdir = xx[4];
        *disdir = xx[5];
        if (us.fSector&&us.fSweHouse&&fGauqMode>=2&&!us.fSectorApprox) {
            geopos[0]=-oo;
            geopos[1]=aa;
            geopos[2]=NN;
                swe_gauquelin_sector(jd,iobj,NULL,flag,fGauqMode,geopos,us.atpressDef,us.attempDef,disdir,serr);
        }
        if (us.fDebug && strlen(serr))
        {
            sprintf(serr+strlen(serr),"\n SWE Error at obj %d \n",ind);
            PrintWarning(serr);
            return fFalse;
        }
        if (ind > starHi && FBetween(iobj,SE_AST_OFFSET,SE_AST_OFFSET+MaxNumberedAsteroid))
            SetSweAstName(iobj,ind);
        return fTrue;
    }
    if (us.fDebug)
    {
        sprintf(serr+strlen(serr),"\n SWE Error at obj %d \n",ind);
        PrintWarning(serr);
    }
    return fFalse;
}

/* see swepdate.c
double julday(int month, int day, int year, double hour, int gregflag)
{
  double jd;
  jd = swe_julday(year, month, day, hour, gregflag);
  return jd;
}

void revjul (double jd, int gregflag,
	     int *jmon, int *jday, int *jyear, double *jut)
{
  swe_revjul(jd, gregflag, jyear, jmon, jday, jut);
}
*/

/* Added by V. Abramov, calls calculation of fixed stars. */

bool FPlacalcStar(ind, jd, obj, objalt,sector,magnitude)
int ind;
double jd;
real *obj, *objalt,*sector,*magnitude;
{
    long flag,sflag;
    char serr[AS_MAXCH];
    char iobj[SE_MAX_STNAME * 2 + 1];
    char namebuf[SE_MAX_STNAME *2 + 1];
    double jde, xx[6],geopos[3];
    int pos;
    char *nomname;
    static AS_BOOL sid_built = FALSE;

    init_ephe_path();
    sprintf (iobj, "%i", ind);
    flag = SEFLG_SWIEPH;
    flag |= GetFlag((us.objCenter!=0));
    if (!fdeltaT)
        deltaT=swe_deltat(jd);
    jde = jd + deltaT;
    if (us.fSidereal) {
        flag|= SEFLG_SIDEREAL;
        sflag=0;
        if (us.fSidEclDate)
            sflag|= SE_SIDBIT_ECL_T0;
        if (us.fSidSolSysPlane)
            sflag|= SE_SIDBIT_SSY_PLANE;
        sflag|= us.fSiderealMode;
        if (!sid_built&&us.fSiderealMode==255&&!us.nSidEclDate) {
            us.nSidEclDate=jde;
            sid_built=fTrue;
        } /* conflict with setastnames ???*/
        /*sprintf(serr,"%d %f %d\n",sflag,us.nSidEclDate,sid_built);PrintSz(serr);*/
        swe_set_sid_mode(sflag,us.nSidEclDate,us.rZodiacOffset);
    }
      if (us.fSector&&us.fSweHouse&&!us.fSectorApprox&&(fGauqMode>=2)) {
        geopos[0]=-DecToDeg(OO);
        geopos[1]=DecToDeg(AA);
        geopos[2]=NN;
          swe_gauquelin_sector(jd,0,iobj,flag,fGauqMode,geopos,us.atpressDef,us.attempDef,sector,serr);
    }
    if (swe_fixstar(iobj, jde, flag/*|SEFLG_STARDIST*/, xx, serr) >= 0) {
        *obj    = xx[0];
        *objalt = xx[1];
        swe_fixstar_mag(iobj,magnitude,serr);
        rStarDist[ind] = 0 /*xx[3]*/;
        nomname = memccpy(namebuf, iobj, ',', SE_MAX_STNAME+1);
        pos = nomname - namebuf;
        if (pos > starMaxName)
            namebuf[starMaxName] = chNull;
        else
            namebuf[pos-1] = chNull;
#if defined PC && defined DOS
        strcpy(szObjName[ind+starLo-1], namebuf);
#else
        szObjName[ind+starLo-1] = strdup(namebuf);
#endif
        strcpy(namebuf, iobj);
        namebuf[pos+starMaxNName] = chNull;
#if defined PC && defined DOS
        strcpy(szStarConName[ind], nomname);
#else
        szStarConName[ind] = strdup(nomname);
#endif
        if (szObjName[ind+starLo-1][0] == chNull)
           szObjName[ind+starLo-1] = strdup(nomname);

        return fTrue;
    }
    if (us.fSeconds) {
        PrintWarning(serr);
        ErrorEphem(SE_STARFILE, ind);
    }
    return fFalse;
}

/* Added by N.Scharnagl , calculates swiss ephemeris houses */

bool FPlacalcHouses(jd,mode)
double jd;
bool mode;
{
    double cusps[37],ascmc[10],armc,eps,nutlo[12],jde,xpin[2];
    int system,i;
    long flag,sflag;
    char serr[AS_MAXCH];
    static AS_BOOL sid_built = FALSE;
    if (us.nHouseSystem == 0) /* Placidus */
        system=80;
    else if (us.nHouseSystem == 8) /* Polich/Page topocentric */
        system=84;
    else if (us.nHouseSystem == 1) /* Koch */
        system=75;
    else if (us.nHouseSystem == 3) /* Campanus */
        system=67;
    else if (us.nHouseSystem == 5) /* Regiomontanus */
        system=82;
    else if (us.nHouseSystem == 6) /* Porphyrius */
        system=79;
    else if (us.nHouseSystem == 7) /* Morinus */
        system=77;
    else if (us.nHouseSystem == 9) /* Alcabitus */
        system=66;
    else if (us.nHouseSystem == 2) /* Equal */
        system=65;
    else if (us.nHouseSystem == 15) /* Axial */
        system=88;
    else if (us.nHouseSystem == 16) /* Horizontal */
        system=72;
    else if (us.nHouseSystem == 17) /* Vehlow Equal */
        system=86;
    else if (us.nHouseSystem == 18) /* Krusinski-Pisa */
        system=85;
    else if (us.nHouseSystem == 19) /* Gauquelin Sectors */
        system=71;
    else
        return fFalse;
    if (!fdeltaT)
        deltaT=swe_deltat(jd);
    jde = jd + deltaT;
    flag=0;
    if (us.fNoNutation)
        flag |= SEFLG_NONUT;
    if (us.fSidereal) {
        flag|= SEFLG_SIDEREAL;
        sflag=0;
        if (us.fSidEclDate)
            sflag|= SE_SIDBIT_ECL_T0;
        if (us.fSidSolSysPlane)
            sflag|= SE_SIDBIT_SSY_PLANE;
        sflag|= us.fSiderealMode;
        if (!sid_built&&us.fSiderealMode==255&&!us.nSidEclDate) {
            us.nSidEclDate=jde;
            sid_built=fTrue;
        } /* conflict with setastnames ???*/
        /*sprintf(serr,"%d %f %d\n",sflag,us.nSidEclDate,sid_built);PrintSz(serr);*/
        swe_set_sid_mode(sflag,us.nSidEclDate,us.rZodiacOffset);
    }
    if (mode) { /* called after the positions are calculated */
        swe_calc(jde,SE_ECL_NUT,0,nutlo,serr);
        eps = us.fNoNutation ? nutlo[1] : nutlo[0];
        armc = planet[oArmc];
        if (us.fDebug > 6) {
            sprintf(serr,"ARMC is %f",armc);
            PrintSz(serr);
        }
        if (fGauqMode < 2)
          for (i = 1; i <= objMax; i++)
            if (!FIgnore(i)) {
                xpin[0]=planet[i]+(((i==us.nEclObj) && fSynodicReturnChart)|| (us.fDirect && (us.fTransit || us.fTransitInf))  ? rAddAngle : 0); /* Promissor aspects */
                xpin[1]=(fGauqMode == 1) ? 0 : planetalt[i];
                if (us.nHouseSystem==19)
                    disret[i]=0+1*(0+swe_house_pos(armc,DecToDeg(AA),eps,system,xpin,serr));
                else disret[i]=1+3*(13-swe_house_pos(armc,DecToDeg(AA),eps,system,xpin,serr));
            }
        return fTrue;
    }
       /* called with mode = fFalse  */
    else { if (us.fProgress && !us.fDirect) {
        eps = swi_epsiln(jde) * RADTODEG;
        swi_nutation(jde, nutlo);
        for (i = 0; i < 2; i++)
            nutlo[i] *= RADTODEG;
        armc = swe_degnorm(swe_sidtime0(jd, eps + (us.fNoNutation ? 0 : nutlo[1]), (us.fNoNutation ? 0: nutlo[0])) * 15 +
                           (-oo)+TT*15.0-360.0*RFract(jd-rRound));
        i=swe_houses_armc(armc,DecToDeg(AA),eps+nutlo[1],system,cusps,ascmc);
        if (us.fSidereal) {
            for (i = 1; i <= 12; i++)
                cusps[i] = swe_degnorm(cusps[i] + is.rSid - nutlo[0]);
            for (i = 0; i < 10; i++)
                ascmc[i] = swe_degnorm(ascmc[i] + is.rSid - nutlo[0]);
        }
    }
    else  if (0&& us.fProgress && us.fDirect && us.nRel) {
        swe_calc(jde,SE_ECL_NUT,0,nutlo,serr);
        eps = us.fNoNutation ? nutlo[1] : nutlo[0];
        if (planet[oArmc]==0)
           armc = Midpoint(cp1.obj[oArmc],cp2.obj[oArmc]);
        if (us.fDebug > 4) {
            sprintf(serr,"ARMC is %f, Latitude is %f",armc,DecToDeg(AA));
            PrintSz(serr);
        }
        i=swe_houses_armc(armc,DecToDeg(AA),eps+nutlo[1],system,cusps,ascmc);
        if (us.fSidereal) {
            for (i = 1; i <= 12; i++)
                cusps[i] = swe_degnorm(cusps[i] + is.rSid - nutlo[0]);
            for (i = 0; i < 10; i++)
                ascmc[i] = swe_degnorm(ascmc[i] + is.rSid - nutlo[0]);
        }
    }
    else if (us.fGeodetic) {
        eps = swi_epsiln(jde) * RADTODEG;
        swi_nutation(jde, nutlo);
        for (i = 0; i < 2; i++)
            nutlo[i] *= RADTODEG;
        armc = swe_degnorm(-oo);
        i=swe_houses_armc(armc,DecToDeg(AA),eps+nutlo[1],system,cusps,ascmc);
        if (us.fSidereal) {
            for (i = 1; i <= 12; i++)
                cusps[i] = swe_degnorm(cusps[i] + is.rSid - nutlo[0]);
            for (i = 0; i < 10; i++)
                ascmc[i] = swe_degnorm(ascmc[i] + is.rSid - nutlo[0]);
        }
    }
    else
        i=swe_houses_ex(jd,flag,aa,-oo,system,cusps,ascmc);
    if (i>= 0) {
        for (i=cuspLo; i<= cuspHi; i++) {
            if (us.nHouseSystem==19)
            {
                planet[i] = cusps[(40-3*(i-cuspLo+1))%36];
                chouse[i-cuspLo+1] = cusps[(40-3*(i-cuspLo+1))%36];
            }
            else {
                planet[i] = cusps[i-cuspLo+1];
                chouse[i-cuspLo+1] = cusps[i-cuspLo+1];
            }
            planetdis[i]=altret[i]=0;
            ret[i]=365.25;
            if (us.fSector&&fGauqMode >=2&&!us.fSectorApprox)
                disret[i]= (real)3*(cuspLo-i)+1;
        }
        if (us.nHouseSystem==7 || us.nHouseSystem==16)
            {
        planet[oAsc] = ascmc[SE_ASC];
        planet[oMC] = ascmc[SE_MC];
        planet[oDes] = Mod(rDegHalf+ascmc[SE_ASC]);
        planet[oNad] = Mod(rDegHalf+ascmc[SE_MC]);
            }
            
        planet[oArmc] = ascmc[SE_ARMC];
        if (!us.objAst&&!us.objNode)
           planet[oVtx] = ascmc[SE_VERTEX];
        planet[oEP] = ascmc[SE_EQUASC];
        planet[oCoasc] = ascmc[SE_COASC2];
        planet[oPolasc] = ascmc[SE_POLASC];
        planetalt[oCoasc] = planetalt[oPolasc] = planetalt[oArmc] = 0;
        altret[oCoasc] = altret[oPolasc] = altret[oArmc] =planetdis[oCoasc] =
                                               planetdis[oPolasc] = planetdis[oArmc] =0;
        ret[oCoasc] = ret[oPolasc] = ret[oArmc] = 365.25;
        return fTrue;
    }
    return fFalse;
}
}

double FPlacalcRefrac(i,alt,flag)
int i,flag;
double alt;
{
    double pos,radius;

    if (us.objCenter!=oEar)
        return 0;
    if (i<=oMoo) {
        radius=asind((i==oSun ? RSUN : RMOON)/planetdis[i]);
        if (i<=oMoo && flag==SE_TRUE_TO_APP+2)
            pos=swe_refrac(radius+alt,us.atpressDef,us.attempDef,SE_TRUE_TO_APP);
        else if (flag==SE_APP_TO_TRUE+2 && i<=oMoo)
            pos=-radius+swe_refrac(alt,us.atpressDef,us.attempDef,SE_APP_TO_TRUE);
        else
            pos=swe_refrac(alt,us.atpressDef,us.attempDef,flag);
    }
    else
        pos=swe_refrac(alt,us.atpressDef,us.attempDef,flag);
    return pos;
}

real FPlacalcEclipseWhenList(nobj,jd,ifltype,backward)
int *nobj;
double jd;
long ifltype;
bool backward;
{
    int j,reti=0,obj,nextobject,counter=0,objleft;
    double occjd[cObjOpt+1];
    int occrank[cObjOpt+1];
    double xx[6];
    double tret[10],attr[20],geopos[9],nextone=0.0,jdt=0.0,jd0=0.0;
    char serr[AS_MAXCH],sz[cchSzDef],starname[2*SE_MAX_STNAME+1];

    init_ephe_path();
    backward=(bool)fEclipseBackward;
    geopos[0]=-DecToDeg(OO);
    geopos[1]=DecToDeg(AA);
    geopos[2]=NN;
    objleft=*nobj;
    jdt=jd+ (backward ? 5 : -5 );
    jd0=jd;
    for (j=0; j<=cObjOpt; j++)
    {
        occjd[j]=  -200000000.0;
    }
    nextone=jdt+ (backward ? -29.01 : 29.01);
    for (j=oMer; j<=cObjOpt; j++) {
        if (FIgnore(j)|| FBetween(j,LunarLo,TerranHi) ||FBetween(j,HypuranLo,HypuranHi)
#ifdef ASTEROIDS
                ||FBetween(j,RiyaLo,RiyaHi)
#endif
           )
            continue;
        if (us.fDebug>3) {
            sprintf(sz,"%d\n",j);
            PrintSz(sz);
        }
        if FNorm(j) {
            if (!fEclipseDetails) {
                reti=swe_lun_occult_when_glob(jdt,AlToSweObj(j),NULL,SEFLG_SWIEPH|SE_ECL_ONE_TRY,0,tret,backward,serr);
                /*    if (tret[0])
                      swe_lun_occult_where(tret[0],AlToSweObj(j),NULL,SEFLG_SWIEPH|SE_ECL_ONE_TRY,geopos,attr,serr); */
            } else {
                reti = swe_lun_occult_when_loc(jdt,AlToSweObj(j),NULL,SEFLG_SWIEPH|SE_ECL_ONE_TRY,geopos,tret,attr,backward,serr);
            }
        }
        else if FStar(j) {
            sprintf(starname, "%i", j-starLo+1);
            FPlacalcStar(j-starLo+1,jdt,&xx[0],&xx[1],&xx[2],&xx[3]);
            tret[0]=0;
            if (RAbs(xx[1])<6.5) {
                /*sprintf(sz,"%d %s ",counter,szObjName[j]);PrintSz(sz);*/
                if (!fEclipseDetails) {
                    reti=swe_lun_occult_when_glob(jdt,0,starname,SEFLG_SWIEPH|SE_ECL_ONE_TRY|SEFLG_XYZ,0,tret,backward,serr);
                    /*     if (tret[0])
                           swe_lun_occult_where(tret[0],0,starname,SEFLG_SWIEPH|SE_ECL_ONE_TRY,geopos,attr,serr);*/
                } else {
                    reti = swe_lun_occult_when_loc(jdt,0,starname,SEFLG_SWIEPH|SE_ECL_ONE_TRY|SEFLG_XYZ,geopos,tret,attr,backward,serr);
                }
            }
        }
        if (((!backward && (jd < tret[0])&&(tret[0] < nextone))||(backward &&(jd > tret[0]) && (tret[0]>nextone))) && reti>0  ) {
            counter++;
            occjd[j]=tret[0];
            if (us.fDebug>2) {
                sprintf(sz,"%f %s ",tret[0],szObjName[j]);
                PrintSz(sz);
            }

        }
    }
    if (us.fDebug>2) {
        sprintf(sz,"DDD %f\n",jdt);
        PrintSz(sz);
    }
    if (counter>0) {
        SortRank(occjd,occrank,cObjOpt);
        if (backward) {
            for (j=1; j<=counter && objleft > 0; j++) {
                for (nextobject=0; nextobject<=cObjOpt; nextobject++) {
                    if (occrank[nextobject]==j) {
                        obj = nextobject;
                        jdt = occjd[nextobject];
                        if ((jdt>jd0-20.01)) {
                            sprintf(sz,"%11.11s",szObjName[obj]);
                            PrintSz(sz);
                            jd=FPlacalcEclipseWhen(obj,jdt+4.912,4,backward);
                            objleft--;
                        } else if (us.fDebug>3) PrintWarning("possible problem with occultations");
                    }
                }
            }
        } else {
            for (j=counter; j>=1&&objleft >0; j--) {
                for (nextobject=0; nextobject<=cObjOpt; nextobject++) {
                    if (occrank[nextobject]==j) {
                        obj = nextobject;
                        jdt = occjd[nextobject];
                        if (!FIgnore(obj)&&jdt<jd0+20.01&&obj<=cObjOpt) {
                            sprintf(sz,"%11.11s",szObjName[obj]);
                            PrintSz(sz);
                            jd=FPlacalcEclipseWhen(obj,jdt-4.912,4,backward);
                            objleft--;
                        } else if (us.fDebug>3) PrintWarning("possible problem with occultations");
                    }
                }
            }
        }
    } /*else { jd = jd0 + (backward ? -20.0 : 20.0);}
    if (*nobj == objleft)*/
    jd = jd0 + (backward ? -20.0 : 20.0);
    *nobj = objleft;
    return jd;
}

real FPlacalcEclipseWhen(obj,jd,ifltype,backward)
long ifltype;
bool backward;
double jd;
int obj;
{
    int j,reti=0,topo;
    double tret[10],attr[20],geopos[9],xx[6];
    char serr[AS_MAXCH],sz[cchSzDef],starname[2*SE_MAX_STNAME +1];

    backward=(bool)fEclipseBackward;
    init_ephe_path();
    geopos[0]=-DecToDeg(OO);
    geopos[1]=DecToDeg(AA);
    geopos[2]=NN;
    if (obj==oMoo) {
        reti=swe_lun_eclipse_when(jd,SEFLG_SWIEPH,65535,tret,backward,serr);
        swe_set_topo(geopos[0],geopos[1],geopos[2]);
    }
    else if (obj==oSun) {
        if (!fEclipseDetails){
        reti=swe_sol_eclipse_when_glob(jd,SEFLG_SWIEPH,0,tret,backward,serr);
            swe_sol_eclipse_where(tret[0]-3,SEFLG_SWIEPH,geopos,attr,serr);
            }
        else
           reti=swe_sol_eclipse_when_loc(jd,SEFLG_SWIEPH,geopos,tret,attr,backward,serr);
        swe_set_topo(geopos[0],geopos[1],geopos[2]);
    }
    else if FNorm(obj) {
        if (!fEclipseDetails){
        reti=swe_lun_occult_when_glob(jd,AlToSweObj(obj),NULL,SEFLG_SWIEPH,0,tret,backward,serr);
            swe_lun_occult_where(tret[0]-3,AlToSweObj(obj),NULL,SEFLG_SWIEPH,geopos,attr,serr);
            }
        else reti=swe_lun_occult_when_loc(jd,AlToSweObj(obj),NULL,SEFLG_SWIEPH,geopos,tret,attr,backward,serr);
        swe_set_topo(geopos[0],geopos[1],geopos[2]);
    }

    else if FStar(obj) {
        sprintf(starname, "%i", obj-starLo+1);
        if (!fEclipseDetails){
        reti=swe_lun_occult_when_glob(jd,0,starname,SEFLG_SWIEPH,0,tret,backward,serr);
            swe_lun_occult_where(tret[0]-3,0,starname,SEFLG_SWIEPH,geopos,attr,serr);
            }
        else swe_lun_occult_when_loc(jd,0,starname,SEFLG_SWIEPH,geopos,tret,attr,backward,serr);
        swe_set_topo(geopos[0],geopos[1],geopos[2]);
    }
    revjul(tret[0],tret[0] >= 2299171.0 ? SE_GREG_CAL : SE_JUL_CAL,&Mon,&Day,&Yea,&Tim);
    topo=us.fTopocentric;
    us.fTopocentric=fTrue;
    if (FNorm(obj))
        FPlacalcPlanet(obj,tret[0],fFalse,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
    else if FStar(obj)
        FPlacalcStar(obj-starLo+1,tret[0],&xx[0],&xx[1],&xx[2],&xx[3]);
    us.fTopocentric=topo;
    if (us.fEclipse) {
        sprintf(sz,"%11.11s %s at ",
                reti & SE_ECL_TOTAL ? "Total" : reti & SE_ECL_PARTIAL ? "Partial" :
                reti & SE_ECL_CENTRAL ? "Central" : reti & SE_ECL_NONCENTRAL ? "Non-central" :
                reti & SE_ECL_ANNULAR ? "Annular" : reti & SE_ECL_ANNULAR_TOTAL ? "Annular total" :
                reti & SE_ECL_PENUMBRAL ? "Penumbral" :  serr, obj==oMoo ? "lunar Eclipse" : obj==oSun ? "solar Eclipse": "lunar occultation");
        PrintSz(sz);
        PrintZodiac(xx[0]);
        sprintf(sz,"%s %s UT ",
                SzDate(Mon, Day, Yea, 3),SzTim(DegToDec(Tim)));
        PrintSz(sz);
        if (obj!=oMoo) {
            sprintf(sz,"%s \n",SzLocation(DegToDec(-geopos[0]),DegToDec(geopos[1])));
            PrintSz(sz);
        }
        else {
            sprintf(sz,"\n");
            PrintSz(sz);
        }
        if (fEclipseDetails) {
            if (obj!=oMoo) {
                sprintf(sz,"Fraction of %s diameter covered by moon (magnitude) %f\n",obj==oSun ? "solar" : szObjName[obj] ,attr[0]);
                PrintSz(sz);
                sprintf(sz,"Fraction of %s disc covered by moon (obscuration)   %f\n",obj==oSun ? "solar" : szObjName[
                            obj],attr[2]);
                PrintSz(sz);
                sprintf(sz,"Diameter of core shadow in km                          %f\n",attr[3]);
                PrintSz(sz);
                sprintf(sz,"True altitude of %-11.11s                           %s\n",szObjName[obj] ,SzAltitude(attr[5]));
                PrintSz(sz);
                sprintf(sz,"Azimuth of %-11.11s                                 %s\n",szObjName[obj],SzDegree(attr[4]));
                PrintSz(sz);
                sprintf(sz,"Elongation of Moon                                     %s\n",SzAltitude(attr[7]));
                PrintSz(sz);
                if (obj!=oSun) {
                    for (j=1; j<=4; j++) {
                        if (tret[j])
                            revjul(tret[j],tret[j] >= 2299171.0 ? SE_GREG_CAL : SE_JUL_CAL,&Mon,&Day,&Yea,&Tim);
                        sprintf(sz,"%-14.14s %s %s UT \n", j==1 ? "1st contact" : j==2 ? "2nd Contact" : j==3 ? "3rd   Contact" : "4th contact",
                                tret[j] ? SzDate(Mon, Day, Yea, 3): "none" ,tret[j] ? SzTim(DegToDec(Tim)) : " " );
                        PrintSz(sz);
                    }
                } else {
                    for (j=1; j<=4; j++) {
                        if (tret[j])
                            revjul(tret[j],tret[j] >= 2299171.0 ? SE_GREG_CAL : SE_JUL_CAL,&Mon,&Day,&Yea,&Tim);
                        sprintf(sz,"%-14.14s %s %s UT \n", j==1 ? "Begin contact" : j==2 ? "Totality Begin" : j==3 ? "Totality   End" : "End contact",
                                tret[j] ? SzDate(Mon, Day, Yea, 3): "none" ,tret[j] ? SzTim(DegToDec(Tim)) : " " );
                        PrintSz(sz);
                    }
                }
            }
            PrintL();
        }
    }
    else {
        OO=-DegToDec(geopos[0]); /* approx */
        AA=DegToDec(geopos[1]);
    }
    if (fOccultAll)
        return (backward ? Min(jd,tret[0]) : Max(jd,tret[0]));
    else
     /*   return (backward ? Max(tret[0],tret[0]): Min(tret[0],tret[0]));*/
         return tret[0];
}

bool FPlacalcNode(ind, jd, helio, obj, objalt, dir, space, altdir,disdir)
int ind, helio;
double jd;
real *obj, *objalt, *dir, *space, *altdir, *disdir;
{
    int32 iobj,flag,sflag,mode;
    char serr[AS_MAXCH];
    static AS_BOOL sid_built = FALSE;
    real jde,xnasc[6],xndsc[6],xperi[6],xaphe[6];

    init_ephe_path();
    flag=GetFlag(helio);
    if (us.fNodeMean)
        mode=SE_NODBIT_MEAN;
    else if (us.fNodeBary)
        mode=SE_NODBIT_OSCU_BAR;
    else
        mode=SE_NODBIT_OSCU;
    if (us.fNodeFocal)
        mode+=SE_NODBIT_FOPOINT;
    if (us.fEquator&&!us.fGalPlane)
        flag |= SEFLG_EQUATORIAL;
    iobj=AlToSweObj(ind);
    if (!fdeltaT)
        deltaT=swe_deltat(jd);
    jde = jd + deltaT;
    if (us.fSidereal) {
        flag|= SEFLG_SIDEREAL;
        sflag=0;
        if (us.fSidEclDate)
            sflag|= SE_SIDBIT_ECL_T0;
        if (us.fSidSolSysPlane)
            sflag|= SE_SIDBIT_SSY_PLANE;
        sflag|= us.fSiderealMode;
        if (!sid_built&&us.fSiderealMode==255&&!us.nSidEclDate) {
            us.nSidEclDate=jde;
            sid_built=fTrue;
        } /* conflict with setastnames ???*/
        /*sprintf(serr,"%d %f %d\n",sflag,us.nSidEclDate,sid_built);PrintSz(serr);*/
        swe_set_sid_mode(sflag,us.nSidEclDate,us.rZodiacOffset);
    }
#ifdef ASTEROIDS
    if (swe_sx_nod_aps(jde, ind, flag, mode, xnasc, xndsc, xperi, xaphe, serr) >= 0) {
#else
    if (swe_nod_aps(jde, iobj, flag, mode, xnasc, xndsc, xperi, xaphe, serr) >= 0) {
#endif
        if ((us.fNodeApPeri && !us.fNodeDscPeri) || us.fNodeFocal) {
            *obj    = xaphe[0];
            *objalt = xaphe[1];
            *space  = xaphe[2];
            *dir    = xaphe[3];
            *altdir = xaphe[4];
            *disdir = xaphe[5];
            return fTrue;
        }
        else if (us.fNodeApPeri && us.fNodeDscPeri) {
            *obj    = xperi[0];
            *objalt = xperi[1];
            *space  = xperi[2];
            *dir    = xperi[3];
            *altdir = xperi[4];
            *disdir = xperi[5];
            return fTrue;
        }
        else if (!us.fNodeApPeri && us.fNodeDscPeri) {
            *obj    = xndsc[0];
            *objalt = xndsc[1];
            *space  = xndsc[2];
            *dir    = xndsc[3];
            *altdir = xndsc[4];
            *disdir = xndsc[5];
            if (iobj==SE_SUN)
                return fFalse;
            else
                return fTrue;
        }
        else {
            *obj    = xnasc[0];
            *objalt = xnasc[1];
            *space  = xnasc[2];
            *dir    = xnasc[3];
            *altdir = xnasc[4];
            *disdir = xnasc[5];
            if (iobj==SE_SUN)
                return fFalse;
            else
                return fTrue;
        }
    }
    return fFalse;
}

int32 AlToSweObj(ind)
int ind;
{
    int32 iobj;

    if (ind == oVtx && us.objNode && us.objNode!=oVtx )
        ind = us.objNode;

    if (FBetween(ind, HypuranLo, HypuranHi- 1))
        iobj = ind - HypuranLo + SE_CUPIDO;
    else if (ind == HypuranHi)
        iobj = SE_WHITE_MOON;
    else if (FBetween(ind,oSun,oPlu))
        iobj = ind-1;
    else if (ind == oEar)
        iobj = SE_EARTH;
    else if (ind == oVtx && us.objAst)
        iobj = SE_AST_OFFSET + us.objAst;
    else if (ind == oChi)
        iobj = SE_CHIRON;
    else if (FBetween(ind, oCer, oVes))
        iobj = ind - oCer + SE_CERES;
    else if (ind == oNod)
        iobj = us.fTrueNode ? SE_TRUE_NODE : SE_MEAN_NODE;
    else if (ind == oLil)
        iobj = SE_MEAN_APOG;
    else if (ind == oApo)
        iobj = SE_OSCU_APOG;
    /* else if (ind == 33)
      iobj = us.fTrueNode ? SE_MEAN_NODE : SE_TRUE_NODE; */
#ifdef ASTEROIDS
    else if (ind == CentLo) /*Centaurs*/
        iobj = SE_PHOLUS;
    else if (ind == CentLo+1)
        iobj = SE_AST_OFFSET + 10199;
    else if (ind == CentLo+2)
        iobj = SE_AST_OFFSET + 10370;
    else if (ind == CentLo+3)
        iobj = SE_AST_OFFSET + 7066;
    else if (ind == CentLo+4)
        iobj = SE_AST_OFFSET + 8405;
    else if (ind == CentLo+5)
        iobj = SE_AST_OFFSET + 15874;
    else if (ind == CentLo+6)
        iobj = SE_AST_OFFSET + 26181;
    else if (ind == CentLo+7)
        iobj = SE_AST_OFFSET + 26375;
    else if (ind == CentLo+8)
        iobj = SE_AST_OFFSET + 29981;
    else if (ind == CentLo+9)
        iobj = SE_AST_OFFSET + 31824;
    else if (ind == CentLo+10)
        iobj = SE_AST_OFFSET + 32532;
    else if (ind == CentLo+11)
        iobj = SE_AST_OFFSET + 33128;
    else if (ind == CentLo+12)
        iobj = SE_AST_OFFSET + 42355;
    else if (ind == TrneLo)  /*Transneptunians */
        iobj = SE_AST_OFFSET + 28978;
    else if (ind == TrneLo+1)
        iobj = SE_AST_OFFSET + 20000;
    else if (ind == TrneLo+2)
        iobj = SE_AST_OFFSET + 15760;
    else if (ind == TrneLo+3)
        iobj = SE_AST_OFFSET + 136199; /* Eris */
    else if (ind == TrneLo+4)
        iobj = SE_AST_OFFSET + 58534; /* logos */
    else if (ind == TrneLo+5)
        iobj = SE_AST_OFFSET + 15807;
    else if (ind == TrneLo+6)
        iobj = SE_AST_OFFSET + 15809;
    else if (ind == TrneLo+7)
        iobj = SE_AST_OFFSET + 15810;
    else if (ind == TrneLo+8)
        iobj = SE_AST_OFFSET + 15820;
    else if (ind == TrneLo+9)
        iobj = SE_AST_OFFSET + 225088;
    else if (ind == TrneLo+10)
        iobj = SE_AST_OFFSET + 15875;
    else if (ind == TrneLo+11)
        iobj = SE_AST_OFFSET + 136108;
    else if (ind == TrneLo+12)
        iobj = SE_AST_OFFSET + 136472;
    else if (ind == TrneLo+13)
        iobj = SE_AST_OFFSET + 148780;
    else if (ind == TrneLo+14)
        iobj = SE_AST_OFFSET + 19299;
    else if (ind == TrneLo+15)
        iobj = SE_AST_OFFSET + 19308;
    else if (ind == TrneLo+16)
        iobj = SE_AST_OFFSET + 19521;
    else if (ind == TrneLo+17)
        iobj = SE_AST_OFFSET + 53311;
    else if (ind == TrneLo+18)
        iobj = SE_AST_OFFSET + 20161;
    else if (ind == TrneLo+19)
        iobj = SE_AST_OFFSET + 24835;
    else if (ind == TrneLo+20)
        iobj = SE_AST_OFFSET + 84922;
    else if (ind == TrneLo+21)
        iobj = SE_AST_OFFSET + 88611;
    else if (ind == TrneLo+22)
        iobj = SE_AST_OFFSET + 66652;
    else if (ind == TrneLo+23)
        iobj = SE_AST_OFFSET + 90482;
    else if (ind == TrneLo+24)
        iobj = SE_AST_OFFSET + 90377;
    else if (ind == TrneLo+25)
        iobj = SE_AST_OFFSET + 38083;
    else if (ind == TrneLo+26)
        iobj = SE_AST_OFFSET + 38628;
    else if (ind == TrneLo+27)
        iobj = SE_AST_OFFSET + 42301;


    else if (ind == AgenLo) /* other */
        iobj = SE_AST_OFFSET + 8;
    else if (ind == AgenLo+1)
        iobj = SE_AST_OFFSET + 16;
    /*  else if (ind == AgenLo+2)
        iobj = SE_AST_OFFSET + 17;*/
    else if (ind == AgenLo+2)
        iobj = SE_AST_OFFSET + 19;
    /*  else if (ind == AgenLo+4)
        iobj = SE_AST_OFFSET + 41;*/
    else if (ind == AgenLo+3)
        iobj = SE_AST_OFFSET + 43;
    else if (ind == AgenLo+4)
        iobj = SE_AST_OFFSET + 52;
    else if (ind == AgenLo+5)
        iobj = SE_AST_OFFSET + 55;
    else if (ind == AgenLo+6)
        iobj = SE_AST_OFFSET + 75;
    else if (ind == AgenLo+7)
        iobj = SE_AST_OFFSET + 15504;
    else if (ind == AgenLo+8)
        iobj = SE_AST_OFFSET + 94;
    /*else if (ind == AgenLo+11)
      iobj = SE_AST_OFFSET + 105;*/
    else if (ind == AgenLo+9)
        iobj = SE_AST_OFFSET + 129;
    else if (ind == AgenLo+10)
        iobj = SE_AST_OFFSET + 130;
    else if (ind == AgenLo+11)
        iobj = SE_AST_OFFSET + 153;
    else if (ind == AgenLo+12)
        iobj = SE_AST_OFFSET + 227;
    else if (ind == AgenLo+13)
        iobj = SE_AST_OFFSET + 279;
    else if (ind == AgenLo+14)
        iobj = SE_AST_OFFSET + 763;
    else if (ind == AgenLo+15)
        iobj = SE_AST_OFFSET + 430;
    else if (ind == AgenLo+16)
        iobj = SE_AST_OFFSET + 944;
    else if (ind == AgenLo+17)
        iobj = SE_AST_OFFSET + 1181;
    else if (ind == AgenLo+18)
        iobj = SE_AST_OFFSET + 1388;
    else if (ind == AgenLo+19)
        iobj = SE_AST_OFFSET + 5335;
    else if (ind == AgenLo+20)
        iobj = SE_AST_OFFSET + 1809;
    else if (ind == AgenLo+21)
        iobj = SE_AST_OFFSET + 1387;
    else if (ind == AgenLo+22)
        iobj = SE_AST_OFFSET + 25384;

    else if (ind == MuseLo) /*Muses */
        iobj = SE_AST_OFFSET + 18;
    else if (ind == MuseLo+1)
        iobj = SE_AST_OFFSET + 22;
    else if (ind == MuseLo+2)
        iobj = SE_AST_OFFSET + 23;
    else if (ind == MuseLo+3)
        iobj = SE_AST_OFFSET + 27;
    else if (ind == MuseLo+4)
        iobj = SE_AST_OFFSET + 30;
    else if (ind == MuseLo+5)
        iobj = SE_AST_OFFSET + 33;
    else if (ind == MuseLo+6)
        iobj = SE_AST_OFFSET + 62;
    else if (ind == MuseLo+7)
        iobj = SE_AST_OFFSET + 81;
    else if (ind == MuseLo+8)
        iobj = SE_AST_OFFSET + 84;

    else if (ind == ApolLo) /* Apollos */
        iobj = SE_AST_OFFSET + 1566;
    else if (ind == ApolLo+1)
        iobj = SE_AST_OFFSET + 1620;
    else if (ind == ApolLo+2)
        iobj = SE_AST_OFFSET + 1685;
    else if (ind == ApolLo+3)
        iobj = SE_AST_OFFSET + 1862;
    else if (ind == ApolLo+4)
        iobj = SE_AST_OFFSET + 1863;
    else if (ind == ApolLo+5)
        iobj = SE_AST_OFFSET + 1864;
    else if (ind == ApolLo+6)
        iobj = SE_AST_OFFSET + 1865;
    else if (ind == ApolLo+7)
        iobj = SE_AST_OFFSET + 1866;
    else if (ind == ApolLo+8)
        iobj = SE_AST_OFFSET + 1981;
    else if (ind == ApolLo+9)
        iobj = SE_AST_OFFSET + 2063;
    else if (ind == ApolLo+10)
        iobj = SE_AST_OFFSET + 2101;
    else if (ind == ApolLo+11)
        iobj = SE_AST_OFFSET + 2102;
    else if (ind == ApolLo+12)
        iobj = SE_AST_OFFSET + 3200;
    else if (ind == ApolLo+13)
        iobj = SE_AST_OFFSET + 3361;
    else if (ind == ApolLo+14)
        iobj = SE_AST_OFFSET + 4450;
    else if (ind == ApolLo+15)
        iobj = SE_AST_OFFSET + 5143;

    else if (ind == AmorLo)         /*Amors */
        iobj = SE_AST_OFFSET + 433;
    else if (ind == AmorLo+1)         /*Amors */
        iobj = SE_AST_OFFSET + 1221;
    else if (ind == AmorLo+2)         /*Amors */
        iobj = SE_AST_OFFSET + 1915;
    else if (ind == AmorLo+3)
        iobj = SE_AST_OFFSET + 1980;
    else if (ind == AmorLo+4)
        iobj = SE_AST_OFFSET + 3102;
    else if (ind == AmorLo+5)
        iobj = SE_AST_OFFSET + 3552;
    else if (ind == AmorLo+6)
        iobj = SE_AST_OFFSET + 3671;

    else if (ind == AtenLo)
        iobj = SE_AST_OFFSET + 2062; /* Aten */
    else if (ind == AtenLo+1)
        iobj = SE_AST_OFFSET + 2100;
    else if (ind == AtenLo+2)
        iobj = SE_AST_OFFSET + 2340;
    else if (ind == AtenLo+3)
        iobj = SE_AST_OFFSET + 3362;
    else if (ind == AtenLo+4)
        iobj = SE_AST_OFFSET + 3554;
    else if (ind == AtenLo+5)
        iobj = SE_AST_OFFSET + 3753;
    else if (ind == AtenLo+6)
        iobj = SE_AST_OFFSET + 5381;
#endif
#ifdef ASTEROIDS
    else if (FBetween(ind,AsteLo+us.nExtraStar,AsteLo+us.nExtraStar+us.nExtraAst-1) && astnr[ind-AsteLo-us.nExtraStar]>0)
        iobj = SE_AST_OFFSET + astnr[ind-AsteLo-us.nExtraStar];
    else if (FBetween(ind,AsteLo+us.nExtraStar,AsteLo+us.nExtraStar+us.nExtraAst-1) && astnr[ind-AsteLo-us.nExtraStar]<0)
        iobj = SE_COMET_OFFSET - astnr[ind-AsteLo-us.nExtraStar];
#endif

    /* more Hypothetical
      else if (ind == xx)
        iobj = SE_WHITE_MOON;
      else if (ind == xx)
        iobj = SE_ISIS;
    */
    else
        iobj=SE_WHITE_MOON;

    return iobj;
}

void sx_get_planet_name(int ipl,char *s)
{
    int iplf=0,ipli=ipl - SE_AST_OFFSET;
    FILE *fp;
    char si[AS_MAXCH], *sp, *sp2;
    sprintf(s,"nr%d",ipli);
    if ((fp = FileOpen(SE_ASTNAMFILE, 1)) != NULL) {
        while (ipli != iplf && (sp = fgets(si, AS_MAXCH, fp)) != NULL) {
            while (*sp == ' ' || *sp == '\t'
                    || *sp == '(' || *sp == '[' || *sp == '{')
                sp++;
            if (*sp == '#' || *sp == '\r' || *sp == '\n' || *sp == '\0')
                continue;
            /* catalog number of body of current line */
            iplf = atoi(sp);
            if (ipli != iplf)
                continue;
            /* set pointer after catalog number */
            sp = strpbrk(sp, " \t");
            if (sp == NULL)
                continue; /* there is no name */
            while (*sp == ' ' || *sp == '\t')
                sp++;
            sp2 = strpbrk(sp, "#\r\n");
            if (sp2 != NULL)
                *sp2 = '\0';
            if (*sp == '\0')
                continue;
            swi_right_trim(sp);
            strcpy(s, sp);
        }
        fclose(fp);
    } else sprintf(s,"XXXX %d",ipli);
}
void SetAstName(ind)
int ind;
{
    char name[20];
    int offset=0;
    swe_get_planet_name(SE_AST_OFFSET+ind,name);
    if (!us.fDiscoveryYear &&(name[0]=='2'||name[0]=='1'))
        offset = 5;
    else if (!us.fDiscoveryYear &&(name[0]=='?'))
        offset = 7;
    else if (us.fDiscoveryYear &&(name[0]=='?'))
        offset= 2;
    else
        offset= 0;
    szObjName[oVtx]=ind ? strdup(name+offset) : "Vertex" ;
}

void SetSweAstName(iobj,ind)
int iobj,ind;
{
    char name[20];
    int offset=0;
    if (ind==SE_WHITE_MOON)
        return;
    swe_get_planet_name(iobj,name);
    if (!us.fDiscoveryYear &&(name[0]=='2'||name[0]=='1'))
        offset = 5;
    else if (!us.fDiscoveryYear &&(name[0]=='?'))
        offset = 7;
    else if (us.fDiscoveryYear &&(name[0]=='?'))
        offset= 2;
    else
        offset= 0;
    szObjName[ind]= strdup(name+offset);
}

int NextAsteroid(ind)
int ind;
{
    double r1,r2,r3,r4,r5,r6;
    int i,save;
    planet[oVtx]=0;
    r1=r3=0;
    i=ind;
    save=us.objAst;
    while ((r1==0&&r3==0)
#ifdef ASTEROIDS
            &&(us.fRiyalAst ? planet[oVtx]==0 : fTrue)
#endif
          ) {
        i++;
        if (i >= MaxNumberedAsteroid)
            i=1;
        us.objAst=i;
        FPlacalcPlanet(oVtx, 2451544.0, 0, &r1, &r2, &r3, &r4,&r5,&r6);
#ifdef ASTEROIDS
        if (us.fRiyalAst) {
            FRiyalNumbered(2451544.0,oVtx,0);
        }
#endif
    }
    us.objAst=save;
    return i;
}
void PrevAsteroid()
{
    double r1,r2,r3,r4,r5,r6;
    int save;
    r1=0;
    save=us.fRiyalAst;
    us.fRiyalAst=fFalse;
    while (r1==0 && us.objAst > 0) {
        us.objAst--;
        FPlacalcPlanet(oVtx, 2451544.0, 0, &r1, &r2, &r3, &r4, &r5,&r6);
    }
    us.fRiyalAst=save;
    return;
}
#ifdef ASTEROIDS
void SetObjNumbers(start,maxload)
int start,maxload;
{
    int i,j,k,l;
    k=0;
    j=0;
    i=l=start-1;
    while (i< MaxNumberedAsteroid) {
        l=i;
        if (us.fIgnoreGaps)
            i++;
        else
            i=NextAsteroid(l);
             if (fSkipHard){
          while (i> skip[j])
            if (j<=cHard-1)
              j++;
          if (i==skip[j]) {
             j++;continue;}} 
        us.nExtraAst++;
        astnr[us.nExtraAst-1]=i;
        k++;
        if (us.nExtraAst>AsteHi-AsteLo-us.nExtraStar||i<l||k>maxload) {
            i=MaxNumberedAsteroid;
            us.nExtraAst--;
        }
    }
}
void SetObjNames(iobj)
int iobj;
{
    char name[20];
    int i,ind,start;
    init_ephe_path();
    if (iobj==0)
        start=AsteLo+us.nExtraStar;
    else start=AsteLo+us.nExtraStar+us.nExtraAst-1;
    for (i=start; i<=AsteLo+us.nExtraStar+us.nExtraAst-1; i++) {
        ind=AlToSweObj(i);
        if (FBetween(ind,SE_COMET_OFFSET,SE_COMET_OFFSET+MaxComet))
            szObjName[i]=cometName[ind-SE_COMET_OFFSET];
        else {
            swe_get_planet_name(ind,name);
            if (!us.fDiscoveryYear &&(name[0]=='2'||name[0]=='1'))
                szObjName[i]=strdup(name+5);
            else if (!us.fDiscoveryYear &&(name[0]=='?'))
                szObjName[i]=strdup(name+7);
            else if (us.fDiscoveryYear &&(name[0]=='?'))
                szObjName[i]=strdup(name+2);
            else
                szObjName[i]=strdup(name);
        }
    }
}

#endif
long GetFlag(helio)
int helio;
{
    long flag;
    flag = helio ? SEFLG_SPEED | SEFLG_HELCTR : SEFLG_SPEED;
    if (fUseJPL)
        flag|= SEFLG_JPLEPH;
    if (fUseJ2000)
        flag|= SEFLG_J2000;
    if (us.fTruePos)
        flag |= SEFLG_TRUEPOS;
    if (us.fBarycentric)
        flag |= SEFLG_BARYCTR;
    if (us.fNoNutation)
        flag |= SEFLG_NONUT;
    if (us.fNoAberration)
        flag |= SEFLG_NOABERR;
    if (us.fNoGravityDeflection)
        flag |= SEFLG_NOGDEFL;
    if (us.fEquator&&!us.fGalPlane)
        flag |= SEFLG_EQUATORIAL;
    if (us.fTopocentric && !helio ) {
        swe_set_topo(-DecToDeg(OO),DecToDeg(AA),NN);
        flag |= SEFLG_TOPOCTR;
    }
    return flag;
}
void CoorXForm_sp(x1,x2,x3,x4,x5,x6,angle)
double *x1,*x2,*x3,*x4,*x5,*x6,angle;
{
    double x[6];
    memcpy(&x[0],x1,8);
    memcpy(&x[1],x2,8);
    memcpy(&x[2],x3,8);  /*Polar Radians Position */
    memcpy(&x[3],x4,8);
    memcpy(&x[4],x5,8);
    memcpy(&x[5],x6,8);  /* Polar Radians Speed */
    swe_cotrans_sp(x,x,angle);
    *x1=x[0];
    *x2=x[1];
    *x3=x[2];
    *x4=x[3];
    *x5=x[4];
    *x6=x[5];
    return;
}
void RecToSph_sp(x1,x2,x3,x4,x5,x6,xx,yy,zz,dx,dy,dz)
double *xx,*yy,*zz,*dx,*dy,*dz;
double x1,x2,x3,x4,x5,x6;
{
    double x[6],xxx[6];
    x[0]=x1;
    x[1]=x2;
    x[2]=x3;  /*Cart Position */
    x[3]=x4;
    x[4]=x5;
    x[5]=x6;  /* Cart Speed */
    swi_cartpol_sp(x,xxx);
    *xx=xxx[0];
    *yy=xxx[1];
    *zz=xxx[2];  /*Polar Radians Position */
    *dx=xxx[3];
    *dy=xxx[4];
    *dz=xxx[5];  /* Polar Radians Speed */
    return;
}
void SphToRec_sp(x1,x2,x3,x4,x5,x6,xx,yy,zz,dx,dy,dz)
double *xx,*yy,*zz,*dx,*dy,*dz;
double x1,x2,x3,x4,x5,x6;
{
    double x[6],xxx[6];
    x[0]=x1;
    x[1]=x2;
    x[2]=x3;  /*Polar Radians Position */
    x[3]=x4;
    x[4]=x5;
    x[5]=x6;  /* Polar Radians Speed */
    swi_polcart_sp(x,xxx);
    *xx=xxx[0];
    *yy=xxx[1];
    *zz=xxx[2];
    *dx=xxx[3];
    *dy=xxx[4];
    *dz=xxx[5];
    return;
}

static AS_BOOL path_built=FALSE;
                          void init_ephe_path()
{
    char serr[AS_MAXCH];
    /*
     * if the function is called the first time, we build an ephemeris path
     * in the same way as FileOpen() in io.c searchs for a file.
     * we use serr to build the list of directories in it.
     * If environment variable SE_EPHE_PATH exists, it will override all this.
     */
    if (! path_built) {
#ifdef ENVIRON
        char *env;
        /* Next add the directory indicated by the version */
        /* specific system environment variable.          */
        env = getenv("SE_EPHE_PATH");
        if (env && *env)
            sprintf(serr, "%s", env);
#endif
        /* Finally add several directories specified at compile time. */
        sprintf(serr + strlen(serr), "%s%s", PATH_SEPARATOR, EPHE_DIR);
        sprintf(serr + strlen(serr), "%s%s", PATH_SEPARATOR, DEFAULT_DIR);
        /*    sprintf(serr + strlen(serr), "%s%s", PATH_SEPARATOR, CHART_DIR);*/
        swe_set_ephe_path(serr);
        if (fUseJPL==405)
            swe_set_jpl_file("de405.eph");
        if (fUseJPL==406)
            swe_set_jpl_file("de406.eph");
        swe_set_topo(-oo,aa,NN);
        path_built = TRUE;
    }
    return;
}
#endif

