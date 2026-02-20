#import "@preview/tablem:0.1.0" as tablem
#import "@preview/numbly:0.1.0": numbly
#import "@preview/oxifmt:0.2.1": strfmt

#import "../template.typ": *
#import "../Config/GeneralConfig.typ": *
#import "@local/academic-tools:0.1.10": *

#set text(lang: "Nl")
#show link: underline

// ============================================================
// TESTRAPPORT — ROBOTPOOT
// ============================================================

// ---------- Instelbare metadata ----------
#let meta = (
  title: "Testrapport Robotpoot",
  project: "Robothond",
  version: "1.0",
  date: datetime.today(),
  authors: ("Ruben van der Veen", "Perijn Huijser", "Daan Smit"),
  reviewers: ("R. Moedt",),
  approver: "R. Moedt",
  organization: "NHL Stenden - Embedded Systems",
  dut: "Robotpoot (1 poot als subsysteem)",
)

// ---------- Resultaat enum ----------
#let PASS = "PASS"
#let FAIL = "FAIL"
#let BLOCKED = "BLOCKED"
#let NA = "N/A"

// ---------- Test omgeving (invullen) ----------
#let env = (
  hardware: (
    "Robotpoot assembly (mechanica + motor/driver + encoder)",
    "Testfixture / montage-opstelling",
    "Voeding (type/instelling invullen)",
    "E-Stop aanwezig en getest",
  ),
  software: (
    "Firmware versie/commit: ________",
    "Driver config export: ________",
    "Logging tool: ________",
  ),
  tools: (
    "Multimeter / stroommeting",
    "IR thermometer / thermokoppel",
    "Hoekmeter / gradenboog",
    "Schuifmaat",
  ),
  notes: (
    "Kalibratie/zeroing uitgevoerd: ja/nee",
    "Testlocatie: ________",
  )
)

// ---------- Testcases database ----------
// Vul hier je tests in. Je kunt de IDs 1-op-1 uit je testplan gebruiken.
#let tests = (
  (
    id: "TC-101",
    title: "Visuele inspectie + spelingmeting (nulmeting)",
    type: "Functional",
    priority: "High",
    requirement: "N.v.t. / PvE-xxx",
    procedure: (
      "Controleer montage, scheuren, kabelrouting.",
      "Meet speling/backlash op kritieke joints (mm/°).",
      "Leg foto’s vast.",
    ),
    expected: "Geen scheuren; correcte montage; speling binnen acceptabel (waarde documenteren).",
    // ---- Uitvoering invullen ----
    result: PASS,
    measured: "Speling: 0.6° (joint2), 0.4° (joint3)",
    evidence: "Foto’s: /logs/TC-101/ ; Meetlog: /logs/TC-101/speling.csv",
    issues: "Geen",
    notes: "—",
    date: "2026-01-14",
    tester: "R. van der Veen",
  ),
  (
    id: "TC-201",
    title: "Max/min joint-hoeken bereiken (soft-limits actief)",
    type: "Functional",
    priority: "High",
    requirement: "PvE-xxx",
    procedure: (
      "Activeer soft-limits.",
      "Stuur joint naar min en max hoeken met lage snelheid.",
      "Controleer dat er geen overschrijding/aanlopen is.",
    ),
    expected: "Bereik gehaald zonder overschrijding; limieten begrenzen correct.",
    result: FAIL,
    measured: "Joint2 max bereikt: 78° (verwacht 85°)",
    evidence: "Log: /logs/TC-201/ ; Video: /logs/TC-201/video.mp4",
    issues: "Issue #12: mechanisch aanlopen kabel bij 80°",
    notes: "Kabelrouting aanpassen en opnieuw testen.",
    date: "2026-01-14",
    tester: "P. Huijser",
  ),
  (
    id: "TC-801",
    title: "E-Stop functionaliteit (tijdens beweging en tijdens hold)",
    type: "Safety",
    priority: "High",
    requirement: "PvE-xxx (veiligheid)",
    procedure: (
      "Start beweging (lage snelheid).",
      "Druk E-Stop tijdens beweging.",
      "Herhaal tijdens ‘hold pose’.",
      "Controleer: motor output uit + foutstatus gelogd.",
    ),
    expected: "Actuatie stopt onmiddellijk; systeem gaat naar veilige toestand.",
    result: PASS,
    measured: "Stop-tijd: < 150 ms (op basis van log timestamps)",
    evidence: "Log: /logs/TC-801/ ; Scope: /logs/TC-801/scope.png",
    issues: "—",
    notes: "E-Stop werkt consistent, 5/5 herhalingen.",
    date: "2026-01-14",
    tester: "D. Smit",
  ),
)

// ============================================================
// HELPER FUNCTIES (samenvatting / tabellen / details)
// ============================================================

#let count_where(arr, pred) = {
  let c = 0
  for it in arr {
    if pred(it) { c += 1 }
  }
  c
}

#let total = tests.len()
#let pass_n = count_where(tests, it => it.result == PASS)
#let fail_n = count_where(tests, it => it.result == FAIL)
#let blocked_n = count_where(tests, it => it.result == BLOCKED)
#let na_n = count_where(tests, it => it.result == NA)

