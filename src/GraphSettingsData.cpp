/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :        kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 


/**********************************************************************

	--- Qt Architect generated file ---

	File: GraphSettingsData.cpp
	Last generated: Tue Nov 16 15:58:08 1999

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "GraphSettingsData.h"

#define Inherited QDialog

#include <qlabel.h>
#include <qpushbt.h>
#include <qradiobt.h>

GraphSettingsData::GraphSettingsData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, TRUE, 0 )
{
	WheelChart = new QButtonGroup( this, "ButtonGroup_1" );
	WheelChart->setGeometry( 10, 160, 170, 130 );
	WheelChart->setMinimumSize( 0, 0 );
	WheelChart->setMaximumSize( 32767, 32767 );
	connect( WheelChart, SIGNAL(clicked(int)), SLOT(wheelchartslot(int)) );
	WheelChart->setFocusPolicy( QWidget::NoFocus );
	WheelChart->setBackgroundMode( QWidget::PaletteBackground );
	WheelChart->setFontPropagation( QWidget::NoChildren );
	WheelChart->setPalettePropagation( QWidget::NoChildren );
	WheelChart->setFrameStyle( 49 );
	WheelChart->setTitle( transl("Wheel chart rotation") );
	WheelChart->setAlignment( 1 );

	QLabel* qtarch_Label_1;
	qtarch_Label_1 = new QLabel( this, "Label_1" );
	qtarch_Label_1->setGeometry( 10, 10, 150, 20 );
	qtarch_Label_1->setMinimumSize( 0, 0 );
	qtarch_Label_1->setMaximumSize( 32767, 32767 );
	qtarch_Label_1->setFocusPolicy( QWidget::NoFocus );
	qtarch_Label_1->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_Label_1->setFontPropagation( QWidget::NoChildren );
	qtarch_Label_1->setPalettePropagation( QWidget::NoChildren );
	qtarch_Label_1->setText( transl("Horizontal chart size :") );
	qtarch_Label_1->setAlignment( 289 );
	qtarch_Label_1->setMargin( -1 );

	HChartSize = new myLineEdit( this, "LineEdit_1" );
	HChartSize->setGeometry( 230, 10, 60, 20 );
	HChartSize->setMinimumSize( 0, 0 );
	HChartSize->setMaximumSize( 32767, 32767 );
	connect( HChartSize, SIGNAL(returnPressed()), SLOT(hchartsizeslot()) );
	HChartSize->setFocusPolicy( QWidget::StrongFocus );
	HChartSize->setBackgroundMode( QWidget::PaletteBase );
	HChartSize->setFontPropagation( QWidget::NoChildren );
	HChartSize->setPalettePropagation( QWidget::NoChildren );
	HChartSize->setText( "" );
	HChartSize->setMaxLength( 32767 );
	HChartSize->setEchoMode( myLineEdit::Normal );
	HChartSize->setFrame( TRUE );

	QLabel* qtarch_Label_2;
	qtarch_Label_2 = new QLabel( this, "Label_2" );
	qtarch_Label_2->setGeometry( 10, 30, 140, 20 );
	qtarch_Label_2->setMinimumSize( 0, 0 );
	qtarch_Label_2->setMaximumSize( 32767, 32767 );
	qtarch_Label_2->setFocusPolicy( QWidget::NoFocus );
	qtarch_Label_2->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_Label_2->setFontPropagation( QWidget::NoChildren );
	qtarch_Label_2->setPalettePropagation( QWidget::NoChildren );
	qtarch_Label_2->setText( transl("Vertical chart size :") );
	qtarch_Label_2->setAlignment( 289 );
	qtarch_Label_2->setMargin( -1 );

	VChartSize = new myLineEdit( this, "LineEdit_2" );
	VChartSize->setGeometry( 230, 30, 60, 20 );
	VChartSize->setMinimumSize( 0, 0 );
	VChartSize->setMaximumSize( 32767, 32767 );
	connect( VChartSize, SIGNAL(returnPressed()), SLOT(vchartsizeslot()) );
	VChartSize->setFocusPolicy( QWidget::StrongFocus );
	VChartSize->setBackgroundMode( QWidget::PaletteBase );
	VChartSize->setFontPropagation( QWidget::NoChildren );
	VChartSize->setPalettePropagation( QWidget::NoChildren );
	VChartSize->setText( "" );
	VChartSize->setMaxLength( 32767 );
	VChartSize->setEchoMode( myLineEdit::Normal );
	VChartSize->setFrame( TRUE );

	QLabel* qtarch_Label_3;
	qtarch_Label_3 = new QLabel( this, "Label_3" );
	qtarch_Label_3->setGeometry( 10, 50, 210, 20 );
	qtarch_Label_3->setMinimumSize( 0, 0 );
	qtarch_Label_3->setMaximumSize( 32767, 32767 );
	qtarch_Label_3->setFocusPolicy( QWidget::NoFocus );
	qtarch_Label_3->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_Label_3->setFontPropagation( QWidget::NoChildren );
	qtarch_Label_3->setPalettePropagation( QWidget::NoChildren );
	qtarch_Label_3->setText( transl("Horizontal map degree rotation :") );
	qtarch_Label_3->setAlignment( 289 );
	qtarch_Label_3->setMargin( -1 );

	MapDegRot = new myLineEdit( this, "LineEdit_3" );
	MapDegRot->setGeometry( 230, 50, 60, 20 );
	MapDegRot->setMinimumSize( 0, 0 );
	MapDegRot->setMaximumSize( 32767, 32767 );
	connect( MapDegRot, SIGNAL(returnPressed()), SLOT(mapdegrotslot()) );
	MapDegRot->setFocusPolicy( QWidget::StrongFocus );
	MapDegRot->setBackgroundMode( QWidget::PaletteBase );
	MapDegRot->setFontPropagation( QWidget::NoChildren );
	MapDegRot->setPalettePropagation( QWidget::NoChildren );
	MapDegRot->setText( "" );
	MapDegRot->setMaxLength( 32767 );
	MapDegRot->setEchoMode( myLineEdit::Normal );
	MapDegRot->setFrame( TRUE );

	QLabel* qtarch_Label_4;
	qtarch_Label_4 = new QLabel( this, "Label_4" );
	qtarch_Label_4->setGeometry( 10, 70, 200, 20 );
	qtarch_Label_4->setMinimumSize( 0, 0 );
	qtarch_Label_4->setMaximumSize( 32767, 32767 );
	qtarch_Label_4->setFocusPolicy( QWidget::NoFocus );
	qtarch_Label_4->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_Label_4->setFontPropagation( QWidget::NoChildren );
	qtarch_Label_4->setPalettePropagation( QWidget::NoChildren );
	qtarch_Label_4->setText( transl("Vertical globe degree tilt :") );
	qtarch_Label_4->setAlignment( 289 );
	qtarch_Label_4->setMargin( -1 );

	GlobeTilt = new myLineEdit( this, "LineEdit_4" );
	GlobeTilt->setGeometry( 230, 70, 60, 20 );
	GlobeTilt->setMinimumSize( 0, 0 );
	GlobeTilt->setMaximumSize( 32767, 32767 );
	connect( GlobeTilt, SIGNAL(returnPressed()), SLOT(globetiltslot()) );
	GlobeTilt->setFocusPolicy( QWidget::StrongFocus );
	GlobeTilt->setBackgroundMode( QWidget::PaletteBase );
	GlobeTilt->setFontPropagation( QWidget::NoChildren );
	GlobeTilt->setPalettePropagation( QWidget::NoChildren );
	GlobeTilt->setText( "" );
	GlobeTilt->setMaxLength( 32767 );
	GlobeTilt->setEchoMode( myLineEdit::Normal );
	GlobeTilt->setFrame( TRUE );

	WorldMollewide = new QCheckBox( this, "CheckBox_1" );
	WorldMollewide->setGeometry( 10, 90, 250, 20 );
	WorldMollewide->setMinimumSize( 0, 0 );
	WorldMollewide->setMaximumSize( 32767, 32767 );
	WorldMollewide->setFocusPolicy( QWidget::TabFocus );
	WorldMollewide->setBackgroundMode( QWidget::PaletteBackground );
	WorldMollewide->setFontPropagation( QWidget::NoChildren );
	WorldMollewide->setPalettePropagation( QWidget::NoChildren );
	WorldMollewide->setText( transl("World map in Wollewide projection") );
	WorldMollewide->setAutoRepeat( FALSE );
	WorldMollewide->setAutoResize( FALSE );

	QLabel* qtarch_Label_5;
	qtarch_Label_5 = new QLabel( this, "Label_5" );
	qtarch_Label_5->setGeometry( 10, 110, 210, 20 );
	qtarch_Label_5->setMinimumSize( 0, 0 );
	qtarch_Label_5->setMaximumSize( 32767, 32767 );
	qtarch_Label_5->setFocusPolicy( QWidget::NoFocus );
	qtarch_Label_5->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_Label_5->setFontPropagation( QWidget::NoChildren );
	qtarch_Label_5->setPalettePropagation( QWidget::NoChildren );
	qtarch_Label_5->setText( transl("Animation delay in Msec :") );
	qtarch_Label_5->setAlignment( 289 );
	qtarch_Label_5->setMargin( -1 );

	AnimDel = new myLineEdit( this, "LineEdit_5" );
	AnimDel->setGeometry( 230, 110, 60, 20 );
	AnimDel->setMinimumSize( 0, 0 );
	AnimDel->setMaximumSize( 32767, 32767 );
	connect( AnimDel, SIGNAL(returnPressed()), SLOT(animdelslot()) );
	AnimDel->setFocusPolicy( QWidget::StrongFocus );
	AnimDel->setBackgroundMode( QWidget::PaletteBase );
	AnimDel->setFontPropagation( QWidget::NoChildren );
	AnimDel->setPalettePropagation( QWidget::NoChildren );
	AnimDel->setText( "" );
	AnimDel->setMaxLength( 32767 );
	AnimDel->setEchoMode( myLineEdit::Normal );
	AnimDel->setFrame( TRUE );

	UpdateScreen = new QCheckBox( this, "CheckBox_2" );
	UpdateScreen->setGeometry( 10, 130, 240, 20 );
	UpdateScreen->setMinimumSize( 0, 0 );
	UpdateScreen->setMaximumSize( 32767, 32767 );
	UpdateScreen->setFocusPolicy( QWidget::TabFocus );
	UpdateScreen->setBackgroundMode( QWidget::PaletteBackground );
	UpdateScreen->setFontPropagation( QWidget::NoChildren );
	UpdateScreen->setPalettePropagation( QWidget::NoChildren );
	UpdateScreen->setText( transl("Don't automatically update screen") );
	UpdateScreen->setAutoRepeat( FALSE );
	UpdateScreen->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_1;
	qtarch_RadioButton_1 = new QRadioButton( this, "RadioButton_1" );
	qtarch_RadioButton_1->setGeometry( 20, 180, 100, 20 );
	qtarch_RadioButton_1->setMinimumSize( 0, 0 );
	qtarch_RadioButton_1->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_1->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_1->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_1->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_1->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_1->setText( transl("None") );
	qtarch_RadioButton_1->setAutoRepeat( FALSE );
	qtarch_RadioButton_1->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_2;
	qtarch_RadioButton_2 = new QRadioButton( this, "RadioButton_2" );
	qtarch_RadioButton_2->setGeometry( 20, 200, 150, 20 );
	qtarch_RadioButton_2->setMinimumSize( 0, 0 );
	qtarch_RadioButton_2->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_2->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_2->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_2->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_2->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_2->setText( transl("Object at left edge") );
	qtarch_RadioButton_2->setAutoRepeat( FALSE );
	qtarch_RadioButton_2->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_3;
	qtarch_RadioButton_3 = new QRadioButton( this, "RadioButton_3" );
	qtarch_RadioButton_3->setGeometry( 20, 220, 150, 20 );
	qtarch_RadioButton_3->setMinimumSize( 0, 0 );
	qtarch_RadioButton_3->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_3->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_3->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_3->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_3->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_3->setText( transl("Object at top edge") );
	qtarch_RadioButton_3->setAutoRepeat( FALSE );
	qtarch_RadioButton_3->setAutoResize( FALSE );

	QLabel* qtarch_Label_6;
	qtarch_Label_6 = new QLabel( this, "Label_6" );
	qtarch_Label_6->setGeometry( 20, 240, 110, 20 );
	qtarch_Label_6->setMinimumSize( 0, 0 );
	qtarch_Label_6->setMaximumSize( 32767, 32767 );
	qtarch_Label_6->setFocusPolicy( QWidget::NoFocus );
	qtarch_Label_6->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_Label_6->setFontPropagation( QWidget::NoChildren );
	qtarch_Label_6->setPalettePropagation( QWidget::NoChildren );
	qtarch_Label_6->setText( transl("Use this planet :") );
	qtarch_Label_6->setAlignment( 289 );
	qtarch_Label_6->setMargin( -1 );

	Planet = new QComboBox( FALSE, this, "ComboBox_1" );
	Planet->setGeometry( 70, 260, 100, 20 );
	Planet->setMinimumSize( 0, 0 );
	Planet->setMaximumSize( 32767, 32767 );
	connect( Planet, SIGNAL(highlighted(int)), SLOT(planetslot(int)) );
	Planet->setFocusPolicy( QWidget::StrongFocus );
	Planet->setBackgroundMode( QWidget::PaletteBackground );
	Planet->setFontPropagation( QWidget::AllChildren );
	Planet->setPalettePropagation( QWidget::AllChildren );
	Planet->setSizeLimit( 10 );
	Planet->setAutoResize( FALSE );

	QPushButton* qtarch_PushButton_1;
	qtarch_PushButton_1 = new QPushButton( this, "PushButton_1" );
	qtarch_PushButton_1->setGeometry( 190, 220, 100, 30 );
	qtarch_PushButton_1->setMinimumSize( 0, 0 );
	qtarch_PushButton_1->setMaximumSize( 32767, 32767 );
	connect( qtarch_PushButton_1, SIGNAL(clicked()), SLOT(cancelslot()) );
	qtarch_PushButton_1->setFocusPolicy( QWidget::TabFocus );
	qtarch_PushButton_1->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_PushButton_1->setFontPropagation( QWidget::NoChildren );
	qtarch_PushButton_1->setPalettePropagation( QWidget::NoChildren );
	qtarch_PushButton_1->setText( transl("Cancel") );
	qtarch_PushButton_1->setAutoRepeat( FALSE );
	qtarch_PushButton_1->setAutoResize( FALSE );

	QPushButton* qtarch_PushButton_2;
	qtarch_PushButton_2 = new QPushButton( this, "PushButton_2" );
	qtarch_PushButton_2->setGeometry( 190, 260, 100, 30 );
	qtarch_PushButton_2->setMinimumSize( 0, 0 );
	qtarch_PushButton_2->setMaximumSize( 32767, 32767 );
	connect( qtarch_PushButton_2, SIGNAL(clicked()), SLOT(okslot()) );
	qtarch_PushButton_2->setFocusPolicy( QWidget::TabFocus );
	qtarch_PushButton_2->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_PushButton_2->setFontPropagation( QWidget::NoChildren );
	qtarch_PushButton_2->setPalettePropagation( QWidget::NoChildren );
	qtarch_PushButton_2->setText( transl("OK") );
	qtarch_PushButton_2->setAutoRepeat( FALSE );
	qtarch_PushButton_2->setAutoResize( FALSE );

	WheelChart->insert( qtarch_RadioButton_1 );
	WheelChart->insert( qtarch_RadioButton_2 );
	WheelChart->insert( qtarch_RadioButton_3 );

	resize( 300,300 );
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );
}


GraphSettingsData::~GraphSettingsData()
{
}
void GraphSettingsData::wheelchartslot(int)
{
}
void GraphSettingsData::hchartsizeslot()
{
}
void GraphSettingsData::vchartsizeslot()
{
}
void GraphSettingsData::mapdegrotslot()
{
}
void GraphSettingsData::globetiltslot()
{
}
void GraphSettingsData::animdelslot()
{
}
void GraphSettingsData::planetslot(int)
{
}
void GraphSettingsData::cancelslot()
{
}
void GraphSettingsData::okslot()
{
}