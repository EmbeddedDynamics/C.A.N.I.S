// ============================================
// ELRS research paper
// ============================================

#set page(numbering: "1")

#align(center)[
  = ExpressLRS
  \ 
  Naam: Daan Smit
  
  Datum: 30-3-2026
]

#pagebreak()

= Inhoudsopgave
#outline()

#pagebreak()

= Inleiding
In dit document word de algemene werking van het expressLRS uitgelegd en waarom dit wordt toegepast in de robothond.

#pagebreak()

= Basisprincipe
ExpressLRS is een lange afstand radio protocol met een erg lage vertraging. Het gebruikt LoRa modulatie voor de communicatie. Hierdoor is het erg snel, open source en erg lange afstand. 

= Frequentie en kanalen
ExpressLRS werkt normaal op 2.4 GHz en 868 MHz. 

= Ontvanger
Voor het project willen we een PWM ontvanger gebruiken. De onvanger zet de LoRa signalen om naar PWM. De werking begint bij het ontvangen van de ELRS signalen. De ontvanger luistert continu naar de door de zender uitgezonden data die via een LoRa modulatie wordt verstuurd. Deze signalen bevatten informatie over de positie van de kanalen. Zodra de ontvanger deze signalen ontvangt decodeert hij de data en zet hij deze om naar kanaalwaarden meestal tussen 1000 en 2000 microseconden waarbij 1500 de middelste positie is. Bij een PWM ontvanger wordt vervolgens voor elk kanaal een PWM pulse uitgezonden op een aparte pin. De frequentie van deze pulsen ligt meestal rond de 50 Hz wat neerkomt op een periode van 20 milliseconden en de pulsbreedte varieert tussen 1 milliseconde (minimum), 1,5 milliseconde (midden) en 2 milliseconden (maximum). Een belangrijk kenmerk van PWM ELRS ontvangers is dat ze een zeer lage vertraging hebben ondanks de ontvanger eerst de ELRS pakketten moet decoderen voordat de PWM signalen worden uitgezonden blijft de reactietijd doorgaans rond de 10 tot 15 milliseconden. Tot slot wordt de ontvanger van stroom voorzien via een 5V.

= Waarom ExpressLRS gebruiken
Omdat het makkelijk is om de PWM waarden te gebruiken en we de robothond op lange afstand kunnen gebruiken. Verder had Daan al een ELRS PWM ontvanger liggen dus het ligt voor de hand om dit te gebruiken. 