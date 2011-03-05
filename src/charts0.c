/*
** Astrolog (Version 5.42J) File: charts0.c
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
** Table Display Routines.
******************************************************************************
*/

/* A subprocedure of the credit displayed below, this prints out one line */
/* of credit information on the screen. Given a string, it's displayed    */
/* centered with left and right borders around it, in the given color.    */

void PrintW(sz, col)
char *sz;
int col;
{
    int i;

    if (!sz) {

        /* Null string means print the top, bottom, or a separator row. */

        if (col < 0)
            AnsiColor(kRed);
        PrintCh((char)(col ? (col > 0 ? chSW : chNW) : chJE));
        PrintTab(chH, CREDITWIDTH);
        PrintCh((char)(col ? (col > 0 ? chSE : chNE) : chJW));
    } else {
        i = CchSz(sz);
        PrintCh(chV);
        PrintTab(' ', (CREDITWIDTH-i)/2 + (i&1));
        AnsiColor(col);
        PrintSz(sz);
        PrintTab(' ', (CREDITWIDTH-i)/2);
        AnsiColor(kRed);
        PrintCh(chV);
    }
    PrintL();
}


/* Display a list of credits showing those who helped create the various  */
/* parts of Astrolog, as well as important copyright and version info, as */
/* displayed with the -Hc switch.                                         */

void DisplayCredits()
{
    char sz[cchSzDef];

    PrintW(NULL, -1);
    sprintf(sz, "** %s version %s **", szAppName, szVersionCore);
    PrintW(sz, kWhite);
    sprintf(sz, "As of %s. By Walter D. Pullen", szDateCore);
    PrintW(sz, kLtGray);
    sprintf(sz, "(%s)", szAddressCore);
    PrintW(sz, kCyan);
    PrintW(NULL, 0);
    PrintW("Main planetary calculation formulas were converted from", kGreen);
    PrintW(
        "routines by James Neely, as listed in 'Manual of Computer Programming",
        kGreen);
    PrintW(
        "for Astrologers' by Michael Erlewine, available from Matrix Software.",
        kGreen);
    PrintW("PostScript graphics routines by Brian D. Willoughby.", kYellow);
    PrintW(
        "Extended ephemeris calculation and formulas are by Astrodienst AG,",
        kMagenta);
    PrintW(
        "library SWISS EPHEMERIS, available at www.astro.com/swisseph.", kMagenta);
    PrintW(
        "Modifications in version 5.42x were done by Nicolas Scharnagl", kDkGreen);
    sprintf(sz, "(%s)", szAddressCoren);
    PrintW(sz, kDkGreen);
    PrintW(
        "IMPORTANT: Astrolog is 'freeware', but is copyrighted and not in public",
        kLtGray);
    PrintW(
        "domain. Permission is granted to freely use and distribute these",
        kLtGray);
    PrintW(
        "routines provided one does not sell, restrict, or profit from the",
        kLtGray);
    PrintW(
        "program or its output in any way. Modification is allowed provided",
        kLtGray);
    PrintW(
        "these exact notices remain with any altered or edited versions of the",
        kLtGray);
    PrintW(
        "program. These conditions are true of both the program in whole and of",
        kLtGray);
    PrintW(
        "all parts by any individual author. Violators are subject to copyright",
        kLtGray);
    PrintW(
        "law penalties, and negative karmic debts to aforementioned contributors.",
        kLtGray);
    PrintW(NULL, 0);
    PrintW(
        "Special thanks to all those unmentioned, seen and unseen, who have",
        kBlue);
    PrintW(
        "pointed out problems, suggested features, & sent many positive vibes! :)",
        kBlue);
    PrintW(NULL, 1);
    AnsiColor(kDefault);
}


/* Print out a command switch or keypress info line to the screen, as done  */
/* with the -H switch or 'H' key in a graphic window. This is just printing */
/* out the string, except in Ansi mode we set the proper colors: Red for    */
/* header lines, Green for individual switches or keys, and White for the   */
/* rest of the line telling what it does. We also prefix each switch with   */
/* either Unix's '-' or PC's '/', whichever is appropriate for the system.  */

void PrintS(sz)
char *sz;
{
    int chDash;
    char ch;

    chDash = sz[1];
    if (*sz != ' ')
        AnsiColor(kRed);
    else if (chDash != ' ')
        AnsiColor(chDash == 'P' || sz[3] == ' ' || sz[3] == ':' ?
                  kGreen : kDkGreen);
    else
        AnsiColor(kDefault);
    while ((ch = *sz) && ch != ':' &&
            (chDash != 'P' || (ch != ' ' || *(sz+1) != 't'))) {
        if (ch != '_')
            PrintCh(ch);
        else
            PrintCh(chSwitch);
        sz++;
    }
    if (*sz)
        PrintCh(*sz++);
    AnsiColor(kDefault);
    while ((ch = *sz)) {
        if (ch != '_')
            PrintCh(ch);
        else
            PrintCh(chSwitch);
        sz++;
    }
    PrintL();
}


/* Print a list of every command switch that can be passed to the program, */
/* and a description of what it does. This is what the -H switch prints.   */

void DisplaySwitches()
{
    char sz[cchSzDef];

    sprintf(sz, "%s (version %s) command switches:", szAppName, szVersionCore);
    PrintS(sz);
    PrintS(" _H: Display this help list.");
    PrintS(" _Hc: Display program credits and copyrights.");
    PrintS(" _HC: Display names of zodiac signs and houses.");
    PrintS(" _HO: Display available planets and other celestial objects.");
    PrintS(" _HA: Display available aspects, their angles, and present orbs.");
#ifdef CONSTEL
    PrintS(" _HF: Display names of astronomical constellations.");
#endif
    PrintS(" _HS: Display information about planets in the solar system.");
#ifdef INTERPRET
    PrintS(" _HI: Display meanings of signs, houses, planets, and aspects.");
#endif
    sprintf(sz,
            " _He: Display all info tables together (_Hc_H_Y_HX_HC_HO_HA%s_HS%s).",
#ifdef CONSTEL
            "_HF",
#else
            "",
#endif
#ifdef INTERPRET
            "_HI");
#else
            ""
           );
#endif
    PrintS(sz);
    PrintS(" _Q: Prompt for more command switches after display finished.");
#ifdef SWITCHES
    PrintS(" _Q0: Like _Q but prompt for additional switches on startup.");
#endif
    PrintS(" _M <1-48>: Run the specified command switch macro.");
    PrintS(" _M0 <1-48> <string>: Define the specified command switch macro.");
    PrintS(" _Y: Display help list of less commonly used command switches.");
    PrintS("\nSwitches which determine the type of chart to display:");
    PrintS(" _v: Display list of object positions (chosen by default).");
    PrintS(" _v0: Express velocities relative to average speed.(only without swisseph). Also supresses sidebar in graphics display");
    PrintS(" _w [<rows>]: Display chart in a graphic house wheel format.");
    PrintS(" _w0 [..]: Like _w but reverse order of objects in houses 4..9.");
    PrintS(" _g: Display aspect and midpoint grid among planets.");
    PrintS(" _g0: Like _g but flag aspect configurations (e.g. Yod's) too.");
    PrintS(" _g0: For comparison charts, show midpoints instead of aspects.");
    PrintS(" _ga: Like _g but indicate applying instead of difference orbs.");
    PrintS(" _gp: Like _g but generate parallel and contraparallel aspects.");
    PrintS(" _a: Display list of all aspects ordered by influence.");
    PrintS(" _a0: Like _a but display aspect summary too.");
    PrintS(" _a[0]a: Like _a but indicate applying and separating orbs.");
    PrintS(" _a[0]p: Like _a but do parallel and contraparallel aspects.");
    PrintS(" _m: Display all object midpoints in sorted zodiac order.");
    PrintS(" _m0: Like _m but display midpoint summary too.");
    PrintS(" _ma: Like _m but show aspects from midpoints to planets as well.");
    PrintS(" _Z: Display planet locations with respect to the local horizon.");
    PrintS(" _Z0: Like _Z but express coordinates relative to polar center.");
    PrintS(" _Zd: Search day for object local rising and setting times.");
    PrintS(" _Zdp: Search day for object pairs rising,setting and culm. simultaneously.");
