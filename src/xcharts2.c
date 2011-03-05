/*
** Astrolog (Version 5.42J) File: xcharts2.c
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
** Chart Graphics Utility Procedures.
******************************************************************************
*/

/* Return whether the specified object should be displayed in the current */
/* graphics chart type. For example, don't include the Moon in the solar  */
/* system charts when Placalc is off, don't include house cusps in        */
/* astro-graph charts, and so on, in addition to checking restrictions.   */

bool FProper(i)
int i;
{
    bool f;

    f = !ignore[i] && (gi.nMode == gOrbit || i != us.objCenter);
    if (gi.nMode == gHorizon || fMap || gi.nMode == gGlobe ||
            gi.nMode == gPolar)                      /* Horizon and map charts */
        f &= FObject(i);
    else if (gi.nMode == gOrbit)               /* Solar system charts */
        f &= FObject(i) && (i != oMoo || us.fPlacalc);
    else if (gi.nMode == gSector || gi.nMode == gEphemeris)
        f &= (FAst(i)||FThing(i));                          /* Sector amd ephemeris charts */
    return f;
}


/* Adjust an array of zodiac positions so that no two positions are within   */
/* a certain orb of each other. This is used by the wheel drawing chart      */
/* routines in order to make sure that we don't draw any planet glyphs on    */
/* top of each other. We'll later draw the glyphs at the adjusted positions. */

void FillSymbolRing(symbol, factor)
real *symbol;
real factor;
{
    real orb = DEFORB*256.0/(real)gs.yWin*(real)gi.nScale*factor, k1, k2, temp;
    int i, j, k = 1, l;

    /* Keep adjusting as long as we can still make changes, or until we do 'n' */
    /* rounds. (With many objects, there just may not be enough room for all.) */

    for (l = 0; k && l < us.nDivision*2; l++) {
        k = 0;
        for (i = 0; i <= cObjOpt; i++) if (FProper(i)) {

                /* For each object, determine who is closest on either side. */

                k1 = rLarge;
                k2 = -rLarge;
                for (j = 0; j <= cObjOpt; j++)
                    if (FProper(j) && i != j) {
                        temp = symbol[j]-symbol[i];
                        if (RAbs(temp) > rDegHalf)
                            temp -= rDegMax*RSgn(temp);
                        if (temp < k1 && temp > 0.0)
                            k1 = temp;
                        else if (temp > k2 && temp <= 0.0)
                            k2 = temp;
                    }

                /* If an object's too close on one side, then we move to the other. */

                if (k2 > -orb && k1 > orb) {
                    k = 1;
                    symbol[i] = Mod(symbol[i]+orb*0.51+k2*0.49);
                } else if (k1 < orb && k2 < -orb) {
                    k = 1;
                    symbol[i] = Mod(symbol[i]-orb*0.51+k1*0.49);

                    /* If we are bracketed by close objects on both sides, then let's move */
                    /* to the midpoint, so we are as far away as possible from either one. */

                } else if (k2 > -orb && k1 < orb) {
                    k = 1;
                    symbol[i] = Mod(symbol[i]+(k1+k2)*0.5);
                }
            }
    }
}


/* Adjust an array of longitude positions so that no two are within a    */
/* certain orb of each other. This is used by the astro-graph routine to */
/* make sure we don't draw any planet glyphs marking the lines on top of */
/* each other. This is almost identical to the FillSymbolRing() routine  */
/* used by the wheel charts; however, there the glyphs are placed in a   */
/* continuous ring, while here we have the left and right screen edges.  */
/* Also, here we are placing two sets of planets at the same time.       */

void FillSymbolLine(symbol)
real *symbol;
{
    real orb = DEFORB*1.35*(real)gi.nScale, max = rDegMax, k1, k2, temp;
    int i, j, k = 1, l;

    if (gi.nMode != gEphemeris)
        max *= (real)gi.nScale;
    else
        orb *= rDegMax/(real)gs.xWin;

    /* Keep adjusting as long as we can still make changes. */

    for (l = 0; k && l < us.nDivision*2; l++) {
        k = 0;
        for (i = 1; i <= cObjOpt*2; i++)
            if (FProper((i+1)/2) && symbol[i] >= 0.0) {

                /* For each object, determine who is closest to the left and right. */

                k1 = max-symbol[i];
                k2 = -symbol[i];
                for (j = 1; j <= cObjOpt*2; j++) {
                    if (FProper((j+1)/2) && i != j) {
                        temp = symbol[j]-symbol[i];
                        if (temp < k1 && temp > 0.0)
                            k1 = temp;
                        else if (temp > k2 && temp <= 0.0)
                            k2 = temp;
                    }
                }

                /* If an object's too close on one side, then we move to the other. */

                if (k2 > -orb && k1 > orb) {
                    k = 1;
                    symbol[i] = symbol[i]+orb*0.51+k2*0.49;
                } else if (k1 < orb && k2 < -orb) {
                    k = 1;
                    symbol[i] = symbol[i]-orb*0.51+k1*0.49;
                } else if (k2 > -orb && k1 < orb) {
                    k = 1;
                    symbol[i] = symbol[i]+(k1+k2)*0.5;
                }
            }
    }
}


