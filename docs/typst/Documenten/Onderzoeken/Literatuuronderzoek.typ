#import "@preview/fletcher:0.5.8" as fletcher: diagram, node, edge
#import "template.typ": *
#import "config.typ": *
#set text(lang: "Nl")

// Front page and general definitions
#show: academic-frontpage.with(
  title: [Literatuuronderzoek],
  author: students.map(s => s.name).join(", "),
  degree: [#degree],
  degree-goal: [Onderzoek voor het realiseren van een Robothond],
  department: [#department],
  university: [#university, #universitydesc],
  program-type: [#program],
  degree-year: [#year],
  location: [#city, #country],
  supervisor: [#supervisor],
  tutor: [#tutor],
  logo: "/Assets/NHL_logo.jpg",
  month: [#month],
  year: [#year],
  abstract: [In dit project ontwikkelen wij als bachelorstudenten van NHL Stenden een embedded 
    systeem in vorm van een Robothond. Het apparaat is bedoeld als demonstrator 
    op opendagen om de mogelijkheden van technische innovatie aan toekomstige 
    studenten uit te leggen. We volgen hiervoor de Design-Thinking methodologie met 
    nadruk op iteratieve prototyping en uitvoerige testen. Doel is een volledig 
    functioneel product te realiseren dat voldoet aan de stakeholder-eisen en markt-
    gereed is.],
  keywords: [],
  acknowledgments: [],
  acronyms: (
    "PvE": ("Pakket van Eisen","Pakket van Eisen"),
    "MVP": ("Minimum Viable Product","Minimum Viable Product"),
    "PvA": ("Plan van Aanpak","Plan van Aanpak"),
    //"MVP": "Minimum Viable Product",
    //"PvA": "Plan van Aanpak",
  ),
)

#table(
  columns: (auto, auto),
  align: (left, center),
  inset: 6pt,
  stroke: 0.5pt + black,
  
  // Header
  [*MoSCoW-analyse Robothond*], [],
  
  // Kolomkoppen
  [*Eis / Beschrijving*], [*Prioriteit*],
  
  // ---------------------------
  // MUST HAVE (1)
  // ---------------------------
  [*Must-have eisen*], [],
  
  [Het systeem gebruikt een PSoC5 als primaire controller.], [1],
  [Het systeem bevat minimaal één draadloze communicatie of regelsysteem of beide.], [1],
  [Er is minimaal één HDL-component (Verilog).], [1],
  [Er wordt input verwerkt van minimaal één sensor of gebruiker.], [1],
  [Het product toont een demonstratiebare functionaliteit geschikt voor open dagen.], [1],
  [Het systeem bevat minimaal twee energiebesparingsmaatregelen.], [1],
  [Geen spontane resets of crashes tijdens gebruik.], [1],
  [Gebruiksvriendelijk voor niet-technische gebruikers.], [1],
  [Mechanisch robuust tijdens normaal gebruik.], [1],
  [Datacommunicatie is betrouwbaar en reproduceerbaar.], [1],
  [Alle ontwerpkeuzes zijn onderbouwd en traceerbaar.], [1],
  [Alle schema’s, code en ontwerpen worden als bijlage toegevoegd.], [1],
  [Documentatie volgt het Design-Thinking-proces.], [1],
  [Er is een PvE met MoSCoW-prioriteiten.], [1],
  [Er is een MVP gedefinieerd.], [1],
  [Er is een PvA met planning, rollen en risicoanalyse.], [1],
  [Het team houdt één gezamenlijk logboek bij.], [1],
  [Wekelijkse tutor-gesprekken met notulen.], [1],
  [Gebruik van versiebeheer (Git).], [1],
  [Peer review op PvE en blokdiagram is uitgevoerd en verwerkt.], [1],
  [Teamrollen zijn toegewezen volgens hoofdstuk 1.1–1.2.], [1],
  [MVP — lopen op vier poten met inverse kinematics.], [1],
  [MVP — bestuurbaar via een controller.], [1],
  
  // ---------------------------
  // SHOULD HAVE (2)
  // ---------------------------
  [*Should-have eisen*], [],
  
  [Efficiënt energieverbruik + ondersteuning slaapstanden.], [2],
  [Knoppen zijn duidelijk gelabeld en toegankelijk.], [2],
  [Consistente performance tijdens demonstraties.], [2],
  [Volledige traceerbaarheid eisen → ontwerp → test.], [2],
  [High-level en detailed blokdiagram volgens standaard.], [2],
  [Testplannen sluiten aantoonbaar aan op eisen.], [2],

  // ---------------------------
  // COULD HAVE (3)
  // ---------------------------
  [*Could-have eisen*], [],
  
  [Autonome navigatie door het schoolgebouw.], [3],
  [Soundboard / audiofuncties.], [3],
  [Koffieschenksysteem met pomp of actuator.], [3],
  [Dansfunctionaliteit of animaties.], [3],
  [Uitgebreide AI-persoonlijkheid of gedrag.], [3],
  [Extra draadloze protocollen.], [3],
  [Esthetische uitbreidingen zoals verlichting of covers.], [3],

  // ---------------------------
  // WON’T HAVE (4)
  // ---------------------------
  [*Won’t-have eisen*], [],
  
  [Volledig commerciële of marktklare robothond.], [4],
  [Geavanceerde SLAM- of LIDAR- systemen.], [4],
  [Autonome routeplanning buiten school.], [4],
  [Smartphone-app of cloudkoppelingen.], [4],
  [Modulair, industrieel been- of chassisplatform.], [4],
)
