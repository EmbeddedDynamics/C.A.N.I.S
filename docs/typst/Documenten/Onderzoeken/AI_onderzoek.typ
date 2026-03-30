// ============================================
// AI Research paper - Spraakherkenning & Perceptie
// ============================================

// Import Template
#import "@local/academic-tools:0.1.20": *

// Import Packages
#import "@preview/unify:0.7.1": num, qty, numrange, qtyrange

// Set parameters for the document
#set text(lang: "nl")
#set math.equation(numbering: "(1)")


// ============================================
// Front Page en general definitions
// ============================================

#show: IEEE-academic-journal.with(
  title: [Onderzoek voor de AI gestuurde\ diepte herkenning en interactie],
  authors: "Ruben van der Veen",

  degree: [Bachelor of Electrical Engineering],
  degree-goal: [
    Een onderzoek naar spraakherkenning en omgevingsperceptie voor een interactieve demonstrator door gebruik te maken van AI.
  ],

  department: [Bachelors student],
  university: [NHL Stenden, Hogeschool],
  supervisor: [R. Moedt\ C. Mari Spies],

  date: datetime.today(),
  location: [Leeuwarden, Nederland],

  use-front-cover: true,
  logo: [#image("../Assets/Logo/NHL_logo.jpg", width: 140pt)],

  extend-abstract: false,
  abstract: [
    Dit onderzoek is uitgevoerd in het kader van het Embedded Systems Project
    2025-2026 aan NHL Stenden Hogeschool Leeuwarden. Het project heeft als
    doel een functioneel embedded systeem te ontwikkelen in de vorm van een
    robothond, bestemd als demonstrator voor open dagen.

    In dit kader richt het onderzoek zich op twee AI-deelsystemen: spraakgestuurde
    interactie en stereovisie-gebaseerde omgevingsperceptie. Voor elk deelsysteem
    worden meerdere architecturen geëvalueerd op criteria als nauwkeurigheid,
    latentie, offline werking en rekenvereisten op embedded hardware. Op basis
    hiervan wordt een implementatie gepresenteerd die volledig on-device draait
    op de NVIDIA Jetson Orin Nano Super, zonder afhankelijkheid van een externe
    netwerkverbinding.
  ],

  footer-config: (
    short-title: [AI perceptie & interactie],
    version: "1.0",
    confidentiality: [Internal Use Only],
    author: [],
    affiliation: [NHL Stenden],
    font-size: 8.5pt,
  ),

  references: [#bibliography("/Documenten/Referenties/AI_research.bib", style: "ieee")],
  reference-style: "ieee",
)


// ============================================
// Document contents
// ============================================

= Introductie

Het realiseren van een interactieve robothond als open-dag demonstrator stelt specifieke
eisen aan de embedded AI-componenten. De robot dient niet alleen te bewegen, maar ook
actief te reageren op zijn omgeving en op de bezoekers die ermee in aanraking komen. Dit
vereist twee functionele AI-lagen:

+ *Spraakgestuurde interactie:* de robot luistert naar gesproken taal en reageert hierop
  met contextgepaste geluiden en bewegingen. Dit geeft de indruk van persoonlijkheid en
  maakt de robot aantrekkelijker als demonstrator.

+ *Omgevingsperceptie:* de robot detecteert obstakels en valpartijen in zijn directe
  omgeving en past zijn beweging hierop aan. Dit is essentieel voor autonoom en veilig
  opereren in de ongecontroleerde omgeving van een open dag.

Beide systemen moeten volledig offline functioneren: de robot is niet afhankelijk van een betrouwbare netwerkverbinding. Daarnaast gelden strikte beperkingen op gebied van latentie (reactietijd), rekenvermogen en energieverbruik, opgelegd door de
beschikbare embedded hardware.

Dit onderzoek evalueert de beschikbare architecturen voor elk deelsysteem en legt de
onderbouwing vast voor de gemaakte implementatiekeuzes.


= Spraakherkenning

== Eisen aan het systeem

Voor het spraakherkenningscomponent gelden de volgende systeemeisen:

- *Offline werking*: het systeem mag niet afhankelijk zijn van een cloudverbinding.
- *Lage latentie*: de vertraging tussen uitgesproken woord en systeemreactie dient
  bij voorkeur onder de #qty("500", "ms") te blijven voor een natuurlijk aanvoelende interactie.
- *Nederlandse taalondersteuning*: de primaire doelgroep bestaat uit Nederlandstalige
  bezoekers.
- *Lage resourcebelasting*: het model moet draaien op de NVIDIA Jetson Orin Nano Super
  naast de diepteperceptiepipeline, zonder overmatig geheugen- of GPU-gebruik.
- *Keywordgebaseerde herkenning*: volledige transcriptie is niet vereist; het systeem
  hoeft uitsluitend relevante trefwoorden te detecteren.

== Opties voor spraakherkenning

Diverse architecturen zijn beschikbaar voor spraakherkenning op embedded systemen.
De voornaamste opties worden hieronder vergeleken.

=== Cloud-gebaseerde diensten

Cloudplatforms zoals Google Cloud Speech-to-Text, Microsoft Azure Cognitive Services
en de OpenAI Transcription API bieden hoge nauwkeurigheid voor een breed spectrum aan
talen en accenten. De herkenning vindt echter plaats op externe servers, wat leidt tot:

- Afhankelijkheid van een stabiele internetverbinding.
- Aanzienlijke netwerklatenties, typisch in de orde van #numrange("300", "1500") ms afhankelijk
  van de verbinding.
- Privacyoverwegingen bij het doorsturen van audiogegevens naar externe servers.

Gezien wij graag offline kunnen werken zijn cloudgebaseerde oplossingen niet geschikt voor
dit systeem.

=== Vosk

Vosk @alphacep_vosk_2020 is een open-source framework voor offline spraakherkenning,
ondersteund door een reeks compacte modellen voor meer dan twintig talen, waaronder
Nederlands. De modellen zijn geoptimaliseerd voor embedded hardware en vereisen
slechts #qty("50", "MB") tot #qty("1", "GB") aan geheugen. De nauwkeurigheid is echter significant
lager dan bij grotere modellen, met name voor gevarieerde vocabularia of achtergrondgeluid.

=== wav2vec 2.0

Het wav2vec 2.0 model @baevski_wav2vec_2020 is ontwikkeld door Meta AI Research en
leert spraakrepresentaties op basis van zelfgesuperviseerd leren. Het model biedt hoge
nauwkeurigheid bij beschikbaarheid van domeinspecifieke finetuning-data. De inferentie
is rekenintensief en stelt hogere eisen aan GPU-geheugen dan alternatieve modellen.
Bovendien vergt het finetunen van het model voor Nederlandse trefwoordherkenning
aanzienlijke inspanning.

=== OpenAI Whisper

Whisper @radford_robust_2022 @openai_whisper_github_2022 is een encoder-decoder
sequentiemodel, getraind op #qty("680000", "h") aan meertalig en meertaaksgericht gelabeld
audiomateriaal. Het model is beschikbaar in vijf groottes (Tiny, Base, Small, Medium,
Large), met een directe afweging tussen nauwkeurigheid en rekentijd:

#figure(
  table(
    columns: (auto, auto, auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    align: (left, right, right, left),
    [*Model*], [*Parameters*], [*VRAM*], [*Relatieve snelheid*],
    [tiny],   [39M],   [~1 GB],  [~32×],
    [base],   [74M],   [~1 GB],  [~16×],
    [small],  [244M],  [~2 GB],  [~6×],
    [medium], [769M],  [~5 GB],  [~2×],
    [large],  [1550M], [~10 GB], [1×],
  ),
  caption: [Whisper modelgroottes en bijbehorende resourcevereisten @openai_whisper_github_2022.]
)

Whisper ondersteunt meer dan negentig talen, inclusief het Nederlands. Het model
voert transcriptie uit zonder aparte taaldetectiestap. Bij het opgeven van de taalcode
`nl` wordt de herkenning uitsluitend op Nederlandse spraak gericht, wat de nauwkeurigheid
en snelheid ten goede komt.

== Keuze spraakherkenning

Op basis van de gestelde eisen is gekozen voor *OpenAI Whisper* als spraakherkenningsengine.
De primaire overwegingen zijn:

- *Volledig offline*: alle inferentie vindt lokaal plaats op de Jetson, zonder
  netwerkafhankelijkheid.
- *Meertaligheid*: native ondersteuning voor het Nederlands, inclusief specificatie van
  de doeltaal bij initialisatie.
- *Modelflexibiliteit*: door keuze van de modelgrootte is een directe afweging tussen
  nauwkeurigheid en latentie mogelijk. Op de Jetson Orin Nano Super is het `small`-model
  geschikt: het biedt voldoende nauwkeurigheid bij een latentie die acceptabel is voor
  de toepassing.
- *Ecosysteem*: brede ondersteuning, actieve ontwikkeling en beschikbaarheid als
  Python-package (`openai-whisper`) verlagen de integratiedrempel.

Een nadeel van Whisper ten opzichte van dedicated keyworddetectie-systemen is de hogere
latentie per inferentiecyclus: het model verwerkt audioframes in segmenten en is niet
ontworpen voor streaming word-by-word detectie. Voor de huidige toepassing, waarbij een
reactietijd van enkele honderden milliseconden acceptabel is, vormt dit geen belemmering.

== Nederlandse taalondersteuning

De nauwkeurigheid van Whisper voor het Nederlands is hoog in vergelijking met compactere
alternatieve modellen, maar neemt af bij regionale accenten, dialecten of hoge
achtergrondgeluiden. Gezien de demografische spreiding van de bezoekers op een open dag
(veelal jongvolwassenen uit Noord-Nederland) wordt overwogen om in een volgende iteratie
over te stappen naar een Nederlandstalig finetuned Whisper-model, zoals `GroNLP/whisper-large-ft-coral-nl-v2`.
Dit model is getraind op Nederlandstalige transcriptiedata en presteert aantoonbaar beter
bij dialectvariatie. De afweging is de grotere modelomvang en bijbehorende hogere inferentielatentie.


= Omgevingsperceptie

== Eisen aan het systeem

De omgevingsperceptie dient de robot in staat te stellen obstakels en risicosituaties
(zoals een trap of valpartij) optijd te detecteren. De eisen zijn:

- *Real-time werking*: de diepte-informatie dient met een framerate van minimaal #qty("10", "fps")
  beschikbaar te zijn voor de besturingslogica.
- *Compact en lichtgewicht*: de sensor dient integreerbaar te zijn in het robotchassis
  zonder significant gewicht of volume toe te voegen.
- *Geen actieve emissie vereist*: passieve sensortechnologie geniet de voorkeur
  met het oog op energieverbruik en elektromagnetische compatibiliteit met de CAN-bus.
- *Voldoende bereik*: detectie van obstakels op een afstand van #qty("0.3", "m") tot #qty("2", "m")
  is noodzakelijk voor tijdige reactie bij de gangbare loopsnelheid.

== Opties voor omgevingsperceptie

=== Ultrasone sensoren

Ultrasone sensoren (bijv. HC-SR04) zijn goedkoop en eenvoudig te integreren. De
meetonnauwkeurigheid bedraagt typisch enkele centimeters. Nadelen zijn het smalle
detectiebereik (conische bundel van #qty("15", "deg") tot #qty("30", "deg")) en de beperkte mogelijkheid
tot gelijktijdige meting in meerdere richtingen zonder risico op interferentie.
Voor een systeem dat in meerdere richtingen tegelijk moet meten, is dit suboptimaal.

=== Time-of-Flight (ToF) sensoren

ToF-sensoren zoals de Intel RealSense D435 of de STMicroelectronics VL53L5CX meten
de looptijd van een uitgezonden lichtpuls (infrarood) om afstand te bepalen. Ze bieden
een hoge update-snelheid en nauwkeurige absolute dieptemetingen. De RealSense D435
levert een compleet dieptebeeld (640 × 480 bij 90 fps), maar vergt aanzienlijk meer
rekenvermogen voor verwerking en is relatief duur. Compacte ToF-matrixsensoren zijn
goedkoper maar bieden slechts een beperkte ruimtelijke resolutie (bijv. 8 × 8 pixels
bij de VL53L5CX).

=== Monoculaire diepte-inferentie

Neurale netwerken zoals MiDaS @ranftl_midas_2020 kunnen uit een
enkel camerabeeld een relatieve dieptekaart schatten. Het voordeel is dat slechts
één gewone camera benodigd is. Nadelen zijn de hoge GPU-belasting voor real-time
inferentie en het feit dat de geschatte diepte relatief is: absolute afstandswaarden
zijn niet direct beschikbaar zonder kalibratie.

=== Stereovisiocamera

Een stereovisiocamera maakt gebruik van twee met bekende basislijn gemonteerde
camera's @scharstein_taxonomy_2002. Door dispariteitsmeting tussen de linker- en
rechterafbeelding wordt een absoluut dieptebeeld berekend via:

$ Z = frac(f dot B, d) $ <eq:stereo_depth>

waarbij $Z$ de diepte is, $f$ de brandpuntsafstand in pixels, $B$ de basislijn in
meter en $d$ de dispariteit in pixels. Het Semiglobal Block Matching (SGBM)
algoritme @hirschmuller_stereo_2008 biedt een goede balans tussen nauwkeurigheid
en rekentijd en is beschikbaar als optimale implementatie in OpenCV.

== Keuze perceptiesysteem

Op basis van de gestelde eisen is gekozen voor een *stereovisiocamera*, specifiek de
*Waveshare IMX219-83* @waveshare_imx219_2023. Deze sensor combineert twee IMX219-sensoren
met een vaste basislijn van #qty("83", "mm") in één compacte behuizing met directe CSI-aansluiting
op de Jetson. De keuze is onderbouwd door:

- *Passieve werking*: geen emissie van licht of geluid, geen EMC-problemen.
- *Absoluut dieptebereik*: via @eq:stereo_depth worden absolute afstandswaarden
  berekend, zonder kalibratie-afhankelijkheid.
- *Directe Jetson-integratie*: de IMX219-sensor wordt native ondersteund via de
  Argus Camera API en GStreamer pipeline op de Jetson @jetsonhacksConnectYourRaspberry2025.
- *Lage kosten*: significant goedkoper dan een dedicated ToF-dieptecamera bij
  vergelijkbaar of hoger ruimtelijk bereik.

Een nadeel van stereovisie is de gevoeligheid voor texturarme vlakken (zoals een
egaal witte vloer), waarbij dispariteitsberekening onbetrouwbaar kan zijn. Dit wordt
gemitigeerd door de zonegebaseerde aanpak beschreven in de volgende sectie.

== Zonegebaseerde diepteverwerking

In plaats van het volledig verwerken van het dispariteitsbeeld wordt het beeldveld
opgedeeld in *vier zones*, elk corresponderend met een specifieke detectietaak:

#figure(
  table(
    columns: (auto, auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Zone*], [*Positie in beeld*], [*Functie*],
    [Links],  [Linker kwadrant],         [Obstakeldetectie links],
    [Midden], [Centraal kwadrant],       [Obstakeldetectie recht vooruit],
    [Rechts], [Rechter kwadrant],        [Obstakeldetectie rechts],
    [Onder],  [Onderste horizontale balk], [Val- en gat-detectie],
  ),
  caption: [Vier dieptezones en hun functie binnen de perceptiepipeline.]
)

Per zone wordt de mediaandiepte berekend. Het gebruik van de mediaan in plaats van
het gemiddelde maakt de meting robuust tegen uitschieters veroorzaakt door ruis,
reflecties of texturarme vlakken. De vier mediaanwaarden worden verpakt als een
11-byte serieel pakket en via USB-serieel doorgestuurd naar de PSoC5 command controller
voor verdere verwerking.

Elke zone levert een uint16-waarde in millimeters. Het XOR-checksum stelt de ontvanger
in staat corrupte pakketten te verwerpen, wat essentieel is bij de hogere baudrates
die benodigd zijn voor real-time verwerking.


= Interactiesysteem

== Architectuurkeuze

Op basis van de herkende trefwoorden dient de robot contextgepaste auditieve feedback
te genereren. Hiervoor zijn twee architecturen overwogen:

- *Text-to-speech (TTS)*: real-time gegenereerde spraak op basis van een taalmodel.
  Voordelen: flexibele en gevarieerde reacties. Nadelen: hoge rekenbelasting, hogere
  latentie, minder expressief dan opgenomen geluid.

- *Soundboard met keywordclassificatie*: vooraf opgenomen geluidsbestanden, getriggerd
  op basis van een classificatie van de herkende invoer. Voordelen: instant afspeellatentie,
  geen extra GPU-belasting, volledig deterministische reacties.

Gezien de eisen aan latentie en de reeds hoge GPU-belasting van de Whisper- en
dispariteitspipelines is gekozen voor het *soundboard-model*. De nadruk ligt op
expressieve, karakteristieke reacties die passen bij een robotpersoonlijkheid - een
doel dat beter bereikt wordt met geselecteerd audiomateriaal dan met TTS-uitvoer.

== Keywordclassificatie

De getranskribeerde tekst van Whisper wordt geclassificeerd op basis van aanwezigheid
van trefwoorden uit vooraf gedefinieerde categorieën. De classificatie werkt sequentieel:
zodra een trefwoord in een hogere-prioritaire categorie wordt gevonden, wordt de
bijbehorende categorie geselecteerd en een willekeurig geluidsbestand uit de
corresponderende map afgespeeld.

#figure(
  table(
    columns: (auto, auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Categorie*], [*Triggercondities*], [*Prioriteit*],
    [`command/`],    [Herkende bewegingsopdrachten ("kom", "zit", "loop")], [1],
    [`compliment/`], [Positieve opmerkingen ("goed", "mooi", "lief")],      [2],
    [`negative/`],   [Beledigingen of bedreigingen],                         [3],
    [`meme/`],       [Herkende internetreferenties en uitdrukkingen],        [4],
    [`confused/`],   [Geen trefwoord herkend binnen timeout],                [5],
    [`idle/`],       [Periodiek, onafhankelijk van invoer],                  [6],
  ),
  caption: [Soundboard-categorieën met triggercondities en prioriteitsvolgorde.]
)

