// ============================================
// Gantt (With gantty) – Robothond planning (From Notion)
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
)