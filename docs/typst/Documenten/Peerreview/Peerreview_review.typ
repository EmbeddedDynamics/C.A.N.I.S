// ============================================
// Reaction to the peerreview done on our 
// documentation by Paasei-automaat
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
  degree-goal: [Review van de Peer Review vanuit PaasAI],
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
  abstract: none,
  keywords: none,
  acknowledgments: none,
  acronyms: (:),
    versions: (
    version(
      committee: [R. van der Veen],
      description: [Initiële commit],
      date: "2025-12-19",
      level: 3,
    ),
  )
)


// ============================================
// Document contents
// ============================================

= Pakket van Eisen

+ *Een aantal eisen zijn onvoldoende SMART geformuleerd, Dit maakt het lastig om in het testplan specifiek naar één eis te verwijzen zonder verwarring.* \ \ Welke eisen vallen hieronder? Wij hebben zoveel mogelijk van onze eisen SMART proberen op te stellen.

+ *In eis 2.3.2.1.7 en 2.3.2.2.4 wordt gesproken over een "eigen persoonlijkheid" door unieke bewegingen en spraak. Dit is niet objectief meetbaar. Het is beter om te definiëren hoeveel unieke animaties of spraakfragmenten er minimaal aanwezig moeten zijn.* \ \ Hoe zou het wel meetbaar opgesteld kunnen worden? Een persoonlijkheid is ook niet objectief en daarom hebben wij opgesteld dat we het zouden kunnen meten door middel van spraak en beweging. Het is ook niet realistisch om nu al bij de eisen op te stellen welke specifieke "spraakfragmenten" er afgespeeld zouden moeten worden.

+ *Energie en performance eisen missen kwantitatieve onderbouwing, zoals stroomverbruik of accuduur. Onderbouwing van het ontwerp aanzienlijk worden verhoogd.* \ \ Dit is in de huidige fase van het project niet realistisch, omdat er nog niet precies duidelijk is wat het verbruik zou zijn tijdens normale functionaliteit. En wat wordt er nou precies bedoeld met "Onderbouwing van het ontwerp aanzienlijk worden verhoogd"? Dit is heel onduidelijk.

+ *Hoewel eis 2.3.5.1 stelt dat er een PvE met MOSCOW-prioriteiten wordt opgesteld, zijn deze prioriteiten (Must, Should, Could, Won't) niet direct 9014zichtbaar in de getoonde lijsten van hoofdstuk 2.3.* \ \ De MoSCoW is niet een onderdeel van ons PvE. Dit valt onder een ander hoofdstuk. Daarom zijn de MoSCoW niet getoond in hoofdstuk 2.3. En wat bedoelen jullie met "9014zichtbaar"?

+ *Sommige functionele eisen zijn heel ambitieus binnen de tijdsduur van het project. Een prioritering via MoSCoW kan de haalbaarheid vergroten.* \ \ Dit verbeterpunt is hetzelfde als de vorige.

+ *Bij eis 2.3.2.1.6 wordt een sprongkracht van 5 millimeter genoemd. Klopt het dat het zo’n kleine afstand is of is het een typefout?* \ \ Ja dit is bewust.

+ *in eis 2.3.1.2 worden "draadloze communicatie" en "regeltechniek systeem" in één eis genoemd als een 'of'-keuze. Dit zijn twee verschillende Eisen.* \ \ Dit zijn niet twee verschillende eisen. In de projectwijzer is duidelijk aangegeven dat er tussen deze twee gekozen mag worden, en is dus een enkele eis.

+ *In eis 2.3.4.1 wordt de term "blauwetand x-doos of speelstation controller" gebruikt. Dit komt onprofessioneel over* \ \ Hier zijn wij het ook mee eens. Dit was als een grap naar Rieno Moedt bedoelt.

#pagebreak()

= Blokdiagram

+ *De onderlinge relaties en datastromen tussen de blokken zijn niet weergegeven. Hierdoor is het onduidelijk hoe informatie en aansturingssignalen door het systeem lopen.* \ \ Dit is ook niet vereist bij een hoogover blokdiagram. Als we nu al keuzes gaan maken voor de communicatie tussen blokken, dan limiteren we ons zelf en stellen we het al vast. 

+ *Het diagram is niet consistent in hoe specifiek de blokken zijn. "IK" (Inverse Kinematics) en "FOC" (Field Oriented Control) zijn zeer specifieke technieken, terwijl "Motoren" en "Chassis" erg algemeen blijven.* \ \ Inverse Kinematics is op zichzelf een algemene term en dus niet zeer specifiek. Ik kan begrijpen dat "Motoren" en "Chassis" vrij algemeen lijken vergeleken met een paar andere blokken, maar dit is om de keuze open te houden en ons zelf in te dekken.

+ *Er is geen vermelding van power management of batterijsystemen in het diagram, terwijl energiezuinigheid en voedings mogelijkheden een belangrijk onderdeel van de eisen zijn.* \ \ Dit had er inderdaad ingekunt. Dit zullen wij er nog aan toevoegen.

+ *Het AI-blok is functioneel breed gedefinieerd. Een nadere opsplitsing in bijvoorbeeld perceptie, besluitvorming en gedragsaansturing zou de architectuur verduidelijken.* \ \ Nogmaals het blokdiagram is hoogover en niet specifiek. 

+ *Er is geen blok voor de PSoC5 microcontroller of de HDL/Verilog-component.* \ \ Een PSoC5 controller hadden we nog wel toe kunnen passen. Een HDL/Verilog-component is nog niet relevant om specifiek te benoemen in het blokdiagram.