#ifdef PLACALC
    PrintS(" _ZE[sl]: Search for next solar/lunar eclipses");
    PrintS(" _ZE[o] object: Search for next lunar occultation with object");
    PrintS(" _Ze[sl]: Search for next solar/lunar eclipses at current location,shows details");
    PrintS(" _Ze[o] object: Search for next lunar occultation with object at current location");
    PrintS(" _Z[eE]O object: Search for next lunar occultation with all objects unrestricted");
    PrintS(" _Zc[sl]: Show chart of next solar/lunar eclipse");
    PrintS(" _Zc[o] object: Search chart of next lunar occulatation with object");
    PrintS(" _[ZEb][Zcb]: Search backwards for eclipses/occultations,e.g. for prenatal eclipse");
    PrintS(" _ZEn number: Set number of eclipses to show");
#endif
    PrintS(" _S: Display x,y,z coordinate positions of planets in space.");
    PrintS(" _l: Display Gauquelin sectors for each planet in chart.");
    PrintS(" _l0: Like _l but approximate sectors using Placidus cusps.");
    PrintS(" _j: Display astrological influences of each object in chart.");
    PrintS(" _j0: Like _j but include influences of each zodiac sign as well.");
    PrintS(" _L [<step>]: Display astro-graph locations of planetary angles.");
    PrintS(" _L0 [..]: Like _L but display list of latitude crossings too.");
    PrintS(" _K: Display a calendar for given month.");
    PrintS(" _Ky: Like _K but display a calendar for the entire year.");
    PrintS(" _d [<step>]: Print all aspects and changes occurring in a day.");
    PrintS(" _dm: Like _d but print all aspects for the entire month.");
    PrintS(" _dy: Like _d but print all aspects for the entire year.");
    PrintS(" _dY <years>: Like _d but search within a number of years.");
    PrintS(" _dp[012] <month> <year>: Print aspects within progressed/directed chart.");
    PrintS(" _dp[012]y <year>: Like _dp but search for aspects within entire year.");
    PrintS(" _dp[012]Y <year> <years>: Like _dp but search within number of years.");
    PrintS(" _dp[012][y]n: Search for progressed aspects in current month/year.");
    PrintS(" _D: Like _d but display aspects by influence instead of time.");
    PrintS(" _E: Display planetary ephemeris for given month.");
    PrintS(" _Ey: Display planetary ephemeris for the entire year.");
    PrintS(" _EY <years>: Display planetary ephemeris for a number of years.");
    PrintS(
        " _e: Print all charts together (i.e. _v_w_g0_a_m_Z_S_l_K_j0_L0_d_D_E).");
    PrintS(
        " _t <month> <year>: Compute all transits to natal planets in month.");
    PrintS(
        " _tp <month> <year>: Compute progressions to natal in month for chart.");
    PrintS(" _tr <month> <year>: Compute all returns in month for chart.");
    PrintS(" _t[p[012]]y: <year>: Compute transits/progressions for entire year.");
    PrintS(" _t[p[012]]Y: <year> <years>: Compute transits for a number of years.");
#ifdef TIME
    PrintS(" _t[py]n: Compute transits to natal planets for current time now.");
#endif
    PrintS(" _T <month> <day> <year>: Display transits ordered by influence.");
    PrintS(" _Tt <month> <day> <year> <time>: Display transits ordered by influence.");
    PrintS(" _Ti <file> : Display transits ordered by influence.");
    PrintS(" _Tp <month> <day> <year>: Print progressions instead of transits.");
#ifdef TIME
    PrintS(" _T[p]n: Display transits ordered by influence for current date.");
#endif
#ifdef ARABIC
    PrintS(" _P [<parts>]: Display list of Arabic parts and their positions.");
    PrintS(" _P0 [<parts>]: Like _P but display formulas with terms reversed.");
    PrintS(" _P[z,n,f]: Order parts by position, name, or formula.");
#endif
#ifdef PLACALC
    PrintS(" _N[m,b,h,d,f]: Display -RS restricted list of (mean)(barycentric) nodes,aphelions,perihels,second elliptic focal points positions");
    PrintS(" _Nn[m,b] object : Load (mean)(barycentric) nodes,aphelions,perihels,second elliptic focal points positions of object in place of Terran objects");
#ifdef ASTEROIDS
    PrintS(" _N[a,y] catalogue : Load Asteroid catalogue in place of Vertex (for graphic chart)");
    PrintS(" _N[N,S,y]: Load extra asteroids starting from 1 (N) or (A)rgument catalogue nr. y=with discovery year for unnamed S=no check for gaps");
    PrintS(" _N[A,y] start #ast : Load #ast extra asteroids starting at catalogue nr. start y=with discovery year for unnamed S");
    PrintS(" _N[U,y]: Load extra asteroids as given catalogue nr. as arguments y=with discovery year for unnamed S,negative for comets");
    PrintS(" _N[L]: Load solex-calculated objects not from swiss ephemeris (Centaurs,SDO,TNO)");
    PrintS(" _ZC <fileinsolex><fileoutsx1> [<step><scale>]: Produce .sx1 files from text heliocentric J2000 cartesian ecliptic vectors,negative scale means no header line for composite riyal-like .fle. With _YTI floating point");
#endif
#endif
    PrintS(" _Ns: Sort by position in chart listing/ Sort by power in Influence Listing");
    PrintS(" _ND degrees: Modulo degrees between -360 and 360 for sorting by position,default=30");
    PrintS(" _Nr from to: Degrees to show between -Modulo and Modulo for sorting by position,default=all");
    PrintS(" _NR object orb: Degrees to show around object +-orb degrees for sorting by position");
#ifdef INTERPRET
    PrintS(" _I [<columns>]: Print interpretation of selected charts.");
    PrintS(" _IS : Show sabian symbols.");
#endif
    PrintS("\nSwitches which affect how the chart parameters are obtained:");
#ifdef TIME
    PrintS(" _n: Compute chart for this exact moment using current time.");
    PrintS(" _n[d,m,y]: Compute chart for start of current day, month, year.");
#endif
    PrintS(" _z [<zone>]: Change the default time zone (for _d_E_t_q options).");
    PrintS(" _z0 [<offset>]: Change the default daylight time setting.");
    PrintS(" _zl <long> <lat>: Change the default longitude & latitude.");
    PrintS(" _zL <locationname> : Change the default longitude & latitude according to atlas file.");
    PrintS(" _zL0 <locationname> <occurence>: as -zL but use nth occurence of locationname, -1 shows list of available locationnames");
    PrintS(" _za <alt> : Change the default altitude (in m or km)");
    PrintS(" _zp <atpress> : Change the default atmospheric pressure (in mbar/hPa)");
    PrintS(" _zT <temp> : Change the default atmospheric temperature (in Celsius)");
    PrintS(" _zt <time>: Set only the time of current chart.");
    PrintS(" _zd <date>: Set only the day of current chart.");
    PrintS(" _zm <month>: Set only the month of current chart.");
    PrintS(" _zy <year>: Set only the year of current chart.");
    PrintS(" _zi <name> <place>: Set name and place strings of current chart.");
#ifdef KDE
    PrintS(" _zf <file>: Set zone info file name.");
