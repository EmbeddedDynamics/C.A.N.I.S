// ============================================
// Main document
// ============================================

// Import list
#import "Config/_Imports.typ": *

// Set parameters for the document
#set text(lang: "Nl")
#set par(justify: true)
#set text(costs: (hyphenation: 500%))
#show link: underline

#set math.equation(numbering: "(1)")

// ============================================
// Front Page en general definitions
// ============================================

#show: academic-frontpage.with(
  title: [Embedded Systems Project\ Robothond],
  authors: students.map(s => s.name).join(", "),
  degree: [#degree],
  degree-goal: [Het realiseren van een Robothond],
  department: [#department],
  university: [#university, #universitydesc],
  program-type: [#program],
  degree-year: [#year],
  location: [#city, #country],
  supervisor: [#supervisor],
  tutor: [#tutor],
  logo: "/Documenten/Assets/Logo/NHL_logo.jpg",
  project-logo: "/Documenten/Assets/Logo/ED_Logo.png",
  //month: [#month],
  //year: [#year],
  date: datetime.today(),
  abstract: [In dit project ontwikkelen wij als bachelorstudenten van NHL Stenden een embedded systeem in de vorm van een Robothond. Het apparaat is bedoeld als demonstrator op opendagen om de mogelijkheden van technische innovatie aan toekomstige studenten uit te leggen. We volgen hiervoor de Design-Thinking methodologie met nadruk op iteratieve prototyping en uitvoerige testen. Het doel is een volledig functioneel product te realiseren dat voldoet aan de stakeholder-eisen en markt-gereed is.],
  keywords: [],
  acknowledgments: [],
  acronyms: (acronyms_db),
  versions: (
    version(
      committee: [R. van der Veen\ D. Smit\ P. Huijser],
      description: [Initiële commit],
      date: "2025-11-10",
      level: 3,
    ),
    version(
      committee: [D. Smit\ P. Huijser],
      description: [PvE bijgewerkt op basis van de feedback van Rieno Moedt.],
      date: "2025-12-09",
      level: 3,
    ),
    version(
      committee: [R. van der Veen],
      description: [Bijgewerkte PvE nagelopen en spellingsfouten eruit gehaald.],
      date: "2025-12-09",
      level: 3,
    ),
    version(
      committee: [R. van der Veen\ D. Smit\ P. Huijser],
      description: [Er is een PvA gemaakt en een planning bijgevoegd.],
      date: "2025-12-19",
      level: 3,
    ),
    version(
      committee: [R. van der Veen\ D. Smit\ P. Huijser],
      description: [MoSCoW Herschreven op basis van het bijgewerkte PvE. Een aantal punten die buiten de scope vallen toegevoegd. Versies van het document ingevuld.],
      date: "2026-01-08",
      level: 3,
    ),
    version(
      committee: [R. van der Veen\ D. Smit\ P. Huijser],
      description: [MoSCoW Herschreven op basis van het bijgewerkte PvE. Een aantal punten die buiten de scope vallen toegevoegd. Versies van het document ingevuld.],
      date: "2026-03-30",
      level: 1,
    ),
  )
)


// ============================================
// Document contents
// ============================================

= Organisatie

Voor het project "Embedded Systems" moet er een embedded systeem gerealiseerd worden. Voordat wij gaan beginnen met het realiseren van een apparaat gaan wij extensief onderzoek uitvoeren. Als aller eerst is er een organisatie definitie vereist. Hierin gaan wij onze organisatie definiëren voor een goed structuur en efficiënte samenwerking.

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

De #acr("AI") Developer is verantwoordelijk voor de ontwikkeling van de #acr("AI")-gerelateerde features van de Robothond. De nadruk ligt op het maken van bewuste, onderbouwde keuzes over wat haalbaar en praktisch is gegeven de hardware beperkingen. Naast implementatie is het essentieel dat alle #acr("AI") features grondig worden getest voordat integratie in de embedded pipeline plaatsvindt.

*Hoofd Verantwoordelijkheden:*
- Evalueer #acr("AI") feature requirements op haalbaarheid en performance impact
- Ontwerp en implementeer #acr("AI") algoritmes en modellen
- Voer uitgebreide testen uit op #acr("AI") features (unit tests, integratie tests)
- Integreer #acr("AI") features in de embedded firmware pipeline
- Documenteer #acr("AI") beslissingen en gekozen algoritmes
- Optimaliseer #acr("AI") modellen voor embedded hardware (resource constraints)
- Werk samen met de Software Developer voor naadloze integratie

#pagebreak()


=== Hardware Engineer

De Hardware Engineer heeft als taak om alle hardware gerelateerde aspecten te behandelen, zoals: #acr("PCB") design, componenten selectie en circuit design. Naast het elektrotechnische aspect is het ook belangrijk voor de hardware engineer om ook de #acr("CAD") aspecten op te nemen, zoals: 3D tekenen, technische tekeningen maken, assemblage tekeningen maken indien nodig en het hardware te testen.

*Hoofd Verantwoordelijkheden:*
- Design circuit schema's en verificatie
- Selecteert de juiste componenten op basis van specificaties
- Maakt #acr("PCB") layouts, breadboard designs en schema's
- Test de functionaliteit van de hardware
- Documenteert hardware specificaties, progressie en onderzoeken
- Leidt hardware reviews en troubleshooting
- Zorgt voor versionering van #acr("CAD") bestanden

#v(1em)

=== Git Master

De Git Master is verantwoordelijk voor het beheren van de git repository. Hierbij wordt gelet op nette commits, feature branches, pull requests en duidelijke beschrijvingen die voldoen aan de gestelde git-richtlijnen. De Git Master hanteert een branching strategie die aansluit bij de projectstructuur en zorgt voor een schone, reproduceerbare codebase.

*Hoofd Verantwoordelijkheden:*
- Pull requests reviewen en goedkeuren voordat ze worden gemerged naar de main branch
- Verzekeren dat alle commits duidelijk gedocumenteerd zijn met beschrijvende berichten
- De repository beschermen tegen conflicting merges en ongestructureerde commits
- Een consistent branching strategy hanteren (bijvoorbeeld: Git Flow of trunk-based development)
- Merge conflicts oplossen en escaleren indien nodig
- Toegangsrechten en repository-instellingen beheren

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

#pagebreak()


== Project Tijdlijn

Dit is de tijdlijn dat gespecificeerd is vanuit het project zelf.
#table(
  columns: 3,
  [*Fase*], [*Week Nr*], [*Deadlines*],
  [Empathize], [Week 1-2 (46-47)], [Teams maken en project kiezen],
  [Define], [Week 3-4 (48-49)], [Inleveren #acr("PvE") en blokdiagram\ Presentaties],
  [Ideate], [Week 5-6 (50-51)], [Peer review\ Inleveren #acr("PvA")],
  [Prototype, fase 1], [Week 7-8 (2-3)], [],
  [Test/Validatie], [Week 9 (6)], [Demo #acr("PoC")\ Gedetailleerde blokdiagram\ Lijst met extra's],
  [Prototype, fase 2], [Week 10-11 (7-8)], [],
  [Test/Validatie], [Week 12-13 (10-11)], [],
  [Afronding en reflectie], [Week 14-15 (12-13)], [Documentatie inleveren],
  [Assessment], [Week 16 (14)], [Demonstratie],
)

#pagebreak()


#set page(flipped: false)

== Planning
#gantt(planning)

#pagebreak()


#set page(flipped: false)

== Communicatie Plan

*Regelmatige Vergaderingen:*
- Wekelijkse Team Meetings: Maandag.
- Hardware Review Meetings: Dinsdag (indien nodig)
- Software Code Reviews: Dinsdag (indien nodig)
- #acr("AI") Code Reviews: Dinsdag (indien nodig)
- Git Pull Request Reviews: Woensdag (indien nodig)
- Tutor Meetings: Woensdag (tenzij niet mogelijk)

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

#pagebreak()


== Kwaliteitsstandaarden

*Code Standaarden:*
- Consistente code stijl aanhouden
- Doxygen documentatie voor alle publieke functies
- Minimaal 80% van de code moet testbaar zijn volgens een testplan
- Code reviews voor alle pull requests

#v(1em)
*Hardware Standaarden:*
- #acr("PCB") design volgens industriële standaarden
- Alle componenten gedocumenteerd met datasheets
- Hardware testen volgens test plan
- Veiligheid checks (#acr("ESD"), thermische, mechanische)

#v(1em)
*Documentatie Standaarden:*
- Alle belangrijke beslissingen zijn gedocumenteerd
- Design documenten up-to-date gehouden
- Design Thinking proces komt duidelijk terug in de documentatie
- Technische tekeningen en schema's volgens afgesproken stijl
- README en setup guides compleet

#pagebreak()


= Inleving

#stakeholder_content

#pagebreak()

#persona_body()

#pagebreak()


= Definitie

== Pakket van Eisen
In het #acr("PvE") worden de eisen voor dit project vastgelegd. Deze eisen zijn grofweg te verdelen in twee groepen, eisen van de opdrachtgever en eisen die door het projectteam zelf zijn opgesteld. De verplichte eisen van de opdrachtgever zijn te vinden in @verplichte-eisen. Naast de verplichte eisen zijn de eisen aanvullend geformuleerd door het projectteam.
#v(1em)

// ============================================
// PvE
// Data from /Documenten/Config/PvE_MoSCoW.typ
// ============================================

#pve_range(pve_moscow, "mandatory", "mandatory", 3)
#pagebreak()

=== Functionele eisen
#pve_range(pve_moscow, "functional.1", "functional.3", 4)
#pagebreak()

#pve_range(pve_moscow, "energy", "documentation", 3)
#pagebreak()

#pve_range(pve_moscow, "development", "development", 3)
#pve_range(pve_moscow, "wishes", "wishes", 2)
#pagebreak()

#pve_range(pve_moscow, "wonthaves", "wonthaves", 2)
#v(1em)

== Minimal Viable Product
Voor het project moet er een #acr("MVP") gedefineerd worden. Dit zijn de eigenschappen die minimaal aanwezig zouden moeten zijn voor een werkend, goed product.

#pve_range(pve_moscow, "mvp.1", "mvp.4", 3)
#pagebreak()


== MoSCoW-analyse <moscow>

// ============================================
// MoSCoW-analysis
// Data from /Documenten/Config/PvE_MoSCoW.typ
// ============================================

// Legend
#table(
  columns: (auto, auto, auto),
  align: (left, left, center),
  inset: 6pt,
  stroke: 0.7pt + black,
  [*Prioritisering*], [*Uitleg MoSCoW methode*], [*Indicatie*],
  [Must have],   [Vereist om te kunnen spreken van een werkbaar product.], p1,
  [Should have], [Hoge prioriteit, maar niet vereist voor een bruikbaar product.], p2,
  [Could have],  [Optie die alleen wordt meegenomen als er tijd over is.], p3,
  [Won't have],  [Bewust niet binnen de scope van dit project.], p4,
)

#v(1em)

// MoSCoW
#moscow_range(pve_moscow, "mandatory", "functional.1")
#pagebreak()

#moscow_range(pve_moscow, "functional.2", "userusability")
#pagebreak()

#moscow_range(pve_moscow, "documentation", "development")
#pagebreak()

#moscow_range(pve_moscow, "wishes", "wonthaves")
#pagebreak()

#moscow_range(pve_moscow, "mvp.1", "mvp.4")

#acr("FOC")

#pagebreak()

#set page(flipped: true)


== Opdeling van de Robothond

#Blokdiagram

#pagebreak()


#set page(flipped: false)

= Ideeën genereren

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
- De robothond bevat een #acr("IMU") die de robot stabiliseert door middel van een #acr("PID") controller

=== AI-features
- De robothond kan autonoom bewegen d.m.v. #acr("AI") en omgevingsdetectie
- De robothond kan praten en / of geluid afspelen
- De robothond heeft zijn eigen persoonlijkheid

=== Extra modules
- De robothond kan voorzien worden van extra externe modules
- De robothond kan versnaperingen brengen
- De robothond kan zitten en een poot geven
- De robothond kan koffie deponeren aan de achterkant
- De robothond kan d.m.v. #acr("GPS") locatie een pad lopen en posities opslaan

#pagebreak()


== Plan van Aanpak

// Heading offset to embed PvA properly
#set heading(offset: 2)

// Import PvA without front page
#PvA_body()

// Heading offset to return heading usage to normal
#set heading(offset: 0)

#pagebreak()


== Literatuuronderzoeken



#pagebreak()


= Prototype

#prototype_body()

#pagebreak()


= Test

In de testfase zijn de kritieke subsystemen van de Robothond individueel getest op functionaliteit en mechanische robuustheid. Vanwege tijdsdruk zijn niet alle testcases uit de testplannen formeel gedocumenteerd. De uitgevoerde tests richten zich op de onderdelen die direct bepalend zijn voor de demo-gereedheid: de capstan-aandrijving en de robotpoot.

== Capstan-aandrijving

De capstan-aandrijving is getest op slijtage en mechanische betrouwbaarheid. De aandrijving heeft tien uur continu gedraaid onder nominale belasting. Na afloop is de trommel, de DM-20 kabel en de bevestiging visueel geïnspecteerd.

#figure(
  table(
    columns: (auto, auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    align: (left, left, left),
    [*Test*], [*Bevinding*], [*Resultaat*],
    [Slijtage-inspectie na 10 uur],   [Geen zichtbare slijtage op trommel, kabel of bevestiging],  [*Geslaagd*],
    [Krachtoverbrenging zonder slip],  [Geen slip geconstateerd bij nominale belasting],              [*Geslaagd*],
    [Geluid en vibratie],             [Aandrijving werkt geruisloos; geen storende vibraties],       [*Geslaagd*],
  ),
  caption: [Testresultaten capstan-aandrijving.]
)

== Robotpoot

De robotpoot is getest op bewegingsbereik, positioneringsnauwkeurigheid en mechanische integriteit. Tijdens de eerste testrun werd een probleem geconstateerd waarbij de kabel bij een hoek van circa 80° mechanisch aanliep, waardoor het volledige bewegingsbereik niet gehaald werd (TC-201). Dit is opgelost door de kabelspanning opnieuw in te stellen. Na deze correctie functioneert de poot correct over het volledige bereik.

#figure(
  table(
    columns: (auto, auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    align: (left, left, left),
    [*Test*], [*Bevinding*], [*Resultaat*],
    [Bewegingsbereik joints (TC-201)],  [Na hertensionering: volledig bereik gehaald zonder aanlopen],  [*Geslaagd*],
    [Positioneringsnauwkeurigheid],     [Poot bereikt doelposities nauwkeurig; weinig interne wrijving],  [*Geslaagd*],
    [Slijtage-inspectie],               [Geen zichtbare slijtage na meerdere uren cyclisch bewegen],      [*Geslaagd*],
    [E-Stop functionaliteit (TC-801)],  [Actuatie stopt direct; systeem gaat naar veilige toestand],     [*Geslaagd*],
  ),
  caption: [Testresultaten robotpoot.]
)

== Firmware en communicatie

Naast de mechanische tests zijn de firmware-componenten afzonderlijk gevalideerd. FreeRTOS draait stabiel op beide PSoC5 microcontrollers. De ODrive C-driver is end-to-end getest met de fysieke motoren in position control modus en communiceert correct via de CanStack-bibliotheek over de CAN-bus. De Jetson AI-pipeline met spraakherkenning en diepteperceptie is operationeel getest op de hardware.

#pagebreak()


= Conclusie

Het project Embedded Systems 2025–2026 had als doel het realiseren van een functionele robothond als demonstratieplatform voor open dagen van NHL Stenden. Het team heeft gedurende het traject een volledig embedded systeem ontwikkeld dat bestaat uit mechanica, elektronica, firmware en AI-perceptie.

Op het moment van oplevering zijn alle kernsubsystemen individueel getest en functioneel bevonden. De capstan-aandrijving en het robotpootmechanisme werken naar behoren; de firmware draait stabiel op beide PSoC5-controllers; de ODrive motorcontrollers communiceren correct over de CAN-bus; en de Jetson verwerkt spraakcommando's en dieptedata. De demonstratie is gepland op 2 april 2026 en het systeem is hier klaar voor.

Het project is daarmee grotendeels geslaagd. Niet alles wat oorspronkelijk beoogd was is volledig afgerond, de volledige integratie van alle twaalf motoren tegelijk en de aansturing van de schoudermodules zijn nog niet volledig getest als geheel. Maar de technische basis is solide en de realisatie ligt ver boven wat bij aanvang realistisch leek voor een project van deze omvang.

== Aanbevelingen

Op basis van de ervaringen tijdens dit project worden de volgende aanbevelingen meegegeven voor verdere ontwikkeling:

- *Beter vastleggen van testresultaten*: Tests zijn uitgevoerd maar niet altijd formeel gedocumenteerd. In een volgend project verdient het de voorkeur om testresultaten direct vast te leggen, zodat bevindingen en aanpassingen traceerbaar blijven.

- *Scopebewaking*: Het project heeft veel tijd gevraagd. De technische ambities waren hoog en de complexiteit van de integratie werd onderschat. Een striktere scopedefinitie in een vroeg stadium, met heldere prioriteiten per fase, had tijdsverlies kunnen voorkomen.

- *Integratie eerder starten*: De subsystemen zijn afzonderlijk goed getest, maar de volledige systeemintegratie (alle motoren, schoudermodules, Jetson-PSoC communicatie) is laat in het project gestart. Eerder integreren geeft meer ruimte voor het oplossen van onverwachte problemen.

- *Diepteperceptie verder verbeteren*: De stereovisiepipeline werkt, maar de nauwkeurigheid van de dieptemeting kan verder worden verbeterd, met name voor texturarme vloeroppervlakken. Een finetuned Nederlandstalig Whisper-model kan bovendien de spraakherkenningsnauwkeurigheid verhogen.

#pagebreak()


= Appendix

== Initiële kostenberekening

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

#pagebreak()


#set page(flipped: false)

#let subtotaal = 35.00 + 50.00 + 25.00 + 10.00 + 15.00
#let btw = subtotaal * 0.21
#let totaal = subtotaal + btw
#v(12pt)

=== Samenvatting kosten

#pagebreak()

// Inserting the complete contract
#Samenwerkingscontract
