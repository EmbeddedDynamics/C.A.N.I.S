// ============================================
// Prototype — Technische realisatie
// ============================================

== Systeemarchitectuur

Het systeem is opgebouwd rond twee #acr("PSoC")5LP microcontrollers die elk een afgebakende taak vervullen. Deze splitsing zorgt voor een duidelijke scheiding tussen invoerverwerking en bewegingsbesturing, waardoor beide taken onafhankelijk van elkaar kunnen worden ontwikkeld en getest.

#figure(
  table(
    columns: (auto, 1fr),
    stroke: 0.7pt,
    inset: 8pt,
    align: (left, left),
    [*Controller*], [*Verantwoordelijkheid*],
    [PSoC5 #1 — Command Controller],
    [Ontvangt en arbitreert alle externe invoer: Bluetooth-opdrachten via de ESP32-S3 brug, RC-signalen van de ExpressLRS ontvanger en bewegings- en dieptedata van de Jetson via USB-serieel. Herbergt de MPU-6050 #acr("IMU") voor oriëntatiemeting. Stuurt bewegingsopdrachten door naar de motion controller via een directe UART of I²C verbinding. Draait FreeRTOS.],
    [PSoC5 #2 — Motion Controller],
    [Voert de inverse kinematica solver uit, hardwareversneld door een custom CORDIC-eenheid in de programmeerbare logica van de PSoC5. Berekent de gewrichtshoeken voor alle benen en stuurt 12 motoren aan via ODrive v3 motorcontrollers over de CAN-bus. Draait FreeRTOS.],
  ),
  caption: [Taakverdeling tussen de twee PSoC5LP microcontrollers.]
)

De twee controllers zijn verbonden via een directe seriële link (UART of I²C). De communicatie met de ODrives verloopt via een #acr("CAN") 2.0 bus op 1000 kbps met 11-bit identifiers.

#pagebreak()

== Mechanisch Ontwerp

=== Capstan Aandrijving

Elke poot en schoudermodule maakt gebruik van een capstan-aandrijving. Dit principe zet de hoge snelheid en lage kracht van een BLDC-motor om naar lage snelheid en hoge kracht, zonder de nadelenvan tandwielreductie zoals speling (backlash).

Een kleine drum is bevestigd op de motorenas. Om deze drum loopt een DM-20 kabel die verbonden is met een grotere drum. Het diameterverhouding tussen de kleine en grote drum bedraagt *7:1*: zeven omwentelingen van de motor resulteren in één omwenteling van de grote drum. De kleine drums zijn gefabriceerd uit PC-ABS vanwege de hogere stijfheid en slijtvastheid. De overige constructieve onderdelen zijn 3D-geprint in PLA.

#figure(
  table(
    columns: (auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Parameter*], [*Waarde*],
    [Reductie verhouding], [7:1],
    [Kabel type], [DM-20],
    [Kleine drum materiaal], [PC-ABS],
    [Grote drum / structuur materiaal], [PLA],
  ),
  caption: [Capstan aandrijving specificaties.]
)

=== Diamant-Linkage Pootmechanisme

Aan de grote drum is een arm van *85 mm* bevestigd. Aan het uiteinde van deze arm zijn twee schakelarms van elk *120 mm* verbonden. Deze twee arms sluiten aan het ondereinde op een gemeenschappelijk punt, waardoor een gesloten diamantvormige (⟨⟩) vierstaaf-kinematica ontstaat. Door de twee driverarmen (via de twee motoren per module) ten opzichte van elkaar te verstellen, kan het eindpunt van de poot naar elke gewenste positie binnen het werkbereik worden gebracht.

#figure(
  table(
    columns: (auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Onderdeel*], [*Maat*],
    [Driver arm (aan grote drum)], [85 mm],
    [Schakelarm (limb)], [120 mm],
  ),
  caption: [Geometrie van de diamant-linkage.]
)

=== Schoudermodules

Elk schoudermodule bevat twee motoren en bestuurt de pitch (kanteling) van twee benen. De grote drum van de schouder is direct als rotatie-as van de pootbevestiging gebruikt: de poot is gemonteerd op de as van de grote drum in plaats van op een verlenging zoals bij de poten zelf het geval is. Dit maakt een vloeiende kanteling van de volledige poot mogelijk rond één rotatiepunt.

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
  caption: [Overzicht van modules en motoren.]
)

=== Fabricage

De meeste constructieve onderdelen zijn 3D-geprint in PLA. Verbindingen worden gemaakt met geïnsertte M-bouten (heat-set inserts). Alle onderdelen zijn gefabriceerd en individueel getest op maatnauwkeurigheid en mechanische robuustheid.

#pagebreak()

== Motoren & Motorcontrollers

=== BLDC Motoren

Alle 12 motoren zijn van het type *6210 BLDC 200 kV*. De motor is via de capstan-aandrijving verbonden met het mechanisme; de motorenas en de aandrijfas van de poot zijn niet collineair.

=== ODrive Motorcontrollers

De motoren worden aangestuurd door *ODrive v3* motorcontrollers in *position control* modus. Elke ODrive bestuurt twee assen (één module). De ODrives zijn voorzien van custom firmware en worden geconfigureerd via een YAML-configuratiebestand. Na het laden van de configuratie hoeven alleen de #acr("CAN")-adressen per unit te worden aangepast.

