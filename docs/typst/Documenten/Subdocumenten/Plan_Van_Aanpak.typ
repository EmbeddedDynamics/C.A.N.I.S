#import "@preview/gantty:0.5.1": gantt

#import "@local/academic-tools:0.1.10": *

#import "/Documenten/template.typ": *
#import "/Documenten/Config/GeneralConfig.typ": *
#import "/Documenten/Config/Planning.typ": *

#set text(lang: "Nl")

#show link: underline

//---------------------------------------
// Front page and general definitions
//---------------------------------------
#show: academic-frontpage.with(
  title: [Embedded Systems Project\ Plan van Aanpak],
  authors: students.map(s => s.name).join(", "),
  degree: [#degree],
  degree-goal: [Plan van Aanpak voor het embedded systems project 2025-2026],
  department: [#department],
  university: [#university, #universitydesc],
  program-type: [#program],
  degree-year: [#year],
  location: [#city, #country],
  supervisor: [#supervisor],
  tutor: [#tutor],
  logo: "/Documenten/Assets/NHL_logo.jpg",
  project-logo: none,
  //month: [#month],
  //year: [#year],
  date: datetime.today(),
  abstract: none,
  keywords: none,
  acknowledgments: none,
  acronyms: (
    "PvE": ("Pakket van Eisen","Pakket van Eisen"),
    "MVP": ("Minimum Viable Product","Minimum Viable Product"),
    "PvA": ("Plan van Aanpak","Plan van Aanpak"),
    "IK": ("Inverse Kinematics"),
    "PSoC5": ("Programmable System on a Chip 5"),
    "RTOS": ("Real-Time Operating System"),
    "MoSCoW": ("Must have, Should have, Could have, Won’t have"),
    "PoC": ("Proof of Concept"),
    "LiDAR": ("Light Detection And Ranging"),
    "ELRS": ("Express Long Range System")
  ),
  versions: (
    version(
      committee: [R. van der Veen\ D. Smit],
      description: [Initiële commit],
      date: "2025-12-19",
      level: 3,
    ),
    version(
      committee: [R. van der Veen],
      description: [Subtaken van de planning toegelicht.\ De risico-analyse voorzien van plannen op basis van haalbaarheid.],
      date: "2026-02-03",
      level: 1,
    ),
    version(
      committee: [R. van der Veen],
      description: [Kleine verbeteringen aangebracht. Goedgekeurd door D. Smit en P. Huijser.],
      date: "2026-02-20",
      level: 3,
    ),
  )
) 

//---------------------------------------
// Document contents
//---------------------------------------
#let PvA_body() = [
= Project achtergrond

#v(1em)

== Opdrachtnemer 
Perijn Huijser, Daan Smit en Ruben van der Veen zijn tweedejaars studenten van NHL Stenden Leeuwarden. Hierbij hebben zij in de tweede en derde periode de opdracht gekregen om een interactieve demonstratie te ontwerpen en te realiseren voor de open dag van de opleiding Elektrotechniek, met bijbehorende verslaglegging. Dit project is ter uitbreiding van de kennis van de studenten binnen de opleiding Elektrotechniek, met focus op embedded systems.

#v(1em)

== Opdrachtgever
Het project wordt geleid door R. Moedt, hij is een docent bij de opleiding Elektrotechniek en beoordeelt onze werk. Daarnaast is R. Moedt ook één van onze belangrijkste stakeholders voor dit project.

#v(1em)

== Tutor
Voor dit project is Chel-Mari Spies aangewezen als tutor. Zij begeleidt het proces en houdt de voortgang in de gaten door middel van wekelijkse tutor gesprekken. Indien de studenten vragen hebben over eventuele problemen, kunnen zij bij de tutor terecht.

#v(1em)

== Projectomschrijving
Voor dit project moeten de studenten gezamenlijk een embedded systeem ontwikkelen, als doel voor de komende open dagen van de opleiding Elektrotechniek. Tijdens de zestien geplande weken wordt een werkend systeem gerealiseerd, in de vorm van een robothond, die op een betrouwbare en inzichtelijke manier de mogelijkheden van embedded technieken laat zien. De bijbehorende handleiding en projectrichtlijnen worden als leidraad gebruikt. Aan het einde van het project beoordeelt de opdrachtgever de studenten op het behaalde eindresultaat en de geschiktheid van de demonstratie voor de open dagen.

#pagebreak()


== Doelstelling
Het doel van dit project vanuit NHL Stenden is het ontwikkelen van een werkend embedded systeem. Dit willen wij gaan realiseren door middel van een robothond. Om daar te komen gaan wij systematisch te werk en actief het proces documenteren. Daarnaast willen wij gebruik maken van prototypes en tests om het systeem stap voor stap betrouwbaarder te maken. Ook leggen we keuzes en resultaten goed vast, zodat er per eis terug gezien kan worden wat we hebben ontworpen en hoe we hebben getest. Uiteindelijk willen we dat de robothond tijdens open dagen steeds hetzelfde gedrag laat zien en veilig te bedienen is onder begeleiding.

#v(1em)
#pagebreak()


= Probleemstelling
Het probleem is dat er op het moment nog weinig interactieve demonstrators aanwezig zijn voor de open dagen, om interesse te weken voor het vak gebied embedded systemen. 

#v(1em)

== Hoofdvraag
Op welke wijze kunnen tweedejaars studenten Elektrotechniek een geavanceerd embedded systeem ontwikkelen dat voldoet aan de geldende technische eisen, zodat deze kan worden ingezet als representatieve demonstrator tijdens de open dag van de opleiding Elektrotechniek aan NHL Stenden?

#v(1em)

== Deelvragen
Om de hoofdvraag makkelijker te kunnen beantwoorden, is deze opgesplitst in 11 deelvragen die gezamenlijk alle aspecten van het project dekken.

#v(1em)

#text(size: 15pt)[*Technische Deelvragen*]
+ Welke rol speelt de #acr("PSoC5") in de systeemarchitectuur? 
+ Hoe kan de beweging worden gerealiseerd?
+ Welke transmissie systemen kunnen er toegepast worden voor robot actuatoren?
+ Hoe kunnen draadloze communicatieprotocollen worden toegepast zoals Bluetooth?
+ Welke toepassingen kunnen gebruikt worden om het systeem energiezuinig te maken?
+ Hoe kan de #acr("PSoC5") zo efficiënt mogelijk geprogrammeerd worden?
+ Welke minimale #acr("RTOS")-functionaliteit demonstreren we en hoe testen we dat?
+ Hoe kan Verilog toegepast worden in het project? 

#v(1em)

#text(size: 15pt)[*Proces Deelvragen*]
+ Hoe kan het Design Thinking proces worden toegepast om iteratief te komen tot een werkend prototype?
+ Hoe kunnen wij de stakeholders betrekken bij het proces?
+ Op welke manier wordt er gebruik gemaakt van versiebeheer?

#v(1em)
#pagebreak()


= Aanpak
Voor dit project hanteren wij de Design Thinking methodologie met vijf fasen: Empathize, Define, Ideate, Prototype en Test. Deze iteratieve aanpak stelt ons in staat om gebruikersgericht te ontwerpen en stakeholder-feedback continu te integreren. Door vroeg te prototypen en uitvoerig te testen, identificeren wij op tijd de technische risico's.

#v(1em)

== Empathize 

In de Empathize-fase hebben wij de gebruiker en context in kaart gebracht. Omdat het eindproduct tijdens open dagen ingezet wordt, hebben wij ons in deze fase gericht op wat bezoekers aantrekkelijk vinden. In de praktijk betekende dit het uitvoeren en afronden van de stakeholderanalyse, aangevuld met het verzamelen van input via een enquête. Deze inzichten vormen het fundament voor de latere eisen: we willen voorkomen dat we een technisch knap systeem maken dat voor de doelgroep onduidelijk of oninteressant is.

#v(1em)

== Define 

In de Define-fase maken wij de stap van inzichten naar een scherp afgebakende opdracht. We formuleren de ontwerp uitdaging en leggen wij de eisen vast in het #acr("PvE"), inclusief #acr("MoSCoW")-prioritering. In dezelfde fase definiëren we het #acr("MVP"), zodat vanaf het begin duidelijk is wat minimaal nodig is om een voldoende en demonstreerbaar product te behalen. Ook werken we het hoog-over blokdiagram uit om de systeemarchitectuur op hoofdlijnen vast te leggen. Hiermee creëren we een gedeeld technisch kader dat later verfijnd kan worden.

#pagebreak()


== Ideate

Tijdens de Ideate-fase verzinnen en selecteren wij verschillende oplossingen op de hoofdvraag en deelvragen. We verkennen concepten voor de besturing, systeemarchitectuur, mechanische aandrijving, energiezuinigheid en extra functies. Er wordt een literatuuronderzoek geschreven voor deze verschillende onderdelen van de robothond, die ook vastgelegd zijn in het blokschema. Bij dit onderzoek zijn er ook #acr("PoC") gerealiseerd van de aandrijving en de #acr("IK"). In deze fase voeren wij ook de peer review uit op het #acr("PvE") en blokdiagram van het andere team. Uit de feedback die wij ontvangen gaan wij onze eigen eisen verbeteren. Het #acr("PvA") wordt in deze fase afgerond, zodat de Prototype-fase wordt gestart met een helder plan, duidelijke scope en afgesproken werkwijze. 

#v(1em)

== Prototype – fase 1

In de eerste Prototype-fase verschuift de focus naar het aantonen van de kernfunctionaliteit. We starten met het bouwen en testen van een verbeterde versie van de Capstan-drive en #acr("IK"). Daarnaast wordt er aandacht besteed aan het zelfgeschreven protocol en het toepassen van #acr("RTOS") op de #acr("PSoC5"). Als deze onderdelen goed functioneren, gaan we beginnen met het realiseren van een robotpoot-prototype en werken we naar een compleet prototype waarin de essentiële keten samenkomt: basisbeweging, aansturing, en een werkend pad richting het #acr("MVP"). In deze fase integreren wij hardware en firmware stapsgewijs en voeren we vooral unit- en integratietesten uit om fouten vroeg te isoleren. 

#v(1em)

== Test - fase 1: eerste verificatie/validatie 

In de eerste Test-fase plannen wij de eerste verificatie en validatie in de vorm van de demo van het #acr("MVP"). Deze dient als een #acr("PoC") van het gehele product. Dit moment is bedoeld om aantoonbaar te maken dat de gekozen aanpak werkt en om feedback op te halen over bediening, stabiliteit en demonstratiewaarde. Er wordt met een uitgebreid testplan en rapport gevalideerd waaraan het prototype wel, of juist niet, voldoet. Deze resultaten vormen de input voor de verdere afbakening van uitbreidingen en voor het verhogen van de betrouwbaarheid en functionaliteit in Prototype - fase 2.

Direct na het testen van deze #acr("PoC") volgt een korte beoordeling waarin wij, op basis van de demo-inzichten, de volgende documentatie opleveren: het gedetailleerde blokdiagram en de lijst met extra’s.

#v(1em)
#pagebreak()


== Prototype – fase 2 

In de tweede Prototype-fase werken wij aan het verhogen van reproduceerbaarheid en het implementeren van extra functionaliteiten. Problemen die in de eerste testronde naar voren komen worden waar mogelijk opgelost. Wanneer iets niet binnen de projectduur op te lossen is, documenteren we het probleem inclusief analyse en oplossingsrichting. In deze periode passen wij de extra functies toe en werken we toe naar een compleet prototype.

#v(1em)

== Test - fase 2: eindverificatie/validatie 

In de tweede Test-fase voeren wij de eindverificatie en -validatie uit. Hierbij toetsen we het systeem aantoonbaar aan de kritieke #acr("PvE") en leggen we resultaten vast in testrapporten aan de hand van een testplan. Het uitgangspunt is dat de eindbeoordeling aantoont dat het product reproduceerbare acties uitvoert en voldoet aan duidelijk geformuleerde acceptatiecriteria.

#v(1em)

== Afronding en reflectie 

In de laatste weken van het project wordt de documentatie bijgewerkt en leveren wij het projectportfolio in. Verder bereiden wij de demonstratie voor richting het assessment. Tijdens het assessment presenteren wij het product en onderbouwen we de keuzes die zijn gemaakt in het project. Het is belangrijk dat elk teamlid begrijpt wat iedereen heeft gedaan. Zo moet iedereen keuzes en de werking kunnen uitleggen. Hier ligt het belang van goede documentatie zodat ieder teamlid snel kan opzoeken naar de uitleg en werking van een onderdeel. Als laatst moet de projectdocumentatie worden ingeleverd. Gedurende het project wordt hier aan gewerkt zodat aan het eind alle informatie is vastgelegd. 

Tot slot reflecteren we op het proces. Dit houdt in hoe de samenwerking verlopen is, of individuele leerdoelen zijn gehaald en hoe het Design-Thinking proces is verlopen. 

#v(1em)
#pagebreak()

= Planning
Deze planning geeft weer hoe wij onze aanpak in de 16 weken willen gaan realiseren. In de einddocumentatie gaan we reflecteren of we aan deze planning konden houden.\
#text(size: 18pt)[*Overzicht:*]
#gantt(planning)

#pagebreak()


#text(size: 18pt)[*Inhoudelijk*\ ]
#render-task(planning, "Emphatize")
#render-subtasks(planning, "Emphatize")
#render-task(planning, "Define")
#render-subtasks(planning, "Define")

#pagebreak()

#render-task(planning, "Ideate")
#render-subtasks(planning, "Ideate")

#pagebreak()


#render-task(planning, "Prototype – fase 1")
#render-subtasks(planning, "Prototype – fase 1")
#render-task(planning, "Test – fase 1")
#render-subtasks(planning, "Test – fase 1")

#pagebreak()


#render-task(planning, "Prototype – fase 2")
#render-subtasks(planning, "Prototype – fase 2")
#render-task(planning, "Test – fase 2")
#render-subtasks(planning, "Test – fase 2")
#render-task(planning, "Doorlopend")
#render-subtasks(planning, "Doorlopend")

#pagebreak()


= Projectgrenzen

Binnen de scope vallen alle activiteiten die nodig zijn om het #acr("MVP") te realiseren en aantoonbaar te maken. Dat omvat de ontwikkeling van beweging met #acr("IK"), embedded firmware en regelsystemen, draadloze communicatie, en het opstellen en uitvoeren van testplannen met bijbehorende testrapporten. Ook hoort hierbij de volledige documentatie en traceerbaarheid, zodat het eindresultaat toetsbaar is.

Buiten de scope vallen activiteiten die te veel risico geven of niet nodig zijn voor de demonstratie op de open dag, zoals het koppelen met de cloud, het maken van smartphone-apps, geavanceerde navigatiesystemen met #acr("LiDAR"), officiële certificeringen en langdurige tests van meerdere maanden.

#pagebreak()


= Tussenresultaten en mijlpalen

Wij leveren tussentijdse resultaten op om voortgang aantoonbaar te maken en om tijdig feedback te kunnen verwerken. In de Define-fase leveren we het #acr("PvE") en een hoog-over blokdiagram op. In de Ideate-fase ronden we dit #acr("PvA") af. Na prototype fase 1 volgt een #acr("PoC") demo van het #acr("MVP"), waarna in week 9 van het project (02/02/26 - 06/02/26) een gedetailleerd blokdiagram en de lijst met extra’s worden opgeleverd. Het project eindigt met de einddemonstratie en volledige documentatie tijdens het assessment.

#pagebreak()


= Kwaliteit en waarborging

Er wordt gezorgd voor kwaliteit door regelmatig feedback te vragen, technische onderdelen te controleren en bij te houden hoe elke eis wordt getest. Elke week bespreken we de voortgang en eventuele problemen tijdens teamvergaderingen en tutor-gesprekken, waarbij alle afspraken en acties in notulen worden vastgelegd. Indien nodig plannen we extra controles van de hardware en software om mogelijke problemen vroegtijdig te ontdekken en op te lossen

Technisch borgen wij kwaliteit via consistente code- en documentatiestandaarden, pull requests en code reviews, en het ontwerpen met testbaarheid in gedachten. Voor hardware documenteren we componentkeuzes met datasheets en testen we prototypes volgens testplannen. Voor documentatie geldt dat belangrijke beslissingen actueel blijven, dat het Design Thinking-proces aantoonbaar terugkomt in de structuur, en dat de traceerbaarheid wordt bijgehouden.

#pagebreak()


= Risicoanalyse

Het project heeft technische en teamgerelateerde risico's. Technisch kan de complexiteit van het project zorgen voor onverwachte problemen. Het is hierdoor van belang om vroeg te beginnen met prototypen zodat vroegtijdig problemen op te sporen. Het integreren van elk systeem moet in kleine stappen gebeuren om te voor zorgen dat er geen fouten worden gemaakt door haast. 

== De risico's beperken

Betrouwbaarheid van het mechanische deel vormt een risico. Bij de aandrijvingscomponenten zijn er risico's zoals slijtage en drift. Hierom zal er getest worden met duurtesten om vooraf te zien hoe het component zich gedraagt en of er ontwerpaanpassingen nodig zijn. 

Teamrisico's zoals communicatieproblemen worden geminimaliseerd door vaste overlegmomenten te hebben. Verder is er een duidelijke rolverdeling zodat iedereen weet wat zijn taak is. Het bijhouden van een urenverantwoording en een logboek is van belang om de werklastverdeling te monitoren en ervoor te zorgen dat alle teamleden evenredig bijdragen aan het project.

Planningsrisico’s ontstaan vooral wanneer producten later worden opgeleverd dan gepland. Als één onderdeel vertraging oploopt, kan dit het hele project ophouden. Daarom is het belangrijk dat de planning strak wordt gevolgd en dat afwijkingen zo snel mogelijk worden besproken en bijgestuurd.

#pagebreak()


== Het plan op basis van haalbaarheid

Het beperken van de risico's kan niet helemaal het risico vermijden. Er bestaat een kans dat deze risico's invloed hebben op het proces waardoor deadlines niet haalbaar zijn en bepaalde functionaliteit gelimiteerd moet worden. Hiervoor stellen we een aantal plannen afhankelijk van de grootte van deze invloed. Zo kan er bij grote problemen alsnog een product opgeleverd worden.

#text(size: 16pt)[*Plan A:*\ ]
*Doel:* Een robothond met alle functionaliteit benoemd in de #acr("MoSCoW")-analyse onder could-have prioriteiten.\ 
*In scope:* Alle should-have en could-have prioriteiten uit de #acr("MoSCoW")-analyse.\ 
*Niet in scope:* Wont-have prioriteiten uit de #acr("MoSCoW")-analyse.\ 
*Criteria:* Als er geen afwijkingen geweest zijn in de planning en alle should-have en could-have prioriteiten behaald zijn.\ 
*Wanneer niet behaald:* Als er wijzigingen zijn in de planning, en na re-evaluatie besloten is dat niet alle extra functionaliteit onder should-have en could-have prioriteiten behaald kunnen worden.\ 
*Evaluatie moment:* 02/02/26 - 06/02/26\ 
*Validatie moment:* 02/03/26 - 06/03/26\ 
*Vervolg actie:* Schakel naar Plan B.

#pagebreak()


#text(size: 16pt)[*Plan B:*\ ]
*Doel:* Een robothond met een aantal could-have functionaliteit benoemd in de #acr("MoSCoW")-analyse.\ 
*In scope:* Alle should-have prioriteiten uit de #acr("MoSCoW")-analyse en een aantal could-have prioriteiten zoals dansjes, een soundboard, autonomie en #acr("ELRS").\ 
*Niet in scope:* Wont-have prioriteiten en een aantal could-have prioriteiten zoals GPS, en koffie schenk functionaliteit.\ 
*Wanneer behaald:* Als alle should-have met een aantal could-have prioriteiten behaald zijn.\ 
*Wanneer niet behaald:* Als er te veel wijzigingen zijn in de planning, en na re-evaluatie besloten is dat niet voldaan kan worden aan de in scope eisen bij dit plan.\ 
*Evaluatie moment:* 02/02/26 - 06/02/26\ 
*Validatie moment:* 02/03/26 - 06/03/26\ 
*Vervolg actie:* Schakel naar Plan C.

#text(size: 16pt)[*Plan C:*\ ]
*Doel:* Een robothond met alle must-have en should-have functionaliteit uit de #acr("MoSCoW")-analyse, zonder risicovolle could-haves.\ 
*In scope:* Alle must-have en should-have prioriteiten.\ 
*Niet in scope:* Alle could-have en wont-have prioriteiten.\ 
*Wanneer behaald:* Als alle must-have en should-have prioriteiten behaald zijn binnen de geplande integratie- en testweken.\ 
*Wanneer niet behaald:* Als integratie en testen uitlopen of kernfunctionaliteit instabiel blijft, waardoor niet voldaan kan worden aan alle should-haves.\ 
*Evaluatie moment:* 02/02/26 - 06/02/26\ 
*Validatie moment:* 02/03/26 - 06/03/26\ 
*Vervolg actie:* Schakel naar Plan D.

#pagebreak()


#text(size: 16pt)[*Plan D:*\ ]
*Doel:* Een stabiel werkende #acr("MVP") robothond die de kernfunctionaliteit demonstreert en aantoonbaar voldoet aan de must-have eisen uit de #acr("MoSCoW")-analyse.\ 
*In scope:* Alle must-have prioriteiten met de minimale demonstratie mogelijkheid. Zoals een basisbewegingscyclus, basisbediening en minimale documentatie met testbewijs.\ 
*Niet in scope:* Alle should-have, could-have en wont-have prioriteiten.\ 
*Wanneer behaald:* Als alle must-have acceptatiecriteria uit de #acr("MoSCoW")-analyse getest en goedgekeurd zijn en de demonstratie reproduceerbaar is.\ 
*Wanneer niet behaald:* Als mechanica/aansturing instabiel blijft of de poot niet betrouwbaar kan bewegen binnen de beschikbare tijd.\ 
*Evaluatie moment:* 19/01/26 - 23/01/26\ 
*Validatie moment:* 02/02/26 - 06/02/26\ 
*Vervolg actie:* Schakel naar Plan E.

#text(size: 16pt)[*Plan E:*\ ]
*Doel:* Een #acr("PoC") waarmee de meest kritieke technische kern zoals de robotpoot met aandrijving en basisregeling aantoonbaar werkt, ook als de volledige robothond niet haalbaar is.\ 
*In scope:* Een robotpoot volledig functioneel met capstan, encoder, feedback en basisregeling. Een testplan/testrapport en duidelijke integratie-architectuur.\ 
*Niet in scope:* Volledige robothond integratie, extra features, AI/autonomie, meerdere poten.\ 
*Wanneer behaald:* Als de poot herhaalbaar een beweging uitvoert volgens testcriteria, zoals een half uur lang, en dit gedocumenteerd is.\ 
*Wanneer niet behaald:* Als ook de poot niet betrouwbaar functioneert door hardwareproblemen of fundamentele ontwerpkeuzes.\ 
*Evaluatie moment:* 19/01/26 - 23/01/26\ 
*Validatie moment:* 02/02/26 - 06/02/26\  
*Vervolg actie:* Overleggen met de projecteigenaar wat onze vervolg actie moet zijn.

#pagebreak()


= Conclusie

Dit plan van aanpak zorgt voor een systematische aanpak voor het ontwikkelen van een embedded systeem in de vorm van een robothond. Het heeft een #acr("PSoC5") als centrale microcontroller en legt de focus op regeltechniek, #acr("RTOS") en draadloze communicatie.  

De aanpak is gefocust op het design-thinking proces. Dit zorgt ervoor dat er gestructureerd gewerkt wordt. Door vroegtijdig te prototypen en uitvoerig te testen aan de hand van concrete testplannen wordt de technische haalbaarheid steeds getest en onderbouwd. 

De combinatie van duidelijke projectgrenzen, risicobeheersing en kwaliteitsborging via wekelijkse reviews en versiebeheer creëert een sterke basis om het project te laten slagen. Met vaste overlegmomenten en duidelijke rolverdeling wordt zowel de technische als procesmatige kwaliteit geborgd. Het #acr("PvA") zorgt voor een betrouwbaar document voor het ontwikkelen van de robothond. 

]

#PvA_body()