De `idle`-categorie wordt geactiveerd door een timer: als gedurende een instelbare
periode geen invoer is verwerkt, speelt de robot een willekeurig omgevingsgeluid af.
Dit versterkt de indruk van een "levende" entiteit en trekt de aandacht van passerende
bezoekers.

== Integratie met bewegingsbesturing

De herkende trefwoorden uit de `command`-categorie worden niet alleen gebruikt om een
bevestigingsgeluid te spelen, maar ook als bewegingsopdracht doorgezonden naar de PSoC5
command controller via het USB-serieel kanaal. De seriële communicatie naar de PSoC5 is
gedeeld met de dieptepipeline; pakketten worden gedifferentieerd door hun startbyte
(`0xAA` voor diepte, `0xBB` voor spraakcommando's).


= Hardware platform

De NVIDIA Jetson Orin Nano Super @nvidia_jetson_2024 is gekozen als verwerkingsplatform
voor alle AI-taken. De relevante specificaties voor dit project zijn:

#figure(
  table(
    columns: (auto, auto),
    stroke: 0.7pt,
    inset: 6pt,
    [*Specificatie*], [*Waarde*],
    [CPU],              [6-core Arm Cortex-A78AE],
    [GPU],              [1024-core NVIDIA Ampere],
    [AI-prestatie],     [67 TOPS],
    [Systeemgeheugen],  [8 GB LPDDR5],
    [Voeding],          [7W -- 25W (instelbaar)],
    [Aansluiting PSoC], [USB-serieel (`/dev/ttyACM0`)],
    [Camerainterface],  [2× MIPI CSI-2 (voor IMX219-83)],
  ),
  caption: [NVIDIA Jetson Orin Nano Super systeemspecificaties @nvidia_jetson_2024.]
)

