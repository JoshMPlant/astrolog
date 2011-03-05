/*
** Astrolog (Version 5.42J) File: riyal.c
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
#include "config.h"
#ifdef ASTEROIDS
#include "swephexp.h"
#include "sweph.h"
#include "swephlib.h"
#define SEFLG_EPHMASK	(SEFLG_JPLEPH|SEFLG_SWIEPH|SEFLG_MOSEPH)


/* mean elements for Mercury - Neptune from VSOP87 (mean equinox of date) */
static double el_node[8][4] =
{{ 48.330893,  1.1861890,  0.00017587,  0.000000211,}, /* Mercury */
  { 76.679920,  0.9011190,  0.00040665, -0.000000080,}, /* Venus   */
  {  0       ,  0        ,  0         ,  0          ,}, /* Earth   */
  { 49.558093,  0.7720923,  0.00001605,  0.000002325,}, /* Mars    */
  {100.464441,  1.0209550,  0.00040117,  0.000000569,}, /* Jupiter */
  {113.665524,  0.8770970, -0.00012067, -0.000002380,}, /* Saturn  */
  { 74.005947,  0.5211258,  0.00133982,  0.000018516,}, /* Uranus  */
  {131.784057,  1.1022057,  0.00026006, -0.000000636,}, /* Neptune */
};
static double el_peri[8][4] =
{{ 77.456119,  1.5564775,  0.00029589,  0.000000056,}, /* Mercury */
  {131.563707,  1.4022188, -0.00107337, -0.000005315,}, /* Venus   */
  {102.937348,  1.7195269,  0.00045962,  0.000000499,}, /* Earth   */
  {336.060234,  1.8410331,  0.00013515,  0.000000318,}, /* Mars    */
  { 14.331309,  1.6126668,  0.00103127, -0.000004569,}, /* Jupiter */
  { 93.056787,  1.9637694,  0.00083757,  0.000004899,}, /* Saturn  */
  {173.005159,  1.4863784,  0.00021450,  0.000000433,}, /* Uranus  */
  { 48.123691,  1.4262677,  0.00037918, -0.000000003,}, /* Neptune */
};
static double el_incl[8][4] =
{{  7.004986,  0.0018215, -0.00001809,  0.000000053,}, /* Mercury */
  {  3.394662,  0.0010037, -0.00000088, -0.000000007,}, /* Venus   */
  {  0,         0,          0,           0          ,}, /* Earth   */
  {  1.849726, -0.0006010,  0.00001276, -0.000000006,}, /* Mars    */
  {  1.303270, -0.0054966,  0.00000465, -0.000000004,}, /* Jupiter */
  {  2.488878, -0.0037363, -0.00001516,  0.000000089,}, /* Saturn  */
  {  0.773196,  0.0007744,  0.00003749, -0.000000092,}, /* Uranus  */
  {  1.769952, -0.0093082, -0.00000708,  0.000000028,}, /* Neptune */
};
static double el_ecce[8][4] =
{{  0.20563175,  0.000020406, -0.0000000284, -0.00000000017,}, /* Mercury */
  {  0.00677188, -0.000047766,  0.0000000975,  0.00000000044,}, /* Venus   */
  {  0.01670862, -0.000042037, -0.0000001236,  0.00000000004,}, /* Earth   */
  {  0.09340062,  0.000090483, -0.0000000806, -0.00000000035,}, /* Mars    */
  {  0.04849485,  0.000163244, -0.0000004719, -0.00000000197,}, /* Jupiter */
  {  0.05550862, -0.000346818, -0.0000006456,  0.00000000338,}, /* Saturn  */
  {  0.04629590, -0.000027337,  0.0000000790,  0.00000000025,}, /* Uranus  */
  {  0.00898809,  0.000006408, -0.0000000008, -0.00000000005,}, /* Neptune */
};
static double el_sema[8][4] =
{{  0.387098310,  0.0,  0.0,  0.0,}, /* Mercury */
  {  0.723329820,  0.0,  0.0,  0.0,}, /* Venus   */
  {  1.000001018,  0.0,  0.0,  0.0,}, /* Earth   */
  {  1.523679342,  0.0,  0.0,  0.0,}, /* Mars    */
  {  5.202603191,  0.0000001913,  0.0,  0.0,}, /* Jupiter */
  {  9.554909596,  0.0000021389,  0.0,  0.0,}, /* Saturn  */
  { 19.218446062, -0.0000000372,  0.00000000098,  0.0,}, /* Uranus  */
  { 30.110386869, -0.0000001663,  0.00000000069,  0.0,}, /* Neptune */
};
/* Ratios of mass of Sun to masses of the planets */
static double plmass[9] =
{
  6023600,        /* Mercury */
  408523.5,      /* Venus */
  328900.5,      /* Earth and Moon */
  3098710,        /* Mars */
  1047.350,    /* Jupiter */
  3498.0,      /* Saturn */
  22960,        /* Uranus */
  19314,        /* Neptune */
  130000000,        /* Pluto */
};
static int ipl_to_elem[15] = {2, 0, 0, 1, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 2,};


