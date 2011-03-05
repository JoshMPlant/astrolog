/*
** Astrolog (Version 5.42J) File: charts3.c
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
** Initial programming 8/28,30, 9/10,13,16,20,23, 10/3,6,7, 11/7,10,21/1991.
** X Window graphics initially programmed 10/23-29/1991.
** PostScript graphics initially programmed 11/29-30/1992.
** Last code change made 12/20/1998.
** Modifications from version 5.40 to 5.41 are by Alois Treindl.
** Modifications from version 5.41 to 5.42 are by Nicolas Scharnagl

*/

#include "astrolog.h"
#include "swephexp.h"
real lonz1[objMax], lonz2[objMax], latz1[objMax], latz2[objMax];

/*
******************************************************************************
** Multiple Chart Scanning Routines.
******************************************************************************
*/

/* Search through a day, and print out the times of exact aspects among the  */
/* planets during that day, as specified with the -d switch, as well as the  */
/* times when a planet changes sign or direction. To do this, we cast charts */
/* for the beginning and end of the day, or a part of a day, and do a linear */
/* equation check to see if anything exciting happens during the interval.   */
/* (This is probably the single most complicated procedure in the program.)  */

void ChartInDaySearch(fProg)
bool fProg;
{
    char sz[cchSzDef];
    int source[MAXINDAY], aspect[MAXINDAY], dest[MAXINDAY],
    sign1[MAXINDAY], sign2[MAXINDAY], D1, D2, counttotal = 0, occurcount,
            division, div, fYear, yea0, yea1, yea2, i, j, k, l=0, s1,s2;
    real time[MAXINDAY], divsiz, d1, d2, e1, e2, f1, f2, g, s3, time1, time2;
    real prevs[cObjOpt];
    CI ciT;
    CP cpT;
    /* If parameter 'fProg' is set, look for changes in a progressed chart. */
    ciT = ciTran;
    fYear = us.fInDayMonth && (MonT == 0);
    division = (fYear || fProg) ? 1 : us.nDivision;
    divsiz = 24.0 / (real)division*60.0;


     if (fSecMotion)
            rSidDay=rDegQuad/Mod(MinDistance(15.0*swe_sidtime(is.JDr),15.0*swe_sidtime(is.JDr +(fEclipseBackward ? -rRound/2.0 : rRound/2.0))));
    /* If -dY in effect, then search through a range of years. */

    yea1 = fProg ? YeaT : Yea;
    yea2 = fYear ? (yea1 + us.nEphemYears - 1) : yea1;
    for (yea0 = yea1; yea0 <= yea2; yea0++) {

        /* If -dm in effect, then search through the whole month, day by day. */

        if (us.fInDayMonth) {
            D1 = 1;
            if (fYear) {
                MonT = 1;
                D2 = DayInYearHi(yea0);
            } else
                D2 = DayInMonth(fProg ? MonT : Mon, yea0);
        } else
            D1 = D2 = Day;

        if (us.fParallel)
            for (i=0; i<=cObjOpt; i++)
                prevs[i]=777.0;
#ifdef KDE
        KGetTZ(&Zon);
#endif

        /* Start searching the day or days in question for exciting stuff. */

        for (DayT = D1; DayT <= D2; DayT = AddDay(Mon, DayT, yea0, 1)) {
            occurcount = 0;

            /* Cast chart for beginning of day and store it for future use. */

            SetCIA(ciCore, fYear ? MonT : Mon, DayT, yea0, 0.0, Dst, Zon, Lon, Lat,Alt);
            if (fProg) {
                is.JDp = MdytszToJulian(MonT, DayT, yea0, 0.0, Dst, Zon)-rRound;
                ciCore = ciMain;
                us.fProgress = fTrue;
            }
            CastChart(fTrue);
            if (us.fDirect && us.fSector)
                CastSectors();
            cp1 = cp2;
            cp2 = cp0;

            /* Now divide the day into segments and search each segment in turn. */
            /* More segments is slower, but has slightly better time accuracy.   */

            for (div = 1; div <= division; div++) {

                /* Cast the chart for the ending time of the present segment. The   */
                /* beginning time chart is copied from the previous end time chart. */

                SetCIA(ciCore, fYear ? MonT : Mon, DayT, yea0,
                       DegToDec(24.0*((real)div/(real)division)), Dst, Zon, Lon, Lat,Alt);
                if (fProg) {
                    is.JDp = MdytszToJulian(MonT, DayT, yea0, DegToDec(24.0*(real)(div/division)), Dst, Zon)-rRound;
                    ciCore = ciMain;
                    us.fProgress = fTrue;
                }
                CastChart(fTrue);
                if (!us.fSweHouse) {
                    cp3=cp2;
                    cpT=cp1;
                }
                if (us.fDirect && us.fSector)
                    CastSectors();
                if (!us.fSweHouse) {
                    cp2=cp3;
                    cp1=cpT;
                }

                cp1 = cp2;
                cp2 = cp0;

                if (us.fParallel) {
                    for (i = 0; i <= cObjOpt; i++) if (!ignore[i]) {
                            if (us.fEquator) {
                                lonz1[i] = cp1.obj[i];
                                latz1[i] = cp1.alt[i];
                                lonz2[i] = cp2.obj[i];
                                latz2[i] = cp2.alt[i];
                } else if (us.fSector&&us.fDirect) {
                    lonz1[i] = cp1.obj[i];
                    latz1[i] = -RSinD(cp1.obj[i]);
                    lonz2[i] = cp2.obj[i];
                    latz2[i] = -RSinD(cp2.obj[i]);
                            } else {
                                lonz1[i] = RFromD(Tropical(cp1.obj[i]));
                                latz1[i] = RFromD(cp1.alt[i]);
                                EclToEqu(&lonz1[i], &latz1[i]);
                                latz1[i] = DFromR(latz1[i]);
                                lonz2[i] = RFromD(Tropical(cp2.obj[i]));
                                latz2[i] = RFromD(cp2.alt[i]);
                                EclToEqu(&lonz2[i], &latz2[i]);
                                latz2[i] = DFromR(latz2[i]);
                            }
                        }
                }

                /* Now search through the present segment for anything exciting. */

                for (i = 0; i <= cObjOpt; i++) if (!FIgnore(i) && ((fProg&&!(us.fSector&&us.fDirect)) || FThing(i) || FAst(i))) {
                        s1 = SFromZ(cp1.obj[i])-1;
                        s2 = SFromZ(cp2.obj[i])-1;

                        if (!us.fParallel) {

                            /* Does the current planet change into the next or previous sign? */

                            if (s1 != s2 && !us.fIgnoreSign) {
                                source[occurcount] = i;
                                aspect[occurcount] = aSig;
                                dest[occurcount] = s2+1;
                                time[occurcount] = MinDistance(cp1.obj[i],
                                                               (real)(cp1.dir[i] >= 0.0 ? s2 : s1) * 30.0) /
                                                   MinDistance(cp1.obj[i], cp2.obj[i])*divsiz + (real)(div-1)*divsiz;
                                sign1[occurcount] = sign2[occurcount] = s1+1;
                                occurcount++;
                            }

                            /* Does the current planet go retrograde or direct? */

                            if ((cp1.dir[i] < 0.0) != (cp2.dir[i] < 0.0) && !us.fIgnoreDir) {
                                source[occurcount] = i;
                                aspect[occurcount] = aDir;
                                dest[occurcount] = cp2.dir[i] < 0.0;
                                time[occurcount] = RAbs(cp1.dir[i])/(RAbs(cp1.dir[i])+
                                                                     RAbs(cp2.dir[i]))*divsiz + (real)(div-1)*divsiz;
                                sign1[occurcount] = sign2[occurcount] = s1+1;
                                occurcount++;
                            }

                            /* Now search for anything making an aspect to the current planet. */

                            for (j = i+1; j <= cObjOpt; j++) if (!FIgnore(j) && (fProg || FThing(j))&& !(FIgnoreS(i)&&FIgnoreS(j))&& !(us.fSector && us.fDirect && FCusp(i) && FCusp(j)))
                                    for (k = 1; k <= us.nAsp; k++) if (FAcceptAspect(i, k, j)) {
                                            d1 = cp1.obj[i];
                                            d2 = cp2.obj[i];
                                            e1 = cp1.obj[j];
                                            e2 = cp2.obj[j];
                                            if (MinDistance(d1, d2) < MinDistance(e1, e2)) {
                                                SwapR(&d1, &e1);
                                                SwapR(&d2, &e2);
                                            }

                                            /* We are searching each aspect in turn. Let's subtract the  */
                                            /* size of the aspect from the angular difference, so we can */
                                            /* then treat it like a conjunction.                         */

                                            if (MinDistance(e1, Mod(d1-rAspAngle[k])) <
                                                    MinDistance(e2, Mod(d2+rAspAngle[k]))) {
                                                e1 = Mod(e1+rAspAngle[k]);
                                                e2 = Mod(e2+rAspAngle[k]);
                                            } else {
                                                e1 = Mod(e1-rAspAngle[k]);
                                                e2 = Mod(e2-rAspAngle[k]);
                                            }

                                            /* Check to see if the aspect actually occurs during our    */
                                            /* segment, making sure we take into account if one or both */
                                            /* planets are retrograde or if they cross the Aries point. */

                                            f1 = e1-d1;
                                            if (RAbs(f1) > rDegHalf)
                                                f1 -= RSgn(f1)*rDegMax;
                                            f2 = e2-d2;
                                            if (RAbs(f2) > rDegHalf)
                                                f2 -= RSgn(f2)*rDegMax;
                                            if (MinDistance(Midpoint(d1, d2), Midpoint(e1, e2)) < rDegQuad &&
                                                    RSgn(f1) != RSgn(f2)) {
                                                source[occurcount] = i;
                                                aspect[occurcount] = k;
                                                dest[occurcount] = j;
                                                /*        sprintf(sz,"%f %f %f %f %f %f\n",f1,f2,d1,e1,d2,e2);PrintSz(sz); */

                                                /* Horray! The aspect occurs sometime during the interval.   */
                                                /* Now we just have to solve an equation in two variables to */
                                                /* find out where the "lines" cross, i.e. the aspect's time. */

                                                f1 = d2-d1;
                                                if (RAbs(f1) > rDegHalf)
                                                    f1 -= RSgn(f1)*rDegMax;
                                                f2 = e2-e1;
                                                if (RAbs(f2) > rDegHalf)
                                                    f2 -= RSgn(f2)*rDegMax;
                                                g = (RAbs(d1-e1) > rDegHalf ?
                                                     (d1-e1)-RSgn(d1-e1)*rDegMax : d1-e1)/(f2-f1);
                                                time[occurcount] = g*divsiz + (real)(div-1)*divsiz;
                                                sign1[occurcount] = (int)(Mod(cp1.obj[i]+
                                                                              RSgn(cp2.obj[i]-cp1.obj[i])*
                                                                              (RAbs(cp2.obj[i]-cp1.obj[i]) > rDegHalf ? -1 : 1)*
                                                                              RAbs(g)*MinDistance(cp1.obj[i], cp2.obj[i]))/30.0)+1;
                                                sign2[occurcount] = (int)(Mod(cp1.obj[j]+
                                                                              RSgn(cp2.obj[j]-cp1.obj[j])*
                                                                              (RAbs(cp2.obj[j]-cp1.obj[j]) > rDegHalf ? -1 : 1)*
                                                                              RAbs(g)*MinDistance(cp1.obj[j], cp2.obj[j]))/30.0)+1;
                                                occurcount++;
                                            }
                                        }

                        } else {

                            /* Does the current planet change +- sign in declination? */

                            if ((latz1[i] < 0.0) != (latz2[i] < 0.0) && !us.fIgnoreSign) {
                                source[occurcount] = i;
                                aspect[occurcount] = aSig;
                                dest[occurcount] = 16-(latz2[i] < 0.0);
                                time[occurcount] = RAbs(latz1[i])/(RAbs(latz1[i])+
                                                                   RAbs(latz2[i]))*divsiz + (real)(div-1)*divsiz;
                                sign1[occurcount] = sign2[occurcount] = s1+1;
                                occurcount++;
                            }

                            /* Does the current planet reach min/max declination? */

                            if ((((latz2[i]-latz1[i])<0.0) != (prevs[i]<0.0)) && (prevs[i] < 776.0)&& !us.fIgnoreDir) {
                                source[occurcount] = i;
                                aspect[occurcount] = aSig;
                                dest[occurcount] = 14-((latz2[i]-latz1[i]) < 0.0);
                                time[occurcount] = RAbs(prevs[i])/(RAbs(prevs[i])+
                                                                   RAbs(latz2[i]-latz1[i]))*divsiz + (real)(div-1)*divsiz;
                                sign1[occurcount] = sign2[occurcount] = s1+1;
                                occurcount++;
                            }
                            prevs[i]=latz2[i]-latz1[i];

                            /* Does the current planet go out of north bounds in declination? */

                            if ((latz1[i] < us.nEpsilon) != (latz2[i] < us.nEpsilon) && !us.fIgnoreBounds) {
                                source[occurcount] = i;
                                aspect[occurcount] = aSig;
                                dest[occurcount] = 17+(latz2[i] < us.nEpsilon);
                                time[occurcount] = RAbs(latz1[i])/(RAbs(latz1[i])+
                                                                   RAbs(latz2[i]))*divsiz + (real)(div-1)*divsiz;
                                sign1[occurcount] = sign2[occurcount] = s1+1;
                                occurcount++;
                            }

                            /* Does the current planet go out of south bounds in declination? */

                            if ((latz1[i] > -us.nEpsilon) != (latz2[i] > -us.nEpsilon) && !us.fIgnoreBounds) {
                                source[occurcount] = i;
                                aspect[occurcount] = aSig;
                                dest[occurcount] = 19+(latz2[i] > -us.nEpsilon);
                                time[occurcount] = RAbs(latz1[i])/(RAbs(latz1[i])+
                                                                   RAbs(latz2[i]))*divsiz + (real)(div-1)*divsiz;
                                sign1[occurcount] = sign2[occurcount] = s1+1;
                                occurcount++;
                            }

                            /* Now search for anything making an parallel to the current planet. */

                            for (j = i+1; j <= cObjOpt; j++) if (!FIgnore(j) && (fProg || FThing(j))&& !(FIgnoreS(i)&&FIgnoreS(j))) {
                                    if ((FCusp(i) && FCusp(j)) || !us.nAsp)
                                        continue;
                                    k = 0;
                                    d1 = latz1[i];
                                    d2 = latz2[i];
                                    e1 = latz1[j];
                                    e2 = latz2[j];
                                    if (RAbs(d2 - d1) < RAbs(e2 - e1)) {
                                        SwapR(&d1, &e1);
                                        SwapR(&d2, &e2);
                                    }
                                    if (((d2 > d1) && (FBetween(e1, d1, d2) || FBetween(e2, d1, d2))) ||
                                            ((d2 < d1) && (FBetween(e1, d2, d1) || FBetween(e2, d2, d1)))) {
                                        if ((d2 - d1) != (e2 - e1))
                                            k = 1;
                                    }
                                    if (k == 0 &&us.nAsp > 1) {
                                        e1 = - e1;
                                        e2 = - e2;
                                        if (((d2 > d1) && (FBetween(e1, d1, d2) || FBetween(e2, d1, d2))) ||
                                                ((d2 < d1) && (FBetween(e1, d2, d1) || FBetween(e2, d2, d1)))) {
                                            if ((d2 - d1) != (e2 - e1))
                                                k=2;
                                        }
                                    }
                                    if (k) {
                                        f1 = d2 - d1;
                                        f2 = e2 - e1;
                                        g = (e1 - d1) / (f1 -f2);
                                        time[occurcount] = g*divsiz + (real)(div-1)*divsiz;
                                        time1 = divsiz*(real)(div-1);
                                        time2 = divsiz*(real)div;
                                        if (time[occurcount] >= time1 && time[occurcount] <= time2) {
                                            source[occurcount] = i;
                                            aspect[occurcount] = k;
                                            dest[occurcount] = j;
                                            sign1[occurcount] = (int)(Mod(cp1.obj[i]+
                                                                          RSgn(cp2.obj[i]-cp1.obj[i])*
                                                                          (RAbs(cp2.obj[i]-cp1.obj[i]) > rDegHalf ? -1 : 1)*
                                                                          RAbs(g)*MinDistance(cp1.obj[i], cp2.obj[i]))/30.0)+1;
                                            sign2[occurcount] = (int)(Mod(cp1.obj[j]+
                                                                          RSgn(cp2.obj[j]-cp1.obj[j])*
                                                                          (RAbs(cp2.obj[j]-cp1.obj[j]) > rDegHalf ? -1 : 1)*
                                                                          RAbs(g)*MinDistance(cp1.obj[j], cp2.obj[j]))/30.0)+1;
                                            occurcount++;
                                        }
                                    }
                                }
                        }
                    }
            }

            /* After all the aspects, etc, in the day have been located, sort   */
            /* them by time at which they occur, so we can print them in order. */

            for (i = 1; i < occurcount; i++) {
                j = i-1;
                while (j >= 0 && time[j] > time[j+1]) {
                    SwapN(source[j], source[j+1]);
                    SwapN(aspect[j], aspect[j+1]);
                    SwapN(dest[j], dest[j+1]);
                    SwapR(&time[j], &time[j+1]);
                    SwapN(sign1[j], sign1[j+1]);
                    SwapN(sign2[j], sign2[j+1]);
                    j--;
                }
            }

            /* Finally, loop through and display each aspect and when it occurs. */

            for (i = 0; i < occurcount; i++) {
                s1 = (int)time[i]/60;
                s3 = time[i]-s1*60;
                j = DayT;
                if (fYear || fProg) {
                    l = MonT;
                    while (j > (k = DayInMonth(l, yea0))) {
                        j -= k;
                        l++;
                    }
                }
                SetCIA(ciSave, fYear || fProg ? l : Mon, j, yea0,
                       DegToDec(time[i] / 60.0), Dst, Zon, Lon, Lat,Alt);
                k = DayOfWeek(fYear || fProg ? l : Mon, j, yea0);
                AnsiColor(kRainbowA[k + 1]);
                sprintf(sz, "(%c%c%c) ", chDay3(k));
                PrintSz(sz);
                AnsiColor(kDefault);
                sprintf(sz, "%s %s ",
                        SzDate(fYear || fProg ? l : Mon, j, yea0, 2*MonthFormat),
                        SzTime(s1, (int)s3,(int)(rRound+RFract(s3)*60.0)));
                PrintSz(sz);
                PrintAspect(source[i], sign1[i],
(us.fSector&&us.fDirect) ? fEclipseBackward :
                            (int)RSgn(cp1.dir[source[i]])+(int)RSgn(cp2.dir[source[i]]),
                            aspect[i], dest[i], sign2[i],
                            (int)RSgn(cp1.dir[dest[i]])+(int)RSgn(cp2.dir[dest[i]]),
                            (byte)(fProg ? 'e' : 'd'));
                PrintInDay(source[i], aspect[i], dest[i]);
            }
            counttotal += occurcount;
        }
    }
    if (counttotal == 0)
        PrintSz("No events found.\n");
    else if (us.fDebug) {
        sprintf(sz,"%d events found",counttotal);
        PrintSz(sz);
    }

    /* Recompute original chart placements as we've overwritten them. */

    ciCore = ciMain;
    ciTran = ciT;
    CastChart(fTrue);
}


