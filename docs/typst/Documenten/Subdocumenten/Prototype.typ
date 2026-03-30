// ============================================
// Prototype phase content
// ============================================

// Import general data
#import "../Config/GeneralConfig.typ": *

// Set parameters for the document
#set text(lang: "Nl")
#show link: underline


// ============================================
// Document contents
// ============================================
#let prototype_body() = [
== Van concept naar prototype

In de Prototype-fase van het Design Thinking proces brengen we de inzichten uit de Define- en Ideate-fase tot leven. Het doel is niet een afgewerkt product, maar een werkend systeem dat de kernfunctionaliteit aantoont: een vierpotige robot die beweegt, zijn omgeving waarneemt en reageert op de mensen om hem heen.

Dit hoofdstuk beschrijft hoe de Robothond is gebouwd - welke keuzes zijn gemaakt, waarom, en hoe de afzonderlijke onderdelen samenkomen tot één systeem.

#pagebreak()

== Systeemarchitectuur

Het eerste ontwerpprobleem was architecturaal: hoe verdeel je de rekenwerk over een embedded systeem dat tegelijk invoer moet verwerken, kinematica moet berekenen en twaalf motoren moet aansturen?

Wij hebben ervoor gekozen om de *taakverdeling over twee Cypress #acr("PSoC5") microcontrollers*. Door de verantwoordelijkheden te splitsen, kunnen beide subsystemen onafhankelijk worden ontwikkeld en getest een belangrijk voordeel in een project met meerdere teamleden.

