// ============================================
// Stakeholder config file with matrix function
// and complete database
// ============================================

// Import general data
#import "GeneralConfig.typ": *

// Import Packages
#import "@preview/cetz:0.4.2"
#import "@preview/cetz-plot:0.1.3": chart


// ============================================
// Stakeholder matrix for visual display
// ============================================
#let stakeholder-matrix() = {
  align(center)[
    #text(size: 14pt, weight: "bold")[De 4 Typen Stakeholders:]
    
    // Grid: Y-axis label
    #grid(
      columns: (32pt, 1fr),
      gutter: 16pt,
      align: (center, left),

      // Y-axis label (left, vertically rotated)
      [
        #v(80pt)
        #rotate(-90deg, origin: left)[
          #text(size: 11pt, fill: gray, weight: "bold", hyphenate: false)[
            Belang Stakeholder
          ]
        ]
      ],

      // matrix 2x2 (right side)
      table(
        columns: (1fr, 1fr),
        rows: (auto, auto),
        stroke: none,
        gutter: 12pt,
        inset: 0pt,

        // Infleuncers (Yellow)
        rect(width: 100%, height: 80pt, fill: COL.yellow, radius: 8pt, inset: 10pt,
          align(center + horizon)[
            #text(size: 13pt, weight: "bold", fill: rgb("#333333"))[Beïnvloeder]
            #linebreak()
            #text(size: 10pt, fill: rgb("#333333"))[Tevreden houden]
          ]
        ),

        // Keyfigures (Green)
        rect(width: 100%, height: 80pt, fill: COL.green, radius: 8pt, inset: 10pt,
          align(center + horizon)[
            #text(size: 13pt, weight: "bold", fill: white)[Sleutelfiguur]
            #linebreak()
            #text(size: 10pt, fill: white)[Samenwerken]
          ]
        ),

        // Spectator (Orange)
        rect(width: 100%, height: 80pt, fill: COL.orange, radius: 8pt, inset: 10pt,
          align(center + horizon)[
            #text(size: 13pt, weight: "bold", fill: white)[Toeschouwer]
            #linebreak()
            #text(size: 10pt, fill: white)[Weinig aandacht]
          ]
        ),

        // Interested (Blue)
        rect(width: 100%, height: 80pt, fill: COL.blue, radius: 8pt, inset: 10pt,
          align(center + horizon)[
            #text(size: 13pt, weight: "bold", fill: white)[Geïnteresseerde]
            #linebreak()
            #text(size: 10pt, fill: white)[Informeren]
          ]
        ),
      )
    )

    // X-axis labels
    #grid(
      columns: (1fr, 1fr),
      gutter: 12pt,
      align(center)[#text(size: 11pt, fill: gray)[Laag - Matig]],
      align(center)[#text(size: 11pt, fill: gray)[Hoog - Zeer hoog]]
    )

    #text(size: 12pt, fill: gray, weight: "bold")[Invloed Stakeholder ]
  ]
}



// ============================================
// Stakeholder table for visual display
// ============================================
#let stakeholder-table() = {
  let cel(color, body) = table.cell(fill: color)[body]
  
  table(
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
    table.cell(fill: rgb(COL.yellow))[],  
    table.cell(fill: rgb(COL.yellow))[], 
    table.cell(fill: rgb(COL.green))[MBO studenten/Havo studenten], 
    table.cell(fill: rgb(COL.green))[Project eigenaar],
    
    [*Hoog*], 
    table.cell(fill: rgb(COL.yellow))[], 
    table.cell(fill: rgb(COL.yellow))[], 
    table.cell(fill: rgb(COL.green))[], 
    table.cell(fill: rgb(COL.green))[],
    
    [*Matig*], 
    table.cell(fill: rgb(COL.orange))[Docenten van\ Eletrotechniek], 
    table.cell(fill: rgb(COL.orange))[Ouders/\ Verzorgers], 
    table.cell(fill: rgb(COL.blue))[], 
    table.cell(fill: rgb(COL.blue))[Tutor],
  
    [*Laag*], 
    table.cell(fill: rgb(COL.orange))[Overige Bezoekers], 
    table.cell(fill: rgb(COL.orange))[], 
    table.cell(fill: rgb(COL.blue))[], 
    table.cell(fill: rgb(COL.blue))[],
  )
}



// ============================================
// Database for the stakeholder pie and
// bar charts
// ============================================
#let stakeholdercharts_db = (
  doelgroep: (
    title: "Doelgroep",
    items: (
      (name: "Leerling/Student", 
      value: 18, color: "lightblue"),
      (name: "Docent",
      value: 4,  color: "yellow"),
    ),
  ),
  
  educationlevel: (
    title: "Opleiding",
    items: (
      (name: "HBO",
      value: 14, color: "purple"),
      (name: "MBO",
      value: 5,  color: "green"),
      (name: "VWO",
      value: 1,  color: "red"),
      (name: "HAVO",
      value: 1,  color: "orange"),
      (name: "Anders",
      value: 1,  color: "lightblue"),
    ),
  ),

  attractiveness: (
    title: "Aantrekkelijk om te bekijken",
    items: (
      (name: "Interactief, waarbij je iets ziet bewegen of reageren",
      value: 13, color: "orange"),
      (name: "Technisch indrukwekkend",
      value: 6,  color: "red"),
      (name: "Visueel aantrekkelijk",
      value: 3,  color: "lightblue"),
    ),
  ),
  
  focus: (
    title: "Focus project",
    items: (
      (name: "Een goede mix van beide",
      value: 17, color: "orange"),
      (name: "Meer techniek en vaardigheden",
      value: 4,  color: "red"),
      (name: "Meer vermaak",
      value: 1,  color: "lightblue"),
    ),
  ),

  interest: (
    title: "Interesse functionaliteit",
    items: (
      (name: "AI functionaliteit",
      value: 13, color: "orange"),
      (name: "Afstandbestuurbaar",
      value: 11, color: "red"),
      (name: "Autonome navigatie en\nobstakelvermijding",
      value: 19, color: "lightblue"),
    ),
  ),

  project_goal: (
    title: "Belangrijkste doel project",
    items: (
      (name: "Interesse wekken voor de studie Elektrotechniek",
      value: 10, color: "lightblue"),
      (name: "Laten zien wat studenten kunnen maken",
      value: 6,  color: "red"),
      (name: "Toekomstmogelijkheden in techniek tonen",
      value: 5,  color: "orange"),
      (name: "Hond",
      value: 1,  color: "green"),
    ),
  ),
)


// ============================================
// Legend function for piecharts
// ============================================
#let legend-line(color, value, label) = [
  #box(width: 10pt, height: 10pt, fill: color, stroke: black + 0.5pt)
  #h(6pt)
  #box(width: 10pt)[#(value)]
  #h(6pt)
  #(label)\
]