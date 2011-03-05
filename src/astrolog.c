/*
** Astrolog (Version 5.42J) File: astrolog.c
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
#include "string.h"

/*
******************************************************************************
** Program Dispatch Procedures.
******************************************************************************
*/

/* Initialize the Ansi color arrays with the color to print each object in. */

void InitColors()
{
    int i;

    kObjA[0] = kElemA[eEar];
    for (i = 1; i <= 10; i++)
        kObjA[i] = kSignA(ruler1[i]);
    kObjA[oChi]=kRainbowA[4];
    for (i = 12; i <= 15; i++)
        kObjA[i] = kMainA[8];
    for (i = 16; i <= 20; i++)
        kObjA[i] = kMainA[6];
    for (i = 1; i <= cSign; i++)
        kObjA[cuspLo+i-1] = kSignA(i);
    for (i = TerranLo; i <= TerranHi; i++)
        kObjA[i] = kMainA[6];
#ifdef ASTEROIDS
    for (i = CentLo; i <= CentHi; i++)
        kObjA[i] = kRainbowA[4];
    for (i = TrneLo; i <= TrneHi; i++)
        kObjA[i] = kMainA[5];
    for (i = AgenLo; i <= AgenHi; i++)
        kObjA[i] = kMainA[8];
    for (i = MuseLo; i <= MuseHi; i++)
        kObjA[i] = kRainbowA[2];
    for (i = ApolLo; i <= ApolHi; i++)
        kObjA[i] = kRainbowA[3];
    for (i = AmorLo; i <= AmorHi; i++)
        kObjA[i] = kRainbowA[1];
    for (i = AtenLo; i <= AtenHi; i++)
        kObjA[i] = kRainbowA[6];
#endif
#ifdef ASTEROIDS
    for (i = AsteLo+us.nExtraStar; i <= uranHi; i++)
        kObjA[i] = kMainA[4];
    for (i = RiyaLo; i <= RiyaHi; i++)
        kObjA[i] = kRainbowA[5];
#endif
    for (i = HypuranLo; i <= HypuranHi; i++)
        kObjA[i] = kRainbowA[7];
    for (i = 0; i <= oNormOpt; i++) {
        if (kObjA1[i] != 0)
            kObjA[i] = kObjA1[i];
    }
    for (i = starLo; i <= (starHi
#ifdef ASTEROIDS
                           + ((!us.fRiyalAst && us.nStar) ? cStar-cStarSmall : 0)
#endif
                          ); i++) {
        /*    kObjA[i] = rStarBright[i-oNorm] < 1.0 ? kRainbowA[2] : kMainA[4];  */
        if (rStarBright[i-starLo+1] < 1.0)
            kObjA[i] = kStar1;
        else if (rStarBright[i-starLo+1] < 2.0)
            kObjA[i] = kStar2;
        else
            kObjA[i] = kStar3;
    }
}


/* This is the dispatch procedure for the entire program. After all the   */
/* command switches have been processed, this routine is called to        */
/* actually call the various routines to generate and display the charts. */

void Action()
{
    char sz[cchSzDef];
    int i;
    NoPrintHeader = 0;
#ifdef WIN
    is.fMult = fFalse;    /* Cleared here because no InitVariables routine. */
#endif
    is.fSzPersist = is.fNoEphFile = fFalse;
    is.cchRow = 0;
    AnsiColor(kDefault);
    InitColors();
    if (fSouthNode) {
        szObjName[oSou] = "S.Node";
#ifdef INTERPRET
        szMindPart[oSou] =
            "karmic past, and area of experience but little growth";
#endif
        ruler1[oSou] = sLeo;
    }
    szAspectAbbrev[aCon] = us.fParallel ? "Par" : szAspectAbbrevCon;
    szAspectAbbrev[aOpp] = us.fParallel ? "CPr" : szAspectAbbrevOpp;

    /* First let's adjust the restriction status of the cusps, uranians, and */
    /* fixed stars based on whether -C, -u, and -U switches are in effect.   */
    if (!us.fCusp)
        for (i = cuspLo; i <= cuspHi; i++)
            ignore[i] = ignore2[i] = ignoreS[i] = fTrue;
    if (!us.fUranian)
        for (i = CentLo; i <= HypuranHi; i++)
            ignore[i] = ignore2[i] = ignoreS[i] = fTrue;
    if (!us.nStar)
        for (i = starLo; i <= starHi; i++)
            ignore[i] = ignore2[i] = ignoreS[i] = fTrue;
#ifdef ASTEROIDS
    if (us.nExtraAst<AsteHi-AsteLo-us.nExtraStar+1)
        for (i = AsteLo+us.nExtraAst+us.nExtraStar; i <= AsteHi; i++)
            ignore[i] = ignore2[i] = ignoreS[i] = fTrue;
    if (!us.fRiyalAst&& !us.nStar)
        for (i = RiyaLo; i <= RiyaHi; i++)
            ignore[i] = ignore2[i] = ignoreS[i] = fTrue;
#endif
    /* If the -os switch is in effect, open a file and set a global to */
    /* internally 'redirect' all screen output to.                     */
#ifdef GTOPO
    if (us.fAutoAlt)
        SetAlt(-OO,AA,&NN);
#endif
    if (is.szFileScreen) {
        is.S = fopen(is.szFileScreen, "w");
        if (is.S == NULL) {
            sprintf(sz, "File %s can not be created.", is.szFileScreen);
            PrintError(sz);
            is.S = stdout;
        }
    } else
        is.S = stdout;

    if (FPrintTables())    /* Print out any generic tables specified.        */
        goto LDone;          /* If nothing else to do, we can exit right away. */
    if (is.fMult) {
        PrintL2();
        is.fMult = fFalse;
    }

    /* If -+ or -- switches in effect, then add the specified delta value to */
    /* the date and use that as a new date before proceeding to make chart.  */

    if (us.dayDelta != 0) {
        is.JD = (real)MdyToJulian(MM, DD+us.dayDelta, YY);
        JulianToMdy(is.JD, &MM, &DD, &YY);
    }

    /* Here we either do a normal chart or some kind of relationship chart. */

#ifdef WIN
    if (us.fProgress && !us.nRel && !gs.nAnim && !us.fInDay)
        ciMain = ciTran;
#endif
    if (fReturnChart)
        fSynodicReturnChart ? us.fSector ? ChartPrimaryDirectionSet() : ChartSynodicReturnSet() : ChartReturnSet();
    if (!us.nRel) {
#ifndef WIN
        /* If chart info not in memory yet, then prompt the user for it. */
        if (!is.fHaveInfo && !FInputData(szTtyCore))
            return;
        if (fReturnChart){
            ciTwin.nam=ciCore.nam;
            ciMain=ciTwin;
            ciCore=ciTwin;
        }
        CastChart(fTrue);
#else
        if (wi.fCast) {
            wi.fCast = fFalse;
            CastChart(fTrue);
        }
#endif
        if (!us.fProgress || us.nRel||fReturnChart)
            ciMain = ciCore;
    } else
        CastRelation();
#ifndef WIN
    ciSave = ciMain;
#endif

#ifdef GRAPH
    if (us.fGraphics)         /* If any of the X window switches in effect, */
        FActionX();             /* then go make a graphics chart...           */
    else
#endif
        PrintChart(is.fProgress);    /* Otherwise print chart on text screen. */

    if (us.fWriteFile)        /* If -o switch in effect, then write */
        FOutputData();          /* the chart information to a file.   */

LDone:
    if (is.S != stdout) /* If we were internally directing chart display to a */
        fclose(is.S);     /* file as with the -os switch, close it here.        */

    if (grid) {
        DeallocateFar(grid);
        grid = NULL;
    }
}


#ifndef WIN
/* Reset a few variables to their default values they have upon startup of */
/* the program. We don't reset all variables, just the most volatile ones. */
/* This is called when in the -Q loop to reset things like which charts to */
/* display, but leave setups such as object restrictions and orbs alone.   */

void InitVariables()
{
    us.fInterpret = us.fProgress = is.fHaveInfo = is.fMult = fFalse;
    us.nRel = us.dayDelta = 0;
    is.szFileScreen = NULL;
    ClearB((lpbyte)&us.fListing,
           (int)((lpbyte)&us.fLoop - (lpbyte)&us.fListing));
}
#endif


/*
******************************************************************************
** Command Switch Procedures.
******************************************************************************
*/

/* Given a string representing a command line (e.g. a macro string), go    */
/* parse it into its various switches and parameters, then go process them */
/* and change program settings. Basically a wrapper for other functions.   */

bool FProcessCommandLine(szLine)
char *szLine;
{
    char szCommandLine[cchSzMax], *rgsz[MAXSWITCHES];
    int argc, cb, fT;

    if (szLine == NULL)
        return fTrue;
    if (Mon != -1)
        ciCore = ciMain;
    cb = CchSz(szLine)+1;
    CopyRgb((char *)szLine, (char *)szCommandLine, cb);
    argc = NParseCommandLine(szCommandLine, rgsz);
    fT = FProcessSwitches(argc, rgsz);
    ciMain = ciCore;
    return fT;
}


/* Given string representing a command line, convert it to an "argv" format */
/* of an array of strings, one for each switch or parameter, i.e. exactly   */
/* like the format of the command line as given when the program starts.    */

int NParseCommandLine(szLine, argv)
char *szLine;
char **argv;
{
    int argc = 1, fSpace = fTrue, fQuote = fFalse;
    char *pch = szLine;

    /* Split the entered line up into its individual switch strings. */
    while (*pch >= ' ' || *pch == chTab) {
        if (*pch == ' ' || *pch == chTab) {
            if (fSpace)
                /* Skip over the current run of spaces between strings. */
                ;
            else {
                /* First space after a string, end parameter here. */
                if (!fQuote) {
                    *pch = chNull;
                    fSpace = fTrue;
                }
            }
        } else {
            if (fSpace) {
                /* First character after run of spaces, begin parameter here. */
                if (argc >= MAXSWITCHES-1) {
                    PrintWarning("Too many items - rest of line ignored.");
                    break;
                }
                fQuote = (*pch == '"');
                argv[argc++] = pch + fQuote;
                fSpace = fFalse;
            } else
                /* Skip over the current string. */
                if (fQuote && *pch == '"') {
                    *pch = chNull;
                    fSpace = fTrue;
                }
        }
        pch++;
    }
    argv[0] = szAppNameCore;
    argv[argc] = NULL;         /* Set last string in switch array to Null. */
    return argc;
}


/* This routine is called by the main program to interactively prompt the  */
/* user for command switches and parameters, entered in the same format as */
/* they would be on a command line. This needs to be called with certain   */
/* systems which don't allow passing of a command line to the program,     */
/* or when -Q is in effect. The result of this routine is returned to the  */
/* main program which then processes it as done with a real command line.  */

int NPromptSwitches(line, argv)
char *line, *argv[MAXSWITCHES];
{
    FILE *data;
    char sz[cchSzDef];

    data = is.S;
    is.S = stdout;
    is.cchRow = 0;
    AnsiColor(kWhite);
    sprintf(sz, "** %s version %s ", szAppName, szVersionCore);
    PrintSz(sz);
    sprintf(sz, "(See '%cHc' switch for copyrights and credits.) **\n",
            chSwitch);
    PrintSz(sz);
    AnsiColor(kDefault);
    PrintSz("Enter all parameter options below. ");
    sprintf(sz, "(Enter '%cH' for help. Enter '.' to exit.)\n", chSwitch);
    PrintSz(sz);
    is.S = data;
    InputString("Input command line", line);
    PrintL();
    return NParseCommandLine(line, argv);
}


/* This subprocedure is like FProcessSwitches() below, except that we only */
/* process one switch, which we know to be one of the obscure -Y types.    */

