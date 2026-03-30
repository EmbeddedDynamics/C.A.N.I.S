#set page(paper: "a4", margin: 2.5cm)
#set text(size: 11pt)
#set heading(numbering: "1.")

#align(center)[
  #text(size: 18pt, weight: "bold")[Inverse Kinematics - Robotpoot]
  #v(0.2em)
  #text(size: 9pt, fill: gray)[12 januari 2026]
]
#line(length: 100%, stroke: 0.5pt + gray)
#v(0.8em)

= Hoek $alpha$ - Cosinusregel

$ c = sqrt(x^2 + y^2) $

// ── Diagram 1: 4-bar linkage (diamantvorm) ─────────────────────────────────
// Punten:
//   O  = (100, 40)   - scharnierpunt op x-as (boven)
//   KL = (50,  130)  - linkerkniepunt
//   KR = (150, 130)  - rechterkniepunt
//   P  = (100, 200)  - doelpunt [x,y] (onder)
//
// Stangen: O→KL (L1), O→KR (L1'), KL→P (L2), KR→P (L2)  => gesloten ruit

#figure(
  box(width: 220pt, height: 230pt, {

    // x-as
    place(line(start:(10pt,40pt),  end:(210pt,40pt), stroke: 1.2pt + black))
    place(dx:204pt, dy:32pt)[#text(size:9pt)[$x$]]
    // y-as
    place(line(start:(100pt,40pt), end:(100pt,215pt), stroke: 1.2pt + black))
    place(dx:93pt, dy:208pt)[#text(size:9pt)[$y$]]

    // ── 4 stangen ──
    // L1 links: O(100,40) → KL(50,130)   groen
    place(line(start:(100pt,40pt), end:(50pt,130pt),  stroke: 2.5pt + green.darken(20%)))
    // L1 rechts: O(100,40) → KR(150,130) oranje
    place(line(start:(100pt,40pt), end:(150pt,130pt), stroke: 2.5pt + orange))
    // L2 links: KL(50,130) → P(100,200)  blauw
    place(line(start:(50pt,130pt), end:(100pt,200pt), stroke: 2.5pt + blue))
    // L2 rechts: KR(150,130) → P(100,200) goud
    place(line(start:(150pt,130pt),end:(100pt,200pt), stroke: 2.5pt + yellow.darken(25%)))

    // c stippellijn: O → P
    place(line(start:(100pt,40pt), end:(100pt,200pt),
      stroke:(paint:gray, thickness:1pt, dash:"dashed")))
    place(dx:105pt, dy:110pt)[#text(size:8pt,fill:gray)[$c$]]

    // ── Labels stangen ──
    place(dx:52pt,  dy:75pt)[#text(size:9pt,fill:green.darken(20%))[$L_1$]]
    place(dx:138pt, dy:75pt)[#text(size:9pt,fill:orange)[$L_1'$]]
    place(dx:55pt,  dy:160pt)[#text(size:9pt,fill:blue)[$L_2$]]
    place(dx:130pt, dy:160pt)[#text(size:9pt,fill:yellow.darken(25%))[$L_2$]]

    // ── Scharnierpunten ──
    place(dx:94pt, dy:34pt,  circle(radius:6pt, fill:red))   // O
    place(dx:44pt, dy:124pt, circle(radius:4pt, fill:red))   // KL
    place(dx:144pt,dy:124pt, circle(radius:4pt, fill:red))   // KR
    place(dx:94pt, dy:194pt, circle(radius:5pt, fill:eastern))// P=[x,y]

    // ── Hoeklabels bij O ──
    place(dx:103pt, dy:52pt)[#text(size:8pt,fill:orange)[$alpha$]]
    place(dx:84pt,  dy:52pt)[#text(size:8pt,fill:blue)[$beta$]]
    place(dx:114pt, dy:44pt)[#text(size:8pt,fill:olive)[$theta$]]
    place(dx:90pt,  dy:63pt)[#text(size:8pt,fill:purple)[$gamma$]]

    // ── [x,y] label ──
    place(dx:108pt, dy:196pt)[#text(size:9pt,fill:eastern)[$[x,y]$]]
  }),
  caption: [4-bar linkage (ruit). $O$ is het vaste scharnierpunt op de x-as. $L_1$ loopt naar de twee kniepunten; $L_2$ sluit de ketting bij het doelpunt $[x,y]$.],
)

Cosinusregel op driehoek $(L_1, L_2, c)$:

$ L_2^2 = L_1^2 + c^2 - 2 dot L_1 dot c dot cos(alpha) $

#rect(stroke: red + 1pt, inset: 8pt, radius: 2pt, width: 100%)[
  $ alpha = cos^(-1)lr((frac(L_2^2 - L_1^2 - c^2, -2 dot L_1 dot c))) $
]

= Hoek $theta$ - Tangens

#figure(
  box(width: 180pt, height: 145pt, {
    place(line(start:(10pt,120pt), end:(170pt,120pt), stroke:1.2pt))
    place(dx:163pt, dy:112pt)[#text(size:9pt)[$x$]]
    place(line(start:(10pt,120pt), end:(10pt,10pt),  stroke:1.2pt))
    place(dx:4pt,   dy:4pt)[#text(size:9pt)[$y$]]

    // x-been stippel
    place(line(start:(10pt,120pt), end:(140pt,120pt),
      stroke:(paint:gray,thickness:0.8pt,dash:"dashed")))
    place(dx:70pt, dy:126pt)[#text(size:8pt)[$x$]]
    // y-been stippel
    place(line(start:(140pt,120pt),end:(140pt,40pt),
      stroke:(paint:gray,thickness:0.8pt,dash:"dashed")))
    place(dx:144pt,dy:76pt)[#text(size:8pt)[$y$]]

    // rechthoekje
    place(dx:126pt,dy:106pt, rect(width:10pt,height:10pt,stroke:0.7pt,fill:none))

    // schuine zijde c
    place(line(start:(10pt,120pt),end:(140pt,40pt), stroke:2pt+blue))
    place(dx:58pt, dy:68pt)[#text(size:8pt,fill:blue)[$c$]]

    // punten
    place(dx:5pt,  dy:115pt, circle(radius:3pt,fill:black))
    place(dx:135pt,dy:35pt,  circle(radius:4pt,fill:eastern))
    place(dx:144pt,dy:34pt)[#text(size:8pt,fill:eastern)[$[x,y]$]]

    place(dx:30pt, dy:102pt)[#text(size:8pt,fill:olive)[$theta$]]
  }),
  caption: [Hoek $theta$ via rechthoekige driehoek.],
)

#rect(stroke: red + 1pt, inset: 8pt, radius: 2pt, width: 100%)[
  $ theta = tan^(-1)lr((frac(y, x))) quad quad beta = theta - alpha $
]

= Afmetingen

#table(columns:(auto,auto), stroke:0.5pt, inset:7pt,
  [*Parameter*],[*Waarde*],
  [$L_1$],[76,8 mm],
  [$L_2$],[120 mm],
)

= Samenvatting

+ $c = sqrt(x^2 + y^2)$
+ $alpha = cos^(-1)lr((frac(L_2^2 - L_1^2 - c^2, -2 dot L_1 dot c)))$
+ $theta = tan^(-1)lr((frac(y, x)))$
+ $beta = theta - alpha$

#v(1em)
#line(length:100%, stroke:0.5pt+gray)
#align(center)[#text(size:8pt,fill:gray)[Inverse Kinematics - Robotpoot · 2026]]