#endif
    PrintS(" _q <month> <date> <year> <time>: Compute chart with defaults.");
    PrintS(" _qd <month> <date> <year>: Compute chart for noon on date.");
    PrintS(" _qm <month> <year>: Compute chart for first of month.");
    PrintS(" _qy <year>: Compute chart for first day of year.");
    PrintS(" _qa <month> <date> <year> <time> <zone> <long> <lat>:");
    PrintS("     Compute chart automatically given specified data.");
    PrintS(" _qb <month> <date> <year> <time> <daylight> <zone> <long> <lat>:");
    PrintS("     Like _qa but takes additional parameter for daylight offset.");
    PrintS(" _qc <month> <date> <year> <time> <daylight> <zone> <long> <lat><alt>:");
    PrintS("     Like _qb but takes additional parameter for altitude.");
    PrintS(" _qj <day>: Compute chart for time of specified Julian day.");
    PrintS(" _i <file>: Compute chart based on info in file.");
    PrintS(" _i[2,3,4] <file>: Load chart info into chart slots 2, 3, or 4.");
    PrintS(" _o <file> [..]: Write parameters of current chart to file.");
    PrintS(" _o0 <file> [..]: Like _o but output planet/house positions.");
    PrintS(" _os <file>, > <file>: Redirect output of text charts to file.");
    PrintS("\nSwitches which affect what information is used in a chart:");
    PrintS(" _R [<obj1> [<obj2> ..]]: Restrict specific bodies from displays.");
    PrintS(" _R0 [<obj1> ..]: Like _R but restrict everything first.");
    PrintS(" _R1 [<obj1> ..]: Like _R0 but unrestrict and show all objects.");
    PrintS(" _R[C,F,u,U]: Restrict all cusps,terran points, all main stars, or stars.");
    PrintS(" _R[a,p,m,c,t,h,o]: Restrict all atens,apollons,amors,centaurs,transneptunians,uranians,lunar or other.");
    PrintS(" _R[I,O,L,x]: Restrict all inner pl,outer pl,sun and moon,axes.");
    PrintS(" _RT[0,1,C,u,U,a,p,m,c,t,l,o] [..]: Restrict transiting planets in _t,_T lists.");
    PrintS(" _RS[0,1,C,u,U,a,p,m,c,t,l,o] [..]: Restrict summary planets in _N,_Ts lists.");
    PrintS(" _RA [<asp1> ..]: Restrict aspects by giving them negative orbs.");
    PrintS(" _C: Include angular and non-angular house cusps in charts.");
    PrintS(" _u: Include transneptunian/uranian bodies in charts.");
    PrintS(" _U: Include locations of fixed background stars in charts.");
    PrintS(" _U[z,l,n,b]: Order by azimuth, altitude, name, or brightness.");
    PrintS(" _A <0-18>: Specify the number of aspects to use in charts.");
    PrintS(" _Ao <aspect> <orb>: Specify maximum orb for an aspect.");
    PrintS(" _Am <planet> <orb>: Specify maximum orb allowed to a planet.");
    PrintS(" _Ad <planet> <orb>: Specify orb addition given to a planet.");
    PrintS(" _Aa <aspect> <angle>: Change the actual angle of an aspect.");
    PrintS("\nSwitches which affect how a chart is computed:");
#ifdef PLACALC
    PrintS(" _b: Use ephemeris files for more accurate location computations.");
    PrintS(" _b0: Display to the nearest second.");
    PrintS(" _b1: Display verbose calculation settings.");
#endif
    PrintS(" _c <value>: Select a different default system of houses.");
    PrintS("     (0 = Placidus, 1 = Koch, 2 = Equal, 3 = Campanus,");
    PrintS("     4 = Meridian, 5 = Regiomontanus, 6 = Porphyry, 7 = Morinus,");
    PrintS("     8 = Topocentric, 9 = Alcabitius, 10 = Equal (MC),");
    PrintS("     11 = Neo-Porphyry, 12 = Whole, 13 = Vedic, 14 = None,");
    PrintS("     15 = axial rotation , 16 = azimuthal or horizontal , 17 = Vehlow Equal,");
    PrintS("     18 = Krusinski-Pisa , 19 = Gauquelin sectors.");
    PrintS(" _s [..]: Compute a sidereal instead of the normal tropical chart.");
    PrintS(" _sr: Compute right ascension locations relative to equator.");
    PrintS(" _sg: Compute locations relative to galactic plane.");
    PrintS(" _sm <mode>: Set Swiss Ephemeris sidereal Mode <mode>=0-20,255");
    PrintS(" _st <t0>: Use ecliptic of julian date t0 (together with -sm 255");
    PrintS(" _ss : Solar system plane instead of ecliptic (not together with -sm 255");
    PrintS(" _s[z,h,d]: Display locations as in zodiac, hours/minutes, or degrees.");
    PrintS(" _h [<objnum>]: Compute positions centered on specified object.");
    PrintS(" _p <month> <day> <year>: Cast 2ndary progressed chart for date.");
    PrintS(" _p0 <month> <day> <year>: Cast time arc chart for date.");
    PrintS(" _p1 <month> <day> <year>: Cast solar arc direction chart for date.");
    PrintS(" _p2 <month> <day> <year>: Cast progression of progression chart for date.");
    PrintS(" _p3 <month> <day> <year>: Cast 2ndary progr.ascendant arc direction chart for date.");
    PrintS(" _p4 <month> <day> <year> <time>: Cast 2ndary progressed chart for date/time.");
    PrintS(" _pt : Transits to progressed chart (together with -t).");
#ifdef TIME
    PrintS(" _p[0123]n: Cast progressed chart based on current date now.");
#endif
    PrintS(" _pd <days>: Set no. of days to progress one day (default 365.2422).");
    PrintS(" _pd[ncp]: Set constant defaults ptolemy,naibod,cardan (default is -pdp).");
    PrintS(" _pd[sbv]: Set variable defaults Solar Arc RA,Birthday Solar Arc  RA +-1d,Birth Rate Solar RA (Reset with -pdp).");
    PrintS(" _ps : Switch 2nd motion for primary directions.");
    PrintS(" _pa <+-degrees> : Added Angle for primary directions with -ys switch.(orb,disk,aspect angle)");
    PrintS(" _pm[012345]: SWE Mode for mundane position calculation (0 mundo,1 zodiac,2-5 diurnal arc/w.disk/w.refraction");
    PrintS(" _pm : Switch 2nd motion for primary directions.");
    PrintS(" _x <value>: Cast harmonic chart based on specified factor.");
    PrintS(" _1 [<objnum>]: Cast chart with specified object on Ascendant.");
    PrintS(" _2 [<objnum>]: Cast chart with specified object on Midheaven.");
    PrintS(" _3: Display objects in their zodiac decan positions.");
    PrintS(" _f: Display houses as sign positions (flip them).");
    PrintS(" _G: Compute houses based on geographic location only.");
    PrintS(" _J: Display wheel charts in Vedic format.");
    PrintS(" _9: Display objects in their zodiac navamsa positions.");
    PrintS(" _F <objnum> <sign> <deg>: Force object's position to be value.");
    PrintS(" _+ [<days>]: Cast chart for specified no. of days in the future.");
    PrintS(" _- [<days>]: Cast chart for specified no. of days in the past.");
    PrintS(" _+[m,y] [<value>]: Cast chart for no. of months/years in future.");
    PrintS("\nSwitches for relationship and comparison charts:");
    PrintS(" _r <file1> <file2>: Compute a relationship synastry chart.");
    PrintS(" _rc <file1> <file2>: Compute a composite chart.");
    PrintS(" _rm <file1> <file2>: Compute a time space midpoint chart.");
    PrintS(" _r[c,m]0 <file1> <file2> <ratio1> <ratio2>: Weighted chart.");
    PrintS(" _rd <file1> <file2>: Print time span between files' dates.");
#ifdef BIORHYTHM
    PrintS(" _rb <file1> <file2>: Display biorhythm for file1 at time file2.");
#endif
    PrintS(" _r0 <file1> <file2>: Keep the charts separate in comparison.");
    PrintS(" _rp[0] <file1> <file2>: Like _r0 but do file1 progr. to file2.");
    PrintS(" _rt <file1> <file2>: Like _r0 but treat file2 as transiting.");
#ifdef GRAPH
    PrintS(" _r[3,4]: Make graphics wheel chart tri-wheel or quad-wheel.");
#endif
    PrintS(" _yr <file><object>: Display Return chart of object for file.");
    PrintS(" _ys <file><object1><object2>: Display Synodic Return chart of object1 and object2 for file.");
    PrintS(" _ys <file><promissor><significator> -l: Display Primary Direction chart of promissor and significator for file.");
#ifdef TIME
    PrintS(" _y <file>: Display current/previously house transits for particular chart.");
#ifdef BIORHYTHM
    PrintS(" _y[b,d,p,t] <file>: Like _r0 but compare to current/previously set time now.");
