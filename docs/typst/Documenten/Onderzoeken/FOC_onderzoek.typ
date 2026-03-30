// ============================================
// FOC research paper
// ============================================

#set page(numbering: "1")

#align(center)[
  = FOC en BLDC Motoren
  \ 
  Naam: Daan Smit
  
  Datum: 30-3-2026
]

#pagebreak()

= Inhoudsopgave
#outline()

#pagebreak()

= Inleiding
In dit document wordt de werking van verschillende typen motoren, electronic speed controller en Field Oriented Control (FOC) uitgelegd. Als laatst wordt onderbouwd welke keuzes worden gemaakt voor dit project. 

#pagebreak()

= Werking motoren
In dit hoofdstuk worden de werking van verschillende typen motoren uitgelegd.

== Werking van een BLDC motor
Een brushless DC motor is een gelijkstroommotor zonder borstels, waarbij een elektronische snelheidsregelaar de snelheid en aansturing verzorgt. BLDC-motoren hebben een hogere efficiëntie dan DC motoren met borstels en zijn robuuster omdat er geen borstels aanwezig zijn die kunnen slijten.


#figure(
  image("../Assets/FOC/BLDC motor.png"),
  caption: [Opbouw van een BLDC-motor]
)

Een BLDC motor bestaat uit een stator en een rotor. De stator staat stil terwijl de rotor ronddraait. Wanneer er stroom door de spoelen in de stator loopt ontstaat er een magnetisch veld. Door deze magnetische velden in een bepaalde volgorde te activeren worden de magneten in de rotor afwisselend aangetrokken en afgestoten. Hierdoor ontstaat een draaiende beweging van de rotor. 
#pagebreak()

#figure(
  image("../Assets/FOC/BLDC2.png"),
  caption: [Bekrachtiging spoelen in motor]
)

Om de efficiëntie te verbeteren worden tegenovergestelde spoelen tegelijk aangestuurd, waardoor een sterker magnetisch veld ontstaat en de motor soepeler en krachtiger draait. (howtomechatronics, 2021)


=== Voor en nadelen BLDC
*voordelen*
- Door het ontbreken van borstels is er minder energieverlies en minder warmteontwikkeling
- Minder slijtage omdat er geen borstels zijn
- Met Hall-sensors of encoders kan de snelheid, positie en draairichting precies worden geregeld
- Geen borstels die vervangen hoeven te worden
- Goed geschikt voor toepassingen waar zowel kracht als snelheid belangrijk is

*nadelen*
- Een controller is nodig voor commutatie en positionering
- Voor nauwkeurige positionering zijn Hall-sensors of encoders vereist
- Magnetische storingen kunnen de sensoren beïnvloeden.

#pagebreak()
== Werking stappenmotor
Een steppermotor is een elektrische motor waarvan de as in stapjes draait. Dit betekent dat de motor met een vaste hoek per stap beweegt waardoor je zonder extra sensoren altijd weet in welke positie de as staat. Hierdoor zijn steppermotoren geschikt voor toepassingen waar nauwkeurige positionering belangrijk is. Een steppermotor is een elektrische motor waarvan de as in stapjes draait. Dit betekent dat de motor met een vaste hoek per stap beweegt waardoor je zonder extra sensoren altijd weet in welke positie de as staat. Hierdoor zijn steppermotoren geschikt voor toepassingen waar nauwkeurige positionering belangrijk is. (monoliathicpower, 2026)

=== Voor en nadelen stappenmotor 
*voordelen*
- De motor draait in vaste stappen, waardoor je de positie kunt bepalen door het aantal stappen te tellen
- Door stap voor stap is de motor eenvoudig aan te sturen met een driver of microcontroller
- Met microstepping kan de hoekresolutie zeer klein zijn

*nadelen*
- Bij een te hoge belasting kan de motor stappen overslaan waardoor de positie niet meer klopt
- De motor blijft vaak stroom verbruiken ook wanneer hij stilstaat wat efficiëntie vermindert en warmte genereert
- Bij snelle bewegingen daalt het koppel sterk
- Bij hogere snelheden of microstepping kan de motor trillen en geluid maken
- Voor dezelfde grootte levert een stappenmotor minder koppel dan een BLDC of servomotor


#pagebreak()

== Werking servo
Een servomotor is een motor met een ingebouwd feedbackmechanisme waardoor de positie, snelheid en versnelling van de motoras nauwkeurig kan worden geregeld. De motoras wordt via een tandwielset gekoppeld aan de feedbacksensor. Wanneer een elektrisch signaal via de signaaldraad wordt gestuurd bepaalt de duur van dit signaal de draaihoek van de as. Bijvoorbeeld een puls van 1,5 ms brengt de as in het midden (90°). Kortere pulsen draaien de as naar links en langere pulsen naar rechts. De servomotor gebruikt proportionele besturing wat betekent dat de motorsnelheid afhangt van het verschil tussen de huidige en gewenste positie. Staat de as dicht bij de gewenste positie dan draait de motor langzaam. Als hij verweg staat dan draait hij sneller. Zo werkt de motor efficiënt en precies.

