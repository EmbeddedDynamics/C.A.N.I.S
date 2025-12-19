// ----------------------------------------------------
// Gantt (With gantty) – Robothond planning (From Notion)
// Datestyle: "YYYY-MM-DD"
// ----------------------------------------------------

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
      name: "Empathize",
      subtasks: (
        (name: "Stakeholderanalyse",
        start: "2025-11-16", end: "2025-11-26"),
      ),
    ),
    (
      name: "Define",
      subtasks: (
        (name: "PvE", id: "pve",
        start: "2025-11-11", end: "2025-12-05"),
        (name: "MoSCoW-analyse",
        start: "2025-11-24", end: "2025-12-05"),
        (name: "Blokdiagram (hoog-over)", id: "block-hi",
        start: "2025-11-13", end: "2025-12-05"),
      ),
    ),
    (
      name: "Ideate",
      subtasks: (
        (name: "Capstan-drive – 1ste Prototype",id: "cap1",
        start: "2025-12-01", end: "2025-12-05"),
        (name: "Productideeën presenteren",
        start: "2025-12-05", end: "2025-12-16"),
        (name: "Peer review",
        start: "2025-12-15", end: "2025-12-19"),
        (name: "PvA", id: "pva",
        start: "2025-12-05", end: "2025-12-19"),
        (name: "Inverse kinematics onderzoek",id: "ik", 
        start: "2025-11-15", end: "2025-12-12"),
        (name: "Literatuuronderzoeken",id: "lit", 
        start: "2025-11-29", end: "2026-01-16"),
      ),
    ),
    (
      name: "Prototype – fase 1",
      subtasks: (
        (name: "Capstan-drive - 2de Prototype",id: "cap2",
        start: "2025-12-06", end: "2025-12-16"),
        (name: "Robotpoot - 1ste Prototype", id: "leg1",
        start: "2025-12-22", end: "2026-01-09"),
        (name: "Compleet Prototype, fase 1",
        start: "2025-12-22", end: "2026-01-20"),
      ),
    ),
    (
      name: "Test – fase 1",
      subtasks: (
        (name: "MVP Demo proof of concept",
        start: "2026-01-21", end: "2026-02-03"),
      ),
    ),
    (
      name: "Prototype – fase 2",
      subtasks: (
        (name: "Extra functies toepassen",
        start: "2026-02-04", end: "2026-03-02"),
        (name: "Compleet Prototype, fase 2",
        start: "2026-03-03", end: "2026-03-15"),
      ),
    ),
    (
      name: "Test – fase 2",
      subtasks: (
        (name: "Compleet product (Assesment)",
        start: "2026-03-16", end: "2026-03-30"),
      ),
    ),
    (
      name: "Doorlopend",
      subtasks: (
        (name: "Projectdocumentatie bijhouden",
         start: "2025-11-03", end: "2026-03-30"),
      ),
    ),
  ),

  /* --------------------
  // MILESTONES (Currently unused)
  // --------------------
  milestones: (
    (name: "PvE afgerond (intern)",date: "2025-11-28", show-date: true),
    (name: "PvA afgerond (intern)",date: "2026-01-10", show-date: true),
    (name: "MVP PoC demo",date: "2026-01-30", show-date: true),
    (name: "Inlevering: lijst met extra’s",date: "2026-02-04", show-date: true),
    (name: "Inlevering: gedetailleerd blokdiagram",date: "2026-02-06", show-date: true),
    (name: "Assessment / einddemo",date: "2026-03-28", show-date: true),
  ),*/
  
)