int NProcessSwitchesRare(argc, argv, pos, fOr, fAnd, fNot)
int argc, pos;
bool fOr, fAnd, fNot;
char **argv;
{
    int darg = 0, i, j, k;
    real r;
    char ch1, ch2;
    OE oe;
    lpbyte lpb;
    int FPTR *lpn;
    lpreal lpr;
#ifdef INTERPRET
    char *sz;
#endif

    ch1 = argv[0][pos+1];
    ch2 = argv[0][pos+2];
    switch (argv[0][pos]) {
    case chNull:
        SwitchF(us.fSwitchRare);
        break;

    case 'H':
        SwitchF(PolarMCflip);
        break;

    case 'n':
        if (ch1=='s')
          SwitchF(fAlsoSouthNode);
        else
          SwitchF(us.fTrueNode);
        break;

    case 'd':
        SwitchF(us.fEuroDate);
        break;

    case 'm':
        SwitchF(MonthFormat);
        break;

    case 't':
        SwitchF(us.fEuroTime);
        break;

    case 'T':
        if (ch1=='R')
            SwitchF(us.fTruePos);
        else if (ch1=='C')
            SwitchF(us.fTopocentric);
        else if (ch1=='2')
            SwitchF(fUseJ2000);
        else if (ch1=='I')
            SwitchF(fUseICRS);
        else if (ch1=='J')
            fUseJPL=((fUseJPL==0 ) ? 406 : ((fUseJPL==406) ? 405 : 0));
        else if (ch1=='A')
            SwitchF(us.fNoAberration);
        else if (ch1=='G')
            SwitchF(us.fNoGravityDeflection);
        else if (ch1=='H')
            SwitchF(us.fSweHouse);
        else if (ch1=='F')
            SwitchF(us.fRefraction);
        else if (ch1=='L')
            SwitchF(us.fRelocation);
        else if (ch1=='B')
            SwitchF(us.fBarycentric);
        else if (ch1=='N') {
            SwitchF(us.fAutoAlt);
#ifdef GTOPO
            SetAlt(-us.lonDef,us.latDef,&us.altDef);
#endif
        }
        else if (ch1=='d') {
            SwitchF(fdeltaT);
            if (argc == 1) {
                break;
            }
            deltaT = atof(argv[1])/86400.0;
            darg++;
            break;
        }
        else if (ch1=='D')
            SwitchF(us.fAspect3D);
        else if (ch1=='T')
            SwitchF(us.fNoNutation);
        else if (ch1=='1')
            SwitchF(fRestrictClean);
        break;

    case 'C':
        SwitchF(us.fSmartCusp);
        break;

    case '8':
        SwitchF(us.fClip80);
        break;

    case 'Z':
        SwitchF(fNESW);
        break;

    case 'Q':
        if (argc <= 1) {
            ErrorArgc("YQ");
            return tcError;
        }
        i = atoi(argv[1]);
        if (i < 0) {
            ErrorValN("YQ", i);
            return tcError;
        }
        us.nScrollRow = i;
        darg++;
        break;

    case 's':
        if (argc <= 1) {
            ErrorArgc("Ys");
            return tcError;
        }
        i = atoi(argv[1]);
        us.ftransStep = i;
        if (argc > 1) {
            i = atoi(argv[2]);
            us.ftransIter = i;
            darg++;
        }
        darg++;
        break;

    case 'o':
        SwitchF(us.fWriteOld);
        break;

    case 'c':
        SwitchF(us.fHouseAngle);
        break;

    case 'z':
        if (argc <= 1) {
            ErrorArgc("Yz");
            return tcError;
        }
        us.lTimeAddition = atol(argv[1]);
        darg++;
        break;

    case 'l':
        if (argc <= 1) {
            ErrorArgc("Yl");
            return tcError;
        }
        i = atoi(argv[1]);
        if (!FSector(i)) {
            ErrorValN("Yl", i);
            return tcError;
        }
        SwitchF(pluszone[i]);
        darg++;
        break;

#ifdef ARABIC
    case 'P':
        if (argc <= 1) {
            ErrorArgc("YP");
            return tcError;
        }
        if (ch1 == 'a') {
            i = atoi(argv[1]);
            if (i < 0 || i > 18) {
                ErrorValN("YPa", i);
                return tcError;
            }
            PartAspects = i;
            darg++;
            break;
        }
        if (ch1 == 's') {
            i = atoi(argv[1]);
            if (i < 0 || i > 18) {
                ErrorValN("YPs", i);
                return tcError;
            }
            PartAspectsShow = i;
            darg++;
            break;
        }
        if (ch1 == 'p') {
            r = atof(argv[1]);
            if (r < 0.1 || r > 100.0) {
                ErrorValR("YPp", r);
                return tcError;
            }
            SolidAspect = r;
            darg++;
            break;
        }
        if (ch1 == 'o') {
            r = atof(argv[1]);
            if (r < 0.0 || r > 15.0) {
                ErrorValR("YPo", r);
                return tcError;
            }
            PartOrb = r;
            darg++;
            break;
        }
        i = atoi(argv[1]);
        if (!FBetween(i, -1, 1)) {
            ErrorValN("YP", i);
            return tcError;
        }
        us.nArabicNight = i;
        darg++;
        break;
#endif

    case 'b':
        if (argc <= 1) {
            ErrorArgc("Yb");
            return tcError;
        }
        i = atoi(argv[1]);
        if (!FValidBioday(i)) {
            ErrorValN("Yb", i);
            return tcError;
        }
        us.nBioday = i;
        darg++;
        break;

    case 'E':
        if (argc <= 17) {
            ErrorArgc("YE");
            return tcError;
        }
        i = NParseSz(argv[1], pmObject);
        if (!FHelio(i)||FBetween(i,oNod,AsteHi)) {
            ErrorValN("YE", i);
            return tcError;
        }
        oe.sma = atof(argv[2]);
        oe.ec0 = atof(argv[3]);
        oe.ec1 = atof(argv[4]);
        oe.ec2 = atof(argv[5]);
        oe.in0 = atof(argv[6]);
        oe.in1 = atof(argv[7]);
        oe.in2 = atof(argv[8]);
        oe.ap0 = atof(argv[9]);
        oe.ap1 = atof(argv[10]);
        oe.ap2 = atof(argv[11]);
        oe.an0 = atof(argv[12]);
        oe.an1 = atof(argv[13]);
        oe.an2 = atof(argv[14]);
        oe.ma0 = atof(argv[15]);
        oe.ma1 = atof(argv[16]);
        oe.ma2 = atof(argv[17]);
        rgoe[IoeFromObj(i>oVes ? i-HypuranLo+oNod : i )] = oe;
        darg += 17;
        break;

    case 'R':
        if (argc <= 2 + (ch1 == 'Z')*2) {
            ErrorArgc("YR");
            return tcError;
        }
        i = NParseSz(argv[1], pmObject);
        j = NParseSz(argv[2], pmObject);
        if (ch1 == '0') {
            us.fIgnoreSign = atoi(argv[1]) != 0;
            us.fIgnoreDir  = atoi(argv[2]) != 0;
            us.fIgnoreBounds  = atoi(argv[3]) != 0;
            darg += 3;
            break;
        } else if (ch1 == 'Z') {
            ignorez[0] = i != 0;
            ignorez[1] = j != 0;
            ignorez[2] = atoi(argv[3]) != 0;
            ignorez[3] = atoi(argv[4]) != 0;
            darg += 4;
            break;
        }
        if (!FItem(i)) {
            ErrorValN("YR", i);
            return tcError;
        }
        if (!FItem(j) || j < i) {
            ErrorValN("YR", j);
            return tcError;
        }
        if (argc <= 3+j-i) {
            ErrorArgc("YR");
            return tcError;
        }
        if (ch1 == 'T')
            lpb = ignore2;
        else if (ch1 == 'P')
            lpb = ignore3;
        else if (ch1 == 'S')
            lpb = ignoreS;
        else
            lpb = ignore;
        for (k = i; k <= j; k++)
            lpb[k] = atoi(argv[3+k-i]) != 0;
        darg += 3+j-i;
        break;

    case 'a':
        if (ch1=='w'){
           SwitchF(fWholeSignAspects);
           break;}
        else {
           ErrorArgv("Ya");
           return tcError;}

    case 'A':
        if (argc <= 2) {
            ErrorArgc("YA");
            return tcError;
        }
        k = ch1 == 'm' || ch1 == 'd' ? pmObject : pmAspect;
        i = NParseSz(argv[1], k);
        j = NParseSz(argv[2], k);
        k = (ch1 == 'm' || ch1 == 'd') ? cObj : cAspect;
        if (!FBetween(i, 1, k)) {
            ErrorValN("YA", i);
            return tcError;
        }
        if (!FBetween(j, 1, k) || j < i ) {
            ErrorValN("YA", j);
            return tcError;
        }
        if (argc <= 3+j-i) {
            ErrorArgc("YA");
            return tcError;
        }
        lpr = ch1 == 'o' ? rAspOrb : (ch1 == 'm' ? rObjOrb :
                                      (ch1 == 'd' ? rObjAdd : rAspAngle));
        for (k = i; k <= j; k++)
            lpr[k] = atof(argv[3+k-i]);
        darg += 3+j-i;
        break;

    case 'j':
        if (argc <= 2 + 2*(ch1 == '0')) {
            ErrorArgc("Yj");
            return tcError;
        }
        if (ch1 == '0') {
            rObjInf[oNorm+1] = atof(argv[1]);
            rObjInf[oNorm+2] = atof(argv[2]);
            rHouseInf[cSign+1]  = atof(argv[3]);
            rHouseInf[cSign+2]  = atof(argv[4]);
            darg += 4;
            break;
        }
        k = ch1 == 'C' ? pmSign : (ch1 == 'A' ? pmAspect : pmObject);
        i = NParseSz(argv[1], k);
        j = NParseSz(argv[2], k);
        k = ch1 == 'C' ? cSign : (ch1 == 'A' ? cAspect : cObj);
        if (!FBetween(i, 0, k)) {
            ErrorValN("Yj", i);
            return tcError;
        }
        if (!FBetween(j, 0, k) || j < i) {
            ErrorValN("Yj", j);
            return tcError;
        }
        if (argc <= 3+j-i) {
            ErrorArgc("Yj");
            return tcError;
        }
        lpr = ch1 == 'C' ? rHouseInf : (ch1 == 'A' ? rAspInf :
                                        (ch1 == 'T' ? rTransitInf : rObjInf));
        for (k = i; k <= j; k++)
            lpr[k] = atof(argv[3+k-i]);
        darg += 3+j-i;
        break;

    case 'J':
        if (argc <= 3 - (ch1 == '0')) {
            ErrorArgc("YJ");
            return tcError;
        }
        i = NParseSz(argv[1], pmObject);
        if (!FNorm(i)) {
            ErrorValN("YJ", i);
            return tcError;
        }
        j = NParseSz(argv[2], pmSign);
        if (!FBetween(j, 0, cSign)) {
            ErrorValN("YJ", j);
            return tcError;
        }
        if (ch1 != '0') {
            k = NParseSz(argv[3], pmSign);
            if (!FBetween(k, 0, cSign)) {
                ErrorValN("YJ", k);
                return tcError;
            }
            ruler1[i] = j;
            ruler2[i] = k;
            if (FBetween(i, 1, oMain) && j != 0)
                rules[j] = i;
        } else
            exalt[i] = j;
        darg += 3 - (ch1 == '0');
        break;

    case 'r':
        if (argc <= 2 ) {
            ErrorArgc("Yr");
            return tcError;
        }
        i = NParseSz(argv[1], pmSign);
        if (!FBetween(i, 0, cSign)) {
            ErrorValN("Yr", i);
            return tcError;
        }
        j = NParseSz(argv[2], pmObject);
        if (!FNorm(j)) {
            ErrorValN("Yr", j);
            return tcError;
        }
        rules[i] = j;
        ruler1[j] = i;
        darg += 2;
        break;

    case 'q':
        if (argc <= 2 ) {
            ErrorArgc("Yq");
            return tcError;
        }
        i = NParseSz(argv[1], pmObject);
        if (!FNorm(i)) {
            ErrorValN("Yq", i);
            return tcError;
        }
        j = NParseSz(argv[2], pmColor);
        if (!FBetween(j, 0, cColor)) {
            ErrorValN("Yq", j);
            return tcError;
        }
        kObjA1[i] = j;
        darg += 2;
        break;


#ifdef INTERPRET
    case 'I':
        if (argc <= 2) {
            ErrorArgc("YI");
            return tcError;
        }
        i = NParseSz(argv[1],
                     ch1 == 'A' ? pmAspect : (ch1 == chNull ? pmObject : pmSign));
        j = ch1 == 'A' ? cAspect : (ch1 == chNull ? oNormOpt : cSign);
        if (!FBetween(i, 1, j)) {
            ErrorValN("YI", i);
            return tcError;
        }
        if (ch1 == 'A' && ch2 == '0')
            ch1 = '0';
        sz = SzPersist(argv[2]);
        switch (ch1) {
        case 'A':
            szInteract[i]  = sz;
            break;
        case '0':
            szTherefore[i] = sz;
            break;
        case chNull:
            szMindPart[i]  = sz;
            break;
        case 'C':
            szLifeArea[i]  = sz;
            break;
        case 'v':
            szDesire[i]    = sz;
            break;
        default:
            szDesc[i]      = sz;
        }
        darg += 2;
        break;
#endif
#ifdef ASTEROIDS
    case 'N':
        if (argc <= 2) {
            ErrorArgc("YN");
            return tcError;
        }
        i = NParseSz(argv[1],
                     ch1 == 'L' ? pmObject : (ch1 == chNull ? pmObject : pmObject));
        if ((ch1 == chNull && !FBetween(i, 1, cObj))||(ch1 == 'L' && !FBetween(i,RiyaLo,RiyaHi))) {
            ErrorValN("YN", i);
            return tcError;
        }
        sz = SzPersist(argv[2]);
        switch (ch1) {
        case 'L':
            riyalObjName[i-RiyaLo]  = sz;
            break;
        case chNull:
            szObjName[i] = sz;
            break;
        }
        darg += 2;
        break;
#endif

    case 'k':
        if (argc <= 2 + 2*(ch1 == 'C')) {
            ErrorArgc("Yk");
            return tcError;
        }
        if (ch1 == 'C') {
            kElemA[eFir] = NParseSz(argv[1], pmColor) & 15;
            kElemA[eEar] = NParseSz(argv[2], pmColor) & 15;
            kElemA[eAir] = NParseSz(argv[3], pmColor) & 15;
            kElemA[eWat] = NParseSz(argv[4], pmColor) & 15;
            darg += 4;
            break;
        }
        k = ch1 == 'A' ? pmAspect : 0;
        i = NParseSz(argv[1], k);
        j = NParseSz(argv[2], k);
        k = ch1 == 'A' ? cAspect : (ch1 == '0' ? 7 : 8);
        if (!FBetween(i, ch1 != chNull, k)) {
            ErrorValN("Yk", i);
            return tcError;
        }
        if (!FBetween(j, ch1 != chNull, k) || j < i) {
            ErrorValN("Yk", j);
            return tcError;
        }
        if (argc <= 3+j-i) {
            ErrorArgc("Yk");
            return tcError;
        }
        lpn = ch1 == 'A' ? kAspA : (ch1 == '0' ? kRainbowA : kMainA);
        for (k = i; k <= j; k++)
            lpn[k] = NParseSz(argv[3+k-i], pmColor) & 15;
        darg += 3+j-i;
        break;

    case 'F':
        if (argc <= 8) {
            ErrorArgc("YF");
            return tcError;
        }
        i = NParseSz(argv[1], pmObject);
        if (!FItem(i)) {
            ErrorValN("YF", i);
            return tcError;
        }
        r = Mod((real)(atoi(argv[2]) +
                       (NParseSz(argv[3], pmSign)-1)*30) + atof(argv[4])/60.0);
        if (!FCusp(i))
            planet[i] = r;
        else {
            j = Mod12(i-(cuspLo-1)+6);
            if (FBetween(i, cuspLo-1+4, cuspLo-1+9)) {
                chouse[i-(cuspLo-1)] = r;
                chouse[j] = Mod(r + rDegHalf);
            } else {
                planet[i] = r;
                planet[cuspLo-1+j] = Mod(r + rDegHalf);
            }
        }
        j = atoi(argv[5]);
        r = (j < 0 ? -1.0 : 1.0)*((real)abs(j) + atof(argv[6])/60.0);
        planetalt[i] = Mod((r + rDegQuad) * 2.0) / 2.0 - rDegQuad;
        ret[i] = RFromD(atof(argv[7]));
        if (i <= oNormOpt)
            SphToRec(atof(argv[8]), planet[i], planetalt[i],
                     &spacex[i], &spacey[i], &spacez[i]);
        MM = -1;
        darg += 8;
        break;

    case 'U':
        if (ch1 != 'a' && ch1 != 'o' && ch1 != 'b' && ch1 != 'O' && ch1 != 'P' ) {
            SwitchF(StarRest);
            break;
        }
        if (argc <= 1) {
            ErrorArgc("YU");
            return tcError;
        }
        if (ch1 == 'a') {
            i = atoi(argv[1]);
            if (i < 0 || i > 18) {
                ErrorValN("YUa", i);
                return tcError;
            }
            StarAspects = i;
            darg++;
            break;
        }
        if (ch1 == 'o') {
            r = atof(argv[1]);
            if (r < 0.0 || r > 15.0) {
                ErrorValR("YUo", r);
                return tcError;
            }
            StarOrb = r;
            darg++;
            break;
        }
        if (ch1 == 'P') {
            i = atoi(argv[1]);
            if (i < 0 || i > 3) {
                ErrorValN("YUP", i);
                return tcError;
            }
            PrimeRest = i;
            darg++;
            break;
        }
        if (ch1 == 'b') {
            if (ch2 == '1') {
                i = atoi(argv[1]);
                if (i < -2 || i > 16) {
                    ErrorValN("YUb1", i);
                    return tcError;
                }
                kStar1 = i;
                darg++;
                break;
            }
            if (ch2 == '2') {
                i = atoi(argv[1]);
                if (i < -2 || i > 16) {
                    ErrorValN("YUb2", i);
                    return tcError;
                }
                kStar2 = i;
                darg++;
                break;
            }
            if (ch2 == '3') {
                i = atoi(argv[1]);
                if (i < -2 || i > 16) {
                    ErrorValN("YUb3", i);
                    return tcError;
                }
                kStar3 = i;
                darg++;
                break;
            }
        }
        if (ch1 == 'O') {
            if (ch2 == '1') {
                r = atof(argv[1]);
                if (r < 0.0 || r > 16.0) {
                    ErrorValR("YUO1", r);
                    return tcError;
                }
                PrimeOrb1 = r;
                darg++;
                break;
            }
            if (ch2 == '2') {
                r = atof(argv[1]);
                if (r < 0.0 || r > 16.0) {
                    ErrorValR("YUO2", r);
                    return tcError;
                }
                PrimeOrb2 = r;
                darg++;
                break;
            }
        }
    case 'O':
        if (argc <= 1) {
            ErrorArgc("YO");
            return tcError;
        }
        if (ch1 == 'c') {
            r = atof(argv[1]);
            if (r <= 0) {
                ErrorValR("YOc", r);
                return tcError;
            }
            CoeffComp = r;
            darg++;
            break;
        }
        if (ch1 == 's') {
            r = atof(argv[1]);
            if (r <= 0) {
                ErrorValR("YOs", r);
                return tcError;
            }
            us.nStationOrb = r;
            darg++;
            break;
        }

        if (ch1 == 'p') {
            r = atof(argv[1]);
            if (r <= 0.0) {
                ErrorValR("YOp", r);
                return tcError;
            }
            CoeffPar = r;
            darg++;
            break;
        }
        if (ch1 == 'P') {
            r = atof(argv[1]);
            if (r < 0.0 || r > 100.0) {
                ErrorValR("YOP", r);
                return tcError;
            }
            PowerPar = r;
            darg++;
            break;
        }
        break;

    case 'S':
        if (ch1 == 'a') {
            SwitchF(us.fAppSep);
            break;
        }
        if (ch1 == 'j') {
            SwitchF(us.fInfluenceSign);
            break;
        }
        if (ch1 == 's') {
            SwitchF(us.fAspSummary);
            break;
        }
        if (ch1 == 'p') {
            SwitchF(us.fParallel);
            break;
        }
        break;

#ifdef GRAPH
    case 'X':
        return NProcessSwitchesRareX(argc, argv, pos+1);
#endif
#ifdef KDE
    case 'Y':
        if (argc <= 2) {
            ErrorArgc("YY");
            return tcError;
        }
        i = atoi(argv[1]);
        if (!FBetween(i, 0, 15)) {
            ErrorValN("YY", i);
            return tcError;
        }
        ColorTbl[i] = SzPersist(argv[2]);
        darg += 2;
        break;
    case 'f':
        if (argc <= 1) {
            ErrorArgc("Yf");
            return tcError;
        }
        {
            char buf[80] = "";
            if (strncmp(argv[1], ZoneInfoPath, strlen(ZoneInfoPath)))
                strcpy(buf, ZoneInfoPath);
            strncat(buf, argv[1], 80);
            DeallocateNear(us.tzfile);
            us.tzfile = SzPersist(buf);
        }
        darg++;
        break;
    case 'B':
        SwitchF(us.fTz4chart);
        break;
    case 'a':
        SwitchF(us.fDst4Anim);
        break;
    case 'g':
        if (argc <= 4) {
            ErrorArgc("Yg");
            return tcError;
        }
        GSTEST(1, portx);
        GSTEST(2, porty);
        GSTEST(3, landx);
        GSTEST(4, landy);
        darg += 4;
        break;
#endif

    default:
        ErrorSwitch(argv[0]);
        return tcError;
    }
    return darg;    /* Return the value to be added to argc. */
}