#figure(
  table(
    columns: (auto, 1fr),
    stroke: 0.7pt,
    inset: 8pt,
    align: (left, left),
    [*Controller*], [*Rol in het systeem*],
    [PSoC5 \#1 - Command Controller],
    [Het "brein" voor externe communicatie. Ontvangt Bluetooth-opdrachten via de ESP32-S3, RC-signalen van de ExpressLRS ontvanger en dieptedata van de Jetson via USB-serieel. Herbergt de MPU-6050 #acr("IMU") voor oriëntatiemeting. Vertaalt alle invoer naar bewegingsopdrachten en stuurt die door naar de motion controller. Draait FreeRTOS.],
    [PSoC5 \#2 - Motion Controller],
    [Het "ruggenmerg" voor beweging. Voert de inverse kinematica solver uit voor de diamant-linkage geometrie, hardwareversneld door een custom CORDIC-eenheid in de programmeerbare logica van de PSoC5. Berekent gewrichtshoeken voor alle benen en stuurt de twaalf motoren aan via de ODrive v3 motorcontrollers over de #acr("CAN")-bus. Draait FreeRTOS.],
  ),
  caption: [Taakverdeling tussen de twee PSoC5LP microcontrollers.]
)

De twee controllers zijn direct aan elkaar gekoppeld via een seriële link (#acr("UART") of #acr("I2C")). De communicatie richting de ODrive motorcontrollers verloopt via een #acr("CAN") 2.0 bus op 1000 kbps met 11-bit identifiers - een robuust veldbusprotocol dat goed bestand is tegen elektromagnetische storingen van de BLDC-motoren.

#pagebreak()

== Mechanisch ontwerp


=== De capstan-aandrijving als kern van elke module

De grootste mechanische uitdaging binnen dit ontwerp betreft het ontwikkelen en realiseren van een geschikt transmissiesysteem voor de robotpoot. In een vroeg stadium zijn verschillende aandrijfprincipes onderzocht en geëvalueerd op basis van criteria zoals efficiëntie, speling, complexiteit en maakbaarheid (Zie transmissie onderzoek).

Op basis van deze analyse is uiteindelijk gekozen voor een *capstan-aandrijving*. Dit type aandrijving biedt meerdere voordelen: het is efficiënt, geluidsarm, relatief eenvoudig te implementeren en vrijwel vrij van speling, afhankelijk van het gebruikte transmissiemedium.

Voor het transmissiemedium is gekozen voor een synthetische vezel, namelijk *DM20* (een HMPE/UHMWPE-vezel). Dit materiaal wordt veelvuldig toegepast in de maritieme en heavy-lifting industrie vanwege de hoge sterkte, vrijwel nul rek en gunstige vermoeiingseigenschappen. De extreem lage rek draagt direct bij aan een spelingvrije krachtoverdracht, wat essentieel is voor nauwkeurige positionering van de robotpoot.

Binnen het capstan-systeem is gekozen voor drie wrijvingswikkelingen rondom de aandrijftrommel en twee verplaatsingswikkelingen. De maximale overdraagbare kracht wordt bepaald met behulp van de kapstanformule, die de verhouding beschrijft tussen de trekkrachten aan beide zijden van het touw bij het begin van slip:

$ T_("hoog") / T_("laag") = e^(mu theta) $ <eq:kapstan>

Voor drie wrijvingswikkelingen geldt:

$ theta = 6 pi $

Hieruit volgt dat de maximale krachtsoverdracht exponentieel afhankelijk is van de wrijvingscoëfficiënt $mu$.

Aangezien in dit ontwerp gebruik wordt gemaakt van *PC-ABS* voor de capstan-trommels (vanwege de hoge taaiheid en slagvastheid), is de exacte wrijvingscoëfficiënt tussen PC-ABS en DM20 niet direct beschikbaar in literatuur. Wel zijn afzonderlijke waarden bekend:

- DM20 (HMPE/UHMWPE): $ mu approx 0,05 - 0,07 $
- PC-ABS: $ mu approx 0,16 - 0,37 $

Op basis hiervan wordt een realistische schatting gemaakt voor de effectieve wrijvingscoëfficiënt:

$ mu approx 0,10 - 0,20 $

Met behulp van deze bandbreedte is de kapstanverhouding berekend, zoals weergegeven in @fig:kapstan-bode. Hieruit blijkt dat de krachtverhouding sterk exponentieel toeneemt met de wrijvingscoëfficiënt. Relatief kleine variaties in $mu$ leiden dus tot grote verschillen in maximale krachtsoverdracht.

Dit onderstreept het belang van:
- een consistente oppervlakteafwerking van de trommel,
- een juiste voorspanning van de vezel,
- en een zorgvuldige materiaalkeuze,

om voorspelbaar en reproduceerbaar gedrag van de aandrijving te waarborgen.

#let kapstan-formule(theta, mu) = calc.exp(mu * theta)

#let mu-theta-range = (
  (0.05, 6 * calc.pi),
  (0.10, 6 * calc.pi),
  (0.15, 6 * calc.pi),
  (0.20, 6 * calc.pi),
  (0.25, 6 * calc.pi),
  (0.30, 6 * calc.pi),
  (0.35, 6 * calc.pi),
  (0.40, 6 * calc.pi),
  (0.45, 6 * calc.pi),
  (0.50, 6 * calc.pi)
)

#let force-data = mu-theta-range.map(row => {
  let (mu, theta) = row
  (mu, kapstan-formule(theta, mu))
})

#let mus = force-data.map(p => p.at(0))
#let forces = force-data.map(p => p.at(1))

#figure(
  lq.diagram(
    width: 12cm, 
    height: 8cm,
    title: [Kapstanverhouding $ frac(T_("hoog"), T_("laag"), style: "horizontal")$],
    
    xlim: (0.05, 0.4),
    ylim: (0, 500),
    
    xlabel: [Wrijvingscoëfficiënt $mu$], 
    ylabel: [Krachtverhouding [-]],
    
    grid: (stroke: black, stroke-sub: 0.25pt),
    lq.plot(mus, forces, stroke: 2pt, mark-size: 8pt),
  ),
  caption: [Kapstanverhouding als functie van de wrijvingscoëfficiënt bij $theta = 6 pi$],
) <fig:kapstan-bode>

