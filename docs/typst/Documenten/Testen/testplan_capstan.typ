#import "@preview/fletcher:0.5.8" as fletcher: diagram, node, edge

#import "@preview/oxifmt:0.2.1": strfmt

#import "@preview/tablem:0.1.0": tablem
#import "@preview/numbly:0.1.0": numbly

#import "../template.typ": *
#import "../Config/config.typ": *

#import "@local/academic-tools:0.1.10": *
#set text(lang: "Nl")

#show link: underline

// ============================================================
// TESTPLAN VOORBEELD
// ============================================================

#show: testplan.with(
  title: [Testplan capstan drive],
  project-name: [Robothond],
  version: "1.0",
  date: datetime.today(),
  authors: (
    "Ruben van der Veen",
    "Perijn Huijser",
    "Daan Smit",
  ),
  reviewers: (
    "R. Moedt",
    "R. van der Veen"
  ),
  approver: "R. Moedt",
  organization: [NHL Stenden - Elektronica 2],
  logo: "/files/assets/NHL_logo.jpg",

  show-version-history: true,
  version-history: (
    version(
      committee: "Ruben van der Veen",
      description: [Initiële commit],
      date: "2025-12-15",
      level: 3,
    ),
  ),
  
  show-toc: true,
  
  executive-summary: [
    Dit testplan beschrijft de teststrategie voor de capstan drive, ontwikkeld als onderdeel van het project Robothond. De capstan drive is een kritisch onderdeel van het aandrijvingssysteem van het project, en vereist nauwkeurig en robuust te werken. De correcte functie is essentieel om een goed resultaat te leveren aan de stakeholders.\
  ],
  
  acronyms: (
    "DUT": "Device under Test",
    "ADC": "Analog-to-Digital Converter",
    "I2C": "Inter-Integrated Circuit",
    "SPI": "Serial Peripheral Interface",
    "UART": "Universal Asynchronous Receiver-Transmitter",
    "MCU": "Microcontroller Unit",
    "GPIO": "General Purpose Input/Output",
  ),
)

// ============================================================
// HOOFDSTUK 1: INTRODUCTIE
// ============================================================

= Introductie

== Doel van het Testplan

Dit testplan definieert de teststrategie, scope, middelen en planning voor 
het testen van de capstan drive. De capstan drive is verantwoordelijk is voor de krachtoverbrenging en positionering van het product en speelt dus een cruciale rol binnen het project. Fouten in dit systeem kunnen leiden tot onnauwkeurigheid, verhoogd slijtage of onveilige situaties. Het doel van dit testplan is het verifiëren en valideren van de capstan drive ten opzichte van de vastgestelde functionele en niet-functionele eisen en richt het zich specifiek op het onderbouwen van de betrouwbaarheid en geschiktheid van de capstan drive binnen zijn toepassing.

== Project Achtergrond

De capstan drive is een essentieel onderdeel van het robothond project, dat wordt ontwikkeld als onderdeel van het Embedded Systems project volgens de Design Thinking-methodologie. Het eind product is bedoeld voor demonstratie tijdens open dagen en moet daarom robuust en gebruiksvriendelijk zijn. 

// ============================================================
// HOOFDSTUK 2: TEST SCOPE
// ============================================================

= Test Scope

== In Scope en Out of Scope

#test-scope-table(
  in-scope: (
    "Slijtage van mechanische onderdelen tijdens langdurige belasting",
    "Positioneringsnauwkeurigheid en herhaalbaarheid over tijd",
    "Langetermijngedrag van motor, encoder en driver",
    "Robuustheid van de motor en driver onder nominale belasting",
    "Vermogensverbruik tijdens operatie",
    "Geluid en vibratie tijdens operatie",
  ),
  out-of-scope: (
    "PCB fabricage proces",
    "Enclosure design en productie",
    "Lange-termijn testen (> 6 maanden)",
    "EMC/EMI compliance testing",
    "Certificering procedures",
  ),
)

