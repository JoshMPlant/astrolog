/*****************************************************************************
 * This software is distributed under the terms of the General Public License.
 *
 * Program :         kastrolog 5.4-2
 * Authors : see the AUTHORS file
 * E-Mail  : Llyra@altavista.com
*****************************************************************************/ 

#define ASTROWINDOW

#include <kapp.h>
#include "AstroColor.h"
#include <kpixmap.h>
#include <qkeycode.h>
#include <kprocess.h>
#ifdef KDE1
#include <kimgio.h>
#include <kmsgbox.h>
#else
#include <kimageio.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kstddirs.h>
#include <kglobal.h>
#include <qasciidict.h> 
#include <qcstring.h>
#endif
#include "AstroMenu.h"

AstroApp AstroApp;
QString kdepath;
QPixmap *kdeicon;
AstroWindow *asw;
AstroColorList *Acl;
KApplication* Ka;

extern char *ChartTitle[Keyboard+1];

#ifndef KDE1
#define GLO KGlobal::dirs()
#endif

extern "C"
{
#ifndef ASTROLOG
#include "astrolog.h"
#endif
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

struct icons icons[] =
{
#ifdef KDE1
  { "fileopen.xpm",    NULL, 0 },
  { "info.xpm",        NULL, 1 },
  { "now.xpm",         NULL, 2 },
  { "fileprint.xpm",   NULL, 3 },
  { "inverse.xpm",     NULL, 4 },
  { "bwcolor.xpm",     NULL, 5 },
  { "2-3wheels.xpm",   NULL, 6 },
  { "animation.xpm",   NULL, 7 },
  { "direction.xpm",   NULL, 8 }
#else
  { "fileopen",    NULL, 0 },
  { "info",        NULL, 1 },
  { "now",         NULL, 2 },
  { "fileprint",   NULL, 3 },
  { "inverse",     NULL, 4 },
  { "bwcolor",     NULL, 5 },
  { "2-3wheels",   NULL, 6 },
  { "animation",   NULL, 7 },
  { "direction",   NULL, 8 }
#endif
};

void KBegin()
{
    if(gi.fiskde)
    {
      // Select the glyph for Lilith, Pluto, Uranus
      // Lilith
      //szObjectFont[17] = (gs.nGlyphs % 10 == 2) ? 'e' : 'i';
      szObjectFont[17] = (gs.nGlyphs % 10 == 2) ? '?' : 'a';
      // Pluto
      //szObjectFont[10] = ((gs.nGlyphs / 10) % 10 == 2) ? 'l' : 'Z';
      szObjectFont[10] = ((gs.nGlyphs / 10) % 10 == 2) ? 'H' : 'J';
    // Uranus
      //szObjectFont[8]  = ((gs.nGlyphs / 100) % 10 == 2) ? 'm' : 'X';
      szObjectFont[8]  = ((gs.nGlyphs / 100) % 10 == 2) ? 'F' : 'A';
      Acl = new AstroColorList;
      CHECK_PTR(Acl);
      asw = new AstroWindow;
      CHECK_PTR(asw);
      asw->init();
    }
}

  void KInteract()
  {
    if(gi.fiskde)
      qApp->exec();
  }

  void KEnd()
  {
    Terminate(0);
  }

  void SetMenuName(int menu, char *name)
  {
    int i;
    switch(menu)
    {
       case  1 ... 12 : i = 0; break;
       case 13 ... 24 : i = 
#ifdef KDE1
			  SHIFT
#else
			  Qt::SHIFT
#endif
			  ; break;
       case 25 ... 36 : i = 
#ifdef KDE1
			  CTRL
#else
			  Qt::CTRL
#endif
			  ; break;
       case 37 ... 48 : i = 
#ifdef KDE1
			  ALT
#else
			  Qt::ALT
#endif
			  ; break;
       default        : PrintError(transl("Illegal macro number.")); exit(1);
    }
    asw->am->changeItem(name, i + 
#ifdef KDE1
			Key_F1
#else
			Qt::Key_F1
#endif
			- ((menu /12) * 12) + menu - 1);
}

  char *getlocale(const char *c)
  {
#ifdef KDE1
    return (char *)i18n(c);
#else
    static QAsciiDict<QCString> Dcs;
    QCString *qcs = Dcs[c];
    if(!qcs)
    {
      qcs = new QCString(i18n(c).latin1());
      Dcs.insert(c, qcs);
      }
      return qcs->data();
#endif
  }

  // Data tables transferred from data.c
  char *szSignName[cSign+9], *szSignAbbrev[cSign+1], *szSignEnglish[cSign+1], 
  *szHouseTradition[cSign+1], *szObjName[objMax], *szSystem[cSystem],
  *szAspectName[cAspect+1], *szAspectAbbrev[cAspect+1],
  *szAspectGlyph[cAspect+1], *szAspectConfig[cAspConfig+1], *szElem[4],
  *szMode[3], *szMonth[cSign+1], *szDay[cWeek], *szDir[4], *szSuffix[cSign+1],
  *szCnstlMeaning[cCnstl+1];

  char *notime, *compochart, *shouse, *sidereal, *tropical, *heliocentric;
  char *geocentric, *julianday, *dshouse, *fireearth, *airwater, *carfixmut;
  char *yangyin, *angsuccad, *learnshare;

  static void signew(int i)
  {
    pid_t pid;
    AstroApp.nbchilds++;
    AstroApp.thecounter++;
    if(pid = fork())
      AstroApp.add(pid);
    else
      AstroApp.run(AstroApp.nbchilds, AstroApp.thecounter);
  }

  static void sigquit(int i)
  {
    AstroApp.killall();
  }
};

