
#import "../Config/GeneralConfig.typ": *
#import "@preview/cetz:0.4.2"
#import "@preview/cetz-plot:0.1.3": chart

== Stakeholder analyse

#v(1em)

=== De Stakeholders <stakeholder-analyse>

*Stakeholder 1: Projecteigenaar* \
Meneer Rieno Moedt is de projecteigenaar van dit project. Hij bepaalt de spelregels en acceptatiecriteria. Verder stelt hij de eisen vast en beoordeeld hij het eindresultaat. Zijn voornaamste belangen zijn een goed werkend proof-of-concept en high-fidelity prototype, naleving van alle eisen, en een eindproduct dat geschikt is voor open dagen en toekomstige studenten die enthousiast worden gemaakt. Zijn invloed is zeer hoog: hij bepaalt het uiteindelijke cijfer. De strategie is regelmatig, open overleg met duidelijke verwachtingen en directe feedback.

*Stakeholder 2: Leerlingen/Studenten* \
De leerlingen en studenten die het robothondproject op de open dag zullen zien, zijn de primaire eindgebruikers. Zij vormen de doelgroep waarvan zij enthousiast moeten worden wanneer zij het product aanraken en gebruiken. Hun belangen zijn duidelijk: zij willen een aantrekkelijke en interactieve demonstratie die hen enthousiast maakt en inzicht geeft in wat elektrotechniek te bieden heeft. Hun invloed is matig tot hoog. De leerlingen/Studenten bepalen via hun ervaringen en feedback of het project succesvol is in het aantrekken van toekomstige studenten, wat indirect van invloed is op de perceptie van de opleiding. De communicatie is vooral op de opendagen zelf. Huidige jaar 1 elektrostudenten kunnen wel ondervraagd worden over wat zij willen zien in het project. De strategie is het product intuïtief maken met een betrouwbare en interactieve werking.

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

#let stakeholder-matrix() = {
  align(center)[
    #text(size: 20pt, weight: "bold")[4 Typen Stakeholders]
    
    // Grid: Y-as label LINKS, tabel RECHTS
    #grid(
      columns: (32pt, 1fr),
      gutter: 16pt,
      align: (center, left),

      // Y-as label (linkerkant, verticaal geroteerd)
      [
        #v(80pt)
        #rotate(-90deg, origin: center)[
          #text(size: 11pt, fill: gray, weight: "bold")[Invloed Stakeholder]
        ]
      ],

      // Tabel 2x2 (rechterkant)
      table(
        columns: (1fr, 1fr),
        rows: (auto, auto),
        stroke: none,
        gutter: 12pt,
        inset: 0pt,

        // Toeschouwer (geel)
        rect(width: 100%, height: 80pt, fill: rgb("#FFD65C"), radius: 8pt, inset: 10pt,
          align(center + horizon)[
            #text(size: 13pt, weight: "bold", fill: rgb("#333333"))[Toeschouwer]
            #linebreak()
            #text(size: 10pt, fill: rgb("#333333"))[Weinig aandacht]
          ]
        ),

        // Beïnvloeder (groen)
        rect(width: 100%, height: 80pt, fill: rgb("#5CB85C"), radius: 8pt, inset: 10pt,
          align(center + horizon)[
            #text(size: 13pt, weight: "bold", fill: white)[Beïnvloeder]
            #linebreak()
            #text(size: 10pt, fill: white)[Tevreden houden]
          ]
        ),

        // Sleutelfiguur (oranje)
        rect(width: 100%, height: 80pt, fill: rgb("#F27E39"), radius: 8pt, inset: 10pt,
          align(center + horizon)[
            #text(size: 13pt, weight: "bold", fill: white)[Sleutelfiguur]
            #linebreak()
            #text(size: 10pt, fill: white)[Samenwerken]
          ]
        ),

        // Geïnteresseerde (blauw)
        rect(width: 100%, height: 80pt, fill: rgb("#4589C9"), radius: 8pt, inset: 10pt,
          align(center + horizon)[
            #text(size: 13pt, weight: "bold", fill: white)[Geïnteresseerde]
            #linebreak()
            #text(size: 10pt, fill: white)[Informeren]
          ]
        ),
      )
    )

    #v(20pt)

    // X-as labels
    #grid(
      columns: (1fr, 1fr),
      gutter: 12pt,
      align(center)[#text(size: 11pt, fill: gray)[Laag — Matig]],
      align(center)[#text(size: 11pt, fill: gray)[Hoog — Zeer hoog]]
    )

    #text(size: 12pt, fill: gray, weight: "bold")[Belang Stakeholder]
  ]
}

