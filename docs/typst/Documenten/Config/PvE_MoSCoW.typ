// ============================================
// PvE and MoSCoW database and helper functions
// ============================================

// Import Templates for Acronyms
#import "GeneralConfig.typ": *



// ============================================
// Helper functions
// ============================================

// Priority cells
#let p1 = table.cell(fill: rgb(150, 255, 150))[1] // Must
#let p2 = table.cell(fill: rgb(255, 255, 180))[2] // Should
#let p3 = table.cell(fill: rgb(255, 210, 150))[3] // Could
#let p4 = table.cell(fill: rgb(255, 150, 150))[4] // Won't


// Slice Database
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

// Render function for PvE
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

// Select PvE range to render
#let pve_range(db, start_id, end_id, headerlevel) = {
  pve_render(slice_db(db, start_id, end_id), headerlevel)
}

// Cells to hold in MoSCoW table
#let moscow_cells(db) = {
  let cells = ([*Eis / Beschrijving*], [*Prioriteit*],)
  for sec in db {
    cells = cells + ([*#sec.title*], [])
    for it in sec.items {
      cells = cells + (it.text, it.prio)
    }
  }
  cells
}

// Render MoSCoW table with predefined cells
#let moscow_table(db) = table(
  columns: (auto, auto),
  align: (left, center),
  inset: 6pt,
  stroke: 0.7pt + black,
  ..moscow_cells(db),
)

// Select MoSCoW range to render
#let moscow_range(db, start_id, end_id) = {
  let part = slice_db(db, start_id, end_id)
  moscow_table(part)
}


// ============================================
// PvE and Moscow Database
// ============================================

#let pve_moscow = (

  // ===== Verplichte eisen =====
  (
    ID: "mandatory",
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

  // ===== Functionele eisen =====
  (
    ID: "functional.1",
    label: none,
    title: [Bewegingsfuncties],
    items: (
      (text: [De robot zal minimaal 1 meter naar voren kunnen lopen, met een afwijking van ± 50 centimeter.], prio: p2),
      (text: [De robot zal minimaal 1 meter naar achteren kunnen lopen, met een afwijking van ± 50 centimeter.], prio: p2),
      (text: [De robot zal minimaal 1 meter zijwaarts kunnen lopen, met een afwijking van ± 50 centimeter.], prio: p2),
      (text: [De robot zal een extra #acr("DOF") beschikken in de roll-as.], prio: p2),
      (text: [De robot zal 360 graden om zijn eigen as heen kunnen draaien.], prio: p2),
      (text: [De robot zal 15 millimeter kunnen springen in de lucht.], prio: p2),
      (text: [De robot zal zichzelf kunnen balanceren wanneer deze op een hoek van 30 graden of lager staat, d.m.v. een #acr("IMU").], prio: p2),
      (text: [De robothond zal een eigen persoonlijkheid hebben d.m.v. unieke bewegingen.], prio: p3),
    ),
  ),
  (
    ID: "functional.2",
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
    ID: "functional.3",
    label: none,
    title: [Modulaire functies],
    items: (
      (text: [De robothond zal kunnen voorzien worden van extra externe modules.], prio: p2),
      (text: [De robothond zal modulair ontworpen worden met aluminium extrusies voor het bevestigen van externe modules.], prio: p2),
      (text: [De robothond zal modulair ontworpen worden met pogo connectoren.], prio: p2),
    ),
  ),

  // ===== Energie en performance =====
  (
    ID: "energy",
    label: <energie-eisen>,
    title: [Energie en performance],
    items: (
      (text: [De robothond zal een high-power modus beschikken die de motoren niet limiteert.], prio: p3),
      (text: [De robothond zal een low-power modus beschikken die de motoren limiteert tot 50% vermogen.], prio: p2),
      (text: [De robothond beschikt een slaap modus waarbij dit op de grond gaat liggen.], prio: p2),
      (text: [De robothond zal een #acr("AI")-limitatie modus hebben die #acr("AI") functies uitzet.], prio: p2),
    ),
  ),

  // ===== Gebruikersgerichtheid en betrouwbaarheid =====
  (
    ID: "userusability",
    label: none,
    title: [Gebruikersgerichtheid en betrouwbaarheid],
    items: (
      (text: [De robothond zal bestuurbaar zijn via een bluetooth Xbox of Playstation controller.], prio: p1),
      (text: [De robothond zal een duidelijke fysieke gebruikersinterface hebben met labels bij knoppen/schakelaars.], prio: p2),
      (text: [De robothond zal tijdens werking geen ongewenste of onverwachte bewegingen uitvoeren die afwijken van het gedefinieerde gedrag zoals vastgelegd in het testplan.], prio: p1),
      (text: [De robothond zal bruikbaar zijn voor opendagen gebaseerd op de wensen van de stakeholders. (Zie @stakeholder-analyse)], prio: p1),
    ),
  ),

  // ===== Documentatie =====
  (
    ID: "documentation",
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

  // ===== Eisen aan het ontwikkelproces =====
  (
    ID: "development",
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

  // ===== Pakket van Wensen =====
  (
    ID: "wishes",
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

  // ===== Buiten scope =====
  (
    ID: "wonthaves",
    label: none,
    title: [Buiten de scope van het project],
    items: (
      (text: [De Robothond zal een geavanceerde #acr("LiDAR")-systeem bevatten.], prio: p4),
      (text: [De Robothond zal voorzien zijn van een Smartphone-app of cloudkoppelingen.], prio: p4),
      (text: [De Robothond zal langdurig getest worden over een periode van een paar maanden.], prio: p4),
      (text: [De Robothond zal waterdicht zijn.], prio: p4),
    ),
  ),
  
  // ===== Minimal Viable Product =====
  (
    ID: "mvp.1",
    label: none,
    title: [MVP — Beweging],
    items: (
      (text: [De robothond zal minimaal 1 meter naar voren kunnen lopen, met een afwijking van ± 1 meter.], prio: p1),
      (text: [De robothond zal minimaal 1 meter naar achteren kunnen lopen, met een afwijking van ± 1 meter.], prio: p1),
      (text: [De robothond beschikt #acr("IK") voor de bewegingen van de robot actuatoren.], prio: p1),
    ),
  ),
  (
    ID: "mvp.2",
    label: none,
    title: [MVP — Embedded firmware],
    items: (
      (text: [De robothond heeft minimaal een eigen geschreven library voor een communicatie protocol.], prio: p1),
      (text: [De robothond beschikt over een #acr("RTOS") die werkt op de #acr("PSoC5") architectuur.], prio: p1),
    ),
  ),
  (
    ID: "mvp.3",
    label: none,
    title: [MVP — Draadloze communicatie en regelsystemen],
    items: (
      (text: [De robothond zal bestuurbaar zijn via een Bluetooth Xbox of Playstation controller.], prio: p1),
    ),
  ),
  (
    ID: "mvp.4",
    label: none,
    title: [MVP — Mechanica],
    items: (
      (text: [De robothond bevat een betrouwbaar en getest mechanisch frame volgens testplan.], prio: p1),
    ),
  ),
)


