#import "@local/academic-tools:0.1.10": *

#import "../template.typ": *
#import "../Config/GeneralConfig.typ": *


// THIS DOCUMENT DOESNT FUNCTION AS A STANDALONE
// DOCUMENT AND CAN ONLY BE USED IN "Hoofddocument.typ"


// ============================================
// Document contents
// ============================================
#let PvE_body() = [
== Pakket van Eisen
In het #acr("PvE") worden de eisen voor dit project vastgelegd. Deze eisen zijn grofweg te verdelen in twee groepen, eisen van de opdrachtgever en eisen die door het projectteam zelf zijn opgesteld. De verplichte eisen van de opdrachtgever zijn te vinden in @verplichte-eisen. Naast de verplichte eisen zijn de eisen aanvullend geformuleerd door het projectteam.
#v(1em)

=== Verplichte eisen 
+ Het systeem bevat minimaal één #acr("PSoC5") als microcontroller.
+ Het systeem zal gebruik maken van draadloze communicatie of een regeltechniek systeem.
+ Het systeem zal gebruik maken van een #acr("HDL")-component geïmplementeerd in Verilog.
+ Het systeem zal gebruik maken van een #acr("RTOS").
+ Het systeem zal geoptimaliseerd worden voor energiezuinigheid (Zie @energie-eisen).
+ Het systeem zal minimaal één zelfgeschreven libary voor een communicatie protocol gebruiken.
+ Het systeem zal geschikt moeten zijn voor opendagen. (zie @stakeholder-analyse)

#v(1em)
=== Functionele eisen

#v(1em)
==== Bewegingsfuncties
+ De robot zal minimaal 1 meter naar voren kunnen lopen, met een afwijking van ± 50 centimeter.
+ De robot zal minimaal 1 meter naar achteren kunnen lopen, met een afwijking van ± 50 centimeter.
+ De robot zal minimaal 1 meter zijwaarts kunnen lopen, met een afwijking van ± 50 centimeter.
+ De robot zal een extra DOF beschikken in de roll-as.
+ De robot zal 360 graden om zijn eigen as heen kunnen draaien.
+ De robot zal 5 millimeter kunnen springen in de lucht.
+ De robothond zal een eigen persoonlijkheid hebben D.M.V. unieke bewegingen.

#pagebreak()


==== AI en autonomie
+ De robothond zal kunnen praten D.M.V. ingebouwde speakers.
+ De robothond zal geluid kunnen afspelen D.M.V. ingebouwde speakers.
+ De robothond bestuurt de ingebouwde speakers D.M.V. #acr("AI") of ingebouwde bestuurbare muziekbord.
+ De robothond zal een eigen persoonlijkheid hebben D.M.V. unieke spraak.
+ De robothond zal autonoom door de school kunnen lopen D.M.V. #acr("AI") algoritmes zonder menselijke handelingen.

#v(2em)

==== Modulaire functies
+ De robothond zal kunnen voorzien worden van extra externe modules.
+ De robothond zal modulair ontworpen worden met aluminium extrusies voor het bevestigen van externe modules.
+ De robothond zal modulair ontworpen worden met pogo connectoren.

#v(1em)

=== Energie en performance 
+ De robothond zal een high-power modus beschikken die de motoren niet limiteert.
+ De robothond zal een low-power modus beschikken die de motoren limiteert tot 50% vermogen.
+ De robothond beschikt een sleep modus waarbij dit op de grond gaat liggen.
+ De robothond zal een #acr("AI")-limitatie modus hebben die #acr("AI") functies uitzet.

#v(1em)

=== Gebruikersgerichtheid en betrouwbaarheid
+ De robothond zal bestuurbaar zijn via een blauwetand x-doos of speelstation controller
+ De robothond zal een duidelijke fysieke gebruikers interface hebben met labels bij knoppen/schakelaars
+ De robothond zal tijdens werking geen ongewenste of onverwachte bewegingen uitvoeren die afwijken van het gedefinieerde gedrag zoals vastgelegd in het testplan.
+ De robothond zal bruikbaar zijn voor opendagen gebaseerd op de wensen van de stakeholder (Zie @stakeholder-analyse).

