/*
** Astrolog (Version 5.42J) File: charts1.c
**
** Code changed by Valentin Abramov (valja@tartu.customs.ee)
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


/*
******************************************************************************
** Single Chart Display Routines.
******************************************************************************
*/


/* Print header info showing time and date of the chart being displayed.   */
/* This is used by ChartListing() and the -l sector chart in PrintChart(). */

void PrintHeader()
{
    char sz[cchSzDef];
    int day, fNam, fLoc,fLocl,prasp,k;
    CI ciT;


    if (NoPrintHeader)
        return;
    NoPrintHeader = 1;

    ciT = ciMain;
    if (us.fProgress && !us.nRel) {
        ciMain = ciCore;
    }
    if (fReturnChart){
        ciMain = ciTwin;
        ciTwin=ciT;
        ciMain.nam=ciT.nam;
    }
    fNam = *ciMain.nam > chNull;
    fLoc = *ciMain.loc > chNull;
    fLocl = (CchSz(ciMain.loc) > (us.fSeconds ? 10 : 18));
    AnsiColor(kWhite);
    if (us.nRel == rcDual) {
        sprintf(sz, "%s %s  dual chart ", szAppName, szVersionCore);
        PrintSz(sz);
    } else if (us.nRel == rcSynastry) {
        sprintf(sz, "%s %s synastry chart, houses ", szAppName, szVersionCore);
        PrintSz(sz);
    } else {
        sprintf(sz, "%s %s chart ", szAppName, szVersionCore);
        PrintSz(sz);
    }
    if (fNoTimeOrSpace)
        PrintSz("(No time or space)\n");
    else if (us.nRel == rcComposite){
        PrintSz("(Composite)\n");
        sprintf(sz, "for %s%s", ciMain.nam, fNam ? "\n" : "");
        PrintSz(sz);
    }
    else if (us.nRel == rcMidpoint)
        PrintSz("(Time and space midpoint)\n");
    if (!fNoTimeOrSpace && !(us.nRel == rcComposite)) {
        sprintf(sz, "for %s%s", ciMain.nam, fNam ? "\n" : "");
        PrintSz(sz);
        if (fReturnChart||(fSynodicReturnChart&&us.fSector)){
            if (fSynodicReturnChart&&!us.fSector){
                sprintf(sz, "%s-%s synodic return %s%4d\n",us.nEclObj==oSun ? "Solar" : us.nEclObj==oMoo ? "Lunar" : szObjName[us.nEclObj],
                        us.nEclObj2==oSun ? "Solar" : us.nEclObj2==oMoo ? "Lunar" : szObjName[us.nEclObj2], (us.nEclObj==oMoo||us.nEclObj2==oMoo) ? szMonth[Mon] : "",Yea);
                PrintSz(sz);
            }
            else if (fSynodicReturnChart&&us.fSector){
                prasp=0;
                for (k=1;k<=us.nAsp;k++)
                   if (rAddAngle == rAspAngle[k])
                      prasp=k;
                   else if (-rAddAngle == rAspAngle[k])
                      prasp=-k;
                 
                
                sprintf(sz, "\n%sPr %s %s %s Si %s Primary Direction %s %s\n",(returnnatalpos < 0) ? "C":" ",prasp < -1 ? szAspectAbbrev[-prasp] : "",szObjName[us.nEclObj],prasp > 1 ? szAspectAbbrev[prasp] : prasp == 0 ? "+pa" : "",  
                        szObjName[us.nEclObj2], fGauqMode==0 ? "in mundo" : fGauqMode==1 ? "in zodiac" : "diurnal arc", fSecMotion ? "2nd mot" : "");
                PrintSz(sz);
                sprintf(sz, "Key %f %s Arc %f %s House System %s \n",us.rProgDay,fBirthRate ? "Sol.Birth rate RA" : fSolarArcKey ? "pSol.Arc RA" : "days/degree" ,returnnatalpos,(returnnatalpos > 0) ? "Converse" : "",szSystem[us.nHouseSystem]);
                PrintSz(sz);
            }
            else {
                sprintf(sz, "%s %s %s%4d\n",us.nEclObj==oSun ? "Solar" : us.nEclObj==oMoo ? "Lunar" : szObjName[us.nEclObj],force[us.nEclObj]? "Ingress":"Return" ,us.nEclObj==oMoo ? szMonth[Mon] : "",Yea);
                PrintSz(sz);
            }
        }
        day = DayOfWeek(Mon, Day, Yea);
        sprintf(sz, "%c%c%c %s %s (%cT %s GMT)", chDay3(day),
                SzDate(Mon, Day, Yea, 3), us.fSeconds ? SzTim(Tim) : SzTim(Tim), ChDst(Dst),
                SzZone(Zon));
        PrintSz(sz);
        sprintf(sz, "%c%s%s%s\n", (fLocl || (fLoc && !fNam)) ? '\n' : ' ', ciMain.loc,
                fLoc ? " " : "",
#ifdef KDE
                gi.fiskde ? SzLocation(Lon,Lat) :
#endif
                SzLocationAlt(Lon, Lat,Alt));
        PrintSz(sz);
    }
    if (us.fHorizon) {
        if (us.fPrimeVert)
            sprintf(sz,"Prime Vertical\n");
        else
            sprintf(sz,"Local Horizon\n");
        PrintSz(sz);
    }
    if (us.fProgress || us.nRel == rcTransit || us.nRel == rcDual \
            || us.nRel == rcSynastry || us.nRel == rcProgress \
            || us.fTransitInf) {
        if (us.fProgress ) {
            if (us.fSolarArc )
                sprintf(sz, "Chart directed to\n");
            else
                sprintf(sz, "Chart progressed to\n");
        } else if (us.fTransitInf) {
            if (us.fSolarArc || us.fDirect )
                sprintf(sz, "natal vs. Chart directed %s to\n",(us.fSector && fEclipseBackward) ? "converse" : "");
            else if (is.fProgress)
                sprintf(sz, "natal vs. Chart progressed to\n");
            else
                sprintf(sz, "Transits for\n");

        } else {
            if (us.nRel == rcProgress) {
                if (!us.fInterpret && (us.fInfluence || us.fListing)){
                    if (us.fSolarArc )
                        sprintf(sz, "Natal houses, planets directed to\n");
                    else
                        sprintf(sz, "Natal houses, planets progressed to\n");
                } else {
                    if (us.fSolarArc )
                        sprintf(sz, "Directions for\n");
                    else
                        sprintf(sz, "Progressions for\n");
                }
            } else if (us.nRel == rcTransit) {
                if (!us.fInterpret && (us.fInfluence || us.fListing))
                    sprintf(sz, "Natal houses, transit planets for\n");
                else
                    sprintf(sz, "Transits for\n");
            } else if (us.nRel == rcSynastry) {
                sprintf(sz, "planets for ");
            } else if (us.nRel == rcDual) {
                sprintf(sz, "and ");
            } else
                sprintf(sz, "and\n");
        }
        PrintSz(sz);
        if (us.fProgress)
            ciMain = ciT;
        else if (us.fTransitInf)
            ciMain = ciTran;
        else
            ciMain = ciTwin;
        if (us.nRel == rcDual || us.nRel == rcSynastry) {
            sprintf(sz, "%s\n", ciMain.nam);
            PrintSz(sz);
        }
        day = DayOfWeek(Mon, Day, Yea);
        if (us.fProgress && us.nRel) {
#ifdef PLACALC
            revjul(is.JDp+rRound,is.JDp >= 2299171.0,&Mon,&Day,&Yea,&Tim);
            Zon=Dst=0.0;
            sprintf(sz, "%c%c%c %s %s (%cT %s GMT) \n", chDay3(day),
                    SzDate(Mon, Day, Yea, 3), SzTim(Tim), ChDst(Dst),
                    SzZone(Zon));
            PrintSz(sz);
#endif
        } else {
            sprintf(sz, "%c%c%c %s %s (%cT %s GMT)", chDay3(day),
                    SzDate(Mon, Day, Yea, 3), us.fSeconds ? SzTim(Tim) : SzTim(Tim), ChDst(Dst),
                    SzZone(Zon));
            PrintSz(sz);
            sprintf(sz, "%c%s%s%s\n", fLoc && !fNam ? '\n' : ' ', ciMain.loc,
                    fLoc ? " " : "", SzLocationAlt(Lon, Lat,Alt));
            PrintSz(sz);
        }
        ciMain = ciT;
    }
    if (us.fVerboseHeader)
        PrintVerboseHeader();
}


/* Print the straight listing of planet and house positions and specified */
/* by the -v switch, along with the element table, etc.                   */

