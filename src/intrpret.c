/*
** Astrolog (Version 5.42J) File: intrpret.c
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
*/

#include "astrolog.h"


#ifdef INTERPRET
/*
******************************************************************************
** Interpretation Routines.
******************************************************************************
*/

/* This function is used by the interpretation routines to print out lines  */
/* of text with newlines inserted just before the end of screen is reached. */

void FieldWord(sz)
char *sz;
{
    static char line[cchSzMax];
    static int cursor = 0;
    int i, j;

    /* Hack: Dump buffer if function called with a null string. */

    if (sz == NULL) {
        line[cursor] = 0;
        PrintSz(line);
        PrintL();
        cursor = 0;
        return;
    }
    if (cursor)
        line[cursor++] = ' ';
    for (i = 0; (line[cursor] = sz[i]); i++, cursor++)
        ;

    /* When buffer overflows 'n' columns, display one line and start over. */

    while (cursor >= us.nScreenWidth-1) {
        for (i = us.nScreenWidth-1; line[i] != ' '; i--)
            ;
        line[i] = 0;
        PrintSz(line);
        PrintL();
        line[0] = line[1] = ' ';
        for (j = 2; (line[j] = line[i+j-1]) != 0; j++)
            ;
        cursor -= (i-1);
    }
}


/* Display a general interpretation of what each sign of the zodiac, house, */
/* and planet or object means. This is called to do the -I0 switch table.   */

void InterpretGeneral()
{
    char sz[cchSzDef*3];
    int i;

    PrintSz("Signs of the zodiac represent psychological characteristics.\n\n");
    for (i = 1; i <= cSign; i++) {
        AnsiColor(kSignA(i));
        sprintf(sz, "%s is", szSignName[i]);
        FieldWord(sz);
        sprintf(sz, "%s, and", szDesc[i]);
        FieldWord(sz);
        sprintf(sz, "%s.", szDesire[i]);
        FieldWord(sz);
        FieldWord(NULL);
    }
    AnsiColor(kDefault);
    PrintSz("\nHouses represent different areas within one's life.\n\n");
    for (i = 1; i <= cSign; i++) {
        AnsiColor(kSignA(i));
        sprintf(sz, "The %d%s House is the area of life dealing with",
                i, szSuffix[i]);
        FieldWord(sz);
        sprintf(sz, "%s.", szLifeArea[i]);
        FieldWord(sz);
        FieldWord(NULL);
    }
    AnsiColor(kDefault);
    PrintSz("\nPlanets represent various parts of one's mind or self.\n\n");
    for (i = 1; i <= cObjInt; i++) {
        if (ignore[i] || FCusp(i))
            continue;
        AnsiColor(kObjA[i]);
        if ((i <= oMoo) || ((FBetween(i, oNod, oCore)) && (i != oLil || fSouthNode)))
            FieldWord("The");
        sprintf(sz, "%s%s%s represents one's",
                i == oNod ? "North " : (i == oFor ? "Part of " : ""), szObjName[i],
                i == 13 ? " Athena" : "");
        FieldWord(sz);
        sprintf(sz, "%s.", szMindPart[i]);
        FieldWord(sz);
        FieldWord(NULL);
    }
    AnsiColor(kDefault);
}


/* Display a general interpretation of what each aspect type means. This */
/* is called when printing the interpretation table in the -I0 switch.   */

void InterpretAspectGeneral()
{
    char sz[cchSzDef*3];
    int i;

    PrintSz("\nAspects are different relationships between planets.\n\n");
    for (i = 1; i <= Min(us.nAsp, cAspectInt); i++) {
        AnsiColor(kAspA[i]);
        sprintf(sz, "When planets are %s, one", szAspectName[i]);
        FieldWord(sz);
        sprintf(sz, szInteract[i], "");
        FieldWord(sz);
        FieldWord("another.");
        if (szTherefore[i][0]) {
            sprintf(sz, "%s.", szTherefore[i]);
            FieldWord(sz);
        }
        FieldWord(NULL);
    }
    return;
}


/* Print the interpretation of each planet in sign and house, as specified */
/* with the -I switch. This is basically array accessing combining the     */
/* meanings of each planet, sign, and house, and a couple of other things. */

void InterpretLocation()
{
    char sz[cchSzDef*3], c;
    int i, j;

    PrintL();
    for (i = 1; i <= cObjInt; i++) {
        if (ignore[i] || !FCapCh(ChCap(szMindPart[i][0])) )
            continue;
        AnsiColor(kObjA[i]);
        j = SFromZ(planet[i]);
        c = Dignify(i, j);
        sprintf(sz, "%s%s%s%s in %s", ret[i] < 0.0 ? "Retrograde " : "",
                i == oNod ? "North " : (i == oFor ? "Part of " : ""), szObjName[i],
                i == 13 ? " Athena" : "", szSignName[j]);
        FieldWord(sz);
        sprintf(sz, "and %d%s House:", inhouse[i], szSuffix[inhouse[i]]);
        FieldWord(sz);
        sprintf(sz, "%s's", szPerson);
        FieldWord(sz);
        FieldWord(szMindPart[i]);
        FieldWord("is");
        if (((int)planet[i]) % 30 < 10)
            FieldWord("very");
        sprintf(sz, "%s, and", szDesc[j]);
        FieldWord(sz);
        sprintf(sz, "%s.", szDesire[j]);
        FieldWord(sz);
        FieldWord("Most often this manifests");
        if (ret[i] < 0.0 && i != oNod)
            FieldWord("in an independent, backward, introverted manner, and");
        FieldWord("in the area of life dealing with");
        sprintf(sz, "%s.", szLifeArea[inhouse[i]]);
        FieldWord(sz);

        /* Extra information if planet is in its ruling, falling, etc, sign. */

        if (c == 'R')
            FieldWord("This is a major aspect of their psyche!");
        else if (c == 'F')
            FieldWord("(This bit plays only a minor part in their psyche.)");
        else if (c == 'e')
            FieldWord("It is easy for them to express this part of themself.");
        else if (c == 'd')
            FieldWord("It is difficult for them to express this part of themself.");
        FieldWord(NULL);
    }
}


/* Print an interpretation for a particular aspect in effect in a chart. */
/* This is called from the InterpretGrid and ChartAspect routines.       */

