// Import Templates
#import "@local/academic-tools:0.1.10": *
#import "../template.typ": *
#import "GeneralConfig.typ": *

// ============================================
// Academic page for Acronym usage
// ============================================

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
  project-logo: "/Documenten/Assets/NHL_logo.jpg",
  //month: [#month],
  //year: [#year],
  date: datetime.today(),
  abstract: [],
  keywords: [],
  acknowledgments: [],
  acronyms: (
    "PvE": ("Pakket van Eisen","Pakket van Eisen"),
    "MVP": ("Minimum Viable Product","Minimum Viable Product"),
    "PvA": ("Plan van Aanpak","Plan van Aanpak"),
    "IK": ("Inverse Kinematics"),
    "PSoC5": ("Programmable System on a Chip 5"),
    "RTOS": ("Real-Time Operating System"),
    "MoSCoW": ("Must have, Should have, Could have, Won’t have"),
    "PCB": ("Printed Circuit Board"),
    "CAD": ("Computer Aided Design"),
    "AI": ("Artificial Intelligence"),
    "PoC": ("Proof of Concept"),
    "LiDAR": "Light Detection And Ranging",
    "ESD": ("ElectroStatic Discharge"),
    "GPS": ("Global Positioning System"),
    "ELRS": ("Express Long Range System"),
    "FPV": ("First-Person View"),
    "HDL": ("Hardware Description Language"),
    "IMU": ("Inertial Measurement Uni"),
    "PID": ("Proportional–Integral–Derivative"),
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

// ============================================
// PvE + MoSCoW database
// How to use:
// moscow = ( (title: [..], items: ( (text: [..], prio: p1), ... )), ... )
// ============================================

// Priority cells
#let p1 = table.cell(fill: rgb(150, 255, 150))[1] // Must
#let p2 = table.cell(fill: rgb(255, 255, 180))[2] // Should
#let p3 = table.cell(fill: rgb(255, 210, 150))[3] // Could
#let p4 = table.cell(fill: rgb(255, 150, 150))[4] // Won't


// PvE and MoSCoW Functions
#let slice_db(db, start_id, end_id) = {
  let out = ()
  let take = false

  for sec in db {
    if sec.ID == start_id { take = true }
    if take {
      out = out + (sec,)
    }
    if sec.ID == end_id {
      break
    }
  }
  out
}

#let pve_render(db, headerlevel) = {
  for sec in db {
    v(1em)
    
    [
      #heading(level: headerlevel)[#sec.title] #if sec.label != none { sec.label }
    ]
    
    v(0.5em)
    for it in sec.items {
      set par(leading: 0.95em, spacing: 1.0em)
      enum.item([#it.text])
    }
  }
}

#let pve_range(db, start_id, end_id, headerlevel) = {
  pve_render(slice_db(db, start_id, end_id), headerlevel)
}

#let moscow_cells(db) = {
  let cells = ([*Eis / Beschrijving*], [*Prioriteit*],)
  for sec in db {
    cells = cells + ([*#sec.ID #sec.title*], [])
    for it in sec.items {
      cells = cells + (it.text, it.prio)
    }
  }
  cells
}

#let moscow_table(db) = table(
  columns: (auto, auto),
  align: (left, center),
  inset: 6pt,
  stroke: 0.7pt + black,
  ..moscow_cells(db),
)


#let moscow_range(db, start_id, end_id) = {
  let part = slice_db(db, start_id, end_id)
  moscow_table(part)
}


