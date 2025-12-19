#import "../role-calculations.typ": *

// ============================================
// USAGE EXAMPLE
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
#let year = "2025"

#let supervisor = "Rieno Moedt"
#let tutor = "Chel-Marí Spies"


//----------------------------------------------------------------------------------------------------------
// Materiaal kosten
//----------------------------------------------------------------------------------------------------------

#let Materials = (
  (name: "PSoC 5 dev board", ordernr: "448-CY8CKIT-059-ND", brand: "Infineon", supplier: "DigiKey", amount: 2, unit: "Stuks", unitprice: 18.23, link: "https://www.digikey.nl/nl/products/detail/infineon-technologies/CY8CKIT-059/5184557?gclsrc=aw.ds&gad_source=1&gad_campaignid=20226276216/"),

 (name: "6210 Zwitserse Motor", ordernr: "N.V.T.", brand: "Svvissmotor Store", supplier: "Aliexpress", amount: 12, unit: "Stuks", unitprice: 31.19 / 1.21 * 1, link: "https://nl.aliexpress.com/item/1005010041210790.html?spm=a2g0o.order_list.order_list_main.4.38f979d2whk6Dq&gatewayAdapt=glo2nld"),

 (name: "MKS XDRIVE Mini", ordernr: "N.V.T.", brand: "Makerbase", supplier: "Aliexpress", amount: 12, unit: "Stuks", unitprice: 30.19 / 1.21 * 1, link: "https://nl.aliexpress.com/item/1005008096636346.html?spm=a2g0o.order_list.order_list_main.11.38f979d2whk6Dq&gatewayAdapt=glo2nld"),

 (name: "PC-ABS Filament", ordernr: "N.V.T.", brand: "Sunlu", supplier: "Sunlu", amount: 1, unit: "kg", unitprice: 35.10 / 1.21 * 1, link: "https://sunlu.com/products/pc-abs-3d-printer-filament-high-performance-3d-filament-1kg"),

 
)