*voordelen*
- Dankzij de feedbacksensor kan de motor exact naar de gewenste positie draaien
- De motorsnelheid past zich automatisch aan op basis van de afstand tot de gewenste positie
- De motor kan een positie vasthouden en weerstand bieden tegen externe krachten

*nadelen*
- Beperkte rotatiehoek
- Door de ingebouwde besturing en feedback zijn servos duurder dan gewone motoren
- Werking vereist signaalbesturing en een driver
- Kleine servomotoren leveren niet veel koppel of snelheid vergeleken met grotere motoren
- Bij continu maximale belasting kan slijtage optreden vooral aan tandwielen


#pagebreak()


= Aansturing van een BLDC-motor
Een Electronic Speed Controller (ESC) werkt door MOSFETs aan te sturen om zo een roterend magnetisch veld te creëren. Deze MOSFETs zijn geconfigureerd in zogenaamde half-bridge schakelingen. In een eenvoudige driefasige opstelling bestaat de ESC uit drie half-bridges, wat neerkomt op in totaal zes MOSFETs. Door deze MOSFETs in een specifieke volgorde te schakelen  ontstaan er verschillende schakelpaden waarmee de fasen van de motor worden aangestuurd. Deze schakelpaden zijn de commutatiestappen. In een standaard driefasige BLDC-aansturing zijn er zes verschillende commutatiestappen die samen zorgen voor een continu roterend magnetisch veld. (Greatscott, 2018)

#figure(
  image("../Assets/FOC/FOC1.png"),
  caption: [space vector diagram commutatiestappen]
)

Wanneer alle zes magnetische veldoriëntaties in een space vector diagram worden weergegeven, vormen ze samen een zeshoek. De magneten volgen elke stap van deze zeshoek. Een nadeel van deze methode is.

== 6-step commutatie
Bij 6-step commutatie worden de fasen in zes stappen geschakeld, wat resulteert in een roterend magnetisch veld in discrete stappen.

#pagebreak()

= Positiebepaling

== Sensorless
Bij sensorless aansturing van een BLDC-motor wordt de rotorpositie bepaald zonder gebruik te maken van fysieke sensoren zoals Hall-sensoren of encoders. In plaats daarvan wordt gebruikgemaakt back-EMF die in de motorwikkelingen wordt opgewekt.
Wanneer de rotor draait bewegen de magneten langs de spoelen in de stator. Hierdoor wordt in deze spoelen een spanning opgewekt vergelijkbaar met een generator. Deze spanning wordt de back-EMF genoemd en is recht evenredig met de rotatiesnelheid van de motor. In een driefasige BLDC-motor wordt tijdens elke commutatiestap één fase niet bekrachtigd. In deze fase kan de back-EMF gemeten worden. Door het moment te detecteren waarop deze spanning het nulpunt kruist kan de positie van de rotor worden bepaald. Op basis hiervan kan de ESC de volgende commutatiestap uitvoeren. Het meten van back-EMF kan op verschillende manieren worden uitgevoerd zoals met een comparator die de spanning vergelijkt met een referentie
via een virtueel nulpunt met behulp van een ADC in een microcontroller. Een belangrijk voordeel van sensorless aansturing is dat er geen extra sensoren nodig zijn. De nadelen zijn dat bij stilstand de motor geen back-EMF opwekt waardoor de positie van de rotor niet bekend is. Daarom moet de motor worden opgestart in open loop waarbij de spoelen in een vaste volgorde worden bekrachtigd totdat er voldoende snelheid is opgebouwd. Daarnaast is de back-EMF bij lage snelheden klein en moeilijk te meten, wat kan leiden tot minder nauwkeurige aansturing.
(Digikey, 2026)

== Hall-sensoren
Hall-effect sensoren zijn belangrijke componenten in BLDC motoren en worden gebruikt om de positie van de rotor nauwkeurig te detecteren. Deze informatie is essentieel om de fases van de motor op het juiste moment te laten energiseren wat zorgt voor een efficiënte en soepele werking van de motor. Hall-effect sensoren werken volgens het principe dat wanneer een stroomvoerende geleider zich in een magnetisch veld bevindt een spanning ontstaat die loodrecht op de stroomrichting en het magnetisch veld staat. In BLDC-motoren worden Hall-sensoren dicht bij de stator geplaatst op specifieke hoeken. Bij een driefasige BLDC-motor bevinden drie Hall-sensoren zich meestal 120 graden van elkaar verwijderd. Deze sensoren detecteren de positie van de rotor door veranderingen in het magnetisch veld dat door de rotor wordt opgewekt. De sensoren sturen vervolgens feedback naar de motorcontroller zodat de juiste spoelen van de stator op het juiste moment van stroom worden voorzien. (Mechtex, 2023)

#pagebreak()