/* Given a zodiac position, return the degree on the current wheel chart */
/* circle where that position falls, rotating based on the Ascendant and */
/* adding in the opposite direction for Vedic mode wheels.               */

real PlaceInX(deg)
real deg;
{
    if (us.fVedic)
        deg = -chouse[1]*(gi.nMode != gWheel)*2.0-deg-60.0;
    return Mod(rDegHalf-deg+gi.rAsc);
}


/* Given a zodiac degree, adjust it if need be to account for the expanding */
/* and compacting of parts the zodiac that happen when we display a graphic */
/* wheel chart such that all the houses appear the same size.               */

real HousePlaceInX(deg)
real deg;
{
    int in;

    if (gi.nMode == gWheel)    /* We only adjust for the -w -X combination. */
        return deg;
    in = HousePlaceIn(deg);
    return Mod(ZFromS(in)+MinDistance(chouse[in], deg)/
               MinDistance(chouse[in], chouse[Mod12(in+1)])*30.0);
}


/*
******************************************************************************
** Multiple Chart Graphics Routines.
******************************************************************************
*/

/* Draw another wheel chart; however, this time we have two rings of planets */
/* because we are doing a relationship chart between two sets of data. This  */
/* chart is obtained when the -r0 is combined with the -X switch.            */

void XChartWheelRelation()
{
    real xsign[cSign+1], xhouse1[cSign+1], xplanet1[objMax], xplanet2[objMax],
    symbol[objMax];
    byte ignoreT[objMax];
    int cx, cy, i, j, k, first, n;
    real unitx, unity, temp, argum, argum1, value, valt, l, m;
    real ip, jp, p, ccc=2.0;
    real power2[oNorm+1];

    /* Set up variables and temporarily automatically decrease the horizontal */
    /* chart size to leave room for the sidebar if that mode is in effect.    */

    if (gs.fText && !us.fVelocity)
        gs.xWin -= xSideT;
    cx = gs.xWin/2 - 1;
    cy = gs.yWin/2 - 1;
    unitx = (real)cx;
    unity = (real)cy;
    gi.rAsc = gs.objLeft ? cp1.obj[abs(gs.objLeft)]+rDegQuad*(gs.objLeft < 0) :
              cp1.cusp[1];
    if (us.fVedic)
gi.rAsc = gs.objLeft ? (gs.objLeft < 0 ? 120.0 : -60.0)-gi.rAsc : 0.0;

    if (hRevers == 2)
        gi.rAsc = cp1.cusp[7];

    /* Fill out arrays with the degree of each object, cusp, and sign glyph. */

    if (gi.nMode == gWheel) {
        for (i = 1; i <= cSign; i++)
            xhouse1[i] = PZ(cp1.cusp[i]);
    } else {
        gi.rAsc -= cp1.cusp[1];
        for (i = 1; i <= cSign; i++)
            xhouse1[i] = PZ(ZFromS(i));
    }
    for (i = 1; i <= cSign; i++)
        xsign[i] = PZ(HousePlaceInX(ZFromS(i)));
    for (i = 0; i <= cObjOpt; i++)
        xplanet1[i] = PZ(HousePlaceInX(cp1.obj[i]));
    for (i = 0; i <= cObjOpt; i++)
        xplanet2[i] = PZ(HousePlaceInX(cp2.obj[i]));

    /* Go draw the outer sign and house rings. We are drawing only the */
    /* houses of one of the two charts in the relationship, however.   */

    DrawWheel(xsign, xhouse1, cx, cy, unitx, unity, gi.rAsc,
              0.70, 0.74, 0.78, 0.82, 0.885);

    /* Draw the outer ring of planets (based on the planets in the chart     */
    /* which the houses do not reflect - the houses belong to the inner ring */
    /* below). Draw each glyph, a line from it to its actual position point  */
    /* in the outer ring, and then draw another line from this point to a    */
    /* another dot at the same position in the inner ring as well.           */

    for (i = 0; i <= cObjOpt; i++)
        symbol[i] = xplanet2[i];
    if (us.nRel == rcTransit)
        for (i = 0; i <= cObjOpt; i++) {
            ignoreT[i] = ignore[i];
            ignore[i] = ignore2[i];
        }
    FillSymbolRing(symbol, 1.0);
    if (us.nRel == rcTransit)
        for (i = 0; i <= cObjOpt; i++)
            ignore[i] = ignoreT[i];

    for (i = cObjOpt; i >= 0; i--) if (FProper2(i)) {
            if (gs.fLabel) {
                temp = symbol[i];
                DrawColor(cp2.dir[i] < 0.0 ? gi.kiGray : gi.kiOn);
                DrawDash(cx+POINT1(unitx, 0.58, PX(xplanet2[i])),
                         cy+POINT1(unity, 0.58, PY(xplanet2[i])),
                         cx+POINT2(unitx, 0.61, PX(temp)),
                         cy+POINT2(unity, 0.61, PY(temp)),
                         (cp2.dir[i] < 0.0 ? 1 : 0) - gs.fColor);
                DrawObject(i, cx+POINT1(unitx, 0.65, PX(temp)),
                           cy+POINT1(unity, 0.65, PY(temp)));
            }
            DrawColor(kObjB[i]);
            DrawSpot(cx+POINT1(unitx, 0.56, PX(xplanet2[i])),
                     cy+POINT1(unity, 0.56, PY(xplanet2[i])));
            DrawSpot(cx+POINT1(unitx, 0.43, PX(xplanet2[i])),
                     cy+POINT1(unity, 0.43, PY(xplanet2[i])));
            DrawColor(cp2.dir[i] < 0.0 ? gi.kiGray : gi.kiOn);
            DrawDash(cx+POINT1(unitx, 0.45, PX(xplanet2[i])),
                     cy+POINT1(unity, 0.45, PY(xplanet2[i])),
                     cx+POINT2(unitx, 0.54, PX(xplanet2[i])),
                     cy+POINT2(unity, 0.54, PY(xplanet2[i])), 2-gs.fColor);
        }

    /* Now draw the inner ring of planets. If it weren't for the outer ring,  */
    /* this would be just like the standard non-relationship wheel chart with */
    /* only one set of planets. Again, draw glyph, and a line to true point.  */

    for (i = 0; i <= cObjOpt; i++)
        symbol[i] = xplanet1[i];
    FillSymbolRing(symbol, 1.1);
    DrawSymbolRing(symbol, xplanet1, cp1.dir, cx, cy, unitx, unity,
                   0.43, 0.45, 0.48, 0.52);

    if (!FCreateGridRelation(fFalse))
        return;
    STension = SHarmon = SConjunct = SSumma = SChart = 0.0;

    if (TensChart != 0) {
        DrawColor(kWhite);
        for (i = 0; i < 180; i++) {
            value = 0.20;
            argum = (real)(i)*2.0;
            argum = PZ(HousePlaceInX(argum));
            DrawPoint(cx+POINT1(unitx, value, PX(argum)),
                      cy+POINT1(unity, value, PY(argum)));
        }

        for (i = 1; i <= oNormOpt; i++)if (FNorm(i))
                power2[i] = rObjInf[i];

        if (us.nRel == rcTransit) {
            for (i = 0; i <= cObjOpt; i++)
                ignoreT[i] = ignore2[i];
            for (i = 1; i <= oNormOpt; i++)if (FNorm(i))
                    power2[i] = rTransitInf[i];
        } else {
            for (i = 0; i <= cObjOpt; i++)
                ignoreT[i] = ignore[i];
            if (us.nRel == rcDual)
                for (i = 1; i <= oNormOpt; i++)if (FNorm(i))
                        power2[i] = ppower2[i];
            if (us.nRel == rcProgress) {
                for (i = 0; i <= cObjOpt; i++)
                    ignoreT[i] = ignore3[i];
                for (i = 1; i<= oNormOpt; i++)if (FNorm(i))
                        power2[i] = ppower1[i];
            }
        }

        DrawColor(kWhite);
        for (i = 0; i < 3600; i++) {
            value = 0.0;
            if (TensChart <= 1)
                first = aOpp;
            else
                first = aCon;

            argum = (real)(i)/10.0;
            argum1 = PZ(HousePlaceInX(argum));
            for (j = 1; j <= cObjOpt; j++) if (!ignore[j]) {
                    l = MinDistance(argum, cp1.obj[j]);
                    for (k = first; k <= us.nAsp; k++) {
                        m = RAbs(l - rAspAngle[k]);
                        if (m < GetOrb(1,j,k)){
                            if (FCusp(j) && (k!=1 || k!=10))
                                continue;
                            valt = (sqrt)(20.0*ppower1[j])*rAspInf[k]*
                                   (1.0 - m/GetOrb(1,j,k)) ;
                            if ((k==aOpp || k==aSqu || k==aSSq || k==aSes) && (TensChart <= 1))
                                valt = - valt;
                            value = value + CoeffTens*valt/150.0;
                        }
                    }
                }
            for (j = 1; j <= cObjOpt; j++) if (!ignoreT[j]) {
                    l = MinDistance(argum, cp2.obj[j]);
                    for (k = first; k <= us.nAsp; k++) {
                        m = RAbs(l - rAspAngle[k]);
                        if (m < GetOrb(1,j,k)){
                            if (FCusp(j) && (k!=1 || k!=10))
                                continue;
                            valt = (sqrt)(20.0*power2[j])*rAspInf[k]*
                                   (1.0 - m/GetOrb(1,j,k)) ;
                            if ((k==aOpp || k==aSqu || k==aSSq || k==aSes) && (TensChart <= 1))
                                valt = - valt;
                            value = value + CoeffTens*valt/150.0;
                        }
                    }
                }

            if (TensChart == 1)
                value = 0.2 + value;
            if (TensChart == -1)
                value = 0.2 - value;
            if (value < 0.0)
                value = 0.0;
            if (value > 0.6)
                value = 0.6;

            DrawPoint(cx+POINT1(unitx, value, PX(argum1)),
                      cy+POINT1(unity, value, PY(argum1)));
        }
    } else {

        /* Draw lines connecting planets between the two charts that have aspects. */

        for (i = 0; i <= oNormOpt; i++)if (FNorm(i))
                power2[i] = rObjInf[i];

        if (us.nRel == rcTransit) {          /*  minu lisa, transiitide vorgule */
            for (i = 0; i <= cObjOpt; i++)
                ignoreT[i] = FIgnore2(i);
            for (i = 0; i <= oNormOpt; i++)if (FNorm(i))
                    power2[i] = rTransitInf[i];
        } else {
            for (i = 0; i <= cObjOpt; i++)
                ignoreT[i] = FIgnore(i);
            if (us.nRel == rcDual)
                for (i = 0; i <= oNormOpt; i++)if (FNorm(i))
                        power2[i] = ppower2[i];
            if (us.nRel == rcProgress)
                for (i = 0; i<= oNormOpt; i++)if (FNorm(i))
                        power2[i] = ppower1[i];
        }

        for (j = cObjOpt; j >= 0; j--)
            for (i = cObjOpt; i >= 0; i--)
                if (grid->n[i][j] && FProper2(i) && FProper(j)) {
                    k = grid->n[i][j];
                    ip = FNorm(i) ? power2[i] : 2.5;
                    jp = FNorm(j) ? ppower1[j] : 2.5;
                    p = (rAspInf[k]*(sqrt)(ip*jp)*
                         (1.0-RAbs((real)(grid->v[i][j]))/3600.0/GetOrb(i, j, k)));
                    if (us.fParallel)
                        p = p * PowerPar;
                    DrawColor(kAspB[grid->n[i][j]]);
                    if (!gs.fAlt) {
                        DrawDash(cx+POINT1(unitx, 0.41, PX(xplanet1[j])),
                                 cy+POINT1(unity, 0.41, PY(xplanet1[j])),
                                 cx+POINT1(unitx, 0.41, PX(xplanet2[i])),
                                 cy+POINT1(unity, 0.41, PY(xplanet2[i])),
                                 (int)(RAbs((real)(grid->v[i][j]))/(GetOrb(i,j,grid->n[i][j]))/900.0));
                    } else {
                        if (p <= 0.05)
                            n = 20;
                        else
                            n = (int)(SolidAspect/p);
                        DrawDash(cx+POINT1(unitx, 0.41, PX(xplanet1[j])),
                                 cy+POINT1(unity, 0.41, PY(xplanet1[j])),
                                 cx+POINT1(unitx, 0.41, PX(xplanet2[i])),
                                 cy+POINT1(unity, 0.41, PY(xplanet2[i])),
                                 n);
                    }
                }
    }

    for (j = cObjOpt; j >= 0; j--)
        for (i = cObjOpt; i >= 0; i--)
            if (grid->n[i][j] && FProper2(i) && FProper(j)) {
                k = grid->n[i][j];
                ip = FNorm(i) ? power2[i] : 2.5;
                jp = FNorm(j) ? ppower1[j] : 2.5;
                p = (rAspInf[k]*(sqrt)(ip*jp)*
                     (1.0-RAbs((real)(grid->v[i][j]))/3600.0/GetOrb(i, j, k)));
                if (us.fParallel)
                    p = p * PowerPar;
                if (k == aOpp || k == aSqu || k == aSSq || k == aSes)
                    STension -= ccc*p;
                if (k == aSex || k == aTri || k == aSSx || k == aInc)
                    SHarmon += ccc*p;
                if (k == aCon)
                    SConjunct += ccc*p;
                SSumma += ccc*p;
            }
    SChart = SHarmon + STension;

    /* Go draw sidebar with chart information and positions if need be. */

    DrawInfo();
}


