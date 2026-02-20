#import "@preview/fletcher:0.5.8" as fletcher: diagram, node, edge
#import "@preview/oxifmt:0.2.1": strfmt
#import "@preview/tablem:0.1.0": tablem
#import "@preview/numbly:0.1.0": numbly

#import "../template.typ": *
#import "../Config/GeneralConfig.typ": *

#import "@local/academic-tools:0.1.19": *
#set text(lang: "Nl")
#show link: underline

// ============================================================
// TESTPLAN — ROBOTPOOT (SUBSYSTEEM)
// ============================================================

// Handige placeholders (pas aan naar jullie ontwerp/requirements)
#let specs = (
  pos_tol: "±2° (of equivalent mm aan voet)",
  repeat_tol: "≤ 1° spreiding (3σ) na 20 herhalingen",
  settle_time: "≤ 0.5 s tot binnen tolerantie",
  max_joint_temp: "≤ 70°C behuizing (richtwaarde)",
  max_driver_temp: "≤ 80°C (richtwaarde)",
  max_current: "volgens motor/driver limiet",
  runtime_reliability: "4 uur continu (prototype) / 8 uur (streef)",
)

#show: testplan.with(
  title: [Testplan Robotpoot],
  project-name: [Robothond],
  version: "1.1",
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
  organization: [NHL Stenden - Embedded Systems],
  logo: "/files/assets/NHL_logo.jpg",

  show-version-history: true,
  version-history: (
    version(
      committee: "Ruben van der Veen",
      description: [Omgebouwd van capstan-testplan naar robotpoot-testplan (mechanica + besturing + veiligheid)],
      date: "2026-01-14",
      level: 3,
    ),
  ),

  show-toc: true,

  executive-summary: [
    Dit testplan beschrijft de teststrategie voor de Robotpoot (DUT), ontwikkeld als onderdeel van het project Robothond. De robotpoot is een kritisch subsysteem: hij bepaalt direct de bewegingskwaliteit, veiligheid en betrouwbaarheid van de demonstrator. Het doel is om de robotpoot te verifiëren en valideren op mechanische integriteit, positionering, kracht/torque-gedrag, thermische prestaties, vermogensgedrag, besturingsgedrag en fail-safe gedrag.\
  ],

  acronyms: (
    "DUT": "Device Under Test",
    "DOF": "Degree of Freedom",
    "IK": "Inverse Kinematics",
    "IMU": "Inertial Measurement Unit",
    "PID": "Proportional–Integral–Derivative",
    "FOC": "Field Oriented Control",
    "RTOS": "Real-Time Operating System",
    "MCU": "Microcontroller Unit",
    "PSoC5": "Programmable System on a Chip 5",
    "ADC": "Analog-to-Digital Converter",
    "I2C": "Inter-Integrated Circuit",
    "SPI": "Serial Peripheral Interface",
    "UART": "Universal Asynchronous Receiver-Transmitter",
    "GPIO": "General Purpose Input/Output",
    "E-Stop": "Emergency Stop",
  ),
)

// ============================================================
// HOOFDSTUK 1: INTRODUCTIE
// ============================================================

= Introductie

== Doel van het Testplan

Dit testplan definieert de teststrategie, scope, middelen en planning voor het testen van de robotpoot (DUT). De robotpoot bestaat uit mechanische delen (linkages, lagering, bevestiging), actuatie (motor + driver), sensoren (encoder/feedback) en besturing (firmware, regelkringen en veiligheidslimieten). Fouten kunnen leiden tot onnauwkeurige beweging, verhoogde slijtage of onveilige situaties (knelpunten/runaway). Het doel is het aantonen dat de robotpoot betrouwbaar, reproduceerbaar en veilig werkt binnen de beoogde demonstratie-context.

== Project Achtergrond

De robothond wordt ontwikkeld als demonstrator voor open dagen. De robotpoot is de kern van “bewegingsbeleving”: als één poot al onbetrouwbaar is, wordt lopen/animaties instabiel en risicovol. Daarom wordt de robotpoot als apart subsysteem uitgebreid getest, in lijn met de eis dat elk deelproces een eigen testplan heeft. :contentReference[oaicite:1]{index=1}

// ============================================================
// HOOFDSTUK 2: TEST SCOPE
// ============================================================

= Test Scope

== In Scope en Out of Scope

