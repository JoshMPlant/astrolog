/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :         kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/

#include "myLineEdit.h"

void myLineEdit::focusInEvent(QFocusEvent* e)
{
  setSelection(0, strlen(text()));
  QLineEdit::focusInEvent(e);
}

void myLineEdit::focusOutEvent(QFocusEvent* e)
{
  emit deselect();
  QLineEdit::focusOutEvent(e);
  emit focusoutsignal();
}  