/*#"11-11-2025"
#let enddate  = "03-04-2026"

tasks:
  # Fases
  - name: Fase 1 - Empathize
    intervals:
      - start: 2025-11-11
        end: 2025-11-26
    subtasks:
      - name: Organisatie opzetten
        start: 2025-11-11
        end: 2025-11-15

      - name: Software opzetten
        start: 2025-11-11
        end: 2025-11-15

      - name: Initiele kostberekening
        start: 2025-11-16
        end: 2025-11-20
        
      - name: Stakeholder interviews
        start: 2025-11-13
        end: 2025-11-23

  - name: Fase 2 - Define
    intervals:
      - start: 2025-11-23
        end: 2025-12-07
    subtasks:
      - name: System architecture
        start: 2025-12-15
        end: 2025-12-22
      - name: Database design
        start: 2025-12-22
        end: 2025-12-29
      - name: UI/UX mockups
        start: 2025-12-29
        end: 2026-01-04

  - name: Fase 3 - Ideate
    intervals:
      - start: 2025-12-08
        end: 2025-12-15
    subtasks:
      - name: Backend development
        start: 2026-01-05
        end: 2026-01-26
      - name: Frontend development
        start: 2026-01-12
        end: 2026-02-02
      - name: Integration & testing
        start: 2026-02-02
        end: 2026-02-08

  - name: Fase 4 - Prototype
    intervals:
      - start: 2026-02-09
        end: 2026-02-22
    subtasks:
      - name: QA & bug fixes
        start: 2026-02-09
        end: 2026-02-15
      - name: Deployment preparation
        start: 2026-02-15
        end: 2026-02-22

  - name: Fase 5 - Test
    intervals:
      - start: 2026-02-09
        end: 2026-02-22
    subtasks:
      - name: QA & bug fixes
        start: 2026-02-09
        end: 2026-02-15
      - name: Deployment preparation
        start: 2026-02-15
        end: 2026-02-22

milestones:
  - name: Teams maken
    date: 2025-11-14
    
  - name: Design Review
    date: 2025-12-29
    
  - name: Beta Release
    date: 2026-02-02
    
  - name: Production Release
    date: 2026-02-22
//datetime(day: 1, month: 3, year: 2024)


#let gantt-chart = (
  tasks: (
  (
    name: "Fase 1: Empathize ",
    start: datetime(day: 11, month: 11, year: 2025),
    end: datetime(day: 26, month: 11, year: 2025),
    subtasks: (
      (
        name: [- Enquete maken],
        start: datetime(day: 11, month: 11, year: 2025),
        end: datetime(day: 13, month: 11, year: 2025),
      ),
      (
        name: [- Organisatie opzetten],
        start: datetime(day: 11, month: 11, year: 2025),
        end: datetime(day: 15, month: 11, year: 2025),
      ),
      (
        name: [- Software structuur\ opzetten],
        start: datetime(day: 11, month: 11, year: 2025),
        end: datetime(day: 15, month: 11, year: 2025),
      ),
      (
        name: [- Planning maken],
        start: datetime(day: 11, month: 11, year: 2025),
        end: datetime(day: 18, month: 11, year: 2025),
      ),
      (
        name: [- Initiele kostberekening],
        start: datetime(day: 16, month: 11, year: 2025),
        end: datetime(day: 20, month: 11, year: 2025),
      ),
      (
        name: [- Persona’s en\ gebruikersscenario’s],
        start: datetime(day: 16, month: 11, year: 2025),
        end: datetime(day: 22, month: 11, year: 2025),
      ),
      (
        name: [- Stakeholder interviews],
        start: datetime(day: 17, month: 11, year: 2025),
        end: datetime(day: 26, month: 11, year: 2025),
      ),
    ),
  ),
  
  (
    name: "Fase 2: Define ",
    start: datetime(day: 26, month: 11, year: 2025),
    end: datetime(day: 7, month: 12, year: 2025),
    subtasks: (
      (
        name: [- Opstellen design\ challenges],
        start: datetime(day: 16, month: 11, year: 2025),
        end: datetime(day: 28, month: 11, year: 2025),
      ),
      (
        name: [- Opstellen blockdiagram],
        start: datetime(day: 13, month: 11, year: 2025),
        end: datetime(day: 5, month: 12, year: 2025),
      ),
      (
        name: [- Opstellen PvE],
        start: datetime(day: 20, month: 11, year: 2025),
        end: datetime(day: 5, month: 12, year: 2025),
      ),
      (
        name: [- Moscow analyse],
        start: datetime(day: 1, month: 12, year: 2025),
        end: datetime(day: 4, month: 12, year: 2025),
      ),
      
    ),
  ),

  (
    name: "Fase 3: Ideate ",
    start: datetime(day: 8, month: 12, year: 2025),
    end: datetime(day: 21, month: 12, year: 2025),
    subtasks: (
      (
        name: [- Opstellen PvA],
        start: datetime(day: 24, month: 11, year: 2025),
        end: datetime(day: 5, month: 12, year: 2025),
      ),
      (
        name: [- Opstellen PvA],
        start: datetime(day: 17, month: 11, year: 2025),
        end: datetime(day: 19, month: 12, year: 2025),
      ),
    ),
  ),

  (
    name: "Fase 4: Prototype fase ",
    start: datetime(day: 9, month: 2, year: 2026),
    end: datetime(day: 22, month: 2, year: 2026),
    subtasks: (
      (
        name: [- Prototype testen],
        start: datetime(day: 19, month: 2, year: 2026),
        end: datetime(day: 22, month: 2, year: 2026),
      ),
      
      
    ),
  ),
  
  ),
*/