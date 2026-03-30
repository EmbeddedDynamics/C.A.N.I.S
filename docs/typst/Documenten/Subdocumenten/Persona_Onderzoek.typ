// ============================================
// Persona Onderzoek - Empathize fase
// ============================================

// General data import
#import "../Config/GeneralConfig.typ": *

// Set parameters for the document
#set text(lang: "Nl")
#show link: underline


// ============================================
// Document contents
// ============================================
#let persona_body() = [
== Persona Onderzoek

In de Empathize-fase van het Design Thinking proces stellen we een persona op. Een persona is een fictief maar realistisch persoon die een representatief deel van de doelgroep beschrijft. In dit geval is de doelgroep: een bezoeker van de open dag van NHL Stenden die kennismaakt met de Robothond als demonstrator.

Door ons in te leven in deze persoon kunnen we beter begrijpen wat de Robothond moet kunnen, hoe hij zich moet gedragen en wat de bezoeker enthousiast maakt voor de opleiding.

#v(1em)

// Persona card
#block(
  stroke: 0.7pt,
  radius: 4pt,
  inset: 16pt,
  width: 100%,
  [
    #grid(
      columns: (auto, 1fr),
      gutter: 16pt,

      // Avatar placeholder
      rect(width: 90pt, height: 90pt, radius: 4pt, fill: luma(220))[
        #align(center + horizon)[
          #image("../Assets/Persona/lars.png")
        ]
      ],

      // Naam & basisinfo
      [
        #text(size: 14pt, weight: "bold")[Lars de Vries]
        #v(4pt)
        #grid(
          columns: (80pt, 1fr),
          gutter: 4pt,
          [*Leeftijd*], [17 jaar],
          [*Opleiding*], [5e jaar VWO - profiel N&T],
          [*Woonplaats*], [Drachten],
          [*Interesses*], [Elektronica, gaming, 3D-printen, robotica],
        )
        #v(6pt)
        #emph["Ik wil weten wat ik echt ga doen op een technische opleiding. Niet alleen theorie, maar ook zelf iets maken dat werkt."]
      ]
    )
  ]
)

#v(1em)

=== Achtergrond

Lars is een 17-jarige VWO-scholier uit Drachten met het profiel Natuur & Techniek. Hij knutselt thuis graag met Arduino's en heeft al een paar kleine projectjes gebouwd: een temperatuurmeter en een LED-strip-controller. Hij wil later iets doen met elektronica of software, maar twijfelt nog tussen een hbo-opleiding en een universitaire studie.

Zijn ouders zijn praktisch ingesteld en vinden het belangrijk dat Lars een opleiding kiest waarbij hij ook daadwerkelijk iets leert maken. Lars zelf wil begrijpen wat hij de komende vier jaar gaat doen voordat hij een keuze maakt.

#pagebreak()

=== Doelen

- Een opleiding vinden die aansluit bij zijn interesse in elektronica en technologie
- Begrijpen wat het verschil is tussen theorie en praktijk op een hbo-opleiding
- Iets zien of aanraken dat hem enthousiast maakt voor de studie
- Zijn ouders laten zien dat de keuze voor NHL Stenden goed onderbouwd is

=== Frustraties

- Open dagen zijn vaak saai: veel folders, weinig hands-on
- Hij begrijpt niet altijd wat "embedded systems" of "regeltechniek" in de praktijk betekent
- Demonstrators die achter glas staan en niet aangeraakt mogen worden
- Uitleg die te technisch of juist te vaag is

=== Bezoek aan de open dag

Lars bezoekt de open dag van NHL Stenden samen met zijn moeder. Hij heeft online al gezien dat er een robotica-project te zien is en is nieuwsgierig. Bij de stand van de Robothond ziet hij een vierpotige robot die reageert op zijn stem en obstakels detecteert. Hij mag een commando geven en ziet de robot bewegen. De studenten leggen in gewone taal uit hoe ze de besturing hebben geprogrammeerd, hoe de poten werken en wat de Jetson doet.

Lars stelt vragen over de code, de motoren en hoe moeilijk het was om alles te laten samenwerken. De studenten laten kort de code zien op een laptop. Zijn moeder vraagt wat je ermee kunt doen na de studie.

#pagebreak()

=== Gebruikersscenario

+ Lars loopt door de school en ziet de robot bewegen.
+ Een student vraagt of Lars iets wil zeggen tegen de robot.
+ Lars zegt "hallo" - de robot speelt een grappig geluid af en praat terug.
+ De student legt uit: de Jetson herkent spraak en speelt geluidjes, de #acr("PSoC5") verwerkt het commando.
+ Lars vraagt hoe de poten werken. De student tekent snel de diamant-kinematica op een tablet.
+ Lars ziet dat de studenten dit zelf hebben gebouwd en geprogrammeerd. Van PCB tot firmware.
+ Na 10 minuten vraagt Lars of er een brochure is over de opleiding Elektrotechniek.

=== Inzichten voor het ontwerp

#table(
  columns: (1fr, 1fr),
  stroke: 0.7pt,
  inset: 8pt,
  [*Wat Lars verwacht*], [*Vertaling naar de Robothond*],
  [Iets aanraken of besturen], [Spraakbesturing en live beweging zichtbaar houden],
  [Begrijpelijke uitleg zonder onzin], [Demonstratie is intuïtief:\ input met zichtbare actie],
  [Zien dat studenten het zelf hebben gemaakt], [Laat de PCB's en code zien als onderdeel van de demo],
  [Iets dat indruk maakt op zijn moeder], [Betrouwbare werking, nette afwerking van het systeem],
  [Niet saai / het liefst interactief], [De hond reageert d.m.v. AI. Met woorden of een geluidje.\ Dat maakt de robot persoonlijk],
)
]