#pagebreak()


=== Documentatie
+ Er wordt een #acr("PvE") opgesteld met duidelijke #acr("MoSCoW")-prioriteiten.
+ Er wordt een #acr("PvA") opgesteld met daarin ten minste een planning, rolverdeling en risicoanalyse.  
+ Er zal een portfolio bijgehouden worden met gemaakte keuzes en ontwerpen
+ Alle ontwerpkeuzes worden onderbouwd met berekeningen, argumentatie
 en/of literatuurbronnen.  
+ Alle schema’s, code en andere ontwerpen worden als bijlage toegevoegd en zijn traceerbaar naar de gestelde eisen.  
+ De documentatie zal gestructureerd worden volgens de fasen van Design Thinking Proces, tenzij expliciet anders afgesproken.

#v(1em)

=== Eisen aan het ontwikkelproces
+ Er zal gebruik gemaakt worden van het design thinking proces.
+ Er zal een testplan opgesteld worden voor elk deelproces.
+ Er zullen Blokdiagrammen aanwezig voor verschillende deelprocessen.
+ Er zal een #acr("PoC") gemaakt met één of meer bijhorende prototype.
+ Er wordt een concrete #acr("MVP") gedefinieerd die meetbaar is via één of meerdere testplannen. 
+ Het team houdt één gezamenlijk logboek bij waarin activiteiten, uren en bijdragen per teamlid worden vastgelegd.  
+ Het team voert wekelijks een tutor-gesprek en legt afspraken en besluiten vast in notulen.  
+ Er wordt gebruikgemaakt van versiebeheer (bijvoorbeeld Git of vergelijkbaar).  
+ Er wordt een peer review uitgevoerd op een #acr("PvE") en blokdiagram van de concurrentie.
+ Er worden rollen verdeeld in de groep en duidelijk gedefinieerd wat de verantwoordelijkheden zijn bij deze rol. (@team-structuur)

#pagebreak()


== Pakket van Wensen
Aanleiding stakeholder analyse. Schrijf inleiding

+ De robothond heeft een extern systeem dat koffie kan inschenken D.M.V. #acr("AI") detectie.
+ De robothond beschikt over een ingebouwde soundboard. Hiermee kunnen externe gebruikers met handmatige acties sound effects afspelen D.M.V. een controller.
+ De robothond kan in geprogrammeerde dansjes doen.
+ De robothond beschikt over een #acr("GPS") systeem waardoor die real-time getracked kan worden.
+ De robothond beschikt over het #acr("ELRS") protocol. Hiermee kunnen wij de robothond op een minimale afstand van 1km besturen.
+ De robothond beschikt over een analoge #acr("FPV") camera. Hierdoor kan de gebruiker in het perspectief in de robot hond kijken.

#pagebreak()


== Minimal Viable Product
Voor het project moet er een #acr("MVP") gedefinieerd worden. Dit zijn de eigenschappen die minimaal aanwezig zouden moeten zijn voor een werkend, goed product. In dit hoofdstuk benoemen wij onze eisen voor het #acr("MVP"), dit is nodig om ons project af te ronden met een voldoende.

#v(1em)

=== Beweging
+ De robothond zal minimaal 1 meter naar voren kunnen lopen, met een afwijking van ±1 meter.
+ De robothond zal minimaal 1 meter naar achteren kunnen lopen, met een afwijking van ±1 meter.
+ De robothond beschikt #acr("IK") voor de bewegingen van de robot actuatoren.

#v(1em)

=== Embedded firmware
+ De robothond heeft minimaal een eigen geschreven library voor een communicatie protocol.
+ De robothond beschikt over een #acr("RTOS") die werkt op de #acr("PSoC5") architectuur.

#v(1em)

=== Draadloze communicatie en regelsystemen
+ De robothond zal bestuurbaar zijn via een Bluetooth Xbox of Playstation controller

#v(1em)

=== Mechanica
+ De robothond bevat een betrouwbaar en getest mechanisch frame volgens testplan

]

//#PvE_body()