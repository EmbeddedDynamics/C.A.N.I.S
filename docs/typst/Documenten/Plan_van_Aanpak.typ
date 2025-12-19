#import "@preview/fletcher:0.5.8" as fletcher: diagram, node, edge

#import "@preview/oxifmt:0.2.1": strfmt

#import "@preview/tablem:0.1.0": tablem
#import "@preview/numbly:0.1.0": numbly

#import "template.typ": *
#import "Config/config.typ": *
#import "Stakeholder_analyse.typ": *

#set text(lang: "Nl")

#show link: underline


= Gegenereerd door Ruben:

= Achtergronden

Binnen het project Embedded Systems ontwikkelen teams een embedded device met aantoonbare functionaliteit, onderbouwde ontwerpkeuzes en een compleet documentatiespoor. Omdat NHL Stenden werkt met Design Based Education (DBE), staat een gebruikersgerichte en iteratieve aanpak centraal.

Onze robothond is bedoeld als demonstrator voor open dagen: bezoekers moeten het systeem kunnen zien, begrijpen en beleven. Daardoor liggen de accenten niet alleen op “het werkt”, maar ook op betrouwbaarheid, veiligheid, interactie en uitlegbaarheid.

= Opdrachtnemers

De opdrachtnemers van dit project zijn Ruben van der Veen, Perijn Huijser en Daan Smit. Wij zijn gezamenlijk verantwoordelijk voor het ontwerp, de realisatie en de integratie van het systeem, en daarnaast voor het opstellen van testplannen, het uitvoeren van verificatie en validatie, en het vastleggen van ontwerpbeslissingen. Hiermee borgen wij dat het project niet alleen een werkend prototype oplevert, maar ook een aantoonbaar en toetsbaar ontwikkelproces.

= Opdrachtgever en begeleiding

*Opdrachtgever / toezichthouder:* \ Rieno Moedt – docent Elektrotechniek

*Tutor:* \ Chel-Marí Spies

Begeleiding wordt ingezet voor inhoudelijke feedback, scopebewaking en toetsing van tussenproducten.

= Projectomschrijving

Het projectresultaat is een embedded systeem in de vorm van een robothond. De robothond moet tijdens open dagen zowel technisch inhoudelijk overtuigen als toegankelijk blijven voor een niet-technisch publiek. Dat betekent dat het systeem herhaalbaar moet functioneren, veilig te bedienen is en op een begrijpelijke manier laat zien wat er ‘onder de motorkap’ gebeurt.

Binnen de opdracht liggen de accenten op embedded firmware, regelsystemen en draadloze communicatie, aangevuld met energiebeheer en minimaal één HDL-component. De ontwikkeling gebeurt iteratief volgens Design Thinking. Ontwerpkeuzes worden daarom niet alleen op technische haalbaarheid beoordeeld, maar ook op gebruikerswaarde, risico en planning. Op die manier voorkomen we dat we een complex systeem bouwen dat tijdens demonstraties moeilijk uit te leggen of onvoldoende robuust blijkt.

= Projectresultaat

== Minimum Viable Product (MVP)

Het Minimum Viable Product is het kleinste geheel dat aantoonbaar voldoet aan de must-have eisen en bruikbaar is als demonstratie. In dit project betekent dat dat de robothond zich op vier poten gecontroleerd kan verplaatsen op basis van berekende inverse kinematics (IK). Het MVP is pas ‘geslaagd’ wanneer de robothond minimaal vooruit kan lopen en wanneer een gebruiker hem via een bluetooth controller betrouwbaar kan aansturen. Dit is bewust gekozen: zonder gecontroleerde beweging en zonder eenvoudige bediening is het product tijdens open dagen niet inzetbaar.

== Gewenste uitbreidingen (indien tijd)

Naast het MVP streven wij naar uitbreidingen die de demonstratiewaarde vergroten, zonder de kernbetrouwbaarheid te ondermijnen. Denk hierbij aan extra bewegingsrichtingen (achteruit, zijwaarts, om de as draaien), stabilisatie met een IMU in combinatie met een PID-regelaar, en interactie via audio of gedrag dat ‘persoonlijkheid’ suggereert. Ook onderzoeken we modulaire uitbreidingen, zodat later eenvoudig extra functies toegevoegd kunnen worden wanneer tijd en middelen dit toelaten.