/* Search through a month, year, or years, and print out the times of exact */
/* transits where planets in the time frame make aspect to the planets in   */
/* some other chart, as specified with the -t switch. To do this, we cast   */
/* charts for the start and end of each month, or within a month, and do an */
/* equation check for aspects to the other base chart during the interval.  */

void ChartTransitSearch(fProg)
bool fProg;
{
    real planet3[objMax], house3[cSign+1], ret3[objMax], planet4[objMax], house4[cSign+1], ret4[objMax], time[MAXINDAY];
    char sz[cchSzDef];
    int source[MAXINDAY], aspect[MAXINDAY], dest[MAXINDAY], sign[MAXINDAY],
    isret[MAXINDAY], M1, M2, Y1, Y2, counttotal = 0, occurcount, division,
            div, nAsp, fCusp, i, j, k,kk, s1, s2, DayT2,yea0;
    real lonn[objMax], latn[objMax];
    real jd0,divsiz, daysiz, d, lastdiv, d2, e1, e2, f1, f2,s3,DayT3;
    CI ciT,ci1;
    CP cpt1,cpt2;
    int fProgressT,fEclipseBackwardT,fDirectT,month;

    /* Save away natal chart and initialize things. */
    ciT = ciTran;
    ci1 = ciMain;
    fEclipseBackwardT=fEclipseBackward;
    fProgressT=us.fProgress;
    fEclipseBackward=0;
    if (us.fDirect && us.fSector ) {
        if (FBetween(fBirthRate,1,2)) {
            SetBirthRate();
            ciCore=ciMain;
            CastChart(fTrue);
        }
        if (fSecMotion)
            rSidDay=rDegQuad/Mod(MinDistance(15.0*swe_sidtime(is.JDr),15.0*swe_sidtime(is.JDr +(fEclipseBackward ? -rRound/2.0 : rRound/2.0))));
            jd0=is.JDr;
                                                if (us.fDebug) {
                                                    sprintf(sz,"JDr %f JDp %f sid %f\n",is.JDr,is.JDp,rSidDay);
                                                    PrintSz(sz);
                                                }
        us.fProgress=fFalse;
        CastSectors();
    }
    for (i = 1; i <= cSign; i++)
        house3[i] = house4[i] = chouse[i];
    for (i = 0; i <= cObjOpt; i++) {
        planet3[i] = planet4[i] = planet[i];
        ret3[i] = ret4[i] = ret[i];
    }
    if (fProg||(us.fSector&&us.fDirect /*&&fGauqMode==1*/))
        fCusp = fFalse;
    else {
        fCusp = fTrue;
        for (i = cuspLo; i <= cuspHi; i++)
            fCusp &= ignore2[i];
    }
    division = us.nDivision;
    if (!fProg && !fCusp)
        division = Max(division, 192);
    if (MonT > 0 && us.fDirect && us.fSector && fGauqMode>=2)
        division = 1;
    nAsp = (is.fReturn || (us.fDirect && us.fSector) )? aCon : us.nAsp;

    if (us.fParallel) {
        for (i = 0; i <= cObjOpt; i++) if (!ignore[i]) {
                if (us.fEquator) {
                    lonn[i] = planet[i];
                    latn[i] = planetalt[i];
                } else if (us.fSector&&us.fDirect) {
                    lonn[i] = planet[i];
                    latn[i] = -RSinD(planet[i]);
                } else {
                    lonn[i] = RFromD(Tropical(planet[i]));
                    latn[i] = RFromD(planetalt[i]);
                    EclToEqu(&lonn[i], &latn[i]);
                    latn[i] = DFromR(latn[i]);
                }
            }
    }

    /* Hacks: Searching month number zero means to search the whole year    */
    /* instead, month by month. Searching a negative month means to search  */
    /* multiple years, with the span of the year stored in the "day" field. */

    Y1 = Y2 = YeaT;
    M1 = M2 = MonT;
    DayT2=1;
    DayT3=1.0;
    if (MonT < 1) {
        M1 = 1;
        M2 = 12;
        if (MonT < 0) {
            if (DayT < 1) {
                Y1 = YeaT + DayT + 1;
                Y2 = YeaT;
            } else {
                Y1 = YeaT;
                Y2 = YeaT + DayT - 1;
            }
        }
    }
    else if (DayT > 1) {
        DayT2=DayT ;
        DayT3=(real)DayT2;
    }
#ifdef KDE
    KGetTZ(&ZonT);
#endif

    /* Start searching the year or years in question for any transits. */

    for (yea0 = Y1; yea0 <= Y2; yea0++)

        /* Start searching the month or months in question for any transits. */

        for (MonT = M1; MonT <= M2; MonT++) {
            occurcount = 0;
converse:
            if (fProg && us.fDirect && M2==12 && M1==1) {
                if (MonT>1)
                    continue;
                DayT2=1;
                DayT3=1.0;
                daysiz=(real)(DayInYearHi(yea0)*24.0*60.0);
                divsiz=daysiz/(real)division;
            } else {
                daysiz = (real)(DayInMonth(MonT, yea0)-DayT2+1)*24.0*60.0;
                divsiz = daysiz / (real)division;
            }

            /* Progress chart if -pt switch and store it for future use. */
            /* For relationship charts it stays static */

            if (us.fTransProg && !us.nRel &&! us.fParallel) {
                ciCore = ci1;
                is.JDp = MdydszToJulian(MonT, DayT2, yea0, 0, DstT, ZonT);
                us.fProgress = fTrue;
                fDirectT=us.fDirect;
                us.fDirect=fFalse;
                CastChart(fTrue);
                us.fProgress = fFalse;
                if (fDirectT && us.fSector ) {
                    CastSectors();
                    us.fDirect=fDirectT;
                }
                for (i = 1; i <= cSign; i++)
                    house4[i] = house3[i];
                house3[i] = chouse[i];
                for (i = 0; i <= cObjOpt; i++) {
                    planet4[i] = planet3[i];
                    planet3[i] = planet[i];
                    ret4[i] = ret3[i];
                    ret3[i] = ret[i];
                }
            }
            for (kk=us.nAsp; kk>=-us.nAsp; kk--) {
                if (FBetween(kk,-2,0))
                    continue;
                if (!(us.fSector&&us.fDirect)&&kk!=1)
                    continue;
                if ((us.fParallel ||fGauqMode>=2)&&kk!=1)
                    continue;

                rAddAngle=kk<0 ? -rAspAngle[-kk] : rAspAngle[kk];
                /* Cast chart for beginning of month and store it for future use. */

                SetCIA(ciCore, MonT, DayT2, yea0, 0.0, DstT, ZonT, LonT, LatT,AltT);
                for (i = 0; i <= cObjOpt; i++)
                    SwapN(ignore[i], ignore2[i]);

                if (fProg) {
                    is.JDp = (MdytszToJulian(MM, DD, YY, 0.0, DstT, ZonT))-rRound;
                    ciCore = ci1;
                    us.fProgress = fTrue;
                    if (us.nRel) {
                        CastRelation();
                    }
                    else CastChart(fTrue);
                    if (us.fDirect && us.fSector ) {
                        CastSectors();
                    }
                    us.fProgress = fFalse;
                } else CastChart(fTrue);

                for (i = 0; i <= cObjOpt; i++)
                    SwapN(ignore[i], ignore2[i]);
                cpt2 = cp0;

                /* Divide our month into segments and then search each segment in turn. */

                for (div = 1; div <= division; div++) {
                    /* Cast the chart for the ending time of the present segment, and */
                    /* copy the start time chart from the previous end time chart.    */

                    d = DayT3 + (div*daysiz)/(24.0*60.0*division);


                    /* Progress further if -pt switch */
                    /* For relationship charts it stays static */
                    if (us.fTransProg && !us.nRel ) {
                        ciCore = ciMain;
                        is.JDp = MdydszToJulian(MonT, RFloor(d), yea0, (RFract(d)*24.0), DstT, ZonT);
                        us.fProgress = fTrue;
                        fDirectT=us.fDirect;
                        us.fDirect=fFalse;
                        CastChart(fTrue);
                        if (fDirectT && us.fSector ) {
                            CastSectors();
                            us.fDirect=fDirectT;
                        }
                        us.fProgress = fFalse;
                        for (i = 1; i <= cSign; i++)
                            house4[i] = house3[i];
                        house3[i] = chouse[i];
                        for (i = 0; i <= cObjOpt; i++) {
                            planet4[i] = planet3[i];
                            planet3[i] = planet[i];
                            ret4[i] = ret3[i];
                            ret3[i] = ret[i];
                        }
                    }
                    SetCIA(ciCore, MonT, RFloor(d), yea0,
                           DegToDec(RFract(d)*24.0), DstT, ZonT, LonT, LatT,AltT);
                    lastdiv=-is.JDp;
                    for (i = 0; i <= cObjOpt; i++)
                        SwapN(ignore[i], ignore2[i]);
                    if (fProg) {
                        while (!FValidDay(DD,MM,YY)) {
                            if (DD>DayInMonth(MM,YY)) {
                                DD-=DayInMonth(MM,YY);
                                MM++;
                            }
                            if (MM>12) {
                                MM=1;
                                YY++;
                            }
                        }
                        is.JDp = (MdytszToJulian(MM, DD, YY, DegToDec(RFract(d)*24.0) , DstT, ZonT))-rRound;
                        if (us.fDebug>3) {
                            lastdiv+=is.JDp;
                            sprintf(sz,"lastdiv %f divsize %f JDp %f arc %f\n",lastdiv,divsiz/(24.0*60.0),is.JDp,ProgToArc(jd0,is.JDp));
                            PrintSz(sz);
                        }
                        ciCore = ciMain;
                        us.fProgress = fTrue;
                        if (us.nRel) {
                            ciCore=ci1;
                            CastRelation();
                        }
                        else CastChart(fTrue);
                        if (us.fDirect && us.fSector) {
                            CastSectors();
                        }
                        us.fProgress = fFalse;
                    } else CastChart(fTrue);

                    for (i = 0; i <= cObjOpt; i++)
                        SwapN(ignore[i], ignore2[i]);
                    cpt1 = cpt2;
                    cpt2 = cp0;

                    if (us.fParallel) {
                        for (i = 0; i <= cObjOpt; i++) if (!FIgnore2(i)&&FNorm(i)) {
                                if (us.fEquator) {
                                    lonz1[i] = cpt1.obj[i];
                                    latz1[i] = cpt1.alt[i];
                                    lonz2[i] = cpt2.obj[i];
                                    latz2[i] = cpt2.alt[i];
                } else if (us.fSector&&us.fDirect) {
                    lonz1[i] = cpt1.obj[i];
                    latz1[i] = -RSinD(cpt1.obj[i]);
                    lonz2[i] = cpt2.obj[i];
                    latz2[i] = -RSinD(cpt2.obj[i]);
                                } else {
                                    lonz1[i] = RFromD(Tropical(cpt1.obj[i]));
                                    latz1[i] = RFromD(cpt1.alt[i]);
                                    EclToEqu(&lonz1[i], &latz1[i]);
                                    latz1[i] = DFromR(latz1[i]);
                                    lonz2[i] = RFromD(Tropical(cpt2.obj[i]));
                                    latz2[i] = RFromD(cpt2.alt[i]);
                                    EclToEqu(&lonz2[i], &latz2[i]);
                                    latz2[i] = DFromR(latz2[i]);
                                }
                            }
                    }

                    /* Now search through the present segment for any transits. Note that */
                    /* stars can be transited, but they can't make transits themselves.   */

                    for (i = 0; i <= cObjOpt; i++) if (!FIgnore(i)) {
                            if (us.fSector&&us.fDirect)
                                us.nEclObj2=i; /*Sig*/
                            for (j = 0; j <= oNormOpt; j++) {
                                if ((is.fReturn ? i != j : FIgnore2(j)) || (!us.fSector && FStar(j))||(!FAst(j) && fCusp && !FThing(j)))
                                    continue;

                                /* Between each pair of planets, check if they make any aspects. */

                                if (!us.fParallel) {

                                    for (k = 1; k <= nAsp; k++) if (FAcceptAspect(i, k, j)) {
                                            d = planet3[i];
                                            d2 = planet4[i];
                                            e1 = cpt1.obj[j];
                                            e2 = cpt2.obj[j];
                                            if (MinDistance(e1, Mod(d-rAspAngle[k])) <
                                                    MinDistance(e2, Mod(d+rAspAngle[k]))) {
                                                e1 = Mod(e1+rAspAngle[k]);
                                                e2 = Mod(e2+rAspAngle[k]);
                                            } else {
                                                e1 = Mod(e1-rAspAngle[k]);
                                                e2 = Mod(e2-rAspAngle[k]);
                                            }

                                            /* Check to see if the present aspect actually occurs during the */
                                            /* segment, making sure we check any Aries point crossings.      */
                                            /* Have to use d,d2 because progressed moon may be faster than transit object */

                                            f1 = e1-d2;
                                            if (RAbs(f1) > rDegHalf)
                                                f1 -= RSgn(f1)*rDegMax;
                                            f2 = e2-d;
                                            if (RAbs(f2) > rDegHalf)
                                                f2 -= RSgn(f2)*rDegMax;
                                            if (MinDistance(d, Midpoint(e1, e2)) < rDegQuad &&
                                                    RSgn(f1) != RSgn(f2) && occurcount < MAXINDAY) {

                                                /* Ok, we have found a transit. Now determine the time */
                                                /* and save this transit in our list to be printed.    */

                                                if (us.fDebug>4) {
                                                    sprintf(sz,"f1 %f f2 %f JDp %f TT %f\n",f1,f2,is.JDp,TT);
                                                    PrintSz(sz);
                                                }
                                                source[occurcount] = j;
                                                if (us.fDirect&&us.fSector) {
                                                    aspect[occurcount] = kk;
                                                }
                                                else aspect[occurcount] = k;
                                                dest[occurcount] = i;
                                                time[occurcount] = RAbs(f1)/(RAbs(f1)+RAbs(f2))*divsiz +
                                                                   (real)(div-1)*divsiz;
                                                sign[occurcount] = (int)(Mod(
                                                                             MinDistance(cpt1.obj[j], Mod(d-rAspAngle[k])) <
                                                                             MinDistance(cpt2.obj[j], Mod(d+rAspAngle[k])) ?
                                                                             d-rAspAngle[k] : d+rAspAngle[k])/30.0)+1;
                                                if (us.fDirect&&us.fSector)
                                                    isret[occurcount] = fEclipseBackward;
                                                else isret[occurcount] = (int)RSgn(cpt1.dir[j]) +
                                                                             (int)RSgn(cpt2.dir[j]);
                                                occurcount++;
                                            }
                                        }
                                } else {
                                    k = 0;
                                    d = latn[i];
                                    e1 = latz1[j];
                                    e2 = latz2[j];
                                    if (((e2 > e1) && FBetween(d, e1, e2)) ||
                                            ((e2 < e1) && FBetween(d, e2, e1)))
                                        k = 1;         /*  Found parallel.  */
                                    if (!k) {
                                        d = -d;
                                        if (((e2 > e1) && FBetween(d, e1, e2)) ||
                                                ((e2 < e1) && FBetween(d, e2, e1)))
                                            k = 2;       /*  Found contra-parallel.  */
                                    }
                                    if (k && k<=us.nAsp) {
                                        source[occurcount] = j;
                                        aspect[occurcount] = k;
                                        dest[occurcount]   = i;
                                        f1 = RAbs(d - e1) / RAbs(e2 - e1);
                                        time[occurcount] = divsiz * f1 + (real)(div - 1) * divsiz;
                                        sign[occurcount] = (int)(Mod(cpt1.obj[j]+
                                                                     RSgn(cpt2.obj[j]-cpt1.obj[j])*
                                                                     (RAbs(cpt2.obj[j]-cpt1.obj[j]) > rDegHalf ? -1 : 1)*
                                                                     RAbs(f1)*MinDistance(cpt1.obj[j], cpt2.obj[j]))/30.0)+1;
                                        if (us.fDirect&&us.fSector)
                                            isret[occurcount] = fEclipseBackward;
                                        else isret[occurcount] = 1;
                                        occurcount++;
                                    }
                                }

                            }
                        }
                } /* for division */
            } /* for aspect */
            if (fEclipseBackward==0 && us.fDirect && us.fSector ) { /* for converse */
                fEclipseBackward=1;
                goto converse;
            }
            else
                fEclipseBackward=0;

            /* After all transits located, sort them by time at which they occur. */

            for (i = 1; i < occurcount; i++) {
                j = i-1;
                while (j >= 0 && time[j] > time[j+1]) {
                    SwapN(source[j], source[j+1]);
                    SwapN(aspect[j], aspect[j+1]);
                    SwapN(dest[j], dest[j+1]);
                    SwapR(&time[j], &time[j+1]);
                    SwapN(sign[j], sign[j+1]);
                    SwapN(isret[j], isret[j+1]);
                    j--;
                }
            }

            /* Now loop through list and display all the transits. */

            for (i = 0; i < occurcount; i++) {
                s1 = (_int)time[i]/24/60;
                s3 = time[i]-s1*24*60;
                s2 = (_int)s3/60;
                s3 = s3-s2*60;
                month=MonT;
                while (!FValidDay(s1+DayT2,month,yea0)) {
                    s1-=DayInMonth(month,yea0);
                    month++;
                }
                /*            SetCIA(ciSave, MonT, s1+DayT2, YeaT, DegToDec((real)
                                    ((_int)time[i]-s1*24*60) / 60.0), DstT, ZonT, LonT, LatT,AltT);*/
                sprintf(sz, "%s %s ",
                        SzDate(month, s1+DayT2, yea0, 2*MonthFormat), SzTime(s2, (int)s3,-1 /* (int)(rRound+RFract(s3)*60.0)*/));
                PrintSz(sz);
                PrintAspect(source[i], sign[i], isret[i], aspect[i],
                            dest[i], SFromZ(planet3[dest[i]]), (int)RSgn(ret3[dest[i]]),
                            (byte)(fProg ? ((us.fDirect&&us.fSector) ? 'v' : 'u' ) : us.fTransProg ? 'x' : 't'));

                /* Check for a Solar, Lunar, or any other return. */

                if (aspect[i] == aCon && source[i] == dest[i] && !us.fParallel ) {
                    AnsiColor(kWhite);
                    sprintf(sz, " (%s Return)", source[i] == oSun ? "Solar" :
                            (source[i] == oMoo ? "Lunar" : szObjName[source[i]]));
                    PrintSz(sz);
                }
                PrintL();
#ifdef INTERPRET
                if (us.fInterpret)
                    InterpretTransit(source[i], aspect[i], dest[i]);
#endif
                AnsiColor(kDefault);
            }
            counttotal += occurcount;
        } /* for month */
    if (counttotal == 0)
        PrintSz("No transits found.\n");

    /* Recompute original chart placements as we've overwritten them. */

    ciCore = ciMain;
    ciTran = ciT;
    us.fProgress = fFalse;
    CastChart(fTrue);
    fEclipseBackward=fEclipseBackwardT;
    us.fProgress=fProgressT;
}


