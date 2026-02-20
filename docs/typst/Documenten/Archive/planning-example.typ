/*
#import "planning-lib.typ": planning-document, task, resource, phase, default-config

// ============================================================
// EXAMPLE: PROJECT PLANNING DOCUMENT
// Using the planning-lib.typ library
// ============================================================

// Define the Gantt chart
#import "Documenten/Config/Planning.typ": gantt-chart

// Define project phases with tasks and resources
#let task-data = (
  phase(
    "Phase 1: Requirements & Planning",
    rgb("#E8F4F8"),
    "Dec 1-14 (2 weeks)",
    "Project Lead",
    (
      task(
        "Stakeholder Interviews",
        "Dec 1-8",
        "Alice Chen",
        status: "in-progress",
        subtasks: (
          "Initial kickoff meeting (Dec 1-2)",
          "Business stakeholder interviews (Dec 3-5)",
          "Technical stakeholder interviews (Dec 6-8)",
        ),
        resources: (
          resource("Alice Chen", 80),
          resource("Project Lead", 20),
        ),
        risk: "low",
      ),
      task(
        "Requirements Documentation",
        "Dec 8-14",
        "Bob Martinez",
        status: "planned",
        subtasks: (
          "Compile interview notes (Dec 8-9)",
          "Write functional requirements (Dec 9-12)",
          "Get stakeholder approval (Dec 12-14)",
        ),
        resources: (
          resource("Bob Martinez", 100),
          resource("Alice Chen", 30),
        ),
        risk: "medium",
      ),
    )
  ),
  phase(
    "Phase 2: Design & Architecture",
    rgb("#FFF4E6"),
    "Dec 15 - Jan 4 (3 weeks)",
    "Design Lead",
    (
      task(
        "System Architecture",
        "Dec 15-22",
        "Charlie Dev",
        status: "planned",
        subtasks: (
          "Define system components (Dec 15-17)",
          "Create architecture diagrams (Dec 17-20)",
          "Architecture review & feedback (Dec 20-22)",
        ),
        resources: (
          resource("Charlie Dev", 100),
          resource("Design Lead", 40),
        ),
        risk: "critical",
      ),
      task(
        "Database Design",
        "Dec 22 - Dec 29",
        "Diana Lee",
        status: "planned",
        subtasks: (
          "Data model analysis (Dec 22-24)",
          "Schema design (Dec 24-27)",
          "Performance optimization (Dec 27-29)",
        ),
        resources: (
          resource("Diana Lee", 100),
          resource("Frank Kumar", 50),
        ),
        risk: "critical",
      ),
      task(
        "UI/UX Mockups",
        "Dec 29 - Jan 4",
        "Eva Rossi",
        status: "planned",
        subtasks: (
          "Wireframe creation (Dec 29 - Jan 1)",
          "High-fidelity mockups (Jan 1-3)",
          "Design system documentation (Jan 3-4)",
        ),
        resources: (
          resource("Eva Rossi", 90),
        ),
        risk: "low",
      ),
    )
  ),
  phase(
    "Phase 3: Implementation",
    rgb("#E8F5E9"),
    "Jan 5 - Feb 8 (5 weeks)",
    "Engineering Lead",
    (
      task(
        "Backend Development",
        "Jan 5-26",
        "Frank Kumar",
        status: "planned",
        subtasks: (
          "API endpoint implementation (Jan 5-12)",
          "Database integration (Jan 12-19)",
          "Authentication & security (Jan 19-26)",
        ),
        resources: (
          resource("Frank Kumar", 100),
          resource("Iris Patel", 40),
        ),
        risk: "critical",
      ),
      task(
        "Frontend Development",
        "Jan 12 - Feb 2",
        "Grace Wong",
        status: "planned",
        subtasks: (
          "Component library setup (Jan 12-19)",
          "Page implementation (Jan 19-26)",
          "Integration with backend (Jan 26 - Feb 2)",
        ),
        resources: (
          resource("Grace Wong", 100),
          resource("Karen Li", 80),
        ),
        risk: "medium",
      ),
      task(
        "Integration & Testing",
        "Feb 2-8",
        "Henry Zhao",
        status: "planned",
        subtasks: (
          "End-to-end testing (Feb 2-5)",
          "Performance testing (Feb 5-7)",
          "Bug fixes & final checks (Feb 7-8)",
        ),
        resources: (
          resource("Henry Zhao", 100),
          resource("Iris Patel", 60),
        ),
        risk: "critical",
      ),
    )
  ),
  phase(
    "Phase 4: Testing & Deployment",
    rgb("#FCE4EC"),
    "Feb 9-22 (2 weeks)",
    "QA Lead",
    (
      task(
        "QA & Bug Fixes",
        "Feb 9-15",
        "Iris Patel",
        status: "planned",
        subtasks: (
          "Comprehensive QA testing (Feb 9-12)",
          "User acceptance testing (Feb 12-14)",
          "Critical bug fixes (Feb 14-15)",
        ),
        resources: (
          resource("Iris Patel", 100),
          resource("Jack Smith", 50),
        ),
        risk: "critical",
      ),
      task(
        "Deployment Preparation",
        "Feb 15-22",
        "Jack Smith",
        status: "planned",
        subtasks: (
          "Production environment setup (Feb 15-18)",
          "Deployment runbook creation (Feb 18-20)",
          "Deployment execution & verification (Feb 20-22)",
        ),
        resources: (
          resource("Jack Smith", 100),
          resource("Engineering Lead", 30),
        ),
        risk: "high",
      ),
    )
  ),
)

// Create custom config (optional - use defaults if you want)
#let my-config = (
  ..default-config,
  title: "Enterprise Project Planning",
  enable-gantt: true,
  enable-breakdown: true,
  enable-resources: false,
  enable-critical-path: false,
)

// Generate the document
#planning-document(config: my-config, gantt-chart, task-data)