void ChartListing()
{
    ET et;
    char sz[cchSzDef];
    int i, j=0, k, l, m=0, n,fNam, fLoc, rank[oNormOpt+1];
    real rT,value[oNormOpt+1],f1,f2,offset=0;
    CreateElemTable(&et);
    fNam = *ciMain.nam > chNull;
    fLoc = *ciMain.loc > chNull;
    PrintHeader();    /* Show time and date of the chart being displayed. */
#ifdef INTERPRET
    if (us.nTreshold > 0.01)
        PlanetPPower();
    if (us.fInterpret) {          /* Print an interpretation if -I in effect. */
        if (us.nRel == rcSynastry)
            InterpretSynastry();      /* Print synastry interpretaion for -r -I.  */
        else {
#ifdef INTERPRETALT
            if (InterpretAlt && (!us.nRel || us.nRel == rcTransit)) {
                if (is.fMult)
                    PrintL2();
                if (us.nRel == rcTransit)
                    TransitInterpret();
                else
                    NatalInterpret();
                is.fMult = fTrue;
            } else {
#endif
                if (us.fInterpretSabian)
                    InterpretSabian();
                else
                    InterpretLocation();      /* Do normal interpretation for just -v -I. */
#ifdef INTERPRETALT
            }
#endif
        }
        return;
    }
#endif
    AnsiColor(kDefault);
    if (us.fSeconds) {
        sprintf(sz, us.fPlacalc ? "Body          Location  Ret. %s Rul. House Rul.  Velocity  Distance\n":
                "Body          Location  Ret. %s Rul. House Rul.  Velocity\n",
                us.fEquator ? "Declin. " : "Latitude");
        PrintSz(sz);
    } else {
        sprintf(sz,
                "Body         Locat. Ret. %s. Rul. House  Rul. Veloc.    %s Houses.\n",
                us.fEquator ? "Decl" : "Lati", szSystem[us.nHouseSystem]);
        PrintSz(sz);
    }
    if (!fNam && !fLoc)
        PrintL();

    /* Ok, now print out the location of each object. */

    if (us.fSortRank){
        if (us.nDegObj) {
            us.nDegFrom=RFract(planet[us.nDegObj]/RAbs(us.nSortDial))*RAbs(us.nSortDial)-us.nDegOrb;
            us.nDegTo=RFract(planet[us.nDegObj]/RAbs(us.nSortDial))*RAbs(us.nSortDial)+us.nDegOrb;;
        }
        if (us.nDegFrom>us.nDegTo)
            us.nSortDial *= -1;
        offset= (us.nDegTo < 0 ) ? -us.nDegTo*2 : (us.nDegTo > us.nSortDial) ? 2*(us.nSortDial - us.nDegTo) : 0 + (us.nDegFrom < 0) ? -2*us.nDegFrom : (us.nDegFrom > us.nSortDial) ? 2*(us.nSortDial - us.nDegFrom) : 0 ;
        for (m =0;m<=oNormOpt+1;m++){
            if (!FIgnore(m))
                value[m]=(1-RFract(Mod(offset+planet[m])/RAbs(us.nSortDial)))*us.nSortDial;
            else
                value[m]=-1000.0*us.nSortDial;
            /* sprintf(sz,"%f %f %f \n",us.nDegFrom,us.nDegTo,offset);PrintSz(sz);*/
        }
        (us.objCenter==oEar) ? SortRevRank(value,rank,oNormOpt):
        SortRevRank2(value,rank,oNormOpt);
        m=0;
    }

    for (i = 1-us.fSeconds, n = 0; i <= cObjOpt; i++, n++) {
        if (us.fSeconds || us.fSortRank ) {
            if (FIgnore(i))
                continue;
        } else {
            if (i > oCore && (((i <= cuspHi) || FIgnore(i))))
                continue;
            while (i <= cuspHi && n <= cuspHi && FIgnore(n))
                n++;
        }
        if (i >= cuspLo && i <= cuspHi-2 && n > cuspHi-2) /* Don't print blank */
            continue;                                      /* lines.      VA.   */

        if (i <= cuspHi-2  && n > cuspHi-2)
            PrintTab(' ', 52);
        else {
            if (i > cuspHi-2)
                n = i;
            if (us.fSortRank){
                m++;
                j=rank[m];
                f1=(1-RFract((us.nDegFrom+offset)/RAbs(us.nSortDial)))*us.nSortDial;
                f2=(1-RFract((us.nDegTo+offset)/RAbs(us.nSortDial)))*us.nSortDial;
                if (RAbs(f1-f2)<0.00001)
                    f2+=us.nSortDial;
                if (RAbs(f1-f2)>us.nSortDial-0.00001){
                    f1=0;
                    f2=us.nSortDial;
                }
                /* sprintf(sz,"%f %f %f %f %f \n",value[j],f1,f2,us.nDegFrom,us.nDegTo);PrintSz(sz); */
                if (!FBetween(value[j], Min(f1,f2), Max(f1,f2)))
                    continue;
            }
            else j=n;
            if (j==-1)
                continue;
#ifdef GRAPH
            if (us.fDebug >=4){
                sprintf(sz,"%d %d %d %d %d Orb %f Add %f rul1 %d rul2 %d ex %d \n",i,kObjA1[i],ignore2[i],ignore3[i],ignoreS[i],rObjOrb[i],rObjAdd[i],ruler1[i],ruler2[i],exalt[i]);
                PrintSz(sz);
                sprintf(sz,"inf %f pp1 %f pp2 %f ppt %f ti %f \n ",rObjInf[i],ppower1[i],ppower2[i],ppowerTotal[i],rTransitInf[i]);
                PrintSz(sz);
                sprintf(sz,"%s %s \n",szDrawObject[i],szDrawObject2[i]);
                PrintSz(sz);
                sprintf(sz,"%d %d %f \n",j,m,value[cObj]);
                PrintSz(sz);
            }
#endif
            if (FStar(j)){
                if (!ignore[starLo-1+starname[j-starLo+1]]) {
                    l = starLo-1+starname[j-starLo+1];
                    AnsiColor(kObjA[l]);
                    sprintf(sz, "%-11.11s: ", szObjName[l]);
                    PrintSz(sz);
                    PrintZodiac(planet[l]);
                    PrintSz("   ");
                    PrintAltitude(planetalt[l]);
                    k = inhouse[l];
                    AnsiColor(kSignA(k));
                    sprintf(sz, "%5.2f[%2d%s]",rStarBright[l-starLo+1], k, szSuffix[k]);
                    PrintSz(sz);
                    AnsiColor(kDefault);
                    sprintf(sz, "%s %8.8s Star #%2d:%6.1f\n",
                            us.fSeconds ? "" : "    ", szStarConName[l-starLo+1], j-starLo+1, rStarDist[l-starLo+1]);
                    PrintSz(sz);
                    continue;
                }
            }
            AnsiColor(kObjA[j]);
            sprintf(sz, "%-11.11s: ", szObjName[j]);
            PrintSz(sz);
            PrintZodiac(planet[j]);
            sprintf(sz, "%c%c%c", (RAbs(ret[j])<(us.nStationOrb/planetdis[j]/rDegRad)&& planetdis[j]>0.1) ? chStat : ' ' ,ret[j] >= 0.0 ? ' ' : chRet,(us.fEquator&&!us.fGalPlane&&RAbs(planetalt[j])>us.nEpsilon) ? chOOB : ' ');
            PrintSz(sz);
            if (FObject(j) || FNode(j) || FAst(j)||us.fEquator)
                PrintAltitude(planetalt[j]);
            else
                PrintTab('_', us.fSeconds ? 10 : 7);
            sprintf(sz, " (%c)", Dignify(j, SFromZ(planet[j])));
            PrintSz(FObject(j) ? sz
#ifdef KDE
                    : gi.fiskde ? "  "
#endif
                    : "    " );
            k = inhouse[j];
            l = SFromZ(chouse[k]);
            AnsiColor(kSignA(k));
            sprintf(sz, " [%2d%s] ", k, szSuffix[k]);
            PrintSz(sz);
            AnsiColor(kDefault);
            sprintf(sz, "[%c", Dignify(j, k));
            PrintSz(FObject(j) ? sz : "    ");
            sprintf(sz, "%c] ", Dignify(j, l));
            PrintSz(FObject(j) ? sz : " " );
            if ((j != oMoo || us.fPlacalc) &&
                    (FObject(j) || FNode(j) || FAst(j)) ) {
                PrintCh((char)(ret[j] < 0.0 ? '-' : '+'));
                rT = DFromR(RAbs(ret[j]));
                sprintf(sz, us.fSeconds ? (rT < 10.0 ? "%8.6f" : "%8.5f") :
                                (rT < 10.0 ? "%5.3f" : "%5.2f"), rT);
                PrintSz(sz);
            } else
                PrintTab('_', us.fSeconds ? 9 : 6);
        }
        if (!us.fSeconds) {

            /* For some lines, we have to append the house cusp positions. */

            if (i <= cSign) {
                PrintSz("  -  ");
                AnsiColor(kSignA(i));
                sprintf(sz, "House cusp %2d: ", i);
                PrintSz(sz);
                PrintZodiac(chouse[i]);
            }

            /* For some lines, we have to append the element table information. */

            if (i == cSign+2)
                PrintSz("     Car Fix Mut TOT");
            else if (i > cSign+2 && i < cSign+7) {
                k = i-(cSign+2)-1;
                AnsiColor(kElemA[k]);
                sprintf(sz, "  %c%c%c%3d %3d %3d %3d",
                        szElem[k][0], szElem[k][1], szElem[k][2],
                        et.coElemMode[k][0], et.coElemMode[k][1], et.coElemMode[k][2],
                        et.coElem[k]);
                PrintSz(sz);
                AnsiColor(kDefault);
            } else if (i == cSign+7) {
                sprintf(sz, "  TOT %2d %3d %3d %3d",
                        et.coMode[0], et.coMode[1], et.coMode[2], et.coSum);
                PrintSz(sz);
            } else if (i == oCore)
                PrintTab(' ', 23);
            else if (FBetween(j,HypuranLo,HypuranHi)) {
                sprintf(sz, "  Uranian #%d", j-HypuranLo+1);
                PrintSz(sz);
            }
#ifdef ASTEROIDS
            else if (FBetween(j,CentLo,CentHi)) {
                sprintf(sz, "  Centaur #%d", j-CentLo+2);
                PrintSz(sz);
            }
            else if (FBetween(j,TrneLo,TrneHi)) {
                sprintf(sz, "  TNO #%d", j-TrneLo+1);
                PrintSz(sz);
            }
            else if (FBetween(j,AtenLo,AtenHi)) {
                sprintf(sz, "  Aten #%d", j-AtenLo+1);
                PrintSz(sz);
            }
            else if (FBetween(j,ApolLo,ApolHi)) {
                sprintf(sz, "  Apoll #%d", j-ApolLo+1);
                PrintSz(sz);
            }
            else if (FBetween(j,AmorLo,AmorHi)) {
                sprintf(sz, "  Amor #%d", j-AmorLo+1);
                PrintSz(sz);
            }
            else if (FBetween(j,MuseLo,MuseHi)) {
                sprintf(sz, "  Muse #%d", j-MuseLo+1);
                PrintSz(sz);
            }
            else if (FBetween(j,AgenLo,AgenHi)) {
                sprintf(sz, "  Asteroid #%d", j-AgenLo+5);
                PrintSz(sz);
            }
#endif
#ifdef ASTEROIDS
            else if (FBetween(j,RiyaLo,RiyaHi)) {
                sprintf(sz, "  Uncatalogued #%d", j-RiyaLo+1);
                PrintSz(sz);
            }
            else if (FBetween(j,AsteLo+us.nExtraStar,AsteHi)) {
                sprintf(sz, "  Catalogue #%d", astnr[j-AsteLo-us.nExtraStar]);
                PrintSz(sz);
            }
#endif
            sz[0] = chNull;
            switch (i-cSign-1) {
            case 1:
                sprintf(sz, "   +:%2d", et.coYang);
                break;
            case 2:
                sprintf(sz, "   -:%2d", et.coYin);
                break;
            case 3:
                sprintf(sz, "   M:%2d", et.coMC);
                break;
            case 4:
                sprintf(sz, "   N:%2d", et.coIC);
                break;
            case 5:
                sprintf(sz, "   A:%2d", et.coAsc);
                break;
            case 6:
                sprintf(sz, "   D:%2d", et.coDes);
                break;
            case 7:
                sprintf(sz,    "<:%2d", et.coLearn);
                break;
            }
            PrintSz(sz);
        } else {
            if (!us.fPlacalc){
                PrintSz(" Decan: ");
                is.fSeconds = fFalse;
                PrintZodiac(Decan(planet[j]));
                is.fSeconds = us.fSeconds;
            } else {
                if (us.fDebug)
                    sprintf(sz,"%11.6f AU",planetdis[j]);
                else sprintf(sz,"%8.3f AU",planetdis[j]);
                PrintSz(sz);
            }
        }
        PrintL();
    }

    /* Do another loop to print out the stars in their specified order. */

    /*  moved forward because of sorting if (us.nStar) for (i = starLo; i <= starHi; i++)
      if (!ignore[starLo-1+starname[i-starLo+1]]) {
        j = starLo-1+starname[i-starLo+1];
        AnsiColor(kObjA[j]);
        sprintf(sz, "%-10.10s: ", szObjName[j]); PrintSz(sz);
        PrintZodiac(planet[j]);
        PrintSz("   ");
        PrintAltitude(planetalt[j]);
        k = inhouse[j];
        AnsiColor(kSignA(k));
        sprintf(sz, "     [%2d%s]", k, szSuffix[k]); PrintSz(sz);
        AnsiColor(kDefault);
        sprintf(sz, "   %s %8.8s  Star #%2d: %5.2f\n",
          us.fSeconds ? "   " : "", szStarConName[j-starLo+1], i-starLo+1, rStarBright[j-starLo+1]);
        PrintSz(sz);
      } */
}


