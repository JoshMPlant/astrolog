/*
** Astrolog (Version 5.42I) File: extern.h
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
** Modifications from version 5.41f to 5.42A are by Nicolas Scharnagl.
*/

/*
******************************************************************************
** Function Declarations.
******************************************************************************
*/

#ifdef PROTO
#define P(t) t
#else
#define P(t) ()
#endif

/* From astrolog.c */

extern void InitColors P((void));
extern void Action P((void));
extern void InitVariables P((void));
extern bool FProcessCommandLine P((char *));
extern int NParseCommandLine P((char *, char **));
extern int NPromptSwitches P((char *, char *[MAXSWITCHES]));
extern int NProcessSwitchesRare P((int, char **, int, bool, bool, bool));
extern bool FProcessSwitches P((int, char **));


/* From data.c & data2.c */

#define MM ciCore.mon
#define DD ciCore.day
#define YY ciCore.yea
#define TT ciCore.tim
#define ZZ ciCore.zon
#define SS ciCore.dst
#define OO ciCore.lon
#define AA ciCore.lat
#define oo ciCore.lond
#define aa ciCore.latd
#define NN ciCore.alt

#define Mon ciMain.mon
#define Day ciMain.day
#define Yea ciMain.yea
#define Tim ciMain.tim
#define Zon ciMain.zon
#define Dst ciMain.dst
#define Lon ciMain.lon
#define Lat ciMain.lat
#define Alt ciMain.alt

#define MonT ciTran.mon
#define DayT ciTran.day
#define YeaT ciTran.yea
#define TimT ciTran.tim
#define ZonT ciTran.zon
#define DstT ciTran.dst
#define LonT ciTran.lon
#define LatT ciTran.lat
#define AltT ciTran.alt

#define planet    cp0.obj
#define planetalt cp0.alt
#define planetdis cp0.dis
#define ret       cp0.dir
#define altret    cp0.altdir
#define disret    cp0.disdir
#define chouse    cp0.cusp
#define inhouse   cp0.house

#define ignorea(a) (rAspOrb[a] <= 0.0)

extern US NPTR us;
extern IS NPTR is;
extern CI ciCore, ciMain, ciOne , ciTwin, ciThre, ciFour, ciTran, ciSave;
extern CP cp0, cp1, cp2,cp3;

extern real spacex[oNorm+1], spacey[oNorm+1], spacez[oNorm+1], force[objMax];
extern real spacedx[oNorm+1], spacedy[oNorm+1], spacedz[oNorm+1];
extern GridInfo FPTR *grid;
extern int starname[cStar+1], kObjA[objMax], kObjA1[objMax];

extern byte ignore[objMax], ignore2[objMax], ignore3[objMax], ignoreS[objMax], ignorez[4],
    pluszone[cSector+1];
extern real rAspAngle[cAspect+1], rAspOrb[cAspect+1], rObjOrb[objMax],
    rObjAdd[objMax];
extern int ruler1[objMax], ruler2[objMax], exalt[objMax], rules[cSign+1],
    kMainA[9], kRainbowA[8], kElemA[4], kAspA[cAspect+1];
extern real rObjInf[objMax+2], rHouseInf[cSign+3], rAspInf[cAspect+1],
    rTransitInf[objMax+2];
#ifdef ASTEROIDS
extern int skip[],astnr[AsteHi-AsteLo+1];
#endif
extern CONST char  *szAppName;
extern
#ifndef KDE
    CONST
#endif
    char *szSignName[cSign+9], *szSignAbbrev[cSign+1],
    *szSignEnglish[cSign+1], *szHouseTradition[cSign+1], *szSystem[cSystem],
    *szAspectGlyph[cAspect+1],*szAspectAbbrev[cAspect+1], *szAspectName[cAspect+1],
    *szAspectConfig[cAspConfig+1], *szElem[4], *szMode[3], *szMonth[cSign+1],
    *szDay[cWeek], *szDir[4], *szSuffix[cSign+1];
extern CONST real rZon[cZone];
extern CONST char *szZon[cZone], *szCnstlName[cCnstl+1],
    *szCnstlAbbrev[cCnstl+1],  *szCnstlGenitive[cCnstl+1];
extern char *szAspectAbbrevCon, *szAspectAbbrevOpp;
extern char *szObjName[objMax],*riyalObjName[cRiya],*cometName[MaxComet];
extern
#ifndef KDE
    CONST