int swe_sx_nod_aps(double tjd_et, int ind, int iflag,
                   int  method,
                   double *xnasc, double *xndsc,
                   double *xperi, double *xaphe,
                   char *serr)
{
  int ij, i, j,ipl,fEquatorT,fUseJ2000T;
  int32 iplx;
  int32 ipli;
  int istart, iend;
  int32 iflJ2000;
  double plm;
  double t = (tjd_et - J2000) / 36525, dt;
  double x[6], xx[24], *xp, xobs[6], x2000[6];
  double xpos[3][6], xnorm[6];
  double xposm[6];
  double xn[3][6], xs[3][6];
  double xq[3][6], xa[3][6];
  double xobs2[6], x2[6];
  double *xna, *xnd, *xpe, *xap;
  double incl, sema, ecce, parg, ea, vincl, vsema, vecce, pargx, eax;
  struct plan_data *pedp = &swed.pldat[SEI_EARTH];
  struct plan_data *psbdp = &swed.pldat[SEI_SUNBARY];
  struct plan_data pldat;
  double *xsun = psbdp->x;
  double *xear = pedp->x;
  double *ep;
  double Gmsm, dzmin;
  double rxy, rxyz, fac, sgn;
  double sinnode, cosnode, sinincl, cosincl, sinu, cosu, sinE, cosE, cosE2;
  double uu, ny, ny2, c2, v2, pp, ro, ro2, rn, rn2;
  struct epsilon *oe;
  AS_BOOL is_true_nodaps = FALSE;
  AS_BOOL do_aberr = !(iflag & (SEFLG_TRUEPOS | SEFLG_NOABERR));
  AS_BOOL do_defl = !(iflag & SEFLG_TRUEPOS) && !(iflag & SEFLG_NOGDEFL);
  AS_BOOL do_focal_point = method & SE_NODBIT_FOPOINT;
  AS_BOOL ellipse_is_bary = FALSE;
  int32 iflg0;
  /* function calls for Pluto with asteroid number 134340
   * are treated as calls for Pluto as main body SE_PLUTO */
  ipl = AlToSweObj(ind);
  if (ipl == SE_WHITE_MOON)
    ipl = SE_AST_OFFSET+MaxNumberedAsteroid;
  if (ipl == SE_AST_OFFSET + 134340)
    ipl = SE_PLUTO;
  xna = xx;
  xnd = xx+6;
  xpe = xx+12;
  xap = xx+18;
  xpos[0][0] = 0; /* to shut up mint */
  /* to get control over the save area: */
  swi_force_app_pos_etc();
  method %= SE_NODBIT_FOPOINT;
  ipli = ipl;
  if (ipl == SE_SUN)
    ipli = SE_EARTH;
  if (ipl == SE_MOON)
    {
      do_defl = FALSE;
      if (!(iflag & SEFLG_HELCTR))
        do_aberr = FALSE;
    }
  iflg0 = (iflag & (SEFLG_EPHMASK|SEFLG_NONUT)) | SEFLG_SPEED | SEFLG_TRUEPOS;
  if (ipli != SE_MOON)
    iflg0 |= SEFLG_HELCTR;
  if (ipl == SE_MEAN_NODE || ipl == SE_TRUE_NODE ||
      ipl == SE_MEAN_APOG || ipl == SE_OSCU_APOG ||
      ipl < 0 ||
      (ipl >= SE_NPLANETS && ipl <= SE_AST_OFFSET))
    {
      /*(ipl >= SE_FICT_OFFSET && ipl - SE_FICT_OFFSET < SE_NFICT_ELEM)) */
      if (serr != NULL)
        sprintf(serr, "nodes/apsides for planet %5.0f are not implemented", (double) ipl);
      if (xnasc != NULL)
        for (i = 0; i <= 5; i++)
          xnasc[i] = 0;
      if (xndsc != NULL)
        for (i = 0; i <= 5; i++)
          xndsc[i] = 0;
      if (xaphe != NULL)
        for (i = 0; i <= 5; i++)
          xaphe[i] = 0;
      if (xperi != NULL)
        for (i = 0; i <= 5; i++)
          xperi[i] = 0;
      return ERR;
    }
  for (i = 0; i < 24; i++)
    xx[i] = 0;
  /***************************************
   * mean nodes and apsides
   ***************************************/
  /* mean points only for Sun - Neptune */
  if ((method == 0 || (method & SE_NODBIT_MEAN)) &&
      ((ipl >= SE_SUN && ipl <= SE_NEPTUNE) || ipl == SE_EARTH))
    {
      if (ipl == SE_MOON)
        {
          swi_mean_lunar_elements(tjd_et, &xna[0], &xna[3], &xpe[0], &xpe[3]);
          incl = MOON_MEAN_INCL;
          vincl = 0;
          ecce = MOON_MEAN_ECC;
          vecce = 0;
          sema = MOON_MEAN_DIST / AUNIT;
          vsema = 0;
        }
      else
        {
          iplx = ipl_to_elem[ipl];
          ep = el_incl[iplx];
          incl = ep[0] + ep[1] * t + ep[2] * t * t + ep[3] * t * t * t;
          vincl = ep[1] / 36525;
          ep = el_sema[iplx];
          sema = ep[0] + ep[1] * t + ep[2] * t * t + ep[3] * t * t * t;
          vsema = ep[1] / 36525;
          ep = el_ecce[iplx];
          ecce = ep[0] + ep[1] * t + ep[2] * t * t + ep[3] * t * t * t;
          vecce = ep[1] / 36525;
          ep = el_node[iplx];
          /* ascending node */
          xna[0] = ep[0] + ep[1] * t + ep[2] * t * t + ep[3] * t * t * t;
          xna[3] = ep[1] / 36525;
          /* perihelion */
          ep = el_peri[iplx];
          xpe[0] = ep[0] + ep[1] * t + ep[2] * t * t + ep[3] * t * t * t;
          xpe[3] = ep[1] / 36525;
        }
      /* descending node */
      xnd[0] = swe_degnorm(xna[0] + 180);
      xnd[3] = xna[3];
      /* angular distance of perihelion from node */
      parg = xpe[0] = swe_degnorm(xpe[0] - xna[0]);
      pargx = xpe[3] = swe_degnorm(xpe[0] + xpe[3]  - xna[3]);
      /* transform from orbital plane to mean ecliptic of date */
      swe_cotrans(xpe, xpe, -incl);
      /* xpe+3 is aux. position, not speed!!! */
      swe_cotrans(xpe+3, xpe+3, -incl-vincl);
      /* add node again */
      xpe[0] = swe_degnorm(xpe[0] + xna[0]);
      /* xpe+3 is aux. position, not speed!!! */
      xpe[3] = swe_degnorm(xpe[3] + xna[0] + xna[3]);
      /* speed */
      xpe[3] = swe_degnorm(xpe[3] - xpe[0]);
      /* heliocentric distance of perihelion and aphelion */
      xpe[2] = sema * (1 - ecce);
      xpe[5] = (sema + vsema) * (1 - ecce - vecce) - xpe[2];
      /* aphelion */
      xap[0] = swe_degnorm(xpe[0] + 180);
      xap[1] = -xpe[1];
      xap[3] = xpe[3];
      xap[4] = -xpe[4];
      if (do_focal_point)
        {
          xap[2] = sema * ecce * 2;
          xap[5] = (sema + vsema) * (ecce + vecce) * 2 - xap[2];
        }
      else
        {
          xap[2] = sema * (1 + ecce);
          xap[5] = (sema + vsema) * (1 + ecce + vecce) - xap[2];
        }
      /* heliocentric distance of nodes */
      ea = atan(tan(-parg * DEGTORAD / 2) * sqrt((1-ecce)/(1+ecce))) * 2;
      eax = atan(tan(-pargx * DEGTORAD / 2) * sqrt((1-ecce-vecce)/(1+ecce+vecce))) * 2;
      xna[2] = sema * (cos(ea) - ecce) / cos(parg * DEGTORAD);
      xna[5] = (sema+vsema) * (cos(eax) - ecce - vecce) / cos(pargx * DEGTORAD);
      xna[5] -= xna[2];
      ea = atan(tan((180 - parg) * DEGTORAD / 2) * sqrt((1-ecce)/(1+ecce))) * 2;
      eax = atan(tan((180 - pargx) * DEGTORAD / 2) * sqrt((1-ecce-vecce)/(1+ecce+vecce))) * 2;
      xnd[2] = sema * (cos(ea) - ecce) / cos((180 - parg) * DEGTORAD);
      xnd[5] = (sema+vsema) * (cos(eax) - ecce - vecce) / cos((180 - pargx) * DEGTORAD);
      xnd[5] -= xnd[2];
      /* no light-time correction because speed is extremely small */
      for (i = 0, xp = xx; i < 4; i++, xp += 6)
        {
          /* to cartesian coordinates */
          xp[0] *= DEGTORAD;
          xp[1] *= DEGTORAD;
          xp[3] *= DEGTORAD;
          xp[4] *= DEGTORAD;
          swi_polcart_sp(xp, xp);
        }
      /***************************************
       * "true" or osculating nodes and apsides
       ***************************************/
    } else {
      /* first, we need a heliocentric distance of the planet */
      if (swe_calc(tjd_et, ipli, iflg0, x, serr) == ERR)
        {
          if (FRiyalNumbered(tjd_et,ind,fTrue))
            {
              x[0]=planet[ind];
              x[1]=planetalt[ind];
              x[2]=planetdis[ind];
              x[3]=ret[ind];
              x[4]=altret[ind];
              x[5]=disret[ind];
            }
          else return ERR;
        }
      iflJ2000 = (iflag & SEFLG_EPHMASK)|SEFLG_J2000|SEFLG_EQUATORIAL|SEFLG_XYZ|SEFLG_TRUEPOS|SEFLG_NONUT|SEFLG_SPEED;
      ellipse_is_bary = FALSE;
      if (ipli != SE_MOON)
        {
          if ((method & SE_NODBIT_OSCU_BAR) && x[2] > 6)
            {
              iflJ2000 |= SEFLG_BARYCTR; /* only planets beyond Jupiter */
              ellipse_is_bary = TRUE;
            }
          else
            {
              iflJ2000 |= SEFLG_HELCTR;
            }
        }
      /* we need three positions and three speeds
       * for three nodes/apsides. from the three node positions,
       * the speed of the node will be computed. */
      if (ipli == SE_MOON)
        {
          dt = NODE_CALC_INTV;
          dzmin = 1e-15;
          Gmsm = GEOGCONST * (1 + 1 / EARTH_MOON_MRAT) /AUNIT/AUNIT/AUNIT*86400.0*86400.0;
        }
      else
        {
          if ((ipli >= SE_MERCURY && ipli <= SE_PLUTO) || ipli == SE_EARTH)
            plm = 1 / plmass[ipl_to_elem[ipl]];
          else
            plm = 0;
          dt = NODE_CALC_INTV * 10 * x[2];
          dzmin = 1e-15 * dt / NODE_CALC_INTV;
          Gmsm = HELGRAVCONST * (1 + plm) /AUNIT/AUNIT/AUNIT*86400.0*86400.0;
        }
      if (iflag & SEFLG_SPEED)
        {
          istart = 0;
          iend = 2;
        }
      else
        {
          istart = iend = 0;
          dt = 0;
        }
      for (i = istart, t = tjd_et - dt; i <= iend; i++, t += dt)
        {
          if (istart == iend)
            t = tjd_et;
          if (swe_calc(t, ipli, iflJ2000, xpos[i], serr) == ERR)
            {
              fUseJ2000T=fUseJ2000;
              fEquatorT=us.fEquator;
              fUseJ2000=fTrue;
              us.fEquator=fTrue;
              if (FRiyalNumbered(t,ind,fTrue))
                {
                  xpos[i][0]=spacex[ind];
                  xpos[i][1]=spacey[ind];
                  xpos[i][2]=spacez[ind];
                  xpos[i][3]=spacedx[ind];
                  xpos[i][4]=spacedy[ind];
                  xpos[i][5]=spacedz[ind];
              fUseJ2000=fUseJ2000T;
              us.fEquator=fEquatorT;
                }
              else return ERR;
            }
          /* the EMB is used instead of the earth */
          if (ipli == SE_EARTH)
            {
              if (swe_calc(t, SE_MOON, iflJ2000 & ~(SEFLG_BARYCTR|SEFLG_HELCTR), xposm, serr) == ERR)
                return ERR;
              for (j = 0; j <= 2; j++)
                xpos[i][j] += xposm[j] / (EARTH_MOON_MRAT + 1.0);
            }
          swi_plan_for_osc_elem(iflg0, t, xpos[i]);
        }
      for (i = istart; i <= iend; i++)
        {
          if (fabs(xpos[i][5]) < dzmin)
            xpos[i][5] = dzmin;
          fac = xpos[i][2] / xpos[i][5];
          sgn = xpos[i][5] / fabs(xpos[i][5]);
          for (j = 0; j <= 2; j++)
            {
              xn[i][j] = (xpos[i][j] - fac * xpos[i][j+3]) * sgn;
              xs[i][j] = -xn[i][j];
            }
        }
      for (i = istart; i <= iend; i++)
        {
          /* node */
          rxy =  sqrt(xn[i][0] * xn[i][0] + xn[i][1] * xn[i][1]);
          cosnode = xn[i][0] / rxy;
          sinnode = xn[i][1] / rxy;
          /* inclination */
          swi_cross_prod(xpos[i], xpos[i]+3, xnorm);
          rxy =  xnorm[0] * xnorm[0] + xnorm[1] * xnorm[1];
          c2 = (rxy + xnorm[2] * xnorm[2]);
          rxyz = sqrt(c2);
          rxy = sqrt(rxy);
          sinincl = rxy / rxyz;
          cosincl = sqrt(1 - sinincl * sinincl);
          if (xnorm[2] < 0) cosincl = -cosincl; /* retrograde asteroid, e.g. 20461 Dioretsa */
          /* argument of latitude */
          cosu = xpos[i][0] * cosnode + xpos[i][1] * sinnode;
          sinu = xpos[i][2] / sinincl;
          uu = atan2(sinu, cosu);
          /* semi-axis */
          rxyz = sqrt(square_sum(xpos[i]));
          v2 = square_sum((xpos[i]+3));
          sema = 1 / (2 / rxyz - v2 / Gmsm);
          /* eccentricity */
          pp = c2 / Gmsm;
          ecce = sqrt(1 - pp / sema);
          /* eccentric anomaly */
          cosE = 1 / ecce * (1 - rxyz / sema);
          sinE = 1 / ecce / sqrt(sema * Gmsm) * dot_prod(xpos[i], (xpos[i]+3));
          /* true anomaly */
          ny = 2 * atan(sqrt((1+ecce)/(1-ecce)) * sinE / (1 + cosE));
          /* distance of perihelion from ascending node */
          xq[i][0] = swi_mod2PI(uu - ny);
          xq[i][1] = 0;			/* latitude */
          xq[i][2] = sema * (1 - ecce);	/* distance of perihelion */
          /* transformation to ecliptic coordinates */
          swi_polcart(xq[i], xq[i]);
          swi_coortrf2(xq[i], xq[i], -sinincl, cosincl);
          swi_cartpol(xq[i], xq[i]);
          /* adding node, we get perihelion in ecl. coord. */
          xq[i][0] += atan2(sinnode, cosnode);
          xa[i][0] = swi_mod2PI(xq[i][0] + PI);
          xa[i][1] = -xq[i][1];
          if (do_focal_point)
            {
              xa[i][2] = sema * ecce * 2;	/* distance of aphelion */
            }
          else
            {
              xa[i][2] = sema * (1 + ecce);	/* distance of aphelion */
            }
          swi_polcart(xq[i], xq[i]);
          swi_polcart(xa[i], xa[i]);
          /* new distance of node from orbital ellipse:
           * true anomaly of node: */
          ny = swi_mod2PI(ny - uu);
          ny2 = swi_mod2PI(ny + PI);
          /* eccentric anomaly */
          cosE = cos(2 * atan(tan(ny / 2) / sqrt((1+ecce) / (1-ecce))));
          cosE2 = cos(2 * atan(tan(ny2 / 2) / sqrt((1+ecce) / (1-ecce))));
          /* new distance */
          rn = sema * (1 - ecce * cosE);
          rn2 = sema * (1 - ecce * cosE2);
          /* old node distance */
          ro = sqrt(square_sum(xn[i]));
          ro2 = sqrt(square_sum(xs[i]));
          /* correct length of position vector */
          for (j = 0; j <= 2; j++)
            {
              xn[i][j] *= rn / ro;
              xs[i][j] *= rn2 / ro2;
            }
        }
      for (i = 0; i <= 2; i++)
        {
          if (iflag & SEFLG_SPEED)
            {
              xpe[i] = xq[1][i];
              xpe[i+3] = (xq[2][i] - xq[0][i]) / dt / 2;
              xap[i] = xa[1][i];
              xap[i+3] = (xa[2][i] - xa[0][i]) / dt / 2;
              xna[i] = xn[1][i];
              xna[i+3] = (xn[2][i] - xn[0][i]) / dt / 2;
              xnd[i] = xs[1][i];
              xnd[i+3] = (xs[2][i] - xs[0][i]) / dt / 2;
            }
          else
            {
              xpe[i] = xq[0][i];
              xpe[i+3] = 0;
              xap[i] = xa[0][i];
              xap[i+3] = 0;
              xna[i] = xn[0][i];
              xna[i+3] = 0;
              xnd[i] = xs[0][i];
              xnd[i+3] = 0;
            }
        }
      is_true_nodaps = TRUE;
    }
  /* to set the variables required in the save area,
   * i.e. ecliptic, nutation, barycentric sun, earth
   * we compute the planet */
  if (ipli == SE_MOON && (iflag & (SEFLG_HELCTR | SEFLG_BARYCTR)))
    {
      swi_force_app_pos_etc();
      if (swe_calc(tjd_et, SE_SUN, iflg0, x, serr) == ERR)
        return ERR;
      } else {
          if (swe_calc(tjd_et, ipli, iflg0 | (iflag & SEFLG_TOPOCTR), x, serr) == ERR)
            {
              if (FRiyalNumbered(tjd_et,ind,iflg0 && SEFLG_HELCTR))
                {
                  x[0]=planet[ind];
                  x[1]=planetalt[ind];
                  x[2]=planetdis[ind];
                  x[3]=ret[ind];
                  x[4]=altret[ind];
                  x[5]=disret[ind];
                }
              else return ERR;
            }
            }
          /***********************
           * position of observer
           ***********************/
          if (iflag & SEFLG_TOPOCTR)
            {
              /* geocentric position of observer */
              if (swi_get_observer(tjd_et, iflag, FALSE, xobs, serr) != OK)
                return ERR;
              /*for (i = 0; i <= 5; i++)
                xobs[i] = swed.topd.xobs[i];*/
            }
          else
            {
              for (i = 0; i <= 5; i++)
                xobs[i] = 0;
            }
          if (iflag & (SEFLG_HELCTR | SEFLG_BARYCTR))
            {
              if ((iflag & SEFLG_HELCTR) && !(iflag & SEFLG_MOSEPH))
                for (i = 0; i <= 5; i++)
                  xobs[i] = xsun[i];
            }
          else if (ipl == SE_SUN && !(iflag & SEFLG_MOSEPH))
            {
              for (i = 0; i <= 5; i++)
                xobs[i] = xsun[i];
            }
          else
            {
              /* barycentric position of observer */
              for (i = 0; i <= 5; i++)
                xobs[i] += xear[i];
            }
          /* ecliptic obliqity */
          if (iflag & SEFLG_J2000)
            oe = &swed.oec2000;
          else
            oe = &swed.oec;
          /*************************************************
           * conversions shared by mean and osculating points
           *************************************************/
          for (ij = 0, xp = xx; ij < 4; ij++, xp += 6)
            {
              /* no nodes for earth */
              if (ipli == SE_EARTH && ij <= 1)
                {
                  for (i = 0; i <= 5; i++)
                    xp[i] = 0;
                  continue;
                }
              /*********************
               * to equator
               *********************/
              if (is_true_nodaps && !(iflag & SEFLG_NONUT))
                {
                  swi_coortrf2(xp, xp, -swed.nut.snut, swed.nut.cnut);
                  if (iflag & SEFLG_SPEED)
                    swi_coortrf2(xp+3, xp+3, -swed.nut.snut, swed.nut.cnut);
                }
              swi_coortrf2(xp, xp, -oe->seps, oe->ceps);
              swi_coortrf2(xp+3, xp+3, -oe->seps, oe->ceps);
              if (is_true_nodaps)
                {
                  /****************************
                   * to mean ecliptic of date
                   ****************************/
                  if (!(iflag & SEFLG_NONUT))
                    swi_nutate(xp, iflag, TRUE);
                }
              /*********************
               * to J2000
               *********************/
              swi_precess(xp, tjd_et, J_TO_J2000);
              if (iflag & SEFLG_SPEED)
                swi_precess_speed(xp, tjd_et, J_TO_J2000);
              /*********************
               * to barycenter
               *********************/
              if (ipli == SE_MOON)
                {
                  for (i = 0; i <= 5; i++)
                    xp[i] += xear[i];
                }
              else
                {
                  if (!(iflag & SEFLG_MOSEPH) && !ellipse_is_bary)
                    for (j = 0; j <= 5; j++)
                      xp[j] += xsun[j];
                }
              /*********************
               * to correct center
               *********************/
              for (j = 0; j <= 5; j++)
                xp[j] -= xobs[j];
              /* geocentric perigee/apogee of sun */
              if (ipl == SE_SUN && !(iflag & (SEFLG_HELCTR | SEFLG_BARYCTR)))
                for (j = 0; j <= 5; j++)
                  xp[j] = -xp[j];
              /*********************
               * light deflection
               *********************/
              dt = sqrt(square_sum(xp)) * AUNIT / CLIGHT / 86400.0;
              if (do_defl)
                swi_deflect_light(xp, dt, iflag);
              /*********************
               * aberration
               *********************/
              if (do_aberr)
                {
                  swi_aberr_light(xp, xobs, iflag);
                  /*
                   * Apparent speed is also influenced by
                   * the difference of speed of the earth between t and t-dt.
                   * Neglecting this would result in an error of several 0.1"
                   */
                  if (iflag & SEFLG_SPEED)
                    {
                      /* get barycentric sun and earth for t-dt into save area */
                      if (swe_calc(tjd_et - dt, ipli, iflg0 | (iflag & SEFLG_TOPOCTR), x2, serr) == ERR)
        {
          if (FRiyalNumbered(tjd_et-dt,ind,iflg0 && SEFLG_HELCTR))
            {
              x2[0]=planet[ind];
              x2[1]=planetalt[ind];
              x2[2]=planetdis[ind];
              x2[3]=ret[ind];
              x2[4]=altret[ind];
              x2[5]=disret[ind];
            }
          else return ERR;
        }
                      if (iflag & SEFLG_TOPOCTR)
                        {
                          /* geocentric position of observer */
                          /* if (swi_get_observer(tjd_et - dt, iflag, FALSE, xobs, serr) != OK)
                            return ERR;*/
                          for (i = 0; i <= 5; i++)
                            xobs2[i] = swed.topd.xobs[i];
                        }
                      else
                        {
                          for (i = 0; i <= 5; i++)
                            xobs2[i] = 0;
                        }
                      if (iflag & (SEFLG_HELCTR | SEFLG_BARYCTR))
                        {
                          if ((iflag & SEFLG_HELCTR) && !(iflag & SEFLG_MOSEPH))
                            for (i = 0; i <= 5; i++)
                              xobs2[i] = xsun[i];
                        }
                      else if (ipl == SE_SUN && !(iflag & SEFLG_MOSEPH))
                        {
                          for (i = 0; i <= 5; i++)
                            xobs2[i] = xsun[i];
                        }
                      else
                        {
                          /* barycentric position of observer */
                          for (i = 0; i <= 5; i++)
                            xobs2[i] += xear[i];
                        }
                      for (i = 3; i <= 5; i++)
                        xp[i] += xobs[i] - xobs2[i];
                      /* The above call of swe_calc() has destroyed the
                       * parts of the save area
                       * (i.e. bary sun, earth nutation matrix!).
                       * to restore it:
                       */
                      if (swe_calc(tjd_et, SE_SUN, iflg0 | (iflag & SEFLG_TOPOCTR), x2, serr) == ERR)
                        return ERR;
                    }
                }
              /*********************
               * precession
               *********************/
              /* save J2000 coordinates; required for sidereal positions */
              for (j = 0; j <= 5; j++)
                x2000[j] = xp[j];
              if (!(iflag & SEFLG_J2000))
                {
                  swi_precess(xp, tjd_et, J2000_TO_J);
                  if (iflag & SEFLG_SPEED)
                    swi_precess_speed(xp, tjd_et, J2000_TO_J);
                }
              /*********************
               * nutation
               *********************/
              if (!(iflag & SEFLG_NONUT))
                swi_nutate(xp, iflag, FALSE);
              /* now we have equatorial cartesian coordinates; keep them */
              for (j = 0; j <= 5; j++)
                pldat.xreturn[18+j] = xp[j];
              /************************************************
               * transformation to ecliptic.                  *
               * with sidereal calc. this will be overwritten *
               * afterwards.                                  *
               ************************************************/
              swi_coortrf2(xp, xp, oe->seps, oe->ceps);
              if (iflag & SEFLG_SPEED)
                swi_coortrf2(xp+3, xp+3, oe->seps, oe->ceps);
              if (!(iflag & SEFLG_NONUT))
                {
                  swi_coortrf2(xp, xp, swed.nut.snut, swed.nut.cnut);
                  if (iflag & SEFLG_SPEED)
                    swi_coortrf2(xp+3, xp+3, swed.nut.snut, swed.nut.cnut);
                }
              /* now we have ecliptic cartesian coordinates */
              for (j = 0; j <= 5; j++)
                pldat.xreturn[6+j] = xp[j];
              /************************************
               * sidereal positions               *
               ************************************/
              if (iflag & SEFLG_SIDEREAL)
                {
                  /* project onto ecliptic t0 */
                  if (swed.sidd.sid_mode & SE_SIDBIT_ECL_T0)
                    {
                      if (swi_trop_ra2sid_lon(x2000, pldat.xreturn+6, pldat.xreturn+18, iflag, serr) != OK)
                        return ERR;
                      /* project onto solar system equator */
                    }
                  else if (swed.sidd.sid_mode & SE_SIDBIT_SSY_PLANE)
                    {
                      if (swi_trop_ra2sid_lon_sosy(x2000, pldat.xreturn+6, pldat.xreturn+18, iflag, serr) != OK)
                        return ERR;
                    }
                  else
                    {
                      /* traditional algorithm */
                      swi_cartpol_sp(pldat.xreturn+6, pldat.xreturn);
                      pldat.xreturn[0] -= swe_get_ayanamsa(tjd_et) * DEGTORAD;
                      swi_polcart_sp(pldat.xreturn, pldat.xreturn+6);
                    }
                }
              if ((iflag & SEFLG_XYZ) && (iflag & SEFLG_EQUATORIAL))
                {
                  for (j = 0; j <= 5; j++)
                    xp[j] = pldat.xreturn[18+j];
                  continue;
                }
              if (iflag & SEFLG_XYZ)
                {
                  for (j = 0; j <= 5; j++)
                    xp[j] = pldat.xreturn[6+j];
                  continue;
                }
              /************************************************
               * transformation to polar coordinates          *
               ************************************************/
              swi_cartpol_sp(pldat.xreturn+18, pldat.xreturn+12);
              swi_cartpol_sp(pldat.xreturn+6, pldat.xreturn);
              /**********************
               * radians to degrees *
               **********************/
              for (j = 0; j < 2; j++)
                {
                  pldat.xreturn[j] *= RADTODEG;		/* ecliptic */
                  pldat.xreturn[j+3] *= RADTODEG;
                  pldat.xreturn[j+12] *= RADTODEG;	/* equator */
                  pldat.xreturn[j+15] *= RADTODEG;
                }
              if (iflag & SEFLG_EQUATORIAL)
                {
                  for (j = 0; j <= 5; j++)
                    xp[j] = pldat.xreturn[12+j];
                  continue;
                }
              else
                {
                  for (j = 0; j <= 5; j++)
                    xp[j] = pldat.xreturn[j];
                  continue;
                }
            }
          for (i = 0; i <= 5; i++)
            {
              if (i > 2 && !(iflag & SEFLG_SPEED))
                xna[i] = xnd[i] = xpe[i] = xap[i] = 0;
              if (xnasc != NULL)
                xnasc[i] = xna[i];
              if (xndsc != NULL)
                xndsc[i] = xnd[i];
              if (xperi != NULL)
                xperi[i] = xpe[i];
              if (xaphe != NULL)
                xaphe[i] = xap[i];
            }
          return OK;
        }
#endif
