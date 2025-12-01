#import "@preview/fletcher:0.5.8" as fletcher: diagram, node, edge

#import "@preview/oxifmt:0.2.1": strfmt

#import "@preview/tablem:0.1.0": tablem
#import "@preview/numbly:0.1.0": numbly

#import "template.typ": *
#import "Config/config.typ": *
#import "Stakeholder_analyse.typ": *

#import "role-calculations.typ" as rc

#let stakeholder_content = include "Stakeholder_analyse.typ"

#set text(lang: "Nl")

#show link: underline

//---------------------------------------
// Front page and general definitions
//---------------------------------------
#show: academic-frontpage.with(
  title: [Embedded Systems Project\ Robothond],
  authors: students.map(s => s.name).join(", "),
  degree: [#degree],
  degree-goal: [Onderzoek voor het realiseren van een Robothond],
  department: [#department],
  university: [#university, #universitydesc],
  program-type: [#program],
  degree-year: [#year],
  location: [#city, #country],
  supervisor: [#supervisor],
  tutor: [#tutor],
  logo: "/Documenten/Assets/NHL_logo.jpg",
  project-logo: "/Documenten/Assets/ED_Logo.png",
  //month: [#month],
  //year: [#year],
  date: datetime.today(),
  abstract: [In dit project ontwikkelen wij als bachelorstudenten van NHL Stenden een embedded systeem in vorm van een Robothond. Het apparaat is bedoeld als demonstrator op opendagen om de mogelijkheden van technische innovatie aan toekomstige studenten uit te leggen. We volgen hiervoor de Design-Thinking methodologie met nadruk op iteratieve prototyping en uitvoerige testen. Het doel is een volledig functioneel product te realiseren dat voldoet aan de stakeholder-eisen en markt-gereed is.],
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

//---------------------------------------
// Document contents
//---------------------------------------
= Organisatie

Voor het project "Embedded Systems" moet er een embedded device gerealiseerd worden. Voordat wij gaan beginnen met het realiseren van een apparaat gaan wij extensief onderzoek uitvoeren. Als aller eerst is er een organisatie definitie vereist. Hierin gaan wij onze organisatie definiëren voor een goed structuur en efficiënte samenwerking.

#v(1em)

== Team Structuur <team-structuur>

#let teamstructure = students.map(s => (
  [#s.role],
  [#s.name],
  [#s.responsibilities],
)).flatten()

#table(
  columns: 3,
  stroke: 0.7pt,
  inset: 6pt,
  align: (left, left, left),
  [*Rollen*], [*Team Lid*], [*Verantwoordelijkheid*],
  ..teamstructure
)


#v(1em)
#pagebreak()

== Rol Beschrijvingen <rollen>

=== Project Leider

De Project Leider is verantwoordelijk voor het coördineren van alle aspecten van het project. De project leider houdt de deadlines bij en zorgt ervoor dat deze gehaald kunnen worden. Deze rol dient als bindende factor tussen het team en externe stakeholders.

*Hoofd Verantwoordelijkheden:*
- Regelmatig team vergaderingen plannen en leiden
- Progressie bijhouden tegen de gemaakte planning
- Risico's identificeren en mitigeren
- Conflicten identificeren en oplossen
- Planning op orde houden en aanpassen waar nodig
- Dient als primaire contact persoon tussen de stakeholders
- Rapportage naar tutor en docenten

#v(1em)

=== AI Developer

De AI Developer is verantwoordelijk voor de ontwikkeling van de AI-gerelateerde features van de Robothond. De nadruk ligt op het maken van bewuste, onderbouwde keuzes over wat haalbaar en praktisch is gegeven de hardware beperkingen. Naast implementatie is het essentieel dat alle AI features grondig worden getest voordat integratie in de embedded pipeline plaatsvindt.

*Hoofd Verantwoordelijkheden:*
- Evalueer AI feature requirements op haalbaarheid en performance impact
- Ontwerp en implementeer AI algoritmes en modellen
- Voer uitgebreide testen uit op AI features (unit tests, integratie tests)
- Integreer AI features in de embedded firmware pipeline
- Documenteer AI beslissingen en gekozen algoritmes
- Optimaliseer AI modellen voor embedded hardware (resource constraints)
- Werk samen met de Software Developer voor naadloze integratie

#v(1em)
#pagebreak()

=== Hardware Engineer

De Hardware Engineer heeft als taak om alle hardware gerelateerde aspecten te behandelen, zoals: PCB design, componenten selectie en circuit design. Naast het elektrotechnische aspect is het ook belangrijk voor de hardware engineer om ook de CAD aspecten op te nemen, zoals: 3D tekenen, technische tekeningen maken, assemblage tekeningen maken indien nodig en het hardware te testen.

*Hoofd Verantwoordelijkheden:*
- Design circuit schema's en verificatie
- Selecteert de juiste componenten op basis van specificaties
- Maakt PCB layouts, breadboard designs en schema's
- Test de functionaliteit van de hardware
- Documenteert hardware specificaties, progressie en onderzoeken
- Leidt hardware reviews en troubleshooting
- Zorgt voor versionering van CAD bestanden

#v(1em)

=== Git Master

De Git Master is verantwoordelijk voor het beheren van de git repository. Hierbij wordt gelet op nette commits, feature branches, pull requests en duidelijke beschrijvingen die voldoen aan de gestelde git-richtlijnen. De Git Master hanteert een branching strategie die aansluit bij de projectstructuur en zorgt voor een schone, reproduceerbare codebase.

*Hoofd Verantwoordelijkheden:*
- Pull requests reviewen en goedkeuren voordat ze worden gemerged naar de main branch
- Verzekeren dat alle commits duidelijk gedocumenteerd zijn met beschrijvende berichten
- De repository beschermen tegen conflicting merges en ongestructureerde commits
- Een consistent branching strategy hanteren (bijv... Git Flow of trunk-based development)
- Merge conflicts oplossen en escaleren indien nodig
- Toegangsrechten en repository-instellingen beheren

#v(1em)
#pagebreak()

=== Software Developer

De Software Developer heeft als taak om de firmware ontwikkeling bij te houden. Hierbij ligt de focus op: code kwaliteit, goede systeem integratie en het uitvoeren van functionele testen.

*Hoofd Verantwoordelijkheden:*
- Ontwikkelt embedded firmware volgens best practices
- Ontwikkelt en integreert libraries en frameworks
- Implementeert de juiste communicatie protocollen
- Schrijft en voert test plannen uit voor de firmware
- Optimaliseert de code voor embedded performance
- Implementeert goede code documentatie (bijvoorbeeld Doxygen)
- Voert code reviews uit en onderhoudt code kwaliteit

#v(1em)

=== Notulist

De Notulist zorgt ervoor dat alle projectinformatie correct wordt vastgelegd, georganiseerd en toegankelijk is voor het team. Deze rol is essentieel voor kennisbehoud en documentatie.

*Hoofd Verantwoordelijkheden:*
- Neemt notulen van team- en tutorvergarderingen op
- Onderhoudt technische documentatie en wiki's
- Creëert gebruikshandleidingen en manuals
- Bereidt presentatiematerialen voor
- Documenteert design beslissingen en rationale
- Organiseert project bestanden en versionering
- Zorgt voor consistentie in documentatie

#v(1em)
#pagebreak()

== Project Tijdlijn

#table(
  columns: 3,
  [*Fase*], [*Week Nr*], [*Deadlines*],
  [Empathize], [Week 1-2 (46-47)], [Teams maken en project kiezen],
  [Define], [Week 3-4 (48-49)], [Inleveren PvE en blokdiagram\ Presentaties],
  [Ideate], [Week 5-6 (50-51)], [Peer review\ Inleveren PvA],
  [Prototype, fase 1], [Week 7-8 (2-3)], [],
  [Test/Validatie], [Week 9 (6)], [Demo proof of concept\ Gedetailleerde blokdiagram\ Lijst met extra's],
  [Prototype, fase 2], [Week 10-11 (7-8)], [],
  [Test/Validatie], [Week 12-13 (10-11)], [],
  [Afronding en reflectie], [Week 14-15 (12-13)], [Documentatie inleveren],
  [Assessment], [Week 16 (14)], [Demonstratie],
)

#v(1em)
#pagebreak()

== Communicatie Plan

*Regelmatige Vergaderingen:*
- Wekelijkse Team Meetings: Maandag.
- Hardware Review Meetings: Dinsdag (indien nodig)
- Software Code Reviews: Dinsdag (indien nodig)
- AI Code Reviews: Dinsdag (indien nodig)
- Git Pull Request Reviews: Woensdag (indien nodig)
- Tutor Meetings: [DAG]

*Communicatie Kanalen:*
- Primair: [Discord]
- Secundair: [Whatsapp]
- Documentatie Repository: [Github / GitButler / OneDrive / Typst]
- Interne team management: [Notion / Toggl]
- Real-time Communicatie: [WhatsApp]

*Escalatie Proces:*
- Problemen eerste melden aan de verantwoordelijke teamlid
- Indien onopgelost, escaleren naar Project Leider
- Kritieke issues escaleren naar tutor of docent
- Reguliere status updates naar relevante stakeholders
- Indien nog niet opgelost wordt het teamlid verwijdert

#v(1em)
#pagebreak()

== Risico Management

Dit project kent verschillende risico's die proactief beheerd dienen te worden:

*Technische Risico's:*
- Hardware compatibiliteit problemen → Grondige component selectie en prototyping
- Software integratie issues → Vroege integratie testen
- Performance bottlenecks → Regelmatige profiling en optimalisatie

*Team Risico's:*
- Workload imbalans → Regelmatige check-ins en taakherziening
- Communicatie breakdown → Duidelijke communicatie plan naleven
- Onverwachte afwezigheid → Cross-training van team leden

*Planning Risico's:*
- Vertragingen in hardware beschaffing → Early ordering en buffertime
- Scope creep → Strik adheren aan requirements
- Onvoorziene technische uitdagingen → Reserve tijd in planning

#v(1em)
#pagebreak()


== Besluitvormingsproces 
Voor snelle en effectieve besluitvorming hanteren wij het volgende proces:

*Routinebeslissingen (Taak niveau):*
- Individuele teamleden nemen zelf beslissingen binnen hun verantwoordelijkheid
- Informeren andere teamleden via communicatiekanaal

*Meerdere Leden Betrokken (Taak niveau):*
- Discussieer op wekelijkse team meeting
- Project Leider en Hoofdverantwoordelijke (indien aanwezig) maakt finale beslissing

*Grote Impact Beslissingen (Project niveau):*
- Breng in bij tutor of docent
- Document de rationale achter de beslissing

#v(1em)
#pagebreak()

== Kwaliteitsstandaarden

*Code Standaarden:*
- Consistente code stijl aanhouden
- Doxygen documentatie voor alle publieke functies
- Minimaal 80% van de code moet testbaar zijn volgens een testplan
- Code reviews voor alle pull requests

#v(1em)
*Hardware Standaarden:*
- PCB design volgens industriële standaarden
- Alle componenten gedocumenteerd met datasheets
- Hardware testen volgens test plan
- Veiligheid checks (ESD, thermal, mechanical)

#v(1em)
*Documentatie Standaarden:*
- Alle belangrijke beslissingen zijn gedocumenteerd
- Design documenten up-to-date gehouden
- Design Thinking proces komt duidelijk terug in de documentatie
- Technische tekeningen en schema's volgens afgesproken stijl
- README en setup guides compleet

#v(1em)
= Inleving

#stakeholder_content

#v(1em)
#pagebreak()

== Pakket van Eisen
Typ inleiding :D Zie @verplichte-eisen voor ...

=== Verplichte eisen <verplichte-eisen>
+ Het systeem bevat minimaal één PSoC5 als microcontroller.
+ Het systeem zal gebruik maken van draadloze communicatie of een regeltechniek systeem.
+ Het systeem zal gebruik maken van een HDL-component geïmplementeerd in Verilog.
+ Het systeem zal gebruik maken van een RTOS.
+ Het systeem zal geoptimaliseerd worden voor energiezuinigheid (Zie @energie-eisen).
+ Het systeem zal minimaal één zelfgescrheven libary voor een communicatie protocol gebruiken.
+ Het systeem zal geschikt moeten zijn voor opendagen. (zie...)

#v(1em)
=== Functionele eisen

#v(1em)
==== Bewegingsfuncties
+ De robot zal minimaal 1 meter naar voren kunnen lopen, met een afwijking van ±1 meter.
+ De robot zal minimaal 1 meter naar achteren kunnen lopen, met een afwijking van ±1 meter.
+ De robot zal minimaal 1 meter zijwaards kunnen lopen, met een afwijking van ±1 meter.
+ De robot zal een extra DOF beschikken in de roll-as. ?
+ De robot zal 360 graden om zijn eigen as heen kunnen draaien.
+ De robot zal 5mm kunnen springen in de lucht.
+ De robothond zal een eigen persoonlijkheid hebben D.M.V. unieke bewegingen

#v(1em)
==== AI en autonomie
+ De robothond zal kunnen praten D.M.V. ingebouwde speakers.
+ De robothond zal geluid kunnen afspelen D.M.V. ingebouwde speakers.
+ De robothond bestuurt de ingebouwde speakers D.M.V. AI of ingebouwde bestuurbare muziekbord.
+ De robothond zal een eigen persoonlijkheid hebben D.M.V. unieke spraak.

#v(1em)
==== Modulaire functies
+ De robothond zal kunnen voorzien worden van extra externe modules
+ De robothond zal modulair ontworpen worden met aluminium extrusies
+ De robothond zal modulair ontworpen worden met pogo connectoren

#v(1em)
=== Energie en performance <energie-eisen>
+ De robothond zal een high-power modus beschikken die de motoren niet limiteert.
+ De robothond zal een low-power modus beschikken die de motoren limiteert tot 50% vermogen.
+ De robothond beschikt een sleep modus waarbij dit op de grond gaat liggen.
+ De robothond zal een AI-limitatie modus hebben die AI functies uitzet.

#v(1em)
=== Gebruikersgerichtheid en betrouwbaarheid
+ De robothond zal bestuurbaar zijn via een blauwetand x-doos of speelstation controller
+ De robothond zal een duidelijke fysieke gebruikers interface hebben met labels bij knoppen/schakelaars
+ De robothond zal geen onvoorspelbare beweging uitvoeren volgens een uitgebreid testplan.
+ De robothond zal bruikbaar zijn voor opendagen gebaseerd op de wensen van de stakeholder (Zie ...stakeholder).

#v(1em)
=== Documentatie
+ Er wordt een #acr("PvE") opgesteld met duidelijke MoSCoW-prioriteiten.
+ Er wordt een PvA opgesteld met daarin ten minste een planning, rolverdeling en risicoanalyse.  
+ Er zal een portfolio bijgehouden worden met gemaakte keuzes en ontwerpen
+ Alle ontwerpkeuzes worden onderbouwd met berekeningen, argumentatie
 en/of literatuurbronnen.  
+ Alle schema’s, code en andere ontwerpen worden als bijlage toegevoegd en zijn traceerbaar naar de gestelde eisen.  
+ De documentatie is gestructureerd volgens de fasen van Design Thinking Proces, tenzij expliciet anders afgesproken.

#v(1em)
=== Eisen aan het ontwikkelproces
+ Er zal gebruik gemaakt worden van het design thinking proces.
+ Er zal een testplan opgesteld worden voor elk deelproces.
+ Er zullen Blokdiagrammen aanwezig voor verschillende deelprocessen.
+ Er zal een proof of concept gemaakt met één of meer bijhorende prototype.
+ Er wordt een concrete #acr("MVP") gedefinieerd die meetbaar is via één of meerdere testplannen. 
+ Het team houdt één gezamenlijk logboek bij waarin activiteiten, uren en bijdragen per teamlid worden vastgelegd.  
+ Het team voert wekelijks een tutor-gesprek en legt afspraken en besluiten vast in notulen.  
+ Er wordt gebruikgemaakt van versiebeheer (bijvoorbeeld Git of vergelijkbaar).  
+ Er wordt een peer review uitgevoerd op een PvE en blokdiagram van de concurrentie.
+ Er worden rollen verdeeld in de groep en duidelijk gedefinieerd wat de verantwoordelijkheden zijn bij deze rol. (@team-structuur)

#pagebreak()

== Pakket van Wensen

+ De robothond heeft een systeem dat koffie kan inschenken via een pomp of actuator aan de achterkant van de robot.
+ De robothond kan autonoom door het schoolgebouw lopen.
+ De robothond heeft een soundboard en kan dus audio afspelen. 
+ De robothond kan dansen.


#v(1em)
== Minimal Viable Product
Voor het project moet er een #acr("MVP") gedefinieerd worden. Dit zijn de eigenschappen die minimaal aanwezig zouden moeten zijn voor een werkend, goed product.

- Het apparaat moet minimaal naar voren kunnen lopen door middel van de vier poten.
- Het apparaat naar voren lopen door middel van berekende inverse kinematics.
- Het apparaat moet minimaal bestuurbaar zijn via een controller.

#v(1em)
#pagebreak()

== MoSCoW-analyse

// Priority
#let p1 = table.cell(
  fill: rgb(150, 255, 150))[1] // priority 1, green
#let p2 = table.cell(
  fill: rgb(255, 255, 180))[2] // priority 2, yellow
#let p3 = table.cell(
  fill: rgb(255, 210, 150))[3] // priority 3, orange
#let p4 = table.cell(
  fill: rgb(255, 150, 150))[4] // priority 4, red

#table(
  columns: (auto, auto, auto),
  align: (left, left, center),
  inset: 6pt,
  stroke: 0.7pt + black,
  [*Prioritisering*], [*Uitleg MoSCoW methode*], [*Indicatie*],
  [Must have], [Vereist om te kunnen spreken van een werkbaar product.], p1,
  [Should have], [Hoge prioriteit, maar niet vereist voor een bruikbaar product.], p2,
  [Could have], [Optie die alleen wordt meegenomen als er tijd over is.], p3,
  [Won't have], [Geen prioriteit], p4,
)

#v(1em)

#table(
  columns: (auto, auto),
  align: (left, center),
  inset: 6pt,
  stroke: 0.7pt + black,

  [*Eis / Beschrijving*], [*Prioriteit*],

  [*2.2.1 Functionele eisen (algemeen)*], [],
  [Het systeem gebruikt een PSoC5 als primaire controller.], p1,
  [Het systeem bevat ten minste één vorm van draadloze communicatie of regelsysteem of beide.], p1,
  [Het systeem bevat ten minste één HDL-component.], p1,
  [Het systeem verwerkt input van minimaal één sensor of gebruiker.], p1,
  [Het systeem levert een demonstratiebare functionaliteit geschikt voor open dagen.], p1,

  [*2.2.2 Energie & performance*], [],
  [Het product bevat minimaal twee energiebesparingsmaatregelen.], p2,
  [Het systeem vertoont geen spontane resets of crashes.], p1,

  [*2.2.3 Gebruikersgerichtheid*], [],
  [Het product is begrijpelijk voor niet-technische gebruikers.], p1,
  [Knoppen zijn duidelijk gelabeld en toegankelijk.], p2,

  [*2.2.4 Betrouwbaarheid & robuustheid*], [],
  [Het systeem is bestand tegen normaal gebruik, zoals:\ lopen of gebruik van knoppen.], p1,
  [Datacommunicatie voldoet aan gespecificeerde protocol eisen.], p1,
  [Consistente performance tijdens demonstraties.], p2,

  [*2.2.5 Documentatie & traceerbaarheid*], [],
  [Alle ontwerpkeuzes zijn onderbouwd en traceerbaar.], p1,
  [Alle schema’s, code en ontwerpen worden als bijlage toegevoegd.], p1,
  [Documentatie volgt het Design Thinking-proces.], p1,
  [Volledige traceerbaarheid: eisen → ontwerp → testplan → testrapport.], p2,

  [*2.2.6 Eisen aan het ontwikkelproces*], [],
  [Er is een PvE met MoSCoW-prioriteiten.], p1,
  [Er is een MVP gedefinieerd.], p1,
  [Er is een PvA met planning, rollen en risicoanalyse.], p1,
  [Het team houdt één gezamenlijk logboek bij.], p1,
  [Er zijn wekelijkse tutor-gesprekken met notulen.], p1,
  [Gebruik van versiebeheer (Git).], p1,
  [Teamrollen zijn toegewezen volgens hoofdstuk 1.1–1.2.], p1,

  [*2.3 Wensen *], [],
  [Autonome navigatie door het schoolgebouw.], p3,
  [Soundboard / audio-afspeelfuncties.], p3,
  [Koffieschenksysteem.], p3,
  [Dansfunctionaliteit of animaties.], p3,
  [Uitgebreide AI-persoonlijkheid of gedrag.], p3,
  [Esthetische uitbreidingen zoals verlichting of covers.], p3,

  [*2.4 MVP Gebruikersgerichtheid*], [],
  [Lopen op vier poten met berekende inverse kinematics.], p1,
  [De hond moet minimaal naar voren kunnen lopen.], p1,
  [Bestuurbaar via een bluetooth controller.], p1,

  [*Won't-have*], [],
  [Geavanceerde LIDAR-systemen.], p4,
  [Smartphone-app of cloudkoppelingen.], p4,
)


#v(1em)
#set page(flipped: true)
#pagebreak()


== Opdeling van de Robothond
#let bent-edge(from, to, ..args) = {
  let midpoint = (from, 50%, to)
  let vertices = (
    from,
    (from, "|-", midpoint),
    (midpoint, "-|", to),
    to,
  )
  edge(..vertices, "-|>", ..args)
}

#diagram(
  node-stroke: luma(80%),
  edge-corner-radius: none,
  spacing: (10pt, 15pt),

  // Nodes
  node((6.5,0), [*Robothond*], name: <root>),
  
  node((1.5,1.5), [*Hardware*], name: <hardware-block>),
  node((9,1.5), [*Software*], name: <software-block>),
  
  node((0.5,3), [*Robotpoot*], name: <robotpoot-block>),
  node((2.8,3), [*Chassis*], name: <chasis-block>),

  node((0,4), [*Aandrijving*], name: <aandrijving-block>),
  node((1,4), [*Motoren*], name: <motoren-block>),
  
  node((6.25,3.25), [*Embedded\ firmware*], name: <embedded-firmware-block>),
  node((9,3.25), [*Regel\ systemen*], name: <regel-systemen-block>),
  node((11.5,3.25), [*AI\ functies*], name: <AI-block>),

  node((8.5,4.5), [*FOC*], name: <foc-block>),
  node((9.5,4.5), [*IMU*], name: <imu-block>),

  node((11,4.75), [*Omgevings\ herkening*], name: <omgeving-block>),
  node((12,4.75), [*Persoonlijkheid*], name: <persoonlijkheid-block>),

  node((5.5,4.50), [*RTOS*], name: <rtos-block>),
  node((6.9,4.5), [*Protocolen*], name: <protocolen-block>),

  node((6.25,5.75), [*Draadloos*], name: <draadloos-block>),
  node((7.5,5.75), [*Galvanisch*], name: <galvanisch-block>),

  // Edges
  
  bent-edge(<root>, <hardware-block>),
  bent-edge(<root>, <software-block>),

  bent-edge(<hardware-block>, <robotpoot-block>),
  bent-edge(<hardware-block>, <chasis-block>),

  bent-edge(<robotpoot-block>, <aandrijving-block>),
  bent-edge(<robotpoot-block>, <motoren-block>),

  bent-edge(<software-block>, <embedded-firmware-block>),
  //bent-edge(<software-block>, <regel-systemen-block>),
  edge(vertices: (<software-block>, <regel-systemen-block>), "-|>"),
  bent-edge(<software-block>, <AI-block>),

  bent-edge(<embedded-firmware-block>, <rtos-block>),
  bent-edge(<embedded-firmware-block>, <protocolen-block>),

  bent-edge(<protocolen-block>, <draadloos-block>),
  bent-edge(<protocolen-block>, <galvanisch-block>),

  bent-edge(<regel-systemen-block>, <foc-block>),
  bent-edge(<regel-systemen-block>, <imu-block>),

  bent-edge(<AI-block>, <omgeving-block>),
  bent-edge(<AI-block>, <persoonlijkheid-block>),
)
#set page(flipped: false)

= Definitie

== Feature lijst
=== Beweging-features
- De robothond kan zich bewegen in de Roll directie
- De robothond kan zich bewegen in de Pitch directie
- De robothond kan zich bewegen in de Yaw directie
- De robothond kan achteruit lopen
- De robothond kan zijwaarts lopen
- De robothond kan omzich heen draaien
- De robothond kan springen
- De robothond kan een trap oplopen of aflopen
- De robothond kan dansen
- De robothond bevat een IMU die de robot stabiliseert door middel van een PID controller

=== AI-features
- De robothond kan autonoom bewegen d.m.v. AI en omgevingsdetectie
- De robothond kan praten en / of geluid afspelen
- De robothond heeft zijn eigen persoonlijkheid

=== Extra modules
- De robothond kan voorzien worden van extra externe modules
- De robothond kan versnaperingen brengen
- De robothond kan zitten en een poot geven
- De robothond kan koffie deponeren aan de achterkant
- De robothond kan d.m.v. GPS locatie een pad lopen en posities opslaan


= Appendix


== Initiële kostenberekening

// ============================================
// USAGE EXAMPLE
// ============================================

#rc.add-roles(groupRoles)
#rc.add-assignments(peopleAssingments)

#rc.role-calculations(title: "Arbeids kosten", level: 3)

#pagebreak()
#set page(flipped: true)

=== Materialen kosten

#let TotalCost = Materials.fold(0, (sum, item) => sum + item.amount * item.unitprice)
#let MaterialList = Materials.map(i => (
    [#i.name],
    [#link(i.link)[#i.ordernr]],
    [#i.brand],
    [#i.supplier],
    [#i.amount],
    [#i.unit],
    [€#strfmt("{:.2}", float(i.unitprice))],
    [€#strfmt("{:.2}", float(i.unitprice * i.amount))],
  )).flatten()

#table(
  columns: (auto, auto, auto, auto, auto, auto, auto, auto),
  align: (left, left, left, left, left, left, right, right),
  inset: 6pt,
  [*Artikel beschrijving*], [*Bestelnummer*], [*Merk*], [*Leverancier*], [*Aantal*], [*Eenheid*], [*Einheidsprijs\ (ex. BTW)*], [*Totaal*],
  ..MaterialList,
  [], [], [], [], [], [], [], [€#strfmt("{:.2}", float(TotalCost))]
)
#set page(flipped: false)

#let subtotaal = 35.00 + 50.00 + 25.00 + 10.00 + 15.00
#let btw = subtotaal * 0.21
#let totaal = subtotaal + btw
#v(12pt)

=== Samenvatting kosten



#pagebreak()

#let ondertekenaars = students.map(s => s.name)

== Samenwerkingscontract

#let studentRows = students.map(s => (
  [#s.name],
  [#link("mailto:" + s.email)[#s.email]],
  [#s.tel],
)).flatten()

#table(
  columns: (auto, auto, auto),
  stroke: 0.7pt,
  inset: 6pt,
  align: (left, left, left),
  fill: (rgb("f7f7f7"), none, none),
  [*Naam studenten*], [*E-mail Adres*], [*Telefoonnummer*],
  ..studentRows
)

#v(8pt)

*Doel van de samenwerking*\
In een hecht en verantwoordelijk team samenwerken aan het ontwerp en de realisatie van een Robothond, waarbij we elkaar versterken en een professioneel eindproduct opleveren.

#v(6pt)

*Contractduur:*\ 
van #startdate tot #enddate.

#v(8pt)

*Afspraken*
+ Aanwezig zijn op afgesproken momenten 
+ Werk op tijd klaar
+ Reageren op berichten
+ Verdere afspraken en verantwoordelijkheden zijn benoemd in @rollen \ (tenzij er een goede reden is zoals ziekte, zijn er consequenties voor het teamlid)


*Consequenties*
- Eerste keer afspraak niet nagekomen:\ Gesprek met het groepje.
- Tweede keer afspraak niet nagekomen:\ Gesprek met tutor.
- Derde keer afspraak niet nagekomen:\ verwijdering uit het groepje.

#v(12pt)

#grid(
  columns: 2,
  gutter: 1fr,
  [*Plaats:* #city, #university],
  [*Datum:* #startdate],
)

#v(18pt)
#pagebreak()

*Getekend door*

#let ondertekenaars = students.map(s => ([#s.name]))

#let handtekening = (name) => block(
  spacing: 6pt,
  //rule(stroke: 0.7pt, width: 7cm),
  emph(name),
)

#grid(
  columns: 2,
  gutter: 2cm,
  ..ondertekenaars.map(n => handtekening(n))
)