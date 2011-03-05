/*
** Astrolog (Version 5.42J) File: charts4.c
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
** Modifications from version 5.41 to 5.42 are by Nicolas Scharnagl

*/
#include <string.h>
#include <ctype.h>
#include "astrolog.h"
#include "config.h"
#include "swephexp.h"
/* Added by N.Scharnagl */
void ChartEclipse()
{
    double jd;
    int i,count;
    jd=MdytszToJulian(Mon,Day,Yea,Tim,Dst,Zon)-0.5;
#ifdef PLACALC
    /*    jd=julday(Mon,Day,Yea,DegToDec(Tim),1);*/
    count=us.nEclipseCount;
    if (fOccultAll) {
        while (count>0) {
            jd=FPlacalcEclipseWhenList(&count,jd,(long)4,fEclipseBackward);
        }
    } else
        for (i=1; i<=us.nEclipseCount; i++) {
            jd=FPlacalcEclipseWhen(us.nEclObj,jd,(long)4,fEclipseBackward);
        }
#endif
}
/* Added by N.Scharnagl */
void ChartReturn()
{
    double jd;
    int i,day;
    char sz[cchSzDef];
    CI ciT;
#ifdef PLACALC
    for (i=1; i<=us.nEclipseCount; i++) {
        ciT=ciCore;
        ciCore=ciTwin;
        if (fSynodicReturnChart) {
            sprintf(sz, "%s-%s synodic return %9s %4d  ",us.nEclObj==oSun ? "Solar" : us.nEclObj==oMoo ? "Lunar" : szObjName[us.nEclObj],
                    us.nEclObj2==oSun ? "Solar" : us.nEclObj2==oMoo ? "Lunar" : szObjName[us.nEclObj2], szMonth[MM]
                    ,YY);
            PrintSz(sz);
        }
        else {
            sprintf(sz, "%s %s %9s %4d  ",us.nEclObj==oSun ? "Solar" : us.nEclObj==oMoo ? "Lunar" : szObjName[us.nEclObj], force[us.nEclObj]? "Ingress" : "Return" ,szMonth[MM] ,YY);
            PrintSz(sz);
        }
        day = DayOfWeek(MM, DD, YY);
        sprintf(sz, "%c%c%c %s %s (%cT %s GMT)", chDay3(day),
                SzDate(MM, DD, YY, 3), us.fSeconds ? SzTim(TT) : SzTim(TT), ChDst(SS),
                SzZone(ZZ));
        PrintSz(sz);
        PrintL();
        jd=MdytszToJulian(MM,DD,YY,TT,SS,ZZ)-0.5;
        jd+=fEclipseBackward ? -1 : 1;
        JulianToMdy(jd,&MM,&DD,&YY);
        TT=DegToDec(RFract(jd-0.5)*24.0);
        SS=ZZ=0;
        ciTwin=ciCore;
        ciCore=ciT;
        fSynodicReturnChart ? ChartSynodicReturnSet() : ChartReturnSet();
    }
#endif
}
/* Added by N.Scharnagl */
void ChartEclipseSet()
{
    double jd;
    jd=MdytszToJulian(MM, DD, YY, TT, SS, ZZ)-0.5;
#ifdef PLACALC
    jd=FPlacalcEclipseWhen(us.nEclObj,jd,(long)4,fFalse);
    JulianToMdy(jd,&MM,&DD,&YY);
    TT=DegToDec(RFract(jd-0.5)*24.0);
    SS=ZZ=0;
#endif
}