== Niet in scope (won’t-have)

Om risico’s en scope creep te beperken maken wij expliciet wat níet wordt nagestreefd binnen dit project. Geavanceerde LIDAR/SLAM-systemen, smartphone-apps of cloudkoppelingen vallen buiten de scope, omdat deze de complexiteit en integratierisico’s sterk verhogen zonder dat ze noodzakelijk zijn voor een overtuigende open-dag demonstratie. Ook industriële certificeringstrajecten zoals EMC/EMI-compliance zijn niet haalbaar binnen de projectduur en vallen daarom buiten de projectgrenzen.

= Probleemstelling

Tijdens open dagen is er behoefte aan demonstrators die:

technisch inhoudelijk sterk zijn,

veilig en betrouwbaar werken,

interactief en aantrekkelijk zijn voor niet-technische bezoekers.

Probleemstelling:
Hoe ontwikkelen wij binnen de projectduur een robothond (embedded demonstrator) die technisch diepgaand is en tegelijk robuust, veilig en gebruiksvriendelijk genoeg voor demonstratie aan een breed publiek?

= Doelstelling

Het hoofddoel van dit project is het realiseren van een demonstrabele robothond die voldoet aan het vastgestelde Pakket van Eisen (PvE), inclusief aantoonbare verificatie en validatie. Daaruit volgen subdoelen die het project bestuurbaar maken: we willen vóór de proof-of-concept demo een werkend MVP opleveren, de betrouwbaarheid verhogen door iteratief prototypen en systematisch testen, en volledige traceerbaarheid realiseren van eis naar ontwerpkeuze en van ontwerpkeuze naar testresultaat. Daarnaast is het doel dat het product tijdens open dagen reproduceerbaar werkt en veilig door bezoekers (onder begeleiding) bediend kan worden.

= Onderzoeksvragen

Om ontwerpkeuzes te onderbouwen en risico’s te reduceren formuleren wij onderzoeksvragen die zowel technisch als gebruikersgericht zijn. We onderzoeken welke hardware- en architectuurkeuzes de beste balans geven tussen performance, energieverbruik en betrouwbaarheid. Ook bepalen we welke regelsystemen nodig zijn om gecontroleerde en herhaalbare beweging (met inverse kinematics) te realiseren, en welke draadloze communicatie het meest robuust is én eenvoudig te demonstreren.

Daarnaast richten we ons op testbaarheid: welke teststrategie is nodig om subsystemen aantoonbaar te verifiëren en valideren, zodat problemen vroeg zichtbaar worden in plaats van pas bij eindintegratie. Tot slot onderzoeken we welke interactievormen de gebruikerswaarde verhogen zonder dat de scope ontspoort.

= Resultaat voor school

Binnen de opleiding wordt verwacht dat wij, binnen de vastgestelde deadlines, een reeks toetsbare tussenproducten en een werkend eindresultaat opleveren. Dit omvat onder andere een PvE met MoSCoW-prioritering en een gedefinieerd MVP, blokdiagrammen (hoog-over en later gedetailleerd), dit PvA, en testplannen met bijbehorende testrapporten voor verificatie en validatie. Daarnaast leveren wij een proof-of-concept demonstratie in de tussenfase, en uiteindelijk een volledig gedocumenteerd eindproduct inclusief portfolio met gemaakte keuzes, ontwerpen en resultaten.

= Programma van Eisen & Wensen

Wij werken met een PvE waarin eisen zijn geprioriteerd met MoSCoW en waarin het MVP expliciet is vastgelegd. Dit PvA gebruikt het PvE als leidend document voor scope, planning, teststrategie en acceptatiecriteria. In de praktijk betekent dit dat het PvE niet alleen een ‘lijstje’ is, maar een stuurmiddel: wanneer er discussies ontstaan over extra functies, gebruiken wij de prioritering om keuzes te maken die passen binnen tijd, risico en demonstratiewaarde.

