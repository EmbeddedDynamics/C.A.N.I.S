#import "@preview/gantty:0.5.1": gantt
#import "@preview/tablex:0.0.9": tablex, cellx

// ============================================================
// PLANNING LIBRARY v1.0
// A professional project planning toolkit for Typst
// 
// Usage:
//   #import "planning-lib.typ": *
//   #planning-document(config, gantt-data, task-data)
// ============================================================

// ============================================================
// CONFIGURATION & CONSTANTS
// ============================================================

#let default-config = (
  title: "Project Planning",
  page-margin: 1.2cm,
  font-name: "Segoe UI",
  font-size: 10pt,
  enable-gantt: true,
  enable-breakdown: false,
  enable-resources: true,
  enable-critical-path: false,
)

// Critical path task indicator
#let critical-path-tasks = (
  "Stakeholder interviews",
  "Requirements documentation",
  "System architecture",
  "Database design",
  "Backend development",
  "Integration & testing",
  "QA & bug fixes",
  "Deployment preparation",
)

// ============================================================
// STATUS & RISK STYLING
// ============================================================

#let status-badge(status) = {
  let (bg, text-color) = if status == "in-progress" {
    (rgb("#2196F3"), white)
  } else if status == "completed" {
    (rgb("#4CAF50"), white)
  } else {
    (rgb("#E0E0E0"), rgb("#424242"))
  }
  
  box(
    fill: bg,
    outset: 2pt,
    radius: 3pt,
    text(fill: text-color, size: 9pt, weight: "semibold", status)
  )
}

#let risk-badge(risk-level) = {
  let (bg, text-color, icon) = if risk-level == "critical" {
    (rgb("#D32F2F"), white, "⚠")
  } else if risk-level == "high" {
    (rgb("#F57C00"), white, "!")
  } else if risk-level == "medium" {
    (rgb("#FBC02D"), rgb("#424242"), "◆")
  } else {
    (rgb("#388E3C"), white, "✓")
  }
  
  box(
    fill: bg,
    outset: 2pt,
    radius: 3pt,
    text(fill: text-color, size: 9pt, weight: "semibold", icon + " " + risk-level)
  )
}

#let resource-allocation-bar(allocations) = {
  // allocations: array of (name, percentage, color)
  let total = allocations.map(a => a.at(1)).sum()
  let normalized = allocations.map(a => (a.at(0), a.at(1) / total * 100, a.at(2)))
  
  box(
    width: 100%,
    height: 20pt,
    fill: rgb("#F5F5F5"),
    stroke: (paint: rgb("#E0E0E0"), thickness: 0.5pt),
    radius: 2pt,
    ..normalized.map(item => {
      let (name, pct, color) = item
      box(
        width: (pct * 1%),
        height: 100%,
        fill: color,
        outset: 0pt,
      )
    })
  )
}

// ============================================================
// CRITICAL PATH HIGHLIGHTING
// ============================================================

#let is-critical(task-name) = {
  critical-path-tasks.any(cp => lower(cp) == lower(task-name))
}

#let critical-indicator(task-name) = {
  if is-critical(task-name) {
    box(
      fill: rgb("#FFEBEE"),
      outset: 3pt,
      radius: 2pt,
      text(fill: rgb("#C62828"), size: 8pt, weight: "bold", "CRITICAL")
    )
  }
}

// ============================================================
// RESOURCE ALLOCATION TABLE
// ============================================================

#let resource-summary-table(task-data) = {
  // Aggregate resource allocations across all tasks
  let resource-map = (:)
  
  for phase-group in task-data {
    for task in phase-group.tasks {
      if "resources" in task {
        for resource in task.resources {
          let name = resource.name
          let allocation = resource.allocation
          
          if name not in resource-map {
            resource-map.insert(name, 0)
          }
          resource-map.insert(name, resource-map.at(name) + allocation)
        }
      }
    }
  }
  
  // Convert to sorted list
  let resources = resource-map.pairs().map(item => (item.at(0), item.at(1))).sorted(key: r => -r.at(1))
  
  table(
    columns: (2fr, 1fr, 1.5fr),
    
    align: (left, center, left),
    
    table.header(
      text(weight: "bold", size: 10pt, "Team Member"),
      text(weight: "bold", size: 10pt, "Total %"),
      text(weight: "bold", size: 10pt, "Workload"),
    ),
    
    ..resources.map(resource => {
      let (name, total-alloc) = resource
      let load-color = if total-alloc > 100 {
        rgb("#D32F2F")  // Red: overallocated
      } else if total-alloc > 90 {
        rgb("#F57C00")  // Orange: near capacity
      } else {
        rgb("#388E3C")  // Green: healthy
      }
      
      let load-label = if total-alloc > 100 {
        "⚠ Overallocated"
      } else if total-alloc > 90 {
        "~ Near Capacity"
      } else {
        "✓ Healthy"
      }
      
      (
        name,
        text(size: 10pt, weight: "bold", str(total-alloc) + "%"),
        box(
          fill: load-color,
          outset: 3pt,
          radius: 2pt,
          text(fill: white, size: 9pt, weight: "semibold", load-label)
        ),
      )
    }).flatten(),
  )
}