De ODrives zijn end-to-end getest met de daadwerkelijke motoren. De communicatie verloopt via de CanStack-bibliotheek op PSoC5 #2 en de ODrive C-driver.

#pagebreak()

== Elektronica & PCB-ontwerp

Er zijn drie custom PCB's ontworpen en gefabriceerd voor dit project.

=== PCB 1 — Hoofd PSoC PCB (Command Controller)

De hoofdprint bevat alle componenten voor de command controller:

- *Cypress PSoC5LP* als hoofdprocessor
- *ESP32-S3* module voor Bluetooth-communicatie
- *MPU-6050* #acr("IMU") voor oriëntatiemeting
- *LDO* spanningsregelaar voor 5V systeemvoeding (vanuit 20V batterijspanning)
- *XT60* connector voor uitgang naar de Jetson Orin Nano (12V via LM2596 buck converter)
- *JST* connectoren voor inter-PCB communicatie en I/O

=== PCB 2 — Motor PSoC PCB (Motion Controller)

De motorprint bevat de aansturing voor de motion controller:

- *Cypress PSoC5LP* als hoofdprocessor
- *6× D-SUB* connectoren voor CAN in/uit van de poot- en schoudermodules
- De CAN-bus is daisy-chained over de D-SUB connectoren op de print
- *JST* connector voor communicatie met PCB 1
- *2× GPIO* per motormodule (totaal 12) voor motor homing
- Externe *CAN-transceivers* (buiten de PSoC5)

De CAN-bedrading bestaat uit zelfgemaakte kabels van CAT6-kabel (twisted pairs) met D-SUB connectoren aan beide uiteinden.

=== PCB 3 — Power Distribution Board

De voedingsprint verdeelt de batterijspanning naar alle verbruikers:

- *14× XT60* connectoren:
  - 12× voor de motorcontrollers (2 per module)
  - 1× voor de PSoC-systemen
  - 1× voor de ingang van de 20V Parkside 4Ah batterij

#figure(
  table(
    columns: (auto, 1fr),
    stroke: 0.7pt,
    inset: 6pt,
    [*Spanning*], [*Gebruik*],
    [20V (batterij)], [ODrive motorcontrollers, ingang Power Distribution Board],
    [12V (LM2596)], [NVIDIA Jetson Orin Nano Super],
    [5V (LDO)], [PSoC5 systemen, ESP32-S3, IMU],
  ),
  caption: [Spanningsoverzicht van het systeem.]
)

#pagebreak()

== Firmware

=== FreeRTOS

Beide PSoC5LP controllers draaien *FreeRTOS* voor real-time taakbeheer. FreeRTOS maakt het mogelijk om meerdere taken (sensoruitlezing, communicatie, besturing) concurrent en deterministisch uit te voeren.

=== Inverse Kinematica & CORDIC

De IKMath-bibliotheek op PSoC5 #2 berekent de gewrichtshoeken voor de diamant-linkage geometrie. De trigonometrische berekeningen worden hardwareversneld door het *bCORDIC*-component, een custom Verilog-module die gesynthetiseerd is in de programmeerbare logica van de PSoC5. Dit offloadt rekenintensieven sinus- en cosinusoperaties van de CPU naar dedicated hardware, met als resultaat kortere cyclustijden.

De IK-pipeline is getest op een fysieke robotpoot en functioneert correct.

=== ODrive Driver

De ODrive C-driver verzorgt de volledige communicatie met de motorcontrollers over CAN. De driver implementeert een toestandsmachine (#acr("FSM")) met heartbeat-monitoring per as. De driver is end-to-end getest met de fysieke motoren in position control modus.

=== CAN Stack

De CanStack-bibliotheek biedt een platform-onafhankelijke CAN 2.0 abstractielaag. De PSoC5-implementatie (`CanStackPSoC5.c`) is de actieve backend en ondersteunt configureerbare RX/TX mailboxen met callbacks en optionele hardwarefiltering.

#pagebreak()

== Jetson AI & Perceptie

De *NVIDIA Jetson Orin Nano Super* verzorgt twee taken: stereodieptewaarneming en spraakgestuurde interactie. De Jetson wordt gevoed op 12V via een LM2596 buck converter op de hoofdprint.

=== Stereodiepte — `depth_zones.py`

Een *Waveshare IMX219-83* stereocamera levert stereobeelden aan. Het script `depth_zones.py` berekent een dispariteitskaart en verdeelt het beeldveld in vier zones:

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

De mediaandiepte per zone wordt als een *11-byte serieel pakket* naar PSoC5 #1 gestuurd via USB-serieel (`/dev/ttyACM0`). Het pakketformaat is: `[0xAA][L_hi][L_lo][M_hi][M_lo][R_hi][R_lo][B_hi][B_lo][XOR][0x55]`, waarbij elke waarde een uint16 is en XOR als controlegetal dient.

=== Spraakherkenning & Soundboard — `robot_dog_ai.py`

Spraakherkenning wordt uitgevoerd met *OpenAI Whisper*. Het systeem luistert continu naar spraakopdrachten en triggert categoriegebonden WAV-geluidsreacties op basis van herkende trefwoorden:

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

De spraakherkenning is operationeel op de Jetson hardware. De nauwkeurigheid wordt nog verbeterd; er wordt overwogen om over te schakelen naar een Nederlandstalig model.