/* Print out the aspect and midpoint grid for a chart, as specified with the */
/* -g switch. (Each grid row takes up 4 lines of text.)                      */

void ChartGrid()
{
    char sz[cchSzDef];
    int x, y, r, x1, y1, temp;

    PrintHeader();     /* Display chart info */

#ifdef INTERPRET
    if (us.fInterpret) {    /* Print interpretation instead if -I in effect. */
        InterpretGrid();
        return;
    }
#endif

    for (y1 = 0, y = 0; y <= cObjOpt; y++) if (FProper2(y))
            for (r = 1; r <= 4; r++) {
                for (x1 = 0, x = 0; x <= cObjOpt; x++) if (FProper2(x)) {
                        if (y1 > 0 && x1 > 0 && y+r > 1)
                            PrintCh((char)(r > 1 ? chV : chC));
                        if (r > 1) {
                            temp = grid->n[x][y];

                            /* Print aspect rows. */

                            if (x < y) {
                                if (temp)
                                    AnsiColor(kAspA[temp]);
                                if (r == 2)
                                    PrintSz(temp ? szAspectAbbrev[temp] : "   ");
                                else if (!temp)
                                    PrintSz("   ");
                                else {
                                    if (r == 3) {
                                        if (grid->v[x][y] < 32700)
                                            sprintf(sz, "%c%2d", us.fAppSep ?
                                                    (grid->v[x][y] < 0 ? 'a' : 's') :
                                                            (grid->v[x][y] < 0 ? '-' : '+'), abs(grid->v[x][y])/3600);
                                        else
                                            sprintf(sz, "%3d", abs(grid->v[x][y])/3600);
                                    } else
                                        sprintf(sz, "%02d'", (abs(grid->v[x][y])%3600)/60);
                                    PrintSz(sz);
                                }

                                /* Print midpoint rows. */

                            } else if (x > y) {
                                AnsiColor(kSignA(temp));
                                if (r == 2) {
                                    temp = grid->n[x][y];
                                    sprintf(sz, "%c%c%c", chSig3(temp));
                                } else if (r == 3) {
                                    sprintf(sz, "%2d%c", grid->v[x][y]/60, chDeg0);
                                } else
                                    sprintf(sz, "%02d'", grid->v[x][y]%60);
                                PrintSz(sz);

                                /* Print the diagonal of object names. */

                            } else {
                                AnsiColor(kReverse);
                                if (r == 2) {
                                    AnsiColor(kObjA[y]);
                                    sprintf(sz, "%c%c%c", chObj3(y));
                                } else {
                                    temp = SFromZ(planet[y]);
                                    AnsiColor(kSignA(temp));
                                    if (r == 3)
                                        sprintf(sz, "%2d%c", (int)planet[y] - (temp-1)*30, chDeg0);
                                    else
                                        sprintf(sz, "%c%c%c", chSig3(temp));
                                }
                                PrintSz(sz);
                            }
                            AnsiColor(kDefault);
                        } else
                            if (y1 > 0)
                                PrintTab(chH, 3);
                        x1++;
                    }
                if (y+r > 1)
                    PrintL();
                y1++;
            }
    if (y1 == 0)
        PrintSz("Empty aspect grid.\n");
}


/* This is a subprocedure of DisplayGrands(). Here we print out one aspect */
/* configuration found by the parent procedure.                            */

void PrintGrand(ac, i1, i2, i3, i4)
char ac;
int i1, i2, i3, i4;
{
    char sz[cchSzDef];
    int asp=0;

    switch (ac) {
    case acS :
        asp = aCon;
        break;
    case acGT:
        asp = aTri;
        break;
    case acTS:
        asp = aOpp;
        break;
    case acY :
        asp = aInc;
        break;
    case acGC:
        asp = aSqu;
        break;
    case acC :
        asp = aSex;
        break;
    case acMR:
        asp = aSex;
        break;
    case acK :
        asp = aTri;
        break;
    default:
        ;
    }
    AnsiColor(kAspA[asp]);
    sprintf(sz, "%-11s", szAspectConfig[(int) ac]);
    PrintSz(sz);
    AnsiColor(kDefault);
    sprintf(sz, " %s ", ac == acS || ac == acGT || ac == acGC ||
            ac == acMR || ac == acK ? "with" : "from");
    PrintSz(sz);
    AnsiColor(kObjA[i1]);
    sprintf(sz, "%c%c%c: ", chObj3(i1));
    PrintSz(sz);
    PrintZodiac(planet[i1]);
    sprintf(sz, " %s ", ac == acS || ac == acGT || ac == acK ? "and" : "to ");
    PrintSz(sz);
    AnsiColor(kObjA[i2]);
    sprintf(sz, "%c%c%c: ", chObj3(i2));
    PrintSz(sz);
    PrintZodiac(planet[i2]);
    sprintf(sz, " %s ", ac == acGC || ac == acC || ac == acMR ? "to " : "and");
    PrintSz(sz);
    AnsiColor(kObjA[i3]);
    sprintf(sz, "%c%c%c: ", chObj3(i3));
    PrintSz(sz);
    PrintZodiac(planet[i3]);
    if (ac == acGC || ac == acC || ac == acMR || ac == acK) {
        PrintSz(" to ");
        AnsiColor(kObjA[i4]);
        sprintf(sz, "%c%c%c: ", chObj3(i4));
        PrintSz(sz);
        PrintZodiac(planet[i4]);
    }
    PrintL();
}


/* Scan the aspect grid of a chart and print out any major configurations, */
/* as specified with the -g0 switch.                                       */

void DisplayGrands()
{
    int cac = 0, i, j, k, l=0;

    for (i = 0; i <= cObjOpt; i++) if (!FIgnore(i))
            for (j = 0; j <= cObjOpt; j++) if (j != i && !FIgnore(j))
                    for (k = 0; k <= cObjOpt; k++) if (k != i && k != j && !FIgnore(k)) {

                            /* Is there a Stellium among the current three planets? */

                            if (i < j && j < k && grid->n[i][j] == aCon &&
                                    grid->n[i][k] == aCon && grid->n[j][k] == aCon) {
                                cac++;
                                PrintGrand(acS, i, j, k, l);

                                /* Is there a Grand Trine? */

                            } else if (i < j && j < k && grid->n[i][j] == aTri &&
                                       grid->n[i][k] == aTri && grid->n[j][k] == aTri) {
                                cac++;
                                PrintGrand(acGT, i, j, k, l);

                                /* Grand Trine is there, may be there is a Kite too ? */
                                /* We look, if some planet is in sextiles with some   */
                                /* pair of planets of Grand Trine, then it's surely   */
                                /* opposed with third one.                            */

                                for (l = 0; l <= cObjOpt; l++) if (!FIgnore(l)) {
                                        if (grid->n[Min(i,l)][Max(i,l)] == aSex &&
                                                grid->n[Min(j,l)][Max(j,l)] == aSex) {
                                            cac++;
                                            PrintGrand(acK, i, j, k, l);
                                        }
                                        if (grid->n[Min(j,l)][Max(j,l)] == aSex &&
                                                grid->n[Min(k,l)][Max(k,l)] == aSex) {
                                            cac++;
                                            PrintGrand(acK, i, j, k, l);
                                        }
                                        if (grid->n[Min(i,l)][Max(i,l)] == aSex &&
                                                grid->n[Min(k,l)][Max(k,l)] == aSex) {
                                            cac++;
                                            PrintGrand(acK, i, j, k, l);
                                        }
                                    }

                                /* Is there a T-Square? */

                            } else if (j < k && grid->n[j][k] == aOpp &&
                                       grid->n[Min(i, j)][Max(i, j)] == aSqu &&
                                       grid->n[Min(i, k)][Max(i, k)] == aSqu) {
                                cac++;
                                PrintGrand(acTS, i, j, k, l);

                                /* Is there a Yod? */

                            } else if (j < k && grid->n[j][k] == aSex &&
                                       grid->n[Min(i, j)][Max(i, j)] == aInc &&
                                       grid->n[Min(i, k)][Max(i, k)] == aInc) {
                                cac++;
                                PrintGrand(acY, i, j, k, l);
                            }
                            for (l = 0; l <= cObjOpt; l++) if (!FIgnore(l)) {

                                    /* Is there a Grand Cross among the current four planets? */

                                    if (i < j && i < k && i < l && j < l && grid->n[i][j] == aSqu &&
                                            grid->n[Min(j, k)][Max(j, k)] == aSqu &&
                                            grid->n[Min(k, l)][Max(k, l)] == aSqu &&
                                            grid->n[i][l] == aSqu &&
                                            MinDistance(planet[i], planet[k]) > 150.0 &&
                                            MinDistance(planet[j], planet[l]) > 150.0) {
                                        cac++;
                                        PrintGrand(acGC, i, j, k, l);

                                        /* Is there a Cradle? */

                                    } else if (i < l && grid->n[Min(i, j)][Max(i, j)] == aSex &&
                                               grid->n[Min(j, k)][Max(j, k)] == aSex &&
                                               grid->n[Min(k, l)][Max(k, l)] == aSex &&
                                               MinDistance(planet[i], planet[l]) > 150.0) {
                                        cac++;
                                        PrintGrand(acC, i, j, k, l);

                                        /* Is there a Mystic Rectangle? */

                                    } else if (i < j &&  i < k && i < l &&
                                               grid->n[Min(i, j)][Max(i, j)] == aOpp &&
                                               grid->n[Min(k, l)][Max(k, l)] == aOpp &&
                                               grid->n[Min(i, k)][Max(i, k)] == aTri &&
                                               grid->n[Min(j, l)][Max(j, l)] == aTri) {
                                        cac++;
                                        PrintGrand(acMR, i, j, k, l);
                                    }
                                }
                        }
    if (!cac)
        PrintSz("No major configurations in aspect grid.\n");
}