#test-scope-table(
  in-scope: (
    "Mechanische integriteit van pootdelen (speling, scheuren, bevestiging, lagering)",
    "Bereik (Range of Motion) per joint en het respecteren van joint-limits",
    "Positioneringsnauwkeurigheid en herhaalbaarheid (encoder + regeling)",
    "Dynamisch gedrag: responstijd, overshoot, stabiliteit (PID/FOC indien van toepassing)",
    "Kracht/torque gedrag (indicatief) en slip/backlash effecten",
    "Backdrivability/compliance (veiligheid bij hand-interactie tijdens demo)",
    "Thermisch gedrag motor/driver tijdens representatieve bewegingen",
    "Vermogensgedrag (gemiddeld verbruik + piekstromen)",
    "Fail-safe gedrag: E-Stop, limieten, watchdog/timeout, runaway preventie",
    "Communicatie/aansturing: commando’s, feedback, foutafhandeling",
  ),
  out-of-scope: (
    "Autonome navigatie/AI (systeemniveau)",
    "Volledige gait-kwalificatie met 4 poten (apart systeemtestplan)",
    "EMC/EMI compliance en certificering",
    "Langetermijnduurtest over meerdere maanden",
    "Behuizing/esthetiek (tenzij het mechanisch interfereert met de poot)",
  ),
)

== Functies te Testen

De volgende functies/features worden getest:

#v(1em)
=== Mechanische integriteit
- Geen blijvende vervorming bij nominale beweging/last.
- Bevestigingen blijven vast (geen loslopen) na duurtest.
- Speling/backlash blijft binnen acceptabel niveau (inspectie + meting).

#v(1em)
=== Bewegingsbereik & limieten
- Elke joint haalt het vereiste bewegingsbereik zonder aanlopen.
- Soft-limits en hard-limits (indien aanwezig) voorkomen overschrijding.

#pagebreak()


=== Positionering & herhaalbaarheid
- Doelposities worden bereikt binnen tolerantie: *#(specs.pos_tol)*.
- Herhaalbaarheid blijft binnen: *#(specs.repeat_tol)*.
- Settling time voldoet: *#(specs.settle_time)*.

#v(1em)
=== Dynamisch gedrag (regeling)
- Geen oscillatie/instabiliteit bij stapcommando’s.
- Overshoot blijft acceptabel (te definiëren) en beweging voelt “demo-veilig”.

#v(1em)
=== Kracht/torque (indicatief)
- Poot kan een representatieve belasting aan (bijv. drukken tegen de grond/fixture) zonder skippen.
- Stroomlimieten voorkomen schade bij blokkeren.

#v(1em)
=== Thermisch gedrag
- Motor/driver temperaturen blijven binnen richtwaarden: motor *#(specs.max_joint_temp)*, driver *#(specs.max_driver_temp)*.
- Geen thermische shutdown in representatieve scenario’s.

#v(1em)
=== Vermogensgedrag
- Gemiddeld verbruik tijdens testscenario’s wordt gelogd.
- Piekstromen blijven onder veilige limieten: *#(specs.max_current)*.

#v(1em)
=== Veiligheid & fail-safe
- E-Stop zet actuatie direct veilig uit.
- Watchdog/timeout: bij verlies van commando’s gaat systeem naar veilige toestand.
- Onverwachte bewegingen worden gedetecteerd/afgebroken (limieten/fault handling).

// ============================================================
// HOOFDSTUK 3: TEST STRATEGIE
// ============================================================

= Test Strategie

== Test Levels

=== Unit Testen
Unit tests op losse componenten: encoder uitlezing, motor-driver interface, limietlogica, E-Stop input, foutcodes, logging.

#v(1em)
=== Integratie Testen
Integratie tussen mechanica + actuatie + feedback + firmware: homing, position loops, commando’s en respons.

#v(1em)
=== Systeem Testen (Robotpoot als subsysteem)
End-to-end: van commando → regeling → fysieke beweging → feedback → logging → foutafhandeling.

#v(1em)
=== Acceptatie Testen (Demo-ready)
Korte demo-scenario’s (herhaalbaar): “pose”, “tap”, “lift”, “hold”, “safe stop”. Stakeholders beoordelen veiligheid en reproduceerbaarheid.

#v(1em)
== Test Types

- *Functionele testing:* bereik, homing, positionering, limieten, commando’s
- *Performance testing:* settling time, overshoot, repeatability, power
- *Reliability testing:* #specs.runtime_reliability continu met cycli
- *Safety testing:* E-Stop, timeout, current limiting, runaway preventie
- *Boundary testing:* lage/hoge supply, minimale/maximale joint-hoeken, lichte blokkade