// ============================================================
// DETAILED BREAKDOWN WITH RESOURCES & RISK
// ============================================================

#let detailed-breakdown-table(task-data, enable-resources: true, enable-critical: true) = {
  for phase-group in task-data {
    // Phase header
    box(
      width: 100%,
      fill: phase-group.color,
      outset: 8pt,
      radius: 6pt,
      text(weight: "bold", size: 13pt, phase-group.phase)
    )
    
    box(
      width: 100%,
      inset: 8pt,
      text(size: 10pt, [
        *Duration:* #phase-group.duration \
        *Phase Lead:* #phase-group.lead
      ])
    )
    
    // Determine column structure
    let columns-list = (1.2fr, 1fr, 0.9fr, 0.8fr)
    let headers = (
      text(weight: "bold", size: 10pt, "Task Name"),
      text(weight: "bold", size: 10pt, "Duration"),
      text(weight: "bold", size: 10pt, "Assigned To"),
      text(weight: "bold", size: 10pt, "Status"),
    )
    
    if enable-resources {
      columns-list.push(0.9fr)
      headers.push(text(weight: "bold", size: 10pt, "Resource %"))
    }
    
    if enable-critical {
      columns-list.push(0.7fr)
      headers.push(text(weight: "bold", size: 10pt, "Risk"))
    }
    
    // Tasks table for this phase
    table(
      columns: columns-list,
      
      align: (left, center, center, center, center, center),
      
      table.header(..headers),
      
      // Task rows
      ..phase-group.tasks.map(task => {
        let row = (
          box(width: 100%, [
            #if enable-critical and is-critical(task.name) [
              #critical-indicator(task.name)
            ]
            *#task.name* \
            #for subtask in task.subtasks [
              • #text(size: 9pt, fill: rgb("#666"), subtask) \
            ]
          ]),
          task.duration,
          task.assignee,
          status-badge(task.status),
        )
        
        if enable-resources and "resources" in task {
          let total-alloc = task.resources.map(r => r.allocation).sum()
          row.push(text(size: 9pt, weight: "semibold", str(total-alloc) + "%"))
        } else if enable-resources {
          row.push([ ])
        }
        
        if enable-critical {
          let risk = if "risk" in task {
            task.risk
          } else {
            "low"
          }
          row.push(risk-badge(risk))
        }
        
        row
      }).flatten(),
    )
    
    v(15pt)
  }
}

// ============================================================
// RESOURCE DETAIL BREAKDOWN
// ============================================================

#let resource-detail-section(task-data) = {
  [
    == Detailed Resource Allocation

    The following table shows per-task resource allocation with identified overallocations:

  ]
  
  for phase-group in task-data {
    for task in phase-group.tasks {
      if "resources" in task and task.resources.len() > 0 {
        [
          *#task.name* (#task.duration)
          
        ]
        
        let resource-items = task.resources.map(r => (r.name, str(r.allocation)  + "%"))
        
        table(
          columns: (2fr, 1fr),
          
          
          table.header(
            text(weight: "bold", size: 9pt, "Team Member"),
            text(weight: "bold", size: 9pt, "Allocation"),
          ),
          
          ..resource-items.map(item => item).flatten(),
        )
        
        v(8pt)
      }
    }
  }
}

// ============================================================
// CRITICAL PATH ANALYSIS
// ============================================================

