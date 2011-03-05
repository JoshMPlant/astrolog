/*
** Astrolog (Version 5.42J) File: xscreen.c
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


#ifdef GRAPH
/*
******************************************************************************
** Astrolog Icon.
******************************************************************************
*/

#ifdef X11
/* This information used to define Astrolog's X icon (Rainbow over Third */
/* Eye) is identical to the output format used by the bitmap program.    */
/* You could extract this section and run xsetroot -bitmap on it.        */

#define icon_width 63
#define icon_height 32
static byte icon_bits[] = {
    0x00,0x00,0x00,0xa8,0x0a,0x00,0x00,0x00,0x00,0x00,0x40,0x55,0x55,0x01,0x00,
    0x00,0x00,0x00,0xa8,0xaa,0xaa,0x0a,0x00,0x00,0x00,0x00,0x54,0xf5,0x57,0x15,
    0x00,0x00,0x00,0x80,0xaa,0xaa,0xaa,0xaa,0x00,0x00,0x00,0x40,0xd5,0xff,0xff,
    0x55,0x01,0x00,0x00,0xa0,0xaa,0xaa,0xaa,0xaa,0x02,0x00,0x00,0x50,0xfd,0xff,
    0xff,0x5f,0x05,0x00,0x00,0xa8,0xaa,0x2a,0xaa,0xaa,0x0a,0x00,0x00,0xd4,0xff,
    0xaf,0xfa,0xff,0x15,0x00,0x00,0xaa,0x2a,0x00,0x00,0xaa,0x2a,0x00,0x00,0xf5,
    0xbf,0xaa,0xaa,0xfe,0x57,0x00,0x80,0xaa,0x02,0x00,0x00,0xa0,0xaa,0x00,0x40,
    0xfd,0xab,0xfa,0xaf,0xea,0x5f,0x01,0xa0,0xaa,0x80,0xff,0xff,0x80,0xaa,0x02,
    0x50,0xff,0xea,0xff,0xff,0xab,0x7f,0x05,0xa0,0x2a,0xf0,0xff,0xff,0x07,0xaa,
    0x02,0xd0,0xbf,0xfa,0x0f,0xf8,0xaf,0x7e,0x05,0xa8,0x0a,0xfc,0x01,0xc0,0x1f,
    0xa8,0x0a,0xd4,0xaf,0x7e,0x00,0x00,0xbf,0xfa,0x15,0xa8,0x0a,0x3f,0x00,0x00,
    0x7e,0xa8,0x0a,0xf4,0xaf,0x1f,0xe0,0x03,0xfc,0xfa,0x15,0xaa,0x82,0x0f,0xdc,
    0x1d,0xf8,0xa0,0x2a,0xf4,0xab,0x07,0x23,0x62,0xf0,0xea,0x17,0xaa,0xc2,0x87,
    0x91,0xc4,0xf0,0xa1,0x2a,0xf4,0xeb,0xc3,0xd0,0x85,0xe1,0xeb,0x17,0xaa,0xe0,
    0x83,0x91,0xc4,0xe0,0x83,0x2a,0xf5,0xeb,0x03,0x23,0x62,0xe0,0xeb,0x57,0xaa,
    0xe0,0x01,0xdc,0x1d,0xc0,0x83,0x2a,0xf5,0xeb,0x01,0xe0,0x03,0xc0,0xeb,0x57,
    0xaa,0xe0,0x01,0x00,0x00,0xc0,0x83,0x2a,0xfd,0xeb,0x01,0x00,0x00,0xc0,0xeb,
    0x5f
};
#endif


/*
******************************************************************************
** Interactive Screen Graphics Routines.
******************************************************************************
*/

/* Set up all the colors used by the program, i.e. the foreground and   */
/* background colors, and all the colors in the object arrays, based on */
/* whether or not we are in monochrome and/or reverse video mode.       */

void InitColorsX()
{
    int i;
#ifdef X11
    Colormap cmap;
    XColor xcol;

    if (!gi.fFile) {
        cmap = XDefaultColormap(gi.disp, gi.screen);

        /* Allocate a color from the present X11 colormap. Given a string like */
        /* "violet", allocate this color and return a value specifying it.     */

        for (i = 0; i < 16; i++) {
            XParseColor(gi.disp, cmap, szColorX[i], &xcol);
            XAllocColor(gi.disp, cmap, &xcol);
            rgbind[i] = xcol.pixel;
        }
    }
#endif
    gi.kiOn   = kMainA[!gs.fInverse];
    gi.kiOff  = kMainA[(int) gs.fInverse];
    gi.kiLite = gs.fColor ? kMainA[2+gs.fInverse] : gi.kiOn;
    gi.kiGray = gs.fColor ? kMainA[3-gs.fInverse] : gi.kiOn;
    for (i = 0; i <= 8; i++)
        kMainB[i]    = gs.fColor ? kMainA[i]    : gi.kiOn;
    for (i = 0; i <= 7; i++)
        kRainbowB[i] = gs.fColor ? kRainbowA[i] : gi.kiOn;
    for (i = 0; i < 4; i++)
        kElemB[i]    = gs.fColor ? kElemA[i]    : gi.kiOn;
    for (i = 0; i <= cAspect; i++)
        kAspB[i]     = gs.fColor ? kAspA[i]     : gi.kiOn;
    for (i = 0; i <= cObjOpt; i++)
        kObjB[i]     = gs.fColor ? kObjA[i]     : gi.kiOn;
#ifdef X11
    if (!gi.fFile) {
        XSetBackground(gi.disp, gi.gc,   rgbind[gi.kiOff]);
        XSetForeground(gi.disp, gi.pmgc, rgbind[gi.kiOff]);
    }
#endif
}


#ifdef ISG
/* This routine opens up and initializes a window and prepares it to be */
/* drawn upon, and gets various information about the display, too.     */

