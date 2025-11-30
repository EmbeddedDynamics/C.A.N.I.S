//#import "@preview/abbr:0.3.0"

//#show: abbr.show-rule

#import "@preview/acrostiche:0.7.0": *

#let academic-frontpage(
  title: [Document Title],
  authors: "Author Name",
  degree: [Bachelor of Science],
  degree-goal: [A Thesis Submitted in Fulfillment of the Requirements \ 
      for the Degree of],
  department: [Department Name],
  university: [University Name],
  supervisor: [Supervisor Name],
  tutor: [Tutor Name],
  date: datetime.today(),
  degree-year: [Year],
  program-type: [Program Type],
  location: [City, Country],
  logo: none,  // Add this parameter
  project-logo: none,
  abstract: none,
  keywords: none,
  acknowledgments: none,
  acronyms: (:),
  biblio: none,
  body
) = {
  // Document metadata
  set document(title: title, author: authors)

  set page(footer: context [
    #let current = counter(page).get().first()
    #let total = counter(page).final().first()
    
    #table(
      columns: (1fr, auto),
      stroke: 0pt,
      inset: 0pt,
      align: (left, horizon + right),
      [
        #if project-logo != none [
          #image(project-logo, width: 120pt)
        ]
      ],
      [
        #text(size: 11pt, fill: rgb("#111111"))[Pg #current van #total]
      ]
    )
  ])
  
  // Heading configuration
  set heading(numbering: none)
  
  show heading: it => {
    let sizes = (25pt, 18pt, 15pt, 12pt, 12pt, 12pt)
    let spacings = (2em, 1.6em, 1.3em, 1.1em, 1em, 1em)
    let level = it.level - 1
    let size = if level < sizes.len() { sizes.at(level) } else { 12pt }
    let spacing = if level < spacings.len() { spacings.at(level) } else { 1em }
    
    // Check if this is a content level 1 heading (Chapter X)
    let is-chapter = false
    let chapter-number = 0
    
    if it.level == 1 and it.numbering != none {
      let numbering-result = numbering(it.numbering, ..counter(heading).get())
      if type(numbering-result) == str and numbering-result.starts-with("Hoofdstuk") {
        is-chapter = true
        let chapter-str = numbering-result.slice(10)
        chapter-number = int(chapter-str)
      }
    }
    
    // Add pagebreak before Chapter 2 and onwards
    if is-chapter and chapter-number >= 2 {
      pagebreak()
    }
    
    v(spacing, weak: true)
    
    // Center alignment for chapter headings
    if it.level == 1 and is-chapter {
      align(center)[
        #text(size: size, weight: "bold")[#it]
      ]
    } else {
      text(size: size, weight: "bold")[#it]
    }
    
    v(spacing, weak: true)
  }
  
  // Figure and caption styling
  show figure.where(kind: table): set figure.caption(position: top)
  show figure.caption.where(kind: table): it => {
    text(size: 10pt, weight: "bold")[#it]
  }
  show figure.caption.where(kind: image): it => {
    text(size: 10pt, weight: "bold")[#it]
  }
  
  // ============================================================
  // COVER PAGE
  // ============================================================
  set text(size: 14pt)
  align(center)[
    // Header section
    #text(size: 22pt, weight: "bold", fill: rgb("#1a1a1a"))[#university]
    #v(8pt, weak: true)
    
    #if logo != none [
      #image(logo, width: 160pt)
      #v(12pt, weak: true)
    ]
    
    // Divider line
    #line(length: 70%, stroke: 1.5pt + rgb("#333333"))
    #v(16pt, weak: true)

    // Title section
    #text(size: 19pt, weight: "bold", fill: rgb("#1a1a1a"))[#title]
    
    #v(10pt, weak: true)
    #text(size: 12pt)[
      #degree-goal
    ]
    #v(28pt, weak: true)
    
    #text(weight: "bold", size: 13pt)[AUTEURS]
    #v(6pt, weak: true)
    #text(size: 12pt)[#authors]
    #v(16pt, weak: true)

    #v(38pt, weak: true)
    #text(weight: "bold", size: 13pt)[AFDELING]
    #v(6pt, weak: true)
    #text(size: 12pt)[
      #department
      
      #university
    ]

    #v(38pt, weak: true)
    #text(size: 12pt)[
      Onder toezicht van: #text(weight: "bold")[#supervisor]
    ]
    
    //#text(size: 11pt)[© #month #year]
    #v(1fr)
    #line(length: 70%, stroke: 1.5pt + rgb("#333333"))
    #v(12pt, weak: true)
    #text(size: 10pt, fill: gray)[
      #date.display("[day] [month repr:long] [year]")
    ]
  ]
  pagebreak()
  
  // ============================================================
  // TITLE PAGE (Back of cover)
  // ============================================================
  set text(size: 12pt)
  set page(numbering: "1")
  counter(page).update(2)
  
  // Header information
  grid(
    columns: (1fr, 1fr),
    column-gutter: 2em,
    row-gutter: 1.5em,
    align: (left, right),
    [
      #program-type (#degree-year) \
      #department
    ],
    [
      #university \
      #location
    ],
  )
  
  v(4em)
  
  // Detailed information grid
  grid(
    columns: (0.4fr, 1fr),
    column-gutter: 1em,
    row-gutter: 2em,
    align: (left, left),
    
    [*TITEL:*],
    [#title],
    
    [*AUTEUR:*],
    [
      #authors \
      #degree, #department \
      #university \
      #location
    ],
    
    [*TOEZICHTHOUDER:*],
    [#supervisor],

    [*TUTOR:*],
    [#tutor],
    
    [*AANTAL PAGINA'S:*],
    [
      #context {
        let total-pages = counter(page).final().first()
        total-pages
      }
    ]
  )
  pagebreak()

  // ============================================================
  // ABSTRACT
  // ============================================================
  set par(justify: true, leading: 1em, spacing: 2.5em)
  if abstract != none {
    [= Abstract]
    
    [#abstract]
    
    v(1.5em)

    if keywords != none {
      text(weight: "bold")[Trefwoorden: ] + [#keywords]
    }
    
    pagebreak()
  }
  
  // ============================================================
  // ACKNOWLEDGMENTS
  // ============================================================
  if acknowledgments != none {
    [= Dankbetuigingen]
    
    [#acknowledgments]
    
    pagebreak()
  }

  // ============================================================
  // TABLE OF CONTENTS
  // ============================================================
  show outline.entry.where(level: 1): it => {
    v(12pt, weak: true)
    strong(it)
  }
  show outline.entry.where(level: 2): it => {
    pad(left: 0em, it)
  }
  show outline.entry.where(level: 3): it => {
    pad(left: 1em, it)
  }
  
  outline(title: text(size: 25pt, weight: "bold")[Inhoudsopgave], indent: 2em)
  pagebreak()
  
  // ============================================================
  // LIST OF FIGURES
  // ============================================================
  heading(level: 1, numbering: none)[Lijst van figuren]
  outline(title: none, target: figure.where(kind: image), indent: 2em)
  pagebreak()
  
  // ============================================================
  // LIST OF TABLES
  // ============================================================
  heading(level: 1, numbering: none)[Lijst van tabellen]
  outline(title: none, target: figure.where(kind: table), indent: 2em)
  pagebreak()

  // ============================================================
  // ACRONYMS
  // ============================================================
  if acronyms != (:) {
    //[= List of Acronyms]
    
    v(1em)
    
    /*figure(
      table(
        columns: (25%, 75%),
        stroke: 0.5pt,
        align: (center, left),
        table.header(
          [*Acronym*], [*Definition*]
        ),
        ..acronyms.pairs().sorted(key: pair => pair.at(0)).map(((key, value)) => (key, value)).flatten()
      ),
      caption: [Definition of Acronyms],
      kind: table
    )*/

    //print_acronym_listing("List of Acronyms")

    init-acronyms(
      acronyms
    )

    //print-index()

    // Or customize it:
    print-index(
      //level: 2,
      //numbering: "1.",
      outlined: false,
      //sorted: "up",
      //used_only: true,
      title: "Afkortingenlijst",
      delimiter: "",
      row-gutter: 6pt, 
      used-only: false, 
      column-ratio: 0.2,
      clickable:true
    )
    
    pagebreak()
  }
  
  // ============================================================
  // MAIN CONTENT
  // ============================================================
  //set page(numbering: "1")
  //counter(page).update(1)
  
  // Reset heading counter and set numbering for main content
  //counter(heading).update(0)
  set heading(numbering: (..nums) => {
    let level = nums.pos().len()
    if level == 1 {
      "Hoofdstuk " + str(nums.pos().at(0))
    } else if level == 2 {
      str(nums.pos().at(0)) + "." + str(nums.pos().at(1))
    } else if level == 3 {
      //let letters = ("A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z")
      //str(nums.pos().at(0)) + "." + str(nums.pos().at(1)) + "." + letters.at(calc.min(nums.pos().at(2) - 1, 25))
      str(nums.pos().at(0)) + "." + str(nums.pos().at(1)) + "." + str(nums.pos().at(2))
    } else if level == 4 {
      nums.pos().map(str).join(".")
    } else {
      nums.pos().map(str).join(".")
    }
  })
  
  // Bibliography styling
  
  
  // Main document body
  body
  
  // biblio
  if biblio != none {
    pagebreak()
    bibliography(biblio, full: true)
  }
}
