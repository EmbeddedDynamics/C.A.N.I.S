// ============================================
// Peerreview of the PvE and block-diagram of
// the Paasei-automaat project
// ============================================

// Import Template and general data
#import "@local/academic-tools:0.1.10": *
#import "/Documenten/Config/GeneralConfig.typ": *

// Set parameters for the document
#set text(lang: "Nl")
#show link: underline


// ============================================
// Front Page en general definitions
// ============================================

#show: academic-frontpage.with(
  title: [Embedded Systems Project\ Peer Reviews],
  authors: students.map(s => s.name).join(", "),
  degree: [#degree],
  degree-goal: [Peer Review van het Pakket van Eisen en het blokdiagram die gemaakt zijn door PaasAI-Automaat],
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
  abstract: none,
  keywords: none,
  acknowledgments: none,
  acronyms: (:),
  versions: (
    version(
      committee: [R. van der Veen\ D. Smit \ Perijn Huisser],
      description: [Initiële commit],
      date: "2025-12-18",
      level: 3,
    ),
  )
)


// ============================================
// Document contents
// ============================================

= Pakket van Eisen

== Sterke Punten
Het Pakket van Eisen heeft een duidelijke structuur en is voorzien van een inhoudsopgave. Het voorblad ziet er netjes uit. Het PvE is goed georganiseerd met vier duidelijke categorieën zoals de Mechanische werking, Codering & AI-functionaliteit, Behuizing & Fysieke Omkasting en Algemene projecteisen. Dit maakt het makkelijk voor ons en de stakeholders om de specifieke eisen te vinden. De ID's van de eisen zijn erg handig zodat je weet welke eis bij welke categorie hoort. De indeling van de eisen in een blok samen met het ID en prioriteit ziet er professioneel uit. 


== Punten van Verbetering
+ Formuleren van eisen is niet consistent. Bijvoorbeeld bij mechanische eisen wordt gebruik gemaakt van de automaat, het transportmechanischme, mechanisme en het systeem. Dit is niet duidelijk en het zal beter zijn als er consistent gebruikt wordt gemaakt van één benaming.  

+ Vaak worden er meerdere eisen in één eis benoemd. Bijvoorbeeld bij eis MECH-001 zijn er twee eisen in één zin. De automaat beschikt over vier dispensers en elke dispenser moet een specifiek paasei uitwerpen. Dit is niet duidelijk voor de lezer.

+ De eis MECH-002 is onduidelijk en niet meetbaar geformuleerd. Wat is bijvoorbeeld makkelijk en wat kan ik zien in een transportmechanisme. 

+ Bij MECH-003 is er onduidelijkheid wat er onder de mechanisme valt. Verder bij deze eis is de tekst tussen de haakjes onduidelijk, wat is het doel en functie van de controller en wat houdt de terugkoppeling in?

+ Bij MECH-005 worden meerdere eisen benoemd zoals modulair opbouwen en eenvoudig vervangbaar. Verder is het onduidelijk wat er met hopper bedoeld wordt, waarom staat het tussen aanhalingstekens. Verder is MECH-005 niet goed meetbaar. Hoe wordt het modulair opgebouwd en wat is eenvoudig te vervangen?

+ Eis CODE-004 powermanagement past niet onder het kopje codering en AI-functionaliteit.

+ In eis FYS-002 staat dat de automaat stabiel is. Dit is niet meetbaar want wat is stabiel? Wees specifieker wat wordt gezien als stabiele functionaliteit.

+ Bij de eis FYS-003 is het woord interactief verkeerd gebruikt. Zoals wij het lezen wekt het de indruk op dat de leds als een indicatie worden gebruikt en niet als een interactief middel.

+ Eis FYS-005 is zeer onduidelijk. Betekent dit dat alle materialen wat de eieren aanraken eetbaar moeten zijn? Denk aan de hopper, lopende band etc. De eis kan beter geformuleerd worden door het alleen Food Safe te noemen en te benoemen dat deze aan europese normen voldoet. 
+ Eis ALG-001 is al eerder vernoemd in CODE-003 en is dus redundant. 

+ Eis ALG-003 is ongelukkig geformuleerd. Verschillende interpretaties zijn mogelijk voor wezenlijke onderdelen.

#pagebreak()

= Blokdiagram (hoog over)

== Sterke Punten
Het blokdiagram ziet er strak uit. Het is overzichtelijk en logisch opgebouwd. Er is zijn duidelijke scheidingen tussen verschillende componenten en welke rol deze speelt in het systeem. Tot slot is het schema goed uitbreidbaar door de modulaire opzet en het benoemen van optionele componenten.


== Punten van Verbetering

+ Het blokschema is niet consistent. De ene keer zijn de blokken erg specifiek en de andere keer algemeen. Zo zijn alle protocollen al vastgelegd maar zijn de Ei-Detectie sensoren weer heel algemeen.

+ In het blokschema wordt er gelijk al gedefinieerd welke protocollen gebruikt worden voor de communicatie van componenten, maar weten je nu al zeker dat je deze protocollen gaat gebruiken? Zo zou er bij de LED Status Indicatie misschien wel een LED strip toegepast moeten worden die niet aangestuurd kan worden door een binaire uitgang maar door een seriële uitgang.

+ Er is geen HDL/Verilog component in het blokschema, terwijl dit wel terugkomt in de eisen. 

+ Er is geen RTOS benoemd in het blokschema.

+ Er staat CSI in het blokschema naar de Jetson Nano maar in het blok van de camera zelf staat USB/CSI. Dus eigenlijk wordt er al bepaald dat er geen USB camera toegepast wordt.

