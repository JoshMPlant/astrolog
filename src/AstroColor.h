/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


#ifndef AstroColor_included
#define AstroColor_included
#include <qcolor.h>
#include <qlist.h>

class AstroColor : public QColor
{
public:
  int index;
  char name[20];

  AstroColor(int r, int g, int b, int index, char *name);
};

#define RGBFILE "/usr/X11R6/lib/X11/rgb.txt"

class AstroColorList : public QList<AstroColor> // A list of AstroColors
{
public:

  AstroColorList();
  ~AstroColorList();
  char* GetName(int i); //Give the color name from its index
  AstroColor& GetColor(int i); // Give the color ref from its index
  AstroColor& GetColor(const char *); // Give the color ref from its name
};
#endif // AstroColor_included