Voor het uiteindelijke ontwerp is gekozen voor een overdrachtsverhouding van *1:7* voor de pootmodule en *1:5* voor de schoudermodule. Deze verhoudingen bieden een balans tussen koppelvergroting, snelheid en regelbaarheid.

#figure(
  table(
    columns: (auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Parameter*], [*Waarde*],
    [Reductieverhouding (poot)], [7:1],
    [Reductieverhouding (schouder)], [5:1],
    [Kabeltype], [DM20 4 mm],
    [Materiaal kleine drum], [PC-ABS],
    [Materiaal grote drum / structuur], [PLA],
  ),
  caption: [Specificaties van de capstan-aandrijving.]
)

=== De diamant-linkage

Voor de kinematica is gekozen voor een *4-bar diamond linkage*-model. Dit mechanisme vertoont sterke overeenkomsten met een *5-bar linkage*, met als belangrijk verschil dat beide aandrijvingen een gemeenschappelijke rotatie-as delen. Hierdoor ontstaat een compacter, mechanisch eenvoudiger en stijver systeem, dat bovendien relatief eenvoudig wiskundig te modelleren is.

#figure(
  image("../Assets/IK/4bar_diamond_linkage.png"),
  caption: [4-bar diamond linkage mechanisme],
  kind: image
) <img:4bar_diamond_linkage>

Het mechanisme bestaat uit vier starre armen: twee driverarmen met een lengte van *#zi.mm(85)* en twee schakelarmen met een lengte van *#zi.mm(120)*. Deze configuratie vormt een gesloten vierstangenmechanisme met een diamantvormige geometrie.

Op basis van deze afmetingen is een MATLAB-model ontwikkeld, waarmee het mechanisme gesimuleerd kan worden. Dit model biedt inzicht in het werkbereik, de haalbare posities en de gevoeligheid van het systeem voor verschillende configuraties.

#figure(
  image("../Assets/IK/4bar_matlab.png"),
  caption: [Simulatiemodel van de diamant-linkage in MATLAB],
  kind: image
) <img:4bar_diamond_linkage_matlab>

De kracht van dit parallelle mechanisme ligt in de aansturing: door de relatieve hoek tussen de twee driverarmen (elk aangedreven door een afzonderlijke motor) te variëren, kan het eindpunt van de poot naar elke positie binnen het werkbereik worden gestuurd. Dit maakt zowel positionering als trajectcontrole mogelijk.