/* Draw a tri-wheel chart or quad-wheel chart, where we have three or four */
/* rings, among three or four sets of data we're comparing. This chart is  */
/* obtained when the -r3 or -r4 switch is combined with the -X switch.     */

void XChartWheelThreeFour()
{
    real xsign[cSign+1], xhouse1[cSign+1], xplanet1[objMax], xplanet2[objMax],
    symbol[objMax];
    int cx, cy, i, fQuad;
    real unitx, unity, base;
    CP cpT;

    /* Set up variables and temporarily automatically decrease the horizontal */
    /* chart size to leave room for the sidebar if that mode is in effect.    */

    if (gs.fText && !us.fVelocity)
        gs.xWin -= xSideT;
    cx = gs.xWin/2 - 1;
    cy = gs.yWin/2 - 1;
    unitx = (real)cx;
    unity = (real)cy;
    gi.rAsc = gs.objLeft ? cp1.obj[abs(gs.objLeft)]+rDegQuad*(gs.objLeft < 0) :
              cp1.cusp[1];
    if (us.fVedic)
gi.rAsc = gs.objLeft ? (gs.objLeft < 0 ? 120.0 : -60.0)-gi.rAsc : 0.0;
    fQuad = (us.nRel == rcQuadWheel);
    base = (fQuad ? 0.22 : 0.35);

    if (hRevers == 2)
        gi.rAsc = cp1.cusp[7];

    /* Fill out arrays with the degrees of the cusps and sign glyphs, and the */
    /* positions of the outer two rings.                                      */

    if (gi.nMode == gWheel) {
        for (i = 1; i <= cSign; i++)
            xhouse1[i] = PZ(cp1.cusp[i]);
    } else {
        gi.rAsc -= cp1.cusp[1];
        for (i = 1; i <= cSign; i++)
            xhouse1[i] = PZ(ZFromS(i));
    }
    for (i = 1; i <= cSign; i++)
        xsign[i] = PZ(HousePlaceInX(ZFromS(i)));
    for (i = 0; i <= cObjOpt; i++)
        xplanet1[i] = PZ(HousePlaceInX(cp1.obj[i]));
    for (i = 0; i <= cObjOpt; i++)
        xplanet2[i] = PZ(HousePlaceInX(cp2.obj[i]));

    /* Go draw the outer sign and house rings. We are drawing the houses */
    /* of only the outermost ring of the wheel, however.                 */

    DrawWheel(xsign, xhouse1, cx, cy, unitx, unity, gi.rAsc,
              0.745, 0.78, 0.815, 0.84, 0.895);

    /* Draw the outer ring of planets (i.e. the one the house cusps reflect). */
    /* Draw each glyph, a line from it to its actual position point in the    */
    /* outer ring, and then draw another line from this point to a another    */
    /* dot at the same position on the innermost ring as well.                */

    for (i = 0; i <= cObjOpt; i++)
        symbol[i] = xplanet1[i];
    FillSymbolRing(symbol, 0.9);
    DrawSymbolRing(symbol, xplanet1, ret, cx, cy, unitx, unity,
                   0.61, 0.63, 0.66, 0.70);
    for (i = cObjOpt; i >= 0; i--) if (FProper(i)) {
            DrawColor(kObjB[i]);
            DrawPoint(cx+POINT1(unitx, base, PX(xplanet1[i])),
                      cy+POINT1(unity, base, PY(xplanet1[i])));
            if (gs.fAlt) {
                DrawColor(ret[i] < 0.0 ? gi.kiGray : gi.kiOn);
                DrawDash(cx+POINT1(unitx, base+0.02, PX(xplanet1[i])),
                         cy+POINT1(unity, base+0.02, PY(xplanet1[i])),
                         cx+POINT2(unitx, 0.59, PX(xplanet1[i])),
                         cy+POINT2(unity, 0.59, PY(xplanet1[i])), 3+fQuad-gs.fColor);
            }
        }

    /* Now draw the second to outermost ring of planets. Again, draw each */
    /* glyph, a line to its true point, and a line to the innermost ring. */

    for (i = 0; i <= cObjOpt; i++)
        symbol[i] = xplanet2[i];
    FillSymbolRing(symbol, 1.1);
    DrawSymbolRing(symbol, xplanet2, cp2.dir, cx, cy, unitx, unity,
                   0.48, 0.50, 0.53, 0.57);
    for (i = cObjOpt; i >= 0; i--) if (FProper(i)) {
            DrawColor(kObjB[i]);
            DrawPoint(cx+POINT1(unitx, base, PX(xplanet2[i])),
                      cy+POINT1(unity, base, PY(xplanet2[i])));
            if (gs.fAlt) {
                DrawColor(cp2.dir[i] < 0.0 ? gi.kiGray : gi.kiOn);
                DrawDash(cx+POINT1(unitx, base+0.02, PX(xplanet2[i])),
                         cy+POINT1(unity, base+0.02, PY(xplanet2[i])),
                         cx+POINT2(unitx, 0.46, PX(xplanet2[i])),
                         cy+POINT2(unity, 0.46, PY(xplanet2[i])), 2+fQuad-gs.fColor);
            }
        }

    /* The third ring (either the innermost or second to innermost) is next.   */
    /* Cast the chart on the fly, and draw the glyphs and lines to true point. */

    ciCore = ciThre;
    cpT = cp0;
    CastChart(fTrue);
    for (i = 0; i <= cObjOpt; i++)
        xplanet1[i] = PZ(HousePlaceInX(planet[i]));
    cp0 = cpT;
    for (i = 0; i <= cObjOpt; i++)
        symbol[i] = xplanet1[i];
    FillSymbolRing(symbol, 1.4);
    DrawSymbolRing(symbol, xplanet1, ret, cx, cy, unitx, unity,
                   0.35, 0.37, 0.40, 0.44);

    if (fQuad) {

        /* If a fourth ring is being done, first finish the third one by */
        /* drawing lines from the true positions to the inner ring.      */

        for (i = cObjOpt; i >= 0; i--) if (FProper(i)) {
                DrawColor(kObjB[i]);
                DrawPoint(cx+POINT1(unitx, base, PX(xplanet1[i])),
                          cy+POINT1(unity, base, PY(xplanet1[i])));
                if (gs.fAlt) {
                    DrawColor(ret[i] < 0.0 ? gi.kiGray : gi.kiOn);
                    DrawDash(cx+POINT1(unitx, base+0.02, PX(xplanet1[i])),
                             cy+POINT1(unity, base+0.02, PY(xplanet1[i])),
                             cx+POINT2(unitx, 0.33, PX(xplanet1[i])),
                             cy+POINT2(unity, 0.33, PY(xplanet1[i])), 2-gs.fColor);
                }
            }

        /* If the fourth (innermost) ring is being done, cast the chart on the */
        /* fly, and draw the glyphs and lines to the true positions.           */

        ciCore = ciFour;
        cpT = cp0;
        CastChart(fTrue);
        for (i = 0; i <= cObjOpt; i++)
            xplanet2[i] = PZ(HousePlaceInX(planet[i]));
        cp0 = cpT;
        for (i = 0; i <= cObjOpt; i++)
            symbol[i] = xplanet2[i];
        FillSymbolRing(symbol, 1.8);
        DrawSymbolRing(symbol, xplanet2, ret, cx, cy, unitx, unity,
                       0.22, 0.24, 0.27, 0.31);
    }

    /* Go draw sidebar with chart information and positions if need be. */

    ciCore = ciMain;
    DrawInfo();
}