// This function fills an array of char pointers with the null terminated 
// list of pointers

static void fill(char *t[], char *c, ...)
{
  char **ptr = &c;
  while(*ptr)
      *t++ = *ptr++;
}

#ifdef ARABIC
// Fills the ai array with internationaized datas

static void fillai(char *c, ...)
{
  char **ptr = &c;
  AI* a = ai;
  while(*ptr)
  {
    a->name = *ptr++;
    a++;
  }
}
#endif

// Initialization of all the data tables

void initvals()
{
  notime = transl("No time or space.");
  compochart = transl("Composite chart.");
  shouse = transl("%s houses.");
  sidereal = transl("Sidereal");
  tropical = transl("Tropical");
  heliocentric = transl("Heliocentric");
  geocentric = transl("Geocentric");
  julianday =  transl("Sidereal time: %s");
  dshouse = transl("%2d%s house: ");
  fireearth = transl("Fire: %d, Earth: %d,");
  airwater = transl("Air : %d, Water: %d");
  carfixmut = transl("Car: %d, Fix: %d, Mut: %d");
  yangyin = transl( "Yang: %d, Yin: %d");
  angsuccad = transl("Ang: %d, Suc: %d, Cad: %d");
  learnshare = transl("Learn: %d, Share: %d");
  fill(szSignName, "",
  transl("Aries"), transl("Taurus"), transl("Gemini"), transl("Cancer"), transl("Leo"),
  transl("Virgo"), transl("Libra"), transl("Scorpio"), transl("Sagittarius"), transl("Capricorn"),
   transl("Aquarius"), transl("Pisces"), NULL);

  fill(szSignAbbrev, "", transl("Ar"),  transl("Ta"), transl("Ge"),  transl("Cn"),  transl("Le"),
  transl("Vi"), transl("Li"),  transl("Sc"),  transl("Sg"),  transl("Cp"),  transl("Aq"),  transl("Pi"),
  NULL);

  fill(szSignEnglish, "", transl("Ram"), transl("Bull"), transl("Twins"), transl("Crab"),
  transl("Lion"), transl("Virgin"), transl("Scales"), transl("Scorpio"), transl("Archer"),
  transl("Sea Goat"), transl("Water Bearer"), transl("Fishes"), NULL);

  fill(szHouseTradition, "", transl("Personality"), transl("Money"),
  transl("Communication"), transl("Home"), transl("Children"), transl("Servants"),
  transl("Marriage"), transl("Death"), transl("long Journeys Over Water"), transl("Career"),
  transl("Friends"), transl("Troubles"), NULL);
  fill(szObjName, transl("Earth"), transl("Sun"), transl("Moon"), transl("Mercury"), transl("Venus"), transl("Mars"),       /* Planets   */
  transl("Jupiter"), transl("Saturn"), transl("Uranus"), transl("Neptune"), transl("Pluto"),
  transl("Chiron"), transl("Ceres"), transl("Pallas"), transl("Juno"), transl("Vesta"),             /* Asteroids */
  transl("Node"), transl("MeanApogee"), transl("Apogee"), transl("Vertex"), transl("EastPoint"),      /* Others    */
  transl("Ascendant"), transl("2nd Cusp"), transl("3rd Cusp"), transl("I.Coeli"),             /* Cusps     */
  transl("5th Cusp"), transl("6th Cusp"), transl("Descendant"), transl("8th Cusp"),
  transl("9th Cusp"), transl("M.Coeli"), transl("11th Cusp"), transl("12th Cusp"),
  transl("Fortune"),
  transl("PolarAsc"),transl("ARMC"),transl("Coascendant"),
  transl("Pholus"), transl("Chariklo"), transl("Hylonome"),                      /* Centaurs and SDO's  */
  transl("Nessus"), transl("Asbolus"),
  transl("TL66"),transl("GQ21"),transl("DE9"),transl("TD10"),transl("Elatus"),transl("Thereus"),transl("BU48"),transl("Typhon"),
  transl("Ixion"), transl("Varuna"),   /* Transneptunians*/
  transl("QB1"), transl("Eris"),transl("Logos"),transl("GV9"),transl("JS94"),transl("JR1"),transl("TB94"),transl("DA2"),transl("TP66"),transl("CR29"),transl("JQ1"),transl("VK8"),
  transl("SZ4"),transl("TO66"),transl("Chaos"),transl("Deucalion"),transl("TR66"),transl("SM55"),transl("VS2"),transl("HJ151"),transl("SM165"),transl("Orcus"),transl("Sedna"),transl("Rhadamanthus"),
  transl("Huya"),transl("UR163"),
   /*other*/
  transl("Flora"),transl("Psyche"),transl("Fortuna"),transl("Ariadne"),transl("Europa"),transl("Pandora"),transl("Eurydike"),transl("RG33"),transl("Aurora"),transl("Antigone"),transl("Elektra"),transl("Hilda"),transl("Philosophia"),transl("Thule"),transl("Cupido"),transl("Hybris"),transl("Hidalgo"),transl("Lilith"),transl("Aphrodite"),transl("Damocles"),transl("Prometheus"),
  transl("Kama"),transl("Partizanske"),
  /* Muses */
  transl("Melpomene"),transl("Kalliope"),transl("Thalia"),transl("Euterpe"),transl("Urania"),transl("Polyhymnia"),transl("Erato"),transl("Terpsichore"),transl("Klio"),
  /* Apollos */
  transl("Icarus"),transl("Geographos"),transl("Toro"),transl("Apollo"),transl("Antinous"),transl("Daedalus"),transl("Cerberus"),transl("Sisyphus"),transl("Midas"),
  transl("Bacchus"),transl("Adonis"),transl("Tantalus"),transl("Phaeton"),transl("Orpheus"),transl("Pan"),transl("Heracles"),
  transl("Eros"),transl("Amor"),transl("Quetzalcoatl"),transl("Tezcatlipoca"),transl("Krok"),transl("DonQuixote"),transl("Dionysus"), /*Amors*/
  transl("Aten"), transl("Ra-Shalom"),transl("Hathor"),transl("Khufu"),transl("Amun"),transl("Cruithne"),transl("Sekhmet"), /*Atens */
/*  transl("WhiteMoon"),transl("Isis(Sevin)"), */
  transl("Cupido"), transl("Hades"), transl("Zeus"), transl("Kronos"),                      /* Uranians  */
  transl("Apollon"), transl("Admetos"), transl("Vulkanus"), transl("Poseidon"), transl("Isis"),
  /*  Stars  */
  transl("Achernar      "), transl("Polaris       "), transl("Zeta Retic.   "), transl("Alcyone       "),
  transl("Aldebaran     "), transl("Capella       "), transl("Rigel         "), transl("Bellatrix     "),
  transl("Elnath        "), transl("Alnilam       "), transl("Betelgeuse    "), transl("Menkalinan    "),
  transl("Mirzam        "), transl("Canopus       "), transl("Alhena        "), transl("Sirius        "),
  transl("Adhara        "), transl("Wezen         "), transl("Castor        "), transl("Procyon       "),
  transl("Pollux        "), transl("Suhail        "), transl("Avior         "), transl("Miaplacidus   "),
  transl("Alphard       "), transl("Regulus       "), transl("Dubhe         "), transl("Acrux         "),
  transl("Gacrux        "), transl("Mimosa        "), transl("Alioth        "), transl("Spica         "),
  transl("Alkaid        "), transl("Agena         "), transl("Arcturus      "), transl("Toliman       "),
  transl("Antares       "), transl("Shaula        "), transl("Sargas        "), transl("Kaus Austr.   "),
  transl("Vega          "), transl("Altair        "), transl("Peacock       "), transl("Deneb         "),
  transl("Alnair        "), transl("Fomalhaut     "), transl("Andromeda     "), transl("Alpheratz     "),
  transl("Algenib       "), transl("Schedar       "), transl("Mirach        "), transl("Alrischa      "),
  transl("Almac         "), transl("Algol         "), transl("Mintaka       "), transl("Wasat         "),
  transl("Acubens       "), transl("Merak         "), transl("Vindemiatrix  "), transl("Mizar         "),
  transl("Kochab        "), transl("Zuben Elgen.  "), transl("Zuben Escha.  "), transl("Alphecca      "),
  transl("Unuk Alhai    "), transl("Ras Alhague   "), transl("Albireo       "), transl("Alderamin     "),
  transl("Nashira       "), transl("Skat          "), transl("Scheat        "), transl("Markab        "), 
  transl("Apex          "), transl("SGal.Center   "), transl("SGal.N.Pole   "),
  transl("Gal.Center  "),transl("Gal.Equ.Node  "),transl("Gal.Zero  "),transl("Gal.N.Pole  "),NULL);

  fill(szSystem, transl("Placidus"), transl("Koch"), transl("Equal"), transl("Campanus"),
  transl("Meridian"), transl("Regiomontanus"), transl("Porphyry"), transl("Morinus"),
  transl("Topocentric"), transl("Alcabitius"), transl("Equal (MC)"), transl("Neo-Porphyry"),
  transl("Whole"), transl("Vedic"), transl("Null"), NULL);

  fill(szAspectName, "", transl("Conjunct"), transl("Opposite"), transl("Square"),
  transl("Trine"), transl("Sextile"), transl("Inconjunct"), transl("Semisextile"),
  transl("Semisquare"), transl("Sesquiquadrate"), transl("Quintile"), transl("Biquintile"),
  transl("Semiquintile"), transl("Septile"), transl("Novile"), transl("Binovile"),
  transl("Biseptile"), transl("Triseptile"), transl("Quatronovile"), NULL); 

  fill(szAspectAbbrev, "", transl("Con"), transl("Opp"), transl("Squ"), transl("Tri"),
  transl("Sex"), transl("Inc"), transl("SSx"), transl("SSq"), transl("Ses"), transl("Qui"), transl("BQn"),
  transl("SQn"), transl("Sep"), transl("Nov"), transl("BNv"), transl("BSp"), transl("TSp"), transl("QNv"),
  NULL);

  fill(szAspectGlyph, "", transl("Circle with extending line"), 
  transl("Two circles joined by line"), transl("Quadrilateral"), transl("Triangle"),
  transl("Six pointed asterisk"), transl("'K' rotated right"), transl("'K' rotated left"),
  transl("Acute angle"), transl("Square with extending lines"), transl("Letter 'Q'"),
  transl("'+' over '-'"), transl("'-' over '+'"), transl("Number '7'"), transl("Number '9'"),
  transl("'9' under Roman 'II'"), transl("'7' under Roman 'II'"),
  transl("'7' under Roman 'III'"), transl("'9' under Roman 'IV'"), NULL);

  fill(szAspectConfig, "", transl("Stellium"), transl("Grand Trine"), transl("T-Square"),
  transl("Yod"), transl("Grand Cross"), transl("Cradle"), NULL);

  fill(szElem, transl("Fire"), transl("Earth"), transl("Air"), transl("Water"), NULL);

  fill(szMode, transl("Cardinal"), transl("Fixed"), transl("Mutuable"), NULL);

  fill(szMonth, "", transl("January"), transl("February"), transl("March"), transl("April"),
  transl("May"), transl("June"), transl("July"), transl("August"), transl("September"),
  transl("October"), transl("November"), transl("December"), NULL);

  fill(szDay, transl("Sunday"), transl("Monday"), transl("Tuesday"), transl("Wednesday"),
  transl("Thursday"), transl("Friday"), transl("Saturday"), NULL);

  fill(szDir, transl("North"), transl("East"), transl("South"), transl("West"), NULL);

  fill(szSuffix, "", transl("st"), transl("nd"), transl("rd"), transl("th"), transl("th"),
  transl("th"), transl("th"), transl("th"), transl("th"), transl("th"), transl("th"), transl("th"), NULL);

  fill(szCnstlMeaning, "",
  transl("Chained Maiden"), transl("Air Pump"), transl("Bird of Paradise"), transl("Water Bearer"),
  transl("Eagle"), transl("Altar"), transl("Ram"), transl("Charioteer"),
  transl("Herdsman"), transl("Chisel"), transl("Giraffe"), transl("Crab"),
  transl("Hunting Dogs"), transl("Great Dog"), transl("Little Dog"), transl("Sea Goat"),
  transl("Keel"), transl("Queen"), transl("Centaur"), transl("King"),
  transl("Sea Monster"), transl("Chameleon"), transl("Drawing Compass"), transl("Dove"),
  transl(" Berenice's Hair"), transl("Southern Crown"), transl("Northern Crown"), transl("Crow"),
  transl("Cup"), transl("Southern Cross"), transl("Swan"), transl("Dolphin"),
  transl("Dolphinfish"), transl("Dragon"), transl("Little Horse"), transl("River"),
  transl("Furnace"), transl("Twins"), transl("Crane"), transl("Strongman"),
  transl("Clock"), transl("Sea Serpent"), transl("Water Snake"), transl("Indian"),
  transl("Lizard"), transl("Lion"), transl("Little Lion"), transl("Hare"),
  transl("Scales"), transl("Wolf"), transl("Lynx"), transl("Lyre"),
  transl("Table Mountain"), transl("Microscope"), transl("Unicorn"), transl("Fly"),
  transl("Level"), transl("Octant"), transl("Serpent Bearer"), transl("Hunter"),
  transl("Peacock"), transl("Winged Horse"), transl("Hero"), transl("Phoenix"),
  transl("Painter"), transl("Fishes"), transl("Southern Fish"), transl("Stern"),
  transl("Compass"), transl("Net"), transl("Arrow"), transl("Archer"),
  transl("Scorpion"), transl("Sculptor"), transl("Shield"), transl(" Head/Tail of the Snake"),
  transl("Sextant"), transl("Bull"), transl("Telescope"), transl("Triangle"),
  transl("Southern Triangle"), transl("Toucan"), transl("Great Bear"), transl("Little Bear"),
  transl("Sail"), transl("Virgin"), transl("Flying Fish"), transl("Fox"), NULL);
#ifdef ARABIC
  fillai(
  transl("Fortune"),
  transl("Spirit"),
  transl("Victory"),
  transl("Valor & Bravery"),
  transl("Mind & Administrators"),
  transl("Property & Goods"),
  transl("Siblings"),
  transl("Death of Siblings"),
  transl("Death of Parents"),
  transl("Grandparents"),
  transl("Real Estate"),
  transl("Children & Life"),
  transl("Expected Birth"),
  transl("Disease & Defects (1)"),
  transl("Disease & Defects (2)"),
  transl("Captivity"),
  transl("Servants"),
  transl("Partners"),
  transl("Death"),
  transl("Sickness & Murder"),
  transl("Danger, Violence & Debt"),
  transl("Journeys"),
  transl("Travel by Water"),
  transl("Faith, Trust & Belief"),
  transl("Deep Reflection"),
  transl("Understanding & Wisdom"),
  transl("Fame & Recognition"),
  transl("Rulers & Disassociation"),
  transl("Father, Fate & Karma"), /* Combust */
  transl("Sudden Advancement"),
  transl("Celebrity of Rank"),
  transl("Surgery & Accident"),
  transl("Merchants & Their Work"),
  transl("Merchandise (Exchange)"), /* Moon */
  transl("Mother"),
  transl("Glolry & Constancy"),
  transl("Honorable Acquaintances"),
  transl("Success"),
  transl("Worldliness"),
  transl("Acquaintances"),
  transl("Violence"),
  transl("Liberty of Person"),
  transl("Praise & Acceptance"),
  transl("Enmity"),
  transl("Bad Luck"),
  transl("Debilitated Bodies"),
  transl("Boldness & Violence"),
  transl("Trickery & Deceit"),
  transl("Necessities"),
  transl("Realization of Needs"),
  transl("Retribution"),
  transl("Children (Male)"),
  transl("Children (Female)"),
  transl("Play & Variety"), /* Change */
  transl("Stability"),
  transl("Speculation"),
  transl("Art"),
  transl("Sexual Attraction"),
  transl("Sex Drive & Stimulation"),
  transl("Passion"),
  transl("Emotion & Affection"),
  transl("Most Perilous Year"),
  transl("Peril"),
  transl("Occultism"),
  transl("Commerce"),
  transl("Marriage Contracts"),
  transl("Travel by Land"),
  transl("Travel by Air"),
  transl("Destiny"),
  transl("Vocation & Status"),
  transl("Honor, Nobility (Day)"),
  transl("Honor, Nobility (Night)"),
  transl("Organization"),
  transl("Divorce"),
  transl("Ostracism & loss"),
  transl("Friends"),
  transl("Tragedy & Brethren"),
  transl("Race (Consciousness)"),
  transl("Bondage & Slavery"),
  transl("Imprisonment & Sorrow"),
  transl("Perversion"),
  transl("Self-Undoing"),
  transl("Treachery & Entrapment"),
  transl("Bereavement"),
  transl("Suicide (Yang)"),
  transl("Suicide (Yin)"),
  transl("Depression"),
  transl("Assassination (Yang)"),
  transl("Assassination (Yin)"),
  transl("Cancer (Disease)"),
  transl("Catastrophe"),
  transl("Foolhardiness"),
  transl("Release & Luck"),
  transl("Benevolence & Assurance"),
  transl("Hope & Sensitivity"),
  transl("Aptness & Aloofness"),
  transl("Charm & Personality"),
  transl("Faith & Good Manners"),
  transl("Temperament"),
  transl("Security & Treasure"),
  transl("Originality"),
  transl("Eccentricity, Astrology"),
  transl("Divination"),
  transl("Intrusion"),
  transl("Negotiation"),
  transl("Discord & Controversy"),
  transl("Coincidence"),
  transl("Unpreparedness"),
  transl("Popularity"),
  transl("Misunderstanding"),
  transl("Sentiment & Marriage"),
  transl("loneliness"),
  transl("Success in Investment"),
  transl("Frugality & Labor"),
  transl("Wastefulness"),
  transl("Vanity"),
  transl("Corruptness"),
  transl("Initiative"),
  transl("Memory"),
  transl("Love, Beauty & Peace"),
  transl("Disinterest & Boredom"),
  transl("Accomplishment"),
  transl("Influence"),
  transl("Increase & Impression"),
  transl("Caution"),
  transl("Timidity"),
  transl("Entertainment"),
  transl("Bequest"),
  transl("Genius"),
  transl("Revelation"),
  transl("Delusion"),
  transl("Misinterpretation"),
  transl("Intellectuality"),
  transl("Earth"),
  transl("Water"),
  transl("Air & Wind"),
  transl("Fire"),
  transl("Clouds"),
  transl("Rains"),
  transl("Cold"),
  transl("Wheat"),
  transl("Barley & Meats"),
  transl("Rice & Millet"),
  transl("Corn"),
  transl("Lentils, Iron, Pungents"),
  transl("Beans & Onions"),
  transl("Chick Peas"),
  transl("Sesame & Grapes"),
  transl("Sugar & Legumes"),
  transl("Honey"),
  transl("Oils"),
  transl("Nuts & Flax"),
  transl("Olives"),
  transl("Apricots & Peaches"),
  transl("Melons"),
  transl("Salt"),
  transl("Sweets"),
  transl("Astrigents & Purgatives"),
  transl("Silk & Cotton"),
  transl("Purgatives (Bitter)"),
  transl("Purgatives (Acid)"),
  transl("Secrets"),
  transl("Information True/False"),
  transl("Injury to Business"),
  transl("Freedmen & Servants"),
  transl("Employers"),
  transl("Marriage"),
  transl("Time for Action/Success"),
  transl("Time Occupied in Action"),
  transl("Dismissal & Resignation"),
  transl("Life/Death of Absentee"),
  transl("Lost Animal (Light)"),
  transl("Lost Animal (Dark)"),
  transl("Lawsuit"),
  transl("Decapitation"),
  transl("Torture"),
  transl("Lost Objects"), NULL);
#endif

  fill(ChartTitle, transl("Standard List"), transl("House Wheel"), 
       transl("Aspect Midpoint Grid"), transl("Aspect List"), transl("Midpoint List"),
       transl("Local Horizon"), transl("Solar System Orbit"), transl("Gauquelin Sectors"), 
       transl("Calendar"), transl("Influence"), transl("Astro-Graph"), transl("Ephemeris"), 
       transl("Arabic Parts"), transl("Rising and Setting"), transl("Transits..."), 
       transl("Progressions..."), transl("No RelationShip/Comparison Chart"),
       transl("Synastry Chart"),transl("Composite Chart"),
       transl("Time/Space Midpoint Chart"), transl("Date Difference Chart"),
       transl("Biorhythm Chart"), transl("Transit and Natal"),
       transl("Progressed and Natal"), transl("Show World Map"), transl("Show Globe"),
       transl("Show Polar Globe"),  transl("List Signs"),
       transl("List Objects"), transl("List Aspects"), transl("List Constellations"),
       transl("List Planet Info"), transl("List General Meanings"),
       transl("List Switches"), transl("List Obscure Switches"),
       transl("List KeyStrokes"), NULL);
  icons[0].tip = transl("Open a file");
  icons[1].tip = transl("Set Chart Info...");
  icons[2].tip = transl("Now");
  icons[3].tip = transl("Print...");
  icons[4].tip = transl("Reverse Background");
  icons[5].tip = transl("Monochrome");
  icons[6].tip = transl("Single or dual display");
  icons[7].tip = transl("Animation");
  icons[8].tip = transl("Reverse Direction");
} 

