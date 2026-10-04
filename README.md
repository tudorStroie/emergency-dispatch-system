# Emergency Dispatch System

A simulation of an emergency incident dispatch system implemented in C as part of a Data Structures course.

The program manages emergency incidents with different priority levels and assigns available intervention units according to incident severity.

## Features

- Add emergency incidents with different priority levels
- Maintain separate queues for high, medium and low priority incidents
- Track available intervention units
- Dispatch units to incidents based on priority
- Mark incidents as solved
- Undo the latest active dispatch
- Display information about incidents, units and interventions

## Data Structures

The project uses several custom data structures:

- **Circular Doubly Linked Lists** — used to store incidents and interventions
- **Queues** — used for incident priority levels and available intervention units
- **Stack** — used to maintain dispatch history and support undo operations
- **Dynamic Arrays / Structures** — used to manage intervention units

## Dispatch Logic

Incidents are processed according to the following priority order:

1. High
2. Medium
3. Low

An incident with a lower priority can only be dispatched when all higher-priority queues are empty.

## Implemented Operations

- `ADD_INCIDENT`
- `CHECK_UNITS_AVAILABILITY`
- `DISPATCH`
- `UNDO_LAST_DISPATCH`
- `SOLVED_INCIDENT`
- `SHOW_UNIT`
- `SHOW_INCIDENT`
- `SHOW_INTERVENTIONS`

## Concepts Practiced

- Dynamic memory allocation
- Pointers
- Linked data structures
- Queues and stacks
- Priority-based scheduling
- Manual memory management in C

## Technologies

- C
- Standard C Library

## Project Structure

```text
emergency-dispatch-system/
├── main.c
└── README.md