#stakeholder-matrix()

#let yellow = rgb("#FFD65C")
#let green = rgb("#5CB85C")
#let orange = rgb("#F27E39")
#let blue = rgb("#4589C9")

#let cel(color, body) = table.cell(fill: color)[body]


#table(
  columns: (3cm, 3.5cm, 3.5cm, 3.5cm, 3.5cm),
  rows: (2.5cm, 2cm, 2cm, 2cm, 2cm),
  align: center + horizon,
  stroke: 0.8pt + gray,
  fill: (col, row) => {
    if row == 0 or col == 0 { luma(220) }
    else { white }
  },
  
  // Rij 0 (koppen)
  table.cell(rowspan: 1, colspan: 1)[
    #set align(left + top)
    #v(0.3em)
    #h(0.3em)
    *Invloed* →
    #v(0.8em)
    #h(2.5em)
    ↓
    #v(0.5em)
    #align(right + bottom)[
      #h(0.3em)
      *Belang*
      #v(0.3em)
    ]
  ],
  [*Laag*], [*Matig*], [*Hoog*], [*Zeer hoog*],
  
  [*Zeer hoog*],
  table.cell(fill: rgb("#FFD65C"))[],  
  table.cell(fill: rgb("#FFD65C"))[], 
  table.cell(fill: rgb("#5CB85C"))[MBO studenten/Havo studenten], 
  table.cell(fill: rgb("#5CB85C"))[Project eigenaar],
  
  [*Hoog*], 
  table.cell(fill: rgb("#FFD65C"))[], 
  table.cell(fill: rgb("#FFD65C"))[], 
  table.cell(fill: rgb("#5CB85C"))[], 
  table.cell(fill: rgb("#5CB85C"))[],
  
  [*Matig*], 
  table.cell(fill: rgb("#F27E39"))[Docenten van Eletrotechniek], 
  table.cell(fill: rgb("#F27E39"))[Ouders/Verzorgers], 
  table.cell(fill: rgb("#4589C9"))[], 
  table.cell(fill: rgb("#4589C9"))[Tutor],

  [*Laag*], 
  table.cell(fill: rgb("#F27E39"))[Overige Bezoekers], 
  table.cell(fill: rgb("#F27E39"))[], 
  table.cell(fill: rgb("#4589C9"))[], 
  table.cell(fill: rgb("#4589C9"))[],
)

#pagebreak()


=== Analyse Enquete
Hier worden alle vragen die in de enquete worden gevraagd geanalyseert.

*Tot welke groep behoort u?*

#cetz.canvas({
  import cetz.draw: *
  
  chart.piechart(
    (
      ("Leerling/Student", 18),
      ("Docent", 4)
    ),
    value-key: 1,
    label-key: 0,
    radius: 3,
    slice-style: (
      (fill: blue),
      (fill: yellow)
    ),
    inner-label: (
      content: "%",
      radius: 120%
    ),
  )
})

#v(1em)

*Wat is je huidige opleidingsniveau?*

#cetz.canvas({
  import cetz.draw: *
  
  chart.piechart(
    (
      ("HBO", 14),
      ("MBO", 5),
      ("VWO", 1),
      ("HAVO", 1),
      ("Anders", 1)
    ),
    value-key: 1,
    label-key: 0,
    radius: 3,
    slice-style: (
      (fill: rgb(147, 51, 234)),   // paars (HBO)
      (fill: rgb(34, 197, 94)),     // groen (MBO)
      (fill: rgb(239, 68, 68)),     // rood (VWO)
      (fill: rgb(249, 115, 22)),    // oranje (HAVO)
      (fill: rgb(59, 130, 246))     // lichtblauw (Anders)
    ),
    inner-label: (
      content: "%",
      radius: 150%
    ),
  )
})

