#import "@preview/elembic:1.1.1" as e

// Define the role type
#let role = e.types.declare(
  "role",
  prefix: "@preview/role-calculations,v1",
  doc: "Relevant data for a project related role",
  
  fields: (
    e.field("name", str, doc: "Project role name", required: true),
    e.field("hourly-rate", float, doc: "Hourly rate for this role", default: 40.0),
  ),
  
)

// Define the assignment type (person assigned to role with hours)
#let assignment = e.types.declare(
  "assignment",
  prefix: "@preview/role-calculations,v1",
  doc: "Person assigned to a project role with hours worked",
  
  fields: (
    e.field("person", str, doc: "Person's name", required: true),
    e.field("role", str, doc: "Assigned role", required: true),
    e.field("hours", float, doc: "Hours worked in this role", default: 0.0),
  ),
 
)

// State management for roles and assignments
#let __role-calc-state = state("role-calc-state", (roles: (), assignments: ()))

// Function to add a role
#let add-role(name, hourly-rate: 40.0) = {
  __role-calc-state.update(state => {
    state.roles.push((name: name, hourly-rate: hourly-rate))
    state
  })
}

// Function to add an assignment (person to role with hours)
#let add-assignment(person, role, hours: 0.0) = {
  __role-calc-state.update(state => {
    state.assignments.push((person: person, role: role, hours: hours))
    state
  })
}

// Function to add multiple roles at once
#let add-roles(roles-array) = {
  for role-item in roles-array {
    add-role(role-item.name, hourly-rate: role-item.at("hourly-rate", default: 40.0))
  }
}

// Function to add multiple assignments at once
#let add-assignments(assignments-array) = {
  for assignment-item in assignments-array {
    add-assignment(
      assignment-item.name,
      assignment-item.role,
      hours: assignment-item.at("hours", default: 0.0)
    )
  }
}

// Main role calculations display function using element
#let role-calculations = e.element.declare(
  "roleCalculations",
  prefix: "@preview/role-calculations,v1",
  
  display: it => {
      let state-value = __role-calc-state.get()
      let roles = state-value.roles
      let assignments = state-value.assignments
      
      // Display header
      heading(level: it.level, it.title)
      
      // Calculate totals per person
      let people-totals = assignments.fold((:), (acc, assignment) => {
        let role-data = roles.find(r => r.name == assignment.role)
        let rate = if role-data != none { role-data.hourly-rate } else { 0.0 }
        let subtotal = assignment.hours * rate
        let person = assignment.person
        acc + ((person): acc.at(person, default: 0.0) + subtotal)
      })
      
      let total-cost = people-totals.values().fold(0.0, (sum, cost) => sum + cost)

      
      // Display roles table
      if roles.len() > 0 {
        [*Available Roles:*]
        table(
          columns: (2fr, 1fr),
          [Role], [Hourly Rate],
          ..roles.map(r => (r.name, "€" + str(r.hourly-rate))).flatten()
        )
        v(0.5em)
      }
      
      // Display assignments table
      if assignments.len() > 0 {
        [*Assignments:*]
        table(
          columns: (1.5fr, 1.5fr, 1fr, 1fr),
          [Person], [Role], [Hours], [Subtotal],
          ..assignments.map(a => {
            let role-data = roles.find(r => r.name == a.role)
            let rate = if role-data != none { role-data.hourly-rate } else { 0.0 }
            let subtotal = a.hours * rate
            (a.person, a.role, str(a.hours), "€" + str(subtotal))
          }).flatten()
        )
        v(0.5em)
      }
      
      // Display per-person summary
      if people-totals.len() > 0 {
        [*Cost per Person:*]
        table(
          columns: (2fr, 1fr),
          [Person], [Total Cost],
          ..people-totals.pairs().map(pair => (pair.at(0), "€" + str(pair.at(1)))).flatten()
        )
        v(0.5em)
      }
      
      // Display total
      if assignments.len() > 0 {
        [*Total Project Cost: €#total-cost*]
      }
  },
  
  fields: (
    e.field("title", str, doc: "Title of the report", named: true, default: "Role Calculations"),
    e.field("level", int, doc: "Heading level", named: true, default: 1),
  ),
)