#endif
    char *szCnstlMeaning[cCnstl+1];
extern char *szStarConName[cStar+1];
extern real rStarBright[cStar+1],rStarDist[cStar+1];
extern CONST real rStarData[cStarSmall*6];
extern char *szMindPart[cObjInt+1], *szDesc[cSign+1], *szDesire[cSign+1],
    *szLifeArea[cSign+1], *szInteract[cAspectInt+1], *szTherefore[cAspectInt+1],
    *szModify[3][cAspectInt],*szSabian[nDegMax];

extern CONST real rObjDist[oVes+1], rObjYear[oVes+1], rObjDiam[oVes+1],
    rObjDay[oPlu+1], rObjMass[oPlu+1], rObjAxis[oPlu+1];
extern CONST byte cSatellite[oPlu+1];
extern AI ai[cPart];

extern CONST byte rErrorCount[oPlu-oJup+1];
extern CONST byte rErrorOffset[oPlu-oJup+1];
extern CONST real rErrorData[72+51+42*3];
extern OE rgoe[oVes+1+HypuranHi-HypuranLo];
extern char *szMacro[48];
#ifdef KDE
extern char *szMacroName[48];
extern char *ColorTbl[];
#endif
extern CONST char *szColor[cColor];
extern char nullnamloc[1];
extern byte fEquator;

extern real PowerPar;
extern int MonthFormat;

/* Stars colors. Added by V.A. */
extern int kStar1;
extern int kStar2;
extern int kStar3;

/* From general.c */

#define PrintAltitude(deg) PrintSz(SzAltitude(deg))
#define ErrorValR(sz, r) ErrorValN(sz, (int)r)

extern void SwapR P((real *, real *));
extern int CchSz P((CONST char *));
extern int NCompareSz P((CONST char *, CONST char *));
extern void ClearB P((lpbyte, int));
extern void CopyRgb P((byte *, byte *, int));
extern real RSgn P((real));
extern real Angle P((real, real));
extern real Mod P((real));
extern real ModRad P((real));
extern long Dvd P((long, long));
extern int Mod12 P((int));
extern real DecToDeg P((real));
extern real DegToDec P((real));
extern real MinDistance P((real, real));
extern real MinDistance3D P((real, real,real,real));
extern real MinDifference P((real, real));
extern real Midpoint P((real, real));
extern char Dignify P((int, int));
extern int DayInMonth P((int, int));
extern int DaysInMonth P((int, int));
extern int DayOfWeek P((int, int, int));
extern int AddDay P((int, int, int, int));
extern real GetOrb P((int, int, int));
extern void Terminate P((int));
extern void PrintSz P((CONST char *));
extern void PrintCh P((char));
extern void PrintSzScreen P((char *));
extern void PrintNotice P((char *));
extern void PrintWarning P((char *));
extern void PrintError P((char *));
extern void ErrorArgc P((char *));
extern void ErrorValN P((char *, int));
extern void ErrorArgv P((char *));
extern void ErrorSwitch P((char *));
extern void ErrorEphem P((char *, long));
extern void PrintTab P((char, int));
extern void AnsiColor P((int));
extern void PrintZodiac P((real));
extern char *SzZodiac P((real));
extern char *SzAltitude P((real));
extern char *SzDegree P((real));
extern char *SzDate P((int, int, int, int));
extern char *SzTime P((int, int,int));
extern char *SzTim P((real));
extern char *SzTims P((real));
extern char *SzZone P((real));
extern char *SzLocation P((real, real));
extern char *SzLocationAlt P((real, real,real));
extern void GetTimeNow P((int *, int *, int *, real *, real));
extern int NFromAltN P((int));
extern char *ProcessProgname P((char *));
extern char *SzPersist P((char *));
extern lpbyte PAllocate P((long, bool, byte *));


/* From io.c */

extern FILE *FileOpen P((char *, int));
extern bool FProcessSwitchFile P((char *, FILE *));
extern bool FOutputData P((void));
extern int NParseSz P((CONST char *, int));
extern real RParseSz P((CONST char *, int));
extern void InputString P((char *, char *));
extern int NInputRange P((char *, int, int, int));
extern real RInputRange P((char *, real, real, int));
extern bool FInputData P((char *));
extern bool FileFind P((char *));
extern char *szFileNameSearch;
extern char szFilePathWin[cchSzMax];

/* From calc.c */

#define RBiorhythm(day, rate) (RSin(((day)/(rate))*rPi2)*100.0)