/* Draw an aspect (or midpoint) grid in the window, between the planets in  */
/* two different charts, with the planets labeled at the top and side. This */
/* chart is done when the -g switch is combined with the -r0 and -X switch. */
/* Like above, the chart always has a (definable) fixed number of cells.    */

void XChartGridRelation()
{
    byte sz[cchSzDef];
    int unit, siz, x, y, i, j, k, l, m;
    KI c=0;
    byte ignoreT[objMax];

    unit = CELLSIZE*gi.nScale;
    siz = (gs.nGridCell+1)*unit;
    if (!FCreateGridRelation(gs.fAlt != us.fGridConfig))
        return;

    if (us.nRel == rcTransit) {
        for (m = 0; m <= cObjOpt; m++)
            ignoreT[m] = ignore2[m];
    } else if (us.nRel == rcProgress) {
        for (m = 0; m <= cObjOpt; m++)
            ignoreT[m] = ignore3[m];
    } else {
        for (m = 0; m <= cObjOpt; m++)
            ignoreT[m] = ignore[m];
    }

    for (y = 0, j = oEar-2; y <= gs.nGridCell; y++) {
        do {
            j++;
        } while ((ignore[j] || j == us.objCenter) && j != -1 && j <= cObjOpt);
        DrawColor(gi.kiGray);
        DrawDash(0, (y+1)*unit, siz, (y+1)*unit, !gs.fColor);
        DrawDash((y+1)*unit, 0, (y+1)*unit, siz, !gs.fColor);
        DrawColor(gi.kiLite);
        DrawEdge(0, y*unit, unit, (y+1)*unit);
        DrawEdge(y*unit, 0, (y+1)*unit, unit);
        /*    DrawEdge(y*unit, y*unit, (y+1)*unit, (y+1)*unit); */
        if (j <= cObjOpt) for (x = 0, i = oEar-2; x <= gs.nGridCell; x++) {
                do {
                    i++;
                } while ((ignoreT[i] || i == us.objCenter) && i != -1 && i <= cObjOpt);

                /* Again, we are looping through each cell in each row and column. */

                if (i <= cObjOpt) {
                    gi.xTurtle = x*unit+unit/2;
                    gi.yTurtle = y*unit+unit/2 -
                                 (gi.nScale/gi.nScaleT > 2 ? 5*gi.nScaleT : 0);

                    /* If current cell is on top row or left hand column, draw glyph */
                    /* of planet owning the particular row or column in question.    */

                    if (y == 0 || x == 0) {
                        if (x+y > 0)
                            DrawObject(y == 0 ? i : j, gi.xTurtle, gi.yTurtle);
                    } else {

                        /* Otherwise, draw glyph of aspect in effect, or glyph of */
                        /* sign of midpoint, between the two planets in question. */

                        k = grid->n[i][j];
                        if (gs.fAlt == us.fGridConfig) {
                            if (k) {
                                DrawColor(c = kAspB[k]);
                                DrawAspect(k, gi.xTurtle, gi.yTurtle);
                            }
                        } else {
                            DrawColor(c = kSignB(grid->n[i][j]));
                            DrawSign(grid->n[i][j], gi.xTurtle, gi.yTurtle);
                        }
                    }

                    /* Again, when scale size is 300+, print some text in current cell: */

                    if (gi.nScale/gi.nScaleT > 2 && gs.fLabel) {

                        /* For top and left edges, print sign and degree of the planet. */
                        /* For extreme upper left corner, print some little arrows */
                        /* pointing out chart1's planets and chart2's planets.     */

                        if (y == 0 || x == 0) {
                            if (x + y == 0) {
                                c = gi.kiLite;
                                sprintf(sz, "1v 2->");
                            } else {
                                k = SFromZ(y == 0 ? cp2.obj[i] : cp1.obj[j]);
                                l = (int)((y == 0 ? cp2.obj[i] : cp1.obj[j])-ZFromS(k));
                                c = kSignB(k);
                                sprintf(sz, "%c%c%c %02d", chSig3(k), l);
                            }
                        } else {
                            k = abs(grid->v[i][j])/60;

                            /* For aspect cells, print the orb in degrees and minutes. */

                            if (gs.fAlt == us.fGridConfig) {
                                if (grid->n[i][j])
                                    sprintf(sz, "%c%d %02d'", k != (grid->v[i][j])/60 ?
                                            (us.fAppSep ? 'a' : '-') : (us.fAppSep ? 's' : '+'),
                                                    k/60, k%60);
                                else
                                    sprintf(sz, "%c",chNull);

                                /* For midpoint cells, print degree and minute. */

                            } else
                                sprintf(sz, "%2d %02d'", k/60, k%60);
                        }
                        DrawColor(c);
                        DrawSz(sz, x*unit+unit/2, (y+1)*unit-3*gi.nScaleT, dtBottom);
                    }
                }
            }
    }
}