#pagebreak()

*Welke eigenschappen maakt een project volgens u het meest aantrekkelijk om te bekijken?*

#cetz.canvas({
  import cetz.draw: *
  
  chart.piechart(
    (
      ("Interactief, waarbij je iets ziet bewegen of reageren", 13),
      ("Technisch indrukwekkend", 6),
      ("Visueel aantrekkelijk", 3)
    ),
    value-key: 1,
    label-key: 0,
    radius: 3,
    slice-style: (
      (fill: rgb(249, 115, 22)),    // oranje (Interactief)
      (fill: rgb(239, 68, 68)),     // rood (Technisch)
      (fill: rgb(59, 130, 246))     // blauw (Visueel)
    ),
    inner-label: (
      content: "%",
      radius: 120%
    ),
  )
})

#v(1em)

*Moet het project vooral gericht zijn op vermaak, of op het laten zien van technische vaardigheden?*

#cetz.canvas({
  import cetz.draw: *
  
  chart.piechart(
    (
      ("Een goede mix van beide", 17),
      ("Meer techniek en vaardigheden", 4),
      ("Meer vermaak", 1)
    ),
    value-key: 1,
    label-key: 0,
    radius: 3,
    slice-style: (
      (fill: rgb(249, 115, 22)),    // oranje (Interactief)
      (fill: rgb(239, 68, 68)),     // rood (Technisch)
      (fill: rgb(59, 130, 246))     // blauw (Visueel)
    ),
    inner-label: (
      content: "%",
      radius: 150%
    ),
  )
})

#pagebreak()

*Wat vind u interessant? (Beide antwoorden mogen aangevinkt worden)*

#v(1em)

#cetz.canvas({
  import cetz.draw: *
  
  let data = (
    ("AI functionaliteit", 13,),
    ("Afstandbestuurbaar", 11),
    ("Autonome navigatie en\nobstakelvermijding", 19),
  )
  
  chart.barchart(
    mode: "basic",
    size: (10, 4),
    label-key: 0,
    value-key: 1,
    bar-width: 0.6,
    x-tick-step: 5,
    data,
    axes: (
      left: (
        tick: (
          label: (angle: 0, anchor: "east")
        )
      ),
      bottom: (
        min: 0,
        max: 22,
        tick: (step: 5)
      )
    )
  )
})

#v(1em)

*Wat is volgens u het belangrijkste doel van het project dat wij laten zien?*

#cetz.canvas({
  import cetz.draw: *
  
  chart.piechart(
    (
      ("Interesse wekken voor de studie Elektrotechniek", 10),
      ("Laten zien wat studenten kunnen maken", 6),
      ("Toekomstmogelijkheden in techniektonen", 5),
      ("hond", 1)
    ),
    value-key: 1,
    label-key: 0,
    radius: 3,
    slice-style: (
      (fill: rgb(59, 130, 246)),    // blauw (Interesse wekken)
      (fill: rgb(239, 68, 68)),     // rood (Laten zien)
      (fill: rgb(249, 115, 22)),    // oranje (Toekomstmogelijkheden)
      (fill: rgb(34, 197, 94))      // groen (hond)
    ),
    inner-label: (
      content: "%",
      radius: 150%
    ),
  )
})

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

== Uitwerking conclusie
De uitkomsten van de enquête worden rechtstreeks omgezet in concrete functies voor het ontwerp. De hond krijgt daarom een geprogrammeerde dansmodus gecombineerd met een soundboard dat verschillende geluiden kan afspelen. Ook wordt er een AI gestuurde functie toegevoegd voor autonome navigatie en obstakelvermijding, omdat veel mensen dit technisch interessant vinden. Zo sluit het ontwerp goed aan bij de behoefte van de stakeholders en laat het op een duidelijke manier de techniek achter de robothond zien tijdens open dagen.