extern int HousePlaceIn P((real));
extern void ComputeInHouses P((void));
extern void HouseAlcabitius P((void));
extern void HouseEqualMidheaven P((void));
extern void HousePorphyryNeo P((void));
extern void HouseWhole P((void));
extern void HouseNull P((void));
extern void ComputeHouses P((int));
extern void ComputeStars P((real));
extern real Decan P((real));
extern real Navamsa P((real));
extern void SphToRec P((real, real, real, real *, real *, real *));
extern void ComputePlacalc P((real));
extern real CastChart P((bool));
extern void CastSectors P((void));
extern bool FEnsureGrid P((void));
extern bool FAcceptAspect P((int, int, int));
extern void GetAspect P((real *, real *, real *, real *, int, int));
extern void GetAspect3D P((real *, real *, real *, real *, real *,real *,real *,real *,int, int));
extern void GetParallel P((real *, real *, real *, real *,real *, real *, real *, real *, int, int));
extern bool FCreateGrid P((bool));
extern bool FCreateGridRelation P((bool));
extern void CreateElemTable P((ET *));

/* Added by V.A. */

extern real ObjPrime[oNorm+1];
extern real StarPrime[cStar+1];
extern int PrimeRest;
extern real PrimeOrb1;
extern real PrimeOrb2;
extern real StarOrb;
extern real CoeffComp;
extern real CoeffPar;
extern int StarAspects;
extern int hRevers;
extern byte StarRest;
extern byte PolarMCflip;  /* V.A. end    */
extern real SolidAspect;
extern real nut;

/* From matrix.c */

#define EclToEqu(Z, L) CoorXform(Z, L, RFromD(us.nEpsilon))
#define EquToEcl(Z, L) CoorXform(Z, L, RFromD(-us.nEpsilon))
#define EquToLocal(Z, L, T) CoorXform(Z, L, T)
#define JulianDayFromTime(t) ((t)*36525.0+2415020.0)
#define IoeFromObj(obj) \
  (obj < oMoo ? 0 : (obj <= cPlanet ? obj-2 : obj-HypuranLo+cPlanet-1))
#define Tropical(deg) (deg - is.rSid + us.rZodiacOffset)

extern real MC, Asc, RA, OB;

extern long MdyToJulian P((int, int, int));
extern real MdytszToJulian P((int, int, int, real, real, real));
extern real MdydszToJulian P((int, int, int, real, real, real));
extern void JulianToMdy P((real, int *, int *, int *));
extern real ProcessInput P((bool));
extern void PolToRec P((real, real, real *, real *));
extern void RecToPol P((real, real, real *, real *));
extern real RecToSph P((real, real, real));
extern void RecToSph_sp P((real, real, real,real,real,real,real *,real *,real *,real *,real *,real *));
extern void CoorXform P((real *, real *, real));
extern void ComputeVariables P((real *));
extern real CuspMidheaven P((void));
extern real CuspAscendant P((void));
extern real CuspEastPoint P((void));
extern real CuspPlacidus P((real, real, bool));
extern void HousePlacidus P((void));
extern void HouseKoch P((void));
extern void HouseEqual P((void));
extern void HouseCampanus P((void));
extern void HouseMeridian P((void));
extern void HouseRegiomontanus P((void));
extern void HousePorphyry P((void));
extern void HouseMorinus P((void));
extern real CuspTopocentric P((real));
extern void HouseTopocentric P((void));
extern real ReadThree P((real, real, real));
extern void RecToSph2 P((real, real, real, real *, real *, real *));
extern void ErrorCorrect P((int, real *, real *, real *));
extern void ProcessPlanet P((int, real));
extern void ComputePlanets P((void));
extern void ComputeLunar P((real *, real *, real *, real *));


#ifdef PLACALC
/* From swe_call.c */

extern bool FPlacalcPlanet
    P((int, double, int, real *, real *, real *, real *, real *,real *));
extern void CoorXForm_sp
    P(( double *, double *, double *, double *, real *,real *,double));
extern void SphToRec_sp
    P(( double, double, double, double, double, double, double *, double *, double *, double *, double *,double *));
