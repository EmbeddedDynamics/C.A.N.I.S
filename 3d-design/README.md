# 3D Design

All 3D designs for the Robotdog project are maintained in OnShape.

**OnShape Document:** [C.A.N.I.S. CAD](https://cad.onshape.com/documents/de215c13f85991496b305af5/w/90d06f27be5cf5466781f660/e/65f4320ee7f6d17dd1840a11)

## Document Structure

```
C.A.N.I.S.                           # Root document containing the main assemblies
├── Robot leg                        # Leg assemblies with geometry and kinematics
│   ├── Leg Parts                    # Individual leg structural parts
│   └── Connectors                   # Connection 
├── Shoulder                         # Shoulder joint parts and assemblies
├── Capstan                          # Capstan drive parts
│   ├── Leg Drums                    # Drums for the leg capstan drive
│   └── Shoulder Drums               # Drums for the shoulder capstan drive
├── Mounting                         # Frame and body mounting hardware
│   ├── Main Body Construction       # Primary chassis beams and panels
│   ├── Cable Clamps                 # Cable management clamps
│   └── Spacers                      # Spacers and standoffs
├── Tools                            # Design/assembly tooling parts
├── Parameters                       # Parametric variable definitions
│   ├── Capstan                      # Capstan geometry parameters
│   └── Hardware                     # Hardware spec parameters (Inserts, bearings, etc.)
├── Images                           # Reference images
└── CAD Imports                      # Imported external CAD files
```

## Notes

- Most designs are parametric.
- Export formats used: STL (printing) and STEP.
- Import formats used: 3MF, STEP and SLDPRT.
- PCB's are imported from Altium.