// ============================================================
// HOOFDSTUK 4: TEST CASES
// ============================================================

= Test Cases

== Mechanische Integriteit

#test-case-table(
  (
    (
      id: "TC-101",
      description: "Visuele inspectie + spelingmeting (nulmeting)",
      expected: "Geen scheuren, correcte montage, speling/backlash binnen acceptabel (documenteer waarde)",
      type: "Functional",
      priority: "High"
    ),
    (
      id: "TC-102",
      description: "Bevestigingstest (bouten/moeren) na 30 min cycli",
      expected: "Geen loslopen; markeringen blijven uitgelijnd; indien los → borging aanpassen",
      type: "Reliability",
      priority: "High"
    ),
    (
      id: "TC-103",
      description: "Aanslag/aanlopen check over volledige bewegingsrange",
      expected: "Geen mechanisch aanlopen; kabels niet klem; geen schuren op frame",
      type: "Functional",
      priority: "High"
    )
  ),
  caption: [Robotpoot – Mechanische Integriteit Test Cases]
)

== Bewegingsbereik & Limieten

#test-case-table(
  (
    (
      id: "TC-201",
      description: "Max/min joint-hoeken bereiken (soft-limits actief)",
      expected: "Bereik gehaald zonder overschrijding; limieten begrenzen correct",
      type: "Functional",
      priority: "High"
    ),
    (
      id: "TC-202",
      description: "Hard-limit / mechanische stop validatie (indien aanwezig, met lage snelheid)",
      expected: "Systeem detecteert stop (stroom/encoder) en gaat naar veilige foutstatus",
      type: "Safety",
      priority: "High"
    ),
    (
      id: "TC-203",
      description: "Homing/zeroing procedure",
      expected: "Nulpunt is reproduceerbaar; offset wordt correct opgeslagen/logged",
      type: "Functional",
      priority: "High"
    )
  ),
  caption: [Robotpoot – Bewegingsbereik & Limieten Test Cases]
)

== Positionering & Herhaalbaarheid

#test-case-table(
  (
    (
      id: "TC-301",
      description: "Doelpositie bereiken (stapcommando) – 5 willekeurige poses",
      expected: "Binnen tolerantie: ±2°; geen instabiliteit",
      type: "Performance",
      priority: "High"
    ),
    (
      id: "TC-302",
      description: "Herhaalbaarheid: 20x dezelfde pose aanroepen",
      expected: "Spreiding eindpositie binnen #(specs.repeat_tol) (log + rapportage)",
      type: "Performance",
      priority: "High"
    ),
    (
      id: "TC-303",
      description: "Settling time meting bij stapcommando",
      expected: "Binnen #(specs.settle_time) tot binnen tolerantie (log/plot)",
      type: "Performance",
      priority: "Medium"
    ),
    (
      id: "TC-304",
      description: "Driftmeting: pose vasthouden gedurende 10 min",
      expected: "Geen significante drift; afwijking blijft binnen vooraf vastgestelde limiet",
      type: "Reliability",
      priority: "Medium"
    )
  ),
  caption: [Robotpoot – Positionering & Herhaalbaarheid Test Cases]
)

== Dynamisch Gedrag (Regeling)

#test-case-table(
  (
    (
      id: "TC-401",
      description: "Staprespons: overshoot/oscillatie check (lage, middel, hoge gains)",
      expected: "Geen aanhoudende oscillatie; overshoot acceptabel (documenteer)",
      type: "Performance",
      priority: "High"
    ),
    (
      id: "TC-402",
      description: "Snelheidsprofiel: gecontroleerd versnellen/afremmen",
      expected: "Beweging vloeiend; geen ‘stotteren’; encoder feedback consistent",
      type: "Performance",
      priority: "Medium"
    ),
    (
      id: "TC-403",
      description: "Blokkade test (kort, gecontroleerd) met stroomlimiet actief",
      expected: "Stroomlimiet grijpt in; geen schade; systeem meldt fault/recovers veilig",
      type: "Safety",
      priority: "High"
    )
  ),
  caption: [Robotpoot – Dynamisch Gedrag Test Cases]
)

== Kracht/Torque & Belastbaarheid (Indicatief)