void InterpretAspect(x, y)
int x, y;
{
    char sz[cchSzDef*3];
    int n;

    n = grid->n[x][y];
    if (n < 1 || n > cAspectInt ||
            !FCapCh(ChCap(szMindPart[x][0])) || !FCapCh(ChCap(szMindPart[y][0])) || x > cObjInt || y > cObjInt)
        return;
    AnsiColor(kAspA[n]);
    sprintf(sz, "%s %s %s: %s's", szObjName[x],
            szAspectName[n], szObjName[y], szPerson);
    FieldWord(sz);
    FieldWord(szMindPart[x]);
    sprintf(sz, szInteract[n],
            szModify[Min(abs(grid->v[x][y])/9000, 2)][n-1]);
    FieldWord(sz);
    sprintf(sz, "their %s.", szMindPart[y]);
    FieldWord(sz);
    if (szTherefore[n][0]) {
        sprintf(sz, "%s.", szTherefore[n]);
        FieldWord(sz);
    }
    FieldWord(NULL);
}


/* Print the interpretation of each aspect in the aspect grid, as specified */
/* with the -g -I switch. Again, this is done by basically array accessing  */
/* of the meanings of the two planets in aspect and of the aspect itself.   */

void InterpretGrid()
{
    int i, j;

    for (i = 1; i < cObjInt; i++) if (!ignore[i] && !FCusp(i))
            for (j = i+1; j <= cObjInt; j++) if (!ignore[j] && !FCusp(i))
                    InterpretAspect(i, j);
}


/* Print an interpretation for a particular midpoint in effect in a chart. */
/* This is called from the ChartMidpoint routine.                          */

void InterpretMidpoint(x, y)
int x, y;
{
    char sz[cchSzDef*3];
    int n, i;

    if (FCusp(x) || FCusp(y) || x > cObjInt || y > cObjInt)
        return;
    n = grid->n[y][x];
    AnsiColor(kSignA(n));
    sprintf(sz, "%s midpoint %s in %s: The merging of %s's",
            szObjName[x], szObjName[y], szSignName[n], szPerson0);
    FieldWord(sz);
    FieldWord(szMindPart[x]);
    FieldWord("with their");
    FieldWord(szMindPart[y]);
    FieldWord("is");
    if (grid->v[y][x]/3600 < 10)
        FieldWord("very");
    sprintf(sz, "%s, and", szDesc[n]);
    FieldWord(sz);
    sprintf(sz, "%s.", szDesire[n]);
    FieldWord(sz);
    FieldWord("Most often this manifests in");
    if (ret[x]+ret[y] < 0.0 && x != oNod && y != oNod)
        FieldWord("an independent, backward, introverted manner, and");
    FieldWord("the area of life dealing with");
    i = HousePlaceIn(ZFromS(n) + (real)grid->v[y][x]/3600.0);
    sprintf(sz, "%s.", szLifeArea[i]);
    FieldWord(sz);
    FieldWord(NULL);
}


/* This is a subprocedure of ChartInDaySearch(). Print the interpretation    */
/* for a particular instance of the various exciting events that can happen. */

void InterpretInDay(source, aspect, dest)
int source, aspect, dest;
{
    char sz[cchSzDef*3];

    if (source > cObjInt || dest > cObjInt)
        return;

    /* Interpret object changing direction. */

    if (aspect == aDir) {
        AnsiColor(kObjA[source]);
        FieldWord("Energy representing");
        FieldWord(szMindPart[source]);
        FieldWord("will tend to manifest in");
        FieldWord(dest ? "an independent, backward, introverted" :
                  "the standard, direct, open");
        FieldWord("manner.");
        FieldWord(NULL);

        /* Interpret object entering new sign. */

    } else if (aspect == aSig) {
        AnsiColor(kObjA[source]);
        FieldWord("Energy representing");
        FieldWord(szMindPart[source]);
        sprintf(sz, "will be %s,", szDesc[dest]);
        FieldWord(sz);
        sprintf(sz, "and it %s.", szDesire[dest]);
        FieldWord(sz);
        FieldWord(NULL);

        /* Interpret aspect between transiting planets. */

    } else if (aspect > 0 && aspect <= cAspectInt) {
        AnsiColor(kAspA[aspect]);
        FieldWord("Energy representing");
        FieldWord(szMindPart[source]);
        sprintf(sz, szInteract[aspect], szModify[1][aspect-1]);
        FieldWord(sz);
        sprintf(sz, "energies of %s.", szMindPart[dest]);
        FieldWord(sz);
        if (szTherefore[aspect][0]) {
            if (aspect > aCon) {
                sprintf(sz, "%s.", szTherefore[aspect]);
                FieldWord(sz);
            } else
                FieldWord("They will affect each other prominently.");
        }
        FieldWord(NULL);
    }
}


/* This is a subprocedure of ChartTransitSearch(). Print the interpretation */
/* for a particular transit of a planet to a natal object of a chart.       */

void InterpretTransit(source, aspect, dest)
int source, aspect, dest;
{
    char sz[cchSzDef*3];

    if (FNorm(source) && FNorm(dest) && aspect <= cAspectInt && FCapCh(ChCap(szMindPart[dest][0])) && FCapCh(ChCap(szMindPart[source][0])) ) {
        AnsiColor(kAspA[aspect]);
        FieldWord("Energy representing");
        FieldWord(szMindPart[source]);
        sprintf(sz, szInteract[aspect], szModify[1][aspect-1]);
        FieldWord(sz);
        if (source != dest) {
            sprintf(sz, "%s's %s.", szPerson0, szMindPart[dest]);
        } else {
            sprintf(sz, "the same aspect inside %s's makeup.", szPerson0);
        }
        FieldWord(sz);
        if (szTherefore[aspect][0]) {
            if (aspect > aCon) {
                sprintf(sz, "%s.", szTherefore[aspect]);
                FieldWord(sz);
            } else
                FieldWord("This part of their psyche will be strongly influenced.");
        }
        FieldWord(NULL);
    }
}


/* Print the interpretation of one person's planet in another's sign and    */
/* house, in a synastry chart as specified with the -r switch combined with */
/* -I. This is very similar to the interpretation of the standard -v chart  */
/* in InterpretLocation(), but we treat the chart as a relationship here.   */

