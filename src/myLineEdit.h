/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :         kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/

#ifndef myLineEdit_included
#define myLineEdit_included

#include <qlined.h>

class myLineEdit: public QLineEdit
{

 Q_OBJECT

 public:

  inline myLineEdit(QWidget* parent = 0, const char* name = NULL):
    QLineEdit(parent, name)
  {
  }
  void focusInEvent(QFocusEvent *);
  void focusOutEvent(QFocusEvent *);

 signals:

  void focusoutsignal();
};  
#endif
