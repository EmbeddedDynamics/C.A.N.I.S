// Import Packages
#import "@preview/oxifmt:0.2.1": strfmt
#import "@preview/tablem:0.1.0": tablem
#import "@preview/numbly:0.1.0": numbly
#import "@preview/gantty:0.5.1": gantt
#import "@preview/fletcher:0.5.8" as fletcher: diagram, node, edge

// Import Templates
#import "@local/academic-tools:0.1.10": *
//#import "../template.typ": *

// Import Configuration files
#import "GeneralConfig.typ": *
#import "Planning.typ": *
#import "PvE_MoSCoW.typ": *
#import "Role_Calculations.typ" as rc

// Import Subdocuments
#import "../Subdocumenten/Plan_Van_Aanpak.typ": PvA_body
#import "../Subdocumenten/Blokdiagram (Hoog over).typ": *
#import "../Subdocumenten/Stakeholder_analyse.typ": *
#import "../Subdocumenten/Samenwerkingscontract.typ": *

// Use Subdocuments
#let Blokdiagram = include "../Subdocumenten/Blokdiagram (Hoog over).typ"
#let stakeholder_content = include "../Subdocumenten/Stakeholder_analyse.typ"
#let Samenwerkingscontract = include "../Subdocumenten/Samenwerkingscontract.typ"
#let Prototype_content = include "../Subdocumenten/Prototype.typ"
#let Persona_content = include "../Subdocumenten/Persona_Onderzoek.typ"