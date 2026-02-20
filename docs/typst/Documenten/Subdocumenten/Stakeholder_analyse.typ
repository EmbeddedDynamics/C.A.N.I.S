#import "../Config/GeneralConfig.typ": *
#import "../Config/StakeholderConfig.typ": *

== Stakeholder analyse

#v(1em)

=== De Stakeholders <stakeholder-analyse>

*Stakeholder 1: Projecteigenaar* \
Meneer Rieno Moedt is de projecteigenaar van dit project. Hij bepaalt de spelregels en acceptatiecriteria. Verder stelt hij de eisen vast en beoordeeld hij het eindresultaat. Zijn voornaamste belangen zijn een goed werkend #acr("PoC") en high-fidelity prototype, naleving van alle eisen, en een eindproduct dat geschikt is voor open dagen en toekomstige studenten die enthousiast worden gemaakt. Zijn invloed is zeer hoog: hij bepaalt het uiteindelijke cijfer. De strategie is regelmatig, open overleg met duidelijke verwachtingen en directe feedback.

*Stakeholder 2: Leerlingen/Studenten* \
De leerlingen en studenten die het robothond project op de open dag zullen zien, zijn de primaire eindgebruikers. Zij vormen de doelgroep waarvan zij enthousiast moeten worden wanneer zij het product aanraken en gebruiken. Hun belangen zijn duidelijk: zij willen een aantrekkelijke en interactieve demonstratie die hen enthousiast maakt en inzicht geeft in wat elektrotechniek te bieden heeft. Hun invloed is matig tot hoog. De leerlingen/Studenten bepalen via hun ervaringen en feedback of het project succesvol is in het aantrekken van toekomstige studenten, wat indirect van invloed is op de perceptie van de opleiding. De communicatie is vooral op de opendagen zelf. Huidige jaar 1 elektrostudenten kunnen wel ondervraagd worden over wat zij willen zien in het project. De strategie is het product intuïtief maken met een betrouwbare en interactieve werking.

#pagebreak()

*Stakeholder 3: Ouders/Verzorgers* \
De ouders en verzorgers van toekomstige leerlingen die op de open dag komen, zijn indirecte maar belangrijke stakeholders. Zij vormen samen met hun kinderen de doelgroep die zich informeert over de opleiding. Hun belangen zijn inzicht krijgen in wat hun kinderen kunnen leren, het niveau van innovatie en praktische projectwerk aan de school, en of de opleiding aansluit bij hun verwachtingen en hun toekomstbeeld van hun kind. Ze willen zien dat de school studenten voorbereidt op moderne technologie en dat projecten echt werkend zijn, niet alleen theoretisch.
Hun invloed is indirect maar wel erg belangrijk. Positieve indrukken van het robothondproject kunnen een rol spelen in hun inschrijfbeslissing voor de opleiding. Communicatie verloopt via de live demonstratie en korte pitch op de open dag waarbij teamleden duidelijk moeten kunnen uitleggen wat het project doet, hoe het werkt, en welke vaardigheden zij hebben opgebouwd. De strategie is om een leuke technische presentatie te geven van de robothond.  Dit draagt rechtstreeks bij aan het aantrekken van nieuwe studenten en dus de reputatie van de school en opleiding.

*Stakeholder 4: Docenten elektrotechniek* \
De docenten elektrotechniek zijn een belangrijke interne stakeholder groep. Deze groep wilt dat het project van hoge kwaliteit is en goed aansluit op wat wij geleerd hebben. Alle leerstof met betrekking van regeltechniek en embedded wordt hier toegepast. Het project zien zij als een showcase voor onze opleiding. De docenten zijn willen een succesvol eindresultaat. Hun invloed is matig tot hoog. Communicatie verloopt wanneer wij feedback willen hebben van de docenten en wanneer zij meer willen weten wat wij aan het doen zijn. De strategie is om regelmatig vragen te stellen en laten zien wat wij op het moment aan het doen zijn. 

#pagebreak()

*Stakeholder 5: Tutor* \
De tutor is de begeleider die wekelijks met de groep spreekt. De tutor ondersteunt bij planning en voortgang. De tutor verwacht dat wij zelf de afspraken plannen, een agenda opstellen en notulen bijhouden, en dat wij planning en logboek op orde zijn. De invloed van de tutor is hoog in het proces. De communicatiestrategie is structureel een wekelijks overleg met heldere vragen en beslispunten, en tussentijds updates.