De inverse kinematica wordt in real-time berekend op de *PSoC 5LP (\#2)*. Deze solver bepaalt continu welke motorhoeken corresponderen met een gewenste pootpositie, waardoor nauwkeurige en vloeiende bewegingen gerealiseerd kunnen worden.

Om de rekentijd van de inverse kinematica te minimaliseren, wordt gebruikgemaakt van een 16-bit hardware *#acr("CORDIC")*-module. Deze hardwareversneller maakt het mogelijk om goniometrische functies efficiënt te berekenen in lineaire, circulaire en hyperbolische coördinatenstelsels. Hierdoor wordt de rekentijd significant gereduceerd en blijft real-time besturing haalbaar.

Bij het ontwerp dient rekening gehouden te worden met mogelijke singulariteiten en niet-lineair gedrag aan de randen van het werkbereik. In deze gebieden kunnen kleine veranderingen in motorhoek leiden tot grote veranderingen in eindpositie of krachtverdeling. Het MATLAB-model is gebruikt om deze gebieden te identificeren en te vermijden in de uiteindelijke bewegingsplanning.

#figure(
  table(
    columns: (auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Onderdeel*], [*Maat*],
    [Driverarm (aan grote drum)], [#zi.mm(85)],
    [Schakelarm (limb)], [#zi.mm(120)],
  ),
  caption: [Geometrische parameters van de diamant-linkage.]
)

=== Schoudermodules
Naast de pootmodules is tevens een schoudermodule ontworpen, waarmee een extra #acr("DOF") rond de x-as wordt toegevoegd. Deze uitbreiding vergroot de bewegingsvrijheid van de robothond, met name in laterale richting, en draagt bij aan een meer natuurlijke en stabiele bewegingsmechanisme.

De implementatie van de schoudermodule introduceert echter aanvullende ontwerpeisen, met name op het gebied van compactheid en integratie. Aangezien twee capstan-aandrijvingen naast elkaar in de schouder worden geplaatst, is het noodzakelijk om de benodigde koppeloverbrengingsverhouding te reduceren om de fysieke afmetingen van het systeem te beperken.

Om deze reden is gekozen voor een reductieverhouding van 1:5, waarbij nog steeds gebruik wordt gemaakt van drie wrijvingswindingen. Deze configuratie biedt een balans tussen voldoende koppeloverdracht en een compacte geometrie. Tevens blijft het mechanisch ontwerp hierdoor geschikt voor productie op de 3D printer, zonder dat complexe ondersteuningsstructuren of nabewerkingen noodzakelijk zijn.

De grotere capstan-trommels worden direct bevestigd aan de pootmodule, waardoor de effectieve krachtoverdrachtslengte wordt geminimaliseerd. Dit reduceert ongewenste flexibiliteit en verhoogt de structurele stijfheid van het systeem, wat gunstig is voor zowel de nauwkeurigheid als de dynamische prestaties.

#figure(
  table(
    columns: (auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Module type*], [*Aantal motoren*],
    [Pootmodule (4×)], [2 motoren per module],
    [Schoudermodule (2×)], [2 motoren per module],
    [*Totaal*], [*12 motoren*],
  ),
  caption: [Overzicht van modules en motorconfiguratie.]
)

== Aandrijving

=== Keuze voor BLDC en ODrive
Op basis van de systeemeisen is gekozen voor het toepassen van #acr("BLDC")-motoren als primaire actuatoren. Deze motortypen bieden een hoge vermogensdichtheid, goede efficiëntie en zijn geschikt voor dynamische toepassingen zoals een robothond.

Voor de aansturing van de motoren is een regelstrategie vereist die zowel een hoge positionele nauwkeurigheid als een soepele koppelafgifte mogelijk maakt. Om deze reden is gekozen voor het #acr("FOC")-algoritme. Deze regelmethode maakt het mogelijk om de motorstromen vectorieel te regelen, waardoor het geleverde koppel nauwkeurig en continu gecontroleerd kan worden.

De implementatie van het #acr("FOC")-algoritme wordt verzorgd door een ODrive v3.6 motorcontroller, die de #acr("BLDC")-motoren aanstuurt via #acr("SVPWM"). Deze combinatie resulteert in:

- een significante reductie van koppelrimpel  
- een geluidsarmere werking  
- een hogere energie-efficiëntie  
- een verbeterde regelbaarheid bij lage snelheden  

In totaal worden twaalf motoren toegepast van het type *6210 BLDC 200 kV*. Dit zijn lichtgewicht buitenlopers met een gunstige vermogensdichtheid, die via de capstan-aandrijving gekoppeld zijn aan het mechanische systeem. De motoras en de aandrijfas van de poot liggen bewust niet collineair; de capstan-transmissie compenseert deze geometrische offset en maakt een flexibele mechanische integratie mogelijk.

Voor de motorbesturing zijn *ODrive v3.6* controllers geselecteerd. Deze open-source high-performance motorcontrollers ondersteunen nauwkeurige positie-, snelheid- en koppelregeling, wat essentieel is voor de kinematica en stabiliteit van de robothond. Elke ODrive bestuurt één as, wat een modulaire en schaalbare systeemarchitectuur mogelijk maakt.

De ODrives zijn voorzien van aangepaste firmware en worden geconfigureerd via een gedeeld JSON-configuratiebestand. Na het laden van deze configuratie hoeven per unit uitsluitend de #acr("CAN")-adressen te worden aangepast, wat de implementatie en schaalbaarheid vereenvoudigt.

De volledige communicatieketen van de PSoC5 #2 via de CanStack en de ODrive C-driver naar de motorcontrollers is end-to-end gevalideerd met fysieke hardware. Hiermee is aangetoond dat het systeem betrouwbaar functioneert binnen de beoogde architectuur.

== Elektronica

In plaats van losse draden en modules te combineren zijn er *drie custom PCB's* ontworpen en gefabriceerd. Elke print heeft een specifieke rol en past in de algehele systeemarchitectuur.

=== PCB 1 - Hoofd PSoC PCB (Command Controller)

De hoofdprint is het communicatiecentrum van de robot. Alle externe invoerkanalen komen hier samen: de ESP32-S3 Bluetooth-module, de MPU-6050 #acr("IMU") voor oriëntatiemeting en de connectors voor de RC-ontvanger en de Jetson. Een LDO-regelaar zet de 20V batterijspanning om naar de 5V die de PSoC-systemen nodig hebben; een LM2596 buck converter levert 12V naar de Jetson via een XT60-connector. JST-connectoren verzorgen de communicatie met PCB 2.

=== PCB 2 - Motor PSoC PCB (Motion Controller)

De motorprint draait de motion controller en vormt de schakel naar de twaalf ODrive motorcontrollers. Zes D-SUB connectoren verzorgen de CAN in- en uitgang van elk poot- en schoudermodule de CAN-bus is als daisy-chain over de print doorgelust. Per motormodule zijn twee GPIO-uitgangen beschikbaar voor het homen van de motoren. De externe CAN-transceivers (buiten de PSoC5) zijn eveneens op deze print gemonteerd. De verbindingskabels tussen de print en de modules zijn zelfgemaakt van CAT6-kabel (twisted pairs) met D-SUB connectoren - een bewuste keuze voor storingsweerstand in de buurt van de krachtige BLDC-motoren.

=== PCB 3 - Power Distribution Board

De voedingsprintplaat is functioneel eenvoudig maar kritisch: veertien XT60-connectoren verdelen de 20V Parkside 4Ah batterijspanning naar alle verbruikers.  Twaalf connectors voor de ODrives, één voor de PSoC-systemen en één voor de batterij-ingang. Door de voedingsverdeling op een aparte print te plaatsen, blijven de hoge stromen van de motorcontrollers gescheiden van de signaalelectronica.

#figure(
  table(
    columns: (1fr, 1fr, 3fr),
    stroke: 0.7pt,
    inset: 6pt,
    align: (left),
    [*Spanning*], [*Bron*], [*Gebruik*],
    [20V], [Parkside Accu\ 4Ah 20V], [ODrive motorcontrollers\ ingang Power Distribution Board],
    [12V],[LM2596], [NVIDIA Jetson Orin Nano Super],
    [5V (PCB)], [LDO], [PSoC5 systemen, ESP32-S3, IMU],
    [5V (Audio)], [Buck converter\ DC-DC 5V 5A], [Audio versterker],
  ),
  caption: [Spanningsoverzicht van het systeem.]
)


