// ============================================
// Transmission (capstan) research paper
// ============================================

// Import Template
#import "@local/academic-tools:0.1.20": *

// Import Packages
#import "@preview/unify:0.7.1": num,qty,numrange,qtyrange

// Set parameters for the document
#set text(lang: "nl")
#set math.equation(numbering: "(1)")


// ============================================
// Front Page en general definitions
// ============================================

#show: IEEE-academic-journal.with(
  // Cover page parameters
  title: [Power transmissie robot actuatoren analyse],
  authors: "Perijn Huijser",

  degree: [Bachelor of Electrical engineering],
  degree-goal: [
        Een onderzoek benodigd voor het realiseren van een accurate robot actuator
      ],

  department: [Bachelors student],
  university: [NHL Stenden, Hogeschool],
  supervisor: [R. Moedt\ C. Mari Spies],

  date: datetime.today(),
  location: [Leeuwarden, Nederland],
  
  use-front-cover: true,
  logo: [#image("../Assets/Logo/NHL_logo.jpg", width: 140pt)],
  
  // Front matter
  extend-abstract: false,
  abstract: [Dit onderzoek is uitgevoerd in het kader van het Embedded Systems Project 2025–2026 aan NHL Stenden Hogeschool te Leeuwarden. Het project heeft als doel een functioneel embedded apparaat te ontwikkelen dat ingezet kan worden tijdens toekomstige open dagen. In deze context richt het onderzoek zich op het ontwerpen en realiseren van een robothond als demonstratieplatform voor embedded technologieën.

  Om tot een technisch onderbouwd en betrouwbaar ontwerp te komen, is uitgebreide literatuurstudie noodzakelijk. Deze studie stelt het projectteam in staat om weloverwogen ontwerpkeuzes te maken op het gebied van mechanica, aandrijving, sensortechniek, elektrotechniek en systeemintegratie. Het onderzoek vormt daarmee de basis voor het ontwerptraject en draagt bij aan de realisatie van een robuust, functioneel en didactisch inzetbaar demonstratiemodel.],
  
  footer-config: (
    short-title: [Transmissie analyse],
    version: "1.0",
    confidentiality: [Internal Use Only],
    author: [],
    affiliation: [NHL Stenden],
    font-size: 8.5pt,
  ),
  
  // Bibliography
  references: [#bibliography("/Documenten/Referenties/Transmissie.bib", style: "ieee")],
  reference-style: "ieee",
)


// ============================================
// Document contents
// ============================================

= Introductie

Het ontwerpen van een functionele robothond vereist zorgvuldige en onderbouwde ontwerpkeuzes, in het bijzonder met betrekking tot het mechanische transmissiesysteem. Dit document richt zich op de transmissie-architectuur, een kerncomponent in het actuatorsysteem, die bepalend is voor de volgende prestatiecriteria:

+ *Minimale speling (backlash)* --- ter voorkoming van accumulerende positiefouten en ter waarborging van nauwkeurige en reproduceerbare bewegingen.
+ *Hoge energie-overdrachtsefficiëntie* --- met het oog op het maximaliseren van de operationele batterijduur en het reduceren van thermische verliezen.
+ *Hoog mechanisch koppel* --- noodzakelijk om robuuste interacties met de omgeving mogelijk te maken, waaronder het dragen van belasting en het overwinnen van obstakels.
+ *Optimalisatie van motorsnelheid* --- om een gunstige balans te realiseren tussen motorbesturing, dynamische respons en controllerbandbreedte.

Het afwegen van de samenhang van deze ontwerpeisen is essentieel voor het realiseren van een actuatorsysteem dat zowel nauwkeurig als betrouwbaar functioneert.

== Mechanische backlash

Binnen robotische systemen vormt het minimaliseren van backlash een fundamentele ontwerpprioriteit. Backlash wordt gedefinieerd als de ongewenste mechanische speling tussen samenwerkende tandwielen of transmissie-elementen en beïnvloedt zowel de positionele nauwkeurigheid als de dynamische prestaties van het systeem. De belangrijkste negatieve effecten zijn:

- *Accumulatie van positionele fouten*: Speling kan zich over opeenvolgende bewegingen ophopen, waardoor onzekerheid in de werkelijke pootpositie ontstaat en de stabiliteit van de beweging wordt aangetast.
- *Versnelde slijtage*: Herhaalde wisselingen in belasting en draairichting leiden tot verhoogde materiaalinteractie in de tandwielovergangen, wat de structurele levensduur van de transmissie vermindert.

Gezien deze nadelige gevolgen dient backlashreductie beschouwd te worden als een belangrijke eis om te minimaliseren

= Huidige transmissie-opties

Verschillende transmissie-architecturen zijn theoretisch in staat om te voldoen aan de
vooropgestelde prestatie-eisen inzake koppeloverdracht, positioneernauwkeurigheid,
compactheid en energetische efficiëntie. Elke architectuur introduceert echter
specifieke mechanische beperkingen en ontwerpafwegingen. Aangezien de transmissie
een bepalende invloed heeft op de dynamische prestaties, regelbaarheid en
structurele integratie van de robothond, is een systematische vergelijking van de
beschouwde concepten noodzakelijk.

+ *Harmonische aandrijvingen*:
  Worden gekenmerkt door een zeer lage hoekspeling (≈ #zi.arcminute(2))
  @noauthor_comparison_2021, wat resulteert in een hoge positioneernauwkeurigheid
  en gunstige eigenschappen voor precisietoepassingen. De elastische vervorming van
  de flexspline impliceert echter een beperkte torsiestijfheid onder hoge belasting.
  Daarnaast zijn deze aandrijvingen relatief kostbaar en beperkt in het maximaal
  continu overdraagbare koppel.

+ *Planetaire gearbox*:
  Vertonen een hoge mechanische efficiëntie (≈ #numrange("75", "99")\%)
  @august_dynamics_1984 @bertoldi_efficiency_2021 en bieden een gunstige
  vermogensdichtheid bij compacte inbouwafmetingen. De typische hoekspeling
  bedraagt $approx$ #zi.arcsecond(20), afhankelijk van toleranties en kwaliteitsklasse.
  De modulaire opbouw laat bovendien flexibele configuraties van
  reductieverhouding en trapopbouw toe.

+ *Cycloïdale reductoren*:
  Maken hoge reductieverhoudingen per trap mogelijk en combineren dit met een extreem lage hoekspeling ($approx$ #zi.arcsecond(0)). Door de meervoudige contactpunten tussen schijf en penconstructie wordt een hoge schokbestendigheid en lastverdeling
  gerealiseerd. Het niet-terugdraaibare karakter kan voordelig zijn voor
  statische positionering, maar beperkt de intrinsieke backdrivability in
  dynamische toepassingen.

+ *Capstan-aandrijvingen*:
  Worden gekenmerkt door een verwaarloosbare hoekspeling
  ($approx$ #zi.arcminute(0)) @mazumdar_synthetic_2017 en een hoge transmissie
  efficiëntie ($approx$ #numrange("95", "96")%) @mazumdar_synthetic_2017.
  Door het gebruik van kabel- of riemoverbrenging wordt een lage massa en
  minimale wrijvingsverliezen gerealiseerd. De koppelcapaciteit is echter
  sterk afhankelijk van de wrijvingscoëfficiënt, omwikkelhoek en
  voorspanning, wat nauwkeurige dimensionering vereist.

= Keuze transmissie systeem
Op basis van de analyse van de verschillende transmissie-architecturen is gekozen voor een capstan-aandrijving. Deze keuze is primair ingegeven door de specifieke systeemeisen van de robothond, waarbij lage massa, hoge positioneernauwkeurigheid en goede dynamische prestaties centraal staan.

De capstan-aandrijving onderscheidt zich door een vrijwel afwezige mechanische speling, wat resulteert in een hoge positionele nauwkeurigheid en voorspelbaar regelgedrag. Daarnaast kent het systeem een hoge energie-overdrachtsefficiëntie, doordat verliezen als gevolg van mediumcontact en interne wrijving beperkt blijven. Ook is de aandrijving relatief geluidsarm, wat bijdraagt aan een robuuste en stabiele werking.

Een belangrijk bijkomend voordeel is de mogelijkheid om de actuatorconfiguratie zodanig te ontwerpen dat de motoren dichter bij het lichaam van de robothond geplaatst kunnen worden. Hierdoor wordt de massa van de bewegende delen gereduceerd, wat leidt tot een verbetering van de dynamische prestaties en energie-efficiëntie.

Tegenover deze voordelen staat dat capstan-aandrijvingen minder gangbaar zijn binnen robotische toepassingen en hogere eisen stellen aan het ontwerp en de implementatie. Met name de afhankelijkheid van wrijving, omwikkelhoek en kabelvoorspanning vereist een zorgvuldige dimensionering om slip te voorkomen en betrouwbare koppeloverdracht te garanderen.

Desondanks wordt geconcludeerd dat de voordelen, met name op het gebied van backlash, massa en dynamisch gedrag, zwaarder wegen dan de nadelen. De capstan-aandrijving vormt daarmee een geschikte en onderbouwde keuze voor toepassing binnen het actuatorsysteem van de robothond.
  
= Capstan-aandrijving
De capstan-aandrijving is een transmissiesysteem dat historisch
veelvuldig werd toegepast in maritieme toepassingen, onder meer
voor het hijsen en positioneren van lasten. Het werkingsprincipe
berust op wrijvingsversterking: een relatief klein houdkoppel kan,
via wrijving tussen kabel en trommel, resulteren in een aanzienlijk
groter belastingskoppel. Hierdoor functioneert het systeem als een
mechanische koppelversterker.

Het transmissiemechanisme bestaat typisch uit twee of meerdere
trommels waarover een kabel of riem is gewikkeld. Tijdens bedrijf
wordt het flexibele transmissiemedium van de ene trommel naar de
andere getransporteerd, waarbij krachtoverdracht plaatsvindt via
droge of gecontroleerde wrijving.

De relatie tussen het houdkoppel en het belastingskoppel wordt
beschreven door de capstanvergelijking:

$ T_("load") = T_("hold") e^(mu phi) $ <eq:capstan>

waarbij $mu$ de wrijvingscoëfficiënt tussen kabel en trommel
voorstelt en $phi$ de omwikkelhoek in radialen.

Uit vergelijking @eq:capstan blijkt dat het overdraagbare koppel
exponentieel toeneemt met de omwikkelhoek en de wrijvingscoëfficiënt.
Hierdoor kan met een beperkte ingangsbelasting een significant
uitgangskoppel worden gerealiseerd, mits voldoende contactlengte
en correcte voorspanning.

= Materiaalkeuze
Voor de capstan-aandrijving dient een geschikt transmissiemedium geselecteerd te worden, aangezien dit een cruciale rol speelt in zowel de krachtoverdracht als de betrouwbaarheid van het systeem. In het geval van een capstan betreft dit doorgaans een kabel of touw, waarbij de mechanische eigenschappen direct van invloed zijn op de prestaties van de transmissie.

Gezien de eis tot een compact ontwerp wordt gekozen voor een transmissiemedium met een relatief kleine diameter. Hierbij ontstaat echter een belangrijke ontwerpbeperking in de vorm van de zogenaamde D/d-verhouding, gedefinieerd als de verhouding tussen de diameter van de trommel (D) en de diameter van de kabel (d). Deze verhouding is bepalend voor de buigbelasting in het materiaal en daarmee voor de levensduur en mechanische eigenschappen van de kabel.

Voor staalkabels geldt doorgaans een hoge vereiste D/d-verhouding. Dit betekent dat zelfs bij een kleine kabeldiameter een relatief grote trommeldiameter noodzakelijk is om overmatige buigspanningen en vermoeiing te voorkomen. Hierdoor zijn staalkabels minder geschikt voor compacte toepassingen zoals een robothond, waar de beschikbare inbouwruimte beperkt is.

Synthetische vezels, zoals UHMWPE-gebaseerde kabels, vormen in dit opzicht een aantrekkelijk alternatief. Deze materialen vereisen een aanzienlijk lagere D/d-verhouding, waardoor kleinere trommeldiameters mogelijk zijn zonder significante degradatie van de mechanische eigenschappen. Daarnaast bieden zij een zeer hoge sterkte-gewichtsverhouding, wat gunstig is voor dynamische toepassingen.

Een veelvoorkomend nadeel van conventionele UHMWPE-kabels (zoals standaard Dyneema) is echter het optreden van *creep*, oftewel tijdsafhankelijke permanente rek onder langdurige belasting. Dit kan leiden tot verlies van voorspanning en daarmee tot verminderde koppeloverdracht of slip in het capstan-mechanisme.

Om dit probleem te mitigeren wordt in dit ontwerp gekozen voor *Dyneema DM20*. Dit materiaal is specifiek ontwikkeld voor toepassingen met statische of semi-statische belasting en vertoont een sterk gereduceerde creep-eigenschap ten opzichte van standaard UHMWPE-vezels. Hierdoor blijft de kabelspanning over langere tijd stabiel, wat essentieel is voor een betrouwbare werking van de capstan-aandrijving.

Door het gebruik van DM20 kan het creep-effect vrijwel worden geëlimineerd binnen de operationele levensduur van het systeem, waardoor de nadelen van synthetische kabels significant worden gereduceerd.

Op basis van deze overwegingen wordt geconcludeerd dat een synthetische kabel op basis van Dyneema DM20 de meest geschikte keuze vormt voor het transmissiemedium binnen de capstan-aandrijving van de robothond.

= Conclusie

In dit onderzoek is een analyse uitgevoerd naar verschillende transmissie-architecturen voor toepassing in een robothond, met specifieke aandacht voor backlash, efficiëntie, koppelcapaciteit en dynamisch gedrag. Op basis van een literatuurstudie en technische vergelijking zijn harmonische aandrijvingen, planetaire gearboxes, cycloïdale reductoren en capstan-aandrijvingen geëvalueerd.

Uit de analyse blijkt dat traditionele tandwielgebaseerde transmissies weliswaar hoge koppelcapaciteiten en robuustheid bieden, maar nadelen introduceren op het gebied van massa, backlash en efficiëntie. Deze eigenschappen zijn met name nadelig in dynamische robottoepassingen waarbij snelle en nauwkeurige bewegingen vereist zijn.

De capstan-aandrijving onderscheidt zich door een combinatie van vrijwel afwezige backlash, hoge efficiëntie en lage massa. Deze eigenschappen maken het systeem bijzonder geschikt voor toepassing in een robothond, waarbij nauwkeurige positionering en dynamisch gedrag essentieel zijn. Daarnaast maakt de capstan-configuratie het mogelijk om motoren dichter bij het lichaam te positioneren, waardoor de inertie van de bewegende beensegmenten significant wordt gereduceerd.

De werking van de capstan-aandrijving is echter sterk afhankelijk van parameters zoals wrijvingscoëfficiënt, omwikkelhoek en kabelvoorspanning. Dit stelt hogere eisen aan de dimensionering en implementatie van het systeem. Ook vormt materiaalkeuze een cruciale factor binnen het ontwerp.

In dit kader is gekozen voor een synthetisch transmissiemedium op basis van Dyneema DM20. Dit materiaal combineert een hoge sterkte-gewichtsverhouding met een sterk gereduceerde creep-eigenschap, waardoor langdurige spanningsstabiliteit gewaarborgd blijft. Hiermee wordt een belangrijk nadeel van conventionele synthetische kabels effectief gemitigeerd.

Op basis van de uitgevoerde analyse wordt geconcludeerd dat de capstan-aandrijving, in combinatie met een zorgvuldig gekozen kabelmateriaal zoals Dyneema DM20, een technisch verantwoorde en veelbelovende oplossing vormt voor het actuatorsysteem van de robothond. De voordelen op het gebied van precisie, efficiëntie en dynamiek wegen zwaarder dan de verhoogde complexiteit in ontwerp en implementatie.

Aanvullend wordt aanbevolen om de theoretische bevindingen experimenteel te valideren door middel van prototyping en testen, met specifieke aandacht voor slipgedrag, voorspanning en langdurige stabiliteit van het systeem.