Daarnaast zijn er proces-eisen die direct invloed hebben op onze aanpak. Wij voeren wekelijkse tutor-gesprekken en leggen agenda, besluiten en acties vast in notulen. We houden één gezamenlijk logboek bij waarin activiteiten en bijdragen worden geregistreerd. Voor de ontwikkeling gebruiken wij versiebeheer (Git) en werken wij met toegewezen rollen om eigenaarschap en continuïteit te borgen. Tot slot leggen wij traceerbaarheid expliciet vast, zodat iedere eis herleidbaar is naar ontwerpbeslissingen en bijbehorende tests.

= Projectactiviteiten (Design Thinking)

Wij gebruiken Design Thinking als ontwikkel- én documentatieframe. Dit voorkomt tunnelvisie en zorgt dat de oplossing aantoonbaar aansluit bij de gebruiker.

== Empathize (week 1–2)

In de Empathize-fase brengen wij de gebruiker en context in kaart. Omdat het eindproduct tijdens open dagen gebruikt wordt, is het cruciaal om te begrijpen wat bezoekers aantrekkelijk, begrijpelijk en veilig vinden. In deze fase voeren wij een stakeholderanalyse uit en verzamelen wij input via enquêtes en gesprekken. De resultaten vertalen wij naar persona’s en gebruikersscenario’s, zodat we een concreet beeld hebben van wie de robothond gaat bedienen en welke situaties tijdens demonstraties realistisch zijn. De output van deze fase bestaat uit de stakeholderanalyse, de enquêteanalyse en een samenvatting van trends en inzichten die richting geven aan het PvE.

== Define (week 3–4)

In de Define-fase maken wij de stap van inzichten naar een scherp afgebakende opdracht. We formuleren de design challenge en leggen requirements vast in het PvE, inclusief MoSCoW-prioritering. In dezelfde fase definiëren we het MVP, zodat vanaf het begin duidelijk is wat minimaal nodig is om een voldoende en demonstrabel product te behalen. Ook werken we het hoog-over blokdiagram uit om de systeemarchitectuur op hoofdlijnen vast te leggen. Hiermee creëren we een gedeeld technisch kader dat later verfijnd kan worden.

== Ideate (week 5–6)

Tijdens Ideate genereren en selecteren wij oplossingsrichtingen. We verkennen concepten voor locomotie, besturing, systeemarchitectuur en interactie, en toetsen deze aan haalbaarheid binnen de projectduur. In deze fase voeren wij ook de peer review uit op het PvE en blokdiagram van een ander team; de feedback gebruiken we om onze eigen keuzes te scherpen en ambiguïteit in eisen te verminderen. Het PvA wordt in deze fase afgerond, zodat de realisatiefase start met een helder plan, duidelijke scope en afgesproken werkwijze.

== Prototype – fase 1 (week 7–8)

Prototype fase 1 is gericht op het aantonen van de kernfunctionaliteit. We integreren basis hardware en firmware en realiseren de eerste versie van de inverse kinematics-loop met een eenvoudige looprichting (vooruit). Tegelijk bouwen we de basis draadloze besturing zodat de robothond op een intuïtieve manier bediend kan worden. In deze fase voeren we vooral unit- en integratietesten uit om subsystemen vroeg te valideren en integratieproblemen niet op te sparen tot het einde.

== Test: eerste verificatie/validatie (week 9)

In week 9 ligt de focus op aantoonbaarheid en feedback. We leveren een gedetailleerd blokdiagram op, stellen de lijst met mogelijke extra’s vast en demonstreren het proof-of-concept van het MVP. Het doel is om zowel technisch als gebruikersgericht te toetsen: we verzamelen feedback via observatie en korte interviews, en voeren technische checks uit op basis van testplannen. De uitkomsten vormen input voor fase 2, waarin betrouwbaarheid en geselecteerde uitbreidingen worden doorontwikkeld.

== Prototype – fase 2 (week 10–11)