AstroWindow::AstroWindow()
{
  QString s, s1, ksticon = 
#ifdef KDE1
"kastrolog.xpm";
#else
"kastrolog";
#endif
#ifdef KDE1
  if(AstroApp.nbchilds > 1)
    s.sprintf("%s %s (%d)",  szAppNameCore, szVersionCore, AstroApp.thecounter);
  else
    s.sprintf("%s %s", szAppNameCore, szVersionCore);
  setCaption(s.data());
#endif
  Ksb = statusBar();
  // For KDE 2, instead of Ktb = toolBar();
  Ktb = new KToolBar(this, QMainWindow::Top, FALSE, 0, FALSE, FALSE);
  CHECK_PTR(Ktb);
#ifdef KDE1
  Ksb->setBorderWidth(2);
#endif
  Kil = new KIconLoader;
  CHECK_PTR(Kil);
#ifdef KDE1
  s1 = Ka->kde_icondir() + "/" + ksticon;
#endif
  am = new AstroMenu(this);
  CHECK_PTR(am);
  setMenu(am);
  central = new AstroWidget(this, NULL);
  CHECK_PTR(central);
  setView(central, FALSE);
#ifdef KDE1
  resize(gs.xWin+20, gs.yWin+45+Ksb->height()+Ktb->height());
  kdeicon = new QPixmap(s1.data());
#else
  resize(gs.xWin+20, gs.yWin+65+Ksb->height()+Ktb->height());
  kdeicon = new QPixmap(Kil->loadIcon(ksticon, KIcon::Desktop));
#endif
  CHECK_PTR(kdeicon);
}

