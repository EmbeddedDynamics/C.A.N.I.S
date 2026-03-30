# C.A.N.I.S. — Project Documentation

> **Typst documentation for the CANIS Robothond project · NHL Stenden BD.ELT · 2025/2026**

This directory contains the full project documentation written in [Typst](https://typst.app). The document follows the **Design Thinking** methodology and covers all project phases from stakeholder analysis through technical prototype and test results.

---

## Compiling
- Using Typst:
  1. Create a document inside [Typst](https://typst.app)
  2. Import all documents and directories inside this typst/ directory
  3. Install the dependencies listed further in this document
  4. Click the eye-icon next to the document to compile

- Or with commands:
```bash
# Compile the main document to PDF
typst compile Documenten/Hoofddocument.typ

# Watch for changes and recompile automatically
typst watch Documenten/Hoofddocument.typ
```

**Dependencies:**

- [Typst](https://typst.app) ≥ 0.13
- Local package `@local/academic-tools:0.1.20` — install from the [academic-tools repository](https://github.com/EmbeddedDynamics/typst-academic-tools) into your local Typst package directory
- Preview packages (resolved automatically by Typst):
  - `@preview/oxifmt:0.2.1` — string formatting
  - `@preview/gantty:0.5.1` — Gantt chart
  - `@preview/fletcher:0.5.8` — diagrams
  - `@preview/unify:0.7.1` — unit formatting (used in research papers)

---

## Document Structure

All source lives under `Documenten/`. The entry point is `Hoofddocument.typ`, which imports everything via `Config/_Imports.typ`.

```
Documenten/
├── Hoofddocument.typ            # Entry point — compiles the full document
│
├── Config/
│   ├── _Imports.typ             # Central import list — add new subdocuments here
│   ├── GeneralConfig.typ        # Team members, acronyms, materials list, project metadata
│   ├── Planning.typ             # Gantt planning data
│   ├── PvE_MoSCoW.typ           # Requirements and MoSCoW prioritisation
│   └── Role_Calculations.typ    # Role definitions and cost calculation helpers
│
├── Subdocumenten/               # Main content sections (included in Hoofddocument)
│   ├── Plan_Van_Aanpak.typ      # Project plan (PvA)
│   ├── Blokdiagram (Hoog over).typ  # High-level system block diagram
│   ├── Stakeholder_analyse.typ  # Stakeholder analysis
│   ├── Samenwerkingscontract.typ    # Team collaboration contract
│   ├── Persona_Onderzoek.typ    # Persona research — "Lars de Vries" (Empathize phase)
│   └── Prototype.typ            # Technical prototype description (Prototype phase)
│
├── Onderzoeken/                 # Research documents
│   ├── Literatuuronderzoek.typ  # Literature review
│   ├── Transmissie.typ          # Transmission / capstan research (by Perijn)
│   ├── AI_onderzoek.typ         # AI depth perception & speech recognition research (by Ruben)
│   └── Onderzoek IK.typ         # Inverse kinematics research
│
├── Reflecties/                  # Individual project reflections
│   ├── Reflectie_Ruben.typ
│   ├── Reflectie_Perijn.typ
│   └── Reflectie_Daan.typ
│
├── Leerdoelen/                  # Individual learning objectives
│   └── Leerdoel_Daan.typ
│
├── Notities/                    # Working notes (not included in main document)
│   └── PCB_notities.typ         # PCB design notes
│
├── Peerreview/                  # Peer review documents
│   ├── Peerreview.typ
│   └── Peerreview_review.typ
│
├── Assets/                      # Images and logos
│   ├── Logo/
│   │   ├── NHL_logo.jpg
│   │   └── ED_Logo.png
│   └── Persona/
│       └── lars.png             # Persona photo for Lars de Vries
│
└── Archive/                     # Deprecated drafts — do not include in builds
```

---

## Document Outline

The compiled document follows the Design Thinking phases:

| Chapter | Content |
|---|---|
| **Organisatie** | Team structure, roles, timeline, Gantt planning, communication plan |
| **Inleving** | Stakeholder analysis, persona research (Lars de Vries) |
| **Definitie** | PvE requirements, MoSCoW analysis, MVP definition, block diagram |
| **Ideeën genereren** | Feature list, Plan van Aanpak, literature research |
| **Prototype** | Technical realization — architecture, mechanics, electronics, firmware, Jetson AI |
| **Test** | Capstan wear test, robot leg functionality (TC-201 resolved), firmware validation |
| **Conclusie** | Project outcome, status at delivery, recommendations for further development |
| **Appendix** | Cost calculations (labour + materials), collaboration contract |

---

## Configuration

### Team, project metadata, and acronyms — `Config/GeneralConfig.typ`

All team-level data lives here. To update a team member, edit the `students` array. To add an acronym, add an entry to `acronyms_db`:

```typst
#let acronyms_db = (
  "CAN":  ("Controller Area Network", ""),
  "IMU":  ("Inertial Measurement Unit", ""),
  // add new entries here
)
```

Use `#acr("KEY")` anywhere in the document body to insert the abbreviation. Acronyms are expanded on first use and abbreviated thereafter.

> **Important:** `#acr()` requires that `init-acronyms()` has already run. This happens automatically via `#show: academic-frontpage.with(acronyms: acronyms_db)` at the top of `Hoofddocument.typ`. Never evaluate content containing `#acr()` calls inside a `#let` at import time — always use a function (`#let body() = [...]`) or a direct `#include` in the document body.

### Materials list — `Config/GeneralConfig.typ`

The `Materials` array in `GeneralConfig.typ` feeds the materials cost table in the Appendix. Each entry follows:

```typst
(name: "...", ordernr: "...", brand: "...", supplier: "...",
 link: "...", amount: 1, unit: "st", unitprice: 0.00),
```

### Planning — `Config/Planning.typ`

The Gantt chart data is defined here and rendered with `#gantt(planning)` on a landscape page in the Organisatie chapter.

---

## Adding a New Section

1. Create a new `.typ` file in the appropriate subdirectory (`Subdocumenten/`, `Onderzoeken/`, or `Testen/`).
2. Define its content inside a function, e.g. `#let my_section_body() = [ ... ]`.
3. Import the function in `Config/_Imports.typ`:
   ```typst
   #import "../Subdocumenten/MySection.typ": my_section_body
   ```
4. Call it in `Hoofddocument.typ` at the right position:
   ```typst
   = Mijn Sectie
   #my_section_body()
   ```

Alternatively, use a direct `#include` for files that do not need to export reusable symbols:
```typst
= Mijn Sectie
#include "Subdocumenten/MySection.typ"
```

---

*NHL Stenden University of Applied Sciences — BD.ELT — 2025/2026*