== Firmware

=== FreeRTOS als fundament

Alleen #acr("PSoC5") \#1 draait op FreeRTOS. De keuze voor een realtime besturingssysteem is ingegeven door de gelijktijdige eisen aan het systeem: sensoruitlezing, communicatieverwerking en bewegingsbesturing moeten elk op hun eigen tempo en met gegarandeerde latentie kunnen draaien. FreeRTOS biedt de taakscheduler, mutexen en queues die dit mogelijk maken zonder dat taken elkaar blokkeren.

=== Inverse kinematica versneld met CORDIC

De IKMath-bibliotheek op PSoC5 \#2 berekent voor elk tijdstap de gewrichtshoeken die horen bij een gewenste pootpositie in de diamant-linkage geometrie. Deze berekeningen vereisen trigonometrische functies. Dat is precies waar de *bCORDIC*-module zijn waarde bewijst. CORDIC is een custom Verilog-component die gesynthetiseerd is in de programmeerbare logica van de PSoC5 en sinus- en cosinusoperaties berekent in hardware, buiten de CPU om. Het resultaat: kortere cyclustijden en een CPU die vrij blijft voor overige taken. De volledige IK-pipeline is gevalideerd op een fysieke robotpoot.

=== ODrive driver en CAN Stack

De *ODrive C-driver* beheert de volledige communicatie met de motorcontrollers. De driver implementeert een #acr("FSM") met hartslag-monitoring per as: als een as niet meer reageert, detecteert de driver dit zelfstandig. Onder de ODrive-driver ligt de *CanStack-bibliotheek*, een platform-onafhankelijke CAN 2.0 abstractielaag. De PSoC5-backend (`CanStackPSoC5.c`) ondersteunt configureerbare RX/TX mailboxen met callbacks en optionele hardwarefiltering. Zo kan de hogere laag met CAN-berichten werken zonder platformspecifieke details te kennen.


