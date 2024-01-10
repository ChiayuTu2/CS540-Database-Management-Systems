# Summary of Database Query Languages

This document provides a summary of the basic concepts, examples, and notation used in Datalog, Relational Calculus, and Relational Algebra.

## Datalog

Datalog is a declarative database query language based on the logic programming paradigm, primarily used for deductive databases. It uses rules and facts for queries.

### Notation and Symbols

Notable symbols include ":-" for inference rules, "()" for defining facts and rules, and uppercase letters like "X", "Y" for variables.

### Example Query

Given facts: `parent(bob, alice)`, `parent(alice, sarah)`, `parent(alice, john)`. Rules: `ancestor(X, Y) :- parent(X, Y)` and `ancestor(X, Y) :- parent(X, Z), ancestor(Z, Y)`. Question: Who are Sarah's ancestors? Answer: Bob and Alice.

## Relational Calculus

Relational Calculus is a non-procedural query language that includes Tuple Relational Calculus (TRC) and Domain Relational Calculus (DRC).

### TRC and DRC

TRC expresses queries as formulas consisting of variables, predicates, and quantifiers, focusing on what to retrieve. DRC is based on specifying domain variables and a formula describing relations among these variables.

### Notation and Symbols

Uses "{}" for defining query scope or set, "| or :" to separate the range and conditions, and logical symbols like "∧" (AND), "∨" (OR), "¬" (NOT), "∃" (Exists), and "∀" (For all).

### Example Query

Find all employees in the 'Marketing' department with a salary over 50000 in the Employees table. Expression: `{T | Employees(T) ∧ T.department = "Marketing" ∧ T.salary > 50000}`.

## Relational Algebra

Relational Algebra is a procedural query language using a set of operations to manipulate tables.

### Notation and Symbols

Includes σ (Select), π (Project), ∪ (Union), ∩ (Intersection), − (Set Difference), ⨝ (Join), and × (Cartesian Product).

### Example Query

Find the names of all students enrolled in the 'Mathematics' course from the Students and Enrollments tables. Use projection and natural join operations.