#endif
#endif /* TIME */
    PrintS("\nSwitches to access graphics options:");
    PrintS(" _k: Display text charts using Ansi characters and color.");
    PrintS(" _k0: Like _k but only use special characters, not Ansi color.");
#ifdef MSG
    PrintS(" _V: <25,43,50>: Start up with text mode set to number of rows.");
#endif

    /* If graphics features are compiled in, call an additional procedure to */
    /* display the command switches offered dealing with the graphics stuff. */

#ifdef GRAPH
    DisplaySwitchesX();
#ifdef WIN
    DisplaySwitchesW();  /* Windows version has its own set of switches too. */
#endif
#endif /* GRAPH */
}


/* Print a list of the obscure command switches that can be passed to the */
/* program and a description of them. This is what the -Y switch prints.  */

void DisplaySwitchesRare()
{
    char sz[cchSzDef];

    sprintf(sz, "%s (version %s) obscure command switches:",
            szAppName, szVersionCore);
    PrintS(sz);
    PrintS(" _Y: Display this help list.");
    PrintS(" _Yn: Compute location of true instead of mean node.");
    PrintS(" _Yns: Compute location of lunar south node (instead of mean lunar apogee.");
    PrintS(" _Yd: Display dates in D/M/Y instead of M/D/Y format.");
    PrintS(" _Yt: Display times in 24 hour instead of am/pm format.");
    PrintS(" _YC: Automatically ignore insignificant house cusp aspects.");
    PrintS(" _Y8: Clip text charts at the rightmost (e.g. 80th) column.");
    PrintS(" _YQ <rows>: Pause text scrolling after a page full has printed.");
    PrintS(" _Yo: Output chart info and position files in old style format.");
    PrintS(" _Yc: Angular cusp objects are house positions instead of angles.");
    PrintS(" _Yz <min>: Forward clock by amount for current moment charts.");
    PrintS(" _Yl <1-36>: Toggle plus zone status of sector for sector chart.");
#ifdef ARABIC
    PrintS(" _YP <-1,0,1>: Set how Arabic parts are computed for night charts.");
#endif
    PrintS(" _Yb <days>: Set number of days to span for biorhythm chart.");
    PrintS(" _YE <obj> <semi-major axis> <eccentricity (3)> <inclination (3)>");
    PrintS("     <perihelion (3)> <ascending node (3)> <time offset (3)>");
    PrintS("     Change orbit of object to be the given elements.");
    PrintS(
        " _YR <obj1> <obj2> <flag1>..<flag2>: Set restrictions for object range.");
    PrintS(
        " _YRT <obj1> <obj2> <flag1>..<flag2>: Transit restrictions for range.");
    PrintS(
        " _YR0 <flag1> <flag2>: Set restrictions for sign, direction changes.");
    PrintS(
        " _YRZ <rise> <zenith> <set> <nadir>: Set restrictions for _Zd chart.");
    PrintS(
        " _YZ : Switch between NESW and ENWS azimuth display.");
    PrintS(
        " _Yaw : Switch whole-sign aspect selection");
    PrintS(
        " _YAo <asp1> <asp2> <orb1>..<orb2>: Set aspect orbs for range.");
    PrintS(
        " _YAm <obj1> <obj2> <orb1>..<orb2>: Set max planet orbs for range.");
    PrintS(
        " _YAd <obj1> <obj2> <orb1>..<orb2>: Set planet orb additions for range.");
    PrintS(
        " _YAa <asp1> <asp2> <ang1>..<ang2>: Set planet aspect angles for range.");
    PrintS(
        " _Yj <obj1> <obj2> <inf1>..<inf2>: Set influences for object range.");
    PrintS(
        " _YjC <cusp1> <cusp2> <inf1>..<inf2>: Set influences for house cusps.");
    PrintS(
        " _YjA <asp1> <asp2> <inf1>..<inf2>: Set influences for aspect range.");
    PrintS(
        " _YjT <obj1> <obj2> <inf1>..<inf2>: Set transit influences for range.");
    PrintS(
        " _Yj0 <inf1> <inf2> <inf3> <inf4>: Set influences given to planets");
    PrintS("     in ruling sign, exalted sign, ruling house, exalted house.");
    PrintS(" _YJ <obj> <sign> <cosign>: Set sign planet rules and co-rules.");
    PrintS(" _YJ0 <obj> <sign>: Set zodiac sign given planet exalts in.");
    PrintS(" _YI <obj> <string>: Customize interpretation for object.");
    PrintS(
        " _YIa <sign> <string>: Customize interpretation adjective for sign.");
    PrintS(" _YIv <sign> <string>: Customize interpretation verb for sign.");
    PrintS(" _YIC <house> <string>: Customize interpretation for house.");
    PrintS(" _YIA <asp> <string>: Customize interpretation for aspect.");
    PrintS(" _YIA0 <asp> <string>: Customize aspect interpretation statement.");
    PrintS(" _YkC <fir> <ear> <air> <wat>: Customize element colors.");
    PrintS(" _YkA <asp1> <asp2> <col1>..<col2>: Customize aspect colors.");
    PrintS(" _Yk0 <1..7> <1..7> <col1>..<col2>: Customize 'rainbow' colors.");
    PrintS(" _Yk <0..8> <0..8> <col1>..<col2>: Customize 'general' colors.");
    PrintS(" _YXG <0-2><0-2><0-2><0-3>: Select among different graphic glyphs");
    PrintS("     for Capricorn, Uranus, Pluto, and Lilith.");
    PrintS(" _YXg <cells>: Set number of cells for graphic aspect grid.");
    PrintS(" _YXf <val>: Set usage of actual system fonts in graphic file.");
    PrintS(" _YXp <-1,0,1>: Set paper orientation for PostScript files.");
    PrintS(" _YXp0 <hor> <ver>: Set paper size for PostScript files.");
#ifdef PCG
    PrintS(" _YX <hi-res> <lo-res>: Set modes to use for PC screen graphics.");
#endif
#ifdef KDE
    PrintS(" _YY <0-15> <colorname>: Give the nb'th color name (Kastrolog only).");
    PrintS(" _Yf <file name>: Give the default zoneinfo file name (Kastrolog only).");
    PrintS(" _YB : Allow default zoneinfo file for zoneless charts (Kastrolog only). ");
    PrintS(" _Ym <portx> <porty> <landx> <landy>: Set extra x y margins, given in cm,  for portrait and landscape KDE printing. (Kastrolog only).");
    PrintS(" _Ya : Allow daylight time saving in animations (Kastrolog only).");
#endif

    PrintS(" _YU : Switch use of alternate (star) restrictions");
    PrintS(" _YUo : Orbs of fixed stars. Default 1.2 deg.");
    PrintS(" _YUa : Number of star aspects taked into acconunt. Default 1.");
    PrintS(" _YUl : Select object where to start alternate restrictions");
    PrintS(" _YUh : Select object where to stop considering aspects for alternate restrictions");
    PrintS(" _YTR : Switch use of true instead of apparent positions");
    PrintS(" _YTC : Switch use of topocentric instead of apparent positions");
    PrintS(" _YTF : Switch use of refraction approximation for local horizon (-Z)");
    PrintS(" _YTT : Switch use of nutation");
    PrintS(" _YTH : Switch use of Swiss Ephemeris house calculations");
    PrintS(" _YTN : Switch use of GTOPO30 altitude (overrides chart files also)");
    PrintS(" _YTD : Switch use of 3dimensional aspects angles instead of ecliptical");
    PrintS(" _YT1 : Switch use of forced restriction of objects with uncertain orbit etc.");
    PrintS(" _YT2 : Flag for J2000 reference frame output");
    PrintS(" _YTI : Flag for ICRS reference frame output, will also affect -ZC");
    PrintS(" _YTJ : Flag looking for DE406 or DE405 (if twice) ephemeris file de405.eph de406.eph");
    PrintS(" _YTd value : Set deltaT to fixed value");
    PrintS(" _YN[L] object name : Set custom name for object,L for unnumbered");
    PrintS(" _YS <hh><steps> : For switch -Tc use <steps> of <hh> hours each");
    PrintS(" _0[o,i,q,X]: Disallow file output, input, exiting, and graphics.");
    PrintS(" _;: Ignore rest of command line and treat it as a comment.");
}