== Functies te Testen

De volgende features worden getest:

#v(1em)
=== Krachtoverbrenging
- De capstan kan de kracht overbrengen zonder slip in het touw bij nominale belasting.
- De onderdelen hebben geen blijvende vervorming bij nominale belasting.

#v(1em)
=== Mechanische slijtage-indicatoren
- Slijtage aan het capstan-oppervlak, lagering en touw wordt beoordeeld na een duurzaamheidstest.

#v(1em)
=== Positioneringsfunctionaliteit
- De capstan drive bereikt een doelpositie en houdt deze vast binnen een vooraf bepaalde tolerantie. 
- Bij langdurig gebruik behoudt de drive zijn positie nauwkeurigheid.

#v(1em)
=== Thermisch gedrag
- De temperatuur van motor en driver blijft binnen veilige grenzen en prestaties blijven acceptabel bij opwarming.

#v(1em)
=== Vermogensgedrag
- De capstan drive wordt getest op het gemiddelde vermogens verbruik om het energieverbruik te bepalen tijdens operatie.

#v(1em)
=== Geluid en vibratie
- Het geluid van de capstan drive is niet opmerkelijk en storend tijdens operatie.
- De vibraties van de capstan drive hebben geen invloed op de functionaliteit van het systeem.

// ============================================================
// HOOFDSTUK 3: TEST STRATEGIE
// ============================================================

= Test Strategie

== Test Levels

=== Unit Testen
Unit tests worden uitgevoerd voor individuele software modules en hardware 
componenten. Deze tests verifiëren de correcte werking van geïsoleerde 
componenten.

#v(1em)
=== Integratie Testen
Integratie testen verifiëren de communicatie en interactie tussen verschillende modules.

#v(1em)
=== Systeem Testen
Systeem testen valideren het complete systeem volgens de requirements, inclusief 
end-to-end scenarios.

#v(1em)
=== Acceptatie Testen
Acceptatie Testen worden uitgevoerd door stakeholders tijdens de open dag demonstratie.

#v(1em)
== Test Types

- *Functionele Testing:* Verificatie van alle functionele requirements
- *Performance Testing:* Response time, refresh rate, power consumption
- *Usability Testing:* Gebruiksvriendelijkheid van de interface
- *Reliability Testing:* Stabiliteit bij langdurig gebruik (48 uur continue werking)
- *Boundary Testing:* Gedrag bij extreme temperaturen (0°C en 100°C)

// ============================================================
// HOOFDSTUK 4: TEST CASES
// ============================================================

= Test Cases

== Krachtoverbrenging

#test-case-table(
  (
    (
      id: "TC-101",
      description: "Sliptest bij nominale belasting",
      expected: "Geen zichtbare slip en positie-afwijking binnen tolerantie",
      type: "Functional",
      priority: "High"
    ),
    (
      id: "TC-102",
      description: "Stabiliteit bij nominale belasting",
      expected: "Drive functioneert stabiel zonder onderbrekingen of slip",
      type: "Functional",
      priority: "High"
    ),
    (
      id: "TC-103",
      description: "Mechanische vervorming inspectie",
      expected: "Geen blijvende vervorming van capstan of bevestiging",
      type: "Functional",
      priority: "Medium"
    )
  ),
  caption: [Capstan Drive – Krachtoverbrenging Test Cases]
)

== Mechanische Slijtage

#test-case-table(
  (
    (
      id: "TC-201",
      description: "Slijtage-inspectie na duurzaamheidstest (6 uur)",
      expected: "Slijtage binnen acceptabel niveau; geen beschadiging",
      type: "Reliability",
      priority: "High"
    ),
    (
      id: "TC-202",
      description: "Lager- en asinspectie na belasting",
      expected: "Geen verhoogde speling of hoorbare slijtage",
      type: "Reliability",
      priority: "Medium"
    )
  ),
  caption: [Capstan Drive – Mechanische Slijtage Test Cases]
)

== Positioneringsfunctionaliteit

