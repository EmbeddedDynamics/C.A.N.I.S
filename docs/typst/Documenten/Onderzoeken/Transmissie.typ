#import "@preview/unify:0.7.1": num,qty,numrange,qtyrange

#import "@local/academic-tools:0.1.8": *

#set text(lang: "nl")

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
  extend-abstract: true,
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
  references: "/zotero.bib",
  reference-style: "ieee",
)



#place-columns()[
  = Introductie

  Het ontwerpen van een functionele robothond vereist zorgvuldige en onderbouwde ontwerpkeuzes, in het bijzonder met betrekking tot het mechanische transmissiesysteem. Dit document richt zich op de transmissie-architectuur, een kerncomponent in het actuatorsysteem, die bepalend is voor de volgende prestatiecriteria:
  
  + *Minimale speling (backlash)* — ter voorkoming van accumulerende positiefouten en ter waarborging van nauwkeurige en reproduceerbare bewegingen.
  + *Hoge energie-overdrachtsefficiëntie* — met het oog op het maximaliseren van de operationele batterijduur en het reduceren van thermische verliezen.
  + *Hoog mechanisch koppel* — noodzakelijk om robuuste interacties met de omgeving mogelijk te maken, waaronder het dragen van belasting en het overwinnen van obstakels.
  + *Optimalisatie van motorsnelheid* — om een gunstige balans te realiseren tussen motorbesturing, dynamische respons en controllerbandbreedte.
  
  Het in onderlinge samenhang afwegen van deze ontwerpeisen is essentieel voor het realiseren van een actuatorsysteem dat zowel nauwkeurig als betrouwbaar functioneert.
  
  = Kritische ontwerpeisen
  
  Binnen robotische systemen vormt het minimaliseren van backlash een fundamentele ontwerpprioriteit. Backlash wordt gedefinieerd als de ongewenste mechanische speling tussen samenwerkende tandwielen of transmissie-elementen en beïnvloedt zowel de positionele nauwkeurigheid als de dynamische prestaties van het systeem. De belangrijkste negatieve effecten zijn:
  
  - *Accumulatie van positionele fouten*: Zelfs geringe speling kan zich over opeenvolgende bewegingen ophopen, waardoor onzekerheid in de werkelijke pootpositie ontstaat en de stabiliteit van de beweging wordt aangetast.
  - *Bewegingsimprecisie*: Richtingsveranderingen (bijvoorbeeld een omschakeling van hef- naar daalbeweging) kunnen leiden tot schokkerig gedrag wanneer speling aanwezig is, hetgeen natuurlijke loop- of manoeuvreerpatronen verstoort.
  - *Versnelde slijtage*: Herhaalde wisselingen in belasting en draairichting leiden tot verhoogde materiaalinteractie in de tandwielovergangen, wat de structurele levensduur van de transmissie vermindert.
  
  Gezien deze nadelige gevolgen dient backlashreductie beschouwd te worden als een *primaire ontwerpeis* voor het actuator- en transmissiesysteem. Naast backlash is het ook essentieel om efficiëntie van de overdacht mee te nemen, aangezien embedded devices gelimiteerde energie capaciteiten hebben.
  
  = Huidige transmissie-opties
  
  Diverse transmissie-architecturen zijn in staat om lage tot zeer lage speling te realiseren. Elk van deze opties brengt specifieke voordelen en beperkingen met zich mee, afhankelijk van de beoogde toepassing:
  
  + *Harmonische aandrijvingen*: Kenmerkend door een zeer lage backlash ($ approx numrange("2", "4", thousandsep: "'") " boogminuten"$)@noauthor_comparison_2021, maar relatief kostbaar en beperkt in maximaal overdraagbare koppel.
  
  + *Planetaire gearbox*: Hoge mechanische efficiëntie ($ approx numrange("75", "99", thousandsep: "'") "%"$)@august_dynamics_1984 @bertoldi_efficiency_2021, matige backlash (15–30 boogseconden) en een compact designs.
  
  + *Cycloidal reductoren*: Ondersteunen hoge reductieverhoudingen, lage backlash (5–15 boogseconden) en zijn van nature niet-terugdraaibaar.
  
  + *Capstan drives*: Biedt extreem lage backlash ($approx 0 "boogminuten"$)@mazumdar_synthetic_2017, hoge efficiëntie ($ approx numrange("95", "96", thousandsep: "'") "%"$) @mazumdar_synthetic_2017 en simpele designs
  
  In de verdere analyse zullen deze transmissie-opties worden geëvalueerd aan de hand van kosten, efficiëntie, backlash prestaties en integratiecomplexiteit, met als doel een optimaal transmissieconcept te selecteren voor de beoogde robotische toepassing.

  = Architectuur Analyse 
  Om te bepalen welke transmissie architectuur het beste bij onze doeleindes passen, gaan wij de verschillende opties analyseren. Hierbij ligt de hoofd focus op backlash en efficiëntie. 
]