/* This is subprocedure of ChartWheel(). Here we print out the location */
/* of a particular house cusp as well as what house cusp number it is.  */

void PrintHouse(i, left)
int i, left;
{
    char sz[cchSzDef];
    real r;
    int j;

    if (us.fVedic) {
        j = Mod12(12-i);
        r = ZFromS(j);
    } else {
        j = i;
        r = chouse[i];
    }
    if (!left)
        PrintZodiac(r);
    AnsiColor(kSignA(j));
    sprintf(sz, "<%d>", j);
    PrintSz(sz);
    if (left)
        PrintZodiac(r);
    else
        AnsiColor(kDefault);
}


/* Another subprocedure of ChartWheel(). Print out one of the chart info */
/* rows in the middle of the wheel (which may be blank) given an index.  */

void PrintWheelCenter(irow)
int irow;
{
    char sz[cchSzDef], szT[8];
    int cch, nT;

    if (*ciMain.nam == chNull && *ciMain.loc == chNull)    /* Try to center */
        irow--;
    if (*ciMain.nam == chNull && irow >= 1)    /* Don't have blank lines if */
        irow++;                                  /* the name and/or location  */
    if (*ciMain.loc == chNull && irow >= 3)    /* strings are empty.        */
        irow++;
    switch (irow) {
    case 0:
        sprintf(sz, "%s %s chart", szAppName, szVersionCore);
        break;
    case 1:
        sprintf(sz, "%s", ciMain.nam);
        break;
    case 2:
        nT = DayOfWeek(Mon, Day, Yea);
        sprintf(sz, "%c%c%c %s %s", chDay3(nT), SzDate(Mon, Day, Yea, 2),
                SzTim(Tim));
        break;
    case 3:
        sprintf(sz, "%s", ciMain.loc);
        break;
    case 4:
        nT = (int)(RFract(RAbs(Zon))*100.0+rRound);
        sprintf(sz, "%cT %c%02d:%02d, %s", ChDst(Dst),
                Zon > 0.0 ? '-' : '+', (int)RAbs(Zon), nT, SzLocation(Lon, Lat));
        break;
    case 5:
        nT = us.fEuroTime;
        us.fEuroTime = fTrue;
        sprintf(szT, "%s", SzTim(DegToDec((us.fSweHouse ? planet[oArmc] : DFromR(is.RA))*(24.0/rDegMax))));
        sprintf(sz, "%s: %s, Sid.T: %s", ((fdeltaT && deltaT==0)? "ET":"UT"),SzTim(Tim+Zon-Dst), szT);
        us.fEuroTime = nT;
        break;
    case 6:
        sprintf(sz, "%s Houses", szSystem[us.nHouseSystem]);
        break;
    case 7:
        sprintf(sz, "%s / %s", us.fSidereal ? "Sidereal" : "Tropical",
                us.objCenter == oSun ? "Heliocentric" :
                (us.objCenter == oEar ? "Geocentric" : szObjName[us.objCenter]));
        break;
    case 8:
        sprintf(sz, "Julian Day = %12.4f", JulianDayFromTime(is.T));
        break;
    default:
        *sz = chNull;
    }
    cch = CchSz(sz);
    nT = WHEELCOLS*2-1 + is.fSeconds*8;
    PrintTab(' ', (nT - cch) / 2);
    PrintSz(sz);
    PrintTab(' ', nT-cch - (nT - cch) / 2);
}


/* Yet another subprocedure of ChartWheel(). Here we print out one line */
/* in a particular house cell (which may be blank).                     */

void PrintWheelSlot(obj)
int obj;
{
    char sz[cchSzDef];

    if (obj >= oEar) {
        AnsiColor(kObjA[obj]);
        if (us.fSeconds){
            sprintf(sz, " %-5.5s ", szObjName[obj]);
            PrintSz(sz);
        }
        else{
            sprintf(sz, " %c%c%c ", chObj3(obj));
            PrintSz(sz);
        }
        PrintZodiac(planet[obj]);
        sprintf(sz, "%c ", ret[obj] < 0.0 ? 'r' : ' ');
        PrintSz(sz);
        PrintTab(' ', WHEELCOLS-(us.fSeconds ?17:15));
    } else                            /* This particular line is blank. */
        PrintTab(' ', WHEELCOLS-1 + is.fSeconds*4);
}


/* Display all the objects in a wheel format on the screen, as specified */
/* with the -w switch. The wheel is divided into the 12 houses and the   */
/* planets are placed accordingly.                                       */

void ChartWheel()
{
    int wheel[cSign][WHEELROWS], wheelcols, count = 0, i, j, k, l;

    /* If the seconds (-b0) flag is set, we'll print all planet and house    */
    /* locations to the nearest zodiac second instead of just to the minute. */

    wheelcols = WHEELCOLS + is.fSeconds*4;

    for (i = 0; i < cSign; i++)
        for (j = 0; j < us.nWheelRows; j++)
            wheel[i][j] = -1;                    /* Clear out array. */

    /* This section of code places each object in the wheel house array. */

    for (i = 0; i <= cObjOpt && count < us.nWheelRows*12; i++) {
        if (FIgnore(i) || (FCusp(i) &&
                           MinDistance(planet[i], chouse[i-oAsc+1]) < rRound/60.0))
            continue;

        /* Try to put object in its proper house. If no room, */
        /* then overflow over to the next succeeding house.   */

        for (j = (us.fVedic ? Mod12(11-SFromZ(planet[i])) : inhouse[i])-1;
                j < cSign; j = j < cSign ? (j+1)%cSign : j) {

            /* Now try to find the proper place in the house to put the object. */
            /* This is in sorted order, although a check is made for 0 Aries.   */

            if (wheel[j][us.nWheelRows-1] >= 0)
                continue;
            l = chouse[j+1] > chouse[Mod12(j+2)];
            for (k = 0; wheel[j][k] >= 0 && (planet[i] >= planet[wheel[j][k]] ||
                                             (l && planet[i] < rDegHalf && planet[wheel[j][k]] > rDegHalf)) &&
                    !(l && planet[i] > rDegHalf && planet[wheel[j][k]] < rDegHalf); k++)
                ;

            /* Actually insert object in proper place. */

            if (wheel[j][k] < 0)
                wheel[j][k] = i;
            else {
                for (l = us.nWheelRows-1; l > k; l--)
                    wheel[j][l] = wheel[j][l-1];
                wheel[j][k] = i;
            }
            count++;
            j = cSign;
        }
    }

    /* Now, if this is really the -w switch and not -w0, then reverse the  */
    /* order of objects in western houses for more intuitive reading. Also */
    /* reverse the order of everything in the reverse order Vedic wheels.  */

    if (us.fVedic)
        for (i = 0; i < cSign; i++)
            for (j = 0; j < us.nWheelRows/2; j++) {
                k = us.nWheelRows-1-j;
                l = wheel[i][j];
                wheel[i][j] = wheel[i][k];
                wheel[i][k] = l;
            }
    if (!us.fWheelReverse)
        for (i = 3; i < 9; i++)
            for (j = 0; j < us.nWheelRows/2; j++) {
                k = us.nWheelRows-1-j;
                l = wheel[i][j];
                wheel[i][j] = wheel[i][k];
                wheel[i][k] = l;
            }

    /* Here we actually print the wheel and the objects in it. */

    PrintCh(chNW);
    PrintTab(chH, WHEELCOLS-8);
    PrintHouse(11, fTrue);
    PrintTab(chH, WHEELCOLS-11+us.fVedic);
    PrintHouse(10, fTrue);
    PrintTab(chH, WHEELCOLS-10+us.fVedic);
    PrintHouse(9, fTrue);
    PrintTab(chH, wheelcols-4);
    PrintCh(chNE);
    PrintL();
    for (i = 0; i < us.nWheelRows; i++) {
        for (j = 10; j >= 7; j--) {
            PrintCh(chV);
            PrintWheelSlot(wheel[j][i]);
        }
        PrintCh(chV);
        PrintL();
    }
    PrintHouse(12, fTrue);
    PrintTab(chH, WHEELCOLS-11);
    PrintCh(chC);
    PrintTab(chH, wheelcols-1);
    PrintCh(chJN);
    PrintTab(chH, wheelcols-1);
    PrintCh(chC);
    PrintTab(chH, WHEELCOLS-10);
    PrintHouse(8, fFalse);
    PrintL();
    for (i = 0; i < us.nWheelRows; i++) {
        PrintCh(chV);
        PrintWheelSlot(wheel[11][i]);
        PrintCh(chV);
        PrintWheelCenter(i);
        PrintCh(chV);
        PrintWheelSlot(wheel[6][i]);
        PrintCh(chV);
        PrintL();
    }
    PrintHouse(1, fTrue);
    PrintTab(chH, WHEELCOLS-10-us.fVedic);
    PrintCh(chJW);
    PrintWheelCenter(us.nWheelRows);
    PrintCh(chJE);
    PrintTab(chH, WHEELCOLS-10);
    PrintHouse(7, fFalse);
    PrintL();
    for (i = 0; i < us.nWheelRows; i++) {
        PrintCh(chV);
        PrintWheelSlot(wheel[0][i]);
        PrintCh(chV);
        PrintWheelCenter(us.nWheelRows+1 + i);
        PrintCh(chV);
        PrintWheelSlot(wheel[5][i]);
        PrintCh(chV);
        PrintL();
    }
    PrintHouse(2, fTrue);
    PrintTab(chH, WHEELCOLS-10-us.fVedic);
    PrintCh(chC);
    PrintTab(chH, wheelcols-1);
    PrintCh(chJS);
    PrintTab(chH, wheelcols-1);
    PrintCh(chC);
    PrintTab(chH, WHEELCOLS-10);
    PrintHouse(6, fFalse);
    PrintL();
    for (i = 0; i < us.nWheelRows; i++) {
        for (j = 1; j <= 4; j++) {
            PrintCh(chV);
            PrintWheelSlot(wheel[j][i]);
        }
        PrintCh(chV);
        PrintL();
    }
    PrintCh(chSW);
    PrintTab(chH, wheelcols-4);
    PrintHouse(3, fFalse);
    PrintTab(chH, WHEELCOLS-10);
    PrintHouse(4, fFalse);
    PrintTab(chH, WHEELCOLS-10);
    PrintHouse(5, fFalse);
    PrintTab(chH, WHEELCOLS-7);
    PrintCh(chSE);
    PrintL();
}


/* This is a subprocedure of ChartAspect() and ChartAspectRelation().       */
/* Display summary information about the aspect list, i.e. the total number */
/* of aspects of each type, and the number of aspects to each object, as    */
/* done when the -a0 aspect summary setting is set.                         */