#test-case-table(
  (
    (
      id: "TC-301",
      description: "Bereiken van doelpositie",
      expected: "Doelpositie bereikt binnen vooraf vastgestelde tolerantie",
      type: "Functional",
      priority: "High"
    ),
    (
      id: "TC-302",
      description: "Herhaalbaarheid van positionering",
      expected: "Spreiding eindpositie blijft binnen tolerantie",
      type: "Performance",
      priority: "High"
    ),
    (
      id: "TC-303",
      description: "Driftmeting tijdens langdurig gebruik",
      expected: "Positie-afwijking blijft binnen limiet over tijd",
      type: "Performance",
      priority: "Medium"
    )
  ),
  caption: [Capstan Drive – Positioneringsfunctionaliteit Test Cases]
)

== Thermisch gedrag

#test-case-table(
  (
    (
      id: "TC-401",
      description: "Temperatuur motor bij nominale belasting",
      expected: "Temperatuur blijft onder maximale grenswaarde",
      type: "Performance",
      priority: "High"
    ),
    (
      id: "TC-402",
      description: "Temperatuur driver bij langdurige belasting",
      expected: "Geen thermische beveiliging of prestatieverlies",
      type: "Performance",
      priority: "High"
    )
  ),
  caption: [Capstan Drive – Thermisch Gedrag Test Cases]
)

== Vermogensgedrag

#test-case-table(
  (
    (
      id: "TC-501",
      description: "Gemiddeld vermogensverbruik tijdens operatie",
      expected: "Vermogensverbruik blijft binnen vastgestelde grenzen",
      type: "Performance",
      priority: "Medium"
    ),
    (
      id: "TC-502",
      description: "Stroompieken tijdens versnellen en afremmen",
      expected: "Geen overschrijding van veilige stroomlimieten",
      type: "Performance",
      priority: "Medium"
    )
  ),
  caption: [Capstan Drive – Vermogensgedrag Test Cases]
)

== Geluid en Vibratie

#test-case-table(
  (
    (
      id: "TC-601",
      description: "Geluidsniveau tijdens normale operatie",
      expected: "Geluid is niet storend of afwijkend",
      type: "Non-functional",
      priority: "Low"
    ),
    (
      id: "TC-602",
      description: "Invloed van vibraties op positionering",
      expected: "Vibraties hebben geen invloed op functionaliteit",
      type: "Non-functional",
      priority: "Medium"
    )
  ),
  caption: [Capstan Drive – Geluid en Vibratie Test Cases]
)




// ============================================================
// HOOFDSTUK 5: TEST OMGEVING
// ============================================================

= Test Omgeving

#test-environment(
  hardware: [
    - 1x Capstan Drive Assembly
    - 1x Festo bevestegingsplaat
    - 4x M6 T-nuts
    - 4x M6x20 bouten
    - 1x 40mm extrusion 100+ mm
  ],
  
  software: [
    - Odrivetool V0.6.10
  ],
  
  tools: [
    - Power supply (36V/10A)
  ],
  
  data: [
    - Testplan
  ]
)

// ============================================================
// HOOFDSTUK 6: ENTRY EN EXIT CRITERIA
// ============================================================

= Entry en Exit Criteria

#criteria-section(
  entry: (
    "Hardware prototype is compleet en functioneel",
    "Firmware is geïnstalleerd en basic functionaliteit is geverifieerd",
    "Test omgeving is opgezet en geconfigureerd",
    "Test cases zijn goedgekeurd door projectleider",
    "Test data en referentiewaarden zijn beschikbaar",
  ),
  
  exit: (
    "Alle High priority test cases zijn succesvol uitgevoerd",
    "≥ 95% van alle test cases zijn geslaagd",
    "Alle kritieke bugs zijn opgelost",
    "Test rapportage is compleet en goedgekeurd",
    "Acceptance test is succesvol afgerond",
  )
)

// ============================================================
// HOOFDSTUK 7: RISICO ANALYSE
// ============================================================