#test-case-table(
  (
    (
      id: "TC-501",
      description: "Statische hold: poot houdt pose tegen externe kracht (fixture/handkracht) 10 s",
      expected: "Pose blijft binnen tolerantie; geen doorslippen/backdrive boven limiet",
      type: "Functional",
      priority: "Medium"
    ),
    (
      id: "TC-502",
      description: "Backdrivability/compliance check (veiligheid bij aanraken)",
      expected: "Bij duwen geeft poot gecontroleerd mee of stopt veilig (afhankelijk van mode)",
      type: "Safety",
      priority: "High"
    )
  ),
  caption: [Robotpoot – Belastbaarheid Test Cases]
)

== Thermisch Gedrag

#test-case-table(
  (
    (
      id: "TC-601",
      description: "Temperatuur motor/joint tijdens 30 min cycli",
      expected: "Motor/joint < #(specs.max_joint_temp) en prestaties blijven stabiel",
      type: "Performance",
      priority: "High"
    ),
    (
      id: "TC-602",
      description: "Temperatuur driver tijdens 30 min cycli",
      expected: "Driver < #(specs.max_driver_temp); geen thermal throttling/shutdown",
      type: "Performance",
      priority: "High"
    )
  ),
  caption: [Robotpoot – Thermisch Gedrag Test Cases]
)

== Vermogensgedrag

#test-case-table(
  (
    (
      id: "TC-701",
      description: "Gemiddeld vermogen tijdens representatieve bewegingen",
      expected: "Vermogen wordt gelogd; blijft binnen ontwerpbudget (documenteer grens)",
      type: "Performance",
      priority: "Medium"
    ),
    (
      id: "TC-702",
      description: "Stroompieken bij versnellen/afremmen + blokkade",
      expected: "Pieken blijven onder veilige limieten; geen brown-out/reset",
      type: "Safety",
      priority: "High"
    )
  ),
  caption: [Robotpoot – Vermogensgedrag Test Cases]
)

== Veiligheid & Fail-safe

#test-case-table(
  (
    (
      id: "TC-801",
      description: "E-Stop functionaliteit (tijdens beweging en tijdens hold)",
      expected: "Actuatie stopt onmiddellijk; systeem gaat naar veilige toestand",
      type: "Safety",
      priority: "High"
    ),
    (
      id: "TC-802",
      description: "Commando-timeout/watchdog (simuleer communicatieverlies)",
      expected: "Na timeout: motoren veilig uit of naar safe pose; fault gelogd",
      type: "Safety",
      priority: "High"
    ),
    (
      id: "TC-803",
      description: "Onverwachte beweging detectie (setpoint/feedback mismatch)",
      expected: "Systeem detecteert afwijking en stopt/limiteert; foutmelding aanwezig",
      type: "Safety",
      priority: "High"
    )
  ),
  caption: [Robotpoot – Veiligheid & Fail-safe Test Cases]
)

== Betrouwbaarheid (Duurtest)

#test-case-table(
  (
    (
      id: "TC-901",
      description: "Duurtest: cyclisch bewegen gedurende #(specs.runtime_reliability)",
      expected: "Geen vastlopers; geen significante drift; geen losse bevestigingen; log compleet",
      type: "Reliability",
      priority: "High"
    ),
    (
      id: "TC-902",
      description: "Post-duurtest inspectie (speling/temperatuur/geluid)",
      expected: "Geen overmatige slijtage; speling niet significant toegenomen; geen afwijkend geluid",
      type: "Reliability",
      priority: "High"
    )
  ),
  caption: [Robotpoot – Betrouwbaarheid Test Cases]
)

// ============================================================
// HOOFDSTUK 5: TEST OMGEVING
// ============================================================

= Test Omgeving

#test-environment(
  hardware: [
    - 1x Robotpoot assembly (complete: mechanica + motor/driver + encoder)
    - 1x Test- of montagefixture (om poot veilig te fixeren)
    - 1x Voeding (lab PSU of accu met zekering)
    - 1x Bekabeling + gezekerde powerlijn
    - 1x E-Stop (fysiek bereikbaar)
  ],

  software: [
    - Firmware build (commit hash / versie invullen)
    - Driver configuratie (parameterset / export)
    - Logging script/tool (UART/USB)
  ],

  tools: [
    - Multimeter + stroomtang (of shuntmeting)
    - Oscilloscoop (optioneel, voor ruis/resets)
    - IR-thermometer of thermokoppel
    - Digitale hoekmeter / gradenboog (ROM meting)
    - Schuifmaat (speling/maatvoering)
    - Weegschaal/gewichten of load cell (indicatieve krachtmeting)
    - Laptop met tooling
  ],

  data: [
    - Dit testplan
    - Testlogboek (ruwe logs)
    - Parameterlijst (gains/limits)
    - Testrapport (samenvatting + conclusies)
  ]
)

