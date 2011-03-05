/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 




#include "AstroColor.h"
#include <qmessagebox.h>
extern "C"
{
#include <stdio.h>
}


AstroColorList::AstroColorList()
{
  FILE *f;
  AstroColor *acptr;
  int i = 0, r, g, b;
  char *c;
  char buf[80];
  setAutoDelete(TRUE);
  if(!(f = fopen(RGBFILE, "r")))
  {
    snprintf("The database file %s doesn't exists. !", 80, RGBFILE);
    QMessageBox::critical(NULL, "File not found !", buf);
    return;
  }
  fgets(buf, 80, f); // discard first line
  while(!feof(f))
  {
    fscanf(f, "%d%d%d%%[\t ]", &r, &g, &b, buf); // get the red, green, blwe values
    fgets(buf, 80, f); // get the color name
    for(c = buf; *c; c++) // strip \n
      if(*c == '\n') *c = 0;
    acptr = new AstroColor(r, g, b, i++, buf);
    append(acptr);
    
  }
  fclose(f);
}

AstroColorList::~AstroColorList()
{
 clear();
}

char * AstroColorList::GetName(int i)
{
  AstroColor *acl;
  for(acl = first(); acl != NULL;  acl = next())
    if(acl->index == i)
      return acl->name;
  return NULL;
}

AstroColor& AstroColorList::GetColor(int i)
{
  AstroColor *acl;
  for(acl = first(); acl != NULL;  acl = next())
    if(acl->index == i)
      return *acl;
  return *first();
}

AstroColor& AstroColorList::GetColor(const char *s)
{
  AstroColor *acl;
  for(acl = first(); acl != NULL;  acl = next())
    if(!strcmp(acl->name, s))
      return *acl;
  return *first();
}

AstroColor::AstroColor(int r, int g, int b, int i, char *c) : QColor(r, g, b)
{
  index = i;
  strncpy(name, c, 20);
}