Prototype fase 2 is de ‘verstevigingsfase’. Problemen die in de eerste testronde naar voren komen worden waar mogelijk opgelost. Wanneer iets niet binnen de projectduur repareerbaar is, documenteren we het probleem inclusief analyse en mogelijke oplossingsrichting, zodat de beoordeling ziet dat het issue is begrepen en beheerst. Daarna integreren we de belangrijkste feedback en realiseren we, afhankelijk van MoSCoW, één of meer extra functionaliteiten die de demonstratiewaarde vergroten zonder de kernrobustheid te ondermijnen.

== Test: eindverificatie/validatie (week 12–13)

In de eindtestfase verifiëren we dat het systeem aantoonbaar aan de eisen voldoet en valideren we dat het product aansluit bij de verwachtingen van de gebruiker. We voeren systeemtesten uit, organiseren eindgebruikertesten en leggen de resultaten vast in testrapporten. Het uitgangspunt is dat iedere kritieke eis uit het PvE gekoppeld is aan een test, zodat de beoordeling niet afhankelijk is van ‘het leek te werken’, maar gebaseerd is op reproduceerbare metingen en duidelijke acceptatiecriteria.

== Afronding en reflectie (week 14–16)

In de afrondende weken werken wij de documentatie bij, leveren we het projectportfolio en bereiden we de demonstratie voor. Tijdens het assessment presenteren we het product en onderbouwen we gemaakte keuzes en resultaten. Tot slot reflecteren we op het proces, de samenwerking en de leerdoelen, zodat duidelijk wordt welke competenties zijn ontwikkeld en hoe Design Thinking in de praktijk is toegepast.

= Projectgrenzen (scope)

Binnen de scope vallen alle activiteiten die nodig zijn om het MVP te realiseren en aantoonbaar te maken. Dat omvat de ontwikkeling van beweging met inverse kinematics, embedded firmware en regelsystemen, draadloze communicatie, en het opstellen en uitvoeren van testplannen met bijbehorende testrapporten. Ook hoort hierbij de volledige documentatie en traceerbaarheid, zodat het eindresultaat toetsbaar is.

Buiten de scope vallen activiteiten die onevenredig veel integratierisico opleveren of niet nodig zijn voor de demonstratiedoelen, zoals cloudintegraties, smartphone-apps, geavanceerde LIDAR/SLAM, certificeringstrajecten en meermaandige duurtesten.

= Tussenresultaten en mijlpalen

Wij leveren tussentijds op om voortgang aantoonbaar te maken en om tijdig feedback te kunnen verwerken. In de Define-fase leveren we het PvE en een hoog-over blokdiagram op. In de Ideate-fase ronden we dit PvA af. Na prototype fase 1 volgt een proof-of-concept demo van het MVP, waarna in week 9 een gedetailleerd blokdiagram en de lijst met extra’s worden opgeleverd. Het project eindigt met de einddemonstratie en volledige documentatie tijdens het assessment.

= Kwaliteit en borging

Kwaliteit borgen wij door structurele feedbackmomenten, technische reviews en een expliciet bewijs-spoor van eis naar testresultaat. Wekelijks bespreken we voortgang en knelpunten in teamoverleg en in tutor-gesprekken; besluiten en acties worden vastgelegd in notulen. Waar nodig plannen we extra hardware- of software-reviews om risico’s vroeg te signaleren.

Technisch borgen wij kwaliteit via consistente code- en documentatiestandaarden, pull requests en code reviews, en het ontwerpen met testbaarheid in gedachten. Voor hardware documenteren we componentkeuzes met datasheets, testen we prototypes volgens testplannen en voeren we veiligheidschecks uit op ESD-, thermische en mechanische aspecten. Voor documentatie geldt dat belangrijke beslissingen actueel blijven, dat het Design Thinking-proces aantoonbaar terugkomt in de structuur, en dat de traceerbaarheid (eis → ontwerp → testplan → testrapport) expliciet wordt bijgehouden.

= Bronnen en begeleiding

== Bronnen

Wij gebruiken bij voorkeur primaire en erkende bronnen (datasheets, handleidingen, wetenschappelijke artikelen, vendor docs). Niet-erkende bronnen worden alleen gebruikt ter oriëntatie en nooit als enige onderbouwing.

== Begeleiding door ervaringsdeskundigen