De Jetson wordt gevoed op #qty("12", "V") via een LM2596 buck converter, gevoed vanuit de
#qty("20", "V") Parkside-accu via de hoofd-PCB. Dit is de enige verbinding tussen de Jetson
en de overige elektronica, naast de USB-seriële link naar de PSoC5.


= Conclusie

In dit onderzoek zijn twee AI-deelsystemen geanalyseerd en geïmplementeerd voor de
Robothond: spraakgestuurde interactie en stereovisie-gebaseerde omgevingsperceptie.

Voor spraakherkenning is OpenAI Whisper gekozen vanwege de combinatie van hoge
nauwkeurigheid, offline werking, native Nederlandse taalondersteuning en de flexibiliteit
in modelgrootte. Het `small`-model biedt op de Jetson Orin Nano Super een acceptabele
latentie bij adequate herkenningsnauwkeurigheid. Voor toekomstige iteraties wordt
finetuning op Nederlandstalige data aanbevolen, met name om de prestaties bij regionale
accenten te verbeteren.

Voor omgevingsperceptie is de Waveshare IMX219-83 stereocamera geselecteerd als passief,
compact en direct integreerbaar platform. De zonegebaseerde diepteverwerking met
mediaanfiltering biedt een robuuste en rekenefficiënte aanpak die geschikt is voor
real-time gebruik naast de spraakpipeline. De seriële communicatie via een gestructureerd
pakketprotocol borgt betrouwbare overdracht naar de PSoC5 command controller.