/* Print out a list of the various objects - planets, asteroids, house     */
/* cusps, stars - recognized by the program, and their index values. This  */
/* is displayed when the -O switch is invoked. For some objects, display   */
/* additional information, e.g. ruling signs for planets, brightnesses and */
/* positions in the sky for fixed stars, etc.                              */

void PrintObjects()
{
    char sz[cchSzDef];
    CI ci;
    int i, j, k;
    real Off;

    if (!us.fCusp)
        for (i = cuspLo; i <= cuspHi; i++)    /* Set up restrictions properly: */
            ignore[i] = fTrue;                  /* Minor cusps and uranians      */
    if (!us.fUranian)                       /* included only if -C and -u    */
        for (i = uranLo; i <= oNormOpt; i++)if (FNorm(i))   /* switches are in effect.       */
                ignore[i] = fTrue;
    sprintf(sz, "%s planets and objects:\n", szAppName);
    PrintSz(sz);
    PrintSz("No. Name       Rule Co-Rule Fall Co-Fall Exalt Debilitate\n\n");
    for (i = 1; i <= oNormOpt; i++) if (!ignore[i]) {
            AnsiColor(kObjA[i]);
            sprintf(sz, "%2d %-12s", i, szObjName[i]);
            PrintSz(sz);

            /* Print rulerships and exaltations for the planets. */

            j = ruler1[i];
            k = ruler2[i];
            if (j) {
                sprintf(sz, "%c%c%c", chSig3(j));
                PrintSz(sz);
            } else
                PrintSz("   ");
            PrintSz("  ");
            if (k) {
                sprintf(sz, "%c%c%c", chSig3(k));
                PrintSz(sz);
            } else
                PrintSz("   ");
            PrintTab(' ', 5);
            if (j) {
                sprintf(sz, "%c%c%c", chSig3(Mod12(j+6)));
                PrintSz(sz);
            } else
                PrintSz("   ");
            PrintSz("  ");
            if (k) {
                sprintf(sz, "%c%c%c", chSig3(Mod12(k+6)));
                PrintSz(sz);
            } else
                PrintSz("   ");
            PrintTab(' ', 5);
            j = exalt[i];
            if (j) {
                sprintf(sz, "%c%c%c", chSig3(j));
                PrintSz(sz);
            } else
                PrintSz("   ");
            PrintSz("   ");
            if (j) {
                sprintf(sz, "%c%c%c", chSig3(Mod12(j+6)));
                PrintSz(sz);
            } else
                PrintSz("   ");
            if (FCusp(i)) {
                sprintf(sz, "  House Cusp #%d", i-cuspLo+1);
                PrintSz(sz);
            }
            else if (FBetween(i,HypuranLo,HypuranHi)) {
                sprintf(sz, "  Uranian #%d", i-HypuranLo+1);
                PrintSz(sz);
            }
#ifdef ASTEROIDS
            else if (FBetween(i,CentLo,CentHi)) {
                sprintf(sz, "  Centaur #%d", i-CentLo+2);
                PrintSz(sz);
            }
            else if (FBetween(i,TrneLo,TrneHi)) {
                sprintf(sz, "  TNO #%d", i-TrneLo+1);
                PrintSz(sz);
            }
            else if (FBetween(i,AtenLo,AtenHi)) {
                sprintf(sz, "  Aten #%d", i-AtenLo+1);
                PrintSz(sz);
            }
            else if (FBetween(i,ApolLo,ApolHi)) {
                sprintf(sz, "  Apoll #%d", i-ApolLo+1);
                PrintSz(sz);
            }
            else if (FBetween(i,AmorLo,AmorHi)) {
                sprintf(sz, "  Amor #%d", i-AmorLo+1);
                PrintSz(sz);
            }
            else if (FBetween(i,MuseLo,MuseHi)) {
                sprintf(sz, "  Muse #%d", i-MuseLo+1);
                PrintSz(sz);
            }
            else if (FBetween(i,AgenLo,AgenHi)) {
                sprintf(sz, "  Asteroid #%d", i-AgenLo+5);
                PrintSz(sz);
            }
            else if (FBetween(i,AsteLo+us.nExtraStar,AsteHi)) {
                sprintf(sz, "  Extra Asteroid #%d", i-AsteLo-us.nExtraStar+1);
                PrintSz(sz);
            }
#endif
            PrintL();
        }

    /* Now, if -U in effect, read in and display stars in specified order. */

    if (us.nStar) {
        ci = ciCore;
        Off = ProcessInput(fTrue);
        ciCore = ci;
        ComputeStars(us.fSidereal ? 0.0 : -Off);
        for (i = starLo; i <= starLo-1+(us.fRiyalAst ? cStarSmall : cStar); i++) if (!ignore[i]) {
                j = starLo-1+starname[i-starLo+1];
                AnsiColor(kObjA[j]);
                sprintf(sz, "%2d %-12s", i, szObjName[j]);
                PrintSz(sz);
                sprintf(sz, "Star #%2d   ", i-starLo+1);
                PrintSz(sz);
                PrintZodiac(planet[j]);
                PrintTab(' ', 4);
                PrintAltitude(planetalt[j]);
                sprintf(sz, " %5.2f\n", rStarBright[j-starLo+1]);
                PrintSz(sz);
            }
    }
    AnsiColor(kDefault);
}


/* Print out a list of all the aspects recognized by the program, and info  */
/* about them: their names, index numbers, degree angles, present orbs, and */
/* the description of their glyph. This gets displayed when the -HA switch   */
/* is invoked (without any argument).                                       */

void PrintAspects()
{
    char sz[cchSzDef];
    int i;

    sprintf(sz,
            "%s aspects:\nNo. Name         Abbrev. ", szAppName);
    PrintSz(sz);
    PrintSz("Angle    Orb          Description of glyph\n\n");
    for (i = 1; i <= cAspect; i++) {
        AnsiColor(kAspA[i]);
        sprintf(sz, "%2d %-15s(%s) %6.2f +/- %1.0f degrees - %s\n",
                i, szAspectName[i], szAspectAbbrev[i],
                rAspAngle[i], rAspOrb[i], szAspectGlyph[i]);
        PrintSz(sz);
    }
    AnsiColor(kDefault);
}


/* Print out a list of the 12 signs and houses of the zodiac, and their    */
/* standard and traditional names, as done when the -H0 switch is invoked. */

void PrintSigns()
{
    char sz[cchSzDef];
    int i;

    sprintf(sz, "%s signs and houses:\n", szAppName);
    PrintSz(sz);
    PrintSz("Sign        English name      House Traditional name");
    PrintTab(' ', 19);
    PrintSz("Ruler\n\n");
    for (i = 1; i <= cSign; i++) {
        AnsiColor(kSignA(i));
        sprintf(sz, "%-12sthe %-14s%2d%s  House of %-24s  %s\n",
                szSignName[i], szSignEnglish[i], i, szSuffix[i], szHouseTradition[i],
                szObjName[rules[i]]);
        PrintSz(sz);
    }
    AnsiColor(kDefault);
}


#ifdef CONSTEL
/* Given the standard 'noun' form of a constellation string, convert it  */
/* to its genitive or posessive form. Some standard rules are used but a */
/* special instructions string is passed for special cases.              */

char *GetSzGenitive(szGen, szInst)
char *szGen, *szInst;
{
    char *pch, ch1, ch2;
    int cch;

    pch = szGen + CchSz(szGen);
    if (*szInst == ' ')            /* Instructions starting with a space or */
        szInst++;                    /* that are empty means no special case. */
    else if (*szInst) {
        cch = *szInst - '0';
        if (cch < 10) {          /* Instructions starting with a number means */
            szInst++;              /* hack off that many characters of string.  */
            pch -= cch;
        }
        while (*szInst > '9')  /* Instructions end with characters to append. */
            *pch++ = *szInst++;
        *pch = chNull;
        return szInst;
    }
    ch1 = *(pch-1);
    ch2 = *(pch-2);
    if (ch1 == 'a') {           /* Standard rule: 'a' ending -> 'ae'. */
        *pch++ = 'e';
        *pch = chNull;
    } else if (ch1 == 's' && ch2 == 'u') {     /* 'us' ending -> 'i'. */
        *(pch-2) = 'i';
        *(pch-1) = chNull;
    } else if (ch1 == 'm' && ch2 == 'u') {     /* 'um' ending -> 'i'. */
        *(pch-2) = 'i';
        *(pch-1) = chNull;
    } else if (ch1 == 'x')     /* Standard rule: 'x' ending -> 'cis'. */
        sprintf(pch-1, "cis");
    return szInst;
}


