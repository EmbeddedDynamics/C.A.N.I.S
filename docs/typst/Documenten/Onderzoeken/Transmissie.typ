#import "@preview/fletcher:0.5.8" as fletcher: diagram, node, edge

#import "@preview/oxifmt:0.2.1": strfmt

#import "@preview/tablem:0.1.0": tablem
#import "@preview/numbly:0.1.0": numbly

#import "/Documenten/template.typ": *
#import "/Documenten/Config/config.typ": *

#set text(lang: "Nl")

#show link: underline

//---------------------------------------
// Front page and general definitions
//---------------------------------------
#show: academic-frontpage.with(
  title: [Embedded Systems Project\ Robothond],
  authors: students.map(s => s.name).join(", "),
  degree: [#degree],
  degree-goal: [Onderzoek voor het realiseren van een Robothond],
  department: [#department],
  university: [#university, #universitydesc],
  program-type: [#program],
  degree-year: [#year],
  location: [#city, #country],
  supervisor: [#supervisor],
  tutor: [#tutor],
  logo: "/Documenten/Assets/NHL_logo.jpg",
  project-logo: "/Documenten/Assets/ED_Logo.png",
  //month: [#month],
  //year: [#year],
  date: datetime.today(),
  abstract: [In dit project ontwikkelen wij als bachelorstudenten van NHL Stenden een embedded systeem in vorm van een Robothond. Het apparaat is bedoeld als demonstrator op opendagen om de mogelijkheden van technische innovatie aan toekomstige studenten uit te leggen. We volgen hiervoor de Design-Thinking methodologie met nadruk op iteratieve prototyping en uitvoerige testen. Het doel is een volledig functioneel product te realiseren dat voldoet aan de stakeholder-eisen en markt-gereed is.],
  
  keywords: [],
  
  acknowledgments: [],
  
  acronyms: (
    "PvE": ("Pakket van Eisen","Pakket van Eisen"),
    "MVP": ("Minimum Viable Product","Minimum Viable Product"),
    "PvA": ("Plan van Aanpak","Plan van Aanpak"),
    //"MVP": "Minimum Viable Product",
    //"PvA": "Plan van Aanpak",
  ),

  biblio: "/Documenten/Referenties/Transmissie.bib",
)

test