*Stakeholder 6: Overige Bezoekers* \
Overige bezoekers op de open dag zoals bedrijven uit de regio en geïnteresseerden vormen een brede, indirecte doelgroep. Hun belang is voornamelijk informatief en reputatiegericht. Zij willen een indruk krijgen van de kwaliteit, innovatie en praktijkgerichtheid van de opleiding.
Hun invloed is laag tot matig. Zij beslissen niet over eisen of cijfers, maar hun feedback dragen bij aan de reputatie van de opleiding en kunnen deuren openen naar contacten of stagekansen. Communicatie gebeurt via de demo en korte uitleg bij de opleiding. De strategie is om te informeren en te inspireren. Dit ondersteunt het opendag doel en de showcase functie.

#pagebreak()

=== Visuele spreiding

#v(1em)

#stakeholder-matrix()
#stakeholder-table()

#pagebreak()

=== Analyse Enquête
Hier worden alle vragen die in de enquête worden gevraagd weergegeven en geanalyseerd.

*Tot welke groep behoort u?*

#let group_items = stakeholdercharts_db.doelgroep.items
#cetz.canvas({
  import cetz.draw: *
  chart.piechart(
    (
      group_items.map(it => (it.name, it.value))
    ),
    value-key: 1,
    label-key: none,
    radius: 3,
    slice-style: group_items.map(it => (fill: COL.at(it.color))),
    inner-label: (
      content: "%",
      radius: 110%,
    ),
    legend: (label: none),
  )
})

#stack(spacing: 3pt)[
  #for it in group_items {
    legend-line(COL.at(it.color), it.value, it.name)
  }
]

*Wat is uw huidige opleidingsniveau?*

#let educationlevel_items = stakeholdercharts_db.educationlevel.items
#cetz.canvas({
  import cetz.draw: *
  chart.piechart(
    (
      educationlevel_items.map(it => (it.name, it.value))
    ),
    value-key: 1,
    label-key: none,
    radius: 3,
    slice-style: educationlevel_items.map(it => (fill: COL.at(it.color))),
    inner-label: (
      content: "%",
      radius: 150%
    ),
  )
})

#stack(spacing: 3pt)[
  #for it in educationlevel_items {
    legend-line(COL.at(it.color), it.value, it.name)
  }
]

#pagebreak()

*Welke eigenschappen maakt een project volgens u het meest aantrekkelijk om te bekijken?*

#let attractiveness_items = stakeholdercharts_db.attractiveness.items
#cetz.canvas({
  import cetz.draw: *
  chart.piechart(
    (
      attractiveness_items.map(it => (it.name, it.value))
    ),
    value-key: 1,
    label-key: none,
    radius: 3,
    slice-style: attractiveness_items.map(it => (fill: COL.at(it.color))),
    inner-label: (
      content: "%",
      radius: 120%,
    ),
    legend: (label: none),
  )
})

#stack(spacing: 3pt)[
  #for it in attractiveness_items {
    legend-line(COL.at(it.color), it.value, it.name)
  }
]

#v(1em)

*Moet het project vooral gericht zijn op vermaak, of op het laten zien van technische vaardigheden?*

#let focus_items = stakeholdercharts_db.focus.items
#cetz.canvas({
  import cetz.draw: *
  chart.piechart(
    (
      focus_items.map(it => (it.name, it.value))
    ),
    value-key: 1,
    label-key: none,
    radius: 3,
    slice-style: focus_items.map(it => (fill: COL.at(it.color))),
    inner-label: (
      content: "%",
      radius: 150%
    ),
    legend: (label: none),
  )
})

#stack(spacing: 3pt)[
  #for it in focus_items {
    legend-line(COL.at(it.color), it.value, it.name)
  }
]

#pagebreak()

*Wat vind u interessant? (Beide antwoorden mogen aangevinkt worden)*

#let interest_items = stakeholdercharts_db.interest.items
#let interest_data = interest_items.map(it => (it.name, it.value))
#cetz.canvas({
  import cetz.draw: *
  chart.barchart(
    mode: "basic",
    size: (10, 4),
    label-key: 0,
    value-key: 1,
    bar-width: 0.7,
    x-tick-step: 5,
    bar-style: i => (fill: COL.at(interest_items.at(i).color)),
    interest_data,
    axes: (
      left: (
        tick: (
          label: (angle: 0, anchor: "east")
        )
      ),
      bottom: (
        min: 0,
        max: 22,
        tick: (step: 1)
      )
    )
  )
})

#v(1em)

*Wat is volgens u het belangrijkste doel van het project dat wij laten zien?*

#let project_goal_items = stakeholdercharts_db.project_goal.items
#cetz.canvas({
  import cetz.draw: *
  chart.piechart(
    (
      project_goal_items.map(it => (it.name, it.value))
    ),
    value-key: 1,
    label-key: none,
    radius: 3,
    slice-style: project_goal_items.map(it => (fill: COL.at(it.color))),
    inner-label: (
      content: "%",
      radius: 150%,
    ),
    legend: (label: none),
  )
})