#let pass_rate = if (total - na_n) > 0 {
  ((pass_n) / (total - na_n) * 100)
} else { 0 }

// Go/No-Go: streng op High priority FAIL/BLOCKED
#let high_fail = count_where(tests, it => it.priority == "High" and (it.result == FAIL or it.result == BLOCKED))
#let go_nogo = if high_fail == 0 { "GO" } else { "NO-GO" }

// -------- Table renderers --------
#let env_block(title, items) = [
  *#title*\
  #v(0.2em)
  - #items.join("\n- ")
  #v(0.8em)
]

#let results_overview_table(tests) = {
  tablem.table(
    columns: (1.2fr, 2.6fr, 1.2fr, 1fr, 1fr, 1.4fr),
    inset: 6pt,
    align: (left, left, left, center, center, left),
    header: (
      [*ID*], [*Testcase*], [*Type*], [*Prio*], [*Result*], [*Issue/Notes*]
    ),
    rows: tests.map(it => (
      [#it.id],
      [#it.title],
      [#it.type],
      [#it.priority],
      [#it.result],
      [#it.issues],
    )),
  )
}

#let testcase_detail(it) = [
  == #(it.id) — #(it.title)

  *Meta*\
  - Type: #(it.type)\
  - Prioriteit: #(it.priority)\
  - Requirement: #(it.requirement)\
  - Tester: #(it.tester)\
  - Datum: #(it.date)\

  *Procedure*\
  #v(0.2em)
  #enum(it.procedure.map(step => [#step]))

  *Expected*\
  #(it.expected)

  *Uitkomst*\
  - Resultaat: *#(it.result)*\
  - Gemeten: #(it.measured)\
  - Evidence: #(it.evidence)\
  - Issues: #(it.issues)\
  - Notities: #(it.notes)\

  #v(0.8em)
]

// ============================================================
// DOCUMENT BODY
// ============================================================

// (Optioneel) gebruik je eigen template cover als je die hebt.
// Anders: simpele titelpagina:
#align(center)[
  #text(size: 20pt, weight: "bold")[#(meta.title)]
  #v(0.6em)
  #(meta.project)\
  #v(0.6em)
  Versie #(meta.version) — #(meta.date)\
  #v(1.2em)
  *DUT:* #(meta.dut)\
  #v(1.2em)
  *Auteurs:* #(meta.authors.join(", "))\
  #v(0.2em)
  *Reviewers:* #(meta.reviewers.join(", "))\
  #v(0.2em)
  *Approver:* #(meta.approver)\
]

#pagebreak()

= Managementsamenvatting

Dit testrapport beschrijft de uitgevoerde verificatie en validatie tests op de robotpoot (DUT). De focus ligt op veiligheid, reproduceerbaarheid en demo-gereedheid.

#v(0.8em)
*Resultaten in het kort:*\
- Totaal testcases: *#total*\
- Pass: *#pass_n*\
- Fail: *#fail_n*\
- Blocked: *#blocked_n*\
- N/A: *#na_n*\
- Pass rate (excl. N/A): *#pass_rate%*\
- High priority FAIL/BLOCKED: *#high_fail*\
- Beslissing: *#go_nogo*\

#v(0.8em)
*Conclusie (invullen):*\
(1–2 alinea’s: wat is goed, wat blokkeert, wat is de actie richting demo.)

= Testomgeving

#env_block("Hardware", env.hardware)
#env_block("Software", env.software)
#env_block("Meetmiddelen", env.tools)
#env_block("Opmerkingen", env.notes)

= Testresultaten Overzicht

#results_overview_table(tests)

#v(0.8em)
*Opmerkingen bij afwijkingen:*\
- Beschrijf hier de belangrijkste fails/risico’s en welke retests nodig zijn.

= Testdetails

// Render alle testcase details
#for it in tests {
  testcase_detail(it)
}

= Issues & Afwijkingen

== Openstaande issues
- Issue #__: omschrijving, impact, status, owner, ETA (geen tijdschatting nodig in document, alleen planning/volgende stap)

== Afwijkingen t.o.v. testplan
- (Bijv. test niet uitgevoerd door ontbrekend meetmiddel, andere opstelling, etc.)

= Aanbevelingen & Retest Plan

- Prioriteit 1 (veiligheid): …
- Prioriteit 2 (positionering/repeatability): …
- Retest selectie: TC-___, TC-___

= Go/No-Go

*Beslissing:* *#go_nogo*\
*Onderbouwing:*\
- High priority issues: #(high_fail)\
- Kritieke safety tests: (verwijs naar TC-801 etc.)\
- Demo-risico: (kort)

= Goedkeuring

#v(1.5em)
#grid(
  columns: (1fr, 1fr),
  column-gutter: 2em,
  row-gutter: 2.5em,

  [
    *Naam:* #(meta.approver)\
    *Rol:* Approver\
    *Handtekening:*\
    *Datum:*
  ],

  [
    *Naam:* (Reviewer invullen)\
    *Rol:* Reviewer\
    *Handtekening:*\
    *Datum:*
  ],
)