/* Given a constellation index, fill out a string with the genitive or   */
/* posessive form of its name. This basically just calls GetSzGenitive() */
/* above, however it has to be called twice for names having two words.  */

void GetSzConstel(szGen, i)
char *szGen;
int i;
{
    char sz1[cchSzDef], sz2[cchSzDef], *pchSpace, *szInst;

    sprintf(szGen, "%s", szCnstlName[i]);
    for (pchSpace = szGen; *pchSpace && *pchSpace != ' '; pchSpace++)
        ;
    szInst = (char *)szCnstlGenitive[i];
    if (*pchSpace == chNull) {
        GetSzGenitive(szGen, szInst);
        return;
    }
    *pchSpace = chNull;
    if (szInst[0] == '!') {
        GetSzGenitive(szGen, szInst+1);
        return;
    }
    sprintf(sz1, "%s", szGen);
    sprintf(sz2, "%s", pchSpace+1);
    szInst = GetSzGenitive(sz1, szInst);
    GetSzGenitive(sz2, szInst);
    sprintf(szGen, "%s %s", sz1, sz2);
}


/* Print out a list of the 88 constellations used in astronomy, in their  */
/* standard, English, and genitive forms, as invoked with the -HF switch. */

void PrintConstellations()
{
    int i, j = eWat;
    char szGen[cchSzDef], sz[cchSzDef], chLast = chNull;

    sprintf(sz, "%s constellations:\n", szAppName);
    PrintSz(sz);
    PrintSz("No. Name                Abbrev.   ");
    PrintSz("Meaning            Genitive form\n\n");
    for (i = 1; i <= cCnstl; i++) {
        if (szCnstlName[i][0] != chLast) {
            chLast = szCnstlName[i][0];
            j = (j + 1) & 3;
            AnsiColor(kElemA[j]);
        }
        sprintf(sz, "%2d: %-19s (%s) ", i, szCnstlName[i], szCnstlAbbrev[i]);
        PrintSz(sz);
        if (szCnstlMeaning[i][0] == ' ')
            sprintf(sz, "%-22s", szCnstlMeaning[i]+1);
        else
            sprintf(sz, "the %-18s", szCnstlMeaning[i]);
        PrintSz(sz);
        GetSzConstel(szGen, i);
        sprintf(sz, " (%s)\n", szGen);
        PrintSz(sz);
    }
    AnsiColor(kDefault);
}
#endif /* CONSTEL */


/* Print out a list of the planets in the solar system (and our Moon), */
/* listing astronomical info on them, as invoked with the -HS switch.  */

void PrintOrbit()
{
    char sz[cchSzDef];
    int i;
    real r;

    sprintf(sz, "%s planets:\n", szAppName);
    PrintSz(sz);
    PrintSz("   Name: Distance   Year Diameter     Day       Mass Density  ");
    PrintSz("Axis Satellites\n\n");
    for (i = 0; i <= oVes; i++) {
        AnsiColor(kObjA[i]);
        sprintf(sz, "%7s:%9.3f%7.2f%9.3f", szObjName[i],
                rObjDist[i]/rObjDist[0], rObjYear[i], rObjDiam[i]/rObjDiam[0]);
        PrintSz(sz);
        if (i <= oPlu) {
            r = rObjDiam[i]*1000.0/2.0;
            sprintf(sz, "%8.2f%11.3f%8.2f%6.2f%11d",
                    rObjDay[i], rObjMass[i],
                    (rObjMass[i]*5.974E24/1000.0)/((4.0/3.0)*(r*r*r)*rPi),
                    rObjAxis[i], cSatellite[i]);
            PrintSz(sz);
        }
        PrintL();
    }
    AnsiColor(kDefault);
}


#ifdef ARABIC
/* Compare the strings corresponding to two Arabic part formulas. Like   */
/* NCompareSz, this returns 0 if they are equal, a positive value if the */
/* first is greater, and negative if the second is greater.              */

int NCompareSzPart(ap1, ap2)
int ap1, ap2;
{
    char *pch1, *pch2;
    int ich;

    pch1 = ai[ap1].form;
    pch2 = ai[ap2].form;
    for (ich = 0; pch1[ich] && pch1[ich] == pch2[ich]; ich++) {
        if (!us.fArabicFlip) {

            /* If formulas are being displayed in alternate form, we need to */
            /* effectively swap two sections in the string and then compare. */

            if (ich == 2)
                ich = 5;
            else if (ich == 8)
                ich = 2;
            else if (ich == 5)
                ich = 8;
        }
    }
    return pch1[ich] - pch2[ich];
}


/* Print out a list of all the Arabic parts in the current chart, computing */
/* their positions first, as brought up with the -P switch.                 */