extern double julday P((int, int, int, double, int));
extern void revjul P((double, int, int *, int *, int *, double *));
extern bool FPlacalcStar P((int, double, real *, real *,real *,real *));
extern bool FPlacalcHouses P((real,bool));
extern double FPlacalcRefrac P((int,real,int));
extern real FPlacalcEclipseWhen P((int ,real,long,bool));
extern real FPlacalcEclipseWhenList P((int *,real,long,bool));
extern bool FPlacalcNode
    P((int, double, int, real *, real *, real *, real *, real *,real *));
extern int AlToSweObj P((int));
extern long GetFlag P((int));
extern int NextAsteroid P((int));
extern void PrevAsteroid P((void));
extern void SetAstName P((int));
extern void SetSweAstName P((int,int));
extern void sx_get_planet_name P((int, char *));
#ifdef ASTEROIDS
extern void SetObjNumbers P((int,int));
extern void SetObjNames P((int));
#endif
extern void SetNodeNames P((int));
extern void init_ephe_path P((void));
extern double deltaT;
extern int fdeltaT;
#endif


/* From charts0.c */

extern void PrintW P((char *, int));
extern void DisplayCredits P((void));
extern void PrintS P((char *));
extern void DisplaySwitches P((void));
extern void DisplaySwitchesRare P((void));
extern void PrintObjects P((void));
extern void PrintAspects P((void));
extern void PrintSigns P((void));
#ifdef CONSTEL
extern char *GetSzGenitive P((char *, char *));
extern void GetSzConstel P((char *, int));
extern void PrintConstellations P((void));
#endif
extern void PrintOrbit P((void));
extern int NCompareSzPart P((int, int));
extern void DisplayArabic P((void));
#ifdef GRAPH
extern void DisplayKeysX P((void));
extern void DisplaySwitchesX P((void));
#ifdef WIN
extern void DisplaySwitchesW P((void));
#endif
#endif /* GRAPH */
extern bool FPrintTables P((void));
extern real PartOrb;          /*  Added by V.A.  */
extern int PartAspects;       /*  Added by V.A.  */
extern int PartAspectsShow;   /*  Added by V.A.  */


/* From charts1.c */

extern void PrintHeader P((void));
extern void ChartListing P((void));
extern void ChartGrid P((void));
extern void PrintGrand P((char, int, int, int, int));
extern void DisplayGrands P((void));
extern void PrintHouse P((int, int));
extern void PrintWheelCenter P((int));
extern void PrintWheelSlot P((int));
extern void ChartWheel P((void));
extern void PrintAspectSummary P((int *, int *, int, real));
extern void ChartAspect P((void));
extern void PrintMidpointSummary P((int *, int, long));
extern void ChartMidpoint P((void));
extern void ChartHorizon P((void));
extern void ChartOrbit P((void));
extern void ChartSector P((void));
extern void ChartAstroGraph P((void));
extern void PrintChart P((bool));


/* From charts2.c */

extern void ChartGridRelation P((void));
extern void ChartAspectRelation P((void));
extern void ChartMidpointRelation P((void));
extern void CastRelation P((void));
extern void PrintInDay P((int, int, int));
extern void PrintAspect P((int, int, int, int, int, int, int, char));
extern void PrintAspect3D P((int, int, int, double, int, int, int, int, char));
extern void ChartInDayInfluence P((void));
extern void ChartTransitInfluence P((bool,bool));
extern void EclToHorizon P((real *, real *, real, real, real, real, real));
extern void ChartCalendarMonth P((void));
extern void ChartCalendarYear P((void));
extern void DisplayRelation P((void));
extern real ppower2[objMax];


/* From charts3.c */

extern void ChartInDaySearch P((bool));
extern void ChartTransitSearch P((bool));
extern void ChartInDayHorizon P((void));
extern void ChartEphemeris P((void));


/* From charts4.c */ /* Added by N.S. */
extern void PrintVerboseHeader P((void));
extern void ChartEclipse P((void));
extern void ChartEclipseSet P((void));
extern void ChartReturnSet P((void));
extern void ChartSynodicReturnSet P((void));
extern void ChartPrimaryDirectionSet P((void));
extern void ChartReturn P((void));
extern void ChartNodeListing P((void));
extern void ChartInDaySearch3D P((bool));
extern void ChartTransitSearch3D P((bool));
extern void sumChartTransitInfluence P((bool,bool));
extern int MinimumRank P((real *,int));
extern void SortRevRank P((real *, int *, int));
extern void SortRevRank2 P((real *, int *, int));
extern byte *AtlasRequest P((char *, double *, double *,double *));
extern double ArcToProg P((real , real ));
extern double ProgToArc P((real , real ));
extern void SetBirthRate P((void));