void BeginX()
{
#ifdef X11
    gi.disp = XOpenDisplay(gs.szDisplay);
    if (gi.disp == NULL) {
        PrintError("Can't open display.");
        Terminate(tcFatal);
    }
    gi.screen = DefaultScreen(gi.disp);
    bg = BlackPixel(gi.disp, gi.screen);
    fg = WhitePixel(gi.disp, gi.screen);
    hint.x = gi.xOffset;
    hint.y = gi.yOffset;
    hint.width = gs.xWin;
    hint.height = gs.yWin;
    hint.min_width = BITMAPX1;
    hint.min_height = BITMAPY1;
    hint.max_width = BITMAPX;
    hint.max_height = BITMAPY;
    hint.flags = PPosition | PSize | PMaxSize | PMinSize;
#if FALSE
    wmhint = XGetWMHints(gi.disp, gi.wind);
    wmhint->input = True;
    XSetWMHints(gi.disp, gi.wind, wmhint);
#endif
    gi.depth = DefaultDepth(gi.disp, gi.screen);
    if (gi.depth < 5) {
        gi.fMono = fTrue;      /* Is this a monochrome monitor? */
        gs.fColor = fFalse;
    }
    gi.root = RootWindow(gi.disp, gi.screen);
    if (gs.fRoot)
        gi.wind = gi.root;     /* If -XB in effect, we'll use the root window. */
    else
        gi.wind = XCreateSimpleWindow(gi.disp, DefaultRootWindow(gi.disp),
                                      hint.x, hint.y, hint.width, hint.height, 5, fg, bg);
    gi.pmap = XCreatePixmap(gi.disp, gi.wind, gs.xWin, gs.yWin, gi.depth);
    gi.icon = XCreateBitmapFromData(gi.disp, DefaultRootWindow(gi.disp),
                                    icon_bits, icon_width, icon_height);
    if (!gs.fRoot)
        XSetStandardProperties(gi.disp, gi.wind, szAppName, szAppName, gi.icon,
                               (byte **)xkey, 0, &hint);

    /* We have two graphics workareas. One is what the user currently sees in */
    /* the window, and the other is what we are currently drawing on. When    */
    /* done, we can quickly copy this to the viewport for a smooth look.      */

    gi.gc = XCreateGC(gi.disp, gi.wind, 0, 0);
    XSetGraphicsExposures(gi.disp, gi.gc, 0);
    gi.pmgc = XCreateGC(gi.disp, gi.wind, 0, 0);
    InitColorsX();                                  /* Go set up colors. */
    if (!gs.fRoot)
        XSelectInput(gi.disp, gi.wind, KeyPressMask | StructureNotifyMask |
                     ExposureMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
    XMapRaised(gi.disp, gi.wind);
    XSync(gi.disp, 0);
    XFillRectangle(gi.disp, gi.pmap, gi.pmgc, 0, 0, gs.xWin, gs.yWin);
#endif /* X11 */
#ifdef KDE
    gi.fMono = fFalse;      /* Is this a monochrome monitor? */
    gs.fColor = fTrue;
    InitColorsX();
    KBegin();
#endif

#ifdef WIN
    if (wi.fChartWindow && (wi.xClient != gs.xWin ||
                            wi.yClient != gs.yWin) && wi.hdcPrint == hdcNil)
        ResizeWindowToChart();
    gi.xOffset = NMultDiv(wi.xClient - gs.xWin, wi.xScroll, nScrollDiv);
    gi.yOffset = NMultDiv(wi.yClient - gs.yWin, wi.yScroll, nScrollDiv);
    SetWindowOrg(wi.hdc, -gi.xOffset, -gi.yOffset);
    SetWindowExt(wi.hdc, wi.xClient, wi.yClient);
    SetMapMode(wi.hdc, MM_ANISOTROPIC);
    SelectObject(wi.hdc, GetStockObject(NULL_PEN));
    SelectObject(wi.hdc, GetStockObject(NULL_BRUSH));
    if (!gs.fJetTrail || wi.hdcPrint != hdcNil)
        PatBlt(wi.hdc, -gi.xOffset, -gi.yOffset, wi.xClient, wi.yClient,
               gs.fInverse ? WHITENESS : BLACKNESS);
    InitColorsX();
#endif /* WIN */

#ifdef MSG
    if (!FValidResmode(gi.nRes))    /* Initialize graphics mode to hi-res. */
        gi.nRes = gs.nResHi;
    _setvideomode(gi.nRes == -1 ? _VRES16COLOR : gi.nRes);
    if (_grstatus()) {
        PrintError("Can't enter graphics mode.");
        Terminate(tcFatal);
    }
    _getvideoconfig((struct videoconfig far *) &gi.cfg);
    if (gi.cfg.numcolors < 16) {
        gi.fMono  = fTrue;
        gs.fColor = fFalse;
    }
    _remapallpalette((long FPTR *) rgb);
    _setactivepage(0);
    _setvisualpage(0);
    InitColorsX();
#ifdef MOUSE
    MouseInit(xPcScreen, yPcScreen);
#endif
    /* Make sure we reset textrows upon restart. */
    gs.nTextRows = abs(gs.nTextRows);
#endif /* MSG */

#ifdef BGI
    int i;
    static struct palettetype pal;

    if (!FValidResmode(gi.nRes))    /* Initialize graphics mode to hi-res. */
        gi.nRes = gs.nResHi;
    if (!gi.fLoaded) {
        registerfarbgidriver(ATT_driver_far);      /* attf.obj     */
        registerfarbgidriver(CGA_driver_far);      /* cgaf.obj     */
        registerfarbgidriver(EGAVGA_driver_far);   /* egavgaf.obj  */
        registerfarbgidriver(Herc_driver_far);     /* hercf.obj    */
        registerfarbgidriver(IBM8514_driver_far);  /* ibm8514f.obj */
        registerfarbgidriver(PC3270_driver_far);   /* pc3270f.obj  */
        gi.nDriver = DETECT;
        initgraph(&gi.nDriver, &gi.nGraph, "");
        gi.fLoaded = fTrue;
    }
    if (gi.nRes <= 0) {
        switch (gi.nDriver) {
        case CGA:
            gi.nGraph = CGAHI;
            break;
        case MCGA:
            gi.nGraph = MCGAHI;
            break;
        case EGA:
            gi.nGraph = EGAHI;
            break;
        case EGA64:
            gi.nGraph = EGA64HI;
            break;
        case EGAMONO:
            gi.nGraph = EGAMONOHI;
            break;
        case HERCMONO:
            gi.nGraph = HERCMONOHI;
            break;
        case ATT400:
            gi.nGraph = ATT400HI;
            break;
        case VGA:
            gi.nGraph = VGAHI;
            break;
        case PC3270:
            gi.nGraph = PC3270HI;
            break;
        case IBM8514:
            gi.nGraph = IBM8514HI;
            break;
        default:
            gi.nGraph = 0;
        }
    } else {
        switch (gi.nDriver) {
        case CGA:
            gi.nGraph = CGAHI;
            break;
        case MCGA:
            gi.nGraph = MCGAHI;
            break;
        case EGA:
            gi.nGraph = EGAHI;
            break;
        case EGA64:
            gi.nGraph = EGA64HI;
            break;
        case EGAMONO:
            gi.nGraph = EGAMONOHI;
            break;
        case HERCMONO:
            gi.nGraph = HERCMONOHI;
            break;
        case ATT400:
            gi.nGraph = ATT400HI;
            break;
        case VGA:
            gi.nGraph = VGAMED;
            break;
        case PC3270:
            gi.nGraph = PC3270HI;
            break;
        case IBM8514:
            gi.nGraph = IBM8514LO;
            break;
        default:
            gi.nGraph = 0;
        }
    }
    setgraphmode(gi.nGraph);
    if (graphresult()) {
        PrintError("Can't enter graphics mode.");
        Terminate(tcFatal);
    }
    gi.nPages = 1 + (gi.nDriver == HERCMONO ||
                     (gi.nDriver == VGA && gi.nGraph != VGAHI) || gi.nDriver == EGA);
    if (getmaxcolor()+1 < 16) {
        gi.fMono  = fTrue;
        gs.fColor = fFalse;
    }
    getpalette(&pal);
    for (i = 0; i < pal.size; i++)
        pal.colors[i] = (byte)rgb[i];
    setallpalette(&pal);
    setactivepage(0);
    setvisualpage(0);
    gi.nPageCur = 0;
    InitColorsX();
#ifdef MOUSE
    MouseInit(xPcScreen, yPcScreen);
#endif
    /* Make sure we reset textrows upon restart. */
    gs.nTextRows = abs(gs.nTextRows);
#endif /* BGI */

#ifdef MACG
    MaxApplZone();
    InitGraf(&thePort);
    InitFonts();
    FlushEvents(everyEvent, 0);
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(0L);
    InitCursor();

    gi.rcDrag = screenBits.bounds;
    gi.rcBounds.left = 20;
    gi.rcBounds.top = 20 + GetMBarHeight();
    gi.rcBounds.right = gi.rcBounds.left + gs.xWin;
    gi.rcBounds.bottom = gi.rcBounds.top + gs.yWin;
    gi.wpAst = NewCWindow(0L, &gi.rcBounds, "\pAstrolog 5.42D with Swiss Ephemeris", true,
                          noGrowDocProc, (WindowPtr)-1L, true, 0);
    SetPort(gi.wpAst);
    InitColorsX();
#endif /* MACG */
}


#endif /* ISG */
#endif /* GRAPH */
/* Add a certain amount of time to the current hour/day/month/year quantity */
/* defining the present chart. This is used by the chart animation feature. */
/* We can add or subtract anywhere from 1 to 9 seconds, minutes, hours,     */
/* days, months, years, decades, centuries, or millenia in any one call.    */
/* This is mainly just addition to the appropriate quantity, but we have    */
/* to check for overflows, e.g. Dec 30 + 3 days = Jan 2 of Current year + 1 */
/* Also used by -Tc */
void AddTime(mode, toadd)
int mode, toadd;
{
    int d;
    real h, m;

    if (!FBetween(mode, 1, 9))
        mode = 4;

    h = RFloor(TT);
    m = RFract(TT)*100.0;
    if (mode == 1)
        m += 1.0/60.0*(real)toadd;    /* Add seconds. */
    else if (mode == 2)
        m += (real)toadd;             /* add minutes. */

    /* Add hours, either naturally or if minute value overflowed. */

    if (m < 0.0 || m >= 60.0 || mode == 3) {
        if (m >= 60.0) {
            m -= 60.0;
            toadd = NSgn(toadd);
        } else if (m < 0.0) {
            m += 60.0;
            toadd = NSgn(toadd);
        }
        h += (real)toadd;
    }

    /* Add days, either naturally or if hour value overflowed. */

    if (h >= 24.0 || h < 0.0 || mode == 4) {
        if (h >= 24.0) {
            h -= 24.0;
            toadd = NSgn(toadd);
        } else if (h < 0.0) {
            h += 24.0;
            toadd = NSgn(toadd);
        }
        DD = AddDay(MM, DD, YY, toadd);
    }

    /* Add months, either naturally or if day value overflowed. */

    if (DD > (d = DayInMonth(MM, YY)) || DD < 1 || mode == 5) {
        if (DD > d) {
            DD -= d;
            toadd = NSgn(toadd);
        } else if (DD < 1) {
            DD += DayInMonth(Mod12(MM - 1), YY);
            toadd = NSgn(toadd);
        }
        MM += toadd;
    }

    /* Add years, either naturally or if month value overflowed. */

    if (MM > 12 || MM < 1 || mode == 6) {
        if (MM > 12) {
            MM -= 12;
            toadd = NSgn(toadd);
        } else if (MM < 1) {
            MM += 12;
            toadd = NSgn(toadd);
        }
        YY += toadd;
    }
    if (mode == 7)
        YY += 10 * toadd;      /* Add decades.   */
    else if (mode == 8)
        YY += 100 * toadd;     /* Add centuries. */
    else if (mode == 9)
        YY += 1000 * toadd;    /* Add millenia.  */
    TT = h+m/100.0;          /* Recalibrate hour time. */
}

/* Animate the current chart based on the given values indicating how much  */
/* to update by. We update and recast the current chart info appropriately. */
/* Note animation mode for comparison charts will update the second chart.  */

#ifdef GRAPH
#ifdef ISG
void Animate(mode, toadd)
int mode, toadd;
{
    CI ciT;

    if (us.fProgress && !us.nRel)
        ciT = ciCore;

    if (gi.nMode == gWorldMap || gi.nMode == gGlobe || gi.nMode == gPolar) {
        gs.nRot += toadd;
        if (gs.nRot >= nDegMax)     /* For animating globe display, add */
            gs.nRot -= nDegMax;       /* in appropriate degree value.     */
        else if (gs.nRot < 0)
            gs.nRot += nDegMax;
    } else {
        if (mode == 10) {
#ifdef TIME
            /* For the continuous chart update to present moment */
            /* animation mode, go get whatever time it is now.   */
            FInputData(szNowCore);
#else
            if (us.nRel)
                ciCore = ciTwin;
            else
                ciCore = ciMain;
            AddTime(1, toadd);
#endif
        } else {  /* Otherwise add on appropriate time vector to chart info. */
            if (us.nRel)
                ciCore = ciTwin;
            else
                ciCore = ciMain;
            AddTime(mode, toadd);
        }
        if (us.nRel) {
            ciTwin = ciCore;
            ciCore = ciMain;
            if (fReturnChart&&fReturnChartCast)
                fSynodicReturnChart ? ChartSynodicReturnSet() : ChartReturnSet();
        }
        else if (fReturnChart&&fReturnChartCast){
            ciTwin = ciCore;
            fSynodicReturnChart ? ChartSynodicReturnSet() : ChartReturnSet();
            ciMain = ciTwin;
            ciCore = ciTwin;
        }
        else
            ciMain = ciCore;
        if (us.fProgress && !us.nRel)
            ciCore = ciT;
        if (us.nRel)
            CastRelation();
        else
            CastChart(fTrue);
    }
}


/* This routine exits graphics mode, prompts the user for a set of command */
/* switches, processes them, and returns to the previous graphics with the */
/* new settings in effect, allowing one to change most any setting without */
/* having to lose their graphics state or fall way back to a -Q loop.      */

void CommandLineX()
{
    char szCommandLine[cchSzMax], *rgsz[MAXSWITCHES];
    int argc, fT, fPause = fFalse;

    ciCore = ciMain;
#ifdef MSG
    _setvideomode(_DEFAULTMODE);
    _settextrows(gs.nTextRows);
#endif
#ifdef BGI
    restorecrtmode();
    if (gs.nTextRows > 25)
        textmode(C4350);
#endif
    fT = us.fLoop;
    us.fLoop = fTrue;
    argc = NPromptSwitches(szCommandLine, rgsz);
    is.cchRow = 0;
    is.fSzInteract = fTrue;
    if (!FProcessSwitches(argc, rgsz))
        fPause = fTrue;
    else {
        is.fMult = fFalse;
        FPrintTables();
        if (is.fMult) {
            ClearB((lpbyte)&us.fCredit,
                   (int)((lpbyte)&us.fLoop - (lpbyte)&us.fCredit));
            fPause = fTrue;
        }
    }

#ifdef PCG
    /* Pause for the user if there was either an error processing the    */
    /* switches, or one of the informational text tables was brought up. */

    if (fPause) {
        AnsiColor(kDefault);
        is.cchRow = 0;
        PrintSz("Press any key to return to graphics.\n");
        while (!kbhit())
            ;
        getch();
    }
#endif
    is.fSzInteract = fFalse;
    us.fLoop = fT;
    ciMain = ciCore;
    BeginX();
}


/* Given two chart size values, adjust them such that the chart will look */
/* "square". We round the higher value down and check certain conditions. */

void SquareX(x, y, force)
int *x, *y, force;
{
    if (!force && !fSquare)    /* Unless we want to force a square, realize */
        return;                  /* that some charts look better rectangular. */
    if (*x > *y)
        *x = *y;
    else
        *y = *x;
#ifdef PCG
    if (FEgaRes(gi.nRes))         /* Scale horizontal size if we're in a PC */
        *x = VgaFromEga(*x);        /* graphics mode without "square" pixels. */
    else if (FCgaRes(gi.nRes))
        *x = VgaFromCga(*x);
#endif
    if (fSidebar)      /* Take into account chart's sidebar, if any. */
        *x += xSideT;
}


#ifndef WIN
/* This routine gets called after graphics are brought up and displayed     */
/* on the screen. It loops, processing key presses, mouse clicks, etc, that */
/* the window receives, until the user specifies they want to exit program. */

void InteractX()
{
#ifdef X11
    char sz[cchSzDef];
    XEvent xevent;
    KeySym keysym;
    int fResize = fFalse, fRedraw = fFalse;
#endif
#ifdef KDE
    int fResize = fFalse, fRedraw = fFalse;
#endif
#ifdef PCG
    int xcorner = 7,buttonx=-1,buttoy=-1;
#ifdef MOUSE
    int eventx, eventy, eventbtn;
#endif
    int fResize = fTrue, fRedraw = fFalse;
#endif /* PCG */
#ifdef MACG
    EventRecord erCur;
    WindowPtr wpCur;
    int wc, fEvent, fResize = fFalse, fRedraw = fTrue;
#endif
    int fBreak = fFalse, fPause = fFalse, fCast = fFalse, 
                                  mousex = -1, mousey = -1, dir = 1,
                                                                      length, key, i;
    bool fT;
    KI coldrw = gi.kiLite;

    neg(gs.nAnim);
    while (!fBreak) {
#ifdef KDE
        if (!key && gs.nAnim)
            break;
#endif

        gi.nScale = gs.nScale/100;

        /* Some chart windows, like the world maps and aspect grids, should */
        /* always be a certian size, so correct if a resize was attempted.  */

        if (fMap) {
            length = nDegMax*gi.nScale;
            if (gs.xWin != length) {
                gs.xWin = length;
                fResize = fTrue;
            }
            length = nDegHalf*gi.nScale;
            if (gs.yWin != length) {
                gs.yWin = length;
                fResize = fTrue;
            }
        } else if (gi.nMode == gGrid) {
            if (gs.xWin != (length =
                                (gs.nGridCell + (us.nRel <= rcDual))*CELLSIZE*gi.nScale+1)) {
                gs.xWin = length;
                fResize = fTrue;
            } if (gs.yWin != length) {
                gs.yWin = length;
                fResize = fTrue;
            }

            /* Make sure the window isn't too large or too small. */

        } else {
            if (gs.xWin < BITMAPX1) {
                gs.xWin = BITMAPX1;
                fResize = fTrue;
            } else if (gs.xWin > BITMAPX) {
                gs.xWin = BITMAPX;
                fResize = fTrue;
            }
            if (gs.yWin < BITMAPY1) {
                gs.yWin = BITMAPY1;
                fResize = fTrue;
            } else if (gs.yWin > BITMAPY) {
                gs.yWin = BITMAPY;
                fResize = fTrue;
            }
        }

        /* If in animation mode, ensure we are in the flicker free resolution. */

        if (gs.nAnim < 0) {
            neg(gs.nAnim);
#ifdef PCG
            if (gi.nRes == gs.nResHi && !gs.fJetTrail) {
                gi.nRes = gs.nResLo;
                BeginX();
                gs.xWin = xPcScreen;
                gs.yWin = yPcScreen;
                SquareX(&gs.xWin, &gs.yWin, fFalse);
                fResize = fTrue;
            }
#endif
        }

        /* Physically resize window if we've changed the size parameters. */

        if (fResize) {
            fResize = fFalse;
#ifdef X11
            XResizeWindow(gi.disp, gi.wind, gs.xWin, gs.yWin);
            XFreePixmap(gi.disp, gi.pmap);
            gi.pmap = XCreatePixmap(gi.disp, gi.wind, gs.xWin, gs.yWin, gi.depth);
#endif
#ifdef PCG
            if (xPcScreen > gs.xWin)
                gi.xOffset = (xPcScreen - gs.xWin) / 2;
            else {
                if (xcorner % 3 == 1)
                    gi.xOffset = 0;
                else if (xcorner % 3 == 0)
                    gi.xOffset = -gs.xWin + xPcScreen;
                else
                    gi.xOffset = -(gs.xWin - xPcScreen) / 2;
            }
            if (yPcScreen > gs.yWin)
                gi.yOffset = (yPcScreen - gs.yWin) / 2;
            else {
                if (xcorner > 6)
                    gi.yOffset = 0;
                else if (xcorner < 4)
                    gi.yOffset = -gs.yWin + yPcScreen;
                else
                    gi.yOffset = -(gs.yWin - yPcScreen) / 2;
            }
#endif
#ifdef MACG
            SizeWindow(gi.wpAst, gs.xWin, gs.yWin, fTrue);
#endif
            fRedraw = fTrue;
        }

        /* Recast chart if the chart information has changed any. */

        if (fCast) {
            fCast = fFalse;
            if (!us.fProgress || us.nRel)
                ciCore = ciMain;
            if (us.nRel)
                CastRelation();
            else
                CastChart(fTrue);
            fRedraw = fTrue;
        }
        if (gs.nAnim && !fPause)
            fRedraw = fTrue;

        /* Update the screen if anything has changed since last time around. */

        if (fRedraw && (!fPause || gs.nAnim)) {
            fRedraw = fFalse;

            /* If we're in animation mode, change the chart info appropriately. */

            if (gs.nAnim && !fPause)
#ifdef KDE
            {
                dir = getdir();
#endif
                Animate(gs.nAnim, dir);
#ifdef KDE
            }
#endif

            /* Clear the screen and set up a buffer to draw in. */


#ifdef X11
            XFillRectangle(gi.disp, gi.pmap, gi.pmgc, 0, 0, gs.xWin, gs.yWin);
#endif
#ifdef PCG
#ifdef MOUSE
            MouseShow(fFalse);
#endif
#ifdef MSG
            if (gi.cfg.numvideopages > 1)
                _setactivepage(_getactivepage() == gs.fJetTrail);
#else
            if (gi.nPages > 1) {
                gi.nPageCur = (gi.nPageCur == gs.fJetTrail);
                setactivepage(gi.nPageCur);
            }
#endif
#endif /* PCG */
#ifdef MACG
            SetPort(gi.wpAst);
            InvalRect(&gi.wpAst->portRect);
            BeginUpdate(gi.wpAst);
            EraseRect(&gi.wpAst->portRect);
#endif

            DrawChartX();

            /* Make the drawn chart visible in the current screen buffer. */

#ifdef X11
            XSync(gi.disp, 0);
            XCopyArea(gi.disp, gi.pmap, gi.wind, gi.gc,
                      0, 0, gs.xWin, gs.yWin, 0, 0);
#endif
#ifdef PCG
#ifdef MSG
            if (gi.cfg.numvideopages > 1)
                _setvisualpage(_getactivepage());
#else
            if (gi.nPages > 1)
                setvisualpage(gi.nPageCur);
#endif
#ifdef MOUSE
            if (!gs.nAnim || fPause)
                MouseShow(fTrue);
#endif
#endif /* PCG */
#ifdef MACG
            EndUpdate(gi.wpAst);
#endif
        }  /* if */

        /* Now process what's on the event queue, i.e. any keys pressed, etc. */

#ifdef X11
        if (XEventsQueued(gi.disp, QueuedAfterFlush /*QueuedAfterReading*/) ||
                !gs.nAnim || fPause) {
            XNextEvent(gi.disp, &xevent);

            /* Restore what's on window if a part of it gets uncovered. */

            if (xevent.type == Expose && xevent.xexpose.count == 0) {
                XSync(gi.disp, 0);
                XCopyArea(gi.disp, gi.pmap, gi.wind, gi.gc,
                          0, 0, gs.xWin, gs.yWin, 0, 0);
            }
            switch (xevent.type) {

                /* Check for a manual resize of window by user. */

            case ConfigureNotify:
                gs.xWin = xevent.xconfigure.width;
                gs.yWin = xevent.xconfigure.height;
                XFreePixmap(gi.disp, gi.pmap);
                gi.pmap = XCreatePixmap(gi.disp, gi.wind, gs.xWin, gs.yWin, gi.depth);
                fRedraw = fTrue;
                break;
            case MappingNotify:
                XRefreshKeyboardMapping((XMappingEvent *)&xevent);
                break;

#ifdef MOUSE
                /* Process any mouse buttons the user pressed. */

            case ButtonPress:
                mousex = xevent.xbutton.x;
                mousey = xevent.xbutton.y;
                if (xevent.xbutton.button == Button1) {
                    DrawColor(gi.kiLite);
                    DrawPoint(mousex, mousey);
                    XSync(gi.disp, 0);
                    XCopyArea(gi.disp, gi.pmap, gi.wind, gi.gc,
                              0, 0, gs.xWin, gs.yWin, 0, 0);
                } else if (xevent.xbutton.button == Button2 && (gi.nMode ==
                           gAstroGraph || gi.nMode == gWorldMap) && gs.nRot == 0) {
                    Lon = DegToDec(rDegHalf -
                                   (real)(xevent.xbutton.x-1)/(real)(gs.xWin-2)*rDegMax);
                    Lat = DegToDec(rDegQuad -
                                   (real)(xevent.xbutton.y-1)/(real)(gs.yWin-2)*181.0);
                    sprintf(sz, "Mouse is at %s.", SzLocation(Lon, Lat));
                    PrintNotice(sz);
                } else if (xevent.xbutton.button == Button3)
                    fBreak = fTrue;
                break;

                /* Check for user dragging any of the mouse buttons across window. */

            case MotionNotify:
                DrawColor(coldrw);
                DrawLine(mousex, mousey, xevent.xbutton.x, xevent.xbutton.y);
                XSync(gi.disp, 0);
                XCopyArea(gi.disp, gi.pmap, gi.wind, gi.gc,
                          0, 0, gs.xWin, gs.yWin, 0, 0);
                mousex = xevent.xbutton.x;
                mousey = xevent.xbutton.y;
                break;
#endif

                /* Process any keys user pressed in window. */

            case KeyPress:
                length = XLookupString((XKeyEvent *)&xevent, xkey, 10, &keysym, 0);
                if (length == 1) {
                    key = xkey[0];
#endif /* X11 */

#ifdef PCG
#ifdef MOUSE
                    if ((!gs.nAnim || fPause) && MouseStatus(&eventx, &eventy, &eventbtn)) {

                        /* If the left button is down, draw on the screen. */
                        if (eventbtn == mfLeft && mousex >= 0) {
                            MouseShow(fFalse);
                            DrawColor(coldrw);
                            PcMoveTo(mousex, mousey);
                            buttonx = eventx;
                            buttony = eventy;
                            PcLineTo(buttonx, buttony);

                            /* If the right button is down, change the default location. */
                        } else if (eventbtn == mfRight) {
                            if (fMap && gs.nRot == 0 && !gs.fConstel && !gs.fMollewide) {
                                Lon = DegToDec(rDegHalf-(real)(eventx-gi.xOffset)/
                                               (real)gs.xWin*rDegMax);
                                if (Lon < -rDegHalf)
                                    Lon = -rDegHalf;
                                else if (Lon > rDegHalf)
                                    Lon = rDegHalf;
                                Lat = DegToDec(rDegQuad-(real)(eventy-gi.yOffset)/
                                               (real)gs.yWin*181.0);
                                if (Lat < -rDegQuad)
                                    Lat = -rDegQuad;
                                else if (Lat > rDegQuad)
                                    Lat = rDegQuad;
                                fCast = fTrue;

                                /* Right button means draw lines if not in a world map mode. */
                            } else if (buttonx >= 0) {
                                MouseShow(fFalse);
                                DrawColor(coldrw);
                                PcMoveTo(buttonx, buttony);
                                PcLineTo(eventx, eventy);
                            }

                            /* Middle button (which most PC's don't have) means exit program. */
                        } else if (eventbtn == mfMiddle)
                            fBreak = fTrue;

                        mousex = eventx;
                        mousey = eventy;
                        MouseShow(fTrue);
                    } else
#endif /* MOUSE */
                        if (kbhit()) {
                            key = getch();
#endif /* PCG */

#ifdef MACG
                            HiliteMenu(0);
                            SystemTask();
                            fEvent = GetNextEvent(everyEvent, &erCur);
                            if (fEvent) {
                                switch (erCur.what) {
                                case mouseDown:
                                    wc = FindWindow(erCur.where, &wpCur);
                                    switch (wc) {
                                    case inSysWindow:
                                        SystemClick(&erCur, wpCur);
                                        break;
                                    case inMenuBar:
                                        MenuSelect(erCur.where);
                                        break;
                                    case inDrag:
                                        if (wpCur == gi.wpAst)
                                            DragWindow(gi.wpAst, erCur.where, &gi.rcDrag);
                                        break;
                                    case inContent:
                                        if (wpCur == gi.wpAst && wpCur != FrontWindow())
                                            SelectWindow(gi.wpAst);
                                        break;
                                    case inGoAway:
                                        if (wpCur == gi.wpAst && TrackGoAway(gi.wpAst, erCur.where))
                                            HideWindow(gi.wpAst);
                                        break;
                                    }
                                    break;
                                case updateEvt:
                                    /*fRedraw = fTrue;*/
                                    break;
                                case keyDown:
                                case autoKey:
                                    key = (byte)(erCur.message & charCodeMask);
#endif /* MACG */
#ifdef KDE
                                    if (!key)
                                        break;
                                    key = getkey();
                                    if (key)
                                    {
#endif

#ifdef PCG
LSwitch:
#endif
                                        switch (key) {
#ifdef PCG
                                        case chNull:
                                            key = NFromAltN(getch());
                                            goto LSwitch;
#endif
                                        case ' ':
                                            fRedraw = fTrue;
                                            break;
                                        case 'p':
                                            not2(fPause);
                                            break;
                                        case 'r':
                                            neg(dir);
                                            break;
                                        case 'x':
                                            not2(gs.fInverse);
                                            InitColorsX();
                                            fRedraw = fTrue;
                                            break;
                                        case 'm':
                                            if (!gi.fMono) {
                                                not2(gs.fColor);
#ifdef MSG
                                                _getvideoconfig((struct videoconfig far *) &gi.cfg);
#endif
                                                InitColorsX();
                                                fRedraw = fTrue;
                                            }
                                            break;
                                        case 'B':
#ifdef X11
                                            XSetWindowBackgroundPixmap(gi.disp, gi.root, gi.pmap);
                                            XClearWindow(gi.disp, gi.root);
#endif
#ifdef PCG
                                            gs.xWin = xPcScreen;
                                            gs.yWin = yPcScreen;
                                            SquareX(&gs.xWin, &gs.yWin, fFalse);
                                            fResize = fTrue;
#endif
                                            break;
                                        case 't':
                                            not2(gs.fText);
                                            fRedraw = fTrue;
                                            break;
                                        case 'i':
                                            not2(gs.fAlt);
                                            fRedraw = fTrue;
                                            break;
                                        case 'I':
                                            not2(InfoFlag);
                                            fRedraw = fTrue;
                                            break;
                                        case 'b':
                                            not2(gs.fBorder);
                                            fRedraw = fTrue;
                                            break;
                                        case 'l':
                                            not2(gs.fLabel);
                                            fRedraw = fTrue;
                                            break;
                                        case 'j':
                                            not2(gs.fJetTrail);
                                            break;
                                        case '<':
                                            if (gs.nScale > 100) {
                                                gs.nScale -= 100;
                                                fResize = fTrue;
                                            }
                                            break;
                                        case '>':
                                            if (gs.nScale < MAXSCALE) {
                                                gs.nScale += 100;
                                                fResize = fTrue;
                                            }
                                            break;
                                        case '[':
                                            if (gi.nMode == gGlobe && gs.rTilt > -rDegQuad) {
                                                gs.rTilt = gs.rTilt > -rDegQuad ? gs.rTilt-TILTSTEP : -rDegQuad;
                                                fRedraw = fTrue;
                                            }
                                            break;
                                        case ']':
                                            if (gi.nMode == gGlobe && gs.rTilt < rDegQuad) {
                                                gs.rTilt = gs.rTilt < rDegQuad ? gs.rTilt+TILTSTEP : rDegQuad;
                                                fRedraw = fTrue;
                                            }
                                            break;
                                        case 'Q':
                                            SquareX(&gs.xWin, &gs.yWin, fTrue);
                                            fResize = fTrue;
                                            break;
                                        case 'R':
                                            for (i = oChi; i <= oVes; i++)
                                                not2(ignore[i]);
                                            for (i = oLil; i <= oApo; i++)
                                                not2(ignore[i]);
                                            fCast = fTrue;
                                            break;
                                        case 'w':
                                            for (i = oEar; i <= oPlu; i++)
                                                not2(ignore[i]);
                                            for (i = oNod; i <= oNod; i++)
                                                not2(ignore[i]);
                                            fCast = fTrue;
                                            break;
                                        case 'C':
                                            not2(us.fCusp);
                                            for (i = cuspLo; i <= cuspHi; i++)
                                                ignore[i] = !us.fCusp || !ignore[i];
                                            fCast = fTrue;
                                            break;
                                        case 'u':
                                            not2(us.fUranian);
                                            for (i = uranLo; i <= uranHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
#ifdef ASTEROIDS
                                        case '1':
                                            not2(us.fUranian);
                                            for (i = AtenLo; i <= AtenHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
                                        case '2':
                                            not2(us.fUranian);
                                            for (i = ApolLo; i <= ApolHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
                                        case '3':
                                            not2(us.fUranian);
                                            for (i = AmorLo; i <= AmorHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
                                        case '4':
                                            not2(us.fUranian);
                                            for (i = CentLo; i <= CentHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
                                        case '5':
                                            not2(us.fUranian);
                                            for (i = TrneLo; i <= TrneHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
                                        case '6':
                                            not2(us.fUranian);
                                            for (i = AgenLo; i <= AgenHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
                                        case '7':
                                            not2(us.fUranian);
                                            for (i = MuseLo; i <= MuseHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
#endif
                                        case '8':
                                            not2(us.fUranian);
                                            for (i = HypuranLo; i <= HypuranHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
                                        case '9':
                                            if (AsteLo!=AsteHi){
                                                not2(us.fUranian);
                                                for (i = AsteLo+us.nExtraStar; i <= AsteHi; i++) {
                                                    ignore[i] = !us.fUranian || !ignore[i];
                                                    ignore2[i] = !us.fUranian || !ignore2[i];
                                                }
                                                fCast = fTrue;
                                            }
                                            break;
#ifdef ASTEROIDS
                                        case chSec:
                                            not2(us.fUranian);
                                            for (i = RiyaLo; i <= RiyaHi; i++) {
                                                ignore[i] = !us.fUranian || !ignore[i];
                                                ignore2[i] = !us.fUranian || !ignore2[i];
                                            }
                                            fCast = fTrue;
                                            break;
#endif
                                        case 'U':
                                            us.nStar = !us.nStar;
                                            for (i = starLo; i <= ((us.nStar && !us.fRiyalAst) ? starLo+cStar-1: starHi); i++) {
                                                ignore[i] = !us.nStar || !ignore[i];
                                                ignore2[i] = !us.nStar || !ignore2[i];
                                                ignore3[i] = fTrue;
                                            }
                                            fCast = fTrue;
                                            break;
#ifdef PLACALC
                                        case '/':
                                            if (us.objAst) {
                                                us.objAst=NextAsteroid(us.objAst);
                                                ignore[oVtx]=0;
                                                SetAstName(us.objAst);
                                            }
                                            else
                                            {
                                                us.objNode++;
                                                if (us.objNode==oNod)
                                                    us.objNode=CentLo;
                                                if (us.objNode==AtenHi)
                                                    us.objNode=oEar;
                                                SetNodeNames(us.objNode);
                                            }
                                            fCast = fTrue;
                                            break;
                                        case '\\':
                                            if (us.objAst) {
                                                PrevAsteroid();
                                                ignore[oVtx]=0;
                                                SetAstName(us.objAst);
                                            }
                                            else
                                            {
                                                us.objNode--;
                                                if (us.objNode==TerranHi)
                                                    us.objNode=oVes;
                                                if (us.objNode==-1)
                                                    us.objNode=AtenHi;
                                                SetNodeNames(us.objNode);
                                            }
                                            fCast = fTrue;
                                            break;
#endif
                                        case 'c':
                                            us.nRel = us.nRel ? 0 : rcDual;
                                            fCast = fTrue;
                                            break;
                                        case 's':
                                            not2(us.fSidereal);
                                            fCast = fTrue;
                                            break;
                                        case 'h':
                                            not2(us.objCenter);
                                            fCast = fTrue;
                                            break;
                                        case 'f':
                                            not2(us.fFlip);
                                            fCast = fTrue;
                                            break;
                                        case 'g':
                                            not2(us.fDecan);
                                            fCast = fTrue;
                                            break;
                                        case 'z':
                                            not2(us.fVedic);
                                            fRedraw = fTrue;
                                            break;
                                        case 'y':
                                            not2(us.fNavamsa);
                                            fCast = fTrue;
                                            break;
                                        case '+':
                                            fEclipseBackward=fFalse;
                                            fReturnChartCast=fTrue;
                                            Animate(gs.nAnim, abs(dir));
                                            fCast = fTrue;
                                            break;
                                        case '-':
                                            fEclipseBackward=fTrue;
                                            fReturnChartCast=fTrue;
                                            Animate(gs.nAnim, -abs(dir));
                                            fCast = fTrue;
                                            break;
                                        case 'o':
                                            ciSave = ciMain;
                                            break;
                                        case 'O':
                                            ciMain = ciSave;
                                            fCast = fTrue;
                                            break;
#ifdef TIME
                                        case 'n':
                                            FInputData(szNowCore);
                                            ciMain = ciCore;
                                            fCast = fTrue;
                                            break;
#endif
                                        case 'N':                     /* The continuous update animation. */
                                            gs.nAnim = gs.nAnim ? 0 :10 ;
                                            break;

                                            /* These are the nine different "add time to chart" animations. */
                                        case '!':
                                            gs.nAnim = -1;
                                            break;
                                        case '@':
                                            gs.nAnim = -2;
                                            break;
                                        case '#':
                                            gs.nAnim = -3;
                                            break;
                                        case '$':
                                            gs.nAnim = -4;
                                            break;
                                        case '%':
                                            gs.nAnim = -5;
                                            break;
                                        case '^':
                                            gs.nAnim = -6;
                                            break;
                                        case '&':
                                            gs.nAnim = -7;
                                            break;
                                        case '*':
                                            gs.nAnim = -8;
                                            break;
                                        case '(':
                                            gs.nAnim = -9;
                                            break;

                                            /* Should we go switch to a new chart type? */
                                        case 'V':
                                            gi.nMode = gWheel;
                                            fRedraw = fTrue;
                                            break;
                                        case 'A':
                                            gi.nMode = gGrid;
                                            fRedraw = fTrue;
                                            break;
                                        case 'Z':
                                            gi.nMode = gHorizon;
                                            fRedraw = fTrue;
                                            break;
                                        case 'S':
                                            gi.nMode = gOrbit;
                                            fRedraw = fTrue;
                                            break;
                                        case 'M':
                                            gi.nMode = gSector;
                                            us.fSector=fTrue;
                                            fCast = fTrue;
                                            break;
                                        case 'K':
                                            gi.nMode = gCalendar;
                                            fRedraw = fTrue;
                                            break;
                                        case 'J':
                                            gi.nMode = gDisposit;
                                            fRedraw = fTrue;
                                            break;
                                        case 'L':
                                            gi.nMode = gAstroGraph;
                                            fRedraw = fTrue;
                                            break;
                                        case 'E':
                                            gi.nMode = gEphemeris;
                                            fRedraw = fTrue;
                                            break;
                                        case 'W':
                                            gi.nMode = gWorldMap;
                                            fRedraw = fTrue;
                                            break;
                                        case 'G':
                                            gi.nMode = gGlobe;
                                            fRedraw = fTrue;
                                            break;
                                        case 'P':
                                            gi.nMode = gPolar;
                                            fRedraw = fTrue;
                                            break;
#ifdef BIORHYTHM
                                        case 'Y':            /* Should we switch to biorhythm chart? */
                                            us.nRel = rcBiorhythm;
                                            gi.nMode = gBiorhythm;
                                            fCast = fTrue;
                                            break;
#endif
#ifdef CONSTEL
                                        case 'F':
                                            if (!fMap && gi.nMode != gGlobe && gi.nMode != gPolar)
                                                gi.nMode = gWorldMap;
                                            not2(gs.fConstel);
                                            fRedraw = fTrue;
                                            break;
#endif
                                        case '0':
                                            not2(us.fPrimeVert);
                                            not2(us.fCalendarYear);
                                            not2(us.nEphemYears);
                                            not2(gs.fMollewide);
                                            gi.nMode = (gi.nMode == gWheel ? gHouse :
                                                        (gi.nMode == gHouse ? gWheel : gi.nMode));
                                            fRedraw = fTrue;
                                            break;
                                        case 'v':
                                        case 'H':
                                        case '?':
#ifdef MSG
                                            _setvideomode(_DEFAULTMODE);
                                            if (key != 'v')
                                                _settextrows(50);
#endif
#ifdef BGI
                                            restorecrtmode();
                                            if (key != 'v')
                                                textmode(C4350);
#endif
                                            length = us.nScrollRow;
                                            us.nScrollRow = 0;
                                            if (key == 'v')
                                                ChartListing();
                                            else
                                                DisplayKeysX();
                                            us.nScrollRow = length;
#ifdef PCG
                                            while (!kbhit())
                                                ;
                                            key = getch();
                                            if (key == 'q' || key == chEscape || key == chBreak) {
                                                fBreak = fTrue;
                                                break;
                                            }
                                            BeginX();
                                            fResize = fTrue;
#endif
                                            break;
                                        case chReturn:
                                            CommandLineX();
                                            fResize = fCast = fTrue;
                                            break;
#ifdef PCG
                                        case chTab:
                                            if (gi.nRes == gs.nResHi)
                                                gi.nRes = gs.nResLo;
                                            else
                                                gi.nRes = gs.nResHi;
                                            BeginX();
                                            gs.xWin = xPcScreen;
                                            gs.yWin = yPcScreen;
                                            SquareX(&gs.xWin, &gs.yWin, fFalse);
                                            fResize = fTrue;
                                            break;
#endif
                                        case chDelete:
#ifdef PCG
#ifdef MOUSE
                                            MouseShow(fFalse);
#endif
#endif /* PCG */
                                            fT = gs.fJetTrail;
                                            gs.fJetTrail = fFalse;
                                            DrawClearScreen();
                                            gs.fJetTrail = fT;
                                            break;
#ifdef MOUSE
                                        case 'z'-'`':
                                            coldrw = kBlack;
                                            break;
                                        case 'e'-'`':
                                            coldrw = kMaroon;
                                            break;
                                        case 'f'-'`':
                                            coldrw = kDkGreen;
                                            break;
                                        case 'o'-'`':
                                            coldrw = kOrange;
                                            break;
                                        case 'n'-'`':
                                            coldrw = kDkBlue;
                                            break;
                                        case 'u'-'`':
                                            coldrw = kPurple;
                                            break;
                                        case 'k'-'`':
                                            coldrw = kDkCyan;
                                            break;
                                        case 'l'-'`':
                                            coldrw = kLtGray;
                                            break;
                                        case 'd'-'`':
                                            coldrw = kDkGray;
                                            break;
                                        case 'r'-'`':
                                            coldrw = kRed;
                                            break;
                                        case 'g'-'`':
                                            coldrw = kGreen;
                                            break;
                                        case 'y'-'`':
                                            coldrw = kYellow;
                                            break;
                                        case 'b'-'`':
                                            coldrw = kBlue;
                                            break;
                                        case 'v'-'`':
                                            coldrw = kMagenta;
                                            break;
                                        case 'j'-'`':
                                            coldrw = kCyan;
                                            break;
                                        case 'a'-'`':
                                            coldrw = kWhite;
                                            break;
#ifdef PCG
                                        case 't'-'`':
                                            MouseShow(fFalse);
                                            if (buttonx >= 0)
#ifdef MSG
                                                _rectangle(_GBORDER, buttonx, buttony, mousex, mousey);
#else
                                                DrawEdge(Min(buttonx, mousex) - gi.xOffset,
                                                         Min(buttony, mousey) - gi.yOffset,
                                                         Max(mousex, buttonx) - gi.xOffset,
                                                         Max(mousey, buttony) - gi.yOffset);
#endif
                                            MouseShow(fTrue);
                                            break;
                                        case 'x'-'`':
                                            MouseShow(fFalse);
                                            if (buttonx >= 0)
#ifdef MSG
                                                _ellipse(_GBORDER, buttonx, buttony, mousex, mousey);
#else
                                                DrawEllipse(Min(buttonx, mousex) - gi.xOffset,
                                                            Min(buttony, mousey) - gi.yOffset,
                                                            Max(mousex, buttonx) - gi.xOffset,
                                                            Max(mousey, buttony) - gi.yOffset);
#endif
                                            MouseShow(fTrue);
                                            break;
#endif /* PCG */
#endif /* MOUSE */
                                        case 'q':
                                        case chEscape:
                                        case chBreak:
                                            fBreak = fTrue;
#ifdef KDE
                                            KEnd();
#endif
                                            break;
                                        default:
                                            if (key > '0' && key <= '9') {
#ifdef PCG
                                                if (gs.nAnim && !fPause)
#endif
                                                    /* Process numbers 1..9 signifying animation rate. */
                                                    dir = (dir > 0 ? 1 : -1)*(key-'0');
#ifdef PCG
                                                else {
                                                    /* If we aren't in animation mode, then 1..9 refers to the */
                                                    /* clipping "quadrant" to use if chart size > screen size. */
                                                    xcorner = key-'0';
                                                    fResize = fTrue;
                                                }
#endif
                                                break;
                                            } else if (FBetween(key, 201, 248)) {
                                                is.fSzInteract = fTrue;
                                                if (szMacro[key-201]) {
                                                    FProcessCommandLine(szMacro[key-201]);
                                                    fResize = fCast = fTrue;
                                                }
                                                is.fSzInteract = fFalse;
                                                break;
                                            }
                                            putchar(chBell);    /* Any key not bound will sound a beep. */
                                        }  /* switch */
#ifdef KDE
                                        key = 0;
#endif
                                    }  /* if */
#ifdef X11
                                default:
                                    ;
                                }  /* switch */
                            }  /* if */
#endif
#ifdef MACG
                        }  /* if */
#endif
                }  /* while */
            }


            /* This is called right before program termination to get rid of the window. */

            void EndX()
            {
#ifdef X11
                XFreeGC(gi.disp, gi.gc);
                XFreeGC(gi.disp, gi.pmgc);
                XFreePixmap(gi.disp, gi.pmap);
                XDestroyWindow(gi.disp, gi.wind);
                XCloseDisplay(gi.disp);
#endif
#ifdef MSG
                _setvideomode(_DEFAULTMODE);
#endif
#ifdef BGI
                restorecrtmode();
#endif
#ifdef MACG
                DisposeWindow(gi.wpAst);
#endif
            }
#endif /* ISG */
#endif /* WIN */


            /*
            ******************************************************************************
            ** Main Graphics Processing.
            ******************************************************************************
            */

            /* Process one command line switch passed to the program dealing with the    */
            /* graphics features. This is just like the processing of each switch in the */
            /* main program, however here each switch has been prefixed with an 'X'.     */

            int NProcessSwitchesX(argc, argv, pos, fOr, fAnd, fNot)
            int argc, pos;
            bool fOr, fAnd, fNot;
            char **argv;
            {
                int darg = 0, i, j;
                real rT;
                char ch1;

                ch1 = argv[0][pos+1];
                switch (argv[0][pos]) {
                case chNull:
                    break;

                case 'b':
                    if (us.fNoWrite || is.fSzInteract) {
                        ErrorArgv("Xb");
                        return tcError;
                    }
                    ch1 = ChCap(ch1);
                    if (FValidBmpmode(ch1))
                        gs.chBmpMode = ch1;
                    SwitchF2(gs.fBitmap);
                    gs.fPS = gs.fMeta = fFalse;
                    break;

#ifdef PS
                case 'p':
                    if (us.fNoWrite || is.fSzInteract) {
                        ErrorArgv("Xp");
                        return tcError;
                    }
                    gs.fPS = fTrue + (ch1 != '0');
                    gs.fBitmap = gs.fMeta = fFalse;
                    break;
#endif

#ifdef META
                case 'M':
                    if (us.fNoWrite || is.fSzInteract) {
                        ErrorArgv("XM");
                        return tcError;
                    }
                    if (ch1 == '0')
                        SwitchF(gs.fFont);
                    SwitchF2(gs.fMeta);
                    gs.fBitmap = gs.fPS = fFalse;
                    break;
#endif

                case 'o':
                    if (us.fNoWrite || is.fSzInteract) {
                        ErrorArgv("Xo");
                        return tcError;
                    }
                    if (argc <= 1) {
                        ErrorArgc("Xo");
                        return tcError;
                    }
                    if (!gs.fBitmap && !gs.fPS && !gs.fMeta)
                        gs.fBitmap = fTrue;
                    gi.szFileOut = SzPersist(argv[1]);
                    darg++;
                    break;

#ifdef X11
                case 'B':
                    if (is.fSzInteract) {
                        ErrorArgv("XB");
                        return tcError;
                    }
                    SwitchF(gs.fRoot);
                    break;
#endif

                case 'a':
                    if (argc <= 1) {
                        ErrorArgc("Xa");
                        return tcError;
                    }
                    i = atoi(argv[1]);
                    if (i < -1 || i > 2) {
                        ErrorValN("Xa", i);
                        return tcError;
                    }
                    TensChart = i;
                    darg++;
                    break;

                case 'm':
                    SwitchF(gs.fColor);
                    break;

                case 'r':
                    SwitchF(gs.fInverse);
                    break;

                case 'w':
                    if (argc <= 1) {
                        ErrorArgc("Xw");
                        return tcError;
                    }
                    i = atoi(argv[1]);
                    if (argc > 2 && ((j = atoi(argv[2])) || argv[2][0] == '0')) {
                        argc--;
                        argv++;
                        darg++;
                    } else
                        j = i;
                    if (!FValidGraphx(i)) {
                        ErrorValN("Xw", i);
                        return tcError;
                    }
                    if (!FValidGraphy(j)) {
                        ErrorValN("Xw", j);
                        return tcError;
                    }
                    gs.xWin = i;
                    gs.yWin = j;
                    darg++;
                    break;

                case 's':
                    if (argc <= 1) {
                        ErrorArgc("Xs");
                        return tcError;
                    }
                    i = atoi(argv[1]);
                    if (i < 100)
                        i *= 100;
                    if (!FValidScale(i)) {
                        ErrorValN("Xs", i);
                        return tcError;
                    }
                    gs.nScale = i;
                    darg++;
                    break;

                case 'i':
                    SwitchF(gs.fAlt);
                    break;

                case 'I':
                    SwitchF(InfoFlag);
                    break;

                case 't':
                    SwitchF(gs.fText);
                    break;

                case 'u':
                    SwitchF(gs.fBorder);
                    break;

                case 'l':
                    SwitchF(gs.fLabel);
                    break;

                case 'j':
                    SwitchF(gs.fJetTrail);
                    break;

                case '1':
                    if (argc <= 1) {
                        ErrorArgc("X1");
                        return tcError;
                    }
                    i = NParseSz(argv[1], pmObject);
                    if (!FItem(i)) {
                        ErrorValN("X1", i);
                        return tcError;
                    }
                    gs.objLeft = i;
                    darg++;
                    break;

                case '2':
                    if (argc <= 1) {
                        ErrorArgc("X2");
                        return tcError;
                    }
                    i = NParseSz(argv[1], pmObject);
                    if (!FItem(i)) {
                        ErrorValN("X2", i);
                        return tcError;
                    }
                    gs.objLeft = -i;
                    darg++;
                    break;

#ifdef X11
                case 'd':
                    if (is.fSzInteract) {
                        ErrorArgv("Xd");
                        return tcError;
                    }
                    if (argc <= 1) {
                        ErrorArgc("Xd");
                        return tcError;
                    }
                    gs.szDisplay = SzPersist(argv[1]);
                    darg++;
                    break;
#endif

                case 'W':
                    if (argc > 1 && ((i = atoi(argv[1])) || argv[1][0] == '0')) {
                        darg++;
                        if (!FValidRotation(i)) {
                            ErrorValN("XW", i);
                            return tcError;
                        }
                        gs.nRot = i;
                    }
                    gi.nMode = gWorldMap;
                    if (ch1 == '0')
                        gs.fMollewide = fTrue;
                    is.fHaveInfo = fTrue;
                    break;

                case 'G':
                    if (argc > 1 && ((i = atoi(argv[1])) || argv[1][0] == '0')) {
                        darg++;
                        if (!FValidRotation(i)) {
                            ErrorValN("XG", i);
                            return tcError;
                        }
                        gs.nRot = i;
                        if (argc > 2 && ((rT = atof(argv[2])) || argv[2][0] == '0')) {
                            darg++;
                            if (!FValidTilt(rT)) {
                                ErrorValR("XG", rT);
                                return tcError;
                            }
                            gs.rTilt = rT;
                        }
                    }
                    gi.nMode = gGlobe;
                    is.fHaveInfo = fTrue;
                    break;

                case 'P':
                    if (argc > 1 && ((i = atoi(argv[1])) || argv[1][0] == '0')) {
                        darg++;
                        if (!FValidRotation(i)) {
                            ErrorValN("XP", i);
                            return tcError;
                        }
                    } else
                        i = 0;
                    gs.nRot = i;
                    gi.nMode = gPolar;
                    if (ch1 == '0')
                        gs.fPrintMap = fTrue;
                    is.fHaveInfo = fTrue;
                    break;

#ifdef CONSTEL
                case 'F':
                    if (!fMap && gi.nMode != gGlobe && gi.nMode != gPolar)
                        gi.nMode = gWorldMap;
                    not2(gs.fConstel);
                    is.fHaveInfo = fTrue;
                    break;
#endif

#ifdef ISG
                case 'n':
                    if (argc > 1 && (i = atoi(argv[1])))
                        darg++;
                    else
                        i = 10;
                    if (i < 1 || i > 10) {
                        ErrorValN("Xn", i);
                        return tcError;
                    }
                    gs.nAnim = i;
                    break;
#endif

                default:
                    ErrorSwitch(argv[0]);
                    return tcError;
                }
                /* 'darg' contains the value to be added to argc when we return. */
                return darg;
            }


            /* Process one command line switch passed to the program dealing with more  */
            /* obscure graphics options. This is structured very much like the function */
            /* NProcessSwitchesX(), except here we know each switch begins with 'YX'.   */

            int NProcessSwitchesRareX(argc, argv, pos)
            int argc, pos;
            char **argv;
            {
                int darg = 0, i;
                char ch1;
                real r;

#ifdef PS
                ch1 = argv[0][pos+1];
#endif
                switch (argv[0][pos]) {
                case chNull:
                    if (argc <= 2) {
                        ErrorArgc("YX");
                        return tcError;
                    }
#ifdef PCG
                    i = atoi(argv[1]);
                    if (!FValidResmode(i)) {
                        ErrorValN("YX", i);
                        return tcError;
                    }
                    gs.nResHi = i;
                    i = atoi(argv[2]);
                    if (!FValidResmode(i)) {
                        ErrorValN("YX", i);
                        return tcError;
                    }
                    gs.nResLo = i;
                    gs.fBitmap = gs.fPS = gs.fMeta = fFalse;
#endif
                    darg += 2;
                    break;

                case 'c':
                    if (argc <= 1) {
                        ErrorArgc("YXc");
                        return tcError;
                    }
                    r = atof(argv[1]);
                    if (r < 0.2 || r > 4.0) {
                        ErrorValR("YXc",r);
                        return tcError;
                    }
                    CoeffTens = r;
                    darg++;
                    break;

                case 'G':
                    if (argc <= 1) {
                        ErrorArgc("YXG");
                        return tcError;
                    }
                    i = atoi(argv[1]);
                    if (!FValidGlyphs(i)) {
                        ErrorValN("YXg", i);
                        return tcError;
                    }
                    gs.nGlyphs = i;
                    darg++;
                    break;

                case 'g':
                    if (argc <= 1) {
                        ErrorArgc("YXg");
                        return tcError;
                    }
                    i = atoi(argv[1]);
                    if (!FValidGrid(i)) {
                        ErrorValN("YXg", i);
                        return tcError;
                    }
                    gs.nGridCell = i;
                    darg++;
                    break;

                case 'f':
                    if (argc <= 1) {
                        ErrorArgc("YXf");
                        return tcError;
                    }
                    gs.fFont = atoi(argv[1]);
                    darg++;
                    break;

#ifdef PS
                case 'p':
                    if (ch1 == '0') {
                        if (argc <= 2) {
                            ErrorArgc("YXp0");
                            return tcError;
                        }
                        gs.xInch = atof(argv[1]);
                        gs.yInch = atof(argv[2]);
                        darg += 2;
                        break;
                    }
                    if (argc <= 1) {
                        ErrorArgc("YXp");
                        return tcError;
                    }
                    gs.nOrient = atoi(argv[1]);
                    darg++;
                    break;
#endif

                default:
                    ErrorSwitch(argv[0]);
                    return tcError;
                }
                /* 'darg' contains the value to be added to argc when we return. */
                return darg;
            }


            /* This is the main interface to all the graphics features. This routine     */
            /* is called from the main program if any of the -X switches were specified, */
            /* and it sets up for and goes and generates the appropriate graphics chart. */
            /* We return fTrue if successfull, fFalse if some non-fatal error occurred.  */

            bool FActionX()
            {
                gi.fFile = (gs.fBitmap || gs.fPS || gs.fMeta);
#ifdef PS
                gi.fEps = gs.fPS > fTrue;
#endif
                /* First figure out what graphic mode to generate the chart in, based on */
                /* various non-X command switches, e.g. -L combined with -X, -g combined */
                /* with -X, and so on, and determine the size the window is to be, too.  */
                if (gi.nMode == 0) {
                    if (us.fWheel)
                        gi.nMode = gHouse;
                    else if (us.fGrid || us.fMidpoint)
                        gi.nMode = gGrid;
                    else if (us.fHorizon)
                        gi.nMode = gHorizon;
                    else if (us.fOrbit)
                        gi.nMode = gOrbit;
                    else if (us.fSector && !fSynodicReturnChart)
                        gi.nMode = gSector;
                    else if (us.fInfluence)
                        gi.nMode = gDisposit;
                    else if (us.fAstroGraph)
                        gi.nMode = gAstroGraph;
                    else if (us.fCalendar)
                        gi.nMode = gCalendar;
                    else if (us.fEphemeris)
                        gi.nMode = gEphemeris;
                    else if (us.nRel == rcBiorhythm)
                        gi.nMode = gBiorhythm;
                    else
                        gi.nMode = gWheel;
                }
                if (gi.nMode == gGrid) {
                    if (us.nRel <= rcDual && us.fMidpoint && !us.fAspList)
                        us.fGridConfig = fTrue;
                    gs.xWin = gs.yWin =
                                  (gs.nGridCell + (us.nRel <= rcDual))*CELLSIZE*gi.nScale + 1;
                } else if (fMap) {
                    gs.xWin = nDegMax*gi.nScale;
                    gs.yWin = nDegHalf*gi.nScale;
                }
#ifdef WIN
                if (fSidebar || us.fInfluence)
                    gs.xWin -= SIDESIZE;
#endif
#ifdef KDE
                if (fSidebar && !gi.fiskde)
                    gs.xWin -= SIDESIZE;
#endif

                gi.nScaleT = gs.fPS ? PSMUL : (gs.fMeta ? METAMUL : 1);
#ifdef WIN
                if (wi.hdcPrint != hdcNil)
                    gi.nScaleT = METAMUL;
#endif
                if (gi.fFile) {
#ifdef KDE
                    gi.fiskde = fFalse;
#endif

                    if (gs.xWin == 0)
                        gs.xWin = DEFAULTX;
                    if (gs.yWin == 0)
                        gs.yWin = DEFAULTY;
                    if (fSidebar || us.fInfluence)
                        gs.xWin += SIDESIZE;
                    if (gs.xWin > BITMAPX)
                        gs.xWin = BITMAPX;
                    if (gs.yWin > BITMAPY)
                        gs.yWin = BITMAPY;
                    BeginFileX();
                    if (gs.fBitmap) {
                        gi.cbBmpRow = (gs.xWin + 1) >> 1;
                        gi.yBand = gs.yWin;
                        if (!FEnsureGrid())
                            return fFalse;
                        while ((gi.bm = PAllocate((long)gi.cbBmpRow * gi.yBand, fTrue, NULL)) ==
                                NULL) {
                            PrintWarning("The bitmap must be generated in multiple stages.");
                            gi.yBand = (gi.yBand + 1) / 2;
                            if (gi.yBand < 1 || gs.chBmpMode != 'B')
                                return fFalse;
                        }
                        if (gi.yBand == gs.yWin)
                            gi.yBand = 0;
                        else {
                            gi.yOffset = gs.yWin - gs.yWin % gi.yBand;
                            if (gi.yOffset == gs.yWin)
                                gi.yOffset -= gi.yBand;
                        }
                    }
#ifdef PS
                    else if (gs.fPS)
                        PsBegin();
#endif
#ifdef META
                    else {
                        if (!FEnsureGrid())
                            return fFalse;
                        for (gi.cbMeta = MAXMETA; gi.cbMeta > 0 &&
                                (gi.bm = PAllocate(gi.cbMeta, fTrue, NULL)) == NULL;
                                gi.cbMeta -= MAXMETA/8)
                            PrintWarning("Attempting to get maximum memory for metafile.");
                        if (gi.cbMeta == 0)
                            return fFalse;
                        gs.xWin   *= METAMUL;  /* Increase chart sizes and scales behind the */
                        gs.yWin   *= METAMUL;  /* scenes to make graphics look smoother.     */
                        gs.nScale *= METAMUL;
                    }
#endif
                    InitColorsX();
                }
#ifdef ISG
                else {
#ifdef PCG
                    BeginX();
                    if (gs.xWin == 0 || gs.yWin == 0) {
                        if (gs.xWin == 0)
                            gs.xWin = xPcScreen;
                        if (gs.yWin == 0)
                            gs.yWin = yPcScreen;
                        SquareX(&gs.xWin, &gs.yWin, fFalse);
                    } else if (fSidebar || us.fInfluence)
                        gs.xWin += SIDESIZE;
#else
                    if (gs.xWin == 0 || gs.yWin == 0) {
                        if (gs.xWin == 0)
                            gs.xWin = DEFAULTX;
                        if (gs.yWin == 0)
                            gs.yWin = DEFAULTY;
                        SquareX(&gs.xWin, &gs.yWin, fFalse);
                    } else if (fSidebar || us.fInfluence)
                        gs.xWin += SIDESIZE;
                    BeginX();
#endif
                }
#endif /* ISG */
                if (gi.fFile || gs.fRoot)    /* Go draw the graphic chart. */
                    DrawChartX();
                if (gi.fFile) {    /* Write bitmap to file if in that mode. */
                    EndFileX();
                    while (gi.yBand) {
                        gi.yOffset -= gi.yBand;
                        DrawChartX();
                        EndFileX();
                    }
                    if (!gs.fPS)
                        DeallocateHuge(gi.bm);
                }
#ifdef ISG
                else {
#ifdef X11
                    if (gs.fRoot) {                                         /* Process -XB. */
                        XSetWindowBackgroundPixmap(gi.disp, gi.root, gi.pmap);
                        XClearWindow(gi.disp, gi.root);

                        /* If -Xn in effect with -XB, then enter infinite loop where we */
                        /* calculate and animate chart, displaying on the root window.  */
                        while (gs.nAnim) {
                            Animate(gs.nAnim, 1);
                            if (!gs.fJetTrail)
                                XFillRectangle(gi.disp, gi.pmap, gi.pmgc, 0, 0, gs.xWin, gs.yWin);
                            DrawChartX();
                            XSetWindowBackgroundPixmap(gi.disp, gi.root, gi.pmap);
                            XClearWindow(gi.disp, gi.root);
                        }
                    } else
#endif
#ifndef WIN
#ifdef KDE
                        KInteract();
#else
                        InteractX();    /* Window's up; process commands given to window now. */
                    EndX();
#endif
#else
                        DrawChartX();
#endif
                }
#endif /* ISG */
                return fTrue;
            }
#endif /* GRAPH */

            /* xscreen.c */