== Interactie met AI

De *NVIDIA Jetson Orin Nano Super* voegt twee lagen toe aan de robot die buiten het bereik van de PSoC5-controllers vallen: ruimtelijk bewustzijn en menselijke interactie.

=== Zien: stereodiepte

Een *Waveshare IMX219-83* stereocamera levert stereobeeldparen aan. Het script `depth_zones.py` berekent hieruit een dispariteitskaart en verdeelt het beeldveld in vier zones, elk gericht op een specifieke detectietaak:

#figure(
  table(
    columns: (auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Zone*], [*Functie*],
    [Links], [Obstakeldetectie links],
    [Midden], [Obstakeldetectie recht vooruit],
    [Rechts], [Obstakeldetectie rechts],
    [Onder], [Val- en gatdetectie],
  ),
  caption: [Dieptezones van de stereocamera.]
)

De mediaandiepte per zone wordt verpakt als een *11-byte serieel pakket* en via USB-serieel naar PSoC5 \#1 gestuurd. Het pakketformaat `[0xAA][L_hi][L_lo][M_hi][M_lo][R_hi][R_lo][B_hi][B_lo][XOR][0x55]` bevat vier uint16-waarden en een XOR-controlegetal, zodat de PSoC corrupte pakketten kan verwerpen.

=== Horen en reageren: Whisper en het soundboard

Naast ruimtelijk bewustzijn heeft de robot ook een stem. *OpenAI Whisper* herkent continu gesproken trefwoorden en triggert op basis hiervan WAV-geluidsreacties uit een gesorteerde geluidsbibliotheek. Dit maakt de robot herkenbaar en interactief - een directe vertaling van de inzichten uit het persona-onderzoek, waarbij bezoekers iets willen _zien doen_ als gevolg van hun invoer.

#figure(
  table(
    columns: (auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Categorie*], [*Trigger*],
    [`compliment/`], [Positieve opmerkingen ("goed zo", "mooi")],
    [`negative/`], [Beledigingen of bedreigingen],
    [`meme/`], [Herkende meme-zinnen],
    [`command/`], [Bevestiging van uitgevoerde opdracht],
    [`confused/`], [Niet-herkende invoer],
    [`idle/`], [Periodieke omgevingsgeluiden],
  ),
  caption: [Soundboard categorieën en triggers.]
)

De spraakherkenning is operationeel op de Jetson hardware. De nauwkeurigheid wordt nog verder verbeterd; er wordt overwogen om over te stappen naar een Nederlandstalig Whisper-model voor betere herkenning in een Nederlandse omgeving.
]