void AstroApp::pidList::add(pid_t pid)
{
  pid_t* ptr = new pid_t;
  *ptr = pid;
  append(ptr);
}

void AstroApp::pidList::killall()
{
  pid_t* ptr;
  for(ptr = first(); ptr; ptr = next())
    kill(*ptr, SIGTERM);
}
 
AstroApp::AstroApp()
{
   nbchilds = thecounter = 1;
   mainpid = getpid();   
   signal(SIGCONT, signew);
   signal(SIGQUIT, sigquit);
}

void AstroApp::bye(bool b)
{
#ifdef KDE1
  if(KMsgBox::yesNo(NULL, transl("Quitting"), b ? transl("Do you really want to quit ?") : transl("Do you really want to close this window ?"), KMsgBox::QUESTION) == 1)
#else
  QString text =  b ? transl("Do you really want to quit ?") : transl("Do you really want to close this window ?"), caption = transl("Quitting");
  if(KMessageBox::questionYesNo(NULL, text, caption) == KMessageBox::Yes)
#endif
  {
    if(b)
	kill(mainpid, SIGQUIT);
    else
      exit(0);
  }
}

void AstroApp::init(int argc, char** argv)
{
  int status;
  pid_t pid;
  theargc = argc;
  theargv = argv;
  //#ifndef DEBUG
  if((pid = fork()))
  {
    add(pid);
    while(1)
    {
      wait(&status);
      nbchilds--;
      if(!nbchilds)
	exit(0);
    }
  }
  else
    //#else
    //#warning Monotask !
    //#endif
    run(1, 1);
}