// PvE_MoSCoW Data
#let pve_moscow = (

  // ===== 2.3.1 Verplichte eisen =====
  (
    ID: "2.3.1",
    label: <verplichte-eisen>,
    title: [Verplichte eisen],
    items: (
      (text: [Het systeem bevat minimaal één #acr("PSoC5") als microcontroller.], prio: p1),
      (text: [Het systeem zal gebruik maken van draadloze communicatie of een regeltechniek systeem.], prio: p1),
      (text: [Het systeem zal gebruik maken van een #acr("HDL")-component geïmplementeerd in Verilog.], prio: p1),
      (text: [Het systeem zal gebruik maken van een #acr("RTOS").], prio: p1),
      (text: [Het systeem zal geoptimaliseerd worden voor energiezuinigheid. (Zie @energie-eisen)], prio: p1),
      (text: [Het systeem zal minimaal één zelfgeschreven libary voor een communicatie protocol gebruiken], prio: p1),
      (text: [Het systeem zal geschikt moeten zijn voor opendagen. (zie @stakeholder-analyse)], prio: p1),
    ),
  ),

  // ===== 2.3.2 Functionele eisen =====
  (
    ID: "2.3.2.1",
    label: none,
    title: [Bewegingsfuncties],
    items: (
      (text: [De robot zal minimaal 1 meter naar voren kunnen lopen, met een afwijking van ± 50 centimeter.], prio: p2),
      (text: [De robot zal minimaal 1 meter naar achteren kunnen lopen, met een afwijking van ± 50 centimeter.], prio: p2),
      (text: [De robot zal minimaal 1 meter zijwaarts kunnen lopen, met een afwijking van ± 50 centimeter.], prio: p2),
      (text: [De robot zal een extra DOF beschikken in de roll-as.], prio: p2),
      (text: [De robot zal 360 graden om zijn eigen as heen kunnen draaien.], prio: p2),
      (text: [De robot zal 5 millimeter kunnen springen in de lucht.], prio: p2),
      (text: [De robothond zal een eigen persoonlijkheid hebben d.m.v. unieke bewegingen.], prio: p3),
    ),
  ),
  (
    ID: "2.3.2.2",
    label: none,
    title: [AI en autonomie],
    items: (
      (text: [De robothond zal kunnen praten d.m.v. ingebouwde speakers.], prio: p3),
      (text: [De robothond zal geluid kunnen afspelen d.m.v. ingebouwde speakers.], prio: p2),
      (text: [De robothond bestuurt de ingebouwde speakers d.m.v. #acr("AI") of ingebouwde bestuurbare muziekbord.], prio: p2),
      (text: [De robothond zal een eigen persoonlijkheid hebben d.m.v. unieke spraak.], prio: p3),
      (text: [De robothond zal autonoom door de school kunnen lopen d.m.v. #acr("AI") algoritmes zonder menselijke handelingen.], prio: p3),
    ),
  ),
  (
    ID: "2.3.2.3",
    label: none,
    title: [Modulaire functies],
    items: (
      (text: [De robothond zal kunnen voorzien worden van extra externe modules.], prio: p2),
      (text: [De robothond zal modulair ontworpen worden met aluminium extrusies voor het bevestigen van externe modules.], prio: p2),
      (text: [De robothond zal modulair ontworpen worden met pogo connectoren.], prio: p2),
    ),
  ),

  // ===== 2.3.3 Energie en performance =====
  (
    ID: "2.3.3",
    label: <energie-eisen>,
    title: [Energie en performance],
    items: (
      (text: [De robothond zal een high-power modus beschikken die de motoren niet limiteert.], prio: p3),
      (text: [De robothond zal een low-power modus beschikken die de motoren limiteert tot 50% vermogen.], prio: p2),
      (text: [De robothond beschikt een slaap modus waarbij dit op de grond gaat liggen.], prio: p2),
      (text: [De robothond zal een #acr("AI")-limitatie modus hebben die #acr("AI") functies uitzet.], prio: p2),
    ),
  ),

  // ===== 2.3.4 Gebruikersgerichtheid en betrouwbaarheid =====
  (
    ID: "2.3.4",
    label: none,
    title: [Gebruikersgerichtheid en betrouwbaarheid],
    items: (
      (text: [De robothond zal bestuurbaar zijn via een bluetooth Xbox of Playstation controller.], prio: p1),
      (text: [De robothond zal een duidelijke fysieke gebruikersinterface hebben met labels bij knoppen/schakelaars.], prio: p2),
      (text: [De robothond zal tijdens werking geen ongewenste of onverwachte bewegingen uitvoeren die afwijken van het gedefinieerde gedrag zoals vastgelegd in het testplan.], prio: p1),
      (text: [De robothond zal bruikbaar zijn voor opendagen gebaseerd op de wensen van de stakeholders. (Zie @stakeholder-analyse)], prio: p1),
    ),
  ),

  // ===== 2.3.5 Documentatie =====
  (
    ID: "2.3.5",
    label: none,
    title: [Documentatie],
    items: (
      (text: [Er wordt een #acr("PvE") opgesteld met duidelijke #acr("MoSCoW")-prioriteiten.], prio: p1),
      (text: [Er wordt een #acr("PvA") opgesteld met daarin ten minste een planning, rolverdeling en risicoanalyse.], prio: p1),
      (text: [Er zal een portfolio bijgehouden worden met gemaakte keuzes en ontwerpen.], prio: p1),
      (text: [Alle ontwerpkeuzes worden onderbouwd met berekeningen, argumentatie en/of literatuurbronnen.], prio: p1),
      (text: [Alle schema’s, code en andere ontwerpen worden als bijlage toegevoegd en zijn traceerbaar naar de gestelde eisen.], prio: p1),
      (text: [De documentatie zal gestructureerd worden volgens de fasen van het Design Thinking proces, tenzij expliciet anders afgesproken.], prio: p1),
    ),
  ),

  // ===== 2.3.6 Eisen aan het ontwikkelproces =====
  (
    ID: "2.3.6",
    label: none,
    title: [Eisen aan het ontwikkelproces],
    items: (
      (text: [Er zal gebruik gemaakt worden van het Design Thinking proces.], prio: p1),
      (text: [Er zal een testplan opgesteld worden voor elk deelproces.], prio: p1),
      (text: [Er zullen blokdiagrammen aanwezig zijn voor verschillende deelprocessen.], prio: p1),
      (text: [Er zal een #acr("PoC") gemaakt met één of meer bijhorende prototype.], prio: p1),
      (text: [Er wordt een concrete #acr("MVP") gedefinieerd die meetbaar is via één of meerdere testplannen.], prio: p1),
      (text: [Het team houdt één gezamenlijk logboek bij waarin activiteiten, uren en bijdragen per teamlid worden vastgelegd.], prio: p1),
      (text: [Het team voert wekelijks een tutor-gesprek en legt afspraken en besluiten vast in notulen.], prio: p1),
      (text: [Er wordt gebruikgemaakt van versiebeheer (bijvoorbeeld Git of vergelijkbaar).], prio: p1),
      (text: [Er wordt een peer review uitgevoerd op een #acr("PvE") en blokdiagram van de concurrentie.], prio: p1),
      (text: [Er worden rollen verdeeld in de groep en duidelijk gedefinieerd wat de verantwoordelijkheden zijn bij deze rol. (zie @team-structuur)], prio: p1),
    ),
  ),

  // ===== 2.4 Pakket van Wensen =====
  (
    ID: "2.4",
    label: none,
    title: [Pakket van Wensen],
    items: (
      (text: [De robothond heeft een extern systeem dat koffie kan inschenken D.M.V. #acr("AI") detectie.], prio: p3),
      (text: [De robothond beschikt over een ingebouwde soundboard waarmee externe gebruikers sound effects kunnen afspelen d.m.v. een controller.], prio: p3),
      (text: [De robothond kan geprogrammeerde dansjes doen.], prio: p3),
      (text: [De robothond beschikt over een #acr("GPS") systeem waardoor die real-time getracked kan worden.], prio: p3),
      (text: [De robothond beschikt over het #acr("ELRS") protocol. Hiermee kunnen wij de robothond op een minimale afstand van 1km besturen.], prio: p3),
      (text: [De robothond beschikt over een analoge #acr("FPV") camera. Hierdoor kan de gebruiker in het perspectief van de robot hond kijken.], prio: p3),
    ),
  ),

  // ===== 2.5 Buiten scope =====
  (
    ID: "2.5",
    label: none,
    title: [Buiten de scope van het project],
    items: (
      (text: [De Robothond zal een geavanceerde #acr("LiDAR")-systemen.], prio: p4),
      (text: [De Robothond zal voorzien zijn van een Smartphone-app of cloudkoppelingen.], prio: p4),
      (text: [De Robothond zal langdurig getest worden over een periode van een paar maanden.], prio: p4),
      (text: [De Robothond zal waterdicht zijn.], prio: p4),
    ),
  ),
  
  // ===== 2.6 Minimal Viable Product =====
  (
    ID: "2.6.1",
    label: none,
    title: [MVP — Beweging],
    items: (
      (text: [De robothond zal minimaal 1 meter naar voren kunnen lopen, met een afwijking van ± 1 meter.], prio: p1),
      (text: [De robothond zal minimaal 1 meter naar achteren kunnen lopen, met een afwijking van ± 1 meter.], prio: p1),
      (text: [De robothond beschikt #acr("IK") voor de bewegingen van de robot actuatoren.], prio: p1),
    ),
  ),
  (
    ID: "2.6.2",
    label: none,
    title: [MVP — Embedded firmware],
    items: (
      (text: [De robothond heeft minimaal een eigen geschreven library voor een communicatie protocol.], prio: p1),
      (text: [De robothond beschikt over een #acr("RTOS") die werkt op de #acr("PSoC5") architectuur.], prio: p1),
    ),
  ),
  (
    ID: "2.6.3",
    label: none,
    title: [MVP — Draadloze communicatie en regelsystemen],
    items: (
      (text: [De robothond zal bestuurbaar zijn via een Bluetooth Xbox of Playstation controller], prio: p1),
    ),
  ),
  (
    ID: "2.6.4",
    label: none,
    title: [MVP — Mechanica],
    items: (
      (text: [De robothond bevat een betrouwbaar en getest mechanisch frame volgens testplan.], prio: p1),
    ),
  ),
)