void PrintAspectSummary(ca, co, count, rPowSum)
int *ca, *co, count;
real rPowSum;
{
    char sz[cchSzDef];
    int i, j, k;

    if (!us.fAspSummary)
        return;
    if (count == 0) {
        PrintSz("No aspects in list.\n");
        return;
    }
    PrintL();
    sprintf(sz, "Sum power: %.2f - Average power: %.2f\n",
            rPowSum, rPowSum/(real)count);
    PrintSz(sz);
    k = us.fParallel ? Min(us.nAsp, aOpp) : us.nAsp;
    for (j = 0, i = 1; i <= k; i++) if (!ignorea(i)) {
            if (!(j & 7)) {
                if (j)
                    PrintL();
            } else
                PrintSz("   ");
            AnsiColor(kAspA[i]);
            sprintf(sz, "%s:%3d", szAspectAbbrev[i], ca[i]);
            PrintSz(sz);
            j++;
        }
    PrintL();
    for (j = 0, i = 0; i <= cObjOpt; i++) if (!FIgnore(i)) {
            if (!(j & 7)) {
                if (j)
                    PrintL();
            } else
                PrintSz("   ");
            AnsiColor(kObjA[i]);
            sprintf(sz, "%c%c%c:%3d", chObj3(i), co[i]);
            PrintSz(sz);
            j++;
        }
    PrintL();
}


/* Display all aspects between objects in the chart, one per line, in       */
/* sorted order based on the total "power" of the aspect, as specified with */
/* the -a switch. The same influences used for -I charts are used here.     */

void ChartAspect()
{
    int ca[cAspect + 1], co[objMax];
    char sz[cchSzDef];
    int pcut = 300000000, icut=0, jcut=0, phi=0, ihi=0, jhi=0, ahi=0, p, i, j, k, count = 0;
    real ip, jp, rPowSum = 0.0;
    real power1[oNormOpt+1];

    PrintHeader();     /* Display chart info */

    for (i = 0; i <= oNormOpt; i++)
        power1[i] = rObjInf[i];

    PlanetPPower();
    for (i = 0; i <= oNormOpt; i++)
        power1[i] = ppower1[i];

    ClearB((lpbyte)ca, (cAspect + 1)*(int)sizeof(int));
    ClearB((lpbyte)co, objMax*(int)sizeof(int));
    loop {
        phi = -1;

        /* Search for the next most powerful aspect in the aspect grid. */

        for (i = 1; i <= cObjOpt; i++) if (!FIgnore(i))
                for (j = 0; j < i; j++) if (!FIgnore(j)&& !(FIgnoreS(i)&&FIgnoreS(j)))
                        if ((k = grid->n[j][i])) {
                            ip = (FStar(i)) ? 2.5 : (power1[i]*rObjInf[i]/13.0);
                            jp = FStar(j) ? 2.5 : (power1[j]*rObjInf[j]/13.0);
                            p = (int)(rAspInf[k]*(sqrt)(ip*jp)*
                                      (1.0-RAbs((real)(grid->v[j][i]))/3600.0/GetOrb(i, j, k))*500.0);
                            if (us.fParallel)
                                p = (int)(p * PowerPar);
                            if ((p < pcut || (p == pcut && (i > icut ||
                                                            (i == icut && j > jcut)))) && p > phi) {
                                ihi = i;
                                jhi = j;
                                phi = p;
                                ahi = k;
                            }
                        }
        if (phi < 0)    /* Exit when no less powerful aspect found. */
            break;
        pcut = phi;
        icut = ihi;
        jcut = jhi;
        count++;                               /* Display the current aspect.   */
#ifdef INTERPRET
        if (us.fInterpret && ihi <= oNorm && jhi <= oNorm) {                   /* Interpret it if -I in effect. */
            InterpretAspect(jhi, ihi);
        }
#endif
        rPowSum += (real)phi/500.0;
        ca[ahi]++;
        co[jhi]++;
        co[ihi]++;
        sprintf(sz, "%3d: ", count);
        PrintSz(sz);
        PrintAspect(jhi, SFromZ(planet[jhi]), (int)RSgn(ret[jhi]), ahi,
                    ihi, SFromZ(planet[ihi]), (int)RSgn(ret[ihi]), 'a');
        k = grid->v[jhi][ihi];
        AnsiColor(k > 0 ? kBlack : kWhite);
        sprintf(sz, us.fSeconds ? " - orb: %c%d%c%02d'%02d\""
                : " - orb: %c%d%c%02d'",

                us.fAppSep ? (k > 0 ? 's' : 'a') : (k < 0 ? '-' : '+'),
                        abs(k)/3600, chDeg1,(abs(k)%3600)/60 ,abs(k)%60);
        PrintSz(sz);
        AnsiColor(kDkGreen);
        /*    sprintf(sz, " - power:%6.2f\n", (real)phi/500.0); PrintSz(sz); */
        sprintf(sz, " - power:");
        PrintSz(sz);
        AnsiColor(k < 0 ? kWhite : kDkGreen);
        sprintf(sz, "%6.2f\n", (real)phi/500.0);
        PrintSz(sz);
        AnsiColor(kDefault);
    }

    PrintAspectSummary(ca, co, count, rPowSum);
    if (us.fAspSummary) {
        PrintL2();
        DisplayGrands();
    }
}


/* This is a subprocedure of ChartMidpoint() and ChartMidpointRelation().  */
/* Display summary information about the midpoint list, i.e. the total     */
/* number of midpoints in each sign, and their average span in degrees, as */
/* done when the -m0 midpoint summary setting is set.                      */

void PrintMidpointSummary(cs, count, lSpanSum)
int *cs, count;
long lSpanSum;
{
    char sz[cchSzDef];
    int m, i;

    if (!us.fMidSummary)
        return;
    if (count == 0) {
        PrintSz("No midpoints in list.\n");
        return;
    }
    PrintL();
    m = (int)(lSpanSum/count);
    sprintf(sz, "Average span:%4d%c%02d'\n", m/60, chDeg1, m%60);
    PrintSz(sz);
    for (i = 1; i <= cSign; i++) {
        if (i == sLib)
            PrintL();
        else if (i != sAri)
            PrintSz("   ");
        AnsiColor(kSignA(i));
        sprintf(sz, "%c%c%c:%3d", chSig3(i), cs[i]);
        PrintSz(sz);
    }
    PrintL();
}


/* Display locations of all midpoints between objects in the chart, */
/* one per line, in sorted zodiac order from zero Aries onward, as  */
/* specified with the -m switch.                                    */

void ChartMidpoint()
{
    int cs[cSign + 1];
    char sz[cchSzDef];
    int mcut = -1, icut=0, jcut=0, mlo, ilo=0, jlo=0, m, i, j, k, count = 0;
    long lSpanSum = 0;
    real l, n, mid;
/*    CI ciT; */

    ClearB((lpbyte)cs, (cSign + 1)*(int)sizeof(int));
    is.fSeconds = fFalse;
    loop {
        mlo = 21600;

        /* Search for the next closest midpoint farther down in the zodiac. */

        for (i = 0; i < cObjOpt; i++) if (!FIgnore(i))
                for (j = i+1; j <= cObjOpt; j++) if (!FIgnore(j)) {
                        m = (grid->n[j][i]-1)*30*60 + grid->v[j][i];
                        if ((m > mcut || (m == mcut && (i > icut ||
                                                        (i == icut && j > jcut)))) && m < mlo) {
                            ilo = i;
                            jlo = j;
                            mlo = m;
                        }
                    }
        if (mlo >= 21600)    /* Exit when no midpoint farther in zodiac found. */
            break;
        mcut = mlo;
        icut = ilo;
        jcut = jlo;
        count++;                               /* Display the current midpoint. */
#ifdef INTERPRET
        if (us.fInterpret) {                   /* Interpret it if -I in effect. */
            InterpretMidpoint(ilo, jlo);
            continue;
        }
#endif
        cs[mlo/60/30+1]++;
        sprintf(sz, "%4d: ", count);
        PrintSz(sz);
        mid = (real)mlo/60.0;
        PrintZodiac(mid);
        PrintCh(' ');
        PrintAspect(ilo, SFromZ(planet[ilo]), (int)RSgn(ret[ilo]), 0,
                    jlo, SFromZ(planet[jlo]), (int)RSgn(ret[jlo]), 'm');
        AnsiColor(kDefault);
        m = (int)(MinDistance(planet[ilo], planet[jlo])*3600.0+rRound);
        lSpanSum += m;
        sprintf(sz, us.fSeconds ? "-%4d%c%02d'%02d\" degree span.\n":"-%4d%c%02d' degree span.\n", m/3600, chDeg1, (m%3600)/60,m%60);
        PrintSz(sz);

        /* If the -ma switch is set, determine and display each aspect from one */
        /* of the planets to the current midpoint, and the aspect's orb.        */

        if (!us.fMidAspect)
            continue;
        if (us.nRel==rcSynastry){
            cp2=cp0;
            cp0=cp1;
            cp1=cp2;
        }
        for (i = 0; i <= cObjOpt; i++) if (!FIgnore(i)) {
                l = MinDistance(planet[i], mid);
                for (k = us.nAsp; k >= 1; k--) {
                    if (!FAcceptAspect(i, k, ilo))
                        continue;
                    n = l-rAspAngle[k];
                    if (RAbs(n) < GetOrb(i, ilo, k)) {
                        if (us.fAppSep)
                            n = RSgn2((ret[ilo]+ret[jlo])/2.0-ret[i])*
                                RSgn2(MinDifference(planet[i], mid))*RSgn2(n)*RAbs(n);
                        PrintSz("      Midpoint ");
                        PrintZodiac(mid);
                        PrintSz(" makes ");
                        AnsiColor(kAspA[k]);
                        PrintSz(szAspectAbbrev[k]);
                        AnsiColor(kDefault);
                        PrintSz(" to ");
                        AnsiColor(kObjA[i]);
                        sprintf(sz, "%.10s", szObjName[i]);
                        PrintSz(sz);
                        PrintTab(' ', 10-CchSz(szObjName[i]));
                        j = (int)(n*3600.0+rRound);
                        AnsiColor(j < 0 ? kWhite : kDkGreen);
                        sprintf(sz, "- orb: %c%d%c%02d'%02d\"",
                                us.fAppSep ? (j < 0 ? 'a' : 's') : (j < 0 ? '-' : '+'),
                                        abs(j)/3600, chDeg1,(abs(j)%3600)/60 ,abs(j)%60);
                        PrintSz(sz);
                        PrintL();
                        AnsiColor(kDefault);
                    }
                }
            }
        if (us.nRel==rcSynastry){
            cp2=cp0;
            cp0=cp1;
            cp1=cp2;
        }
    }
    is.fSeconds = us.fSeconds;

    PrintMidpointSummary(cs, count, lSpanSum);
}


/* Display locations of the objects on the screen with respect to the local */
/* horizon, as specified with the -Z switch.                                */