/* From gtopo.c */ /* Added by N.S. */
extern void SetAlt P((real,real,real *));
extern void ConvertPgm P((char *,char *));

/* From riyal.c */ /* Added by N.S. */
extern void Everett P((int,double,int *,double * ,double *));
extern void FRiyalUnnumbered P((double,int));
extern int FRiyalNumbered P((double,int,int));
extern void PrintSolex P((void));
extern void ConvertSolex P((char *,char *,int,int,bool));
/* From riyal2.c */ /* Added by N.S. */
extern int swe_sx_nod_aps P((double,int,int,int,double *,double *,double *,double *,char *));
/* From intrpret.c */

extern int RulerPlanet P((int));             /* added by V.A. */
extern void InterpretSabian P((void));  /* added by N.S. */

#ifdef INTERPRET
extern void FieldWord P((char *));
extern void InterpretGeneral P((void));
extern void InterpretAspectGeneral P((void));
extern void InterpretLocation P((void));
extern void InterpretAspect P((int, int));
extern void InterpretGrid P((void));
extern void InterpretMidpoint P((int, int));
extern void InterpretInDay P((int, int, int));
extern void InterpretTransit P((int, int, int));
extern void InterpretSynastry P((void));
extern void InterpretAspectRelation P((int, int));
extern void InterpretGridRelation P((void));
extern void InterpretMidpointRelation P((int, int));
#endif
extern void SortRank P((real *, int *, int));
extern void SortRank2 P((real *, int *, int));
extern void ChartInfluence P((void));
extern void PlanetPPower P((void));   /*  Added by V.A.  */
extern real ppower1[objMax];         /*  Added by V.A.  */
extern real ppowerTotal[objMax];     /*  Added by V.A.  */


#ifdef GRAPH
/* From xdata.c */

extern GS NPTR gs;
extern GI NPTR gi;

#ifdef X11
extern XSizeHints hint;
extern char xkey[];
extern CONST char *szColorX[cColor];
extern KV rgbind[cColor];
extern KV fg, bg;
#endif
#ifdef WIN
extern int ikPalette[cColor];
#endif

extern CONST KV rgb[cColor], rgbbmp[cColor];
extern KI kMainB[9], kRainbowB[8], kElemB[4], kAspB[cAspect+1], kObjB[objMax];
extern char szObjectFont[oNorm+2];
extern CONST char szAspectFont[cAspect+1],
    *szDrawSign[cSign+2], *szDrawSign2[cSign+2], *szDrawObject[oNorm+5],
    *szDrawObject2[oNorm+5], *szDrawHouse[cSign+1], *szDrawHouse2[cSign+1],
    *szDrawAspect[cAspect+3], *szDrawAspect2[cAspect+3], *szDrawCh[255-32+1],
    *szWorldData[62*3], *szDrawConstel[cCnstl+1];


/* From xgeneral.c */

#define BBmGet(B, X, Y) ((B)[(long)(Y)*(long)(gi.cbBmpRow) + ((X) >> 1)])
#define FBmGet(B, X, Y) ((BBmGet(B, X, Y) >> ((((X)&1)^1) << 2)) & 15)
#define BmSet(B, X, Y, O) BBmGet(B, X, Y) = ((BBmGet(B, X, Y) & \
  (15 << (((X)&1) << 2))) | ((O) << ((((X)&1)^1) << 2)))

#define PutByte(A) putc((byte) (A), file)
#define PutWord(A) PutByte(BLo(A)); PutByte(BHi(A))
#define PutLong(A) PutWord(WLo(A)); PutWord(WHi(A))

#define DrawEdge(X1, Y1, X2, Y2) \
  DrawBox(X1, Y1, X2, Y2, gi.nScaleT, gi.nScaleT)
#define DrawEdgeAll() DrawEdge(0, 0, gs.xWin-1, gs.yWin-1)
#define DrawLine(X1, Y1, X2, Y2) DrawDash(X1, Y1, X2, Y2, 0)
#define DrawCircle(X, Y, RX, RY) \
  DrawEllipse((X)-(RX), (Y)-(RY), (X)+(RX), (Y)+(RY))