/* Display a list of planetary rising times relative to the local horizon */
/* for the day indicated in the chart information, as specified with the  */
/* -Zd switch. For the day, the time each planet rises (transits horizon  */
/* in East half of sky), sets (transits horizon in West), reaches its     */
/* zenith point (transits meridian in South half of sky), and nadirs      */
/* transits meridian in North), is displayed.                             */

void ChartInDayHorizon()
{
    char sz[cchSzDef];
    int source[MAXINDAY], type[MAXINDAY], sign[MAXINDAY],
    fRet[MAXINDAY], occurcount, division, div, s1, i, j,jjj, kk,ll,jj,fT;
    real time[MAXINDAY], rgalt1[objMax], rgalt2[objMax], azialt[MAXINDAY],terran[FastHi-FastLo],
    azi1, azi2, alt1, alt2, aalt1,aalt2, horiz, lon, lat, mc1, mc2, xA, yA, xV, yV, d, k,s2,azia,se;
    CI ciT;
    byte EquT, MCpolarT;

    if (us.nHouseSystem==2||us.nHouseSystem==17||(us.nHouseSystem!=9&&us.nHouseSystem!=6&&us.nHouseSystem!=15&&us.nHouseSystem!=16&&RAbs(Lat)>=66.0)) {
        sprintf(sz,"Warning, try another housesystem\n");
        PrintSz(sz);
        sprintf(sz,"As it is now bogus results will be displayed\n");
        PrintSz(sz);
    }
    for (i=FastLo; i<=FastHi; i++)
        terran[i-FastLo+1]=planet[i];
    fT = us.fSidereal;
    us.fSidereal = fFalse;
    EquT = us.fEquator;
    us.fEquator = fFalse;
    MCpolarT = PolarMCflip;
    PolarMCflip = fFalse;
    lon = RFromD(Mod(DecToDeg(Lon)));
    lat = RFromD(DecToDeg(Lat));
    division = us.nDivision * 4;
    occurcount = 0;

    ciT = ciTwin;
    ciCore = ciMain;
    ciCore.tim = 0.0;
    CastChart(fTrue);
    mc2 = RFromD(planet[PolarMCflip ? oNad : oMC]);
    k = RFromD(planetalt[oMC]);
    for (i=FastLo; i<=FastHi; i++)
        planet[i]=terran[i-FastLo+1];
    EclToEqu(&mc2, &k);

    cp2 = cp0;
    for (i = 1; i <= cObjOpt; i++) {
        rgalt2[i] = planetalt[i];
    }


    /* Loop through the day, dividing it into a certain number of segments. */
    /* For each segment we get the planet positions at its endpoints.       */

    for (div = 1; div <= division; div++) {
        ciCore = ciMain;
        ciCore.tim = DegToDec(24.0*(real)div/(real)division);
        CastChart(fTrue);
        mc1 = mc2;
        mc2 = RFromD(planet[PolarMCflip ? oNad: oMC]);
        k = RFromD(planetalt[oMC]);
        for (i=FastLo; i<=FastHi; i++)
            planet[i]=terran[i-FastLo+1];
        EclToEqu(&mc2, &k);
        cp1 = cp2;
        cp2 = cp0;
        for (i = 1; i <= cObjOpt; i++) {
            rgalt1[i] = rgalt2[i];
            rgalt2[i] = planetalt[i];
        }

        /* For our segment, check to see if each planet during it rises, sets, */
        /* reaches its zenith, or reaches its nadir.                           */

        for (i = 1; i <= cObjOpt; i++) if (!ignore[i] /*&& FThing(i))*/||i<=oMoo) {
                EclToHorizon(&azi1, &alt1, cp1.obj[i], rgalt1[i], lon, lat, mc1);
                EclToHorizon(&azi2, &alt2, cp2.obj[i], rgalt2[i], lon, lat, mc2);
                j = 0;
#ifdef PLACALC
                if (us.fRefraction && us.objCenter == oEar) {
                    aalt1=FPlacalcRefrac(i,alt1,0+2); /* Flag +2: consider sun/moon radius */
                    aalt2=FPlacalcRefrac(i,alt2,0+2); /* Flag 0 True to App Flag 1 App to True */
                    horiz=FPlacalcRefrac(i,0,1+2);
                } else
#endif
                {
                    horiz=0;
                    aalt1=alt1;
                    aalt2=alt2;
                }
                /* Check for transits to the horizon. */
                if ((aalt1 > 0) != (aalt2 > 0)) {
                    d = RAbs(alt1-horiz)/(RAbs(alt1-horiz)+RAbs(alt2-horiz));
                    k = Mod(azi1 + d*MinDifference(azi1, azi2));
                    j = 1 + 2*(MinDistance(k, rDegHalf) < rDegQuad);
                    /* Check for transits to the meridian. */
                } else if (RSgn(MinDifference(azi1, rDegQuad)) !=
                           RSgn(MinDifference(azi2, rDegQuad))) {
                    jjj = 2 + 2 * (MinDistance(azi1, rDegQuad) < rDegQuad);
                    j = 2 + 2*(alt1 < 0);
                    d = RAbs(azi1 - (jjj > 2 ? rDegQuad : 270.0))/MinDistance(azi1, azi2);
                    k = alt1 + d*(alt2-alt1);
                    if (MCpolarT && hRevers)
                        j = 2 + 2*(MinDistance(azi1, rDegQuad) > rDegQuad);
                }
                if (j && !ignorez[j-1] && !ignore[i] && occurcount < MAXINDAY) {
                    source[occurcount] = i;
                    type[occurcount] = j;
                    time[occurcount] = 24.0*((real)(div-1)+d)/(real)division*60.0;
                    sign[occurcount] = (int)Mod(cp1.obj[i] +
                                                d*MinDifference(cp1.obj[i], cp2.obj[i]))/30 + 1;
                    fRet[occurcount] = (int)RSgn(cp1.dir[i]) + (int)RSgn(cp2.dir[i]);
                    azialt[occurcount] = k;
                    ciSave = ciMain;
                    ciSave.tim = DegToDec(time[occurcount] / 60.0);
                    occurcount++;
                }
            }
        if ( occurcount >= MAXINDAY )
            PrintSz("Overflow of event counter\n");
    }

    /* Sort each event in order of time when it happens during the day. */

    for (i = 1; i < occurcount; i++) {
        j = i-1;
        while (j >= 0 && time[j] > time[j+1]) {
            SwapN(source[j], source[j+1]);
            SwapN(type[j], type[j+1]);
            SwapR(&time[j], &time[j+1]);
            SwapN(sign[j], sign[j+1]);
            SwapN(fRet[j], fRet[j+1]);
            SwapR(&azialt[j], &azialt[j+1]);
            j--;
        }
    }

    /* Finally display the list showing each event and when it occurs. */
    if (us.fParans) {
        for (kk=0; kk<=cObjOpt; kk++) if (!FIgnoreS(kk)) {
                for (jj=1; jj<=4; jj++) if (!ignorez[jj-1]) {
                        for (i = 0; i <= occurcount; i++) if (source[i]==kk&&type[i]==jj) {
                                ll=i-1;
                                while ((time[i]-time[ll])<40.0&&ll>=0) {
                                    ll--;
                                }
                                while ((time[ll]-time[i])<40.0) {
                                    if (type[ll]==type[i]&&i!=ll&&ll<=occurcount&&(RAbs(time[i]-time[ll])/4.0)<GetOrb(source[i],source[ll],aCon)) {
                                        AnsiColor(kDefault);
                                        s1 = (int)time[i]/60;
                                        s2 = time[i]-s1*60.0;
                                        sprintf(sz, "%s %s ", SzDate(Mon, Day, Yea, 2*MonthFormat), SzTime(s1, (int)s2,(int)(rRound+RFract(s2)*60.0)));
                                        PrintSz(sz);
                                        AnsiColor(kObjA[source[i]]);
                                        sprintf(sz, "%7.7s ", szObjName[source[i]]);
                                        PrintSz(sz);
                                        AnsiColor(kSignA(sign[i]));
                                        sprintf(sz, "%c%c%c%c%c ",
                                                fRet[i] > 0 ? '(' : (fRet[i] < 0 ? '[' : '<'), chSig3(sign[i]),
                                                fRet[i] > 0 ? ')' : (fRet[i] < 0 ? ']' : '>'));
                                        PrintSz(sz);
                                        AnsiColor(kElemA[type[i]-1]);
                                        if (type[i] == 1)
                                            PrintSz("rises     with ");
                                        else if (type[i] == 2)
                                            PrintSz("culm.(up) with ");
                                        else if (type[i] == 3)
                                            PrintSz("sets      with ");
                                        else
                                            PrintSz("culm.(lo) with ");
                                        AnsiColor(kObjA[source[ll]]);
                                        sprintf(sz, "%7.7s ", szObjName[source[ll]]);
                                        PrintSz(sz);
                                        AnsiColor(kSignA(sign[ll]));
                                        sprintf(sz, "%c%c%c%c%c ",
                                                fRet[ll] > 0 ? '(' : (fRet[ll] < 0 ? '[' : '<'), chSig3(sign[ll]),
                                                fRet[ll] > 0 ? ')' : (fRet[ll] < 0 ? ']' : '>'));
                                        PrintSz(sz);
                                        AnsiColor(kDefault);
                                        s1 = (int)(RAbs(time[i]-time[ll]))/60;
                                        s2 = RAbs(time[i]-time[ll])-s1*60.0;
                                        sprintf(sz,"%s %s\n",3+SzTime(s1,(int)s2,(int)(rRound+RFract(s2)*60.0)),((time[ll]-time[i])>0) ? "late" : "prev" );
                                        PrintSz(sz);
                                    }
                                    ll++;
                                }
                            }
                    }
            }
    }
    else {
        for (i = 0; i < occurcount; i++) {
            ciSave = ciMain;
            ciSave.tim = DegToDec(time[i] / 60.0);
            j = DayOfWeek(Mon, Day, Yea);
            AnsiColor(kRainbowA[j + 1]);
            sprintf(sz, "(%c%c%c) ", chDay3(j));
            PrintSz(sz);
            AnsiColor(kDefault);
            s1 = (int)time[i]/60;
            s2 = time[i]-s1*60;
            sprintf(sz, "%s %s ", SzDate(Mon, Day, Yea, 2*MonthFormat), SzTime(s1, (int)s2,(int)(rRound+RFract(s2)*60.0)));
            PrintSz(sz);
            AnsiColor(kObjA[source[i]]);
            sprintf(sz, "%7.7s ", szObjName[source[i]]);
            PrintSz(sz);
            AnsiColor(kSignA(sign[i]));
            sprintf(sz, "%c%c%c%c%c ",
                    fRet[i] > 0 ? '(' : (fRet[i] < 0 ? '[' : '<'), chSig3(sign[i]),
                    fRet[i] > 0 ? ')' : (fRet[i] < 0 ? ']' : '>'));
            PrintSz(sz);
            AnsiColor(kElemA[type[i]-1]);
            if (type[i] == 1)
                PrintSz("rises    ");
            else if (type[i] == 2)
                PrintSz("culm.(up)");
            else if (type[i] == 3)
                PrintSz("sets     ");
            else
                PrintSz("culm.(lo) ");
            AnsiColor(kDefault);
            PrintSz(" at ");
            if (type[i] & 1) {
                j = (int)(RFract(azialt[i])*60.0);
                if (fNESW)
                    azia = Mod(rDegQuad - azialt[i]);
                else
                    azia = azialt[i];
                j = (int)(RFract(azia)*60.0);
                se = RFract(azia)*60.0;
                se = RFract(se)*60.0;
                if (!us.fSeconds)
                    sprintf(sz, "%3d%c%02d'", (int)azia, chDeg1, j);
                else
                    sprintf(sz, "%3d%c%02d'%02d\"", (int)azia, chDeg1, j, (int)se);
                PrintSz(sz);

                /* For rising and setting events, we'll also display a direction  */
                /* vector to make the 360 degree azimuth value thought of easier. */

                xA = RCosD(azialt[i]);
                yA = RSinD(azialt[i]);
                if (RAbs(xA) < RAbs(yA)) {
                    xV = RAbs(xA / yA);
                    yV = 1.0;
                } else {
                    yV = RAbs(yA / xA);
                    xV = 1.0;
                }
                sprintf(sz, " (%.2f%c %.2f%c)",
                        yV, yA < 0.0 ? 's' : 'n', xV, xA > 0.0 ? 'e' : 'w');
                PrintSz(sz);
            } else
                PrintAltitude(azialt[i]);
            PrintL();
        }
    }
    if (occurcount == 0)
        PrintSz("No horizon events found.\n");

    /* Recompute original chart placements as we've overwritten them. */

    ciCore = ciMain;
    ciTwin = ciT;
    us.fSidereal = fT;
    us.fEquator = EquT;
    PolarMCflip = MCpolarT;
    CastChart(fTrue);
}