void ChartHorizon()
{
    char sz[cchSzDef], szFormat[cchSzDef];
    real lon, lat, sx, sy, vx, vy,
    lonz[objMax], latz[objMax], azi[objMax], alt[objMax];
    int fPrime, i, j, k, tot;
    real lonMC;

    PrintHeader();
    PrintL();    /* Display chart info */

    /* Set up some initial variables. */

    fPrime = us.fPrimeVert;
    lon = RFromD(Mod(DecToDeg(OO)));
    lat = RFromD(DecToDeg(AA));
    tot = us.nStar ? cObj : oNormOpt;

    /* First find zenith location on Earth of each object. */

    for (i = 0; i <= tot; i++) if (!ignore[i] || i <= oMoo||i==oMC) {
            if (us.fEquator) {
                lonz[i] = RFromD(planet[i]);
                latz[i] = RFromD(planetalt[i]);
            } else {
                lonz[i] = RFromD(Tropical(planet[i]));
                latz[i] = RFromD(planetalt[i]);
                EclToEqu(&lonz[i], &latz[i]);
            }
        }
    if (PolarMCflip && hRevers)
        lonMC = Mod(lonz[oMC]+rPi);
    else
        lonMC = lonz[oMC];

    /* Then, convert this to local horizon altitude and azimuth. */

    for (i = 1; i <= tot; i++) if (!ignore[i] ||i<=oMoo||i==oMC) {
            lonz[i] = ModRad(lonMC-lonz[i]+rPiHalf);
            if (!fEquator)
                EquToLocal(&lonz[i], &latz[i], rPiHalf-lat);
            azi[i] = rDegMax-DFromR(lonz[i]);
            alt[i] = DFromR(latz[i]);
        }

    /* If the -YTF switch flag is in effect,consider refraction */
#ifdef PLACALC
    if (us.fRefraction) {
        for (i = 1; i <= tot; i++) if (!ignore[i] && i != oMC)
                alt[i]=FPlacalcRefrac(i,alt[i],0);
    }
#endif

    /* If the -Z0 switch flag is in effect, convert from altitude/azimuth  */
    /* coordinates to prime vertical coordinates that we'll print instead. */

    if (fPrime) {
        for (i = 1; i <= tot; i++) if (!ignore[i] && i != oMC) {
                azi[i] = RFromD(azi[i]);
                alt[i] = RFromD(alt[i]);
                CoorXform(&azi[i], &alt[i], rPiHalf);
                azi[i] = DFromR(azi[i]);
                alt[i] = DFromR(alt[i]);
            }
    }
    /* Now, actually print the location of each object. */

    sprintf(szFormat, is.fSeconds ? " " : "");
    if (fPrime) {
        sprintf(sz, "Body      %s%sAmplitude%s %s%sPrm-Vrt%s%s  Azi. Vector%s    ",
                szFormat, szFormat, szFormat, szFormat, szFormat, szFormat, szFormat,
                szFormat);
        PrintSz(sz);
    } else {
        sprintf(sz, "Body      %s%sAltitude%s %s%sAzimuth%s%s  Azi. Vector%s    ",
                szFormat, szFormat, szFormat, szFormat, szFormat, szFormat, szFormat,
                szFormat);
        PrintSz(sz);
    }
    sprintf(sz, "%s Vector%s%s    Moon Vector\n\n",
            us.objCenter != oSun ? "Sun" : "Earth", szFormat, szFormat);
    PrintSz(sz);
    for (k = 1; k <= tot; k++) {
        i = k <= oNorm ? k : oNorm+starname[k-oNorm];
        if (ignore[i] /*|| !FThing(i)*/)
            continue;
        AnsiColor(kObjA[i]);
        sprintf(sz, "%-11.11s: ", szObjName[i]);
        PrintSz(sz);
        PrintAltitude(alt[i]);

        /* Determine directional vector based on azimuth. */
        if (fNESW && !fPrime) {
            sprintf(sz, " %s", SzDegree(Mod(rDegQuad-azi[i])));
            PrintSz(sz);
        } else {
            sprintf(sz, " %s", SzDegree(azi[i]));
            PrintSz(sz);
        }
        sx = RCos(RFromD(azi[i]));
        sy = RSin(RFromD(azi[i]));
        if (RAbs(sx) < RAbs(sy)) {
            vx = RAbs(sx / sy);
            vy = 1.0;
        } else {
            vy = RAbs(sy / sx);
            vx = 1.0;
        }
        sprintf(sz, is.fSeconds ? " (%.3f%c" : " (%.2f%c", vy,
                sy < 0.0 ? (fPrime ? 'u' : 's') : (fPrime ? 'd' : 'n'));
        PrintSz(sz);
        sprintf(sz, is.fSeconds ? " %.2f%c)" : " %.2f%c)", vx,
                sx > 0.0 ? 'e' : 'w');
        PrintSz(sz);

        /* Determine distance vector of current object from Sun and Moon. */

        vx = azi[1]-azi[i];
        vy = azi[2]-azi[i];
        j = 1 + is.fSeconds;
        sprintf(szFormat, " [%%%d.%df%%%d.%df] [%%%d.%df%%%d.%df]",
                j+5, j, j+5, j, j+5, j, j+5, j);
        sprintf(sz, szFormat,
                RAbs(vx) < rDegHalf ? vx : RSgn(vx)*(rDegMax-RAbs(vx)), alt[1]-alt[i],
                RAbs(vy) < rDegHalf ? vy : RSgn(vy)*(rDegMax-RAbs(vy)), alt[2]-alt[i]);
        PrintSz(sz);
        if (!is.fSeconds && i >= CentLo) {
            if (FBetween(i,HypuranLo,HypuranHi))
                sprintf(sz, "  Uran. #%d", i-HypuranLo+1);
#ifdef ASTEROIDS
            else if (FBetween(i,CentLo,CentHi))
                sprintf(sz, "  Centaur #%d", i-CentLo+2);
            else if (FBetween(i,TrneLo,TrneHi))
                sprintf(sz, "  TNO #%d", i-TrneLo+1);
            else if (FBetween(i,AtenLo,AtenHi))
                sprintf(sz, "  Aten #%d", i-AtenLo+1);
            else if (FBetween(i,ApolLo,ApolHi))
                sprintf(sz, "  Apoll #%d", i-ApolLo+1);
            else if (FBetween(i,AmorLo,AmorHi))
                sprintf(sz, "  Amor #%d", i-AmorLo+1);
            else if (FBetween(i,MuseLo,MuseHi))
                sprintf(sz, "  Muse #%d", i-MuseLo+1);
            else if (FBetween(i,AgenLo,AgenHi))
                sprintf(sz, "  Asteroid #%d", i-AgenLo+5);
            else if (FBetween(i,AsteLo,AsteHi))
                sprintf(sz, "  Extra Asteroid #%d", i-AsteLo+1);
#endif
            else
                sprintf(sz, "  Star #%2d", i-starLo+1);
            PrintSz(sz);
        }
        PrintL();
    }
    AnsiColor(kDefault);
}


/* Display x,y,z locations of each body (in AU) with respect to the Sun */
/* (or whatever the specified center planet is), as in the -S switch.   */
/* These values were already determined when calculating the planet     */
/* positions themselves, so this procedure is basically just a loop.    */

void ChartOrbit()
{
    char sz[cchSzDef], szFormat[cchSzDef];
    real x, y, z,dx,dy,dz;
    int i;
    PrintHeader();
    sprintf(szFormat, is.fSeconds ? " " : "");
    sprintf(sz, "Body%s    Angle%s%s%s%s    ",
            szFormat, szFormat, szFormat, szFormat, szFormat);
    PrintSz(sz);
    sprintf(sz,
            "%sX axis%s%s%s    %sY axis%s%s%s    %sZ axis%s%s%s    %sLength\n",
            szFormat, szFormat, szFormat, szFormat, szFormat, szFormat, szFormat,
            szFormat, szFormat, szFormat, szFormat, szFormat, szFormat);
    PrintSz(sz);
    for (i = 0; i <= oNorm; i++) {
        if (ignore[i] || (!FThing(i) ||
                          ((i == oMoo || i == oNod || i == oSou) && !us.fPlacalc)))
            continue;
        AnsiColor(kObjA[i]);
        sprintf(sz, "%c%c%c%c: ", chObj3(i),
                szObjName[i][3] ? szObjName[i][3] : ' ');
        PrintSz(sz);
        x = spacex[i];
        y = spacey[i];
        z = spacez[i];
        dx = spacedx[i];
        dy = spacedy[i];
        dz = spacedz[i];
        sprintf(sz, is.fSeconds ? "[%11.7f] [%11.7f] [%11.7f] [%11.7f] [%11.7f]" :
                "[%7.3f] [%7.3f] [%7.3f] [%7.3f] [%7.3f]",
                planet[i], x, y, z, RSqr(x*x+y*y+z*z));
        PrintSz(sz);
        if (us.fDebug){
            sprintf(sz, is.fSeconds ? "[%11.8f] [%11.8f] [%11.8f] [%11.8f] [%11.8f]" :
                    "[%7.4f] [%7.4f] [%7.4f] [%7.4f] [%7.4f]",
                    ret[i], dx, dy, dz, RSqr(dx*dx+dy*dy+dz*dz));
            PrintSz(sz);
        }
        if (!is.fSeconds && i >= uranLo) {
            sprintf(sz, "  Uranian #%d", i-uranLo+1);
            PrintSz(sz);
        }
        PrintL();
    }
    AnsiColor(kDefault);
}


/* Display locations of the planets on the screen with respect to the 36    */
/* Gauquelin sectors and their plus zones, as specified with the -l switch. */

