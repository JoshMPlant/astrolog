/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroCommand.h
	Last generated: Mon Sep 20 19:22:27 1999

 *********************************************************************/

#ifndef AstroCommand_included
#define AstroCommand_included

#include "AstroCommandData.h"

class AstroCommand : public AstroCommandData
{
    Q_OBJECT

public:

    AstroCommand
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~AstroCommand();

public slots:
      
      void okslot();
      void cancelslot(); 

};
#endif // AstroCommand_included