void InterpretSynastry()
{
    char sz[cchSzDef*3], c;
    int i, j;

    PrintL();
    for (i = 1; i <= cObjInt; i++) {
        if (ignore[i] || FCusp(i))
            continue;
        AnsiColor(kObjA[i]);
        j = SFromZ(planet[i]);
        c = Dignify(i, j);
        sprintf(sz, "%s%s%s%s in %s,", ret[i] < 0.0 ? "Retrograde " : "",
                i == oNod ? "North " : (i == oFor ? "Part of " : ""), szObjName[i],
                i == 13 ? " Athena" : "", szSignName[j]);
        FieldWord(sz);
        sprintf(sz, "in their %d%s House:", inhouse[i], szSuffix[inhouse[i]]);
        FieldWord(sz);
        sprintf(sz, "%s's", szPerson2);
        FieldWord(sz);
        FieldWord(szMindPart[i]);
        FieldWord("is");
        if (((int)planet[i]) % 30 < 10)
            FieldWord("very");
        sprintf(sz, "%s, and", szDesc[j]);
        FieldWord(sz);
        sprintf(sz, "%s.", szDesire[j]);
        FieldWord(sz);
        FieldWord("This");
        if (ret[i] < 0.0 && i != oNod)
            FieldWord(
                "manifests in an independent, backward, introverted manner, and");
        sprintf(sz, "affects %s in the area of life dealing with %s.",
                szPerson1, szLifeArea[inhouse[i]]);
        FieldWord(sz);

        /* Extra information if planet is in its ruling, falling, etc, sign. */

        if (c == 'R') {
            sprintf(sz, "This is a major aspect of %s's psyche!", szPerson2);
            FieldWord(sz);
        } else if (c == 'F') {
            sprintf(sz, "(This bit plays only a minor part in %s's psyche.)",
                    szPerson2);
            FieldWord(sz);
        } else if (c == 'e') {
            sprintf(sz, "%s is affected harmoniously in this way.", szPerson1);
            FieldWord(sz);
        } else if (c == 'd') {
            sprintf(sz, "%s is affected discordantly in this way.", szPerson1);
            FieldWord(sz);
        }
        FieldWord(NULL);
    }
}


/* Print an interpretation for a particular aspect in effect in a comparison */
/* relationship chart. This is called from the InterpretGridRelation and     */
/* the ChartAspectRelation routines.                                         */

void InterpretAspectRelation(x, y)
int x, y;
{
    char sz[cchSzDef*3];
    int n;

    n = grid->n[y][x];
    if (n < 1 || n > cAspectInt ||
            FCusp(x) || FCusp(y) || x > cObjInt || y > cObjInt)
        return;
    AnsiColor(kAspA[n]);
    sprintf(sz, "%s %s %s: %s's", szObjName[x],
            szAspectName[n], szObjName[y], szPerson1);
    FieldWord(sz);
    FieldWord(szMindPart[x]);
    sprintf(sz, szInteract[n],
            szModify[Min(abs(grid->v[y][x])/9000, 2)][n-1]);
    FieldWord(sz);
    sprintf(sz, "%s's %s.", szPerson2, szMindPart[y]);
    FieldWord(sz);
    if (szTherefore[n][0]) {
        if (n != 1) {
            sprintf(sz, "%s.", szTherefore[n]);
            FieldWord(sz);
        } else
            FieldWord("These parts affect each other prominently.");
    }
    FieldWord(NULL);
}


/* Print the interpretation of each aspect in the relationship aspect grid, */
/* as specified with the -r0 -g -I switch combination.                      */

void InterpretGridRelation()
{
    int i, j;

    for (i = 1; i <= cObjInt; i++) if (!ignore[i])
            for (j = 1; j <= cObjInt; j++) if (!ignore[j])
                    InterpretAspectRelation(i, j);
}


/* Print the interpretation of a midpoint in the relationship grid, as */
/* specified with the -r0 -m -I switch combination.                    */

void InterpretMidpointRelation(x, y)
int x, y;
{
    char sz[cchSzDef*3];
    int n;

    if (FCusp(x) || FCusp(y) || x > cObjInt || y > cObjInt)
        return;
    n = grid->n[y][x];
    AnsiColor(kSignA(n));
    sprintf(sz, "%s midpoint %s in %s: The merging of %s's",
            szObjName[x], szObjName[y], szSignName[n], szPerson1);
    FieldWord(sz);
    FieldWord(szMindPart[x]);
    sprintf(sz, "with %s's", szPerson2);
    FieldWord(sz);
    FieldWord(szMindPart[y]);
    FieldWord("is");
    if (grid->v[y][x]/3600 < 10)
        FieldWord("very");
    sprintf(sz, "%s, and", szDesc[n]);
    FieldWord(sz);
    sprintf(sz, "%s.", szDesire[n]);
    FieldWord(sz);
    if (cp1.dir[x]+cp2.dir[y] < 0.0 && x != oNod && y != oNod) {
        FieldWord("Most often this manifests in");
        FieldWord("an independent, backward, introverted manner.");
    }
    FieldWord(NULL);
}
#endif /* INTERPRET */


/*
******************************************************************************
** Chart Influence Routines.
******************************************************************************
*/

/* This is a subprocedure of ChartInfluence(). Based on the values in the */
/* array parameter 'value', store numbers in array 'rank' reflecting the  */
/* relative order, e.g. value[x] 2nd greatest array value -> rank[x] = 2. */

void SortRank(value, rank, size)
real *value;
int *rank, size;
{
    int h, i, j, k;

    value[0] = -1000.0;
    for (i = 1; i <= size; i++)
        rank[i] = -1;
    for (h = 1, i = 0; h <= size; h++) {
        if (size != cSign && (!FProper2(h)))
            continue;
        i++;
        k = 0;
        for (j = 1; j <= size; j++) {
            if (size != cSign && (!FProper2(j)))
                continue;
            if (value[j] > value[k] && rank[j] < 0)
                k = j;
        }

        /* 'k' is the current position of the 'i'th place planet. */

        rank[k] = i;
    }
}


/* This is a subprocedure of ChartInfluence(). Based on the values in the */
/* array parameter 'value', store numbers in array 'rank' reflecting the  */
/* relative order, e.g. value[x] 2nd greatest array value -> rank[x] = 2. */
/* But this time central object is other than Earth, so value[0] and      */
/* rank[0] represents Earth, and we have to swap these values temporarily */
/* with free place of central planet */

void SortRank2(value, rank, size)
real *value;
int *rank, size;
{
    int h, i, j, k;
    real tempval;

    tempval = value[us.objCenter];
    value[us.objCenter] = value[0];
    value[0] = -1000.0;
    for (i = 1; i <= size; i++)
        rank[i] = -1;
    for (h = 1, i = 0; h <= size; h++) {
        if (size != cSign && (!FProper2I(h)))
            continue;
        i++;
        k = 0;
        for (j = 1; j <= size; j++) {
            if (size != cSign && (!FProper2I(j)))
                continue;
            if (value[j] > value[k] && rank[j] < 0)
                k = j;
        }

        /* 'k' is the current position of the 'i'th place planet. */

        rank[k] = i;
    }
    rank[0] = rank[us.objCenter];
    value[0] = value[us.objCenter];
    value[us.objCenter] = tempval;
}


/* Calculate position part of planet powers. Used by calculation of total */
/* planet powers, also by calculating of aspects powers.                  */

