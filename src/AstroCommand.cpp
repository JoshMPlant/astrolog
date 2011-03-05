/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: AstroCommand.cpp
	Last generated: Mon Sep 20 19:22:27 1999

 *********************************************************************/

#include "AstroCommand.h"
#include "AstroWidget.h"

#define Inherited AstroCommandData

AstroCommand::AstroCommand(QWidget* parent, const char* name) : Inherited( parent, name )
{
  emit setCaption(transl("Enter Command Line"));
  theline->setFocus();
}


AstroCommand::~AstroCommand()
{
}

void AstroCommand::okslot()
{
  close(FALSE);
  central->RunLine(theline->text());
}

void AstroCommand::cancelslot()
{
  close(FALSE);
}