/* Added by N.Scharnagl */
void ChartReturnSet()
{
    double pos,prevpos,jd,xx[6],f1,f2,divsiz,divsiz0;
    int found=fFalse;
    int obj;
    char sz[cchSzDef];
    CI ciT;
    obj=us.nEclObj;
    if (returnnatalpos < 0) {
        if (force[obj]==0.0) {
            jd=MdytszToJulian(MM, DD, YY, TT, SS, ZZ)-0.5;
            FPlacalcPlanet(obj,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
            returnnatalpos=xx[0];
        } else returnnatalpos=Mod(force[obj]);
    }
    ciT=ciCore;
    ciCore=ciTwin;
    if (us.objCenter > oSun) {
        sprintf(sz,"Warning, incorrect calculation with center object %s\n",szObjName[us.objCenter]);
        PrintSz(sz);
    }
    jd=MdytszToJulian(MM, DD, YY, TT, SS, ZZ)-0.5;
    FPlacalcPlanet(obj,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
    pos=xx[0];
    divsiz0=divsiz=(us.nEclObj==oMoo) ? 1 : 2.5*sqrt(xx[3]);
    if (fEclipseBackward)
        divsiz*=-1;
    while (!found) {
        prevpos=pos;
recalc:
        FPlacalcPlanet(obj,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
        pos=xx[0];
        f1=prevpos-returnnatalpos;
        f2=pos-returnnatalpos;
        if (RAbs(f1) > rDegHalf)
            f1 -= RSgn(f1)*rDegMax;
        if (RAbs(f2) > rDegHalf)
            f2 -= RSgn(f2)*rDegMax;
        if (us.fDebug > 3)  {
            sprintf(sz,"%f %f %f %f %f \n",returnnatalpos,jd,pos,OO,AA);
            PrintSz(sz);
        }
        if (MinDistance(returnnatalpos, Midpoint(prevpos, pos)) < rDegQuad && RSgn(f1) != RSgn(f2)) {
            if (RAbs(divsiz) >= divsiz0/(625*15625.0)) {
                jd-=divsiz;
                divsiz =divsiz/25.0;
                jd+=divsiz;
                goto recalc;
            } else {
                jd+=RAbs(f1)/(RAbs(f1)+RAbs(f2))*divsiz-divsiz;
                found=fTrue;
            }
        }
        else
            jd+=divsiz;
    }
    JulianToMdy(jd,&MM,&DD,&YY);
    TT=DegToDec(RFract(jd-0.5)*24.0);
    SS=ZZ=0;
    ciTwin=ciCore;
    ciCore=ciT;
}

/* Added by N.Scharnagl */
void ChartSynodicReturnSet()
{
    double pos,prevpos,jd,xx[6],f1,f2,divsiz,divsiz0;
    int found=fFalse;
    int obj1,obj2;
    char sz[cchSzDef];
    CI ciT;
    obj1=us.nEclObj;
    obj2=us.nEclObj2;
    if (returnnatalpos < 0) {
        jd=MdytszToJulian(MM, DD, YY, TT, SS, ZZ)-0.5;
        if (force[obj1]==0.0) {
            FPlacalcPlanet(obj1,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
            returnnatalpos=xx[0];
        }
        else returnnatalpos=force[obj1];
        if (force[obj2]==0.0) {
            FPlacalcPlanet(obj2,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
            returnnatalpos -= xx[0];
        }
        else returnnatalpos -=force[obj2];
        returnnatalpos=Mod(returnnatalpos);
    }
    ciT=ciCore;
    ciCore=ciTwin;
    if (us.objCenter > oSun) {
        sprintf(sz,"Warning, incorrect calculation with center object %s\n",szObjName[us.objCenter]);
        PrintSz(sz);
    }
    jd=MdytszToJulian(MM, DD, YY, TT, SS, ZZ)-0.5;
    FPlacalcPlanet(obj1,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
    pos=xx[0];
    FPlacalcPlanet(obj2,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
    pos -= xx[0];
    pos=Mod(pos);
    divsiz0=divsiz=(us.nEclObj==oMoo||us.nEclObj2==oMoo) ? 1 : 2.5*sqrt(xx[3]);
    if (fEclipseBackward)
        divsiz*=-1;
    while (!found) {
        prevpos=pos;
recalc2:
        FPlacalcPlanet(obj1,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
        pos=xx[0];
        FPlacalcPlanet(obj2,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
        pos -= xx[0];
        pos=Mod(pos);
        f1=prevpos-returnnatalpos;
        f2=pos-returnnatalpos;
        if (us.fDebug > 2)  {
            sprintf(sz,"%f %f %f %f %f \n",returnnatalpos,prevpos,pos,f1,f2);
            PrintSz(sz);
        }
        if (RAbs(f1) > rDegHalf)
            f1 -= RSgn(f1)*rDegMax;
        if (RAbs(f2) > rDegHalf)
            f2 -= RSgn(f2)*rDegMax;
        if (MinDistance(returnnatalpos, Midpoint(prevpos, pos)) < rDegQuad && RSgn(f1) != RSgn(f2)) {
            if (RAbs(divsiz) >= divsiz0/15625.0) {
                jd-=divsiz;
                divsiz =divsiz/25.0;
                jd+=divsiz;
                goto recalc2;
            } else {
                found=fTrue;
                jd+=RAbs(f2)/(RAbs(f1)+RAbs(f2))*divsiz-divsiz;
            }
        }
        else
            jd+=divsiz;
    }
    JulianToMdy(jd,&MM,&DD,&YY);
    TT=DegToDec(RFract(jd-0.5)*24.0);
    SS=ZZ=0; /* return this in ciCore */
    ciTwin=ciT;
}
/* Added by N.Scharnagl */
void ChartPrimaryDirectionSet()
{
    double time0,pos,arc,prpos,jd,jd0,jdp,xx[6],f1,f2,divsiz,divsiz0,sector,prevsector;
    int found=fFalse,revcount=0,raisecount=0,stepcount=0;
    int i,obj1,obj2;
    char sz[cchSzDef],ignoreT[objMax+1];
    CI ciT;

    obj1=us.nEclObj;
    obj2=us.nEclObj2;
    ciT=ciCore;
    for (i=1; i<=objMax; i++)
    {
        ignoreT[i]=ignore[i];
        ignore[i]=1;
    }
    ignore[obj1]=ignore[obj2]=0;
    time0=TT;
/*    jd0=jd=MdytszToJulian(MM,DD,YY,TT,SS,ZZ)-rRound;*/
    CastChart(fTrue);
    jd0=jd=is.JDr;
            rSidDay=rDegQuad/Mod(MinDistance(15.0*swe_sidtime(is.JDr),15.0*swe_sidtime(is.JDr +(fEclipseBackward ? -rRound/2.0 : rRound/2.0))));
    if (us.fSweHouse) {
        prpos=370-10*disret[obj1];
        returnnatalpos = 370-10*disret[obj2];
    }
    else
    {
        CastSectors();
        prpos = planet[obj1];
        returnnatalpos = planet[obj2];
    }
   
          if (us.fParallel){
            returnnatalpos=-rDegRad*RSinD(returnnatalpos);
} else {
    returnnatalpos=Mod(returnnatalpos);}
    pos = DecToDeg(-OO);
    if (us.objCenter > oSun) {
        sprintf(sz,"Warning, incorrect calculation with center object %s\n",szObjName[us.objCenter]);
        PrintSz(sz);
    }

            ciCore=ciT;
    arc = Mod(rDegHalf+prpos - returnnatalpos)-rDegHalf;
    if (fSecMotion) {
        jd = jd0-rSidDay*arc/360.0;
            JulianToMdy(jd,&MM,&DD,&YY);
            TT=DegToDec(RFract(jd-0.5)*24.0);
            SS=ZZ=0;
    } else {
        OO=-DegToDec(Mod(pos-arc+rDegHalf)-rDegHalf);
    }
    if (us.fSweHouse) {
        if (fGauqMode >= 6) /* use eph recalc */
        {
            FPlacalcPlanet(obj1,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
            sector=370-10.0*xx[5];
        } else {
            CastChart(fTrue);
            sector=Mod(370-10.0*disret[obj1]);
        }
    } else {
        CastChart(fTrue);
        CastSectors();
        sector=planet[obj1];
    }

    divsiz= RSgn(Mod(rDegHalf/*+arc*/+returnnatalpos-sector)-rDegHalf)*5;
    divsiz0 = RAbs(divsiz);
          if (us.fParallel){
            sector=-rDegRad*RSinD(sector);
} 
    if (fEclipseBackward)
        divsiz*=-1;
    while (!found) {
        prevsector=sector;
recalc3:
        arc=Mod(rDegHalf+arc)-rDegHalf;
        if (us.fSweHouse) {
            ciCore=ciT;
            if (fSecMotion) {
                jd = jd0 - rSidDay*arc/360.0;
                JulianToMdy(jd,&MM,&DD,&YY);
                TT=DegToDec(RFract(jd-0.5)*24.0);
                SS=ZZ=0;
            } else {
                OO=-DegToDec(Mod(pos-arc+rDegHalf)-rDegHalf);
            }
            if (fGauqMode >= 6)
            {
                FPlacalcPlanet(obj1,jd, us.objCenter != oEar,&xx[0],&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
                sector=370-10*xx[5];
            }
            else {
                CastChart(fTrue);
                /*    FPlacalcHouses(jd,fTrue); */
                sector=Mod(370-10.0*disret[obj1]);
            }
        }
        else {
            ciCore=ciT;
            if (fSecMotion) {
                jd = jd0 - rSidDay*arc/360.0;
                JulianToMdy(jd,&MM,&DD,&YY);
                TT=DegToDec(RFract(jd-0.5)*24.0);
                SS=ZZ=0;
            } else {
                OO=-DegToDec(Mod(pos-arc+rDegHalf)-rDegHalf);
            }
            CastChart(fTrue);
            CastSectors();
            sector=planet[obj1];
        }
        sector=Mod(sector);
          if (us.fParallel){
            sector=-rDegRad*RSinD(sector);
} 
        f1=prevsector-returnnatalpos;
        f2=sector-returnnatalpos;
        if (us.fDebug > 3)  {
            sprintf(sz,"%f %f %f %f %f %f \n",arc,returnnatalpos,sector,prevsector,f1,f2);
            PrintSz(sz);
        }
        if (RAbs(f1) > rDegHalf)
            f1 -= RSgn(f1)*rDegMax;
        if (RAbs(f2) > rDegHalf)
            f2 -= RSgn(f2)*rDegMax;
        if (MinDistance(Midpoint(prevsector,sector), returnnatalpos) < rDegQuad && RSgn(f1) != RSgn(f2)) {
            if (RAbs(divsiz) >= divsiz0/(15625.0)) {
                arc-=divsiz;
                divsiz =divsiz/5.0;
                arc+=divsiz;
                goto recalc3;
            } else {
                found=fTrue;
                arc-=RAbs(f2)/(RAbs(f1)+RAbs(f2))*divsiz;
            }
        }
        else {
            if (RAbs(f2)>1+RAbs(f1)&&revcount < 5) {
                divsiz*=-1;
                revcount++;
            }
            if (RAbs(f2-f1)<0.1*RAbs(divsiz)&&raisecount< 5) {
                divsiz*=2;
                raisecount++;
                if (raisecount >=5) {
                    PrintWarning("Nothing found.");
                    return;
                }
            }
            arc+=divsiz;
            stepcount++;
            if ((real)stepcount*RAbs(divsiz)>1000.0) {
                PrintWarning("Nothing found.");
                return;
            }
        }
    }
    if (us.fDebug)
    {
        sprintf(sz,"Arc is %f degrees\n",arc);
        PrintSz(sz);
    }
    ciCore=ciT;
    jdp=ArcToProg(jd0,-arc);
                JulianToMdy(jdp,&MM,&DD,&YY);
                TT=DegToDec(RFract(jdp-0.5)*24.0);
                SS=ZZ=0;
    if (us.fDebug>5)
    {
        sprintf(sz,"Arc back is %f degrees\n",ProgToArc(jd0,jdp));
        PrintSz(sz);
        sprintf(sz,"Dates jd0 %f jdp %f JD\n",jd0,jdp);
        PrintSz(sz);
    }
    ciTwin=ciCore;
    returnnatalpos=  -arc ; /* for PrintHeader */
    for (i=1; i<=objMax; i++)
    {
        ignore[i]=ignoreT[i];
    }
    CastChart(fTrue);
    cp1=cp0;
    us.nRel=rcDual;
    ciCore=ciT;
}
/* Added by N.Scharnagl */

void ChartNodeListing()
{
#ifdef PLACALC
    double jd,r1,r2,r3,r4,r5,r6,rT,lDis,mDis;
    int i,k,fNam,fLoc,lll,nn,ll,sign;
    char sz[cchSzDef];

    PrintHeader();
    /* jd=julday(Mon,Day,Yea,DegToDec(Tim),1);*/
    jd=MdytszToJulian(Mon,Day,Yea,Tim,Dst,Zon)-rRound;
    fNam = *ciMain.nam > chNull;
    fLoc = *ciMain.loc > chNull;
    AnsiColor(kWhite);
    if (us.fSeconds) {
        sprintf(sz, "Body                  Location   Ret. %s     House    Velocity  Distance\n",
                us.fEquator ? "Declin. " : "Latitude");
        PrintSz(sz);
    } else {
        sprintf(sz,
                "Body                  Locat. Ret.%s.      House   Veloc.  Dist. %s Houses.\n",
                us.fEquator ? "Decl" : "Lati", szSystem[us.nHouseSystem]);
        PrintSz(sz);
    }
    if (!fNam && !fLoc)
        PrintL();

    /* Ok, now print out the location of each object. */
    for (i = us.fNodeApPeri ? oEar : us.fNodeFocal ? oEar : oMoo; i <= AsteHi; i++) {
        if (!FIgnoreS(i) && !FIgnore(i) && !FBetween(i,oNod,TerranHi) && (i!=oMoo || us.objCenter==oEar) ) {
            AnsiColor(kObjA[i]);
            sprintf(sz,
                    us.fNodeFocal ? "%s of %-13.13s" :
                    us.fNodeApPeri ? ( us.fNodeDscPeri ? ( i==oMoo ? "%s of %-11.11s"
                                                           : "%s of %-11.11s")
                                               : ( i==oMoo ? "%s of %-12.12s"
                                                   : "%s of %-13.13s" )) : "%s of %-10.10s",
                            us.fNodeFocal ? "Focal" :
                            us.fNodeApPeri ? ( us.fNodeDscPeri ? ( i==oMoo ? "Perigee" : "Perihel")
                                               : ( i==oMoo ? "Apogee" : "Aphel" ))
                            : (  us.fNodeDscPeri ? "Dsc.Node" : "Asc.Node" ),
                            szObjName[i]);
            PrintSz(sz);
            if (FPlacalcNode(i, jd, us.objCenter != oEar,
                             &r1, &r2, &r3, &r4, &r5,&r6)) {
                PrintZodiac(Mod(r1));
                sprintf(sz, " %c ", r3 >= 0.0 ? ' ' : chRet);
                PrintSz(sz);
                PrintAltitude(r2);
                k = HousePlaceIn(Mod(r1));
                AnsiColor(kSignA(k));
                sprintf(sz, "     [%2d%s]  ", k, szSuffix[k]);
                PrintSz(sz);
                AnsiColor(kDefault);
                PrintCh((byte)(r3 < 0.0 ? '-' : '+'));
                rT = RAbs(r3);
                sprintf(sz, us.fSeconds ? (rT < 10.0 ? "%9.7f" : rT < 100 ? "%9.6f" : "%9.5f" ) :
                                (rT < 10.0 ? "%5.3f" : rT<100 ? "%5.2f" : "%5.1f" ), rT);
                PrintSz(sz);
                sprintf(sz, " %5.2f AU",r4);
                PrintSz(sz);
            }
            PrintL();
            if (PartAspectsShow) {
                for (ll = 1; ll <= cObjOpt; ll++) {
                    if (!ignore[ll]) {
                        lDis = MinDistance(planet[ll], r1);
                        for (nn = 1; nn <=PartAspectsShow; nn++) {
                            if (!FAcceptAspect(i,nn,ll) )
                                continue;
                            mDis = lDis - rAspAngle[nn];
                            if (RAbs(mDis) < PartOrb) {
                                lll = (int) (3600*mDis);
                                PrintTab(' ', 33);
                                AnsiColor(kAspA[nn]);
                                sprintf(sz, " %s ",szAspectAbbrev[nn]);
                                PrintSz(sz);
                                sign=SFromZ(planet[ll]);
                                AnsiColor(kSignA(sign));
                                sprintf(sz, "%c%c%c%c%c ",
                                        ret[ll] > 0 ? '(' : (ret[ll] < 0 ? '[' : '<'), chSig3(sign),
                                        ret[ll] > 0 ? ')' : (ret[ll] < 0 ? ']' : '>'));
                                PrintSz(sz);
                                AnsiColor(kObjA[ll]);
                                sprintf(sz, "%-10.10s", szObjName[ll]);
                                PrintSz(sz);
                                sprintf(sz, us.fSeconds ? " orb: %c%d%c%02d'%02d\"" :
                                        " orb: %c%d%c%02d'",
                                        (mDis < 0 ? '-' : '+'),
                                        abs(lll)/3600, chDeg1, (abs(lll)%3600)/60 ,abs(lll)%60);
                                PrintSz(sz);
                                PrintL();
                            }
                        }
                    }
                }
            }

        }
    }
#endif
}

/* Added by N.Scharnagl */
void SetNodeNames(iobj)
int iobj;
{
    int obj;
    for (obj=oVtx; obj<=TerranHi; obj==oEP ? obj+=(TerranLo-oEP) : obj++) {
        if (iobj==oEar) {
            switch (obj) {
            case TerranLo:
                szObjName[obj]="Fortune";
                break;
            case TerranLo+1:
                szObjName[obj]="PolarAsc";
                break;
            case TerranLo+2:
                szObjName[obj]="ARMC";
                break;
            case TerranHi:
                szObjName[obj]="Coascendant";
                break;
            case oVtx:
                szObjName[obj]="Vertex";
                break;
            default:
                szObjName[obj]="EastPoint";
                break;
            }
        } else {
            switch (obj) {
            case TerranLo:
                if (iobj!=oSun)
                    szObjName[obj]="NNode";
                break;
            case TerranLo+1:
                if (iobj!=oSun)
                    szObjName[obj]="SNode";
                break;
            case TerranLo+2:
                szObjName[obj]="Aphel";
                break;
            case TerranHi:
                szObjName[obj]="Perih";
                break;
            case oEP:
                szObjName[obj]="Focal";
                break;
            default:
                szObjName[obj]=szObjName[iobj];
                break;
            }
            if (obj != oVtx)
                szObjName[obj]=strdup(strncat(strdup(szObjName[obj]),strdup(szObjName[iobj]),6));
        }
    }
}
/* Added by N.Scharnagl */
void SortRevRank(value, rank, size)
real *value;
int *rank, size;
{
    int h, i, j, k;

    for (i = 1; i <= size; i++)
        rank[i] = -1;
    for (h = 1; h <= size; h++) {
        if (size != cSign && (!FProper2(h)))
            continue;
        k=1;
        for (j = 1; j <= size; j++) {
            if (size != cSign && (!FProper2(j)))
                continue;
            if (value[j] > value[h])
                k++;
        }

        /* 'k' is the current position of the 'i'th place planet. */

        rank[k] = h;
    }
}
/* Added by N.Scharnagl */
void SortRevRank2(value, rank, size)
real *value;
int *rank, size;
{
    int h, i, j, k;
    real tempval;
    for (i = 1; i <= size; i++)
        rank[i] = -1;
    tempval = value[us.objCenter];
    value[us.objCenter] = value[0];
    value[0] = -1000.0;
    for (h = 1; h <= size; h++) {
        if (size != cSign && (!FProper2I(h)))
            continue;
        k = 1;
        for (j = 1; j <= size; j++) {
            if (size != cSign && (!FProper2I(j)))
                continue;
            if (value[j] > value[h])
                k++;
        }
        /* 'k' is the current position of the 'i'th place planet. */

        rank[k] = h;
    }
    rank[0] = rank[us.objCenter];
    value[0] = value[us.objCenter];
    value[us.objCenter] = tempval;
}

/* Added by N.Scharnagl */
int MinimumRank(value, size)
real *value;
int size;
{
    int j, k;
    real tempval;
    tempval = value[us.objCenter];
    value[us.objCenter] = value[0];
    value[0] = 1000.0;
    k = 0;
    for (j = 1; j <= size; j++) {
        if (size != cSign && (!FProper2I(j)||FStar(j)))
            continue;
        if (value[j] < value[k])
            k=j;
    }
    value[0] = value[us.objCenter];
    value[us.objCenter] = tempval;
    return k;
}

/* Added by N.Scharnagl */
byte *AtlasRequest(place,longi,latit,alti)
char *place;
double *longi , *latit ,*alti;
{
    FILE *file;
    char *name=" ",*sp,*sp2,*sp3,sz[cchSzDef],si[cchSzDef],*filename="atlas.txt";
    bool found=fFalse;
    double s;
    file=FileOpen(filename,1);
    if (file == NULL) {
        sprintf(sz, "File '%s' not found.", filename);
        PrintError(sz);
    }

    while (!found && (sp = fgets(si, cchSzDef, file)) != NULL) {
        while (*sp == ' ' || *sp == '\t'
                || *sp == '(' || *sp == '[' || *sp == '{')
            sp++;
        if (*sp == '#' || *sp == '\r' || *sp == '\n' || *sp == '\0')
            continue;
        if (strstr(place," "))
            sp2 = strstr(sp,"   ");
        else
            sp2 = strstr(sp, "  ");
        if (!memcmp(place,sp,strlen(place))) {
            if (us.nLocationCount<=1) {
                name=strpbrk(sp,"01234567890");
                sp3=strdup(sp);
                sp3+=name-sp-1;
                *sp3 = chNull;
                sp3-=name-sp-1;
                name=strdup(sp3);
                sp=sp2;
                while ((*sp != '-'||!FNumCh(*(sp+1))) && !FNumCh(*sp))
                    sp++;
                sp2 = strpbrk(sp," #\r\n");
                *sp2 = chNull;
                *latit=RParseSz(sp,pmLat);
                *sp2 = ' ';
                while (*sp != ' ' && *sp != '\t' )
                    sp++;
                while (*sp == ' ' || *sp == '\t'
                        || *sp == '(' || *sp == '[' || *sp == '{')
                    sp++;
                *longi=RParseSz(sp,pmLon);
                while (*sp != ' ' && *sp != '\t' && *sp != '\n' )
                    sp++;
                while (*sp == ' ' || *sp == '\t'
                        || *sp == '(' || *sp == '[' || *sp == '{')
                    sp++;
                s=RParseSz(sp,pmAlt);
                if (s!=rLarge)
                    *alti =s;
                if (us.nLocationCount >= 0)
                    found=fTrue;
                else  {
                    sprintf(sz,"%.30s\t%s \n",name,SzLocation(*longi,*latit));
                    PrintSz(sz);
                }
            } else {
                if (us.nLocationCount > 0)
                    us.nLocationCount--;
            }
        }
        continue;
    }
    fclose(file);
    if (found)
        return (byte *)name;
    else
        return (byte *)"";


}

void ChartInDaySearch3D(fProg)
bool fProg;
{
    char sz[cchSzDef];
    int source[MAXINDAY], dest[MAXINDAY], aspect[MAXINDAY],
    sign1[MAXINDAY], sign2[MAXINDAY], D1, D2, counttotal = 0, occurcount,
            division, div, fYear, yea0, year, yea1, yea2, i, j, k, l, s1,again=fFalse;
    real time[MAXINDAY], angle[MAXINDAY],divsiz, d1, d2, d3, e1, e2, e3, ad1,ad2, ad3, ae1,ae2,ae3,f1, f2, g=0, s2;
    real prevs;
    CI ciT;

    /* If parameter 'fProg' is set, look for changes in a progressed chart. */
    ciT = ciTran;
    fYear = us.fInDayMonth && (MonT == 0);
    division = (fYear || fProg) ? 1+us.nDivision/20 : us.nDivision;
    divsiz = 24.0 / (real)division*60.0;

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

#ifdef KDE
        KGetTZ(&Zon);
#endif

        /* Start searching the day or days in question for exciting stuff. */

        for (DayT = D1; DayT <= D2; DayT = AddDay(Mon, DayT, yea0, 1)) {
            occurcount = 0;

            /* Cast chart for beginning of day and store it for future use. */
            if (!again) {
                SetCIA(ciCore, fYear ? MonT : Mon, DayT, yea0, 0.0, Dst, Zon, Lon, Lat,Alt);
                if (fProg) {
                    is.JDp = MdydszToJulian(MonT, DayT, yea0, 0.0, Dst, Zon);
                    ciCore = ciMain;
                    us.fProgress = fTrue;
                }
                CastChart(fTrue);
                cp2 = cp0;
                AddTime(2,-Max((int)divsiz,1));
                if (fProg) {
                    is.JDp = MdydszToJulian(MonT, DayT, yea0, 0.0, Dst, Zon)-divsiz/1440.0;
                    ciCore = ciMain;
                    us.fProgress = fTrue;
                }
                CastChart(fTrue);
                cp1= cp0;
            }

            /* Now divide the day into segments and search each segment in turn. */
            /* More segments is slower, but has slightly better time accuracy.   */

            for (div = 1; div <= division; div++) {

                /* Cast the chart for the ending time of the present segment. The   */
                /* beginning time chart is copied from the previous end time chart. */

                SetCIA(ciCore, fYear ? MonT : Mon, DayT, yea0,
                       DegToDec(24.0*((real)div/(real)division)), Dst, Zon, Lon, Lat,Alt);
                if (fProg) {
                    is.JDp = MdydszToJulian(MonT, DayT, yea0, (24.0*(real)div/(real)division), Dst, Zon);
                    ciCore = ciMain;
                }
                CastChart(fTrue);
                cp3=cp1 ;
                cp1 = cp2;
                cp2 = cp0;

                /* Now search through the present segment for anything exciting. */

                for (i = 0; i <= cObjOpt; i++) if (!FIgnore(i) &&FThing(i))  {


                        /* Now search for anything making an extremal orb to the current planet. */

                        for (j = i+1; j <= cObjOpt; j++) if (!FIgnore(j) && !(FIgnoreS(i)&&FIgnoreS(j))) {
                                if (!FThing(j))
                                    continue;
                                d1 = cp1.obj[i];
                                d2 = cp2.obj[i];
                                d3 = cp3.obj[i];
                                e1 = cp1.obj[j];
                                e2 = cp2.obj[j];
                                e3 = cp3.obj[j];
                                ad1 = cp1.alt[i];
                                ad2 = cp2.alt[i];
                                ad3=cp3.alt[i];
                                ae1 = cp1.alt[j];
                                ae2 = cp2.alt[j];
                                ae3=cp3.alt[j];
                                f1 = MinDistance3D(d1,e1,ad1,ae1);
                                f2 = MinDistance3D(d2,e2,ad2,ae2);
                                prevs=f1-MinDistance3D(d3,e3,ad3,ae3);
                                if ((((f2-f1)<0.0)!=(prevs<0.0))&&(occurcount < MAXINDAY)) {
                                    source[occurcount] = i;
                                    aspect[occurcount]=((f2-f1)<0.0);
                                    dest[occurcount] = j;
                                    time[occurcount] = (RAbs(prevs)/(RAbs(prevs)+
                                                                     RAbs(f2-f1)))*2.0*divsiz + (real)(div-2.0)*divsiz;
                                    if (time[occurcount]<0) {
                                        time[occurcount] = (RAbs(prevs)/(RAbs(prevs)+
                                                                         RAbs(f2-f1)))*1.0*divsiz + (real)(div-1.0)*divsiz;
                                    }
                                    SetCIA(ciCore, fYear ? MonT : Mon, DayT, yea0,
                                           DegToDec(time[occurcount]/60.0), Dst, Zon, Lon, Lat,Alt);
                                    if (fProg) {
                                        is.JDp = MdydszToJulian(MonT, DayT, yea0, (time[occurcount]/60.0), Dst, Zon);
                                        ciCore = ciMain;
                                    }
                                    CastChart(fTrue);
                                    angle[occurcount] = MinDistance3D(planet[i],planet[j],planetalt[i],planetalt[j]);
                                    /*   if (((f2-angle[occurcount])<0.0!=(f2-f1)<0.0)){
                                          time[occurcount] = (RAbs(f2-f1)/(RAbs(f2-f1)+RAbs(f2-angle[occurcount])))*(div*divsiz-time[occurcount])+time[occurcount];
                                          } else {
                                          time[occurcount] = (RAbs(f2-f1)/(RAbs(f2-f1)+RAbs(f1-angle[occurcount])))*((1-div)*divsiz+time[occurcount])
                                                              +(real)(div-1)*divsiz;
                                          }
                                       SetCIA(ciCore, fYear ? MonT : Mon, DayT, yea0,
                                         DegToDec(time[occurcount]/60.0), Dst, Zon, Lon, Lat,Alt);
                                       if (fProg) {
                                         is.JDp = MdydszToJulian(MonT, DayT, yea0, (time[occurcount]/60.0), Dst, Zon);
                                         ciCore = ciMain;
                                       }
                                       CastChart(fTrue);
                                       angle[occurcount] = MinDistance3D(planet[i],planet[j],planetalt[i],planetalt[j]);*/
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
                        again=fTrue;
                    }
            }
            /* After all the aspects, etc, in the day have been located, sort   */
            /* them by time at which they occur, so we can print them in order. */

            for (i = 1; i < occurcount; i++) {
                j = i-1;
                while (j >= 0 && time[j] > time[j+1]) {
                    SwapN(source[j], source[j+1]);
                    SwapN(aspect[j], aspect[j+1]);
                    SwapR(&angle[j], &angle[j+1]);
                    SwapN(dest[j], dest[j+1]);
                    SwapR(&time[j], &time[j+1]);
                    SwapN(sign1[j], sign1[j+1]);
                    SwapN(sign2[j], sign2[j+1]);
                    j--;
                }
            }

            /* Finally, loop through and display each aspect and when it occurs. */

            for (i = 0; i < occurcount; i++) {
                j = DayT;
                year=yea0;
                if (time[i]<0.0) {
                    j--;
                    time[i]=time[i]+1440.0;
                }
                if (time[i]>1440.0) {
                    j++;
                    time[i]=time[i]-1440.0;
                }
                s1 = (int)time[i]/60;
                s2 = time[i]-s1*60;
                if (fYear || fProg)
                    l = MonT;
                else
                    l = Mon;
                while (j > (k = DayInMonth(l, yea0))) {
                    j -= k;
                    l++;
                }
                while (j < 1) {
                    j = DayInMonth(l-1,yea0);
                    l--;
                }
                while (l < 1) {
                    l=12;
                    year=year-1;
                    j=DayInMonth(l,year);
                }
                while (l > 12) {
                    l=1;
                    year=year+1;
                    j=1;
                }
                /*}*/
                SetCIA(ciSave, /*fYear || fProg ?*/ l /*: Mon*/, j, year,
                       DegToDec(time[i] / 60.0), Dst, Zon, Lon, Lat,Alt);
                k = DayOfWeek(/*fYear || fProg ?*/ l /*: Mon*/, j, year);
                AnsiColor(kRainbowA[k + 1]);
                sprintf(sz, "(%c%c%c) ", chDay3(k));
                PrintSz(sz);
                AnsiColor(kDefault);
                sprintf(sz, "%s %s ",
                        SzDate(/*fYear || fProg ?*/ l /*: Mon*/, j, year, 2*MonthFormat),
                        SzTime(s1, (int)s2,(int)(rRound+RFract(s2)*60.0)));
                PrintSz(sz);
                PrintAspect3D(source[i], sign1[i],
                              (int)RSgn(cp1.dir[source[i]])+(int)RSgn(cp2.dir[source[i]]),
                              angle[i],aspect[i],dest[i], sign2[i],
                              (int)RSgn(cp1.dir[dest[i]])+(int)RSgn(cp2.dir[dest[i]]),
                              (byte)(fProg ? 'e' : 'd'));
                PrintL();
            }
            counttotal += occurcount;
        }
    }
    if (counttotal == 0)
        PrintSz("No events found.\n");

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

void ChartTransitSearch3D(fProg)
bool fProg;
{
    real planet3[objMax], ret3[objMax], planet4[objMax],
    time[MAXINDAY],angle[MAXINDAY],planet5[objMax],planetalt3[objMax],planetalt4[objMax],planetalt5[objMax];
    char sz[cchSzDef];
    int source[MAXINDAY], aspect[MAXINDAY], dest[MAXINDAY], sign[MAXINDAY],isret[MAXINDAY],
    M1, M2, Y1, Y2, counttotal = 0, occurcount, division,
                                 div, fCusp, i, j, s1, s2, DayT2;
    real divsiz, daysiz, prevs, d,d1, d2, d3, e1, e2, e3, ad1,ad2,ad3,ae1,ae2,ae3,f1, f2,s3,DayT3;
    CI ciT;

    /* Save away natal chart and initialize things. */

    ciT = ciTran;
    for (i = 0; i <= cObjOpt; i++) {
        planet3[i] = planet[i];
        planet4[i] = planet[i];
        planet5[i] = planet[i];
        planetalt3[i] = planetalt[i];
        planetalt4[i] = planetalt[i];
        planetalt5[i] = planetalt[i];
        ret3[i] = ret[i];
    }
    if (fProg)
        fCusp = fFalse;
    else {
        fCusp = fTrue;
        for (i = cuspLo; i <= cuspHi; i++)
            fCusp &= ignore2[i];
    }
    division = us.nDivision;
    if (!fProg && !fCusp)
        division = Max(division, 192);


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

    for (YeaT = Y1; YeaT <= Y2; YeaT++)

        /* Start searching the month or months in question for any transits. */

        for (MonT = M1; MonT <= M2; MonT++) {
            daysiz = (real)(DayInMonth(MonT, YeaT)-DayT2+1)*24.0*60.0;
            divsiz = daysiz / (real)division;

            /* Progress chart if -pt switch and store it for future use. */
            /* For relationship charts it stays static */

            if (us.fTransProg && !us.nRel) {
                ciCore = ciMain;
                is.JDp = MdydszToJulian(MonT, DayT2, YeaT, 0.0, DstT, ZonT)-divsiz/1440.0;
                us.fProgress = fTrue;
                CastChart(fTrue);
                us.fProgress = fFalse;
                for (i = 0; i <= cObjOpt; i++) {
                    planet5[i] = planet4[i];
                    planet4[i] = planet3[i];
                    planet3[i] = planet[i];
                    planetalt5[i] = planetalt4[i];
                    planetalt4[i] = planetalt3[i];
                    planetalt3[i] = planetalt[i];
                    ret3[i] = ret[i];
                }
                ciCore = ciMain;
                is.JDp = MdydszToJulian(MonT, DayT2, YeaT, 0.0, DstT, ZonT);
                us.fProgress = fTrue;
                CastChart(fTrue);
                us.fProgress = fFalse;
                for (i = 0; i <= cObjOpt; i++) {
                    planet5[i] = planet4[i];
                    planet4[i] = planet3[i];
                    planet3[i] = planet[i];
                    planetalt5[i] = planetalt4[i];
                    planetalt4[i] = planetalt3[i];
                    planetalt3[i] = planetalt[i];
                    ret3[i] = ret[i];
                }
            }

            /* Cast chart for beginning of month and store it for future use. */
            SetCIA(ciCore, MonT, DayT2, YeaT, 0.0, DstT, ZonT, LonT, LatT,AltT);
            if (fProg) {
                is.JDp = MdydszToJulian(MM, DD, YY, 0.0, DstT, ZonT);
                ciCore = ciMain;
                us.fProgress = fTrue;
            }
            for (i = 0; i <= cObjOpt; i++)
                SwapN(ignore[i], ignore2[i]);
            if (fProg) {
                us.fProgress = fTrue;
                CastChart(fTrue);
                us.fProgress = fFalse;
            } else CastChart(fTrue);
            cp2 = cp0;
            AddTime(2,-divsiz-1);
            if (fProg) {
                is.JDp = MdytszToJulian(MM, DD, YY, TT, DstT, ZonT)-rRound;
                ciCore = ciMain;
                us.fProgress = fTrue;
                CastChart(fTrue);
                us.fProgress = fFalse;
            } else CastChart(fTrue);
            cp1 = cp0;
            for (i = 0; i <= cObjOpt; i++)
                SwapN(ignore[i], ignore2[i]);

            /* Divide our month into segments and then search each segment in turn. */

            for (div = 1; div <= division; div++) {
                occurcount = 0;

                /* Cast the chart for the ending time of the present segment, and */
                /* copy the start time chart from the previous end time chart.    */

                d = DayT3 + (daysiz/24.0/60.0)*(real)div/(real)division;

                /* Progress further if -pt switch */
                /* For relationship charts it stays static */
                if (us.fTransProg && !us.nRel ) {
                    ciCore = ciMain;
                    is.JDp = MdydszToJulian(MonT, (int)d, YeaT, (RFract(d)*24.0), DstT, ZonT);
                    us.fProgress = fTrue;
                    CastChart(fTrue);
                    us.fProgress = fFalse;
                    for (i = 0; i <= cObjOpt; i++) {
                        planet5[i] = planet4[i];
                        planet4[i] = planet3[i];
                        planet3[i] = planet[i];
                        planetalt5[i] = planetalt4[i];
                        planetalt4[i] = planetalt3[i];
                        planetalt3[i] = planetalt[i];
                        ret3[i] = ret[i];
                    }
                }

                SetCIA(ciCore, MonT, (int)d, YeaT,
                       DegToDec(RFract(d)*24.0), DstT, ZonT, LonT, LatT,AltT);
                if (fProg) {
                    is.JDp = MdydszToJulian(MM, DD, YY, RFract(d)*24.0, DstT, ZonT);
                    ciCore = ciMain;
                }
                for (i = 0; i <= cObjOpt; i++)
                    SwapN(ignore[i], ignore2[i]);
                if (fProg) {
                    us.fProgress = fTrue;
                    CastChart(fTrue);
                    us.fProgress = fFalse;
                } else CastChart(fTrue);
                for (i = 0; i <= cObjOpt; i++)
                    SwapN(ignore[i], ignore2[i]);
                cp3=cp1;
                cp1 = cp2;
                cp2 = cp0;

                /* Now search through the present segment for any transits. Note that */
                /* stars can be transited, but they can't make transits themselves.   */

                for (i = 0; i <= cObjOpt; i++) if (!FIgnore(i)) {
                        for (j = 0; j <= oNormOpt; j++) {
                            if (FIgnore2(j) || FStar(j)||(!FAst(j) && fCusp && !FThing(j)))
                                continue;

                            /* Between each pair of planets, check if they make any aspects. */

                            d1 = cp1.obj[j];
                            d2 = cp2.obj[j];
                            d3 = cp3.obj[j];
                            e1 = planet4[i];
                            e2 = planet3[i];
                            e3 = planet5[i];
                            ad1 = cp1.alt[j];
                            ad2 = cp2.alt[j];
                            ad3=cp3.alt[j];
                            ae1 = planetalt4[i];
                            ae2 = planetalt3[i];
                            ae3=planetalt5[i];

                            /* Check to see if the present aspect actually occurs during the */
                            /* segment       */
                            /* progressed moon may be faster than transit object */

                            f1 = MinDistance3D(d1,e1,ad1,ae1);
                            f2 = MinDistance3D(d2,e2,ad2,ae2);
                            prevs=f1-MinDistance3D(d3,e3,ad3,ae3);
                            if ((((f2-f1)<0.0)!=(prevs<0.0))&&(occurcount < MAXINDAY)) {
                                /* Ok, we have found a transit. Now determine the time */
                                /* and save this transit in our list to be printed.    */
                                source[occurcount] = j;
                                aspect[occurcount]=((f2-f1)<0.0);
                                dest[occurcount] = i;
                                time[occurcount] = (RAbs(prevs)/(RAbs(prevs)+
                                                                 RAbs(f2-f1)))*1.0*divsiz + (real)(div-1)*divsiz;
                                angle[occurcount] = f1;
                                sign[occurcount] = (int)(d1/30.0+1);
                                isret[occurcount] = (int)RSgn(cp1.dir[j]) +
                                                    (int)RSgn(cp2.dir[j]);
                                occurcount++;
                            }
                        }
                    }

                /* After all transits located, sort them by time at which they occur. */

                for (i = 1; i < occurcount; i++) {
                    j = i-1;
                    while (j >= 0 && time[j] > time[j+1]) {
                        SwapN(source[j], source[j+1]);
                        SwapN(aspect[j], aspect[j+1]);
                        SwapN(dest[j], dest[j+1]);
                        SwapR(&time[j], &time[j+1]);
                        SwapR(&angle[j], &angle[j+1]);
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
                    SetCIA(ciSave, MonT, s1+DayT2, YeaT, DegToDec((real)
                            ((_int)time[i]-s1*24*60) / 60.0), DstT, ZonT, LonT, LatT,AltT);
                    sprintf(sz, "%s %s ",
                            SzDate(MonT, s1+DayT2, YeaT, 2*MonthFormat), SzTime(s2, (int)s3,(int)(rRound+RFract(s3)*60.0))
                           );
                    PrintSz(sz);
                    PrintAspect3D(source[i], sign[i], isret[i], angle[i],aspect[i],
                                  dest[i], SFromZ(planet3[dest[i]]), (int)RSgn(ret3[dest[i]]),
                                  (byte)(fProg ? 'u'  : us.fTransProg ? 'x' : 't'));
                    PrintL();
                    AnsiColor(kDefault);
                }
                counttotal += occurcount;
            }
        }
    if (counttotal == 0)
        PrintSz("No transits found.\n");

    /* Recompute original chart placements as we've overwritten them. */

    ciCore = ciMain;
    ciTran = ciT;
    us.fProgress = fFalse;
    CastChart(fTrue);
}


void PrintAspect3D(obj1, sign1, ret1, angle, asp, obj2, sign2, ret2, chart)
int obj1, sign1, ret1, obj2, sign2, ret2;
double angle;
char chart;
{
    char sz[cchSzDef];
    double seconds;

    AnsiColor(kObjA[obj1]);
    if (chart == 't' || chart == 'T'||chart == 'x'||chart == 'X')
        PrintSz("tra ");
    else if (chart == 'e' || chart == 'u' || chart == 'U')
        PrintSz("pro ");
    sprintf(sz, "%9.9s", szObjName[obj1]);
    PrintSz(sz);
    AnsiColor(kSignA(sign1));
    sprintf(sz, " %c%c%c%c%c",
            ret1 > 0 ? '(' : (ret1 < 0 ? '[' : '<'), chSig3(sign1),
            ret1 > 0 ? ')' : (ret1 < 0 ? ']' : '>'));
    PrintSz(sz);
    AnsiColor(kWhite);
    PrintCh(' ');
    seconds=RFract(angle)*60.0;
    seconds=RFract(seconds)*60.0;
    sprintf(sz,"%s %3d%c%02d'%02d\"",(asp ? "Max" : "Min"), (int)angle,chDeg1,(int)(RFract(angle)*60.0) ,(int)seconds);
    PrintSz(sz);
    PrintCh(' ');
    if (chart == 'A')
        PrintSz("with ");
    AnsiColor(kSignA(sign2));
    if (chart == 't' || chart == 'u' || chart == 'T' || chart == 'U')
        PrintSz("nat ");
    else if (chart == 'x' || chart == 'X')
        PrintSz("pro ");
    sprintf(sz, "%c%c%c%c%c ",
            ret2 > 0 ? '(' : (ret2 < 0 ? '[' : '<'), chSig3(sign2),
            ret2 > 0 ? ')' : (ret2 < 0 ? ']' : '>'));
    PrintSz(sz);
    AnsiColor(kObjA[obj2]);
    sprintf(sz, "%.9s", szObjName[obj2]);
    PrintSz(sz);
    if (chart == 'D' || chart == 'T' || chart == 'U' ||
            chart == 'a' || chart == 'A' || chart == 'm' || chart == 'M' || chart =='X' || chart=='x')
        PrintTab(' ', 9-CchSz(szObjName[obj2]));
}


/* Added by N.Scharnagl */
void PrintVerboseHeader()
{
    char sz[cchSzDef];
    CI cit;
    double fract;
    sprintf(sz,"Delta T: %10.4f s    ",deltaT*86400.0);
    PrintSz(sz);
    PrintSz("Obliquity:    ");
    PrintAltitude(us.nEpsilon);
    PrintL();
    PrintSz("Positions ");
    if (us.fTopocentric&&us.objCenter==oEar) {
        sprintf(sz,"%s ","Topocentric");
        PrintSz(sz);
    }
    else if (us.objCenter==oEar) {
        sprintf(sz,"%s ","Geocentric");
        PrintSz(sz);
    }
    else if (us.objCenter==oSun) {
        sprintf(sz,"%s ","Heliocentric");
        PrintSz(sz);
    }
    else {
        sprintf(sz,"%s-%s",szObjName[us.objCenter],"centric");
    }
    if (us.fTruePos) {
        sprintf(sz,"%s ","True");
        PrintSz(sz);
    }
    else {
        sprintf(sz,"%s ","Apparent");
        PrintSz(sz);
    }
    if (us.fNoAberration) {
        sprintf(sz,"%s ","No Aberration");
        PrintSz(sz);
    }
    if (us.fNoGravityDeflection) {
        sprintf(sz,"%s ","No Gravity Deflection");
        PrintSz(sz);
    }
    if (us.fNoNutation) {
        sprintf(sz,"%s ","No Nutation");
        PrintSz(sz);
    }
    PrintSz("\nReference Frame ");
    if (us.fEquator) {
        if (us.fGalPlane) {
            sprintf(sz,"%s ","Lunar ");
            PrintSz(sz);
        }
        else {
            sprintf(sz,"%s ","Terran Equator");
            PrintSz(sz);
        }
    }
    else if (us.fGalPlane) {
        sprintf(sz,"%s ",us.fSidSolSysPlane ? "Supergalactic Plane" : "Galactic Plane");
        PrintSz(sz);
    }
    else if (us.fSidereal && us.fSidSolSysPlane && us.fSiderealMode < 25) {
        sprintf(sz,"%s ","Solar System Plane");
        PrintSz(sz);
    }
    else if (us.fHorizon) {
        sprintf(sz,"%s ","Local Horizon");
        PrintSz(sz);
    }
    else {
        sprintf(sz,"%s ","Ecliptic");
        PrintSz(sz);
    }
    PrintSz("\nReference Point Settings ");
    if (us.fSidereal) {
        if (us.fSiderealMode==255) {
            JulianToMdy(us.nSidEclDate,&cit.mon,&cit.day,&cit.yea);
            fract=RFract(us.nSidEclDate);
        }
        sprintf(sz,"%s %s %4.2f","Sidereal",us.fSiderealMode==255 ? SzDate(cit.mon,cit.day,cit.yea,2*MonthFormat) : swe_get_ayanamsa_name(us.fSiderealMode) , us.fSiderealMode==255 ? 24*(fract-0.5):  swe_get_ayanamsa(is.JD)) ;
        PrintSz(sz);
    }
    else if (!us.fEquator&&!us.fGalPlane&!us.fHorizon) {
        sprintf(sz,"%s ","Tropical");
        PrintSz(sz);
    }
    if (us.fGalPlane) {
        if (us.fEquator) {
            sprintf(sz,"%s ","Draconic");
            PrintSz(sz);
        }
        else if (us.fSidSolSysPlane) {
            sprintf(sz,"%s ","Supergalactic Zero/Galactic Node=0Can");
            PrintSz(sz);
        }
        else {
            sprintf(sz,"%s ","IAU Gal.Zero=0Cap");
            PrintSz(sz);
        }
    }
    if (us.fHorizon) {
        if (us.fPrimeVert) {
            sprintf(sz,"%s ","South-North/Zenith");
            PrintSz(sz);
        }
        else {
            if (fNESW) {
                sprintf(sz,"%s ","North counted eastwards");
                PrintSz(sz);
            }
            else {
                sprintf(sz,"%s ","East counted northwards");
                PrintSz(sz);
            }
        }
        PrintL();
        if (us.fRefraction) {
            sprintf(sz,"With refraction, pressure %7.1f hPa, Temperature %5.2f C",us.atpressDef,us.attempDef);
            PrintSz(sz);
        }
        else {
            sprintf(sz,"No refraction at horizon");
            PrintSz(sz);
        }
    }
    PrintSz("\nEphemeris Source requested:");
    if (fUseJPL) {
        sprintf(sz,"%s%d "," JPL DE",fUseJPL);
        PrintSz(sz);
    }
    sprintf(sz,"%s ","Swiss Eph Default selection");
    PrintSz(sz);
    PrintL();
}

real ArcToProg(jd0,arc)
real jd0,arc;
{
    CI ciT,ci0;
    char sz[cchSzDef];
    real sigpos,daydelta;
    int fEclipseBackwardT,fEquatorT,fDirectT,fProgressT,prom;

    ciT=ciCore;
    fEquatorT=us.fEquator;
    fEclipseBackwardT=fEclipseBackward;
    fProgressT=us.fProgress;
    fDirectT=us.fDirect;

        if (fBirthRate){
    if ( fBirthRate==1||fBirthRate==2 ) { /* birth rate RA solar */
        SetBirthRate();
         ciCore=ciT;}
        daydelta = arc > 0 ? RAbs(arc/rConverseBirthRate)*us.rProgDay : RAbs(arc/rBirthRate)*us.rProgDay;
        return jd0+ (fEclipseBackward ? -daydelta : daydelta);

    } else if (fSolarArcKey) { /*solar arc in RA*/
        us.fEquator=fTrue;
        us.fProgress=fFalse;
        us.fDirect=fFalse;
        JulianToMdy(jd0,&MM,&DD,&YY);
        TT=DegToDec(RFract(jd0-0.5)*24.0);
        SS=ZZ=0;
        ci0=ciCore;
        CastChart(fTrue);
        if (us.fDebug>2)
        {
            sprintf(sz,"solar ra %f \n",planet[oSun]);
            PrintSz(sz);
        }
        sigpos= Mod(-arc+planet[oSun]);
        prom=us.nEclObj;
        us.nEclObj=oSun;
        if (us.fDebug>2)
        {
            sprintf(sz,"solar arc %f jd0 %f \n",sigpos,jd0);
            PrintSz(sz);
        }
        force[oSun]=sigpos;
        returnnatalpos=-377;
        ciTwin=ci0;
        ciCore=ci0;
        if (arc > 0)
            fEclipseBackward=fTrue;
        else
            fEclipseBackward=fFalse;
        ChartReturnSet();
        force[oSun]=0;
        us.nEclObj=prom;
        us.fEquator=fEquatorT;
        us.fDirect=fDirectT;
        us.fProgress=fProgressT;
        fEclipseBackward=fEclipseBackwardT;
        ciCore=ciTwin;
        daydelta= RAbs(us.rProgDay*(MdytszToJulian(MM,DD,YY,TT,SS,ZZ)-jd0-rRound));
        if (us.fDebug>2)
        {
            sprintf(sz,"jdp %f \n",jd0+daydelta/us.rProgDay);
            PrintSz(sz);
        }
        ciCore=ciT;
        return jd0+ (fEclipseBackward ? -daydelta: daydelta);
    }
    else  /*Ptolemy key */
        return jd0+ (fEclipseBackward ? -RAbs(arc)*us.rProgDay : RAbs(arc)*us.rProgDay);
}

real ProgToArc(jd0,jdp)
real jd0,jdp;
{
    real jdpt,sun0,sunp,xx[6];
    int fEquatorT,fDirectT,fProgressT;
    char sz[cchSzDef];
    fProgressT=us.fProgress;
    fDirectT=us.fDirect;
    fEquatorT=us.fEquator;
    if (fSolarArcKey) {
        us.fProgress=fFalse;
        us.fDirect=fFalse;
        us.fEquator=fTrue;
        FPlacalcPlanet(oSun,jd0, fFalse,&sun0,&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
        jdpt= fEclipseBackward ? jd0+(jdp-jd0)/us.rProgDay : jd0-(jdp-jd0)/us.rProgDay;
        FPlacalcPlanet(oSun,jdpt, fFalse,&sunp,&xx[1],&xx[2],&xx[3],&xx[4],&xx[5]);
        us.fProgress=fProgressT;
        us.fDirect=fDirectT;
        us.fEquator=fEquatorT;
        if (us.fDebug>2)
        {
            sprintf(sz,"arc %f at %f years\n",sunp-sun0,(jdp-jd0)/us.rProgDay);
            PrintSz(sz);
        }
        return RSgn(jdp-jd0)*MinDistance(sun0,sunp);
    }
    else if (fBirthRate) {
        return fEclipseBackward ? rConverseBirthRate*((jdp-jd0)/us.rProgDay) : rBirthRate*((jdp-jd0)/us.rProgDay);
    }
    else return (jdp-jd0)/us.rProgDay;
}




void SetBirthRate()
{

    int i,fEquatorT,fDirectT,fProgressT;
    char sz[cchSzDef];
    char ignoreT[objMax+1];
    real f1, f2;
    CI ciC;

    if (FBetween(fBirthRate,1,2)) {
        ciC=ciCore;
        fProgressT=us.fProgress;
        fDirectT=us.fDirect;
        fEquatorT=us.fEquator;
        us.fProgress=fFalse;
        us.fDirect=fFalse;
        us.fEquator=fTrue;
        for (i=1; i<=objMax; i++)
        {
            ignoreT[i]=ignore[i];
            ignore[i]=1;
        }
        ignore[oSun]=0;
        CastChart(fTrue);
        if (us.fDebug>2) {
            sprintf(sz,"Birth Pos %f",planet[oSun]);
            PrintSz(sz);
        }
        rBirthRate=rConverseBirthRate=DFromR(ret[oSun]);
        if (!us.fPlacalc||fBirthRate==1) {
            f1=planet[oSun];
            ciCore=ciC;
            DD=AddDay(MM,DD,YY,1);
            CastChart(fTrue);
            f2=planet[oSun];
            rConverseBirthRate=MinDistance(f1,f2);
            ciCore=ciC;
            DD=AddDay(MM,DD,YY,-1);
            CastChart(fTrue);
            f2=planet[oSun];
            rBirthRate=MinDistance(f1,f2);
        }
        us.fProgress=fProgressT;
        us.fDirect=fDirectT;
        us.fEquator=fEquatorT;
        for (i=1; i<=objMax; i++)
        {
            ignore[i]=ignoreT[i];
        }
        ciCore=ciC;
        if (us.fDebug>2) {
            sprintf(sz,"Birth Rate %f C%f",rBirthRate,rConverseBirthRate);
            PrintSz(sz);
        }
    }
    return;
}