void DisplayArabic()
{
    real rPart[cPart], rBit[3], rCur, lDis, mDis;
    char sz[cchSzDef], *pch, ch;
    int iPart[cPart], h, i, j, k, l, ll, lll, nn;

    PrintSz("Num.");
    PrintTab(' ', 20);
    PrintSz("Name Position");
    PrintTab(' ', 1 + 4 * is.fSeconds);
    PrintSz("House Formula              Flip Type\n");

    /* Calculate the zodiac positions of all the parts. */

    for (i = 0; i < cPart; i++) {
        rPart[i] = -rDegMax;
        if (i >= us.nArabicParts)
            goto LNextPart;
        for (j = 0; j < 3; j++) {
            pch = &ai[i].form[j*3];
            ch = pch[1];
            if (ch == ' ')
                k = oAsc;
            else if (ch == 'F')
                k = -apFor;
            else if (ch == 'S')
                k = -apSpi;
            else
                k = (ch-'0') * 10 + (pch[2]-'0');
            ch = *pch;
            if (ch == 'h')      /* House cusp */
                rCur = chouse[k];
            else if (ch == 'r') /* Ruler of house cusp */
                rCur = planet[rules[SFromZ(chouse[k])]];
            else if (ch == 'j') /* 10 degrees of house cusp */
                rCur = chouse[k] + 10.0;
            else if (ch == 'H') /* Planet's house */
                rCur = chouse[inhouse[k]];
            else if (ch == 'R') /* Ruler of planet's house */
                rCur = planet[rules[SFromZ(chouse[inhouse[k]])]];
            else if (ch == 'D') /* Dispositor / ruler of planet's position */
                rCur = planet[rules[SFromZ(planet[k])]];
            else if (FBetween(ch, '0', '3'))
                rCur = (real)((ch-'0') * 100 + k);
            else {
                if (k < 1) {
                    rCur = rPart[-k];
                    if (rCur < 0.0)
                        goto LNextPart;
                } else {
                    if (ignore[k] && (us.fCusp || !FCusp(k)))
                        goto LNextPart;
                    else
                        rCur = planet[k];
                }
            }
            rBit[j] = rCur;
        }
        rCur = rBit[1] - rBit[2];
        if (us.nArabicNight < 0 || (ai[i].form[9] == 'F' &&
                                    inhouse[oSun] < sLib && us.nArabicNight == 0))
            rCur = -rCur;
        rCur = Mod(rCur + rBit[0]);
        rPart[i] = rCur;
LNextPart:
        iPart[i] = i;
    }

    /* Sort parts to figure out what order to display them in. */

    if (us.nArabic > 1) for (i = 1; i < cPart; i++) {
            j = i-1;

            /* Compare part zodiac locations for -Pz switch. */

            if (us.nArabic == 'z') while (j >= 0 &&
                                              rPart[iPart[j]] > rPart[iPart[j+1]]) {
                    SwapN(iPart[j], iPart[j+1]);
                    j--;

                    /* Compare part names for -Pn switch. */

                } else if (us.nArabic == 'n') while (j >= 0 && NCompareSz(
                                                         ai[iPart[j]].name, ai[iPart[j+1]].name) > 0) {
                    SwapN(iPart[j], iPart[j+1]);
                    j--;

                    /* Compare part formulas for -Pf switch. */

                } else if (us.nArabic == 'f') while (j >= 0 && NCompareSzPart(
                                                         iPart[j], iPart[j+1]) > 0) {
                    SwapN(iPart[j], iPart[j+1]);
                    j--;
                }
        }

    /* Display the positions and formulas of the parts. */

    for (h = i = 0; i < cPart; i++) {
        l = iPart[i];
        if (rPart[l] < 0.0)
            continue;

        lll = 0;
        if (PartAspects) {
            for (ll = 1; ll <= cObjOpt; ll++) {
                if (!ignore[ll]) {
                    lDis = MinDistance(rPart[l], planet[ll]);
                    for (nn = 1; nn <=PartAspects; nn++){
                        mDis = lDis - rAspAngle[nn];
                        if (RAbs(mDis) < PartOrb) {
                            lll = 1;
                            continue;
                        }
                    }
                }
            }
        } else
            lll = 1;

        if (!lll)
            continue;

        sprintf(sz, "%3d: %23.23s ", ++h, ai[l].name);
        PrintSz(sz);
        PrintZodiac(rPart[l]);
        j = HousePlaceIn(rPart[l]);
        sprintf(sz, " [%2d%s] ", j, szSuffix[j]);
        AnsiColor(kSignA(j));
        PrintSz(sz);
        AnsiColor(kDefault);
        PrintCh('(');
        for (j = 0; j < 3; j++) {
            k = j < 1 || us.fArabicFlip ? j : 3-j;
            pch = &ai[l].form[k*3];
            ch = pch[1];
            if (ch == ' ')
                k = oAsc;
            else if (ch == 'F')
                k = -apFor;
            else if (ch == 'S')
                k = -apSpi;
            else
                k = (ch-'0') * 10 + (pch[2]-'0');
            ch = *pch;
            if (k < 1) {
                AnsiColor(kObjA[oFor]);
                sprintf(sz, "%3.3s", ai[-k].name);
                PrintSz(sz);
            } else {
                if (ch == ' ' || ch == 'H' || ch == 'R' || ch == 'D') {
                    AnsiColor(kSignA(ruler1[k]));
                    sprintf(sz, "%c%c%c", chObj3(k));
                } else if (FBetween(ch, '0', '3')) {
                    k = (ch-'0') * 100 + k;
                    AnsiColor(kSignA(k/30+1));
                    sprintf(sz, "%2d%c%c%c", k%30, chSig3(k/30+1));
                } else {
                    AnsiColor(kSignA(k));
                    sprintf(sz, "%3d", k);
                }
                PrintSz(sz);
            }
            AnsiColor(kDefault);
            if (sz[3] == chNull) {
                PrintCh(' ');
                switch (ch) {
                case 'h':
                    ch = ' ';
                    break;
                case 'r':
                    ch = 'R';
                    break;
                case 'j':
                    ch = '&';
                    break;
                default:
                    ch = *pch;
                }
                PrintCh(ch);
            }
            if (j < 2) {
                sprintf(sz, " %c ", ((j < 1) == us.fArabicFlip) ? '+' : '-');
                PrintSz(sz);
            }
        }
        PrintCh(' ');
        ch = ai[l].form[9];
        switch (ch) {
        case 'F':
            PrintSz("Y");
            break;
        default:
            AnsiColor(kWhite);
            PrintSz("N");
            AnsiColor(kDefault);
            break;
        }
        PrintSz(") ");
        ch = ai[l].form[10];
        switch (ch) {
        case 'C':
            AnsiColor(kElemA[eWat]);
            PrintSz("Comm.");
            break;
        case 'E':
            AnsiColor(kElemA[eFir]);
            PrintSz("Event");
            break;
        case 'H':
            AnsiColor(kElemA[eEar]);
            PrintSz("Hora.");
            break;
        }
        AnsiColor(kDefault);
        PrintL();


        if (PartAspectsShow) {
            for (ll = 1; ll <= cObjOpt; ll++) {
                if (!ignore[ll]) {
                    lDis = MinDistance(planet[ll], rPart[l]);
                    for (nn = 1; nn <=PartAspectsShow; nn++){
                        if (ll == oFor && nn == 1 && l == 0)
                            continue;
                        mDis = lDis - rAspAngle[nn];
                        if (RAbs(mDis) < PartOrb) {
                            lll = (int) (60*mDis);
                            PrintTab(' ', 28);
                            sprintf(sz, " %s  %-4.4s", szAspectAbbrev[nn], szObjName[ll]);
                            PrintSz(sz);
                            sprintf(sz, "   orb: %c%d%c%02d'\n",
                                    (mDis < 0 ? '-' : '+'),
                                    abs(lll)/60, chDeg1, abs(lll)%60);
                            PrintSz(sz);
                        }
                    }
                }
            }
        }
    }
}
#endif /* ARABIC */


#ifdef GRAPH
/* Print a list of every key that one can press in a graphics window to do  */
/* a certain function, and a description of what it does, as displayed when */
/* one presses the 'H' or '?' key, and as shown with the -HX switch.        */

void DisplayKeysX()
{
    char sz[cchSzDef];

    sprintf(sz, "%s graphics screen key press options (version %s):",
            szAppName, szVersionCore);
    PrintS(sz);
    PrintS(" Press 'H' or '?' to display this list of key options.");
    PrintS(" Press 'p' to toggle pause status on or off.");
    PrintS(" Press 'x' to toggle fg/bg colors on screen.");
    PrintS(" Press 'm' to toggle color/monochrome display on screen.");
    PrintS(" Press 'i' to toggle status of the minor chart modification.");
    PrintS(" Press 't' to toggle header info on current chart on screen.");
    PrintS(" Press 'b' to toggle drawing of a border around the chart.");
    PrintS(" Press 'l' to toggle labeling of object points in chart.");
    PrintS(" Press 'j' to toggle not clearing screen between chart updates.");
    PrintS(" Press 'v' to display current chart positions on text screen.");
    PrintS(" Press 'R', 'C', 'u', 'U' to toggle restriction status of minor");
    PrintS("       objects, house cusps, main stars, and stars.");
    PrintS(" Press 'c' to toggle relationship comparison chart mode.");
    PrintS(" Press 's', 'h', 'f', 'g', 'z', 'y' to toggle status of sidereal");
    PrintS("       zodiac, heliocentric charts, domal charts, decan charts,");
    PrintS("       vedic format wheel charts, and navamsa charts.");
    PrintS(" Press 'O' and 'o' to recall/store a previous chart from memory.");
#ifdef X11
    PrintS(" Press 'B' to dump current window contents to root background.");
#else
    PrintS(" Press 'B' to resize chart display to full size of screen.");
#endif
    PrintS(" Press 'Q' to resize chart display to a square.");
    PrintS(" Press '<' and '>' to decrease/increase the scale size of the");
    PrintS("       glyphs and the size of world map.");
    PrintS(" Press '[' and ']' to decrease/increase tilt in globe display.");
    PrintS(" Press '+' and '-' to add/subtract a day from current chart.");
#ifdef TIME
    PrintS(" Press 'n' to set chart information to current time now.");
#endif
    PrintS(" Press 'N' to toggle animation status on or off. Charts will");
    PrintS("       be updated to current status and globe will rotate.");
    PrintS(" Press '!'-'(' to begin updating current chart by adding times.");
    PrintS("       !: seconds, @: minutes, #: hours, $: days, %: months,");
    PrintS("       ^: years, &: years*10, *: years*100, (: years*1000.");
    PrintS(" Press 'r' to reverse direction of time-lapse or animation.");
    PrintS(" Press '1' to switch Atens");
    PrintS(" Press '2' to switch Apollos");
    PrintS(" Press '3' to switch Amors");
    PrintS(" Press '4' to switch Centaurs");
    PrintS(" Press '5' to switch TNO's");
    PrintS(" Press '6' to switch some other asteroids");
    PrintS(" Press '7' to switch Muses");
    PrintS(" Press '8' to switch Hypothetical Planets");
    PrintS(" Press '9' to switch Extra Asteroids loaded");
    PrintS(" Press '\"' to switch some unnumbered Asteroids");
#ifdef ASTEROIDS
    PrintS(" Press '\/' to find next Node or Asteroid");
    PrintS(" Press '\\' to find previous Node or Asteroid");
#endif
#ifdef PCG
    PrintS(" Press '1'-'9' to determine section of chart to show if clipped.");
#endif
    PrintS(
        " Press 'V','A','Z','S','M','K','J','L','E','W','G','P' to switch to");
    PrintS(
        "       normal (_v), grid (_g), local (_Z), space (_S), sector (_l),");
    PrintS("       calendar (_K), dispositor (_j), astro-graph (_L), ephemeris");
    PrintS("       (_E), world map (_XW), globe (_XG), and polar (_XP) modes.");
    PrintS(" Press 'Y' to switch to biorhythm relation chart mode.");
    PrintS(" Press '0' to toggle between _Z,_Z0 & _XW,_XW0 & _E,_Ey modes.");
#ifdef CONSTEL
    PrintS(" Press 'F' to toggle between world and constellation map modes.");
#endif
#ifdef PCG
    PrintS(" Press 'F1'..'F12' [plus Shift,Ctrl,Alt] to run macros 1..48.");
#endif
    PrintS(" Press 'space' to force redraw of current graphics display.");
    PrintS(" Press 'del' to clear the graphics screen and not redraw.");
#ifdef PCG
    PrintS(" Press 'tab' to toggle between graphics resolutions.");
#endif
    PrintS(" Press 'enter' to input a command line of general switches.");
    PrintS(" Press 'q' to terminate graphics and the program.");
#ifdef MOUSE
    PrintL();
#ifdef X11
    PrintS(" Left   mouse button: Draw line strokes on chart in window.");
    PrintS(" Middle mouse button: Print coordinates of pointer on world map.");
    PrintS(" Right  mouse button: Terminate the window and program.");
#endif
#ifdef PCG
    PrintS(" Left  mouse button: Draw line strokes on chart in screen.");
    PrintS(" Right mouse button: Set coordinates to pointer on world map.");
#endif
#endif /* MOUSE */
}