void PlanetPPower()
{
    real x, Hy,noise,scale,hsl,card,dg;
    int xSub, i, j, l, count=0;
    int oDis[oNorm+1], dLev[oNorm+1], cLev[oNorm+1], dLevH[oNorm+1],
    dLevS[oNorm+1], dLevHd[oNorm+1], Rulers = 12;
    byte SaveRest[oNorm+1], SaveUra, SaveStar;
    byte c;
    CP cpT;
    cpT = cp0;

    for (i = 1; i <= cSign-1; i++)
        for (j = i+1; j <= cSign; j++)
            if ((rules[i] == rules[j]) && (i != j))
                Rulers -= 1;
    for (i = 0; i <= oNormOpt; i++)if (FNorm(i)){
            count+=ignore[i];
            SaveRest[i] = ignore[i];
        }
    scale=((real)oNormOpt-(real)count)/100.0+0.6;

    for (i = 0; i <= oNormOpt; i++)
        if (RulerPlanet(i))
            ignore[i] = 0;
    SaveUra = us.fUranian;
    SaveStar = us.nStar;
    us.fUranian = fTrue;
    us.nStar = fFalse;
    CastChart(fTrue);
    if (us.nHouseSystem != 14)
        Hy = 1.0;
    else
        Hy = 0.0;

    ppower1[0] = rObjInf[0];
    for (i = 0; i <= oNormOpt; i++) {
        if (ignore[i] || FCusp(i)||FStar(i))
            ppower1[i] = rObjInf[i];
        else
            ppower1[i] = 0.0;
    }
    for (i = 0; i <= oNormOpt; i++)if (FNorm(i)){
            dLevS[i] = 0;
            dLevH[i] = 0;
            dLevHd[i] = 0;
        }

    /* Looking for dispositors, look xcharts1.c, XChartDispositor  */

    for (xSub = 0; xSub <= 2; xSub++) {
        for (i = 0; i <= oNormOpt; i++)if (FNorm(i)) {
                dLev[i] = 1;
                if (xSub == 0)
                    oDis[i] = rules[SFromZ(planet[i])];
                if (xSub == 1)
                    oDis[i] = rules[inhouse[i]];
                if (xSub == 2)
                    oDis[i] = rules[SFromZ(chouse[inhouse[i]])];
            }
        do {
            j = fFalse;
            for (i = 0; i <= oNormOpt; i++)if (FNorm(i))
                    cLev[i] = fFalse;
            for (i = 0; i <= oNormOpt; i++)if (FNorm(i))
                    if (dLev[i] && RulerPlanet(i))
                        cLev[oDis[i]] = fTrue;
            for (i = 0; i <= oNormOpt; i++)if (FNorm(i))    /* A planet isn't a final dispositor */
                    if (dLev[i] && !cLev[i] && RulerPlanet(i)) {    /* if nobody is pointing to it.      */
                        dLev[i] = 0;
                        j = fTrue;
                    }
        } while (j);

        for (i = 0; i <= oNormOpt; i++)if (FNorm(i))
                if (!RulerPlanet(i))
                    dLev[i] = 0;

        do {
            j = fFalse;
            for (i = 0; i <= oNormOpt; i++) if (RulerPlanet(i)) {
                    if (!dLev[i]) {
                        if (!dLev[oDis[i]])
                            j = fTrue;
                        else                            /* If my dispositor already has */
                            dLev[i] = dLev[oDis[i]] + 1;   /* a level, mine is one more.   */
                    }
                }
        } while (j);

        if (xSub == 0)
            for (i = 0; i <= oNormOpt; i++)if (FNorm(i))
                    dLevS[i] = dLev[i];            /* Store sign dispositors info */
        if (xSub == 1)
            for (i = 0; i <= oNormOpt; i++)if (FNorm(i))
                    dLevH[i] = dLev[i];            /* Store house dispositors info */
        if (xSub == 2)
            for (i = 0; i <= oNormOpt; i++)  if (FNorm(i))
                    dLevHd[i] = dLev[i];           /* Store house cusp dispositor info */
    }

    for (i = 0; i <= oNormOpt; i++)if (FNorm(i))
            ignore[i] = SaveRest[i];
    us.fUranian = SaveUra;
    us.nStar = SaveStar;
    /*  CastChart(fTrue); */
    cp0 = cpT;

    /* For each object, find its power based on its placement alone. */

    for (i = 0; i <= oNormOpt; i++) if (FNorm(i) && !ignore[i] && !FCusp(i) && i != us.objCenter) {
            j = SFromZ(planet[i]);
            dg=planet[i]-30*(j-1);
            hsl=(planet[i]-chouse[inhouse[i]])/(chouse[Mod12(1+inhouse[i])]-chouse[inhouse[i]]);
            card=(double)((inhouse[i]+(hsl>0.6847))%3==0);
            /*sprintf(sz,"%s %f \n",szObjName[i],hsl);PrintSz(sz);*/
            ppower1[i] += rObjInf[i];             /* Influence of planet itself. */
            if (hsl<0.6847)
                ppower1[i] *= 0.25*card+1.6-hsl/0.6847;
            else
                ppower1[i] *= 0.6+0.25*card+(hsl-0.6847)/(1-0.6847); /* Proximity to cusp */
            ppower1[i] *= FBetween(dg,1,12)? 0.9+dg/60.0 : FBetween(dg,12,29) ? 1.1+(12-dg)/60.0
                          : (dg<1) ? 1.1-dg*0.2 : 1.1-(30-dg)*0.3; /* Influence of planet in sign degree */
            ppower1[i] += rHouseInf[Mod12(inhouse[i]+(hsl>0.966))];  /* Influence of house it's in. */
            if (RAbs(ret[i])<(us.nStationOrb/planetdis[i]/rDegRad)&& planetdis[i]>0.1) /* Stationary */
                ppower1[i] *= 1+3*(0.07-RAbs(ret[i])*planetdis[i]*rDegRad);
            c = Dignify(i, j);
            switch (c) {
            case 'R':
                x = rObjInf[i]*rObjInf[oNorm+1]/10.0;
                break; /* Planets in signs they rule or */
            case 'e':
                x = rObjInf[i]*rObjInf[oNorm+2]/10.0;
                break; /* exalted in have influence.    */
            case 'F':
                x = -rObjInf[i]/3.0;
                break;
            case 'd':
                x = -rObjInf[i]/4.0;
                break;
            default:
                x = 0.0;
            }
            c = Dignify(i, Mod12((inhouse[i]+(hsl>0.966))));
            switch (c) {
            case 'R':
                x += rObjInf[i]*rHouseInf[cSign+1]/20.0;
                break; /* Item in house aligned with*/
            case 'e':
                x += rObjInf[i]*rHouseInf[cSign+2]/20.0;
                break; /* sign ruled has influence. */
            case 'F':
                x -= rObjInf[i]/6.0;
                break;
            case 'd':
                x -= rObjInf[i]/8.0;
                break;
            default:
                ;
            }
            if (FBetween(hsl,0.684,0.966)) {
                c = Dignify(i, (Mod12(inhouse[i]+1)));
                switch (c) {
                case 'R':
                    x += rObjInf[i]*rHouseInf[cSign+1]/20.0;
                    break; /* Item in house aligned with*/
                case 'e':
                    x += rObjInf[i]*rHouseInf[cSign+2]/20.0;
                    break; /* sign ruled has influence. */
                case 'F':
                    x -= rObjInf[i]/6.0;
                    break;
                case 'd':
                    x -= rObjInf[i]/8.0;
                    break;
                default:
                    ;
                }
            }

            /*  Same additions by situative houses.  */

            c = Dignify(i, SFromZ(chouse[inhouse[i]]));
            switch (c) {
            case 'R':
                x += rObjInf[i]*rHouseInf[cSign+1]/20.0;
                break;
            case 'e':
                x += rObjInf[i]*rHouseInf[cSign+2]/20.0;
                break;
            case 'F':
                x -= rObjInf[i]/6.0;
                break;
            case 'd':
                x -= rObjInf[i]/8.0;
                break;
            default:
                ;
            }
            ppower1[i] += x;

            /* Sign ruler has influence, if planet is in this sign.             */
            /* Addition depends of ruling level (see graphics influence chart). */

            if (i != rules[j]) {
                if (RulerPlanet(i))
                    ppower1[rules[j]] += rObjInf[i]/(5.0*sqrt((real)dLevS[rules[j]]));
                else
                    ppower1[rules[j]] += rObjInf[i]/5.0;
            }

            /* House cusp dispositor has influence, if planet is in this house. */
            /* Addition depends of ruling level (see graphics influence chart). */
            /* Influence has both natural and situative rulers of house.        */

            if (i != (j = rules[inhouse[i]])){
                if (RulerPlanet(i))
                    ppower1[j] += rObjInf[i]/(10.0*sqrt((real)dLevH[j]));
                else
                    ppower1[j] += rObjInf[i]/10.0;
            }

            l = SFromZ(chouse[inhouse[i]]);
            if (i != rules[l]) {
                if (RulerPlanet(i))
                    ppower1[rules[l]] += Hy*rObjInf[i]/(10.0*sqrt((real)dLevHd[rules[l]]));
                else
                    ppower1[rules[l]] += Hy*rObjInf[i]/10.0;
            }
        }

    for (i = 1; i <= cSign; i++) {            /* Various planets get influence */
        j = SFromZ(chouse[i]);                  /* if house cusps fall in signs  */
        ppower1[rules[j]] += rHouseInf[i]/2.0;  /* they rule.                    */
    }

    /* Add term for region about 12AU distance */
    /* Subtract noise */
    noise=ppower1[MinimumRank(ppower1,oNormOpt)]-0.5;
    for (i = 0; i <= oNormOpt; i++) if (FNorm(i)&&!FIgnore(i)){
            ppower1[i]-=noise;
            if (us.nTargetDistance > 0.1){
                ppower1[i]+= (planetdis[i]>0.1) ? 0.3/(0.01+RAbs(log(sqrt(planetdis[i]/us.nTargetDistance)))):0;
            }
        }
    /* And now, when all things have calculated positional powers, give  */
    /* additions to house cusps (as objects), if there are something in  */
    /* this house. */

    for (i = 0; i <= cObjOpt; i++) if (!ignore[i] && !FCusp(i) && i != us.objCenter){
            hsl=(planet[i]-chouse[inhouse[i]])/(chouse[Mod12(1+inhouse[i])]-chouse[inhouse[i]]);
            if (hsl<0.6847)
                ppower1[inhouse[i]+cuspLo-1] += FNorm(i) ? (1.6-hsl/0.6847)*ppower1[i]/10.0/scale : 0.2/scale ;
            else {
                ppower1[Mod12(1+inhouse[i])+cuspLo-1] += FNorm(i) ? (0.6+(hsl-0.6847)/(1-0.6847))*ppower1[i]/10.0/scale : 0.2/scale ;
                if (hsl<0.966)
                    ppower1[inhouse[i]+cuspLo-1] += FNorm(i) ? (0.6+(hsl-0.6847)/(1-0.6847))*ppower1[i]/10.0/scale : 0.2/scale ;
            }
        }
    /* Ignore objects below Treshold if defined */
    if (us.nTreshold > 0.01)
        for (i = 0; i <= oNormOpt; i++) if (FNorm(i) && !ignore[i] && !FCusp(i) && i != us.objCenter)
                if (ppower1[i]<us.nTreshold)
                    ignore[i]=fTrue;
}


