/**********************************************************************

	--- Qt Architect generated file ---

	File: ObscureSettingsData.cpp
	Last generated: Thu Apr 27 15:06:08 2000

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "ObscureSettingsData.h"

#define Inherited QDialog

#include <qlabel.h>
#include <qpushbt.h>
#include <qradiobt.h>

ObscureSettingsData::ObscureSettingsData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, TRUE, 0 )
{
	RisSetRestr = new QButtonGroup( this, "ButtonGroup_6" );
	RisSetRestr->setGeometry( 10, 370, 460, 50 );
	RisSetRestr->setMinimumSize( 0, 0 );
	RisSetRestr->setMaximumSize( 32767, 32767 );
	connect( RisSetRestr, SIGNAL(clicked(int)), SLOT(restslot(int)) );
	RisSetRestr->setFocusPolicy( QWidget::NoFocus );
	RisSetRestr->setBackgroundMode( QWidget::PaletteBackground );
	RisSetRestr->setFontPropagation( QWidget::NoChildren );
	RisSetRestr->setPalettePropagation( QWidget::NoChildren );
	RisSetRestr->setFrameStyle( 49 );
	RisSetRestr->setTitle( transl("Rising and setting restrictions") );
	RisSetRestr->setAlignment( 1 );

	LiliGlyph = new QButtonGroup( this, "ButtonGroup_5" );
	LiliGlyph->setGeometry( 340, 260, 130, 70 );
	LiliGlyph->setMinimumSize( 0, 0 );
	LiliGlyph->setMaximumSize( 32767, 32767 );
	connect( LiliGlyph, SIGNAL(clicked(int)), SLOT(lilislot(int)) );
	LiliGlyph->setFocusPolicy( QWidget::NoFocus );
	LiliGlyph->setBackgroundMode( QWidget::PaletteBackground );
	LiliGlyph->setFontPropagation( QWidget::NoChildren );
	LiliGlyph->setPalettePropagation( QWidget::NoChildren );
	LiliGlyph->setFrameStyle( 49 );
	LiliGlyph->setTitle( transl("Lilith glyph") );
	LiliGlyph->setAlignment( 1 );

	PlutGlyph = new QButtonGroup( this, "ButtonGroup_4" );
	PlutGlyph->setGeometry( 340, 180, 130, 70 );
	PlutGlyph->setMinimumSize( 0, 0 );
	PlutGlyph->setMaximumSize( 32767, 32767 );
	connect( PlutGlyph, SIGNAL(clicked(int)), SLOT(plutslot(int)) );
	PlutGlyph->setFocusPolicy( QWidget::NoFocus );
	PlutGlyph->setBackgroundMode( QWidget::PaletteBackground );
	PlutGlyph->setFontPropagation( QWidget::NoChildren );
	PlutGlyph->setPalettePropagation( QWidget::NoChildren );
	PlutGlyph->setFrameStyle( 49 );
	PlutGlyph->setTitle( transl("Pluto glyph") );
	PlutGlyph->setAlignment( 1 );

	UraGlyph = new QButtonGroup( this, "ButtonGroup_3" );
	UraGlyph->setGeometry( 340, 100, 130, 70 );
	UraGlyph->setMinimumSize( 0, 0 );
	UraGlyph->setMaximumSize( 32767, 32767 );
	connect( UraGlyph, SIGNAL(clicked(int)), SLOT(uraslot(int)) );
	UraGlyph->setFocusPolicy( QWidget::NoFocus );
	UraGlyph->setBackgroundMode( QWidget::PaletteBackground );
	UraGlyph->setFontPropagation( QWidget::NoChildren );
	UraGlyph->setPalettePropagation( QWidget::NoChildren );
	UraGlyph->setFrameStyle( 49 );
	UraGlyph->setTitle( transl("Uranus glyph") );
	UraGlyph->setAlignment( 1 );

	CaprGlyph = new QButtonGroup( this, "ButtonGroup_2" );
	CaprGlyph->setGeometry( 340, 20, 130, 70 );
	CaprGlyph->setMinimumSize( 0, 0 );
	CaprGlyph->setMaximumSize( 32767, 32767 );
	connect( CaprGlyph, SIGNAL(clicked(int)), SLOT(caprslot(int)) );
	CaprGlyph->setFocusPolicy( QWidget::NoFocus );
	CaprGlyph->setBackgroundMode( QWidget::PaletteBackground );
	CaprGlyph->setFontPropagation( QWidget::NoChildren );
	CaprGlyph->setPalettePropagation( QWidget::NoChildren );
	CaprGlyph->setFrameStyle( 49 );
	CaprGlyph->setTitle( transl("Capricorn glyph") );
	CaprGlyph->setAlignment( 1 );

	PostOrient = new QButtonGroup( this, "ButtonGroup_1" );
	PostOrient->setGeometry( 10, 270, 230, 90 );
	PostOrient->setMinimumSize( 0, 0 );
	PostOrient->setMaximumSize( 32767, 32767 );
	connect( PostOrient, SIGNAL(clicked(int)), SLOT(orientslot(int)) );
	PostOrient->setFocusPolicy( QWidget::NoFocus );
	PostOrient->setBackgroundMode( QWidget::PaletteBackground );
	PostOrient->setFontPropagation( QWidget::NoChildren );
	PostOrient->setPalettePropagation( QWidget::NoChildren );
	PostOrient->setFrameStyle( 49 );
	PostOrient->setTitle( transl("Postscript paper orientation") );
	PostOrient->setAlignment( 1 );

	TrueNode = new QCheckBox( this, "CheckBox_1" );
	TrueNode->setGeometry( 10, 20, 320, 20 );
	TrueNode->setMinimumSize( 0, 0 );
	TrueNode->setMaximumSize( 32767, 32767 );
	TrueNode->setFocusPolicy( QWidget::TabFocus );
	TrueNode->setBackgroundMode( QWidget::PaletteBackground );
	TrueNode->setFontPropagation( QWidget::NoChildren );
	TrueNode->setPalettePropagation( QWidget::NoChildren );
	TrueNode->setText( transl("Compute true node instead of Mean Node") );
	TrueNode->setAutoRepeat( FALSE );
	TrueNode->setAutoResize( FALSE );

	D24H = new QCheckBox( this, "CheckBox_2" );
	D24H->setGeometry( 10, 40, 320, 20 );
	D24H->setMinimumSize( 0, 0 );
	D24H->setMaximumSize( 32767, 32767 );
	D24H->setFocusPolicy( QWidget::TabFocus );
	D24H->setBackgroundMode( QWidget::PaletteBackground );
	D24H->setFontPropagation( QWidget::NoChildren );
	D24H->setPalettePropagation( QWidget::NoChildren );
	D24H->setText( transl("Display date in D/M/Y instead of M/D/Y format") );
	D24H->setAutoRepeat( FALSE );
	D24H->setAutoResize( FALSE );

	AmPm = new QCheckBox( this, "CheckBox_3" );
	AmPm->setGeometry( 10, 60, 320, 20 );
	AmPm->setMinimumSize( 0, 0 );
	AmPm->setMaximumSize( 32767, 32767 );
	AmPm->setFocusPolicy( QWidget::TabFocus );
	AmPm->setBackgroundMode( QWidget::PaletteBackground );
	AmPm->setFontPropagation( QWidget::NoChildren );
	AmPm->setPalettePropagation( QWidget::NoChildren );
	AmPm->setText( transl("Display Date in 24 hours instead of am/pm format") );
	AmPm->setAutoRepeat( FALSE );
	AmPm->setAutoResize( FALSE );

	InsigCuspAsp = new QCheckBox( this, "CheckBox_4" );
	InsigCuspAsp->setGeometry( 10, 80, 320, 20 );
	InsigCuspAsp->setMinimumSize( 0, 0 );
	InsigCuspAsp->setMaximumSize( 32767, 32767 );
	InsigCuspAsp->setFocusPolicy( QWidget::TabFocus );
	InsigCuspAsp->setBackgroundMode( QWidget::PaletteBackground );
	InsigCuspAsp->setFontPropagation( QWidget::NoChildren );
	InsigCuspAsp->setPalettePropagation( QWidget::NoChildren );
	InsigCuspAsp->setText( transl("Ignore insignificant house cusp aspect") );
	InsigCuspAsp->setAutoRepeat( FALSE );
	InsigCuspAsp->setAutoResize( FALSE );

	RightMost = new QCheckBox( this, "CheckBox_5" );
	RightMost->setGeometry( 10, 100, 320, 20 );
	RightMost->setMinimumSize( 0, 0 );
	RightMost->setMaximumSize( 32767, 32767 );
	RightMost->setFocusPolicy( QWidget::TabFocus );
	RightMost->setBackgroundMode( QWidget::PaletteBackground );
	RightMost->setFontPropagation( QWidget::NoChildren );
	RightMost->setPalettePropagation( QWidget::NoChildren );
	RightMost->setText( transl("Clip text charts at rightmost (e.g .80th) column") );
	RightMost->setAutoRepeat( FALSE );
	RightMost->setAutoResize( FALSE );

	OldStyle = new QCheckBox( this, "CheckBox_6" );
	OldStyle->setGeometry( 10, 120, 320, 20 );
	OldStyle->setMinimumSize( 0, 0 );
	OldStyle->setMaximumSize( 32767, 32767 );
	OldStyle->setFocusPolicy( QWidget::TabFocus );
	OldStyle->setBackgroundMode( QWidget::PaletteBackground );
	OldStyle->setFontPropagation( QWidget::NoChildren );
	OldStyle->setPalettePropagation( QWidget::NoChildren );
	OldStyle->setText( transl("Output chart data file in old style format") );
	OldStyle->setAutoRepeat( FALSE );
	OldStyle->setAutoResize( FALSE );

	CuspPosi = new QCheckBox( this, "CheckBox_7" );
	CuspPosi->setGeometry( 10, 140, 320, 20 );
	CuspPosi->setMinimumSize( 0, 0 );
	CuspPosi->setMaximumSize( 32767, 32767 );
	CuspPosi->setFocusPolicy( QWidget::TabFocus );
	CuspPosi->setBackgroundMode( QWidget::PaletteBackground );
	CuspPosi->setFontPropagation( QWidget::NoChildren );
	CuspPosi->setPalettePropagation( QWidget::NoChildren );
	CuspPosi->setText( transl("Cusp object are positions instead of angles") );
	CuspPosi->setAutoRepeat( FALSE );
	CuspPosi->setAutoResize( FALSE );

	SignChange = new QCheckBox( this, "CheckBox_8" );
	SignChange->setGeometry( 10, 160, 320, 20 );
	SignChange->setMinimumSize( 0, 0 );
	SignChange->setMaximumSize( 32767, 32767 );
	SignChange->setFocusPolicy( QWidget::TabFocus );
	SignChange->setBackgroundMode( QWidget::PaletteBackground );
	SignChange->setFontPropagation( QWidget::NoChildren );
	SignChange->setPalettePropagation( QWidget::NoChildren );
	SignChange->setText( transl("Restrict sign change event in search") );
	SignChange->setAutoRepeat( FALSE );
	SignChange->setAutoResize( FALSE );

	DirectChange = new QCheckBox( this, "CheckBox_9" );
	DirectChange->setGeometry( 10, 180, 320, 20 );
	DirectChange->setMinimumSize( 0, 0 );
	DirectChange->setMaximumSize( 32767, 32767 );
	DirectChange->setFocusPolicy( QWidget::TabFocus );
	DirectChange->setBackgroundMode( QWidget::PaletteBackground );
	DirectChange->setFontPropagation( QWidget::NoChildren );
	DirectChange->setPalettePropagation( QWidget::NoChildren );
	DirectChange->setText( transl("Restrict direction change in searches") );
	DirectChange->setAutoRepeat( FALSE );
	DirectChange->setAutoResize( FALSE );

	QLabel* qtarch_Label_1;
	qtarch_Label_1 = new QLabel( this, "Label_1" );
	qtarch_Label_1->setGeometry( 10, 200, 260, 20 );
	qtarch_Label_1->setMinimumSize( 0, 0 );
	qtarch_Label_1->setMaximumSize( 32767, 32767 );
	qtarch_Label_1->setFocusPolicy( QWidget::NoFocus );
	qtarch_Label_1->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_Label_1->setFontPropagation( QWidget::NoChildren );
	qtarch_Label_1->setPalettePropagation( QWidget::NoChildren );
	qtarch_Label_1->setText( transl("Number of cells in graphic asprct grid") );
	qtarch_Label_1->setAlignment( 289 );
	qtarch_Label_1->setMargin( -1 );

	NbCells = new myLineEdit( this, "LineEdit_1" );
	NbCells->setGeometry( 280, 200, 50, 20 );
	NbCells->setMinimumSize( 0, 0 );
	NbCells->setMaximumSize( 32767, 32767 );
	connect( NbCells, SIGNAL(returnPressed()), SLOT(nbcellsslot()) );
	NbCells->setFocusPolicy( QWidget::StrongFocus );
	NbCells->setBackgroundMode( QWidget::PaletteBase );
	NbCells->setFontPropagation( QWidget::NoChildren );
	NbCells->setPalettePropagation( QWidget::NoChildren );
	NbCells->setText( "" );
	NbCells->setMaxLength( 32767 );
	NbCells->setEchoMode( myLineEdit::Normal );
	NbCells->setFrame( TRUE );

	RealFonts = new QCheckBox( this, "CheckBox_11" );
	RealFonts->setGeometry( 10, 220, 320, 20 );
	RealFonts->setMinimumSize( 0, 0 );
	RealFonts->setMaximumSize( 32767, 32767 );
	RealFonts->setFocusPolicy( QWidget::TabFocus );
	RealFonts->setBackgroundMode( QWidget::PaletteBackground );
	RealFonts->setFontPropagation( QWidget::NoChildren );
	RealFonts->setPalettePropagation( QWidget::NoChildren );
	RealFonts->setText( transl("Use real system font in graphic files") );
	RealFonts->setAutoRepeat( FALSE );
	RealFonts->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_1;
	qtarch_RadioButton_1 = new QRadioButton( this, "RadioButton_1" );
	qtarch_RadioButton_1->setGeometry( 20, 290, 100, 20 );
	qtarch_RadioButton_1->setMinimumSize( 0, 0 );
	qtarch_RadioButton_1->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_1->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_1->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_1->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_1->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_1->setText( transl("Portrait") );
	qtarch_RadioButton_1->setAutoRepeat( FALSE );
	qtarch_RadioButton_1->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_2;
	qtarch_RadioButton_2 = new QRadioButton( this, "RadioButton_2" );
	qtarch_RadioButton_2->setGeometry( 20, 310, 100, 20 );
	qtarch_RadioButton_2->setMinimumSize( 0, 0 );
	qtarch_RadioButton_2->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_2->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_2->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_2->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_2->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_2->setText( transl("Landscape") );
	qtarch_RadioButton_2->setAutoRepeat( FALSE );
	qtarch_RadioButton_2->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_3;
	qtarch_RadioButton_3 = new QRadioButton( this, "RadioButton_3" );
	qtarch_RadioButton_3->setGeometry( 20, 330, 210, 20 );
	qtarch_RadioButton_3->setMinimumSize( 0, 0 );
	qtarch_RadioButton_3->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_3->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_3->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_3->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_3->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_3->setText( transl("Based on chart Dimensions") );
	qtarch_RadioButton_3->setAutoRepeat( FALSE );
	qtarch_RadioButton_3->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_4;
	qtarch_RadioButton_4 = new QRadioButton( this, "RadioButton_4" );
	qtarch_RadioButton_4->setGeometry( 350, 40, 100, 20 );
	qtarch_RadioButton_4->setMinimumSize( 0, 0 );
	qtarch_RadioButton_4->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_4->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_4->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_4->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_4->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_4->setText( transl("American") );
	qtarch_RadioButton_4->setAutoRepeat( FALSE );
	qtarch_RadioButton_4->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_5;
	qtarch_RadioButton_5 = new QRadioButton( this, "RadioButton_5" );
	qtarch_RadioButton_5->setGeometry( 350, 60, 100, 20 );
	qtarch_RadioButton_5->setMinimumSize( 0, 0 );
	qtarch_RadioButton_5->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_5->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_5->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_5->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_5->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_5->setText( transl("European") );
	qtarch_RadioButton_5->setAutoRepeat( FALSE );
	qtarch_RadioButton_5->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_6;
	qtarch_RadioButton_6 = new QRadioButton( this, "RadioButton_6" );
	qtarch_RadioButton_6->setGeometry( 350, 120, 100, 20 );
	qtarch_RadioButton_6->setMinimumSize( 0, 0 );
	qtarch_RadioButton_6->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_6->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_6->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_6->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_6->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_6->setText( transl("Herschel") );
	qtarch_RadioButton_6->setAutoRepeat( FALSE );
	qtarch_RadioButton_6->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_7;
	qtarch_RadioButton_7 = new QRadioButton( this, "RadioButton_7" );
	qtarch_RadioButton_7->setGeometry( 350, 140, 100, 20 );
	qtarch_RadioButton_7->setMinimumSize( 0, 0 );
	qtarch_RadioButton_7->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_7->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_7->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_7->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_7->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_7->setText( transl("Astronomique") );
	qtarch_RadioButton_7->setAutoRepeat( FALSE );
	qtarch_RadioButton_7->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_8;
	qtarch_RadioButton_8 = new QRadioButton( this, "RadioButton_8" );
	qtarch_RadioButton_8->setGeometry( 350, 200, 100, 20 );
	qtarch_RadioButton_8->setMinimumSize( 0, 0 );
	qtarch_RadioButton_8->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_8->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_8->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_8->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_8->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_8->setText( transl("Astronomical") );
	qtarch_RadioButton_8->setAutoRepeat( FALSE );
	qtarch_RadioButton_8->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_9;
	qtarch_RadioButton_9 = new QRadioButton( this, "RadioButton_9" );
	qtarch_RadioButton_9->setGeometry( 350, 220, 100, 20 );
	qtarch_RadioButton_9->setMinimumSize( 0, 0 );
	qtarch_RadioButton_9->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_9->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_9->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_9->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_9->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_9->setText( transl("Astrological") );
	qtarch_RadioButton_9->setAutoRepeat( FALSE );
	qtarch_RadioButton_9->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_10;
	qtarch_RadioButton_10 = new QRadioButton( this, "RadioButton_10" );
	qtarch_RadioButton_10->setGeometry( 350, 280, 100, 20 );
	qtarch_RadioButton_10->setMinimumSize( 0, 0 );
	qtarch_RadioButton_10->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_10->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_10->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_10->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_10->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_10->setText( transl("American") );
	qtarch_RadioButton_10->setAutoRepeat( FALSE );
	qtarch_RadioButton_10->setAutoResize( FALSE );

	QRadioButton* qtarch_RadioButton_11;
	qtarch_RadioButton_11 = new QRadioButton( this, "RadioButton_11" );
	qtarch_RadioButton_11->setGeometry( 350, 300, 100, 20 );
	qtarch_RadioButton_11->setMinimumSize( 0, 0 );
	qtarch_RadioButton_11->setMaximumSize( 32767, 32767 );
	qtarch_RadioButton_11->setFocusPolicy( QWidget::TabFocus );
	qtarch_RadioButton_11->setBackgroundMode( QWidget::PaletteBackground );
	qtarch_RadioButton_11->setFontPropagation( QWidget::NoChildren );
	qtarch_RadioButton_11->setPalettePropagation( QWidget::NoChildren );
	qtarch_RadioButton_11->setText( transl("European") );
	qtarch_RadioButton_11->setAutoRepeat( FALSE );
	qtarch_RadioButton_11->setAutoResize( FALSE );

	Rising = new QCheckBox( this, "CheckBox_12" );
	Rising->setGeometry( 20, 390, 100, 20 );
	Rising->setMinimumSize( 0, 0 );
	Rising->setMaximumSize( 32767, 32767 );
	Rising->setFocusPolicy( QWidget::TabFocus );
	Rising->setBackgroundMode( QWidget::PaletteBackground );
	Rising->setFontPropagation( QWidget::NoChildren );
	Rising->setPalettePropagation( QWidget::NoChildren );
	Rising->setText( transl("Rising") );
	Rising->setAutoRepeat( FALSE );
	Rising->setAutoResize( FALSE );

	Setting = new QCheckBox( this, "CheckBox_13" );
	Setting->setGeometry( 110, 390, 100, 20 );
	Setting->setMinimumSize( 0, 0 );
	Setting->setMaximumSize( 32767, 32767 );
	Setting->setFocusPolicy( QWidget::TabFocus );
	Setting->setBackgroundMode( QWidget::PaletteBackground );
	Setting->setFontPropagation( QWidget::NoChildren );
	Setting->setPalettePropagation( QWidget::NoChildren );
	Setting->setText( transl("Setting") );
	Setting->setAutoRepeat( FALSE );
	Setting->setAutoResize( FALSE );

	Zenith = new QCheckBox( this, "CheckBox_14" );
	Zenith->setGeometry( 200, 390, 140, 20 );
	Zenith->setMinimumSize( 0, 0 );
	Zenith->setMaximumSize( 32767, 32767 );
	Zenith->setFocusPolicy( QWidget::TabFocus );
	Zenith->setBackgroundMode( QWidget::PaletteBackground );
	Zenith->setFontPropagation( QWidget::NoChildren );
	Zenith->setPalettePropagation( QWidget::NoChildren );
	Zenith->setText( transl("Zenith Crossing") );
	Zenith->setAutoRepeat( FALSE );
	Zenith->setAutoResize( FALSE );

	Nadir = new QCheckBox( this, "CheckBox_16" );
	Nadir->setGeometry( 340, 390, 120, 20 );
	Nadir->setMinimumSize( 0, 0 );
	Nadir->setMaximumSize( 32767, 32767 );
	Nadir->setFocusPolicy( QWidget::TabFocus );
	Nadir->setBackgroundMode( QWidget::PaletteBackground );
	Nadir->setFontPropagation( QWidget::NoChildren );
	Nadir->setPalettePropagation( QWidget::NoChildren );
	Nadir->setText( transl("Nadir Crossing") );
	Nadir->setAutoRepeat( FALSE );
	Nadir->setAutoResize( FALSE );

	QPushButton* qtarch_PushButton_1;
	qtarch_PushButton_1 = new QPushButton( this, "PushButton_1" );
	qtarch_PushButton_1->setGeometry( 10, 430, 100, 30 );
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
	qtarch_PushButton_2->setGeometry( 370, 430, 100, 30 );
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

	ZoneLess = new QCheckBox( this, "CheckBox_17" );
	ZoneLess->setGeometry( 10, 240, 320, 20 );
	ZoneLess->setMinimumSize( 0, 0 );
	ZoneLess->setMaximumSize( 32767, 32767 );
	ZoneLess->setFocusPolicy( QWidget::TabFocus );
	ZoneLess->setBackgroundMode( QWidget::PaletteBackground );
	ZoneLess->setFontPropagation( QWidget::NoChildren );
	ZoneLess->setPalettePropagation( QWidget::NoChildren );
	ZoneLess->setText( transl("Use default zoneinfo file for zoneless charts") );
	ZoneLess->setAutoRepeat( FALSE );
	ZoneLess->setAutoResize( FALSE );

	RisSetRestr->insert( Rising );
	RisSetRestr->insert( Setting );
	RisSetRestr->insert( Zenith );
	RisSetRestr->insert( Nadir );

	LiliGlyph->insert( qtarch_RadioButton_10 );
	LiliGlyph->insert( qtarch_RadioButton_11 );

	PlutGlyph->insert( qtarch_RadioButton_8 );
	PlutGlyph->insert( qtarch_RadioButton_9 );

	UraGlyph->insert( qtarch_RadioButton_6 );
	UraGlyph->insert( qtarch_RadioButton_7 );

	CaprGlyph->insert( qtarch_RadioButton_4 );
	CaprGlyph->insert( qtarch_RadioButton_5 );

	PostOrient->insert( qtarch_RadioButton_1 );
	PostOrient->insert( qtarch_RadioButton_2 );
	PostOrient->insert( qtarch_RadioButton_3 );

	resize( 480,470 );
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );
}


ObscureSettingsData::~ObscureSettingsData()
{
}
void ObscureSettingsData::restslot(int)
{
}
void ObscureSettingsData::lilislot(int)
{
}
void ObscureSettingsData::plutslot(int)
{
}
void ObscureSettingsData::uraslot(int)
{
}
void ObscureSettingsData::caprslot(int)
{
}
void ObscureSettingsData::orientslot(int)
{
}
void ObscureSettingsData::nbcellsslot()
{
}
void ObscureSettingsData::cancelslot()
{
}
void ObscureSettingsData::okslot()
{
}
