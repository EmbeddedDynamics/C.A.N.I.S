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
    "LiDAR": "Light Detection And Ranging"
  ),
  versions: (
    version(
      committee: [R. van der Veen\ D. Smit],
      description: [Initiële commit],
      date: "2025-12-19",
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
Voor dit project moeten de studenten gezamenlijk een embedded systeem ontwikkelen, als doel voor de komende open dagen van de opleiding Elektrotechniek. Tijdens de zestien geplande wekken wordt een werkend systeem gerealiseerd, in de vorm van een robothond, die op een betrouwbare en inzichtelijke manier de mogelijkheden van embedded technieken laat zien. De bijbehorende handleiding en projectrichtlijnen worden als leidraad gebruikt. Aan het einde van het project beoordeelt de opdrachtgever de studenten op het behaalde eindresultaat en de geschiktheid van de demonstratie voor de open dagen.

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
Om de hoofdvraag makkelijker te kunnen beantwoorden, is deze opgesplitst in acht deelvragen die gezamenlijk alle aspecten van het project dekken.

#v(1em)

=== Technische Deelvragen
+ Welke rol speelt de #acr("PSoC5") in de systeemarchitectuur? 
+ Hoe kan de beweging worden gerealiseerd?
+ Welke transmissie systemen kunnen er toegepast worden voor robot actuatoren?
+ Hoe kunnen draadloze communicatieprotocollen worden toegepast zoals Bluetooth?
+ Welke toepassingen kunnen gebruikt worden om het systeem energiezuinig te maken?
+ Hoe kan de #acr("PSoC5") zo efficiënt mogelijk geprogrammeerd worden?
+ Hoe kan #acr("RTOS") toegepast worden in het project?
+ Op welke manier kan Verilog toegepast worden in het project? 

#v(1em)

=== Proces Deelvragen
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

In de Define-fase maken wij de stap van inzichten naar een scherp afgebakende opdracht. We formuleren de ontwerp uitdaging en leggen wij de eisen vast in het #acr("PvE"), inclusief MoSCoW-prioritering. In dezelfde fase definiëren we het #acr("MVP"), zodat vanaf het begin duidelijk is wat minimaal nodig is om een voldoende en demonstreerbaar product te behalen. Ook werken we het hoog-over blokdiagram uit om de systeemarchitectuur op hoofdlijnen vast te leggen. Hiermee creëren we een gedeeld technisch kader dat later verfijnd kan worden.

#pagebreak()

== Ideate

Tijdens de Ideate-fase verzinnen en selecteren wij verschillende oplossingen op de hoofdvraag en deelvragen. We verkennen concepten voor de besturing, systeemarchitectuur, mechanische aandrijving, energiezuinigheid en extra functies. Er wordt een literatuuronderzoek geschreven voor deze verschillende onderdelen van de robothond, die ook vastgelegd zijn in het blokschema. Bij dit onderzoek zijn er ook #acr("PoC") gerealiseerd van de aandrijving en de #acr("IK"). In deze fase voeren wij ook de peer review uit op het #acr("PvE") en blokdiagram van het andere team. Uit de feedback die wij ontvangen gaan wij onze eigen eisen verbeteren. Het #acr("PvA") wordt in deze fase afgerond, zodat de Prototype-fase wordt gestart met een helder plan, duidelijke scope en afgesproken werkwijze. 

#v(1em)

== Prototype – fase 1

In de eerste Prototype-fase verschuift de focus naar het aantonen van de kernfunctionaliteit. We starten met het bouwen en testen van een verbeterde versie van de Capstan-drive en #acr("IK"). Daarnaast wordt er aandacht besteed aan het zelfgeschreven protocol en het toepassen van #acr("RTOS") op de #acr("PSoC5"). Als deze onderdelen goed functioneren, gaan we beginnen met het realiseren van een robotpoot-prototype en werken we naar een compleet prototype waarin de essentiële keten samenkomt: basisbeweging, aansturing, en een werkend pad richting het #acr("MVP"). In deze fase integreren wij hardware en firmware stapsgewijs en voeren we vooral unit- en integratietesten uit om fouten vroeg te isoleren. 

#v(1em)

== Test: eerste verificatie/validatie 

In de eerste Test-fase plannen wij de eerste verificatie en validatie in de vorm van de demo van het Minimum Viable Product. Deze dient als een #acr("PoC") van het gehele product. Dit moment is bedoeld om aantoonbaar te maken dat de gekozen aanpak werkt en om feedback op te halen over bediening, stabiliteit en demonstratiewaarde. Er wordt met een uitgebreid testplan en rapport gevalideerd waaraan het prototype wel, of juist niet, voldoet. Deze resultaten vormen de input voor de verdere afbakening van uitbreidingen en voor het verhogen van de betrouwbaarheid en functionaliteit in Prototype - fase 2.

Direct na het testen van deze #acr("PoC") volgt een korte beoordeling waarin wij, op basis van de demo-inzichten, de volgende documentatie opleveren: het gedetailleerde blokdiagram en de lijst met extra’s.

#v(1em)
#pagebreak()

== Prototype – fase 2 

In de tweede Prototype-fase werken wij aan het verhogen van reproduceerbaarheid en het implementeren van extra functionaliteiten. Problemen die in de eerste testronde naar voren komen worden waar mogelijk opgelost. Wanneer iets niet binnen de projectduur op te lossen is, documenteren we het probleem inclusief analyse en oplossingsrichting. In deze periode passen wij de extra functies toe en werken we toe naar een compleet prototype.

#v(1em)

== Test: eindverificatie/validatie 

In de tweede Test-fase voeren wij de eindverificatie en -validatie uit. Hierbij toetsen we het systeem aantoonbaar aan de kritieke #acr("PvE") en leggen we resultaten vast in testrapporten aan de hand van een testplan. Het uitgangspunt is dat de eindbeoordeling aantoont dat het product reproduceerbare acties uitvoert en voldoet aan duidelijk geformuleerde acceptatiecriteria.

#v(1em)

== Afronding en reflectie 

In de laatste weken van het project wordt de documentatie bijgewerkt en leveren wij het projectportfolio in. Verder bereiden wij de demonstratie voor richting het assessment. Tijdens het assessment presenteren wij het product en onderbouwen we de keuzes die zijn gemaakt in het project. Het is belangrijk dat elk teamlid begrijpt wat iedereen heeft gedaan. Zo moet iedereen keuzes en de werking kunnen uitleggen. Hier ligt het belang van goede documentatie zodat ieder teamlid snel kan opzoeken naar de uitleg en werking van een onderdeel. Als laatst moet de projectdocumentatie worden ingeleverd. Gedurende het project wordt hier aan gewerkt zodat aan het eind alle informatie is vastgelegd. 

Tot slot reflecteren we op het proces. Dit houdt in hoe de samenwerking verlopen is, of individuele leerdoelen zijn gehaald en hoe het Design-Thinking proces is verlopen. 

#v(1em)
#pagebreak()

= Planning
Deze planning geeft weer hoe wij onze aanpak in de 16 weken gaan realiseren.
#gantt(planning)

#pagebreak()
#set page(flipped: false)


= Projectgrenzen

Binnen de scope vallen alle activiteiten die nodig zijn om het #acr("MVP") te realiseren en aantoonbaar te maken. Dat omvat de ontwikkeling van beweging met #acr("IK"), embedded firmware en regelsystemen, draadloze communicatie, en het opstellen en uitvoeren van testplannen met bijbehorende testrapporten. Ook hoort hierbij de volledige documentatie en traceerbaarheid, zodat het eindresultaat toetsbaar is.

Buiten de scope vallen activiteiten die te veel risico geven of niet nodig zijn voor de demonstratie op de open dag, zoals het koppelen met de cloud, het maken van smartphone-apps, geavanceerde navigatiesystemen met #acr("LiDAR"), officiële certificeringen en langdurige tests van meerdere maanden.

#pagebreak()

= Tussenresultaten en mijlpalen

Wij leveren tussentijdse resultaten op om voortgang aantoonbaar te maken en om tijdig feedback te kunnen verwerken. In de Define-fase leveren we het #acr("PvE") en een hoog-over blokdiagram op. In de Ideate-fase ronden we dit #acr("PvA") af. Na prototype fase 1 volgt een #acr("PoC") demo van het #acr("MVP"), waarna in week 9 een gedetailleerd blokdiagram en de lijst met extra’s worden opgeleverd. Het project eindigt met de einddemonstratie en volledige documentatie tijdens het assessment.

#pagebreak()

= Kwaliteit en waarborging


Er wordt gezorgd voor kwaliteit door regelmatig feedback te vragen, technische onderdelen te controleren en bij te houden hoe elke eis wordt getest. Elke week bespreken we de voortgang en eventuele problemen tijdens teamvergaderingen en tutor-gesprekken, waarbij alle afspraken en acties in notulen worden vastgelegd. Indien nodig plannen we extra controles van de hardware en software om mogelijke problemen vroegtijdig te ontdekken en op te lossen

Technisch borgen wij kwaliteit via consistente code- en documentatiestandaarden, pull requests en code reviews, en het ontwerpen met testbaarheid in gedachten. Voor hardware documenteren we componentkeuzes met datasheets en testen we prototypes volgens testplannen. Voor documentatie geldt dat belangrijke beslissingen actueel blijven, dat het Design Thinking-proces aantoonbaar terugkomt in de structuur, en dat de traceerbaarheid wordt bijgehouden.

#pagebreak()

= Risicoanalyse
Het project heeft technische en team gerelateerde risico's. Technisch kan de complexiteit van het project zorgen voor onverwachte problemen. Het is hierdoor van belang om vroeg te beginnen met prototypen zodat vroegtijdig problemen op te sporen. Het integreren van elk systeem moet in kleine stappen gebeuren om te voor zorgen dat er geen fouten worden gemaakt door haast. 

Betrouwbaarheid van het mechanische deel vormt een risico. Bij de aandrijvingscomponenten zijn er risico's zoals slijtage en drift. Hierom zal er getest worden met duurtesten om vooraf te zien hoe het component zich gedraagt en of er ontwerpaanpassingen nodig zijn. 

Teamrisico's zoals communicatieproblemen worden geminimaliseerd door vaste overlegmomenten te hebben. Verder is er een duidelijke rolverdeling zodat iedereen weet wat zijn taak is. Het bijhouden van een urenverantwoording en een logboek is van belang om de werklastverdeling te monitoren en ervoor te zorgen dat alle teamleden evenredig bijdragen aan het project.

Planningsrisico’s ontstaan vooral wanneer producten later worden opgeleverd dan gepland. Als één onderdeel vertraging oploopt, kan dit het hele project ophouden. Daarom is het belangrijk dat de planning strak wordt gevolgd en dat afwijkingen zo snel mogelijk worden besproken en bijgestuurd.

#v(1em)
#pagebreak()

= Conclusie

Dit plan van aanpak zorgt voor een systematische aanpak voor het ontwikkelen van een embedded systeem in de vorm van een robothond. Het heeft een #acr("PSoC5") als centrale microcontroller en focus op regeltechniek, #acr("RTOS") en draadloze communicatie.  

De aanpak is gefocust op het design-thinking proces. Dit zorgt ervoor dat er gestructureerd gewerkt wordt. Door vroegtijdig te prototypen en uitvoerig te testen aan de hand van concrete testplannen wordt de technische haalbaarheid steeds getest. 

De combinatie van duidelijke projectgrenzen, risicobeheersing en kwaliteitswaarborging via wekelijkse reviews en versiebeheer creëert een sterke basis om het project te laten slagen. Met vaste overlegmomenten en duidelijke rolverdeling wordt zowel de technische als procesmatige kwaliteit geborgd. Het #acr("PvA") zorgt voor een betrouwbaar document voor het ontwikkelen van de robothond. 

]

#PvA_body()