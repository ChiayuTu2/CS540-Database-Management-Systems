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
                        <li> ':-' : 表示蘊含或“如果...那麼”
                        <li> ','  : 表示邏輯與
                        <li> ';'  : 表示邏輯或
                    </ul>
            </ul>
        <li>Relational calculus 
            <ul>
                <li>Def : Relational calculus is a non-procedural query language that forms the theoretical basis of SQL. It focuses on what to retrieve rather than how to retrieve it. There are two types of relational calculus: Tuple Relational Calculus (TRC) and Domain Relational Calculus (DRC). In TRC, queries are expressed as formulas consisting of several variables, predicates, and quantifiers. DRC, on the other hand, is based on specifying a list of domain variables and a formula that describes the relations among these variables. (關係演算是一種非程序化的查詢語言，是 SQL 的理論基礎。它專注於檢索什麼，而不是如何檢索。關係演算有兩種類型：元組關係演算（TRC）和域關係演算（DRC）。在 TRC 中，查詢以包含幾個變量、謂詞和量詞的公式形式表示。而 DRC 則基於指定一個域變量列表和描述這些變量之間關係的公式。)
                <li>Symbol:
                    <ul>
                        <li> 原子（Atom）：基本的關係謂詞，如 Actor(x, y, z)，表示一個在特定關係中的元組。
                        <li> 邏輯與（Conjunction, ∧）：表示兩個條件都必須滿足，如 P ∧ Q
                        <li> 邏輯或（Disjunction, ∨）：表示兩個條件中至少有一個滿足，如 P ∨ Q。
                        <li> 蘊含（Implication, ⇒）：表示一個條件蘊含另一個條件，如 P ⇒ Q，表示如果 P 為真，則 Q 也為真。
                        <li> 否定（Negation, not）：表示某個條件的否定，如 not(P) 表示 P 不滿足。
                        <li> 全稱量詞（Universal quantification, ∀）：表示對所有可能的元素，某個條件都為真，如 ∀x.P 表示對所有 x，P 都是真。
                        <li> 存在量詞（Existential quantification, ∃）：表示至少有一個元素滿足某個條件，如 ∃x.P 表示存在至少一個 x 使得 P 為真。
                    </ul>
            </ul>
        <li>Relational Algebra
            <ul>
                <li>Def : Relational algebra is a procedural query language. It provides a set of operations to manipulate relations (tables). These operations, like select, project, join, union, and difference, are used to formulate complex queries. Each operation takes one or two relations as input and produces a new relation as output, allowing for the composition of complex queries from simpler operations. (關係代數是一種程序化查詢語言。它提供了一組操作來操作關係（表）。這些操作，如選擇、投影、連接、聯合和差異，被用來制定複雜查詢。每個操作對一個或兩個關係作為輸入，並產生一個新的關係作為輸出，允許從更簡單的操作組合複雜查詢。)
                <li>Symbol:
                    <ul>
                        <li> 選擇（Selection, σ）: 選擇操作用於選取符合特定條件的行（元組）。 它相當於資料庫查詢中的 "WHERE" 子句。
                        <li> 投影（Projection, π）: 投影操作用於刪除關係中不想要的列。 它相當於選擇顯示哪些欄位。
                        <li> 笛卡爾積（Cross-product, ×）: 笛卡爾積允許我們組合兩個關係。 它將一個關係的每一行與另一個關係的每一行配對，形成所有可能的行對組合。
                        <li> 集合差（Set-difference, −）: 集合差運算用來找出屬於第一個關係而不屬於第二個關係的元組。
                        <li> 並集（Union, ∪）: 並集操作用於合併兩個關係中的元組，結果中包含屬於第一個關係和/或第二個關係的所有元組。
                    </ul>
            </ul>
    </ol>