/* Print out a list of power values and relative rankings, based on the */
/* placements of the planets, and their aspects in the aspect grid, as  */
/* specified with the -j "find influences" switch.                      */

void ChartInfluence()
{
    real power[oNormOpt+1], power1[oNormOpt+1], power2[oNormOpt+1],power3[oNormOpt+1],
    Harmon[oNormOpt+1], Tension[oNormOpt+1], AspectSum[oNormOpt+1], Conjunct[oNormOpt+1],
    total, total1, total2, total3, total4, total5, valt, m, ll, lll, ip, jp,scale,noise;
    int rank[oNormOpt+1], rank1[oNormOpt+1], rank2[oNormOpt+1], rank3[oNormOpt+1],count=0,
            i, j, k, l, jo, kk, ssmart,dg;
    byte saveflag;
    char sz[cchSzDef];

    PrintHeader();
    PrintL();    /* Display chart info */

    if (us.nRel == rcDual || us.nRel == rcTransit || us.nRel == rcProgress)
        goto DualInfluence;

    saveflag = us.fParallel;
    us.fParallel = 0;

    for (i = 0; i <= oNormOpt; i++)if (FNorm(i)){
            power2[i] = power3[i] = 0.0;
            count+=ignore[i];
        }
    total = total1 = total2 = total3 = 0.0;
    scale=((real)oNormOpt-(real)count)/100.0+0.6;
    PlanetPPower();



    for (i = 0; i <= oNormOpt; i++) if (FNorm(i)){
            power1[i] = ppower1[i];
            ppowerTotal[i] = ppower1[i];
        }

    /* Second, for each object, find its power based on aspects it makes. */

    if (!FCreateGrid(fFalse))
        return;
    for (j = 0; j <= oNormOpt; j++) if (FNorm(j)&& !ignore[j] && j != us.objCenter)
            for (i = 0; i <= cObjOpt; i++) if (!ignore[i] && i != j && i != us.objCenter && (!FIgnoreS(i)||!FIgnoreS(j))) {
                    k = grid->n[Min(i, j)][Max(i, j)];
                    if (FCusp(i) && FCusp(j))
                        continue;
                    if (k) {
                        l = grid->v[Min(i, j)][Max(i, j)];
                        power2[j] += rAspInf[k]*(sqrt)((FNorm(i) ? power1[i]: 2.0 ) *power1[j])*0.5*
                                     (1.0-RAbs((real)l)/3600.0/GetOrb(i,j,k))/scale;

                        /* Even in case of SmartCusps, give additional power to opposite */
                        /* cusp. But planet has influence only from one cusp!!!          */

                        if (us.fSmartCusp && FCusp(j)) {
                            us.fSmartCusp = fFalse;
                            if (j >= oAsc && j < oDes)
                                jo = j + 6;
                            else
                                jo = j - 6;

                            lll = MinDistance(planet[i], planet[jo]);
                            for (kk = us.nAsp; kk >= 1; kk--) {
                                if (!FAcceptAspect(i, kk, jo))
                                    continue;
                                m = lll-rAspAngle[kk];
                                if (RAbs(m) < GetOrb(i, jo, kk)) {
                                    power2[jo] += rAspInf[kk]*(sqrt)((FNorm(i) ? power1[i]: 2.0)*power1[jo])*0.5*
                                                  (1.0-RAbs((real)l)/3600.0/GetOrb(i,jo,kk))/scale;
                                }
                            }
                            us.fSmartCusp = fTrue;
                        }
                    }
                }

    /* Third, for each object, find its power based on parallel and  */
    /* and contraparallel aspects it makes                           */

    us.fParallel = 1;
    if (!FCreateGrid(fFalse))
        return;

    for (j = 0; j < oNormOpt; j++) if (!ignore[j] && FNorm(j) && j != us.objCenter)
            for (i = 0; i <= cObjOpt; i++) if (!ignore[i] && FNorm(i) && i != j && i != us.objCenter && (!FIgnoreS(i)||!FIgnoreS(j))) {
                    k = grid->n[Min(i, j)][Max(i, j)];
                    if (k) {
                        l = grid->v[Min(i, j)][Max(i, j)];
                        power3[j] += rAspInf[k]*(sqrt)((FNorm(i) ? power1[i] : 2.0)*power1[j])*0.1*
                                     (1.0-RAbs((real)l)/3600.0/GetOrb(i,j,k))/sqrt(scale);
                    }
                }

    us.fParallel = saveflag;

    /* Finally, determine ranks of the arrays, then print everything out. */

    if (us.objCenter) {
        SortRank2(power1, rank1, oNormOpt);
        SortRank2(power2, rank2, oNormOpt);
        SortRank2(power3, rank3, oNormOpt);
    } else {
        SortRank(power1, rank1, oNormOpt);
        SortRank(power2, rank2, oNormOpt);
        SortRank(power3, rank3, oNormOpt);
    }

    noise=power2[MinimumRank(power2,oNormOpt)]-0.5;
    for (j=1;j<= oNormOpt;j++) {  /* Subtract aspect noise */
        if (!FIgnore(j)||FNorm(j))
            power2[j]-=noise;
    }
    /* Calculate total power of each planet. */

    for (i = 0; i <= oNormOpt; i++) if (FNorm(i)){
            power[i] = power1[i] + power2[i] + power3[i];
            if (!FIgnore(i)) {
                total1 += power1[i];
                total2 += power2[i];
                total3 += power3[i];
            }
        }
    total = total1 + total2 +total3;

    if (us.objCenter) {
        SortRank2(power, rank, oNormOpt);
    } else {
        SortRank(power, rank, oNormOpt);
    }


    PrintSz("     Planet:    Position      Aspects        Parallel       Total Rank   Percent\n");
    if (us.fSortRank) {
        i=oNormOpt+1;
        for (j = 1; j<= oNormOpt;j++){
            ppowerTotal[j] = power[j];
#ifdef INTERPRETALT
            if (NoPrint)
                continue;
            else{
#endif
                for (k=0; k<=oNormOpt;k++) if (!FStar(k)){
                        if (rank[k]!=j || ignore[k] || i==us.objCenter)
                            continue;
                        i=k;
                        k=oNormOpt+2;
                    }
                if (k!=oNormOpt+1) {
                    AnsiColor(kObjA[i]);
                    sprintf(sz, "%11.11s: ", szObjName[i]);
                    PrintSz(sz);
                    sprintf(sz, "%6.1f (%4d) +%6.1f (%4d) +%6.1f (%4d) =%7.1f (%4d) /%4.1f%%\n",
                            power1[i], rank1[i], power2[i], rank2[i], power3[i], rank3[i],
                            power[i], rank[i], power[i]/total*100.0);
                    PrintSz(sz);
#ifdef INTERPRETALT
                }
#endif
            }
        }
    } else {
        for (i = 0; i <= oNormOpt; i++) if (FNorm(i)&& !ignore[i] && i != us.objCenter) {
                ppowerTotal[i] = power[i];
#ifdef INTERPRETALT
                if (NoPrint)
                    continue;
                else {
#endif
                    AnsiColor(kObjA[i]);
                    sprintf(sz, "%11.11s: ", szObjName[i]);
                    PrintSz(sz);
                    sprintf(sz, "%6.1f (%4d) +%6.1f (%4d) +%6.1f (%4d) =%7.1f (%4d) /%4.1f%%\n",
                            power1[i], rank1[i], power2[i], rank2[i], power3[i], rank3[i],
                            power[i], rank[i], power[i]/total*100.0);
                    PrintSz(sz);
                }
            }
#ifdef INTERPRETALT
    }
    if (!NoPrint){
#endif
        AnsiColor(kDefault);
        sprintf(sz, "      Total: %6.1f      +%6.1f      +%6.1f      =%7.1f      / 100.0%%\n",
                total1, total2, total3, total);
        PrintSz(sz);
#ifdef INTERPRETALT
    }
#endif

    /* Now, print out a list of power values and relative rankings, based on  */
    /* the power of each sign of the zodiac, as indicated by the placement of */
    /* the planets above, in the chart, as specified with the -j0 switch.     */

    if (!us.fInfluenceSign)
        return;

#ifdef INTERPRETALT
    if (NoPrint)
        return;
#endif
    /* Sign power is determined by power of ruler and */
    /* powers of objects in sign                      */

    for (i = 1; i <= cSign; i++)
        power1[i] = power[rules[i]];

    for (i = 1; i <= oNormOpt; i++) if (FNorm(i) && !ignore[i]) {
            /*if (FThing(i)){*/
            j = SFromZ(planet[i]);
            dg=(int)planet[i]-30*(j-1);
            power1[j] += (power[i]*(1/(1+2*(!FThing(i))))*(FBetween(dg,7,17)? 1.1 : (FBetween(dg,27,29)||dg==1) ? 0.8
                          : (FBetween(dg,18,26)||FBetween(dg,2,6))?1.0 : 0.7)); /* Influence of planet in sign degree */
            if (dg==0)
                power1[Mod12(j-1)] += power[i]*(1/(1+2*(!FThing(i))))*0.5;
            if (dg==29)
                power1[Mod12(j+1)] += power[i]*(1/(1+2*(!FThing(i))))*0.5;
            /*}else
              power1[j] += power[i] / 3.0;*/
        }
    if (!fSouthNode && !ignore[oNod]) {
        power1[Mod12(SFromZ(planet[oNod])+6)] += power[oNod] / 6.0;  /* South */
        power1[Mod12(inhouse[oNod]+6)]        += power[oNod] / 8.0;  /* Node. */
    }

    total1 = 0.0;
    for (i = 1; i <= cSign; i++)
        total1 += power1[i];
    for (i = 1; i <= cSign; i++)
        power1[i] *= total/total1;
    total1 = total;

    /* Again, determine ranks in the array, and print everything out. */

    SortRank(power1, rank1, cSign);
    PrintSz(
        "\n       Sign:  Power Rank  Percent  -   Element  Power  Percent\n");
    for (i = 1; i <= cSign; i++) {
        AnsiColor(kSignA(i));
        sprintf(sz, "%11.11s: ", szSignName[i]);
        PrintSz(sz);
        sprintf(sz, "%6.1f (%4d) /%6.1f%%",
                power1[i], rank1[i], power1[i]/total1*100.0);
        PrintSz(sz);
        if (i <= 4) {
            sprintf(sz, "  -%9.7s:", szElem[i-1]);
            PrintSz(sz);
            total2 = 0.0;
            for (j = 1; j < cSign; j += 4)
                total2 += power1[i-1+j];
            sprintf(sz, "%7.1f /%6.1f%%", total2, total2/total1*100.0);
            PrintSz(sz);
        } else if (i == 6) {
            AnsiColor(kDefault);
            PrintSz("  -      Mode  Power  Percent");
        } else if (i >= 7 && i <= 9) {
            AnsiColor(kModeA(i-7));
            sprintf(sz, "  -%9.8s:", szMode[i-7]);
            PrintSz(sz);
            total2 = 0.0;
            for (j = 1; j < cSign; j += 3)
                total2 += power1[i-7+j];
            sprintf(sz, "%7.1f /%6.1f%%", total2, total2/total1*100.0);
            PrintSz(sz);
        }
        PrintL();
    }
    AnsiColor(kDefault);
    sprintf(sz, "      Total:%7.1f      / 100.0%%\n", total1);
    PrintSz(sz);

    /* House power is determined by internal initial power and natuaral  */
    /* and situative rulers, also powers of objects in house, also       */
    /* house cusp.                                                       */

    for (i = 1; i <= cSign; i++) {
        power1[i]  = rHouseInf[i];
        power1[i] += power[cuspLo + i - 1];
        power1[i] += power[rules[i]] / 2.0;
        power1[i] += power[rules[SFromZ(chouse[i])]] / 2.0;
    }

    /* For each house, determine its power based on the power of the object. */

    for (i = 1; i <= oNormOpt; i++) if (FNorm(i) && !ignore[i] && !FCusp(i))
            power1[inhouse[i]]         += power[i];

    if (!fSouthNode && !ignore[oNod]) {
        power1[Mod12(inhouse[oNod]+6)]        += power[oNod] / 6.0;  /* South */
        power1[Mod12(SFromZ(planet[oNod])+6)] += power[oNod] / 8.0;  /* Node. */
    }

    total1 = 0.0;
    for (i = 1; i <= cSign; i++)
        total1 += power1[i];
    for (i = 1; i <= cSign; i++)
        power1[i] *= total/total1;
    total1 = total;

    /* Again, determine ranks in the array, and print everything out. */

    SortRank(power1, rank1, cSign);
    PrintSz(
        "\n      House:  Power Rank  Percent  -   Element  Power  Percent\n");
    for (i = 1; i <= cSign; i++) {
        AnsiColor(kSignA(i));
        sprintf(sz, "%9d%s: ", i, szSuffix[i]);
        PrintSz(sz);
        sprintf(sz, "%6.1f (%2d) /%6.1f%%",
                power1[i], rank1[i], power1[i]/total1*100.0);
        PrintSz(sz);
        if (i <= 4) {
            sprintf(sz, "  -%9.7s:", szElem[i-1]);
            PrintSz(sz);
            total2 = 0.0;
            for (j = 1; j < cSign; j += 4)
                total2 += power1[i-1+j];
            sprintf(sz, "%7.1f /%6.1f%%", total2, total2/total1*100.0);
            PrintSz(sz);
        } else if (i == 6) {
            AnsiColor(kDefault);
            PrintSz("  -      Mode  Power  Percent");
        } else if (i >= 7 && i <= 9) {
            AnsiColor(kModeA(i-7));
            sprintf(sz, "  -%9.8s:", szMode[i-7]);
            PrintSz(sz);
            total2 = 0.0;
            for (j = 1; j < cSign; j += 3)
                total2 += power1[i-7+j];
            sprintf(sz, "%7.1f /%6.1f%%", total2, total2/total1*100.0);
            PrintSz(sz);
        }
        PrintL();
    }
    AnsiColor(kDefault);
    sprintf(sz, "      Total:%7.1f      / 100.0%%\n", total1);
    PrintSz(sz);
    PrintL();


    /* Harmonious-tension influences by planets in single chart */

    ssmart = us.fSmartCusp;
    for (i = 0; i <= oNormOpt; i++) if (FNorm(i) && !ignore[i] && i != us.objCenter) {
            Tension[i] = Harmon[i] = power3[i] = AspectSum[i] = Conjunct[i] = 0.0;
            for (j = 0; j <= cObjOpt; j++) if (!ignore[j] && i != j && j != us.objCenter) {
                    if (FCusp(i) && FCusp(j))
                        continue;
                    if (FCusp(i))
                        us.fSmartCusp = fFalse;
                    ll = MinDistance(planet[i], planet[j]);
                    for (k = 1; k <= us.nAsp; k++) {
                        m = RAbs(ll - rAspAngle[k]);
                        if (m < GetOrb(i,j,k) && FAcceptAspect(i,k,j)){
                            jp = FNorm(j) ? ppower1[j] : 2.5;
                            valt = (sqrt)(ppower1[i]*jp)*rAspInf[k]*(1.0 - m/GetOrb(i,j,k)) ;
                            if (k == aOpp || k == aSqu || k == aSSq || k == aSes )
                                Tension[i] -= valt;
                            if (k== aInc || k == aTri || k == aSex || k == aSSx || k == aQui || k == aBQn )
                                Harmon[i] += valt;
                            if (k == aCon)
                                Conjunct[i] += valt;
                            AspectSum[i] += valt;
                        }
                    }
                    us.fSmartCusp = ssmart;
                }
            power3[i] = Tension[i] + Harmon[i];
        }
    total4 = total1 = total2 = total3 = total5 =0.0;
    for (i = 0; i <= oNormOpt; i++) if (FNorm(i) && !ignore[i] && i != us.objCenter) {
            total1 += Harmon[i];
            total2 += Tension[i];
            total3 += power3[i];
            total4 += AspectSum[i];
            total5 += Conjunct[i];
        }

    if (us.objCenter) {
        SortRank2(power3, rank1, oNormOpt);
        SortRank2(Conjunct, rank2, oNormOpt);
        SortRank2(AspectSum, rank, oNormOpt);
    } else {
        SortRank(power3, rank1, oNormOpt);
        SortRank(Conjunct, rank2, oNormOpt);
        SortRank(AspectSum, rank, oNormOpt);
    }

    PrintSz("     Planet:     Harm.     Tens.      Sum.           Conj.         Total\n");
    for (i = 0; i <= oNormOpt; i++) if (FNorm(i) && !ignore[i] && i != us.objCenter) {
            AnsiColor(kObjA[i]);
            sprintf(sz, "%11.11s: ", szObjName[i]);
            PrintSz(sz);
            sprintf(sz, "  %6.1f    %6.1f    %6.1f (%2d)    %6.1f (%2d)    %6.1f (%2d)\n",
                    Harmon[i], Tension[i], power3[i], rank1[i], Conjunct[i], rank2[i],
                    AspectSum[i], rank[i]);
            PrintSz(sz);
        }
    AnsiColor(kDefault);
    sprintf(sz, "      Total:   %6.1f    %6.1f    %6.1f         %6.1f         %6.1f\n",
            total1, total2, total3, total5, total4);
    PrintSz(sz);
    return;

    /* Influences from second chart to first one for comparison charts */
    /* (dual,  natal-transit, natal-progressed                         */

DualInfluence:

    if (!FCreateGridRelation(fFalse))
        return;
    for (i = 0; i <= oNormOpt; i++) if (FNorm(i)){
            if (us.nRel == rcDual)
                power2[i] = ppower2[i];
            if (us.nRel == rcTransit)
                power2[i] = rTransitInf[i];
            if (us.nRel == rcProgress)
                power2[i] = ppower1[i];
        }

    for (i = 0; i <= oNormOpt; i++) if (FNorm(i)&&FProper2(i)) {
            Tension[i] = Harmon[i] = power3[i] = AspectSum[i] = Conjunct[i] = 0.0;
            for (j = 0; j <= cObjOpt; j++) if (FProper2(j)) {
                    if (grid->n[i][j]) {
                        k = grid->n[i][j];
                        ip = FNorm(i) ? power2[i] : 2.5;
                        jp = FNorm(j) ? ppower1[j] : 2.5;
                        valt = 2.0*(rAspInf[k]*(sqrt)(ip*jp)*
                                    (1.0-RAbs((real)(grid->v[i][j]))/3600.0/GetOrb(i, j, k)));
                        if (us.fParallel)
                            valt = valt / 4.0;
                        if (k == aOpp || k == aSqu || k == aSSq || k == aSes || k == aInc)
                            Tension [i] -= valt;
                        if (k == aSex || k == aTri || k == aSSx || k == aQui || k == aBQn)
                            Harmon[i] += valt;
                        if (k == aCon)
                            Conjunct[i] += valt;
                        AspectSum[i] += valt;
                    }
                }
            power3[i] = Tension[i] + Harmon[i];
        }
    total4 = total1 = total2 = total3 = total5 =0.0;
    for (i = 0; i <= oNormOpt; i++) if (FNorm(i) && FProper2(i)) {
            total1 += Harmon[i];
            total2 += Tension[i];
            total3 += power3[i];
            total4 += AspectSum[i];
            total5 += Conjunct[i];
        }

    if (us.objCenter) {
        SortRank2(power3, rank1, oNormOpt);
        SortRank2(Conjunct, rank2, oNormOpt);
        SortRank2(AspectSum, rank, oNormOpt);
    } else {
        SortRank(power3, rank1, oNormOpt);
        SortRank(Conjunct, rank2, oNormOpt);
        SortRank(AspectSum, rank, oNormOpt);
    }

    PrintSz("     Planet:     Harm.     Tens.      Sum.           Conj.         Total\n");
    for (i = 0; i <= oNormOpt; i++) if (FNorm(i)&&FProper2(i)) {
            AnsiColor(kObjA[i]);
            sprintf(sz, "%11.11s: ", szObjName[i]);
            PrintSz(sz);
            sprintf(sz, "  %6.1f    %6.1f    %6.1f (%2d)    %6.1f (%2d)    %6.1f (%2d)\n",
                    Harmon[i], Tension[i], power3[i], rank1[i], Conjunct[i], rank2[i],
                    AspectSum[i], rank[i]);
            PrintSz(sz);
        }
    AnsiColor(kDefault);
    sprintf(sz, "      Total:   %6.1f    %6.1f    %6.1f         %6.1f         %6.1f\n",
            total1, total2, total3, total5, total4);
    PrintSz(sz);

}


/* Look, if planet[i] is listed as ruler planet in rules[]  */
/* Used by XChartDispositor() and by PlanetPPower()         */

int RulerPlanet(i)
int i;
{
    int j, IsRuler = 0;

    for (j = 1; j <= cSign; j++) {
        if (i == rules[j])
            IsRuler = 1;
    }
    return IsRuler;
}

void InterpretSabian()
{
    char sz[cchSzDef*3];
    int i, j,k;

    PrintL();
    for (i = 1; i <= cObjInt; i++) {
        if (ignore[i])
            continue;
        AnsiColor(kObjA[i]);
        j = SFromZ(planet[i]);
        k = (int)(planet[i]);
        sprintf(sz, "%s%s%s%s in %2d%s", ret[i] < 0.0 ? "Retrograde " : "",
                i == oNod ? "North " : (i == oFor ? "Part of " : ""), szObjName[i],
                i == 13 ? " Athena" : "", (k%30), szSignName[j]);
        PrintSz(sz);
        PrintL();
        sprintf(sz, "%s",szSabian[k]);
        PrintSz(sz);
        PrintL();
    }
}
/* intrpret.c */