/* Process a command switch line passed to the program. Read each entry in */
/* the argument list and set all the program modes and charts to display.  */

bool FProcessSwitches(argc, argv)
int argc;
char **argv;
{
    int ich, i=0, j;
#ifdef ASTEROIDS
int k;
#endif
    bool fNot, fOr, fAnd;
    real rT;
    char ch1, ch2, *pch, *sz;
    CI ci;
    argc--;
    argv++;
    while (argc) {
        ch1 = argv[0][0];
        fNot = fOr = fAnd = fFalse;
        switch (ch1) {
        case '=':
            fOr  = fTrue;
            break;
        case '_':
            fAnd = fTrue;
            break;
        case ':':
            break;
        default:
            fNot = fTrue;
            break;
        }
        ich = 1 + FChSwitch(argv[0][0]);    /* Leading dash? */
        ch1 = argv[0][ich];
        ch2 = ch1 == chNull ? chNull : argv[0][ich+1];
        switch (argv[0][ich-1]) {

        case 'H':
            if (ch1 == 'c')
                SwitchF(us.fCredit);
            else if (ch1 == 'Y')
                SwitchF(us.fSwitchRare);
#ifdef ISG
            else if (ch1 == 'X')
                SwitchF(us.fKeyGraph);
#endif
            else if (ch1 == 'C')
                SwitchF(us.fSign);
            else if (ch1 == 'O')
                SwitchF(us.fObject);
            else if (ch1 == 'A')
                SwitchF(us.fAspect);
            else if (ch1 == 'F')
                SwitchF(us.fConstel);
            else if (ch1 == 'S')
                SwitchF(us.fOrbitData);
            else if (ch1 == 'I')
                SwitchF(us.fMeaning);
            else if (ch1 == 'e') {
                SwitchF(us.fCredit);
                SwitchF(us.fSwitch);
                SwitchF(us.fSwitchRare);
                SwitchF(us.fKeyGraph);
                SwitchF(us.fSign);
                SwitchF(us.fObject);
                SwitchF(us.fAspect);
                SwitchF(us.fConstel);
                SwitchF(us.fOrbitData);
                SwitchF(us.fMeaning);
            } else
                SwitchF(us.fSwitch);
            break;

        case 'Q':
            if (ch1 == '0')
                SwitchF(us.fLoopInit);
            SwitchF(us.fLoop);
            break;

        case 'M':
#ifdef KDE
            if (ch1 == '1')
            {
                if (argc <= 2)
                {
                    ErrorArgc("M");
                    return fFalse;
                }
                j = atoi(argv[1]);
                if (!FValidMacro(j)) {
                    ErrorValN("M", j);
                    return fFalse;
                }
                j--;
                szMacro[j] = SzPersist(argv[2]);
                szMacroName[j] = SzPersist(argv[3]);
                if (gi.fiskde)
                    SetMenuName(j+1, argv[3]);
                argc -= 3;
                argv += 3;
            }
            else
            {
#endif
                i = (ch1 == '0');
                if (argc <= 1+i) {
                    ErrorArgc("M");
                    return fFalse;
                }
                j = atoi(argv[1]);
                if (!FValidMacro(j)) {
                    ErrorValN("M", j);
                    return fFalse;
                }
                j--;
                if (i)
                    szMacro[j] = SzPersist(argv[2]);
                else
                    FProcessCommandLine(szMacro[j]);
                argc -= 1+i;
                argv += 1+i;
#ifdef KDE
            }
#endif
            break;
#ifdef PLACALC
        case 'N':
            i=0;
            for (j=1;j<=5;j++){
                if ( ch1 == chNull || ch1 == ' ' || ch1 == ';' ){
                    j=5;
                    continue;
                }
                i |= (ch1=='n')+2*(ch1=='d')+4*(ch1=='h')+8*(ch1=='f')+16*(ch1=='m')+
                     32*(ch1=='b')+64*(ch1=='a')+128*(ch1=='A')+256*(ch1=='N')+
                     512*(ch1=='y')+1024*(ch1=='S')+2048*(ch1=='s')+4096*(ch1=='D')+
                     8192*(ch1=='r')+16384*(ch1=='U')+32768*(ch1=='T')+65536*(ch1=='B')+
                     131072*(ch1=='R')+262144*(ch1=='L')+524288*(ch1=='t')+1048576*(ch1=='k');
                ch1=argv[0][ich+j];
            }
            if ( i&2 )
                SwitchF(us.fNodeDscPeri);
            if ( i&4 )
                SwitchF(us.fNodeApPeri);
            if ( i&8)
                SwitchF(us.fNodeFocal);
            if ( i&16)
                SwitchF(us.fNodeMean);
            if ( i&32)
                SwitchF(us.fNodeBary);
            if ( i&2048)
                SwitchF(us.fSortRank);
            if ( i&65536)  {
                SwitchF(us.fDebug);
                while (argc > 1 && !FChSwitch(argv[1][0]) && (j=atoi(argv[1]))>0){
                    us.fDebug = j;
                    argc--;
                    argv++;
                }
            }
            if ( i&1) {
                if (argc <= 1) {
                    ErrorArgc("Nn");
                    return fFalse;
                }
                j = NParseSz(argv[1],pmObject);
                if ((j!=oVtx || !us.objAst)&&(!FObject(j)||FBetween(j,oNod,TerranHi))) {
                    ErrorValN("Nn", j);
                    return fFalse;
                }
                us.objNode=j;
                SetNodeNames(j);
                argc--;
                argv++;
            }
            if ( i&32768) {
                if (argc <= 1) {
                    ErrorArgc("NT");
                    return fFalse;
                }
                rT = atof(argv[1]);
                if (j<0) {
                    ErrorValN("NT", rT);
                    return fFalse;
                }
                us.nTreshold=rT;
                argc--;
                argv++;
            }
            if ( i&524288) {
                if (argc <= 1) {
                    ErrorArgc("Nt");
                    return fFalse;
                }
                rT = atof(argv[1]);
                if (j<0.1) {
                    ErrorValN("Nt", rT);
                    return fFalse;
                }
                us.nTargetDistance=rT;
                argc--;
                argv++;
            }
            if ( i&4096) {
                if (argc <= 1) {
                    ErrorArgc("ND");
                    return fFalse;
                }
                rT = atof(argv[1]);
                if (!FValidOffset(rT)) {
                    ErrorValN("ND", rT);
                    return fFalse;
                }
                us.nSortDial=rT;
                argc--;
                argv++;
            }
            if ( i&8192) {
                if (argc <= 2) {
                    ErrorArgc("Nr");
                    return fFalse;
                }
                rT = atof(argv[1]);
                if (!FValidOffset(rT)) {
                    ErrorValN("Nr", rT);
                    return fFalse;
                }
                us.nDegFrom=rT;
                rT = atof(argv[2]);
                if (!FValidOffset(rT)) {
                    ErrorValN("Nr", rT);
                    return fFalse;
                }
                us.nDegTo=rT;
                argc-=2;
                argv+=2;
            }
            if ( i&131072) {
                if (argc <= 2) {
                    ErrorArgc("NR");
                    return fFalse;
                }
                j = NParseSz(argv[1],pmObject);
                if (j==us.objCenter) {
                    ErrorValN("NR", j);
                    return fFalse;
                }
                us.nDegObj=j;
                rT = atof(argv[2]);
                if (!FValidOffset(rT)) {
                    ErrorValN("NR", rT);
                    return fFalse;
                }
                us.nDegOrb=rT;
                argc-=2;
                argv+=2;
            }
            if ( i&512) {
                SwitchF(us.fDiscoveryYear);
            }
            if ( i & 64 ) {
                if (argc <= 1) {
                    ErrorArgc("Na");
                    return fFalse;
                }
                j = atoi(argv[1]);
                us.objAst=j;
                SetAstName(j);
                argc--;
                argv++;
            }
#ifdef ASTEROIDS
            if (i&262144){
                SwitchF(us.fRiyalAst);
                if (us.fRiyalAst)
                for (j=0;j<cRiya;j++){
            if (!us.fDiscoveryYear &&(riyalObjName[j][0]=='2'||riyalObjName[j][0]=='1'))
                szObjName[j+RiyaLo]=strdup(riyalObjName[j]+4);
                else szObjName[j+RiyaLo]=strdup(riyalObjName[j]);
                }
    init_ephe_path();
            }
            if ( i&1024) {
                SwitchF(us.fIgnoreGaps);
            }
            if ( i&1048576) {
                SwitchF(fSkipHard);
            }
            if ( i&128) {
                if (argc <= 2) {
                    ErrorArgc("NA");
                    return fFalse;
                }
                j=atoi(argv[1]);
                k=atoi(argv[2]);
                if (j>MaxNumberedAsteroid||j<1){
                    ErrorValN("NA", j);
                    return fFalse;
                }
                if (k>AsteHi-AsteLo-us.nExtraAst-us.nExtraStar||k<1){
                    ErrorValN("NA", k);
                    return fFalse;
                }
                SetObjNumbers(j,k);
                /*SetObjNames(0);*/
                argc-=2;
                argv+=2;
            }
            if ( i&256) {
                SetObjNumbers(1,AsteHi-AsteLo-us.nExtraAst-us.nExtraStar);
                /*SetObjNames(0);*/
            }
            if ( i&16384) {
                while (argc > 1 && (j=atoi(argv[1]))){
                    if (!FBetween(j,-MaxNumberedComet,MaxNumberedAsteroid)) {
                        ErrorValN("NU", j);
                        return fFalse;
                    } else {
                        if (j > 0) {
                            if (us.nExtraAst < AsteHi-AsteLo+1- us.nExtraStar)
                                us.nExtraAst++;
                            astnr[us.nExtraAst-1]=j;
                            SetObjNames(j);
                            argc--;
                            argv++;
                        } else { /* comet */
                            if (us.nExtraAst < AsteHi-AsteLo+1- us.nExtraStar)
                                us.nExtraAst++;
                            astnr[us.nExtraAst-1]=j;
                            argc--;
                            argv++;
                        }
                    }
                }
            }
#endif
            if ( !i || i& 30 )
                SwitchF(us.fNodelist);
            break;
#endif
        case 'Y':
            i = NProcessSwitchesRare(argc, argv, ich, fOr, fAnd, fNot);
            if (i < 0)
                return fFalse;
            argc -= i;
            argv += i;
            break;

            /* Switches which determine the type of chart to display: */

        case 'v':
            if (ch1 == '0')
                SwitchF(us.fVelocity);
            SwitchF(us.fListing);
            break;

        case 'w':
            if (ch1 == '0')
                SwitchF(us.fWheelReverse);
            if (argc > 1 && (i = atoi(argv[1]))) {
                argc--;
                argv++;
                if (!FValidWheel(i)) {
                    ErrorValN("w", i);
                    return fFalse;
                }
                us.nWheelRows = i;
            }
            SwitchF(us.fWheel);
            break;

        case 'g':
            if (ch1 == '0' || ch2 == '0')
                SwitchF(us.fGridConfig);
            if (ch1 == 'a')
                SwitchF(us.fAppSep);
            else if (ch1 == 'p')
                SwitchF(us.fParallel);
#ifdef X11
            else if (ch1 == 'e') {
                if (argc <= 1) {
                    ErrorArgc("geometry");
                    return fFalse;
                }
                gs.xWin = atoi(argv[1]);
                if (argc > 2 && (gs.yWin = atoi(argv[2]))) {
                    argc--;
                    argv++;
                } else
                    gs.yWin = gs.xWin;
                if (!FValidGraphx(gs.xWin)) {
                    ErrorValN("geometry", gs.xWin);
                    return fFalse;
                }
                if (!FValidGraphy(gs.yWin)) {
                    ErrorValN("geometry", gs.yWin);
                    return fFalse;
                }
                argc--;
                argv++;
                break;
            }
#endif
            SwitchF(us.fGrid);
            break;

        case 'a':
            SwitchF(us.fAspList);
            if (ch1 == '0') {
                SwitchF(us.fAspSummary);
                ch1 = ch2;
            }
            if (ch1 == 'a')
                SwitchF(us.fAppSep);
            else if (ch1 == 'p')
                SwitchF(us.fParallel);
            break;

        case 'm':
            if (ch1 == '0' || ch2 == '0')
                SwitchF(us.fMidSummary);
            if (ch1 == 'a')
                SwitchF(us.fMidAspect);
            SwitchF(us.fMidpoint);
            break;

        case 'Z':
            if (ch1 == 'E'||ch1 == 'c'||ch1 == 'e'){
                if (ch2== 'b'){
                    SwitchF(fEclipseBackward);
                    break;
                }
                if (ch2== 'n'){
                    if (argc <= 1) {
                        ErrorArgc("ZEn");
                        return fFalse;
                    }
                    us.nEclipseCount=atoi(argv[1]);
                    if (us.nEclipseCount<1)
                        us.nEclipseCount=1;
                    argc--;
                    argv++;
                    break;
                }
                if (ch1== 'E' || ch1=='e')
                    SwitchF(us.fEclipse);
                if (ch1== 'e' )
                    SwitchF(fEclipseDetails);
                if (ch2 == 'l')
                    us.nEclObj=oMoo;
                else if (ch2 == 's')
                    us.nEclObj=oSun;
                else if (ch2 == 'O')
                    fOccultAll=fTrue;
                else if (ch2 == 'o'){
                    if (argc <= 1) {
                        ErrorArgc("ZEo");
                        return fFalse;
                    }
                    us.nEclObj=NParseSz(argv[1], pmObject);
                    if (!FObject(i)) {
                        ErrorValN("ZEo", i);
                        return tcError;
                    }
                    argc--;
                    argv++;
                }
                if (ch1 == 'c')
                    ChartEclipseSet();
                break;
            }
#ifdef ASTEROIDS
            if (ch1 == 'S'){
                SwitchF(us.fSolex);
                break;
            }
            if (ch1 == 'C'){
                if (argc <= 2) {
                    ErrorArgc("ZC");
                    return fFalse;
                }
                if (argc >= 4 && (j=atoi(argv[3]))) {
                    if (!FBetween(j,1,1000)) {
                        ErrorValN("ZC", j);
                        return fFalse;
                    } else if (argc >= 5 && (k=atoi(argv[4]))) {
                        if (!FBetween((double) k,-400000000.0,4000000000.0)) {
                            ErrorValN("ZC", k);
                            return fFalse;
                        } else {
                            ConvertSolex(argv[1],argv[2],j,NSgn(k)*k,(k>0) ? fTrue : fFalse );
                            argc-=4;
                            argv+=4;
                            break;
                        }
                    }
                    }
                if (argc >=4 ) {
                    ConvertSolex(argv[1],argv[2],j,10000000,fTrue);
                    argc-=3;
                    argv+=3;
                    break;
                } else {
                    ConvertSolex(argv[1],argv[2],200,10000000,fTrue);
                    argc-=2;
                    argv+=2;
                    break;
                }
            }
#endif
#ifdef GTOPO
            if (ch1 == 'G'){
                if (argc <= 2) {
                    ErrorArgc("ZG");
                    return fFalse;
                }
                ConvertPgm(argv[1],argv[2]);
                argc-=2;
                argv+=2;
                break;
            }
#endif
            if (ch1 == '0' || ch2 == '0')
                SwitchF(us.fPrimeVert);
            else if (ch1 == 'd' && ch2 != 'e')
                SwitchF(us.fHorizonSearch);
            if (ch1== 'p'||ch2=='p')
                SwitchF(us.fParans);
            if (ch1 == 'e' || ch2 == 'e')
                SwitchF(fEquator);
            SwitchF(us.fHorizon);
            break;

        case 'S':
            SwitchF(us.fOrbit);
            break;

        case 'l':
            if (ch1 == '0')
                SwitchF(us.fSectorApprox);
            SwitchF(us.fSector);
            break;

        case 'j':
            if (ch1 == '0')
                SwitchF(us.fInfluenceSign);
            SwitchF(us.fInfluence);
            break;

        case 'L':
            if (ch1 == '0')
                SwitchF(us.fLatitudeCross);
            if (argc > 1 && (i = atoi(argv[1]))) {
                argc--;
                argv++;
                if (!FValidAstrograph(i)) {
                    ErrorValN("L", i);
                    return fFalse;
                }
                us.nAstroGraphStep = i;
            }
            SwitchF(us.fAstroGraph);
            break;

        case 'K':
            if (ch1 == 'y')
                SwitchF(us.fCalendarYear);
            SwitchF(us.fCalendar);
            break;

        case 'd':
            if (ch1 == 'p') {
                us.fSolarArc = 0;
                if (ch2 == '0')
                    us.fSolarArc = 1;
                if (ch2 == '1')
                    us.fSolarArc = 2;
                if (ch2 == '3')
                    us.fSolarArc = 3;
                if (ch2 == '2')
                    us.fDirect = fTrue;
                if (us.fSolarArc||us.fDirect){
                    ich++;
                    ch2 = argv[0][ich+1];
                }
                i = (ch2 == 'y') + 2*(ch2 == 'Y');
#ifdef TIME
                j = i < 2 && (argv[0][ich+i+1] == 'n');
#else
                j = fFalse;
#endif
                if (!j && argc <= 2-(i&1)) {
                    ErrorArgc("dp");
                    return fFalse;
                }
                is.fProgress = us.fInDayMonth = fTrue;
                DstT = us.dstDef;
                ZonT = us.zonDef;
                LonT = us.lonDef;
                LatT = us.latDef;
                AltT = us.altDef;
#ifdef TIME
                if (j)
                    GetTimeNow(&MonT, &DayT, &YeaT, &TimT, ZonT-DstT);
#endif
                if (i) {
                    MonT = 0;
                    if (!j)
                        YeaT = NParseSz(argv[1], pmYea);
                    us.nEphemYears = (i == 2) ? atoi(argv[2]) : 1;
                } else {
                    if (!j) {
                        MonT = NParseSz(argv[1], pmMon);
                        YeaT = NParseSz(argv[2], pmYea);
                        if (!FValidMon(MonT)) {
                            ErrorValN("dp", MonT);
                            return fFalse;
                        }
                    }
                }
                if (!FValidYea(YeaT)) {
                    ErrorValN("dp", YeaT);
                    return fFalse;
                }
                if (!j) {
                    i = 2-(i&1);
                    argc -= i;
                    argv += i;
                }
            } else if (ch1 == 'm' || ch1 == 'y' || ch1 == 'Y') {
                if (ch1 == 'y')
                    us.nEphemYears = 1;
                else if (ch1 == 'Y') {
                    if (argc <= 1) {
                        ErrorArgc("dY");
                        return fFalse;
                    }
                    i = atoi(argv[1]);
                    if (i < 1) {
                        ErrorValN("dY", i);
                        return fFalse;
                    }
                    us.nEphemYears = i;
                    argc--;
                    argv++;
                }
                SwitchF(us.fInDayMonth);
                MonT = (ch1 == 'm');
            }
#ifdef X11
            else if (ch1 == 'i') {    /* -display switch for X */
                if (argc <= 1) {
                    ErrorArgc("display");
                    return fFalse;
                }
                gs.szDisplay = SzPersist(argv[1]);
                argc--;
                argv++;
                break;
            }
#endif
            else if (argc > 1 && (i = atoi(argv[1]))) {
                if (!FValidDivision(i)) {
                    ErrorValN("d", i);
                    return fFalse;
                }
                us.nDivision = i;
                argc--;
                argv++;
                break;
            }
            SwitchF(us.fInDay);
            break;

        case 'D':
            SwitchF(us.fInDayInf);
            break;

        case 'E':
            if (ch1 == 'Y' && argc <= 1) {
                ErrorArgc("E");
                return fFalse;
            }
            SwitchF(us.fEphemeris);
            if (ch1 == 'y')
                us.nEphemYears = us.fEphemeris ? 1 : 0;
            else if (ch1 == 'Y') {
                i = atoi(argv[1]);
                if (i < 1) {
                    ErrorValN("EY", i);
                    return fFalse;
                }
                argc--;
                argv++;
                us.nEphemYears = i;
            }
            break;

        case 'e':
            SwitchF(us.fListing);
            SwitchF(us.fWheel);
            SwitchF(us.fGrid);
            SwitchF(us.fAspList);
            SwitchF(us.fMidpoint);
            SwitchF(us.fHorizon);
            SwitchF(us.fOrbit);
            SwitchF(us.fSector);
            SwitchF(us.fCalendar);
            SwitchF(us.fInfluence);
            SwitchF(us.fAstroGraph);
            SwitchF(us.fInDay);
            SwitchF(us.fInDayInf);
            SwitchF(us.fEphemeris);
            SwitchF(us.fGridConfig);
            SwitchF(us.fInfluenceSign);
            SwitchF(us.fLatitudeCross);
            break;

        case 't':
            SwitchF(us.fTransit);
            ZonT = us.zonDef;
            DstT = us.dstDef;
            LonT = us.lonDef;
            LatT = us.latDef;
            AltT = us.altDef;
            if (ch1 == 'p') {
                us.fSolarArc = 0;
                if (ch2 == '0')
                    us.fSolarArc = 1;
                if (ch2 == '1')
                    us.fSolarArc = 2;
                if (ch2 == '3')
                    us.fSolarArc = 3;
                if (ch2 == '2')
                    us.fDirect = fTrue;
                if (us.fSolarArc||us.fDirect)
                {
                    ich++;
                    ch2=argv[0][ich+2];
                }
                is.fProgress = fTrue;
                ch1 = argv[0][++ich];
            }
            if (ch1 == 'r') {
                is.fReturn = fTrue;
                ch1 = argv[0][++ich];
            }
            if ((i = (ch1 == 'y') + 2*(ch1 == 'Y')+4*(ch1 == 'd')))
                ch1 = argv[0][++ich];
#ifdef TIME
            if (ch1 == 'n') {
                GetTimeNow(&MonT, &DayT, &YeaT, &TimT, ZonT-DstT);
                if (i == 1)
                    MonT = 0;
                else if (i == 2) {
                    MonT = -1;
                    DayT = NParseSz(argv[1], pmDay);
                }
                else if (i == 4)
                    break;
                else  DayT=1;
                break;
            }
#endif
            if (argc <= 2 - (i & 1)+ (i&4)/4) {
                ErrorArgc("t");
                return fFalse;
            }
            DayT=1;
            if (i) {
                if (i == 1)
                    MonT = 0;
                else if (i == 2){
                    MonT = -1;
                    DayT = NParseSz(argv[2], pmDay);
                } else {
                    MonT = NParseSz(argv[1], pmMon);
                    DayT = NParseSz(argv[2], pmDay);
                    YeaT=NParseSz(argv[3],pmYea);
                    if (!FValidDay(DayT,MonT,YeaT)) {
                        ErrorValN("t", DayT);
                        return fFalse;
                    }
                }
            }
            else {
                MonT = NParseSz(argv[1], pmMon);
                if (!FValidMon(MonT)) {
                    ErrorValN("t", MonT);
                    return fFalse;
                }
            }
            YeaT = NParseSz(argv[2 - (i&1)-(i&2)/2+(i&4)/4 ], pmYea);
            argc -= 2 - (i & 1)+(i&4)/4;
            argv += 2 - (i & 1)+(i&4)/4;
            break;

        case 'T':
            SwitchF(us.fTransitInf);
            ZonT = us.zonDef;
            DstT = us.dstDef;
            LonT = us.lonDef;
            LatT = us.latDef;
            AltT = us.altDef;
            if (ch1 == 'p') {
                us.fSolarArc = 0;
                if (ch2 == '0')
                    us.fSolarArc = 1;
                if (ch2 == '1')
                    us.fSolarArc = 2;
                if (ch2 == '3')
                    us.fSolarArc = 3;
                if (ch2 == '2')
                    us.fDirect = fTrue;
                if (us.fSolarArc||us.fDirect){
                    ich++;
                    ch2 = argv[0][ich+1];
                }
                is.fProgress = fTrue;
                ch1 = argv[0][++ich];
            }
            if (ch1 == 'r') {
                is.fReturn = fTrue;
                ch1 = argv[0][++ich];
            }
            if (ch1 == 'c') {
                SwitchF(us.fContinued);
                ch1 = argv[0][++ich];
            }
            if (ch1 == 's') {
                SwitchF(us.fInfSummary);
                ch1 = argv[0][++ich];
            }
#ifdef TIME
            if (ch1 == 'n') {
                GetTimeNow(&MonT, &DayT, &YeaT, &TimT, ZonT-DstT);
                break;
            }
#endif
            if (ch1 == 'i') {
                if (us.fNoRead) {
                    ErrorArgv("i");
                    return tcError;
                }
                if (argc <= 1) {
                    ErrorArgc("i");
                    return fFalse;
                }
                ci=ciCore;
                if (!FInputData(argv[1]))
                    return fFalse;
                ciTran=ciCore;
                ciCore=ci;
                argc--;
                argv++;
                break;
            }
            if (argc <= 3) {
                ErrorArgc("T");
                return fFalse;
            }
            MonT = NParseSz(argv[1], pmMon);
            DayT = NParseSz(argv[2], pmDay);
            YeaT = NParseSz(argv[3], pmYea);
            if (!FValidMon(MonT)) {
                ErrorValN("T", MonT);
                return fFalse;
            } else if (!FValidDay(DayT, MonT, YeaT)) {
                ErrorValN("T", DayT);
                return fFalse;
            } else if (!FValidYea(YeaT)) {
                ErrorValN("T", YeaT);
                return fFalse;
            }
            if (argc >= 4 && ch1 == 't') {
                TimT =  RParseSz(argv[4], pmTim);
                if (!FValidTim(TimT)) {
                    ErrorValN("T", TimT);
                    return fFalse;
                }
                argc--;
                argv++;
            }
            else TimT = Tim;
            argc -= 3;
            argv += 3;
            break;

#ifdef ARABIC
        case 'P':
            if (argc > 1 && (i = atoi(argv[1]))) {
                argc--;
                argv++;
                if (!FValidPart(i)) {
                    ErrorValN("P", i);
                    return fFalse;
                }
                us.nArabicParts = i;
            }
            if (ch1 == 'z' || ch1 == 'n' || ch1 == 'f') {
                us.nArabic = ch1;
                ch1 = ch2;
            } else
                SwitchF(us.nArabic);
            if (ch1 == '0')
                SwitchF(us.fArabicFlip);
            break;
#endif

#ifdef INTERPRET
        case 'I':
#ifdef INTERPRETALT
            InterpretAlt = 0;
#endif
            if (argc > 1 && (i = atoi(argv[1]))) {
                argc--;
                argv++;
                if (!FValidScreen(i)) {
                    ErrorValN("I", i);
                    return fFalse;
                }
                us.nScreenWidth = i;
            }
#ifdef INTERPRETALT
            if (ch1=='a') {
                InterpretAlt = 1;
                us.fInterpret = 1;
                break;
            }
            if (ch1=='b') {
                InterpretAlt = 2;
                us.fInterpret = 1;
                break;
            }
            if (ch1=='p') {
                InterpretAlt = 3;
                us.fInterpret = 1;
                break;
            }
            if (ch1=='h') {
                InterpretAlt = 4;
                us.fInterpret = 1;
                break;
            }
            if (ch1=='o') {
                InterpretAlt = 5;
                us.fInterpret = 1;
                break;
            }
            if (ch1=='d') {
                InterpretAlt = 6;
                us.fInterpret = 1;
                break;
            }
            if (ch1=='S') {
                us.fInterpret = 1;
                us.fInterpretSabian = 1;
                break;
            }
#endif
            SwitchF(us.fInterpret);
            break;
#endif

            /* Switches which affect how the chart parameters are obtained: */

#ifdef TIME
        case 'n':
            FInputData(szNowCore);
            if (ch1 == 'd')
                TT = 0.0;
            else if (ch1 == 'm') {
                DD = 1;
                TT = 0.0;
            } else if (ch1 == 'y') {
                MM = DD = 1;
                TT = 0.0;
            }
            break;
#endif

        case 'z':
            if (ch1 == '0') {
                if (argc <= 1 || RParseSz(argv[1], pmZon) == rLarge) {
                    i = us.dstDef != 0.0;
                    SwitchF(i);
                    SS = us.dstDef = i ? 1.0 : 0.0;
                } else {
                    SS = us.dstDef = RParseSz(argv[1], pmZon);
                    if (!FValidDst(us.dstDef)) {
                        ErrorValR("z0", us.dstDef);
                        return fFalse;
                    }
                    argc--;
                    argv++;
                }
                break;
            } else if (ch1 == 'l') {
                if (argc <= 2) {
                    ErrorArgc("zl");
                    return fFalse;
                }
                OO = us.lonDef = RParseSz(argv[1], pmLon);
                AA = us.latDef = RParseSz(argv[2], pmLat);
                if (!FValidLon(us.lonDef)) {
                    ErrorValR("zl", us.lonDef);
                    return fFalse;
                } else if (!FValidLat(us.latDef)) {
                    ErrorValR("zl", us.latDef);
                    return fFalse;
                }
                argc -= 2;
                argv += 2;
                break;
            } else if (ch1 == 'L' ) {
                if (argc <= (1+(ch2 == '0'))) {
                    ErrorArgc("zL");
                    return fFalse;
                }
                if (ch2 == '0')
                    us.nLocationCount=atoi(argv[2]);
                else
                    us.nLocationCount=1;
                ciCore.loc=AtlasRequest(argv[1],(&OO),(&AA),(&NN));
                us.lonDef=OO;
                us.latDef=AA;
                argc-=(1+(ch2 == '0'));
                argv+=(1+(ch2 == '0'));
                break;
            } else if (ch1 == 't') {
                if (argc <= 1) {
                    ErrorArgc("zt");
                    return fFalse;
                }
                rT = RParseSz(argv[1], pmTim);
                if (!FValidTim(rT)) {
                    ErrorValR("zt", rT);
                    return fFalse;
                }
                TT = rT;
                argc--;
                argv++;
                break;
            } else if (ch1 == 'd') {
                if (argc <= 1) {
                    ErrorArgc("zd");
                    return fFalse;
                }
                i = NParseSz(argv[1], pmDay);
                if (!FValidDay(i, MM, YY)) {
                    ErrorValN("zd", i);
                    return fFalse;
                }
                DD = i;
                argc--;
                argv++;
                break;
            } else if (ch1 == 'm') {
                if (argc <= 1) {
                    ErrorArgc("zm");
                    return fFalse;
                }
                i = NParseSz(argv[1], pmMon);
                if (!FValidMon(i)) {
                    ErrorValN("zm", i);
                    return fFalse;
                }
                MM = i;
                argc--;
                argv++;
                break;
            } else if (ch1 == 'y') {
                if (argc <= 1) {
                    ErrorArgc("zy");
                    return fFalse;
                }
                i = NParseSz(argv[1], pmYea);
                if (!FValidYea(i)) {
                    ErrorValN("zy", i);
                    return fFalse;
                }
                YY = i;
                argc--;
                argv++;
                break;
            } else if (ch1 == 'a') {
                if (argc <= 1) {
                    ErrorArgc("za");
                    return fFalse;
                }
                i = RParseSz(argv[1], pmAlt);
                NN = us.altDef = i;
                argc--;
                argv++;
                break;
            } else if (ch1 == 'p') {
                if (argc <= 1) {
                    ErrorArgc("zp");
                    return fFalse;
                }
                i = RParseSz(argv[1], pmAtp);
                us.atpressDef = i;
                argc--;
                argv++;
                break;
            } else if (ch1 == 'T') {
                if (argc <= 1) {
                    ErrorArgc("zT");
                    return fFalse;
                }
                i = RParseSz(argv[1], pmAtt);
                us.attempDef = i;
                argc--;
                argv++;
                break;

            } else if (ch1 == 'i') {
                if (argc <= 2) {
                    ErrorArgc("zi");
                    return fFalse;
                }
                ciCore.nam = SzPersist(argv[1]);
                ciCore.loc = SzPersist(argv[2]);
                argc -= 2;
                argv += 2;
                break;
            }
#ifdef KDE
            else if (ch1 == 'f') {
                char buf[60];
                int f;
                buf[0] = 0;
                if (argc <= 1) {
                    ErrorArgc("zf");
                    return fFalse;
                }
                if (strncmp(argv[1], ZoneInfoPath, strlen(ZoneInfoPath)))
                    strncpy(buf, ZoneInfoPath, 60);
                strncat(buf, argv[1], 60);
                if ((f = open(buf, 0)) == -1) {
                    ErrorArgv("zf");
                    return fFalse;
                }
                else
                    close(f);
                /* Here, it seems we must create a local copy of buf */
                is.fSzPersist = fFalse;
                ciCore.tzfile = SzPersist(buf);
                is.fSzPersist = fTrue;
                argc--;
                argv++;
                break;
            }
#endif
            if (argc <= 1 || RParseSz(argv[1], pmZon) == rLarge)
                ZZ -= 1.0;
            else {
                ZZ = us.zonDef = RParseSz(argv[1], pmZon);
                if (!FValidZon(us.zonDef)) {
                    ErrorValR("z", us.zonDef);
                    return fFalse;
                }
                argc--;
                argv++;
            }
            break;

        case 'q':
            i = (ch1 == 'y' || ch1 == 'j') + 2*(ch1 == 'm') + 3*(ch1 == 'd') +
                4*(ch1 == chNull) + 7*(ch1 == 'a') + 8*(ch1 == 'b')+ 9*(ch1 == 'c');
            if (argc <= i) {
                ErrorArgc("q");
                return fFalse;
            }
            is.fHaveInfo = fTrue;
            if (ch1 == 'j') {
                is.JD = atof(argv[1])+rRound;
                TT = RFract(is.JD);
                JulianToMdy(is.JD-TT, &MM, &DD, &YY);
                TT = DegToDec(TT * 24.0);
                SS = ZZ = 0.0;
                OO = us.lonDef;
                AA = us.latDef;
                NN = us.altDef;
            } else {
                MM = i > 1 ? NParseSz(argv[1], pmMon) : 1;
                DD = i > 2 ? NParseSz(argv[2], pmDay) : 1;
                YY = NParseSz(argv[3-(i<3)-(i<2)], pmYea);
                TT = i > 3 ? RParseSz(argv[4], pmTim) : (i < 3 ? 0.0 : 12.0);
                SS = i > 7 ? RParseSz(argv[5], pmDst) : (i > 6 ? 0.0 : us.dstDef);
                ZZ = i > 6 ? RParseSz(argv[5 + (i > 7)], pmZon) : us.zonDef;
                OO = i > 6 ? RParseSz(argv[6 + (i > 7)], pmLon) : us.lonDef;
                AA = i > 6 ? RParseSz(argv[7 + (i > 7)], pmLat) : us.latDef;
                NN = i > 8 ? RParseSz(argv[9], pmAlt) : us.altDef;
                if (!FValidMon(MM)) {
                    ErrorValN("q", MM);
                    return fFalse;
                } else if (!FValidDay(DD, MM, YY)) {
                    ErrorValN("q", DD);
                    return fFalse;
                } else if (!FValidYea(YY)) {
                    ErrorValN("q", YY);
                    return fFalse;
                } else if (!FValidTim(TT)) {
                    ErrorValR("q", TT);
                    return fFalse;
                } else if (!FValidDst(SS)) {
                    ErrorValR("q", SS);
                    return fFalse;
                } else if (!FValidZon(ZZ)) {
                    ErrorValR("a", ZZ);
                    return fFalse;
                } else if (!FValidLon(OO)) {
                    ErrorValR("a", OO);
                    return fFalse;
                } else if (!FValidLat(AA)) {
                    ErrorValR("a", AA);
                    return fFalse;
                }
            }
            argc -= i;
            argv += i;
            break;

        case 'i':
            if (us.fNoRead) {
                ErrorArgv("i");
                return tcError;
            }
            if (argc <= 1) {
                ErrorArgc("i");
                return fFalse;
            }
            ci = ciCore;
            if (!FInputData(argv[1]))
                return fFalse;
            if (ch1 == '2') {
                ciTwin = ciCore;
                ciCore = ci;
            } else if (ch1 == '3') {
                ciThre = ciCore;
                ciCore = ci;
            } else if (ch1 == '4') {
                ciFour = ciCore;
                ciCore = ci;
            }
            argc--;
            argv++;
            break;

        case '>':
            ch1 = 's';
            /* Fall through */

        case 'o':
            if (us.fNoWrite) {
                ErrorArgv("o");
                return tcError;
            }
            if (argc <= 1) {
                ErrorArgc("o");
                return fFalse;
            }
            if (ch1 == 's') {
                is.szFileScreen = SzPersist(argv[1]);
                argc--;
                argv++;
                break;
            } else if (ch1 == '0')
                SwitchF(us.fWritePos);
            SwitchF(us.fWriteFile);
            is.szFileOut = SzPersist(argv[1]);
            if (is.fSzPersist) {
                is.rgszComment = argv;
                do {
                    argc--;
                    argv++;
                    is.cszComment++;
                } while (argc > 1 && !FChSwitch(argv[1][0]));
            }
            break;

            /* Switches which affect what information is used in a chart: */

        case 'R':
            if (ch1 == 'A') {
                while (argc > 1 && (i = NParseSz(argv[1], pmAspect)))
                    if (!FAspect(i)) {
                        ErrorValN("RA", i);
                        return fFalse;
                    } else {
                        rAspOrb[i] = -rDegHalf;
                        argc--;
                        argv++;
                    }
                break;
            }
            if (ch1 == 'T') {
                pch = (char *)ignore2;
                ch1 = argv[0][++ich];
            } else  if (ch1 == 'S') {
                pch = (char *)ignoreS;
                ch1 = argv[0][++ich];
            } else  if (ch1 == 'P') {
                pch = (char *)ignore3;
                ch1 = argv[0][++ich];
            } else pch = (char *)ignore;
            if (ch1 == '0') {
                for (i = 0; i <= cObj; i++)
                    pch[i] = fTrue;
                ch1 = argv[0][++ich];
            }
            else if (ch1 == '1') {
                for (i = 0; i <= cObj ; i++)
                    pch[i] = fFalse;
                ch1 = argv[0][++ich];
            }
            if (ch1=='R')
                for (i = oChi; i <= oVes; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'C')
                for (i = cuspLo; i <= cuspHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'F')
                for (i = FastLo; i <= FastHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'I')
                for (i = oMer; i <= oMar; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'O')
                for (i = oJup; i <= oPlu; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'L')
                for (i = oSun; i <= oMoo; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'x')
                for (i = oAsc; i <= oMC; i=i+3)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'b')
                for (i = oEar; i <= oPlu; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'u')
                for (i = starLo; i <= starHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'h')
                for (i = HypuranLo; i <= HypuranHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'l')
                for (i = LunarLo; i <= LunarHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
#ifdef ASTEROIDS
            else if (ch1 == 'c')
                for (i = CentLo; i <= CentHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 't')
                for (i = TrneLo; i <= TrneHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'a')
                for (i = AtenLo; i <= AtenHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
#endif
#ifdef ASTEROIDS
            else if (ch1 == 'e')
                for (i = AsteLo+us.nExtraStar; i <= AsteHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
#endif
#ifdef ASTEROIDS
            else if (ch1 == 'm')
                for (i = AmorLo; i <= AmorHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'M')
                for (i = MuseLo; i <= MuseHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'p')
                for (i = ApolLo; i <= ApolHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
            else if (ch1 == 'o')
                for (i = AgenLo; i <= AgenHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
#endif
#ifdef ASTEROIDS
            else if (ch1 == 'y'){
                for (i = RiyaLo; i <= RiyaHi; i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
                    }
#endif
            else if (ch1 == 'U'){
                for (i = starLo; i <= ((us.nStar && !us.fRiyalAst) ? starLo+cStar-1:starHi); i++)
                    if (ch2=='0')
                        pch[i] = fTrue;
                    else if (ch2=='1')
                        pch[i] = fFalse;
                    else SwitchF(pch[i]);
                    }
            while (argc > 1 && !FChSwitch(argv[1][0]) && (i = NParseSz(argv[1], pmObject))>=0){
                if (!FItem(i)) {
                    if (i!=-1){
                        ErrorValN("R", i);
                        return fFalse;
                    } else i=0;
                }
                SwitchF(pch[i]);
                argc--;
                argv++;
            }
            break;

        case 'C':
            SwitchF(us.fCusp);
            break;

        case 'u':
            SwitchF(us.fUranian);
            break;

        case 'U':
            if (ch1 == 'z' || ch1 == 'l' || ch1 == 'n' || ch1 == 'b' || ch1 == 'r' || ch1 == 'p')
                us.nStar = ch1;
            else
            {
                SwitchF(us.nStar);
#ifdef ASTEROIDS
                if (!us.fRiyalAst) {
                    us.nExtraStar=us.nExtraStar ? 0 : cStar-cStarSmall+RiyaLo-RiyaHi-1;
                    ComputeStars(0);
                }
                else
                    us.nExtraStar=0;
#endif
            }
            break;

        case 'A':
            if (ch1 != 'o' && ch1 != 'm' && ch1 != 'd' && ch1 != 'a' &&
                    ch1 != 'n' && ch1 != 'A') {
                if (argc <= 1) {
                    ErrorArgc("A");
                    return fFalse;
                }
                i = NParseSz(argv[1], pmAspect);
                if (!FValidAspect(i)) {
                    ErrorValN("A", i);
                    return fFalse;
                }
                us.nAsp = i;
                argc--;
                argv++;
            } else {
                if (argc <= 2) {
                    ErrorArgc("A");
                    return fFalse;
                }
                if (ch1 == 'n' || ch1 == 'A') {
                    i = NParseSz(argv[1], pmAspect);
                    if (i < 1 || i > cAspect) {
                        ErrorValN("A", i);
                        return fFalse;
                    }
                    sz = SzPersist(argv[2]);
                    if (ch1 == 'n')
                        szAspectName[i] = sz;
                    else {
                        szAspectAbbrev[i] = sz;
                        if (i == 1)
                            szAspectAbbrevCon = sz;
                        if (i == 2)
                            szAspectAbbrevOpp = sz;
                    }
                } else {
                    i = NParseSz(argv[1], ch1 == 'o' || ch1 == 'a' ? pmAspect : pmObject);
                    if (i < 1 || i > (ch1 == 'o' || ch1 == 'a' ? cAspect : oNormOpt)) {
                        ErrorValN("A", i);
                        return fFalse;
                    }
                    rT = RParseSz(argv[2], 0);
                    if (rT < -rDegMax || rT > rDegMax) {
                        ErrorValR("A", rT);
                        return fFalse;
                    }
                    if (ch1 == 'o')
                        rAspOrb[i] = rT;
                    else if (ch1 == 'm')
                        rObjOrb[i] = rT;
                    else if (ch1 == 'd')
                        rObjAdd[i] = rT;
                    else
                        rAspAngle[i] = rT;
                }
                argc -= 2;
                argv += 2;
            }
            break;

            /* Switches which affect how a chart is computed: */

        case 'b':
            if (ch1 == '0'){
                SwitchF(us.fSeconds);
                is.fSeconds = us.fSeconds;
                break;
            }
            if (ch1 == '1'){
                SwitchF(us.fVerboseHeader);
                break;
            }
            SwitchF(us.fPlacalc);
            break;

        case 'c':
            if (argc <= 1) {
                ErrorArgc("c");
                return fFalse;
            }
            i = NParseSz(argv[1], pmSystem);
            if (!FValidSystem(i)) {
                ErrorValN("c", i);
                return fFalse;
            }
            us.nHouseSystem = i;
            argc--;
            argv++;
            break;

        case 's':
            if (argc > 1 && ch1!='t' && ch1!='m' && ((rT = atof(argv[1])) != 0.0 || FNumCh(argv[1][0]))) {
                if (!FValidOffset(rT)) {
                    ErrorValR("s", rT);
                    return fFalse;
                }
                argc--;
                argv++;
                us.rZodiacOffset = rT;
            }
            if (ch1 == 'r')
                SwitchF(us.fEquator);
            else if (ch1 == 'h')
                us.nDegForm = 1;
            else if (ch1 == 'd')
                us.nDegForm = 2;
            else if (ch1 == 'z')
                us.nDegForm = 0;
            else if (ch1 == 's')
                SwitchF(us.fSidSolSysPlane);
            else if (ch1 == 'g'){
                SwitchF(us.fGalPlane);
            }
            else if (ch1 == 't'){
                if (argc <= 1) {
                    ErrorArgc("st");
                    return fFalse;
                }
                us.nSidEclDate=atof(argv[1]);
                argc--;
                argv++;
            }
            else if (ch1 == 'm') {
                if (argc > 1 && (j = atoi(argv[1])) >= 0) {
                    if (j>21 && j!=255 ) {
                        ErrorValR("sm", j);
                        return fFalse;
                    }
                    argc--;
                    argv++;
                    us.fSiderealMode = j;
                    if (j==255)
                        us.fSidEclDate=fTrue;
                    else
                        us.fSidEclDate=fFalse;
                }
            }
            else
                SwitchF(us.fSidereal);
            break;

        case 'h':
            if (argc > 1 && (i = NParseSz(argv[1], pmObject))) {
                argc--;
                argv++;
            } else
                i = FSwitchF(us.objCenter != 0);
            if (!FValidCenter(i)) {
                ErrorValN("h", i);
                return fFalse;
            }
            us.objCenter = i;
            break;

        case 'p':
            if (fAnd) {
                us.fProgress = fFalse;
                break;
            }
            us.fSolarArc = 0;
            if (ch1 == '0')
                us.fSolarArc = 1;
            if (ch1 == '1')
                us.fSolarArc = 2;
            if (ch1 == '3')
                us.fSolarArc = 3;
            if (us.fSolarArc)
                    ch1 = (argv[0][++ich]);
            if (ch1 == '2'){
                SwitchF(us.fDirect);
                ch1 = (argv[0][++ich]);}
            if (ch1 == 't') {
               SwitchF(us.fTransProg);
                break;
            }
            if (ch1 == 'd') {
              fBirthRate=0;
              if (ch2 == 's'){
                SwitchF(fSolarArcKey); /* Solar arc in RA  */;break;}
              if (ch2 == 'b'){
                fBirthRate=1 /* Birth rate in RA +- 1 day */;break;}
              if (ch2 == 'v'){
                fBirthRate=2 /* Birth velocity in RA  */;break;}
              if (ch2 == 'n'){
                fBirthRate=3 ;
                rBirthRate=rConverseBirthRate = 0.98564731 /* Naibod */;break;}
              if (ch2 == 'p'){
                us.rProgDay = 365.24218967 /* Ptolemy */;break;}
              if (ch2 == 'c'){
                fBirthRate=4;
                rBirthRate=rConverseBirthRate = 0.98666666 /* Cardan */;break;}
                if (argc <= 1) {
                    ErrorArgc("pd");
                    return fFalse;
                }
                us.rProgDay = atof(argv[1]);
                if (us.rProgDay == 0.0) {
                    ErrorValR("pd", us.rProgDay);
                    return fFalse;
                }
                argc--;
                argv++;
                break;
            }
            if (ch1 == 'a') {
                if (argc <= 1) {
                    ErrorArgc("pa");
                    return fFalse;
                }
                rAddAngle = Mod(atof(argv[1]));
                if (RAbs(rAddAngle) > rDegMax) {
                    ErrorValR("pa", rAddAngle);
                    return fFalse;
                }
                argc--;
                argv++;
                break;
            }
            if (ch1 == 's') {
                SwitchF(fSecMotion);
                break;
            }
            if (ch1 == 'm') {
                if (ch2 == '0')
                     fGauqMode=0;
                if (ch2 == '1')
                     fGauqMode=1;
                if (ch2 == '2')
                     fGauqMode=2;
                if (ch2 == '3')
                     fGauqMode=3;
                if (ch2 == '4')
                     fGauqMode=4;
                if (ch2 == '5')
                     fGauqMode=5;
                break;
                }
            us.fProgress = fTrue;
#ifdef TIME
            if (ch1 == 'n') {
                GetTimeNow(&Mon, &Day, &Yea, &Tim, us.zonDef - us.dstDef);
                if (us.fRelocation){
                    Lon = us.lonDef;
                    Lat = us.latDef;
                    Alt = us.altDef;
                } else {
                    Lon = OO;
                    Lat = AA;
                    Alt = NN;
                }
                is.JDp = MdytszToJulian(Mon, Day, Yea, Tim, us.dstDef, us.zonDef)-rRound;
                break;
            }
#endif
            if (argc <= 3) {
                ErrorArgc("p");
                return fFalse;
            }
            Mon = NParseSz(argv[1], pmMon);
            Day = NParseSz(argv[2], pmDay);
            Yea = NParseSz(argv[3], pmYea);
            if (!FValidMon(Mon)) {
                ErrorValN("p", Mon);
                return fFalse;
            } else if (!FValidDay(Day, Mon, Yea)) {
                ErrorValN("p", Day);
                return fFalse;
            } else if (!FValidYea(Yea)) {
                ErrorValN("p", Yea);
                return fFalse;
            }
            if (us.fRelocation){
                Lon = us.lonDef;
                Lat = us.latDef;
                Alt = us.altDef;
            } else {
                Lon = OO;
                Lat = AA;
                Alt = NN;
            }
            if (ch1 == '4') {
                Tim = RParseSz(argv[4], pmTim);
                Zon=us.zonDef;
                Dst=us.dstDef;
                argc--;
                argv++;
            }
            is.JDp = MdytszToJulian(Mon, Day, Yea, (ch1=='4') ? Tim : 0.0, us.dstDef, us.zonDef)-rRound;
            argc -= 3;
            argv += 3;
            break;

        case 'x':
            if (argc <= 1) {
                ErrorArgc("x");
                return fFalse;
            }
            i = atoi(argv[1]);
            if (!FValidHarmonic(i)) {
                ErrorValN("x", i);
                return fFalse;
            }
            us.nHarmonic = i;
            argc--;
            argv++;
            break;

        case '1':
            if (argc > 1 && (i = NParseSz(argv[1], pmObject))) {
                argc--;
                argv++;
            } else
                i = oSun;
            if (!FItem(i)) {
                ErrorValN("1", i);
                return fFalse;
            }
            us.objOnAsc = fAnd ? 0 : i;
            break;

        case '2':
            if (argc > 1 && (i = NParseSz(argv[1], pmObject))) {
                argc--;
                argv++;
            } else
                i = oSun;
            if (!FItem(i)) {
                ErrorValN("2", i);
                return fFalse;
            }
            us.objOnAsc = fAnd ? 0 : -i;
            break;

        case '3':
            SwitchF(us.fDecan);
            break;

        case 'f':
            SwitchF(us.fFlip);
            break;

        case 'G':
            SwitchF(us.fGeodetic);
            break;

        case 'J':
            SwitchF(us.fVedic);
            break;

        case '9':
            SwitchF(us.fNavamsa);
            break;

        case 'F':
            if (argc <= 3) {
                ErrorArgc("F");
                return fFalse;
            }
            i = NParseSz(argv[1], pmObject);
            if (!FItem(i)) {
                ErrorValN("F", i);
                return fFalse;
            }
            force[i] = (NParseSz(argv[2], pmSign)-1.0)*30.0+DecToDeg(atof(argv[3]));
            if (force[i] < -rDegMax || force[i] >= rDegMax) {
                ErrorValR("F", force[i]);
                return fFalse;
            } else
                force[i] += rDegMax;
            argc -= 3;
            argv += 3;
            break;

        case '+':
            if (argc > 1 && (i = atoi(argv[1])) != 0) {
                argc--;
                argv++;
            } else
                i = 1;
            us.dayDelta += i * (ch1 == 'y' ? 365 : (ch1 == 'm' ? 30 : 1));
            break;

        case chNull:
            if (ich <= 1)
                break;
            /* Fall thorugh */

        case '-':
            if (argc > 1 && (i = atoi(argv[1])) != 0) {
                argc--;
                argv++;
            } else
                i = 1;
            us.dayDelta -= i * (ch1 == 'y' ? 365 : (ch1 == 'm' ? 30 : 1));
            break;

            /* Switches for relationship and comparison charts: */

        case 'r':
            if (fAnd) {
                us.nRel = 0;
                break;
            } else if (FBetween(ch1, '1', '4')) {
                us.nRel = -(int)(ch1-'1');
                break;
            }
            i = 2 + 2*((ch1 == 'c' || ch1 == 'm') && ch2 == '0');
            if (argc <= i) {
                ErrorArgc("r");
                return fFalse;
            }
            if (ch1 == 'c')
                us.nRel = rcComposite;
            else if (ch1 == 'm')
                us.nRel = rcMidpoint;
            else if (ch1 == 'd')
                us.nRel = rcDifference;
#ifdef BIORHYTHM
            else if (ch1 == 'b')
                us.nRel = rcBiorhythm;
#endif
            else if (ch1 == '0')
                us.nRel = rcDual;
            else if (ch1 == 't')
                us.nRel = rcTransit;
            else if (ch1 == 'p') {
                us.nRel = rcProgress;
                us.fSolarArc = 0;
                if (ch2 == '0')
                    us.fSolarArc = 1;
                if (ch2 == '1')
                    us.fSolarArc = 2;
                if (ch2 == '3')
                    us.fSolarArc = 3;
                if (ch2 == '2')
                    us.fDirect = fTrue;
            } else
                us.nRel = rcSynastry;
            if (ch1=='c' && ch2=='3'){
                ciThre = ciCore;
                fProgressRatio=fTrue;
            }
            ci = ciCore;
            if (!FInputData(argv[2]))
                return fFalse;
            ciTwin = ciCore;
            ciCore = ci;
            if (!FInputData(argv[1]))
                return fFalse;
            ciOne = ciCore;
            if (i > 2) {
                us.nRatio1 = atoi(argv[3]);
                us.nRatio2 = atoi(argv[4]);
                if (us.nRatio1 == us.nRatio2)
                    us.nRatio1 = us.nRatio2 = 1;
            }
            argc -= i;
            argv += i;
            break;

#ifdef TIME
        case 'y':
            if (argc <= 1) {
                ErrorArgc("y");
                return fFalse;
            }
            if (ch1 == 'd')
                us.nRel = rcDifference;
#ifdef BIORHYTHM
            else if (ch1 == 'b')
                us.nRel = rcBiorhythm;
#endif
            else if (ch1 == 't')
                us.nRel = rcTransit;
            else if (ch1 == 'r'){
                us.nRel = rcDual;
                fReturnChart=fTrue;
                returnnatalpos=-1;
            }
            else if (ch1 == 's'){
                us.nRel = rcDual;
              /*  if (!us.fSector)*/
                     fReturnChart=fTrue;
                fSynodicReturnChart=fTrue;
                returnnatalpos=-1;
                if (ch2 == '0')
                     fGauqMode=0;
                if (ch2 == '1')
                     fGauqMode=1;
                if (ch2 == '2')
                     fGauqMode=2;
                if (ch2 == '3')
                     fGauqMode=3;
                if (ch2 == '4')
                     fGauqMode=4;
                if (ch2 == '5')
                     fGauqMode=5;
            }
            else if (ch1 == 'p') {
                us.nRel = rcProgress;
                us.fSolarArc = 0;
                if (ch2 == '0')
                    us.fSolarArc = 1;
                if (ch2 == '1')
                    us.fSolarArc = 2;
                if (ch2 == '3')
                    us.fSolarArc = 3;
                if (ch2 == '2')
                    us.fDirect = fTrue;
            } else
                us.nRel = rcDual;
            /*  if (!FInputData(szNowCore))
                return fFalse; */
            ciTwin = ciCore;
            if (!FInputData(argv[1]))
                return fFalse;
            argc--;
            argv++;
            if (ch1 == 'r' && argc <= 1){
                ErrorArgc("yr");
                return fFalse;
            }
            if (ch1 == 's' && argc <= 2){
                ErrorArgc("ys");
                return fFalse;
            }
            if (ch1 == 'r'||ch1=='s'){
                us.nEclObj=NParseSz(argv[1], pmObject);
                if (!FObject(i)) {
                    ErrorValN("ZEo", i);
                    return tcError;
                }
                argc--;
                argv++;
            }
            if (ch1 =='s'){
                us.nEclObj2=NParseSz(argv[1], pmObject);
                if (!FObject(i)) {
                    ErrorValN("ZEo", i);
                    return tcError;
                }
                argc--;
                argv++;
                if (us.nEclObj2==us.nEclObj)
                    fSynodicReturnChart=fFalse;
            }
            break;
#endif

            /* Switches to access graphics options: */

        case 'k':
            if (ch1 == '1') {
                us.fAnsiColor = 2;
                us.fAnsiChar  = 1;
            } else {
                if (ch1 != '0')
                    SwitchF(us.fAnsiColor);
                SwitchF(us.fAnsiChar);
            }
            break;

#ifdef PCG
        case 'V':
            if (argc <= 1) {
                ErrorArgc("V");
                return fFalse;
            }
            i = atoi(argv[1]);
            if (!FValidTextrows(i)) {
                ErrorValN("V", i);
                return fFalse;
            }
            gs.nTextRows = i;
            argc--;
            argv++;
            break;
#endif

#ifdef GRAPH
        case 'X':
            if (us.fNoGraphics) {
                ErrorArgv("X");
                return fFalse;
            }
            i = NProcessSwitchesX(argc, argv, ich, fOr, fAnd, fNot);
            if (i < 0)
                return fFalse;
            SwitchF2(us.fGraphics);
            argc -= i;
            argv += i;
            break;

#ifdef WIN
        case 'W':
            i = NProcessSwitchesW(argc, argv, ich, fOr, fAnd, fNot);
            if (i < 0)
                return fFalse;
            argc -= i;
            argv += i;
            break;
#endif
#endif /* GRAPH */

        case '0':
            while (ch1 != chNull) {
                switch (ch1) {
                case 'o':
                    us.fNoWrite    = fTrue;
                    break;
                case 'i':
                    us.fNoRead     = fTrue;
                    break;
                case 'q':
                    us.fNoQuit     = fTrue;
                    break;
                case 'X':
                    us.fNoGraphics = fTrue;
                    break;
                }
                ch1 = (argv[0][++ich]);
            }
            break;

        case ';':    /* The -; switch means don't process the rest of the line. */
            return fTrue;

        case '@':    /* The -@ switch is just a system flag indicator no-op. */
            break;

        case '.':                /* "-." is usually used to exit the -Q loop. */
            Terminate(tcForce);

        case 'B':                /* For no useful reason, -B sounds a beep. */
#ifndef WIN
            putchar(chBell);
#else
            MessageBeep((UINT)-1);
#endif
            break;

        default:
            ErrorSwitch(argv[0]);
            return fFalse;
        }
        argc--;
        argv++;
    }
    return fTrue;
}


/*
******************************************************************************
** Main Program.
******************************************************************************
*/

#ifndef WIN
#ifndef NOMAIN
/* The main program, the starting point for Astrolog, follows. This routine */
/* basically consists of a loop, inside which we read a command line, and   */
/* go process it, before actually calling a routine to do the neat stuff.   */

#ifdef SWITCHES
#ifdef KDE
void kmain(int argc, char **argv)
{
#else
int main(argc, argv)
int argc;
char **argv;
{
#endif
#else
int main()
{
    int argc;
    char **argv;
#endif
    char szCommandLine[cchSzMax], *rgsz[MAXSWITCHES];
#ifdef KDE
    AllocateNear(us.tzfile, strlen(DEFAULT_TZFILE));
    strcpy(us.tzfile, DEFAULT_TZFILE);
#endif
    /* Read in info from the astrolog.dat file. */
    is.S = stdout;
    FProcessSwitchFile(DEFAULT_INFOFILE, NULL);
#ifdef KDE
    is.readComments = fTrue;
#endif

LBegin:
#ifdef PCG
    if (gs.nTextRows > 0) {
        PcSetTextRows(gs.nTextRows);
        neg(gs.nTextRows);
    }
#endif
    if (us.fNoSwitches) {                             /* Go prompt for    */
        argc = NPromptSwitches(szCommandLine, rgsz);    /* switches if we   */
        argv = rgsz;                                    /* don't have them. */
    }
    is.szProgName = argv[0];
    is.fSzPersist = fTrue;
    if (FProcessSwitches(argc, argv)) {
        if (!us.fNoSwitches && us.fLoopInit) {
            us.fNoSwitches = fTrue;
            goto LBegin;
        }
#ifdef PCG
        if (gs.nTextRows > 0) {
            PcSetTextRows(gs.nTextRows);
            neg(gs.nTextRows);
        }
#endif
#ifdef KDE
        if (us.fGraphics)
            gi.fiskde = fTrue;
#endif
        Action();
    }
    if (us.fLoop || us.fNoQuit) { /* If -Q in effect loop back and get switch */
        PrintL2();                  /* info for another chart to display.       */
        InitVariables();
        us.fLoop = us.fNoSwitches = fTrue;
        goto LBegin;
    }
    Terminate(tcOK);    /* The only standard place to exit Astrolog is here. */
    return tcOK;
}
#endif /* NOMAIN */
#endif /* WIN */

/* astrolog.c */