== Encoders
Een motorencoder is een apparaat dat de positie, snelheid en draairichting van een motoras meet en omzet in elektrische signalen. Hierdoor kan een controller de motor nauwkeurig aansturen. Encoders zijn essentieel in servo- en BLDC-motoren, omdat ze real-time informatie geven over de beweging van de rotor. Encoders registreren de rotatie van de motoras en sturen pulsen of signalen naar een controller. Afhankelijk van de technologie zijn er verschillende soorten encoders. (eltra-encoder, 2021)

- Magnetisch: gebruikt magneten en Hall-effect sensoren om de positie te meten.

- Magnetoresistief: verandert de weerstand bij veranderingen in het magnetisch veld.

- Optisch: gebruikt een schijf met markeringen en een lichtsensor om beweging te detecteren.

#pagebreak()

= Field Oriented Control (FOC)

== Wat is FOC?

Field oriented control of ook wel genoemd vector control is een regelmethode voor motoren waarbij de motorstromen worden ontbonden in twee componenten. Een component voor de flux en een component voor de koppel. Hierdoor wordt de motor niet meer in zes vaste stappen aangestuurd, maar met sinusvormige stromen. Hierdoor ontstaat een vloeiend roterend magnetisch veld. In plaats van slechts zes vaste magnetische veldoriëntaties kan FOC het magnetisch veld continu aanpassen. In een space vector diagram betekent dit dat de vector niet alleen langs de hoeken van een hexagon beweegt maar vloeiend over de hele cirkel kan roteren. (GreatScott, 2018) Dit zorgt voor een soepelere rotatie, minder geluid, hogere efficiëntie en betere controle. (Wikipedia contributors, 2026). 

#figure(
  image("../Assets/FOC/FOC2.png"),
  caption: [space vector diagram Field Oriented Control (FOC)]
)

== Verschil met 6-step
Bij 6-step aansturing verandert het magnetisch veld in discrete stappen. Bij FOC wordt het veld continu aangepast, waardoor de motor soepeler draait. De space vector beweegt zich vloeiend over de circkel inplaats van een 6 hoek bij 6 stap commutatie.

#pagebreak()

== Gebruik met encoder
Bij Field-Oriented Control (FOC) is het belangrijk om te positie te weten van de rotorpositie om de stromen in de stator correct te regelen. Een encoder wordt gebruikt om deze positie direct te meten. De FOC controller kan exact berekenen welke component van de stroom het koppel levert en welke de flux regelt. Dankzij deze feedback kan de motor vloeiend draaien bij lage snelheid een maximaal koppel leveren bij start en efficiënt werken over het volledige snelheidsbereik. Encoders maken FOC betrouwbaarder dan systemen die alleen op back-EMF vertrouwen vooral bij lage snelheden waar back-EMF-signalen zwak zijn. Het gebruik van een encoder bij FOC verbetert de precisie en koppel.

#pagebreak()



= Motivatie van ontwerpkeuzes

Voor de aandrijving van de poten van een robothond is gekozen voor BLDC-motoren vanwege de combinatie van kracht, precisie en efficiëntie. BLDC-motoren leveren een hoog koppel bij lage snelheid wat essentieel is voor soepele bewegingen en stabiliteit tijdens het lopen of rennen van de robot. Dit maakt ze beter geschikt dan bijvoorbeeld stappenmotoren die bij hoge belasting stappen kunnen missen en daardoor de beweging onnauwkeurig maken. Daarnaast zijn BLDC-motoren lichtgewicht en compact waardoor ze eenvoudig in de relatief kleine en beperkte ruimte van een robothondpoot passen. Dit verhoogt de betrouwbaarheid van de robot bij langdurig gebruik. Door het gebruik van Field-Oriented Control (FOC) en encoders kan de BLDC-motor precies worden aangestuurd zodat de robothond nauwkeurige bewegingen kan maken.


#pagebreak()


= Bronnen
Howtomechatronics (2021) *How Brushless DC Motor Works? BLDC and ESC Explained* Geraadpleegd van https://howtomechatronics.com/how-it-works/how-brushless-motor-and-esc-work/ 

Monolithicpower (2026) *Stepper Motors Basics: Types, Uses, and Working Principles* Geraadpleegd van https://www.monolithicpower.com/en/learning/resources/stepper-motors-basics-types-uses

Greatscott (2018) *What is FOC? (Field Oriented Control) And why you should use it! || BLDC Motor* Geraadpleegd van https://www.youtube.com/watch?v=Nhy6g9wGHow&t=343s

Wikipedia contributors (2026). *Field-oriented control*. 
Geraadpleegd van https://en.wikipedia.org/wiki/Field-oriented_control

Digikey (2013)  *Controlling Sensorless, BLDC Motors via Back EMF* Geraadpleeg van https://www.digikey.nl/nl/articles/controlling-sensorless-bldc-motors-via-back-emf 

Mechtex (2023) *Understanding the Hall Effect Sensors in BLDC Motors* Geraadpleeg van https://mechtex.com/blog/understanding-the-hall-effect-sensors-in-bldc-motors

Eltra-encoder (2021) *What Is Motor Encoder?* Geraadpleeg van https://eltra-encoder.eu/news/what-is-motor-encoder