void ChartSector()
{
    char sz[cchSzDef];
    CP cp;
    int c[cSector + 1], i, sec, pls, kPls, cpls = 0, co = 0, cq = 0;
    real r, rT;
    char ignoreT[objMax+1];

    for (i = 1; i <= objMax; i++)
        ignoreT[i] = ignore[i];

    for (i = 1; i <= cSector; i++) {
        c[i] = 0;
        cpls += pluszone[i];
    }
    cp = cp0;
    CastSectors();
    if (fSecMotion)
       cp = cp0;
    AnsiColor(kDkGray);
    PrintSz("Body  Sector ");
    if (is.fSeconds)
        PrintSz(
            "  Plus      House   Sign Loc. Ret. Latitude Velocity Sec.18 Sec.12\n");
    else
        PrintSz("Plus    House   Sign Ret. Latit. Veloc. 18 12\n");
    for (i = 0; i <= cObjOpt; i++) {
        if (FIgnore(i) || !FThing(i))
            continue;
        co++;
        AnsiColor(kObjA[i]);
        sprintf(sz, "%c%c%c%c: ", chObj3(i),
                szObjName[i][3] ? szObjName[i][3] : ' ');
        PrintSz(sz);
        r = GFromO(i);
        sec = (int)r + 1;
        c[sec]++;
        pls = pluszone[sec];
        cq += pls;
        kPls = (pls ? kRainbowA[1] : kMainA[5]);
        AnsiColor(kDkGray);
        PrintSz("Sec");
        AnsiColor(kPls);
        sprintf(sz, " %2d", sec);
        PrintSz(sz);
        if (is.fSeconds) {
            AnsiColor(kDkGray);
            sprintf(sz, "%.3f", RFract(r));
            PrintSz(&sz[1]);
            AnsiColor(kPls);
        }
        sprintf(sz, " %c", (char)(pls ? '+' : '-'));
        PrintSz(sz);
        AnsiColor(kSignA(cp.house[i]));
        sprintf(sz, " [%2d%s house] ", cp.house[i], szSuffix[cp.house[i]]);
        PrintSz(sz);
        PrintZodiac(cp.obj[i]);
        sprintf(sz, " %c ", cp.dir[i] < 0.0 ? chRet : ' ');
        PrintSz(sz);
        PrintAltitude(cp.alt[i]);
        PrintCh(' ');
        if ((i != oMoo || us.fPlacalc) &&
                (FObject(i) || ((i == oNod || i == oLil) && us.fPlacalc))) {
            PrintCh((char)(ret[i] < 0.0 ? '-' : '+'));
            rT = DFromR(RAbs(ret[i]));
            sprintf(sz, is.fSeconds ? (rT < 10.0 ? "%7.5f" : "%7.4f") :
                            (rT < 10.0 ? "%5.3f" : "%5.2f"), rT);
            PrintSz(sz);
        } else
            PrintTab('_', us.fSeconds ? 8 : 6);
        AnsiColor(kPls);
        sprintf(sz, " %2d", (sec-1)/2 + 1);
        PrintSz(sz);
        if (is.fSeconds) {
            AnsiColor(kDkGray);
            sprintf(sz, "%.3f", RFract(r/2.0));
            PrintSz(&sz[1]);
        }
        AnsiColor(kPls);
        sprintf(sz, " %2d", (sec-1)/3 + 1);
        PrintSz(sz);
        if (is.fSeconds) {
            AnsiColor(kDkGray);
            sprintf(sz, "%.3f", RFract(r/3.0));
            PrintSz(&sz[1]);
        }
        PrintL();
    }

    /* Display summary information, i.e. the planet in plus zone ratio. */

    AnsiColor(kDefault);
    sprintf(sz, "\nPlus zones: %d/%d = %.2f%% - ", cpls, cSector,
            (real)cpls/(real)36*100.0);
    PrintSz(sz);
    sprintf(sz, "Planets in plus zones: %d/%d = %.2f%%\n", cq, co,
            co ? (real)cq/(real)co*100.0 : 0.0);
    PrintSz(sz);

    /* Display more summary information, i.e. the number of planets in each */
    /* sector, as well as whether each sector is a plus zone or not.        */

    PrintSz("\nZone:");
    for (i = 1; i <= cSector/2; i++) {
        pls = pluszone[i];
        AnsiColor(pls ? kRainbowA[1] : kMainA[5]);
        sprintf(sz, " %2d%c", i, pls ? '+' : '-');
        PrintSz(sz);
    }
    AnsiColor(kDefault);
    PrintSz("\nNum :");
    for (i = 1; i <= cSector/2; i++) {
        if (c[i]) {
            sprintf(sz, " %2d ", c[i]);
            PrintSz(sz);
        } else
            PrintSz("  . ");
    }
    PrintSz("\nZone:");
    for (i = cSector; i > cSector/2; i--) {
        pls = pluszone[i];
        AnsiColor(pls ? kRainbowA[1] : kMainA[5]);
        sprintf(sz, " %2d%c", i, pls ? '+' : '-');
        PrintSz(sz);
    }
    AnsiColor(kDefault);
    PrintSz("\nNum :");
    for (i = cSector; i > cSector/2; i--) {
        if (c[i]) {
            sprintf(sz, " %2d ", c[i]);
            PrintSz(sz);
        } else
            PrintSz("  . ");
    }
    PrintL();
    for (i = 1; i <= objMax; i++)
        ignore[i] = ignoreT[i];
    CastChart(fTrue);
}


/* Print the locations of the astro-graph lines on the Earth as specified */
/* with the -L switch. This includes Midheaven and Nadir lines, zenith    */
/* positions, and locations of Ascendant and Descendant lines.            */

void ChartAstroGraph()
{
    CrossInfo FPTR *c=0;
    char sz[cchSzDef];
    real planet1[objMax], planet2[objMax], mc[objMax], ic[objMax],
    as[objMax], ds[objMax], as1[objMax], ds1[objMax],
    lo = Lon, longm, w, x, y, z, ad, oa, am, od, dm;
    int cCross = 0, tot = cObjOpt, i, j, k, l, m, n;

    if (us.fLatitudeCross)
    {
        if ((c = (CrossInfo FPTR *)
                 PAllocate(sizeof(CrossInfo), fFalse, "crossing table")) == NULL)
            return;
    }

#ifdef MATRIX
    for (i = 1; i <= cObjOpt; i++) if (!ignore[i] || i == oMC) {
            planet1[i] = RFromD(Tropical(i == oMC ? is.MC : planet[i]));
            planet2[i] = RFromD(planetalt[i]);     /* Calculate zenith location on */
            EclToEqu(&planet1[i], &planet2[i]);    /* Earth of each object.        */
        }

    /* Print header. */

    PrintSz("Object :");
    for (j = 0, i = 1; i <= cObjOpt; i++)
        if (!ignore[i] && FThing(i)) {
            AnsiColor(kObjA[i]);
            sprintf(sz, " %c%c%c", chObj3(i));
            PrintSz(sz);
            j++;
        }
    AnsiColor(kDefault);
    PrintSz("\n------ :");
    for (i = 1; i <= tot; i++)
        if (!ignore[i] && FThing(i))
            PrintSz(" ###");

    /* Print the longitude locations of the Midheaven lines. */

    PrintSz("\nMidheav: ");
    if (lo < 0.0)
        lo += rDegMax;
    for (i = 1; i <= tot; i++)
        if (!ignore[i] && FThing(i)) {
            AnsiColor(kObjA[i]);
            x = planet1[oMC]-planet1[i];
            if (x < 0.0)
                x += 2.0*rPi;
            if (x > rPi)
                x -= 2.0*rPi;
            z = lo+DFromR(x);
            if (z > rDegHalf)
                z -= rDegMax;
            mc[i] = z;
            sprintf(sz, "%3.0f%c", RAbs(z), z < 0.0 ? 'e' : 'w');
            PrintSz(sz);
        }
    AnsiColor(kDefault);

    /* The Nadir lines are just always 180 degrees away from the Midheaven. */

    PrintSz("\nNadir  : ");
    for (i = 1; i <= tot; i++)
        if (!ignore[i] && FThing(i)) {
            AnsiColor(kObjA[i]);
            z = mc[i] + rDegHalf;
            if (z > rDegHalf)
                z -= rDegMax;
            ic[i] = z;
            sprintf(sz, "%3.0f%c", RAbs(z), z < 0.0 ? 'e' : 'w');
            PrintSz(sz);
        }
    AnsiColor(kDefault);

    /* Print the Zenith latitude locations. */

    PrintSz("\nZenith : ");
    for (i = 1; i <= tot; i++)
        if (!ignore[i] && FThing(i)) {
            AnsiColor(kObjA[i]);
            y = DFromR(planet2[i]);
            sprintf(sz, "%3.0f%c", RAbs(y), y < 0.0 ? 's' : 'n');
            PrintSz(sz);
            as[i] = ds[i] = as1[i] = ds1[i] = rLarge;
        }
    PrintL2();

    /* Now print the locations of Ascendant and Descendant lines. Since these */
    /* are curvy, we loop through the latitudes, and for each object at each  */
    /* latitude, print the longitude location of the line in question.        */

    longm = RFromD(Mod(DFromR(planet1[oMC])+lo));
    for (j = 80; j >= -80; j -= us.nAstroGraphStep) {
        AnsiColor(kDefault);
        sprintf(sz, "Asc@%2d%c: ", j >= 0 ? j : -j, j < 0 ? 's' : 'n');
        PrintSz(sz);
        for (i = 1; i <= tot; i++)
            if (!ignore[i] && FThing(i)) {
                AnsiColor(kObjA[i]);
                ad = RTan(planet2[i])*RTan(RFromD(j));
                if (ad*ad > 1.0) {
                    PrintSz(" -- ");
                    as1[i] = ds1[i] = cp2.dir[i] = rLarge;
                } else {
                    ad = RAsin(ad);
                    oa = planet1[i]-ad;
                    if (oa < 0.0)
                        oa += 2.0*rPi;
                    am = oa-rPiHalf;
                    if (am < 0.0)
                        am += 2.0*rPi;
                    z = longm-am;
                    if (z < 0.0)
                        z += 2.0*rPi;
                    if (z > rPi)
                        z -= 2.0*rPi;
                    as1[i] = as[i];
                    as[i] = z = DFromR(z);
                    cp2.dir[i] = ad;
                    sprintf(sz, "%3.0f%c", RAbs(z), z < 0.0 ? 'e' : 'w');
                    PrintSz(sz);
                }
            }

        /* Again, the Descendant position is related to the Ascendant's,  */
        /* being a mirror image, so it can be calculated somewhat easier. */

        AnsiColor(kDefault);
        sprintf(sz, "\nDsc@%2d%c: ", j >= 0 ? j : -j, j < 0 ? 's' : 'n');
        PrintSz(sz);
        for (i = 1; i <= tot; i++)
            if (!ignore[i] && FThing(i)) {
                AnsiColor(kObjA[i]);
                ad = cp2.dir[i];
                if (ad == rLarge)
                    PrintSz(" -- ");
                else {
                    od = planet1[i]+ad;
                    dm = od+rPiHalf;
                    z = longm-dm;
                    if (z < 0.0)
                        z += 2.0*rPi;
                    if (z > rPi)
                        z -= 2.0*rPi;
                    ds1[i] = ds[i];
                    ds[i] = z = DFromR(z);
                    sprintf(sz, "%3.0f%c", RAbs(z), z < 0.0 ? 'e' : 'w');
                    PrintSz(sz);
                }
            }
        PrintL();
#endif /* MATRIX */

        /* Now, if the -L0 switch is in effect, then take these line positions, */
        /* which we saved in an array above as we were printing them, and       */
        /* calculate and print the latitude crossings.                          */

        if (us.fLatitudeCross)
            for (l = 1; l <= cObjOpt; l++) if (!ignore[l] && FThing(l))
                    for (k = 1; k <= cObjOpt; k++) {
                        if (ignore[k] || !FThing(k))
                            continue;
                        for (n = 0; n <= 1; n++) {
                            x = n ? ds1[l] : as1[l];
                            y = n ? ds[l] : as[l];
                            for (m = 0; m <= 1; m++) {

                                /* Check if Ascendant/Descendant cross Midheaven/Nadir. */

                                z = m ? ic[k] : mc[k];
                                if (cCross < MAXCROSS &&
                                        RAbs(x-y) < rDegHalf && RSgn(z-x) != RSgn(z-y)) {
                                    c->obj1[cCross] = n ? -l : l;
                                    c->obj2[cCross] = m ? -k : k;
                                    c->lat[cCross] = (real)j+5.0*RAbs(z-y)/RAbs(x-y);
                                    c->lon[cCross] = z;
                                    cCross++;
                                }

                                /* Check if Ascendant/Descendant cross another Asc/Des. */

                                w = m ? ds1[k] : as1[k];
                                z = m ? ds[k] : as[k];
                                if (cCross < MAXCROSS && k > l &&
                                        RAbs(x-y)+RAbs(w-z) < rDegHalf && RSgn(w-x) != RSgn(z-y)) {
                                    c->obj1[cCross] = n ? -l : l;
                                    c->obj2[cCross] = 100+(m ? -k : k);
                                    c->lat[cCross] = (real)j+5.0*
                                                     RAbs(y-z)/(RAbs(x-w)+RAbs(y-z));
                                    c->lon[cCross] = Min(x, y)+RAbs(x-y)*
                                                     RAbs(y-z)/(RAbs(x-w)+RAbs(y-z));
                                    cCross++;
                                }
                            }
                        }
                    }
    }
    if (!us.fLatitudeCross)
        return;
    PrintL();

    /* Now, print out all the latitude crossings we found.  */
    /* First, we sort them in order of decreasing latitude. */

    for (i = 1; i < cCross; i++) {
        j = i-1;
        while (j >= 0 && c->lat[j] < c->lat[j+1]) {
            SwapN(c->obj1[j], c->obj1[j+1]);
            SwapN(c->obj2[j], c->obj2[j+1]);
            SwapR(&c->lat[j], &c->lat[j+1]);
            SwapR(&c->lon[j], &c->lon[j+1]);
            j--;
        }
    }
    for (i = 1; i < cCross; i++) {
        j = abs(c->obj1[i]);
        AnsiColor(kObjA[j]);
        sprintf(sz, "%c%c%c ", chObj3(j));
        PrintSz(sz);
        AnsiColor(kElemA[c->obj1[i] > 0 ? eFir : eAir]);
        PrintSz(c->obj1[i] > 0 ? "Ascendant " : "Descendant");
        AnsiColor(kWhite);
        PrintSz(" crosses ");
        j = abs(c->obj2[i] - (c->obj2[i] < 50 ? 0 : 100));
        AnsiColor(kObjA[j]);
        sprintf(sz, "%c%c%c ", chObj3(j));
        PrintSz(sz);
        AnsiColor(kElemA[c->obj2[i] < 50 ?
                         (c->obj2[i] > 0 ? eEar : eWat) : (c->obj2[i] > 100 ? eFir : eAir)]);
        sprintf(sz, "%s ", c->obj2[i] < 50 ? (c->obj2[i] > 0 ? "Midheaven " :
                                              "Nadir     ") : (c->obj2[i] > 100 ? "Ascendant " : "Descendant"));
        PrintSz(sz);
        AnsiColor(kDefault);
        sprintf(sz, "at %s%c,", SzDegree(c->lon[i]),
                c->lon[i] < 0.0 ? 'E' : 'W');
        PrintSz(sz);
        j = (int)(RFract(RAbs(c->lat[i]))*60.0);
        sprintf(sz, "%s%c\n", SzDegree(c->lat[i]),
                c->lat[i] < 0.0 ? 'S' : 'N');
        PrintSz(sz);
    }
    DeallocateFar(c);
    if (!cCross) {
        AnsiColor(kDefault);
        PrintSz("No latitude crossings.\n");
    }
}