#let critical-path-section(task-data) = {
  [
    == Critical Path Analysis

    Tasks on the critical path are dependencies that directly impact project completion date. Any delay in critical tasks will delay the entire project.

    *Critical Path Tasks:*

  ]
  
  let critical-tasks = ()
  for phase-group in task-data {
    for task in phase-group.tasks {
      if is-critical(task.name) {
        critical-tasks.push((task.name, phase-group.phase, task.duration, task.assignee))
      }
    }
  }
  
  if critical-tasks.len() > 0 [
    #table(
      columns: (2fr, 1.5fr, 1fr, 1.2fr),
      
      
      table.header(
        text(weight: "bold", size: 9pt, fill: rgb("#D32F2F"), "Task"),
        text(weight: "bold", size: 9pt, fill: rgb("#D32F2F"), "Phase"),
        text(weight: "bold", size: 9pt, fill: rgb("#D32F2F"), "Duration"),
        text(weight: "bold", size: 9pt, fill: rgb("#D32F2F"), "Owner"),
      ),
      
      ..critical-tasks.map(task => (task.at(0), task.at(1), task.at(2), task.at(3))).flatten(),
    )
  ]
}

// ============================================================
// RISK SUMMARY SECTION
// ============================================================

#let risk-summary-section(task-data) = {
  let risks-by-level = (critical: (), high: (), medium: (), low: ())
  
  for phase-group in task-data {
    for task in phase-group.tasks {
      let risk = if "risk" in task { task.risk } else { "low" }
      if risk in risks-by-level {
        risks-by-level.at(risk).push((task.name, phase-group.phase, task.assignee))
      }
    }
  }
  
  [
    == Risk Assessment

    This section summarizes identified risks across all tasks. Prioritize mitigation for critical and high-risk items.

  ]
  
  for (level, tasks) in risks-by-level.pairs() {
    if tasks.len() > 0 {
      let level-badge = risk-badge(level)
      
      [
        *#level-badge Risks (#tasks.len())*

        #table(
          columns: (2fr, 1.5fr, 1.2fr),
          
          
          ..tasks.map(task => (task.at(0), task.at(1), task.at(2))).flatten(),
        )

      ]
      
      v(10pt)
    }
  }
}

// ============================================================
// MAIN PLANNING DOCUMENT FUNCTION
// ============================================================

#let planning-document(config: default-config, gantt-data, task-data) = {
  
  [
    = #config.title

    == Visual Project Timeline

    The Gantt chart below shows all phases, tasks, and milestones with their temporal relationships:

  ]
  
  if config.enable-gantt {
    box(width: 100%, fill: rgb("#F9F9F9"), outset: 8pt, radius: 6pt, [
      #gantt(gantt-data)
    ])
  }
  
  v(15pt)
  
  if config.enable-breakdown {
    [
      == Detailed Task Breakdown

      Below is the comprehensive top-down planning breakdown, showing each phase with its assigned tasks, responsible parties, and weekly sub-activities.

    ]
    
    detailed-breakdown-table(task-data, enable-resources: config.enable-resources, enable-critical: config.enable-critical-path)
  }
  
  if config.enable-resources {
    [
      == Resource Summary

      Overall team allocation across the project:

    ]
    
    resource-summary-table(task-data)
    
    v(15pt)
    
    resource-detail-section(task-data)
    
    v(15pt)
  }
  
  if config.enable-critical-path {
    critical-path-section(task-data)
    
    v(15pt)
  }
  
  risk-summary-section(task-data)
  
  [
    == Key Milestones

    - *December 14* - Requirements documentation complete
    - *December 29* - Design review & approval
    - *February 2* - Beta release for internal testing
    - *February 22* - Production release

  ]
}

// ============================================================
// UTILITY FUNCTIONS FOR LIBRARY USERS
// ============================================================

/// Create a simple task entry
#let task(name, duration, assignee, status: "planned", subtasks: (), resources: (), risk: "low") = (
  name: name,
  duration: duration,
  assignee: assignee,
  status: status,
  subtasks: subtasks,
  resources: resources,
  risk: risk,
)

/// Create a resource allocation entry
#let resource(name, allocation) = (
  name: name,
  allocation: allocation,
)

/// Create a phase group
#let phase(phase-name, color, duration, lead, tasks-list) = (
  phase: phase-name,
  color: color,
  duration: duration,
  lead: lead,
  tasks: tasks-list,
)

/// Update critical path (override default)
#let set-critical-path(tasks-list) = {
  critical-path-tasks = tasks-list
}
