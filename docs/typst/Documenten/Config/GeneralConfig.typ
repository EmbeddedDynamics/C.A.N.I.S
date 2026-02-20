// ============================================
// General Config file with basic definition
// of data used throughout the document
// ============================================

// Imports
#import "@local/academic-tools:0.1.10": *
#import "../template.typ": *

// ============================================
// General Data
// ============================================

#let groupRoles = (
  (name: "Project leider", hourly-rate: 75.0),
  (name: "Lead AI developer", hourly-rate: 65.0),
  (name: "Lead hardware engineer", hourly-rate: 65.0),
  (name: "GIT master", hourly-rate: 55.0),
  (name: "Lead software developer", hourly-rate: 65.0),
  (name: "Notulist", hourly-rate: 55.0),
)

#let peopleAssingments = (
  (name: "Ruben", role: "Project leider", hours: 150.0),
  (name: "Ruben", role: "Lead AI developer", hours: 150.0),
  (name: "Perijn", role: "Lead hardware engineer", hours: 250.0),
  (name: "Perijn", role: "GIT master", hours: 25.0),
  (name: "Daan", role: "Lead software developer", hours: 250.0),
  (name: "Daan", role: "Notulist", hours: 40.0),
)

// Data defines
#let students = (
  (name: "Ruben van der Veen", email: "ruben.van.der.veen1@student.nhlstenden.com", tel: "06 36578836", role: "Project Leider\nAI Developer", responsibilities: "Coördinatie en planning \nAI development"),
  (name: "Perijn Huijser", email: "perijn.huijser@student.nhlstenden.com", tel: "06 30108514", role: "Hardware Engineer \nGit Master", responsibilities: "Circuit design en PCB integratie \nCAD engineering"),
  (name: "Daan Smit", email: "daan.Smit3@student.nhlstenden.com", tel: "06 57294064", role: "Notulist \nSoftware Developer", responsibilities: "Firmware development \nControl systems engineering"),
)

#let city = "Leeuwarden"
#let country = "Nederland"

#let university = "NHL Stenden"
#let universitydesc = "University of Applied Sciences"
#let department = "Afdeling van Elektrotechniek"
#let degree = "Bachelor elektrotechniek"
#let program = "BD.ELT"

#let startdate = "11-11-2025"
#let enddate  = "03-04-2026"
#let month = "November"
#let year = "2026"

#let supervisor = "Rieno Moedt"
#let tutor = "Chel-Marí Spies"


// ============================================
// Acronyms complete document
// ============================================

#let acronyms_db = (
  "PvE":   ("Pakket van Eisen", "Pakket van Eisen"),
  "MVP":   ("Minimum Viable Product", "Minimum Viable Product"),
  "PvA":   ("Plan van Aanpak", "Plan van Aanpak"),
  "IK":    ("Inverse Kinematics"),
  "PSoC5": ("Programmable System on a Chip 5"),
  "RTOS":  ("Real-Time Operating System"),
  "MoSCoW":("Must have, Should have, Could have, Won’t have"),
  "PCB":   ("Printed Circuit Board"),
  "CAD":   ("Computer Aided Design"),
  "AI":    ("Artificial Intelligence"),
  "DOF":   ("Degree of Freedom"),
  "FOC":   ("Field-Orientaded Control"),
  "PoC":   ("Proof of Concept"),
  "LiDAR": "Light Detection And Ranging",
  "ESD":   ("ElectroStatic Discharge"),
  "GPS":   ("Global Positioning System"),
  "ELRS":  ("Express Long Range System"),
  "FPV":   ("First-Person View"),
  "HDL":   ("Hardware Description Language"),
  "IMU":   ("Inertial Measurement Uni"),
  "PID":   ("Proportional–Integral–Derivative"),
)


// ============================================
// Material cost
// ============================================

#let Materials = (
  (name: "PSoC 5 dev board", ordernr: "448-CY8CKIT-059-ND", brand: "Infineon", supplier: "DigiKey", amount: 2, unit: "Stuks", unitprice: 18.23, link: "https://www.digikey.nl/nl/products/detail/infineon-technologies/CY8CKIT-059/5184557?gclsrc=aw.ds&gad_source=1&gad_campaignid=20226276216/"),

 (name: "6210 Zwitserse Motor", ordernr: "N.V.T.", brand: "Svvissmotor Store", supplier: "Aliexpress", amount: 12, unit: "Stuks", unitprice: 31.19 / 1.21 * 1, link: "https://nl.aliexpress.com/item/1005010041210790.html?spm=a2g0o.order_list.order_list_main.4.38f979d2whk6Dq&gatewayAdapt=glo2nld"),

 (name: "MKS XDRIVE Mini", ordernr: "N.V.T.", brand: "Makerbase", supplier: "Aliexpress", amount: 12, unit: "Stuks", unitprice: 30.19 / 1.21 * 1, link: "https://nl.aliexpress.com/item/1005008096636346.html?spm=a2g0o.order_list.order_list_main.11.38f979d2whk6Dq&gatewayAdapt=glo2nld"),

 (name: "PC-ABS Filament", ordernr: "N.V.T.", brand: "Sunlu", supplier: "Sunlu", amount: 1, unit: "kg", unitprice: 35.10 / 1.21 * 1, link: "https://sunlu.com/products/pc-abs-3d-printer-filament-high-performance-3d-filament-1kg"),
)


// ============================================
// Colors
// ============================================

#let COL = (
  purple:     rgb(147, 51, 234),  // Purple
  green:      rgb(34, 197, 94),   // Green
  red:        rgb(239, 68, 68),   // Red
  orange:     rgb(249, 115, 22),  // Orange
  blue:       rgb(69, 137, 201),  // Blue
  lightblue:  rgb(59, 130, 246),  // Lightblue
  yellow:     rgb(255, 214, 92),  // Yellow
)


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
  acronyms: (acronyms_db),
  versions: (
    version(
      committee: [R. van der Veen],
      description: [Initiële commit],
      date: "xx",
      level: 3,
    ),
  )
)