#stack(spacing: 3pt)[
  #for it in project_goal_items {
    legend-line(COL.at(it.color), it.value, it.name)
  }
]

#pagebreak()

*Wat zou u leuk vinden wat de robothond kan doen? (denk aan praten, dansen, springen etc)* \

antwoorden:
- liedjes zingen\
- Kennen jullie van die filmpjes van paarden die kunnen rekenen? Dansen is ook zeker goed. Het belangrijkste is denk ik dat je naast het technisch vernuft, de robot ook een echte persoonlijkheid geeft (uiterlijk, bewegingen, karakter, etc.).\
- dansen & springen\
- liedjes zingen\
- Op commando blaffen\
- Springen\
- Versnaperingen brengen\
- Bijvoorbeeld met beweging van de poot een piano begintune spelen\
- Trucjes zoals zit en poot\
- Hoe het zijn pad kan bepalen doormiddel van de sensoren en route bepaling\
- Ik zou het leuk vinden dat de robot autonoom kan lopen in de school\
- praten\
- salto\
- Ik denk dat het interessant zou zijn om object herkenning te integreren. dus bijvoorbeeld iets met een husky lens. daarnaast zou het natuurlijk gaaf zijn om er voor te zorgen dat de reageert op zijn of haar naam\
- Vergeet niet de mooie technieken te zien! Praten springen en bewegen\
- Soundboard en dansen\
- hij moet rondlopen en mensen aanspreken\

#pagebreak()

=== Conclusie trends

#show table.cell.where(y: 0): set text(fill: white, weight: "bold")

#table(
  columns: 4,
  align: (left, center, center, left),
  fill: (x, y) => {
    if y == 0 { 
      rgb(60, 60, 60) 
    } else if calc.rem(y, 2) == 0 { 
      rgb(240, 240, 240) 
    } else { 
      white 
    }
  },
  stroke: none,
  
  [Trend],
  [Noemingen],
  [%],
  [Belangrijkste Inzichten],
  
  [Dansen/Beweging], [6], [35%], [Meest gewenste feature - beweging is essentieel voor aantrekkelijkheid],
  [Zingen/Geluid], [5], [29%], [Audio feedback + entertainment (soundboard, liedjes, blaffen)],
  [Communicatie/Interactie], [4], [24%], [Praten, naam herkennen, aanspreken bezoekers],
  [Technische Features], [3], [18%], [Sensoren, autonome navigatie, object herkenning],
  [Trucjes/Commando's], [2], [12%], [Gehorzamen, zit, poot geven],
  [Persoonlijkheid/Karakter], [1], [6%], [Uiterlijk + bewegingen = echte persoonlijkheid],
)

*1. Entertainment > Techniek (minimaal voor demo's)*
- Respondenten willen entertainment (dans, geluid) PLUS zien dat techniek werkt
- Niet alleen "kijk hoe slim", maar ook "kijk hoe grappig/leuk"
*2. Top 3 Must-Have Features:*
- Dansen/Dynamische beweging (6x genoemd)
- Audio/Geluid output (5x genoemd - zingen, commando's, soundboard)
- Interactie met bezoekers (4x genoemd - autonomie, aanspreken)
*3. Technische Wensen:*
- Autonome navigatie door school
- Object/gezicht herkenning (Husky Lens)
- Sensor-based path planning
*4. Persoonlijkheid is Cruciaal:*
- "Vergeet niet de mooie technieken te zien!"
- Één respondent benadrukt: techniek + persoonlijkheid = succes

#pagebreak()

*Aanbeveling voor Robothond-Design:*\
*Fase 0 (MVP):* Werkende robothond die kan lopen\
*Fase 1:* Dansen + Audio \
*Fase 2:* Autonome navigatie + interactie\
*Fase 3:* Geavanceerde sensoren zoals object herkenning\

#v(1em)

=== Uitwerking conclusie
De uitkomsten van de enquête worden rechtstreeks omgezet in concrete functies voor het ontwerp. De hond krijgt daarom een geprogrammeerde dansmodus gecombineerd met een soundboard dat verschillende geluiden kan afspelen. Ook wordt er een AI gestuurde functie toegevoegd voor autonome navigatie en obstakelvermijding, omdat veel mensen dit technisch interessant vinden. Zo sluit het ontwerp goed aan bij de behoefte van de stakeholders en laat het op een duidelijke manier de techniek achter de robothond zien tijdens open dagen.