Het soundboard-interactiemodel vormt een bewuste keuze voor expressiviteit boven
flexibiliteit: de deterministische, lage-latentie reacties versterken de beleving van
de robotpersoonlijkheid tijdens demonstraties aanzienlijk meer dan TTS-gegenereerde
uitvoer bij de gegeven hardware-beperkingen.

De combinatie van deze componenten resulteert in een AI-platform dat volledig autonoom
opereert, zonder cloudafhankelijkheid, en dat zowel functioneel als demonstratief
voldoet aan de eisen gesteld door het project.

// Function for the code blocks
#let code-block(content) = {
  block(
  width: 100%,
  fill: luma(160),
  inset: 2pt,
  radius: 6pt,
  block(
    width: 100%,
    fill: luma(243),
    inset: 8pt,
    radius: 4pt,
    content,
    ),
  )
}


= Handige commando's bij de Jetson:

=== List alle camera's die aangesloten zijn.
#code-block(
  ```bash
  ls /dev/video*
  ```
)

=== Open CSI connectors (en meer IO) configuratie scherm.
#code-block(
  ```bash
  sudo python ~/opt/nvidia/jetson-io/jetson-io.py
  ```
)

=== Camera pipeline met GStreamer om beeld te krijgen:
#code-block(
  ```bash
  # sensor-id is voor elke camera. 
  # id komt overeen met ouput van /dev/video*
  
  gst-launch-1.0 nvarguscamerasrc sensor-id=0 ! nv3dsink
  ```
)

=== Stuur een bestand van de Jetson naar je PC.
#code-block(
  ```bash
  scp perijn@192.168.178.231:~/depth_debug.jpg C:\Users\ruben\Downloads\depth_debug.jpg
  ```
)

=== Installeer Ollama.
#code-block(
  ```bash
  curl -fsSL https://ollama.com/install.sh | sh
  ```
)