Voor specifieke vragen (embedded, regelsystemen, PCB, AI) schakelen wij docenten/tutor in om keuzes te valideren en tijdverlies te beperken.

= Projectorganisatie en communicatie

Onze projectorganisatie is gericht op snelle communicatie, duidelijke eigenaarschap en minimale integratieverrassingen. Rollen zijn vastgelegd om verantwoordelijkheid te verdelen, maar we delen kennis actief om de afhankelijkheid van één persoon te beperken. In de praktijk betekent dit dat ontwerpkeuzes en setups reproduceerbaar worden vastgelegd en dat taken zodanig worden toegewezen dat parallelle voortgang mogelijk is.

We plannen vaste overlegmomenten voor teamafstemming en tutorbegeleiding. Communicatie verloopt primair via Discord voor projectinhoud en snelle afstemming, en secundair via WhatsApp voor korte, tijdkritische berichten. Bestanden en versiebeheer worden beheerd via Git (bijvoorbeeld GitHub/GitButler) en aangevuld met OneDrive/Typst voor documentatie. Voor taakbeheer en uren/voortgang gebruiken wij Notion en waar nodig een tijdregistratietool.

Escalatie gebeurt in stappen: problemen worden eerst besproken met de verantwoordelijke, vervolgens met de projectleider, en bij kritieke issues met tutor of docent. Besluitvorming volgt dezelfde lijn: routinebeslissingen worden binnen de rol genomen en gedeeld met het team; beslissingen met impact op planning of architectuur worden in teamoverleg besproken en vastgelegd met rationale.

= Planning

De globale planning volgt de Embedded Systems tijdlijn en is gekoppeld aan de Design Thinking-fasen. We starten met Empathize in week 46–47, gevolgd door Define in week 48–49. In week 50–51 (Ideate) voeren we de peer review uit en leveren we het PvA in. Daarna volgt prototype fase 1 (week 2–3) en de eerste verificatie/validatie in week 6, inclusief proof-of-concept demo, gedetailleerd blokdiagram en de lijst met extra’s. Prototype fase 2 loopt in week 7–8 en de eindverificatie/validatie in week 10–11. Vervolgens ronden we af in week 12–13 en volgt het assessment in week 14.

Naast deze globale lijn houden wij een detailplanning bij in Notion. Daarin delen we taken top-down op naar weekniveau, met per taak een eigenaar en een duidelijke ‘Definition of Done’. Hierdoor kunnen we voortgang objectief volgen en tijdig bijsturen bij vertraging of scopewijziging.

= Risicoanalyse

Het project kent technische, team- en planningsrisico’s. Technisch kan de complexiteit van integratie (hardware, firmware, regeltechniek en communicatie) leiden tot onverwachte problemen als interfaces niet vroeg worden vastgelegd. Daarom prototypen we vroeg, integreren we in kleine stappen en testen we subsystemen apart voordat we naar systeemniveau gaan. Performance- en energieproblemen mitigeren we door te meten en iteratief te optimaliseren in plaats van te gokken.

Ook mechanische betrouwbaarheid vormt een risico, zeker bij aandrijvingscomponenten. Daarom gebruiken we testopstellingen en duurtesten om slijtage, drift en thermisch gedrag vroeg te signaleren, en leggen we bevindingen vast zodat keuzes onderbouwd blijven.

Teamrisico’s zoals workload-imbalance en communicatieproblemen mitigeren we met vaste overlegmomenten, duidelijke rolverdeling, en het consequent bijhouden van notulen en logboek. Bij onverwachte afwezigheid beperken we impact door cross-training en door setups en procedures reproduceerbaar te documenteren.

Planningsrisico’s zitten vooral in leververtragingen en scope creep. Leververtragingen beperken we door vroeg te bestellen en alternatieven paraat te hebben. Scope creep beperken we door het PvE en de MoSCoW-prioritering als harde referentie te gebruiken en wijzigingen alleen door te voeren na expliciete afweging van impact op tijd, risico en demonstratiewaarde.

= Conclusie