extern void DrawColor P((KI));
extern void DrawPoint P((int, int));
extern void DrawSpot P((int, int));
extern void DrawBlock P((int, int, int, int));
extern void DrawBox P((int, int, int, int, int, int));
extern void DrawClearScreen P((void));
extern void DrawDash P((int, int, int, int, int));
extern void DrawWrap P((int, int, int, int, int, int));
extern void ClipLesser P((int *, int *, int *, int *, int));
extern void ClipGreater P((int *, int *, int *, int *, int));
extern void DrawClip P((int, int, int, int, int, int, int, int, int));
extern void DrawEllipse P((int, int, int, int));
extern void DrawSz P((CONST char *, int, int, int));
extern void DrawSign P((int, int, int));
extern void DrawHouse P((int, int, int));
extern void DrawObject P((int, int, int));
extern void DrawAspect P((int, int, int));
extern int NFromPch P((CONST char **));
extern void DrawTurtle P((CONST char *, int, int));


/* From xdevice.c */

#ifdef META
/* Macros to output the various metafile commands used. */

#define MetaRecord(S, R) MetaLong((long)(S)); MetaWord(R)
#define MetaSelectObject(O) MetaRecord(4, 0x12D); MetaWord(O)
#define MetaDeleteObject(O) MetaRecord(4, 0x1F0); MetaWord(O)
#define MetaSaveDc() MetaRecord(3, 0x01E)
#define MetaRestoreDc() MetaRecord(4, 0x127); MetaWord((word)-1)
#define MetaWindowOrg(X, Y) MetaRecord(5, 0x20B); MetaWord(Y); MetaWord(X)
#define MetaWindowExt(X, Y) MetaRecord(5, 0x20C); MetaWord(Y); MetaWord(X)
#define MetaCreatePen(S, W, C) MetaRecord(8, 0x2FA); MetaWord(S); \
  MetaWord(W); MetaWord(W); MetaLong(C)
#define MetaCreateBrush(S, C) MetaRecord(7, 0x2FC); \
  MetaWord(S); MetaLong(C); MetaWord(0 /* Not used */);
#define MetaCreateFont(S, X, Y, C) MetaRecord(12+(S), 0x2FB); MetaWord(Y); \
  MetaWord(X); MetaWord(0 /* Angle */); MetaWord(0 /* Not used */); \
  MetaWord(400 /* Normal Weight */); MetaWord(0 /* Italic, Underline */); \
  MetaWord(WFromBB(0 /* Strikeout */, C)); \
  MetaWord(WFromBB(4 /* TrueType */, 0 /* Clip */))
#define MetaBkMode(M) MetaRecord(4, 0x102); MetaWord(M)
#define MetaTextAlign(A) MetaRecord(4, 0x12E); MetaWord(A)
#define MetaTextColor(C) MetaRecord(5, 0x209); MetaLong(C);
#define MetaTextOut(X, Y, S) MetaRecord(7+((S)+1)/2, 0xA32); \
  MetaWord(Y); MetaWord(X); MetaWord(S); MetaWord(0 /* ETO */)
#define MetaRectangle(X1, Y1, X2, Y2) MetaRecord(7, 0x41B); \
  MetaWord(Y2); MetaWord(X2); MetaWord(Y1); MetaWord(X1)
#define MetaEllipse(X1, Y1, X2, Y2) MetaRecord(7, 0x418); \
  MetaWord(Y2); MetaWord(X2); MetaWord(Y1); MetaWord(X1)
#define MetaEscape(S) MetaRecord((S), 0x626); \
  MetaWord(15 /* MFCOMMENT */); MetaWord(((S)-5)*2 /* Bytes in comment */);
#endif /* META */

extern void WriteXBitmap P((FILE *, char *, char));
extern void WriteAscii P((FILE *));
extern void WriteBmp P((FILE *));
extern void BeginFileX P((void));
extern void EndFileX P((void));
extern void PsStrokeForce P((void));
extern void PsStroke P((int));
extern void PsLineCap P((bool));
extern void PsDash P((int));
extern void PsLineWidth P((int));
extern void PsFont P((int));
extern void PsBegin P((void));
extern void PsEnd P((void));
extern void MetaWord P((word));
extern void MetaLong P((long));
extern void MetaSelect P((void));
extern void MetaInit P((void));
extern void WriteMeta P((FILE *));
extern int MouseInit P((int, int));
extern void MouseShow P((bool));
extern bool MouseStatus P((int *, int *, int *));


/* From xcharts0.c */

extern int DrawPrint P((char *, int, int));
extern void DrawInfo P((void));
extern void DrawWheel
    P((real *, real *, int, int, real, real, real, real,real,real,real,real));