= Risico Analyse

#risk-assessment-table(
  (
    (
      risk: "Hardware breekt",
      description: "De 3D geprinte onderdelen kunnen onder zware belasting breken, wat een gevaarlijke situatie veroorzaakt voor de tester",
      impact: "Hoog",
      mitigation: "Backup hardware beschikbaar, voorzichtige handelen, bij kleine vervorming stoppen met testen"
    ),
    (
      risk: "Sensor kalibratie",
      description: "Calibratie voldoet niet aan specificaties",
      impact: "Hoog",
      mitigation: "Voor een nieuwe calibratie sequentie uit."
    ),
    (
      risk: "Tijd constraint",
      description: "Onvoldoende tijd voor complete test coverage",
      impact: "Medium",
      mitigation: "Prioriteren van kritieke tests, parallelle uitvoering"
    ),
    (
      risk: "Software bugs",
      description: "Late ontdekking van kritieke firmware bugs",
      impact: "Hoog",
      mitigation: "Early integration testing, code reviews"
    ),
  ),
  caption: [Risico's en Mitigatie Strategieën]
)

// ============================================================
// HOOFDSTUK 8: TEST PLANNING
// ============================================================

/*= Test Planning

== Test Schema

#test-schedule-table(
  (
    (
      phase: "Test Voorbereiding",
      start-date: "14-11-2025",
      end-date: "18-11-2025",
      deliverables: "Test omgeving setup, test data"
    ),
    (
      phase: "Unit Testing",
      start-date: "18-11-2025",
      end-date: "22-11-2025",
      deliverables: "Unit test rapportage"
    ),
    (
      phase: "Integration Testing",
      start-date: "25-11-2025",
      end-date: "29-11-2025",
      deliverables: "Integration test rapportage"
    ),
    (
      phase: "System Testing",
      start-date: "02-12-2025",
      end-date: "06-12-2025",
      deliverables: "System test rapportage, bug lijst"
    ),
    (
      phase: "Bug Fixing & Retesting",
      start-date: "09-12-2025",
      end-date: "13-12-2025",
      deliverables: "Geüpdatete firmware, retest rapport"
    ),
    (
      phase: "Acceptance Testing",
      start-date: "16-12-2025",
      end-date: "20-12-2025",
      deliverables: "Final test rapport, go/no-go beslissing"
    ),
  ),
  caption: [Test Uitvoeringsschema]
)

== Rollen en Verantwoordelijkheden

- *Test Lead (Jan Jansen):* Overall test planning en coördinatie
- *Test Engineer (Lisa de Vries):* Test uitvoering en rapportage
- *Developer Support:* Bug fixing en firmware updates
- *Project Supervisor (Dr. A. van der Berg):* Review en approval*/

// ============================================================
// HOOFDSTUK 9: DELIVERABLES
// ============================================================

= Test Deliverables

De volgende documenten en artefacten worden opgeleverd:

+ *Test Plan* (dit document)
+ *Test Cases Specificatie* - Gedetailleerde test procedures
+ *Test Uitvoering Logboek* - Dagelijkse voortgang registratie
+ *Bug Tracking Sheet* - Alle gevonden defects met status
+ *Test Samenvatting Rapport* - Eindrapportage met statistieken
+ *Acceptance Test Certificaat* - Goedkeuring door stakeholders

= Goedkeuring

Dit testplan moet worden goedgekeurd door de volgende stakeholders voordat 
de testfase kan beginnen:

#v(2em)

#grid(
  columns: (1fr, 1fr),
  column-gutter: 2em,
  row-gutter: 3em,
  
  [
    *Naam:* Dr. P. Huijser\
    *Rol:* Lead mechanical engineer \
    *Handtekening:*  \
    *Datum:* 
  ],
  
  [
    *Naam:* Dr. R. van der Veen\
    *Rol:* Technical Reviewer \
    *Handtekening:*  \
    *Datum:* 
  ],
)