Dit PvA beschrijft een gestructureerde, traceerbare en gebruikersgerichte aanpak om de robothond als embedded demonstrator te realiseren. Door de opzet van een klassiek PvA (duidelijk voor beoordelaars) te combineren met Design Thinking (DBE-eis), borgen wij zowel projectbeheersing als iteratieve ontwikkeling richting een robuust, demonstrabel eindproduct.


#pagebreak()
#pagebreak()


= Gegenereerd door Daan:

= Opdrachtnemer 
Perijn Huijser, Daan Smit en Ruben van der Veen zijn tweedejaars studenten van NHL Stenden Leeuwarden. Hierbij hebben zij in de tweede en derde periode de opdracht gekregen om een interactieve demonstratie te ontwerpen en te realiseren voor de open dag van de opleiding Elektrotechniek, met bijbehorende verslaglegging. Dit project is ter uitbreiding van de kennis van de studenten binnen de opleiding Elektrotechniek, met focus op embedded systems en robotica.

= Opdrachtgever
Rieno Moedt is docent bij de opleiding Elektrotechniek, hij is voor dit project de 
opdrachtgever. Mochten er tijdens het project vragen zijn met betrekken tot de eventuele 
becijfering kunnen wij bij de docent terecht. 

= Tutor
Voor dit project hebben wij Chel-Mari Spies (docent Elektrotechniek) aangewezen gekregen als 
tutor. Mochten de studenten tijdens het project vragen hebben over hoe zij eventuele 
problemen het beste kunnen aanvliegen,  dan kunnen zij bij de tutor terecht.  

= Projectomschrijving
Voor dit project moeten de studenten gezamenlijk een interactieve demonstratie ontwikkelen voor de open dag van de opleiding Elektrotechniek, met nadruk op embedded systems. Tijdens de zestien geplande weken wordt een werkende opstelling gerealiseerd in de vorm van een robothond, die op een betrouwbare en inzichtelijke manier de mogelijkheden van embedded technieken laat zien. De bijbehorende handleiding en projectrichtlijnen worden als leidraad gebruikt. Aan het einde van het project beoordeelt de opdrachtgever de studenten op het behaalde eindresultaat en de geschiktheid van de demonstratie voor de open dag.

= Project resultaat 
In de onderstaande paragrafen wordt de uitvoering omschreven van het project. Zodra het project is geslaagd, demonstreren de studenten de werking van de gerealiseerde robothond tijdens de beoordeling en de open dag van de opleiding Elektrotechniek.

= Probleemstelling
Binnen het tweedejaarsproject wordt een robothond ontwikkeld waarin een regelsysteem, AI-functionaliteit en een bestaand real-time operating system worden toegepast; er is momenteel geen werkend prototype, terwijl in 8 weken een functionerend minimal viable product (proof of concept) en in 16 weken een afgerond prototype met bijbehorende opleveringen gerealiseerd moeten worden, onder de randvoorwaarde van beperkte projecturen naast reguliere lessen.

= Doelstelling
Binnen 8 weken een werkend Minimum Viable Product (proof of concept) van de robothond demonstreren dat onder een bestaand RTOS draait, een basisregelsysteem en minimale AI-functionaliteit integreert, en voldoet aan het eigen Pakket van Eisen; en dit in 16 weken doorontwikkelen tot een high‑fidelity prototype met volledige documentatie, verificatie/validatie en een demonstratie geschikt voor open dagen en assessment.

= Hoofdvraag
Hoe kunnen wij als tweedejaars elektrotechniek studenten een volledig functionele en interactieve robothond ontwikkelen die gebruikmaakt van geavanceerde embedded systems, voldoet aan alle technische eisen (waaronder PSoC5 microcontroller, RTOS, HDL-componenten en draadloze communicatie), en geschikt is als aantrekkelijke demonstrator op de open dag van de opleiding Elektrotechniek aan NHL Stenden om toekomstige studenten te enthousiasmeren voor de opleiding?