extern void DrawSymbolRing
    P((real *, real *, real *, int, int, real, real, real,real,real,real));
extern bool FReadWorldData P((char FPTR **, char FPTR **, char FPTR **));
extern bool FGlobeCalc P((real, real, int *, int *, int, int, int, int, int));
extern void DrawLeyLine P((real, real, real, real));
extern void DrawLeyLines P((int));
extern void DrawMap P((bool, bool, int));
extern void DrawChartX P((void));


/* From xcharts1.c */

extern void XChartWheel P((void));
extern void XChartAstroGraph P((void));
extern void XChartGrid P((void));
extern void XChartHorizon P((void));
extern void XChartHorizonSky P((void));
extern void XChartOrbit P((void));
extern void XChartSector P((void));
extern void DrawArrow P((int, int, int, int));
extern void XChartDispositor P((void));
extern void XChartCalendar P((void));
extern real CoeffTens;                       /* added by V.A. */
extern int TensChart;                        /* added by V.A. */
extern int InfoFlag;
extern real STension;
extern real SHarmon;
extern real SConjunct;
extern real SSumma;
extern real SChart;




/* From xcharts2.c */

extern bool FProper P((int));
extern void FillSymbolRing P((real *, real));
extern void FillSymbolLine P((real *));
extern real PlaceInX P((real));
extern real HousePlaceInX P((real));
extern void XChartWheelRelation P((void));
extern void XChartWheelThreeFour P((void));
extern void XChartGridRelation P((void));
extern void XChartEphemeris P((void));
extern void XChartBiorhythm P((void));


/* From xcharts3.c */
extern void XChartEphemeris3D P((void));

/* From xscreen.c */

extern void InitColorsX P((void));
#ifdef ISG
extern void BeginX P((void));
extern void AddTime P((int, int));
extern void Animate P((int, int));
extern void CommandLineX P((void));
extern void SquareX P((int *, int *, int));
extern void InteractX P((void));
extern void EndX P((void));
#endif
extern int NProcessSwitchesX P((int, char **, int, bool, bool, bool));
extern int NProcessSwitchesRareX P((int, char **, int));
extern bool FActionX P((void));
#endif /* GRAPH */


#ifdef WIN
/* From wdriver.c & wdialog.c */

extern WI NPTR wi;
extern OPENFILENAME ofn;
extern PRINTDLG prd;
extern char szFileName[cchSzMaxFile], szFileTitle[cchSzMaxFile], *szFileTemp;

#define TextClearScreen() PatBlt(wi.hdc, 0, 0, wi.xClient, wi.yClient, \
  us.fAnsiColor || !gs.fInverse ? BLACKNESS : WHITENESS);
#define CheckMenu(cmd, f) \
  CheckMenuItem(wi.hmenu, (_int)cmd, f ? MF_CHECKED : MF_UNCHECKED);
#define WiCheckMenu(cmd, f) CheckMenu(cmd, f); wi.fMenu = fTrue
#define WiDoDialog(pfn, dlg) \
  dlgproc = (DLGPROC)MakeProcInstance(pfn, wi.hinst); \
  DialogBox(wi.hinst, MAKEINTRESOURCE(dlg), wi.hwnd, dlgproc); \
  FreeProcInstance((FARPROC)dlgproc)

#define SetCheck(id, f) CheckDlgButton(hdlg, id, f)
#define SetRadio(id, idLo, idHi) CheckRadioButton(hdlg, idLo, idHi, id)
#define SetEdit(id, sz) SetDlgItemText(hdlg, id, (LPCSTR)sz)
#define SetEditN(id, n) SetDlgItemInt(hdlg, id, n, fTrue)
#define SetCombo(id, sz) \
  SendDlgItemMessage(hdlg, id, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)sz)
#define ClearCombo(id) SendDlgItemMessage(hdlg, id, CB_RESETCONTENT, 0, 0);
#define GetCheck(id) IsDlgButtonChecked(hdlg, id)
#define GetEdit(id, sz) GetDlgItemText(hdlg, id, sz, cchSzDef)
#define GetEditN(id) GetDlgItemInt(hdlg, id, NULL, fTrue)
#define EnsureN(n, f, sz) if (!(f)) { ErrorEnsure(n, sz); return fTrue; }
#define EnsureR(r, f, sz) EnsureN((int)r, f, sz)