/* Another important procedure: Display any of the types of (text) charts    */
/* that the user specified they wanted, by calling the appropriate routines. */

void PrintChart(fProg)
bool fProg;
{
    int i;
    CI ci;
    int fCall = fFalse;
    if (us.fListing) {
        if (is.fMult)
            PrintL2();
        if (fReturnChart){
            ciCore=ciTwin;
            CastChart(fTrue);
        }
        if (us.nRel < rcDifference)
            ChartListing();
        else

            /* If the -rb or -rd relationship charts are in effect, then instead  */
            /* of doing the standard -v chart, print either of these chart types. */

            DisplayRelation();
        is.fMult = fTrue;
    }
    if (us.fWheel) {
        if (is.fMult)
            PrintL2();
        ChartWheel();
        is.fMult = fTrue;
    }
    if (us.fGrid) {
        if (is.fMult)
            PrintL2();
        if (us.nRel > rcDual) {
            fCall = us.fSmartCusp;
            us.fSmartCusp = fFalse;
            if (!FCreateGrid(fFalse))
                return;
            us.fSmartCusp = fCall;
            not2(fCall);
            ChartGrid();
            if (us.fGridConfig) {    /* If -g0 switch in effect, then  */
                PrintL();              /* display aspect configurations. */
                if (!fCall)
                    FCreateGrid(fFalse);
                DisplayGrands();
            }
        } else {

            /* Do a relationship aspect grid between two charts if -r0 in effect. */

            fCall = us.fSmartCusp;
            us.fSmartCusp = fFalse;
            if (!FCreateGridRelation(us.fGridConfig))
                return;
            us.fSmartCusp = fCall;
            ChartGridRelation();
        }
        is.fMult = fTrue;
    }
    if (us.fAspList) {
        if (is.fMult)
            PrintL2();
        if (us.nRel > rcDual) {
            if (!fCall) {
                fCall = fTrue;
                if (!FCreateGrid(fFalse))
                    return;
            }
            ChartAspect();

        } else {

            if (!FCreateGridRelation(fFalse))
                return;
            ChartAspectRelation();
        }
        is.fMult = fTrue;
    }
    if (us.fMidpoint) {
        if (is.fMult)
            PrintL2();
        if (us.nRel > rcDual) {
            if (!fCall) {
                if (!FCreateGrid(fFalse))
                    return;
            }
            ChartMidpoint();
        } else {
            if (!FCreateGridRelation(fTrue))
                return;
            ChartMidpointRelation();
        }
        is.fMult = fTrue;
    }
    if (us.fHorizon) {
        if (is.fMult)
            PrintL2();
        if (us.fHorizonSearch)
            ChartInDayHorizon();
        else
            ChartHorizon();
        is.fMult = fTrue;
    }
    if (us.fOrbit) {
        if (is.fMult)
            PrintL2();
        ChartOrbit();
        is.fMult = fTrue;
    }
    if (us.fSector&&!fSynodicReturnChart&&!us.fTransit&&!us.fInDay&&!us.fTransitInf) {
        if (is.fMult)
            PrintL2();
        else
            PrintHeader();    /* Print chart header if it hasn't been done yet. */
        ChartSector();
        is.fMult = fTrue;
    } else if (us.fSector && fSynodicReturnChart && returnnatalpos < -377.0)
      { 
         ChartPrimaryDirectionSet();
      }
    if (us.fInfluence) {
        if (is.fMult)
            PrintL2();
        ChartInfluence();
        is.fMult = fTrue;
    }
    if (us.fAstroGraph) {
        if (is.fMult)
            PrintL2();
        ChartAstroGraph();
        is.fMult = fTrue;
    }
    if (us.fCalendar) {
        if (is.fMult)
            PrintL2();
        if (us.fCalendarYear)
            ChartCalendarYear();
        else
            ChartCalendarMonth();
        is.fMult = fTrue;
    }
    if (us.fInDay) {
        if (is.fMult)
            PrintL2();
        if (us.fAspect3D)
            ChartInDaySearch3D(fProg);
        else
            ChartInDaySearch(fProg);
        is.fMult = fTrue;
    }
    if (us.fInDayInf) {
        if (is.fMult)
            PrintL2();
        ChartInDayInfluence();
        is.fMult = fTrue;
    }
    if (us.fEphemeris) {
        if (is.fMult)
            PrintL2();
        ChartEphemeris();
        is.fMult = fTrue;
    }
    if (us.fEclipse) {
        if (is.fMult)
            PrintL2();
        if (fReturnChart)
            ChartReturn();
        else
            ChartEclipse();
        is.fMult = fTrue;
    }
#ifdef ASTEROIDS
    if (us.fSolex) {
        if (is.fMult)
            PrintL2();
        PrintSolex();
        is.fMult = fTrue;
    }
    /* if (us.fSolexConv) {
      if (is.fMult)
        PrintL2();
      ConvertSolex();
      is.fMult = fTrue;
    }*/
#endif
    if (us.fNodelist) {
        if (is.fMult)
            PrintL2();
        ChartNodeListing();
        is.fMult = fTrue;
    }
    if (us.fTransit) {
        if (is.fMult)
            PrintL2();
        if (us.fAspect3D)
            ChartTransitSearch3D(fProg);
        else
            ChartTransitSearch(fProg);
        is.fMult = fTrue;
    }
    if (us.fTransitInf) {
        if (is.fMult)
            PrintL2();
        if (us.fContinued){
            PlanetPPower();
            for (i=1; i<=us.ftransIter ; i++){
                if (us.fTransProg) {
                    ciCore=ciMain;
                    is.JDp = MdytszToJulian(MonT, DayT, YeaT, TimT, DstT, ZonT)-rRound;
                    us.fProgress = fTrue;
                    CastChart(fTrue);
                    us.fProgress = fFalse;
                }
                if (us.fInfSummary)
                    sumChartTransitInfluence(is.fProgress,fTrue);
                else
                    ChartTransitInfluence(is.fProgress,fTrue);
                ci=ciCore;
                ciCore=ciTran;
                if (abs(us.ftransStep) > 23)
                    AddTime(4,us.ftransStep / 24);
                AddTime(3,us.ftransStep % 24);
                ciTran=ciCore;
                ciCore=ci;
                if (us.nRel)
                    CastRelation();
            }
        }
        else {
            if (us.fInfSummary) sumChartTransitInfluence(is.fProgress,fFalse);
            else ChartTransitInfluence(is.fProgress,fFalse);
        }
        is.fMult = fTrue;
    }
#ifdef ARABIC
    if (us.nArabic) {
        if (is.fMult)
            PrintL2();
        DisplayArabic();
        is.fMult = fTrue;
    }
#endif

    if (!is.fMult) {          /* Assume the -v chart if user */
        us.fListing = fTrue;    /* didn't indicate anything.   */
        PrintChart(fProg);
        is.fMult = fTrue;
    }
}

/* charts1.c */
