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
** Initial programming 8/28,30, 9/10,13,16,20,23, 10/3,6,7, 11/7,10,21/1991.
** X Window graphics initially programmed 10/23-29/1991.
** PostScript graphics initially programmed 11/29-30/1992.
** Last code change made 12/20/1998.
** Modifications from version 5.40 to 5.41 are by Alois Treindl.
** Modifications from version 5.41 to 5.42 are by Nicolas Scharnagl

*/
#include "astrolog.h"
#include "config.h"
#include <string.h>
#include <ctype.h>

#ifdef GTOPO
void ConvertPgm(filename,filenameout)
char *filename,*filenameout;
{
    char ch=chNull,sz[cchSzDef],outmode[3],mode[3],szLine[cchSzMax],*argv[MAXSWITCHES];
    int j,k,argc;
    short i;
    FILE *file,*outfile;

    sprintf(mode,"r%s","");
    sprintf(outmode,"w%s","b");
    file=fopen(filename,mode);
    if (file == NULL){
        sprintf(sz, "File '%s' not found.", filename);
        PrintError(sz);
    }
    outfile=fopen(filenameout,outmode);
    if (outfile == NULL){
        sprintf(sz, "File '%s' cannot be opened for writing.", filenameout);
        PrintError(sz);
    }
    k=0;
    loop{
        while (!feof(file)&& (ch = getc(file)) < ' ')
            ;
        if (feof(file))
            break;
        k++;
        if (ch=='#'||k<=2)
            continue;
        for (szLine[0]=ch, j = 1; j < cchSzMax && !feof(file) &&
                (szLine[j] = getc(file)) >= ' '; j++)
            ;
        szLine[j] = chNull;
        /*sprintf(sz,"%s\n",szLine);PrintSz(sz);*/
        argc = NParseCommandLine(szLine, argv);
        i=atoi(argv[1]);
        /*  sprintf(sz,"%d \n",i);PrintSz(sz);*/
        putc(i%256,outfile);
        putc(i/256,outfile);
    }
    fclose(outfile);
    fclose(file);
    return;
}

void SetAlt(longi,lati,alti)
double *alti,longi,lati;
{
    char *filename=EPHE_DIR"/gtopo"; /* Western Europe */
    int x,y,c=0;
    real xx,yy;
    short z[4];
    char mode[3],sz[cchSzDef],a[2];
    FILE *file;
    xx=(DecToDeg(longi)-GTOPO_LONFR)*GTOPO_PREC;
    yy=(GTOPO_LATTO-DecToDeg(lati))*GTOPO_PREC;
    sprintf(mode,"r%s","");
    file=fopen(filename,mode);
    if (file == NULL){
        sprintf(sz, "File '%s' not found.", filename);
        PrintError(sz);
    }
    for (y=(int)yy ;y<=(int)yy+1;y++){
        for (x=(int)xx ;x<=(int)xx+1;x++){
            fseek(file,2*((GTOPO_PREC*(GTOPO_LONTO-GTOPO_LONFR)+1)*y+x),0);
            a[0]=getc(file);
            a[1]=getc(file);
            memcpy(&z[c],&a[0],2);
            c++;
        }
    }
    /*sprintf (sz,"%d \n",z);PrintSz(sz);*/
    *alti=RFract(yy)*RFract(xx)*(double)z[3]+(1.0-RFract(yy))*(1.0-RFract(xx))*(double)z[0]+
          RFract(xx)*(1-RFract(yy))*(double)z[1]+(1-RFract(xx))*RFract(yy)*(double)z[2];
    return;
}
#endif