extern LONG API WndProc P((HWND, WORD, WORD, LONG));
extern int NProcessSwitchesW P((int, char **, int, bool, bool, bool));
extern void ResizeWindowToChart P((void));
extern WORD WCmdFromRc P((int));
extern void SetRel P((int));
extern void ProcessState P((void));
extern int NWmCommand P((WORD));
extern void API RedoMenu P((void));
extern bool API FRedraw P((void));

extern void SetEditSz(HWND, int, char *);
extern void SetEditR(HWND, int, real, int);
extern void SetEditMDYT(HWND, int, int, int, int, int, int, int, real);
extern void SetEditSZOA(HWND, int, int, int, int, real, real, real, real);
extern void SetEditColor(HWND, int, KI);
extern real GetEditR(HWND, int);
extern void ErrorEnsure(int, char *);
extern bool FOutputSettings P((void));
extern bool API DlgOpenChart P((void));
extern bool API DlgSaveChart P((void));
extern bool API DlgPrint P((void));
extern bool API DlgAbortProc P((HDC, int));
extern bool API DlgAbort P((HWND, _int, WORD, LONG));
extern bool API DlgCommand P((HWND, _int, WORD, LONG));
extern bool API DlgColor P((HWND, _int, WORD, LONG));
extern bool API DlgInfo P((HWND, _int, WORD, LONG));
extern bool API DlgDefault P((HWND, _int, WORD, LONG));
extern bool API DlgInfoAll P((HWND, _int, WORD, LONG));
extern bool API DlgAspect P((HWND, _int, WORD, LONG));
extern bool API DlgObject P((HWND, _int, WORD, LONG));
extern bool API DlgObject2 P((HWND, _int, WORD, LONG));
extern bool API DlgRestrict P((HWND, _int, WORD, LONG));
extern bool API DlgStar P((HWND, _int, WORD, LONG));
extern bool API DlgSetting P((HWND, _int, WORD, LONG));
extern bool API DlgObscure P((HWND, _int, WORD, LONG));
extern bool API DlgTransit P((HWND, _int, WORD, LONG));
extern bool API DlgProgress P((HWND, _int, WORD, LONG));
extern bool API DlgChart P((HWND, _int, WORD, LONG));
extern bool API DlgGraphics P((HWND, _int, WORD, LONG));
extern bool API DlgAbout P((HWND, _int, WORD, LONG));
#endif /* WIN */

#ifdef INTERPRETALT
extern void NatalInterpret P((void));
extern void TransitInterpret P((void));
extern char InterpretAlt;
extern char NoPrint;
#endif
#ifdef KDE
extern void KColor P((KI col));
extern void KPoint P((int x, int y));
extern void KLine P((int x, int y, int x1, int y1));
extern void KDash P((int x, int y, int x1, int y1, int skip));
extern void KBlock P((int x, int y, int x1, int y1));
extern void KEllipse P((int x, int y, int w, int h));
extern void KText P((int fact, const char *s, int x, int y));
extern void KNumber P((int i, int x, int y));
extern void KDrawSign P((int i, int x, int y));
extern _bool KObj P((int i, int x, int y));
extern void KInteract P((void));
extern char getkey P((void));
extern void kmain P((int argc, char **argv));
extern void  gettz P((CI*));
extern char *CheckColor P((int i));
extern void KCheckDst P((void));
extern void KGetTZ P((real*));
extern char *notime, *compochart, *shouse, *sidereal, *tropical, *heliocentric;
extern char *geocentric, *julianday, *dshouse, *fireearth, *airwater, *carfixmut;
extern char *yangyin, *angsuccad, *learnshare;
extern bool FOutputSettings P((void));
#endif /* KDE */

extern char NoPrintHeader;
extern char fNESW;
extern int fEclipseBackward;
extern int fEclipseDetails;
extern int fOccultAll;
extern int fReturnChart;
extern int fSynodicReturnChart;
extern int fReturnChartCast;
extern int fProgressRatio;
extern int fRestrictClean;
extern int fUseJPL;
extern int fUseJ2000;
extern int fUseICRS;
extern int fWholeSignAspects;
extern int fGauqMode;
extern int fSecMotion;
extern int fSolarArcKey;
extern int fBirthRate;
extern int fAlsoSouthNode;
extern int fSkipHard;
extern double rSidDay;
extern double rBirthRate;
extern double rConverseBirthRate;
extern double returnnatalpos;
extern double rAddAngle;

/* extern.h */
