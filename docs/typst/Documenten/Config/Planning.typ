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
    date: 2026-02-22*/
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
)