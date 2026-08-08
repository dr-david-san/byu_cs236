# BYU CS 236 — Datalog Interpreter

Course projects for [CS 236](https://faculty.cs.byu.edu/~barker/cs236/): build a Datalog interpreter from scratch in C++, one layer at a time.

Each folder is a snapshot of the program at that project. Later projects reuse and extend the earlier pieces (scanner → parser → database → rules → optimized evaluation).

## Projects

| Folder | Project | What it does |
|--------|---------|--------------|
| `Lab1/` | 1 — Lexical Analyzer | Scan input into tokens (`Token`, `Scanner`) |
| `Lab2/` / `Project3/` | 3 — Relational Database | Schemes, facts, and queries (`select` / `project` / `rename`) |
| `lab4/` | 4 — Datalog Interpreter | Evaluate rules to a fixed point (join + union) |
| `lab5/` | 5 — Optimizing Rule Evaluation | Dependency graph, SCCs, evaluate per component |

**Project 2 (Parser)** builds on Lab 1: parse tokens into a `DatalogProgram` and print `Success!` / `Failure!`. The parser lives in `Parser.h` and is carried forward into every later project. There isn’t a separate Project 2 folder in this repo.

> `Lab2/` and `Project3/` are both Project 3–era code. Prefer `Project3/` for the file-based `main` that reads an input file from the command line. `Lab2/main.cpp` is mostly a small `Relation` demo.

## Requirements

- A C++17 compiler (`g++` or `clang++`)
- Bash (for the provided test scripts)
- macOS or Linux recommended

## Quick start

From any project folder:

```bash
cd lab5   # or Lab1, Project3, lab4, …
g++ -Wall -std=c++17 -g *.cpp -o project5
./project5 your_input.txt
```

Replace `project5` with `project1`, `project3`, or `project4` to match the folder you’re in.

`lab5` also has a sample:

```bash
cd lab5
g++ -Wall -std=c++17 -g *.cpp -o project5
./project5 sample_input.txt
```

## Running tests

Several folders include `run-tests.sh` (example cases) and `run-passoff.sh` (pass-off buckets). They compile, run each input, and `diff` against the expected output (whitespace-insensitive).

```bash
cd lab4
chmod +x run-tests.sh run-passoff.sh
./run-tests.sh
./run-passoff.sh
```

You need the matching test directories next to the scripts (e.g. `project4-tests/`, `project4-passoff/`). If those folders are missing, unzip the course test packs into that project directory first.

| Folder | Example tests | Pass-off tests |
|--------|---------------|----------------|
| `Lab2/` / `Project3/` | `project3-tests/` | `project3-passoff/` |
| `lab4/` | `project4-tests/` | `project4-passoff/` |
| `lab5/` | `project5-tests/` | `project5-passoff/` |

A run is clean when you see each input printed and **no** `diff failed on test …` lines.

## Repo layout

```
byu_cs236/
├── Lab1/           # Project 1 — scanner
├── Lab2/           # Project 3 work / experiments
├── Project3/       # Project 3 — relational DB + queries
├── lab4/           # Project 4 — rules
│   ├── find-the-bugs.md
│   ├── project4-tests/
│   └── project4-passoff/
├── lab5/           # Project 5 — SCC-optimized rules
│   ├── GUIDE.md    # study guide (read this)
│   └── sample_input.txt
├── docs/           # notes / design writeups
├── hello.cpp       # early file I/O practice
└── README.md
```

Typical headers you’ll see as the interpreter grows:

- `Token.h`, `Scanner.h` — lexing
- `Parser.h` — grammar / datalog AST
- `Scheme.h`, `Tuple.h`, `Relation.h`, `Database.h` — relational store
- `Interpreter.h` — evaluate schemes, facts, rules, queries
- `Node.h`, `Graph.h` — dependency graph + SCCs (Project 5)

## Official lab pages

- [Lab 1 — Lexical Analyzer](https://faculty.cs.byu.edu/~barker/cs236/labs/lab1.html)
- [Lab 2 — Parser](https://faculty.cs.byu.edu/~barker/cs236/labs/lab2.html)
- [Lab 3 — Relational Database](https://faculty.cs.byu.edu/~barker/cs236/labs/lab3.html)
- [Lab 4 — Datalog Interpreter](https://faculty.cs.byu.edu/~barker/cs236/labs/lab4.html)
- [Lab 5 — Optimizing Rule Evaluation](https://faculty.cs.byu.edu/~barker/cs236/labs/lab5-extra.html)

## Tips

1. **Work in the folder for the project you’re submitting.** Don’t mix headers from different labs unless you know they’re compatible.
2. **Read `lab5/GUIDE.md`** if you’re on Project 5 — it explains dependency graphs, reverse graphs, DFS finish order, SCCs, and trivial vs fixed-point evaluation.
3. **`lab4/find-the-bugs.md`** is a short checklist for common Project 4 mistakes (rule output format, `joinTuples` by name vs value).
4. Build with warnings on: `g++ -Wall -std=c++17 -g *.cpp -o …`
5. Compiled binaries and `.dSYM` folders are local build junk; you don’t need to commit them (see `.gitignore`).

## How the pieces connect

```text
input file
   │
   ▼
 Scanner  →  tokens
   │
   ▼
 Parser   →  DatalogProgram (schemes, facts, rules, queries)
   │
   ▼
 Interpreter
   ├── schemes/facts → Database (Relations)
   ├── rules         → join / project / rename / union
   │                     └── Lab 5: only within each SCC, right order
   └── queries       → select / project / rename → Yes/No answers
```