= Deelvragen
Om de hoofdvraag volledig te kunnen beantwoorden, is deze opgesplitst in acht deelvragen die gezamenlijk alle aspecten van het project dekken. Deze deelvragen zijn onderverdeeld in drie categorieën: technische deelvragen die zich richten op de embedded systems en bewegingstechnologie, gebruikers- en stakeholder deelvragen die ingaan op de functionaliteit en gebruikservaring tijdens de open dag, en proces deelvragen die de ontwikkelmethodiek en kwaliteitsborging behandelen. Door systematisch elk van deze deelvragen te onderzoeken en te beantwoorden, kunnen we stapsgewijs tot een volledig functionele robothond komen die voldoet aan alle gestelde eisen en wensen van de stakeholders.

== Technische Deelvragen
+ Hoe kan inverse kinematica worden toegepast om vloeiende en stabiele vierpoot-bewegingen te realiseren?
+ Welke embedded architectuur met PSoC5, RTOS en HDL-componenten is het meest geschikt voor real-time besturing van de robothond?
+ Hoe kunnen draadloze communicatieprotocollen (Bluetooth) worden geïmplementeerd voor betrouwbare besturing via een controller?
+ Welke energiebesparingsstrategieën kunnen worden toegepast om de robothond geschikt te maken voor langdurige demonstraties?

== Gebruikers- en Stakeholder *Deelvragen*
+ Welke functionaliteiten (beweging, geluid, interactie) zijn het meest effectief om toekomstige studenten en hun ouders te enthousiasmeren tijdens de open dag?
+ Hoe kan de robothond een herkenbare persoonlijkheid krijgen door combinatie van bewegingen en audio-feedback?

== Proces Deelvragen
+ Hoe kan het Design Thinking proces worden toegepast om iteratief te komen tot een werkend prototype binnen de beschikbare 16 weken?
+ Welke testmethoden zijn nodig om de betrouwbaarheid van mechanica, hardware en software te waarborgen voor gebruik op de open dag?

#pagebreak()

= Afbakening
Dit project richt zich op de ontwikkeling van een werkende robothond demonstrator binnen de gestelde projectperiode van 16 weken (periode 2 en 3). De volgende afbakeningen zijn van toepassing.

== Technische Afbakening
Het project beperkt zich tot een vierpoot robot met twaalf servo motoren (drie per poot) voor bewegingsfuncties. De embedded architectuur is gebonden aan minimaal één PSoC5 microcontroller als primaire controller, aangevuld met verplichte implementatie van een RTOS en minimaal één HDL-component in Verilog. Voor draadloze communicatie ligt de focus op Bluetooth besturing via een Xbox of PlayStation controller, waarbij andere protocollen zoals ExpressLRS en GPS tot de wensen behoren maar geen prioriteit hebben.

== Functionele Afbakening
De robothond moet minimaal kunnen lopen (vooruit, achteruit, zijwaarts) binnen een tolerantie van ±50 centimeter tot 1 meter, en 360 graden kunnen draaien. Geavanceerde AI-functies zoals autonome navigatie door het schoolgebouw, omgevingsherkenning en objectdetectie vallen onder "Could have" of "Won't have" prioriteiten en worden alleen geïmplementeerd als er voldoende tijd beschikbaar is na realisatie van het Minimum Viable Product. Extra modules zoals een koffieschenksysteem, FPV-camera en soundboard zijn wensen en geen verplichte onderdelen.

== Projectmatige Afbakening
Het eindproduct moet geschikt zijn voor demonstratie op open dagen en niet voor commerciële toepassing of langdurige buitengebruik. De totale projectduur strekt zich uit over periode 2 en 3, met een assessment en demonstratie gepland in week 14. Het budget voor materialen is gelimiteerd tot ongeveer €674, en het team bestaat uit drie studenten met vooraf gedefinieerde rollen.

= Theoretisch Kader
Voor de realisatie van de robothond worden verschillende technische concepten en theorieën toegepast die tijdens de opleiding Elektrotechniek zijn behandeld. Dit hoofdstuk beschrijft de belangrijkste theoretische grondslagen die ten grondslag liggen aan het ontwerp.

== Inverse Kinematica (IK)


== Real-Time Operating System (RTOS)


== Hardware Description Language (HDL) in Verilog


== Regeltechniek en PID-controllers


== Regeltechniek en PID-controllers


== Draadloze Communicatieprotocollen


== Design Thinking Methodologie

= Planning

= Risico's