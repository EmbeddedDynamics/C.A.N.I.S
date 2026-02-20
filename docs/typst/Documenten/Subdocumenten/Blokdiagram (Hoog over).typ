// General data import
#import "../Config/GeneralConfig.typ": *

// Functions import
#import "@preview/fletcher:0.5.8" as fletcher: diagram, node, edge
#import "@preview/oxifmt:0.2.1": strfmt
#import "@preview/tablem:0.1.0": tablem
#import "@preview/numbly:0.1.0": numbly
#import "@preview/gantty:0.5.1": gantt

#let bent-edge(from, to, ..args) = {
  let midpoint = (from, 50%, to)
  let vertices = (
    from,
    (from, "|-", midpoint),
    (midpoint, "-|", to),
    to,
  )
  edge(..vertices, "-|>", ..args)
}

#diagram(
  node-stroke: luma(80%),
  edge-corner-radius: none,
  spacing: (10pt, 15pt),

  // Nodes
  node((6.5,0), [*Robothond*], name: <root>),
  
  node((1.5,1.5), [*Hardware*], name: <hardware-block>),
  node((9,1.5), [*Software*], name: <software-block>),
  
  node((0.5,3), [*Robotpoot*], name: <robotpoot-block>),
  node((2.8,3), [*Chassis*], name: <chasis-block>),

  node((0,4), [*Aandrijving*], name: <aandrijving-block>),
  node((1,4), [*Motoren*], name: <motoren-block>),
  
  node((6.25,3.25), [*Embedded\ firmware*], name: <embedded-firmware-block>),
  node((9,3.25), [*Regel\ systemen*], name: <regel-systemen-block>),
  node((11.5,3.25), [*#acr("AI")\ functies*], name: <AI-block>),

  node((8.5,4.5), [*#acr("FOC")*], name: <foc-block>),
  node((9.5,4.5), [*#acr("IMU")*], name: <imu-block>),

  node((11,4.75), [*Omgevings\ herkening*], name: <omgeving-block>),
  node((12,4.75), [*Persoonlijkheid*], name: <persoonlijkheid-block>),

  node((4.5,4.50), [*#acr("IK")*], name: <IK-block>),
  node((5.8,4.50), [*#acr("RTOS")*], name: <rtos-block>),
  node((6.9,4.5), [*Protocolen*], name: <protocolen-block>),

  node((6.25,5.75), [*Draadloos*], name: <draadloos-block>),
  node((7.5,5.75), [*Galvanisch*], name: <galvanisch-block>),

  // Edges
  
  bent-edge(<root>, <hardware-block>),
  bent-edge(<root>, <software-block>),

  bent-edge(<hardware-block>, <robotpoot-block>),
  bent-edge(<hardware-block>, <chasis-block>),

  bent-edge(<robotpoot-block>, <aandrijving-block>),
  bent-edge(<robotpoot-block>, <motoren-block>),

  bent-edge(<software-block>, <embedded-firmware-block>),
  //bent-edge(<software-block>, <regel-systemen-block>),
  edge(vertices: (<software-block>, <regel-systemen-block>), "-|>"),
  bent-edge(<software-block>, <AI-block>),

  bent-edge(<embedded-firmware-block>, <IK-block>),
  bent-edge(<embedded-firmware-block>, <rtos-block>),
  bent-edge(<embedded-firmware-block>, <protocolen-block>),

  bent-edge(<protocolen-block>, <draadloos-block>),
  bent-edge(<protocolen-block>, <galvanisch-block>),

  bent-edge(<regel-systemen-block>, <foc-block>),
  bent-edge(<regel-systemen-block>, <imu-block>),

  bent-edge(<AI-block>, <omgeving-block>),
  bent-edge(<AI-block>, <persoonlijkheid-block>),
)
