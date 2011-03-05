/*
** Astrolog (Version 5.42J) File: gtopo.c
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

#ifdef GRAPH
void XChartEphemeris3D()
{
    real symbol[cObj*2+1], objSav[objMax],objSavAlt[objMax];
    char sz[4];
    int yea, year, unit = 6*gi.nScale, daytot, d = 1, day=1, mon=1, monsiz=0,
                          x1, y1, x2, y2, xs, ys, m, n, u, v, i, j;

    yea = us.nEphemYears;    /* Is this -Ey -X or just -E -X? */
    year = Yea;
    if (yea) {
        daytot = yea * DayInYearHi(Yea);
        day = 1;
        mon = 1;
        monsiz = 31;
    } else
        daytot = DayInMonth(Mon, Yea);
    x1 = (yea ? 30 : 24)*gi.nScaleT;
    y1 = unit*2;
    x2 = gs.xWin - x1;
    y2 = gs.yWin - y1;
    xs = x2 - x1;
    ys = y2 - y1;

    /* Display glyphs of the zodiac along the bottom axis. */

    for (i = 1; i <= cSign+1; i++) {
        m = x1 + NMultDiv(xs, i-1, 12);
        j = i > cSign ? 1 : i;
        DrawColor(kSignB(j));
        DrawSign(j, m, y2 + unit);
        DrawColor(gi.kiGray);
        DrawDash(m, y1, m, y2, 2);
    }

    /* Loop and display planet movements for one day segment. */

    while (d <= daytot + 1) {
        n = v;
        v = y1 + NMultDiv(ys, d-1, daytot);
        if (!yea || day == 1) {
            DrawColor(gi.kiGray);
            DrawDash(x1, v, x2, v, 1);    /* Marker line for day or month. */
        }
        if (d > 1)
            for (i = 1; i <= cObjOpt; i++){
                objSav[i] = planet[i];
                objSavAlt[i] = planetalt[i];
            }
        ciCore = ciMain;
        if (yea) {
            MM = mon;
            DD = day;
            YY = year;
        } else {
            MM = Mon;
            DD = d;
        }
        CastChart(fTrue);

        /* Draw planet glyphs along top of chart. */


        if (d < 2) {
            for (i = 1; i <= cObjOpt; i++) {
                j = !FProper(i) || (i == oMoo && gs.fAlt);
                symbol[i*2-1] = (j || us.nRel > rcDual) ? -rLarge : cp2.obj[i];
                symbol[i*2] = (j ? -rLarge : planet[i]);
            }
            FillSymbolLine(symbol);
            for (i = cObjOpt*2; i >= 1; i--) {
                j = (i+1) >> 1;
                if (symbol[i] >= 0.0)
                    DrawObject(j, x1 + (int)((real)xs * symbol[i] / rDegMax), unit);
            }
            if (us.nRel <= rcDual) {
                for (i = cObjOpt; i >= 1; i--) {
                    if (!FProper(i) || (i == oMoo && gs.fAlt))
                        continue;
                    j = x1 + (int)((real)xs * cp2.obj[i] / rDegMax);
                    DrawColor(kObjB[i]);
                    DrawDash(j, y1, j, y2, 1);
                }
            }

            /* Draw a line segment for each object during this time section. */

        } else
            for (i = cObjOpt; i >= 1; i--) {
                if (!FProper(i) || (i == oMoo && gs.fAlt))
                    continue;
                m = x1 + (int)((real)xs * objSav[i] / rDegMax);
                u = x1 + (int)((real)xs * planet[i] / rDegMax);
                n = v + (int)((real)ys *  planetalt[i] / rDegMax);
                DrawColor(kObjB[i]);
                DrawWrap(m, n, u, v, ret[i] > 0.0 ? -x1 : x1, x2);
            }

        /* Label months or days in the month along the left and right edges. */

        if (d <= daytot && (!yea || (yea<=5 && day == 1)||(yea > 5 && mon==1 && day==1))) {
            if (yea) {
                if ((mon!=1||yea==1) && yea<=5)
                    sprintf(sz, "%c%c%c", chMon3(mon));
                else sprintf(sz, "%d", year);
                i = (mon == Mon);
            } else {
                sprintf(sz, "%2d", d);
                i = (d == Day);
            }
            DrawColor(i ? gi.kiOn : gi.kiLite);
            DrawSz(sz,     xFont   *gi.nScaleT, v + (yFont-2)*gi.nScaleT,
                   dtLeft | dtBottom);
            DrawSz(sz, x2+(xFont-1)*gi.nScaleT, v + (yFont-2)*gi.nScaleT,
                   dtLeft | dtBottom);
        }

        /* Now increment the day counter. For a month we always go up by one. */
        /* For a year we go up by four or until the end of the month reached. */

        if (yea) {
            i = yea * (us.fSeconds ? 2 : 4);
            day += i;
            if (day > monsiz) {
                d += i-(day-monsiz-1);
                if (d <= daytot + 1) {
                    mon++;
                    if (mon>12){
                        mon=1;
                        year++;
                    }
                    monsiz = DayInMonth(mon, Yea);
                    day = 1;
                }
            } else
                d += i;
        } else
            d++;
    }
    DrawColor(gi.kiLite);
    DrawEdge(x1, y1, x2, y2);

    ciCore = ciMain;    /* Recast original chart. */
    CastChart(fTrue);
}

#endif

