// ============================================
// Contract
// ============================================

// THIS DOCUMENT DOESNT FUNCTION AS A STANDALONE
// DOCUMENT AND CAN ONLY BE USED IN Hoofddocument.typ

// Import general data
#import "../Config/GeneralConfig.typ": *

== Samenwerkingscontract

#let ondertekenaars = students.map(s => s.name)

#let studentRows = students.map(s => (
  [#s.name],
  [#link("mailto:" + s.email)[#s.email]],
  [#s.tel],
)).flatten()

#table(
  columns: (auto, auto, auto),
  stroke: 0.7pt,
  inset: 6pt,
  align: (left, left, left),
  fill: (rgb("f7f7f7"), none, none),
  [*Naam studenten*], [*E-mail Adres*], [*Telefoonnummer*],
  ..studentRows
)

#v(8pt)

*Doel van de samenwerking*\
In een hecht en verantwoordelijk team samenwerken aan het ontwerp en de realisatie van een Robothond, waarbij we elkaar versterken en een professioneel eindproduct opleveren.

#v(6pt)

*Contractduur:*\ 
van #startdate tot #enddate.

#v(8pt)

*Afspraken*
+ Aanwezig zijn op afgesproken momenten 
+ Werk op tijd klaar
+ Reageren op berichten
+ Verdere afspraken en verantwoordelijkheden zijn benoemd in @rollen \ (tenzij er een goede reden is zoals ziekte, zijn er consequenties voor het teamlid)


*Consequenties*
- Eerste keer afspraak niet nagekomen:\ Gesprek met het groepje.
- Tweede keer afspraak niet nagekomen:\ Gesprek met tutor.
- Derde keer afspraak niet nagekomen:\ verwijdering uit het groepje.

#v(12pt)

#grid(
  columns: 2,
  gutter: 1fr,
  [*Plaats:* #city, #university],
  [*Datum:* #startdate],
)

#v(18pt)
#pagebreak()

*Getekend door:*

#let ondertekenaars = students.map(s => ([#s.name]))

#let handtekening = (name) => block(
  spacing: 6pt,
  //rule(stroke: 0.7pt, width: 7cm),
  emph(name),
)

#grid(
  columns: 2,
  gutter: 2cm,
  ..ondertekenaars.map(n => handtekening(n))
)