// ============================================================
// HOOFDSTUK 6: ENTRY EN EXIT CRITERIA
// ============================================================

= Entry en Exit Criteria

#criteria-section(
  entry: (
    "Robotpoot prototype is compleet gemonteerd en visueel geïnspecteerd",
    "Homing/zeroing procedure is beschikbaar en getest (minimaal 1x succesvol)",
    "Safety: E-Stop werkt en is fysiek bereikbaar",
    "Stroomlimieten/soft-limits zijn ingesteld",
    "Testfixture is stevig en voorkomt omvallen/knelgevaar",
    "Logging werkt (minimaal: timestamp + setpoint + feedback + faultcodes)",
  ),

  exit: (
    "Alle High priority test cases zijn succesvol uitgevoerd",
    "Geen kritieke safety issues openstaand (E-Stop/timeout/runaway)",
    "≥ 90% van alle test cases geslaagd (of afwijkingen met mitigerende maatregelen vastgelegd)",
    "Duurtest uitgevoerd en post-inspectie gedocumenteerd",
    "Testrapportage is compleet en gereviewd",
  )
)

// ============================================================
// HOOFDSTUK 7: RISICO ANALYSE
// ============================================================

= Risico Analyse

#risk-assessment-table(
  (
    (
      risk: "Knel-/impactgevaar (bewegende poot)",
      description: "Vingers/kabels kunnen tussen linkages komen of poot kan uitslaan bij runaway",
      impact: "Hoog",
      mitigation: "Test in fixture, E-Stop direct bereikbaar, lage snelheid bij eerste tests, afscherming/afstand houden"
    ),
    (
      risk: "Runaway door fout in regeling/encoder",
      description: "Fout signaal of verkeerde polarity kan leiden tot versnellen tot aanslag",
      impact: "Hoog",
      mitigation: "Soft-limits, current limit, watchdog, testen met lage spanning en lage gains, eerst no-load"
    ),
    (
      risk: "Thermische overbelasting",
      description: "Motor/driver kan oververhitten bij langdurig holden of blokkade",
      impact: "Hoog",
      mitigation: "Temperatuurmonitoring, duty-cycle beperken, thermische shutdown/limits configureren"
    ),
    (
      risk: "Mechanische breuk (3D prints/lagers)",
      description: "Onder belasting kan een onderdeel scheuren of lager loskomen",
      impact: "Medium",
      mitigation: "Nulmeting + inspectie, conservatieve belasting, reserveonderdelen, boutborging"
    ),
    (
      risk: "Tijdconstraint / te veel scope",
      description: "Niet alle tests passen in de planning",
      impact: "Medium",
      mitigation: "MoSCoW op testcases: eerst safety + positionering + duurtest, rest later"
    ),
  ),
  caption: [Robotpoot – Risico’s en Mitigatie Strategieën]
)

// ============================================================
// HOOFDSTUK 8: TEST DELIVERABLES
// ============================================================

= Test Deliverables

De volgende documenten en artefacten worden opgeleverd:

+ *Test Plan* (dit document)
+ *Test Cases Specificatie* – details per test (opstelling, stappen, meetmethode)
+ *Test Uitvoering Logboek* – ruwe logs + meetwaarden
+ *Bug/Issue Lijst* – defects met status en reproduceerbaarheid
+ *Test Samenvatting Rapport* – resultaten, conclusies, aanbevelingen
+ *Go/No-Go* – demo-ready beslissing op basis van safety + betrouwbaarheid

// ============================================================
// HOOFDSTUK 9: GOEDKEURING
// ============================================================

= Goedkeuring

Dit testplan moet worden goedgekeurd door de volgende stakeholders voordat de testfase kan beginnen:

#v(2em)

#grid(
  columns: (1fr, 1fr),
  column-gutter: 2em,
  row-gutter: 3em,

  [
    *Naam:* R. Moedt\
    *Rol:* Opdrachtgever / Beoordelaar\
    *Handtekening:*  \
    *Datum:*
  ],

  [
    *Naam:* (Reviewer invullen)\
    *Rol:* Technical Reviewer\
    *Handtekening:*  \
    *Datum:*
  ],
)
