<h1 align = 'center'>Summary</h1>

<h2 align = 'center'>Purpose</h2>
<p>Use your own understanding and online information to summarize what I learned in the CS 540 Database Management Systems class at Oregon state university in Winter 2024.

<h2 align = "center">Contents</h2>
<div>
    <p>📦Summary</p>
    <ul>
        <li>📄Relation Language</li>
    </ul>
</div>

<h2 align = 'center'>Relation Language</h2>
    <ol>
        <li>Datalog
            <ul>
                <li>Def : Datalog is a declarative database query language primarily used for deductive databases. In Datalog, a query is expressed as a set of rules or facts, and the query processor deduces the answer from these rules. (Datalog 是一種用於演繹型數據庫的聲明式數據庫查詢語言。在 Datalog 中，查詢以一組規則或事實的形式表示，查詢處理器從這些規則中推斷出答案。)
                <li>Symbol:
                    <ul>
                        <li> ':-' : 表示蕴含或“如果...那么”
                        <li> ',' : 表示逻辑与
                        <li> ';' : 表示逻辑或
                    </ul>
            </ul>
        <li>Relational Language
            <ul>
                <li>Def : Relational calculus is a non-procedural query language that forms the theoretical basis of SQL. It focuses on what to retrieve rather than how to retrieve it. There are two types of relational calculus: Tuple Relational Calculus (TRC) and Domain Relational Calculus (DRC). In TRC, queries are expressed as formulas consisting of several variables, predicates, and quantifiers. DRC, on the other hand, is based on specifying a list of domain variables and a formula that describes the relations among these variables. (關係演算是一種非程序化的查詢語言，是 SQL 的理論基礎。它專注於檢索什麼，而不是如何檢索。關係演算有兩種類型：元組關係演算（TRC）和域關係演算（DRC）。在 TRC 中，查詢以包含幾個變量、謂詞和量詞的公式形式表示。而 DRC 則基於指定一個域變量列表和描述這些變量之間關係的公式。)
                <li>Symbol:
                    <ul>
                        <li> 原子（Atom）：基本的关系谓词，如 Actor(x, y, z)，表示一个在特定关系中的元组。
                        <li> 逻辑与（Conjunction, ∧）：表示两个条件都必须满足，如 P ∧ Q
                        <li> 逻辑或（Disjunction, ∨）：表示两个条件中至少有一个满足，如 P ∨ Q。
                        <li> 蕴含（Implication, ⇒）：表示一个条件蕴含另一个条件，如 P ⇒ Q，意味着如果 P 为真，则 Q 也为真。
                        <li> 否定（Negation, not）：表示某个条件的否定，如 not(P) 表示 P 不满足。
                        <li> 全称量词（Universal quantification, ∀）：表示对所有可能的元素，某个条件都为真，如 ∀x.P 意味着对所有 x，P 都是真。
                        <li> 存在量词（Existential quantification, ∃）：表示至少有一个元素满足某个条件，如 ∃x.P 表示存在至少一个 x 使得 P 为真。
                    </ul>
            </ul>
        <li>Relational Algebra
            <ul>
                <li>Def : Relational algebra is a procedural query language. It provides a set of operations to manipulate relations (tables). These operations, like select, project, join, union, and difference, are used to formulate complex queries. Each operation takes one or two relations as input and produces a new relation as output, allowing for the composition of complex queries from simpler operations. (關係代數是一種程序化查詢語言。它提供了一組操作來操作關係（表）。這些操作，如選擇、投影、連接、聯合和差異，被用來制定複雜查詢。每個操作對一個或兩個關係作為輸入，並產生一個新的關係作為輸出，允許從更簡單的操作組合複雜查詢。)
                <li>Symbol:
                    <ul>
                        <li> 选择（Selection, σ）: 选择操作用于选取满足特定条件的行（元组）。它相当于数据库查询中的 "WHERE" 子句。
                        <li> 投影（Projection, π）: 投影操作用于删除关系中不想要的列。它相当于选择显示哪些字段。
                        <li> 笛卡尔积（Cross-product, ×）: 笛卡尔积允许我们组合两个关系。它将一个关系的每一行与另一个关系的每一行配对，形成所有可能的行对组合。
                        <li> 集合差（Set-difference, −）: 集合差操作用于找出属于第一个关系而不属于第二个关系的元组。
                        <li> 并集（Union, ∪）: 并集操作用于合并两个关系中的元组，结果中包含属于第一个关系和/或第二个关系的所有元组。
                    </ul>
            </ul>
    </ol>

