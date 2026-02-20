#import "@preview/unify:0.7.1": num,qty,numrange,qtyrange

#import "@local/academic-tools:0.1.20": *

#set text(lang: "nl")
#set math.equation(numbering: "(1)")

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
  logo: [#image("../Assets/NHL_logo.jpg", width: 140pt)],
  
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

= Introductie

Het ontwerpen van een functionele robothond vereist zorgvuldige en onderbouwde ontwerpkeuzes, in het bijzonder met betrekking tot het mechanische transmissiesysteem. Dit document richt zich op de transmissie-architectuur, een kerncomponent in het actuatorsysteem, die bepalend is voor de volgende prestatiecriteria:

+ *Minimale speling (backlash)* --- ter voorkoming van accumulerende positiefouten en ter waarborging van nauwkeurige en reproduceerbare bewegingen.
+ *Hoge energie-overdrachtsefficiëntie* --- met het oog op het maximaliseren van de operationele batterijduur en het reduceren van thermische verliezen.
+ *Hoog mechanisch koppel* --- noodzakelijk om robuuste interacties met de omgeving mogelijk te maken, waaronder het dragen van belasting en het overwinnen van obstakels.
+ *Optimalisatie van motorsnelheid* --- om een gunstige balans te realiseren tussen motorbesturing, dynamische respons en controllerbandbreedte.

Het in onderlinge samenhang afwegen van deze ontwerpeisen is essentieel voor het realiseren van een actuatorsysteem dat zowel nauwkeurig als betrouwbaar functioneert.

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
  Maken hoge reductieverhoudingen per trap mogelijk en combineren dit met een lage
  hoekspeling ($approx$ #zi.arcsecond(10)). Door de meervoudige contactpunten tussen
  schijf en penconstructie wordt een hoge schokbestendigheid en lastverdeling
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