/* Print a list of every command switch dealing with the graphics features  */
/* that can be passed to the program, and a description of what it does.    */
/* This is part of what the -H switch prints, if graphics were compiled in. */

void DisplaySwitchesX()
{
    PrintS(" _X: Create a graphics chart instead of displaying it as text.");
#ifdef ISG
    PrintS(" _Xb: Create bitmap file instead of putting graphics on screen.");
#endif
    PrintS(" _Xb[n,c,v,a,b]: Set bitmap file output mode to X11 normal,");
    PrintS("     compacted, very compact, Ascii (bmtoa), or Windows bmp.");
#ifdef PS
    PrintS(" _Xp: Create PostScript stroke graphic instead of bitmap file.");
    PrintS(" _Xp0: Like _Xp but create complete instead of encapsulated file.");
#endif
#ifdef META
    PrintS(" _XM[0]: Create Windows metafile stroke graphic instead of bitmap.");
#endif
    PrintS(" _Xo <file>: Write output bitmap or graphic to specified file.");
#ifdef X11
    PrintS(" _XB: Display X chart on root instead of in a separate window.");
#endif
    PrintS(" _Xm: Create monochrome graphic instead of one in color.");
    PrintS(" _Xr: Create chart graphic in reversed colors (white background).");
#ifdef X11
    PrintS(" _Xw <hor> [<ver>], _ge[..]: Change the size of chart graphic.");
#else
    PrintS(" _Xw <hor> [<ver>]: Change the size of chart graphic.");
#endif
    PrintS(" _Xs <100,200,300,400>: Change the size of map or characters by %.");
    PrintS(" _Xi: Create chart graphic in slightly modified form.");
    PrintS(" _Xt: Inhibit display of chart info at bottom of graphic.");
    PrintS(" _Xu: Inhibit display of a border around graphic.");
    PrintS(" _Xl: Inhibit labeling of object points in chart graphic.");
    PrintS(" _Xj: Don't clear screen between chart updates, drawing trails.");
    PrintS(" _X1 <object>: Rotate wheel charts so object is at left edge.");
    PrintS(" _X2 <object>: Rotate wheel charts so object is at top edge.");
#ifdef X11
    PrintS(" _Xd <name>, _di[..] <name>: Open X window on specified display.");
#endif
    PrintS(" _XW: Simply create an image of the world map.");
    PrintS(" _XW0: Like _XW but do a non-rectangular Mollewide projection.");
    PrintS(" _XG [<degrees>]: Display the image of the world as a globe.");
    PrintS(" _XP: Like _XG but create the globe from a polar projection.");
#ifdef CONSTEL
    PrintS(" _XF: Display maps as constellations on the celestial sphere.");
#endif
#ifdef ISG
    PrintS(" _Xn [<mode>]: Start up chart or globe display in animation mode.");
    PrintS(" _HX: Display list of key press options for screen graphics.");
#endif
}


#ifdef WIN
/* Print a list of every command switch dealing with the Windows features */
/* that can be passed to the program, and a description of what it does.  */
/* This is part of what the -H switch prints in the MS Windows version.   */

void DisplaySwitchesW()
{
    PrintS(" _W <value>: Run given Windows menu command internally.");
    PrintS(" _WN <1-32000>: Set animation update delay in milliseconds.");
    PrintS(" _WM <1-48> <text>: Set Windows menu text for macro command.");
    PrintS(" _Wn: Don't redraw screen until user forces update.");
}
#endif
#endif /* GRAPH */


/* This is the dispatch procedure for all the generic table information      */
/* routines, such as those displaying the -H switch list, the list of signs, */
/* objects, default interpretations, and so on not requiring a date or time. */

bool FPrintTables()
{
#ifdef WIN
    if (us.fGraphics)
        return fFalse;
#endif
    if (us.fCredit) {
        DisplayCredits();
        is.fMult = fTrue;
    }
    if (us.fSwitch) {
        if (is.fMult)
            PrintL2();
        DisplaySwitches();
        is.fMult = fTrue;
    }
    if (us.fSwitchRare) {
        if (is.fMult)
            PrintL2();
        DisplaySwitchesRare();
        is.fMult = fTrue;
    }
#ifdef GRAPH
    if (us.fKeyGraph) {
        if (is.fMult)
            PrintL2();
        DisplayKeysX();
        is.fMult = fTrue;
    }
#endif
    if (us.fSign) {
        if (is.fMult)
            PrintL2();
        PrintSigns();
        is.fMult = fTrue;
    }
    if (us.fObject) {
        if (is.fMult)
            PrintL2();
        PrintObjects();
        is.fMult = fTrue;
    }
    if (us.fAspect) {
        if (is.fMult)
            PrintL2();
        PrintAspects();
        is.fMult = fTrue;
    }
#ifdef CONSTEL
    if (us.fConstel) {
        if (is.fMult)
            PrintL2();
        PrintConstellations();
        is.fMult = fTrue;
    }
#endif
    if (us.fOrbitData) {
        if (is.fMult)
            PrintL2();
        PrintOrbit();
        is.fMult = fTrue;
    }
#ifdef INTERPRET
    if (us.fMeaning) {
        if (is.fMult)
            PrintL2();
        InterpretGeneral();
        InterpretAspectGeneral();
        is.fMult = fTrue;
    }
#endif

    /* If we also already have enough information to generate a chart,    */
    /* then go on and do so, else exit. (So things like "-H -i file" will */
    /* work, but things like just "-H" will print and exit right away.)   */

#ifndef WIN
    return is.fMult && !is.fHaveInfo;
#else
    return is.fMult;
#endif
}

/* charts0.c */
