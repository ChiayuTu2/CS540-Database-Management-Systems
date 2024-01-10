<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Summary of Database Query Languages</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            line-height: 1.6;
            max-width: 800px;
            margin: auto;
            padding: 0 20px;
        }
        h1, h2 {
            color: #333;
        }
        h3 {
            color: #555;
        }
        p, li {
            color: #666;
        }
        code {
            background-color: #f4f4f4;
            border: 1px solid #ddd;
            padding: 2px 6px;
        }
    </style>
</head>
<body>
    <h1>Summary of Database Query Languages</h1>
    <p>This document provides a summary of the basic concepts, examples, and notation used in Datalog, Relational Calculus, and Relational Algebra.</p>

    <section>
        <h2>Datalog</h2>
        <p>Datalog is a declarative database query language based on the logic programming paradigm, primarily used for deductive databases. It uses rules and facts for queries.</p>
        <h3>Notation and Symbols</h3>
        <p>Notable symbols include <code>:-</code> for inference rules, <code>()</code> for defining facts and rules, and uppercase letters like <code>X</code>, <code>Y</code> for variables.</p>
        <h3>Example Query</h3>
        <p>Given facts: <code>parent(bob, alice)</code>, <code>parent(alice, sarah)</code>, <code>parent(alice, john)</code>. Rules: <code>ancestor(X, Y) :- parent(X, Y)</code> and <code>ancestor(X, Y) :- parent(X, Z), ancestor(Z, Y)</code>. Question: Who are Sarah's ancestors? Answer: Bob and Alice.</p>
    </section>

    <section>
        <h2>Relational Calculus</h2>
        <p>Relational Calculus is a non-procedural query language that includes Tuple Relational Calculus (TRC) and Domain Relational Calculus (DRC).</p>
        <h3>TRC and DRC</h3>
        <p>TRC expresses queries as formulas consisting of variables, predicates, and quantifiers, focusing on what to retrieve. DRC is based on specifying domain variables and a formula describing relations among these variables.</p>
        <h3>Notation and Symbols</h3>
        <p>Uses <code>{}</code> for defining query scope or set, <code>| or :</code> to separate the range and conditions, and logical symbols like <code>∧</code> (AND), <code>∨</code> (OR), <code>¬</code> (NOT), <code>∃</code> (Exists), and <code>∀</code> (For all).</p>
        <h3>Example Query</h3>
        <p>Find all employees in the 'Marketing' department with a salary over 50000 in the Employees table. Expression: <code>{T | Employees(T) ∧ T.department = "Marketing" ∧ T.salary > 50000}</code>.</p>
    </section>

    <section>
        <h2>Relational Algebra</h2>
        <p>Relational Algebra is a procedural query language using a set of operations to manipulate tables.</p>
        <h3>Notation and Symbols</h3>
        <p>Includes <code>σ (Select)</code>, <code>π (Project)</code>, <code>∪ (Union)</code>, <code>∩ (Intersection)</code>, <code>− (Set Difference)</code>, <code>⨝ (Join)</code>, and <code>× (Cartesian Product)</code>.</p>
        <h3>Example Query</h3>
        <p>Find the names of all students enrolled in the 'Mathematics' course from the Students and Enrollments tables. Use projection and natural join operations.</p>
    </section>
</body>
</html>