/* Draw a chart showing a graphical ephemeris for the given month (or year */
/* if -Ey in effect), with the date on the vertical access and the zodiac  */
/* on the horizontal, as done when the -E is combined with the -X switch.  */

void XChartEphemeris()
{
    real symbol[cObj*2+1], objSav[objMax];
    byte sz[4];
    int yea, year, unit = 6*gi.nScale, daytot, d = 1, day=1, mon=1, monsiz=30,
                          x1, y1, x2, y2, xs, ys, m, n, u, v=0, i, j;

    yea = us.nEphemYears;    /* Is this -Ey -X or just -E -X? */
    year= Yea;
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
        if (!yea || (yea <= 9 && day == 1)|| (mon==1 && day==1)) {
            DrawColor(gi.kiGray);
            DrawDash(x1, v, x2, v, 1);    /* Marker line for day or month. */
        }
        if (d > 1)
            for (i = 1; i <= cObjOpt; i++)
                objSav[i] = planet[i];
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
                DrawColor(kObjB[i]);
                DrawWrap(m, n, u, v, ret[i] > 0.0 ? -x1 : x1, x2);
            }

        /* Label months or days in the month along the left and right edges. */

        if (d <= daytot && (!yea || (yea <=5 && day == 1)|| (yea > 5 && mon == 1 && day==1))) {
            if (yea) {
                if ((yea==1||mon!=1) && yea <= 5)
                    sprintf(sz, "%c%c%c", chMon3(mon));
                else
                    sprintf(sz, "%d", year);
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
            i = yea * us.fSeconds ? 2 : 4;
            day += i;
            if (day > monsiz) {
                d += i-(day-monsiz-1);
                if (d <= daytot + 1) {
                    mon++;
                    if (mon>12){
                        mon=1;
                        year++;
                    }
                    monsiz = DayInMonth(mon, year);
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


#ifdef BIORHYTHM
/* Draw a graphic biorhythm chart on the screen, as is done when the -rb    */
/* switch is combined with -X. This is technically a relationship chart in  */
/* that biorhythm status is determined by a natal chart time at another     */
/* later time. For the day in question, and for two weeks before and after, */
/* the Physical, Emotional, and Mental percentages are plotted.             */

void XChartBiorhythm()
{
    byte sz[6], *c=0;
    real jd, r=0.0, a;
    int x1, x2, xs, cx, y1, y2, ys, cy, i, j, k, x, y, x0=0.0, y0=0.0;

    k = xFont*6*gi.nScaleT;
    x1 = k;
    x2 = gs.xWin-k;
    xs = x2-x1;
    cx = (x1+x2)/2;
    k = CELLSIZE;
    y1 = k;
    y2 = gs.yWin-k;
    ys = y2-y1;
    cy = (y1+y2)/2;

    /* Create a dotted day/percentage grid to graph on. */
    DrawColor(gi.kiGray);
    DrawDash(x1, cy, x2, cy, 1);
    DrawDash(cx, y1, cx, y2, 1);
    for (j = -us.nBioday+1; j <= us.nBioday-1; j++) {
        x = x1 + NMultDiv(xs, j+us.nBioday, us.nBioday*2);
        for (k = -90; k <= 90; k += 10) {
            y = y1 + NMultDiv(ys, 100+k, 200);
            DrawPoint(x, y);
        }
    }

    /* Now actually draw the three biorhythm curves. */
    for (i = 1; i <= 3; i++) {
        jd = RFloor(is.JD + rRound);
        switch (i) {
        case 1:
            r = brPhy;
            c = "PHYS";
            j = eFir;
            break;
        case 2:
            r = brEmo;
            c = "EMOT";
            j = eWat;
            break;
        case 3:
            r = brInt;
            c = "INTE";
            j = eEar;
            break;
        }
        DrawColor(kElemB[j]);
        for (jd -= (real)us.nBioday, j = -us.nBioday; j <= us.nBioday;
                j++, jd += 1.0) {
            a = RBiorhythm(jd, r);
            x = x1 + NMultDiv(xs, j+us.nBioday, us.nBioday*2);
            y = y1 + (int)((real)ys * (100.0-a) / 200.0);
            if (j > -us.nBioday)
                DrawLine(x0, y0, x, y);
            else
                DrawSz(c, x1/2, y+2*gi.nScaleT, dtCent);
            x0 = x;
            y0 = y;
        }
    }

    DrawColor(gi.kiLite);
    /* Label biorhythm percentages along right vertical axis. */
    for (k = -100; k <= 100; k += 10) {
        sprintf(sz, "%c%3d%%", k < 0 ? '-' : '+', abs(k));
        y = y1 + NMultDiv(ys, 100-k, 200);
        DrawSz(sz, (x2+gs.xWin)/2, y+2*gi.nScaleT, dtCent);
    }
    /* Label days on top horizontal axis. */
    k = Max(us.nBioday/7, 1);
    for (j = -us.nBioday+k; j < us.nBioday; j += k) {
        x = x1 + NMultDiv(xs, j+us.nBioday, us.nBioday*2);
        sprintf(sz, "%c%d", j < 0 ? '-' : '+', abs(j));
        DrawSz(sz, x, y1-2*gi.nScaleT, dtBottom);
    }
    DrawEdge(x1, y1, x2, y2);
}
#endif /* BIORHYTHM */
#endif /* GRAPH */

/* xcharts2.c */
