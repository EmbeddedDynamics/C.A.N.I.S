// ============================================
// Complete config for the planning of 
// the project
// ============================================

// Import Templates for Acronyms
#import "GeneralConfig.typ": *


// ============================================
// Helpers: datumformat + zoeken
// ============================================

// Format: "YYYY-MM-DD" -> "DD-MM-YYYY"
#let fmt-date(d) = {
  let parts = d.split("-")
  if parts.len() != 3 { d } else { parts.at(2) + "/" + parts.at(1) + "/" + parts.at(0) }
}

// Vind een task op basis van task-naam
#let find-task(planning, task-name) = {
  for t in planning.tasks {
    if t.name == task-name { return t }
  }
  none
}

// Bepaal fase-start/eind op basis van min/max van subtasks
#let task-range(task) = {
  let starts = task.subtasks.map(s => s.start)
  let ends   = task.subtasks.map(s => s.end)

  // ISO date strings sorteren lexicografisch werkt ("YYYY-MM-DD")
  let start = starts.sorted().first()
  let end   = ends.sorted().last()
  (start: start, end: end)
}


// ============================================
// Render functies
// ============================================

// Render 1 fase (task): titel + datumrange (afgeleid uit subtasks)
#let render-task(planning, task-name) = {
  let task = find-task(planning, task-name)
  if task == none {
    strong([Onbekende fase: ]) + task-name
  } else {
    let r = task-range(task)
    text(size: 16pt)[*#task.name*\ ]
    // compacte meta-regel
    emph([Periode: #fmt-date(r.start) – #fmt-date(r.end)])
  }
}

// Render subtasks van een fase, optioneel filter op ids (handig voor selectie)
#let render-subtasks(planning, task-name, only-ids: none, show-id: false) = {
  let task = find-task(planning, task-name)
  if task == none {
    [Geen subtasks: fase niet gevonden.]
  } else {
    let subs = if only-ids == none {
      task.subtasks
    } else {
      task.subtasks.filter(s => s.contains("id") and only-ids.contains(s.id))
    }

    // mooi lijstje met datumrange
    set par(leading: 0.95em, spacing: 0.25em)
    list(
      ..subs.map(s => [
        #strong[[#fmt-date(s.start) – #fmt-date(s.end)] #s.name]\
        #v(0.6em)
        #strong[Beschrijving:] #s.description\
        #strong[Resultaten:]   #s.result
        #v(1em)
      ])
    )
  }
}


// ============================================
// Planning Database: 
// Robothond planning (From Notion)
// Datestyle: "YYYY-MM-DD"
// ============================================

#let planning = (
  // Show todays date
  show-today: true,

  // Header rules
  headers: ("month", "week"),

  // Timeframe of entire Gantt
  start: "2025-11-03",
  end:   "2026-04-01",

  tasks: (
    (
      name: "Emphatize",
      subtasks: (
        (
          name: "Stakeholderanalyse", id: "stake",
          start: "2025-11-16", end: "2025-11-26",
          description: [Het identificeren van stakeholders, hun belangen en invloed.],
          result: [Een stakeholdermatrix met samenvatting van de belangrijkste behoeftes en een conclusie waarin toegelicht wordt wat er daadwerkelijk toegepast wordt.],
        ),
      ),
    ),
    (
      name: "Define",
      subtasks: (
        (
          name: "Pakket van Eisen", id: "pve",
          start: "2025-11-11", end: "2025-12-05",
          description: [Opstellen en valideren van het Pakket van Eisen op basis van de Stakeholderanalyse.],
          result: [Lijst met alle eisen, SMART geformuleerd.],
        ),
        (
          name: "MoSCoW-analyse", id: "moscow",
          start: "2025-11-24", end: "2025-12-05",
          description: [Prioriteren van eisen en functies volgens #acr("MoSCoW")-categorieën.],
          result: [Een tabel met duidelijke #acr("MoSCoW")-prioriteiten met kleur codering.],
        ),
        (
          name: "Blokdiagram (hoog-over)", id: "block-hi",
          start: "2025-11-13", end: "2025-12-05",
          description: [Maken van een systeem-overzicht met hoofdcomponenten onderverdeeld.],
          result: [Blokdiagram dat de basis vormt voor de integratie van componenten.],
        ),
      ),
    ),
    (
      name: "Ideate",
      subtasks: (
        (
          name: "Capstan-drive – 1ste Prototype", id: "cap1",
          start: "2025-12-01", end: "2025-12-05",
          description: [Eerste concept van de capstan-aandrijving ontwerpen en bouwen om slip en montagehaalbaarheid te testen.],
          result: [Prototype met korte testnotities en verbeterpunten voor iteratie 2.],
        ),
        (
          name: "Productideeën presenteren", id: "prod-pres",
          start: "2025-12-05", end: "2025-12-16",
          description: [Presenteren van conceptkeuzes en afwegingen aan docent om gerichte feedback op te halen.],
          result: [Presentatie in pecha-kucha stijl, waarin afwegingen van meerdere componenten worden beargumenteerd.],
        ),
        (
          name: "Peer review", id: "peer",
          start: "2025-12-15", end: "2025-12-19",
          description: [Reviewen van het #acr("PvE") en blokdiagram van een andere groep op duidelijkheid, haalbaarheid en traceerbaarheid.],
          result: [Peerreviewdocument met verbeterpunten en sterke punten.],
        ),
        (
          name: "Plan van Aanpak", id: "pva",
          start: "2025-12-05", end: "2025-12-19",
          description: [Opstellen van het #acr("PvA") met probleemstelling, aanpak, planning, risico’s en kwaliteitsborging.],
          result: [Ingeleverd #acr("PvA") met versiebeheer en onderbouwing van keuzes.],
        ),
        (
          name: "Inverse kinematics onderzoek", id: "ik",
          start: "2025-11-15", end: "2025-12-12",
          description: [Onderzoeken en kiezen van #acr("IK") passend bij de robotpoot.],
          result: [Gekozen methode toegelicht met calculaties.],
        ),
        (
          name: "Literatuuronderzoeken", id: "lit",
          start: "2025-11-29", end: "2026-01-16",
          description: [Onderzoeken van verschillende concepten die belang hebben bij de onderbouwing van de robothond. Bevat keuzeverantwoording en vergelijkingen met alternatieven.],
          result: [Voor elk concept een netjes gestructureerd document dat is voorzien van vergelijkingen met alternatieven en keuzeverantwoording. Bronnen zijn goed vermeld en betrouwbaar.],
        ),
      ),
    ),
    (
      name: "Prototype – fase 1",
      subtasks: (
        (
          name: "Capstan-drive - 2de Prototype", id: "cap2",
          start: "2025-12-06", end: "2025-12-16",
          description: [Verbeterde capstan bouwen op basis van bevindingen uit prototype 1.],
          result: [2de prototype van de capstan met testplan en meetresultaten zoals: slip, slijtage en temperatuur).],
        ),
        (
          name: "Robotpoot - 1ste Prototype", id: "leg1",
          start: "2025-12-22", end: "2026-01-09",
          description: [Eerste robotpoot-assemblage bouwen en laten bewegen met basisbesturing.],
          result: [Een werkend prototype met minimaal één gecontroleerde beweging en een korte duurtest.],
        ),
        (
          name: "Compleet Prototype, fase 1", id: "proto-f1",
          start: "2025-12-22", end: "2026-01-20",
          description: [Integreren van de poten, het frame, en simpele besturing om een werkend principe te realiseren.],
          result: [Geïntegreerde demo met integratietest om te valideren dat het geheel functioneert naar verwachting.],
        ),
      ),
    ),
    (
      name: "Test – fase 1",
      subtasks: (
        (
          name: "MVP Demo proof of concept", id: "mvp",
          start: "2026-01-21", end: "2026-02-03",
          description: [Aantonen dat de #acr("MVP") functionaliteit voldoet aan Must-haves die gedefinieerd zijn in het #acr("PvE").],
          result: [Demo waarin wordt aangetoond dat het eindproduct haalbaar is en er voldaan is aan de opgestelde eisen voor het #acr("MVP").],
        ),
      ),
    ),
    (
      name: "Prototype – fase 2",
      subtasks: (
        (
          name: "Extra functies toepassen", id: "xtra",
          start: "2026-02-04", end: "2026-03-02",
          description: [Verbeteren van het huidige systeem en toevoegen van Should/Could features zonder de stabiliteit van het #acr("MVP") te breken.],
          result: [Werkende extra functionaliteit vastgelegd met regressietests en bijgewerkte documentatie door middel van een duidelijke lijst.],
        ),
        (
          name: "Compleet Prototype, fase 2", id: "proto-f2",
          start: "2026-03-03", end: "2026-03-15",
          description: [Systeem afronden tot een presenteerbaar prototype met verbeterde betrouwbaarheid en documentatie.],
          result: [Laatste versie met de laatste integratie- en acceptatietesten. Voorzien van testrapporten.],
        ),
      ),
    ),
    (
      name: "Test – fase 2",
      subtasks: (
        (
          name: "Compleet product (Assessment)", id: "prod",
          start: "2026-03-16", end: "2026-03-30",
          description: [Eindvalidatie voor het assessment: aantonen dat het eindproduct voldoet aan PvE en testplan. Extra functionaliteit onderbouwen.],
          result: [Testrapport, demo en reflectie op de gestelde eisen en gemaakte keuzes.],
        ),
      ),
    ),
    (
      name: "Doorlopend",
      subtasks: (
        (
          name: "Projectdocumentatie bijhouden", id: "doc",
          start: "2025-11-03", end: "2026-03-30",
          description: [Doorlopend bijwerken van documentatie, logboek, besluiten en versiebeheer.],
          result: [Actuele projectdocumentatie met versiebeheer, wijzigingslog en vaste documentstructuur.],
        ),
      ),
    ),
  ),
)