static void CheckAstroDat()
{
  KProcess kp;
  int i;
  QString s = GLO->findResourceDir("data", "kastrolog/kastrolog.pfb") + "kastrolog/";
  setenv("ASTR", s.latin1(), 1);
  kdepath = s;
  s = GLO->localkdedir() + "share/apps/kastrolog";
  if(!GLO->exists(s))
  {
    kp << "mkdir" << "-p" << s;
    kp.start(KProcess::Block);
  }
  setenv("ASTROLOG", s.latin1(), 1);
  if(!GLO->exists(s + "/astrolog.dat"))
  {
    kp.clearArguments();
    kp << "cp" << kdepath + "/astrolog.dat" << s;
    kp.start(KProcess::Block);
  }
}

int AstroApp::run(int child, int counter)
{
#ifndef KDE1
  char *FakeArguments[] = {"kastrolog"};
  KCmdLineArgs::init(1, FakeArguments, "kastrolog", "Kastrolog, an astrology software", "5.4-2b");
  KApplication astroapp(theargc, theargv);
  KImageIO::registerFormats();
#else
  KApplication astroapp(theargc, theargv);
#endif
  CheckAstroDat();
  nbchilds = child;
  thecounter = counter;
  initvals();
  Ka = &astroapp;
  tzfile = 0;
  kmain(theargc, theargv);
  return 0;
}

void AstroApp::checkdst()
{
  if(us.fDst4Anim)
  {
    if(tzfile  != ciCore.tzfile)
    {
      tzfile = ciCore.tzfile;
      tz.settz(ciCore.tzfile, TRUE);
    }
    if(tzfile)
    {
      tz.settime(ciCore.tim, ciCore.day, ciCore.mon, ciCore.yea);
      ciCore.dst = tz.isdst() > 0 ? 1.0 : 0.0;
      if(gs.nAnim)
	ciMain.dst = ciCore.dst;
    }
  } 
}

void AstroApp::getTZ(real *r)
{
  if(us.fDst4Anim)
    *r = -tz.setztime();
}

int main(int argc, char **argv)
{   
  int i;
#ifdef KDE1
  kimgioRegister();
  s = kde_datadir();
#endif
  for(i = 0; i < 48; i++)
    szMacro[i] = szMacroName[i] = NULL;  
  AstroApp.init(argc, argv);
}