/* Print out an ephemeris - the positions of the planets (at the time in the */
/* current chart) each day during a specified month, as done with the -E     */
/* switch. Display the ephemeris for the whole year if -Ey is in effect.     */

void ChartEphemeris()
{
    char sz[cchSzDef];
    int yea, yea1, yea2, mon, mon1, mon2, daysiz, i, j, s, d, m;

    /* If -Ey is in effect, then loop through all months in the whole year. */

    if (us.nEphemYears) {
        yea1 = Yea;
        yea2 = yea1 + us.nEphemYears - 1;
        mon1 = 1;
        mon2 = 12;
    } else {
        yea1 = yea2 = Yea;
        mon1 = mon2 = Mon;
    }

    /* Loop through the year or years in question. */

    for (yea = yea1; yea <= yea2; yea++)

        /* Loop through the month or months in question, printing each ephemeris. */

        for (mon = mon1; mon <= mon2; mon++) {
            if (us.fVerboseHeader) {
                PrintVerboseHeader();
                sprintf(sz, "%s Positions at %s (%cT %s GMT)\n",
                        us.fParallel ? ((us.fEquator&&!us.fGalPlane ) ? "Declination" : "Latitude") :"Longitude" ,
                                SzTim(Tim), ChDst(Dst), SzZone(Zon));
                PrintSz(sz);
            }
            daysiz = DayInMonth(mon, yea);
            PrintSz(us.fEuroDate ? "Dy/Mo/Yr" : "Mo/Dy/Yr");
            for (j = 1; j <= cObjOpt; j++) {
                if (!ignore[j] && FThing(j)) {
                    sprintf(sz, "  %s%c%c%c%c", is.fSeconds ? "  " : "", chObj3(j),
                            szObjName[j][3] != 0 ? szObjName[j][3] : ' ');
                    PrintSz(sz);
                    PrintTab(' ', us.fParallel ? 2 + is.fSeconds : 1 + 3*is.fSeconds);
                }
            }
            PrintL();

            for (i = 1; i <= daysiz; i = AddDay(mon, i, yea, 1)) {

                /* Loop through each day in the month, casting a chart for that day. */

                SetCIA(ciCore, mon, i, yea, Tim, Dst, Zon, Lon, Lat,Alt);
                CastChart(fTrue);
                PrintSz(SzDate(mon, i, yea, 0));
                PrintCh(' ');
                for (j = 0; j <= cObjOpt; j++)
                    if (!FIgnore(j) && FThing(j)) {
                        if (!us.fParallel) {
                            if (is.fSeconds)
                                PrintZodiac(planet[j]);
                            else {
                                AnsiColor(kObjA[j]);
                                s = SFromZ(planet[j]);
                                d = (int)planet[j] - (s-1)*30;
                                m = (int)(RFract(planet[j])*60.0);
                                sprintf(sz, "%2d%s%02d", d, szSignAbbrev[s], m);
                                PrintSz(sz);
                            }
                        } else {
                            AnsiColor(kObjA[j]);
                            PrintAltitude(planetalt[j]);
                        }
                        PrintCh((byte)(ret[j] >= 0.0 ? ' ' : '.'));
                    }
                PrintL();
                AnsiColor(kDefault);
            }
            if (mon < mon2 || yea < yea2)
                PrintL();
        }

    ciCore = ciMain;    /* Recast original chart. */
    CastChart(fTrue);
}

/* charts3.c */
