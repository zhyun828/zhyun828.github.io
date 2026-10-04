---
name: learn-topic
description: Create or systematically expand a textbook-style learning document for a topic the user wants to learn from zero. Combine beginner-friendly explanations, intuition, worked examples, formal definitions, formulas, derivations, underlying principles, implementation details, exercises, and authoritative references. Write directly to the specified file and prioritize both clarity and technical depth.
---

# Learn Topic

Use this skill when the user wants to learn a subject systematically from zero and wants a comprehensive learning document written directly into the repository.

This skill is file-oriented rather than conversation-oriented.

The goal is to create a document that has the depth of a good technical textbook while retaining the clarity of a patient teacher.

The document should not force a choice between:

`easy to understand`

and:

`technically deep`

It should provide both.

A beginner should be able to start reading from the beginning, while a more advanced reader should still find rigorous explanations, formulas, mechanisms, derivations, implementation details, and important edge cases later in the document.

The final result should be suitable for:

- first-time learning
- systematic study
- later review
- solving technical questions
- understanding formal documentation
- preparing for implementation
- preparing for exams, interviews, projects, or deeper study

## Input format

Primary format:

`$learn-topic notes <file> <topic1> <topic2> ... [--requirements <additional demands>]`

Examples:

`$learn-topic notes docs/notes/procom/dvb-s2/DVB-S2.md DVB-S2`

`$learn-topic notes docs/notes/procom/dvb-s2/DVB-S2.md DVB-S2 Interleaver LLR`

`$learn-topic notes docs/notes/procom/dvb-s2/DVB-S2.md DVB-S2 --requirements include derivations, worked examples, diagrams, exercises, and sufficient mathematical depth`

`$learn-topic notes docs/notes/procom/versal/Versal-AIE.md Versal AI Engine --requirements start from zero and include architecture, memory, streams, kernels, SIMD, and practical examples`

Multiple topics are allowed.

When several topics are provided:

- determine their conceptual relationships
- identify prerequisite dependencies
- organize them into a coherent learning order
- do not simply create one independent section per keyword
- merge closely related topics naturally
- explain prerequisite topics before topics that depend on them

The optional `--requirements` section contains additional instructions about:

- desired depth
- language
- examples
- formulas
- derivations
- exercises
- diagrams
- implementation detail
- source requirements
- emphasis or exclusions

Text after `--requirements` must not be interpreted as additional learning topics.

If `--requirements` is omitted, infer obvious additional demands from natural language when possible.

# Core objective

The default learning target is:

`zero or weak foundation → strong and systematic understanding`

Do not interpret "beginner-friendly" as "shallow."

Do not remove mathematics, theory, derivations, implementation details, or advanced explanations merely to make the document easier to read.

Instead:

- explain the intuition first
- then introduce formalism
- then go deeper

The learner should be able to understand both:

`what happens`

and:

`why it happens`

and, when appropriate:

`how it is formally derived or implemented`

# Difference from add-knowledge

The distinction between `learn-topic` and `add-knowledge` is fundamental.

## learn-topic

Use when the user wants to learn a topic systematically, especially from little or no prior knowledge.

It should:

- establish prerequisites
- build a complete learning structure
- explain the motivation behind concepts
- develop intuition
- introduce formal definitions
- include formulas and derivations when relevant
- provide worked examples
- connect related concepts
- explain implementation or practical behavior when relevant
- cover common misunderstandings
- prepare the learner for more advanced material

Typical request:

`$learn-topic notes docs/notes/procom/dvb-s2/DVB-S2.md DVB-S2`

## add-knowledge

Use when the user already has an existing knowledge base and wants to add or complete one or more specific knowledge points.

It should:

- inspect existing coverage
- avoid duplication
- add only missing material
- use minimal edits
- preserve the document's existing organization

Typical request:

`$add-knowledge docs/notes/procom/dvb-s2/DVB-S2.md Min-Sum algorithm`

In short:

`learn-topic = build the subject systematically`

`add-knowledge = maintain individual knowledge points inside an existing subject`

# Textbook-style teaching requirements

This section is mandatory.

The generated document should resemble a well-written technical textbook or university course written by a teacher who cares about whether the learner actually understands the material.

## 1. Combine accessibility and depth

Every major concept should normally contain several layers.

A useful progression is:

1. motivation
2. intuitive mental model
3. simple example
4. precise terminology
5. formal definition
6. underlying mechanism
7. equations or formal representation
8. derivation when useful
9. worked example
10. realistic application
11. connections to related concepts
12. important limitations or edge cases

Not every small concept needs every layer.

Use judgment based on importance and complexity.

Do not stop after the intuitive explanation if deeper knowledge is useful.

Do not begin directly with dense formalism when intuition can make the formalism easier to understand.

## 2. Explain why before how

Before introducing a mechanism, establish the problem it solves.

For example:

Do not begin Forward Error Correction with generator matrices.

First establish:

```text
Data crosses a noisy channel
        ↓
Some transmitted information may be corrupted
        ↓
The receiver needs a way to detect or correct errors
        ↓
Redundancy can provide additional constraints
        ↓
This motivates Forward Error Correction
```

Then progressively move toward:

- coding structures
- code rate
- parity
- block codes
- BCH
- LDPC
- mathematical representations

The learner should know why the formal machinery exists.

## 3. Build concepts in dependency order

Determine the conceptual prerequisites before writing the document.

Do not simply follow:

- alphabetical order
- the order of a specification
- the order of API documentation
- the order in which keywords were given by the user

Use the order that best supports understanding.

For example, a DVB-S2 learning path may naturally require:

```text
digital information
↓
bit
↓
symbol
↓
modulation
↓
constellation
↓
Mapper
↓
channel and noise
↓
Demapper
↓
hard and soft decisions
↓
LLR
↓
FEC
↓
BCH / LDPC
↓
Interleaver / Deinterleaver
↓
DVB-S2 framing and detailed standard behavior
```

Reorder official material when necessary for teaching, while keeping the technical facts correct.

## 4. Frequently reconnect to the big picture

Detailed explanations can cause beginners to lose track of the overall system.

After several detailed sections, reconnect the material to the larger structure.

For example:

```text
We now understand:

bits
↓
Mapper
↓
modulated symbols
↓
channel
↓
Demapper
↓
LLR

The next question is:

How must these soft values be reordered before the LDPC decoder can use them?

This leads to the Deinterleaver.
```

The learner should always understand where the current topic fits.

## 5. Use intuitive explanations without sacrificing correctness

Use analogies, visual explanations, and informal language when they genuinely help.

However:

- do not replace the real mechanism with the analogy
- explain the limits of important analogies
- refine simplified models later
- never let a convenient explanation become technically false

Useful phrasing includes:

`For now, you can think of it as...`

followed later by:

`More precisely...`

This two-stage explanation is encouraged.

## 6. Use small examples before real-scale examples

For complicated mechanisms, begin with a reduced example that preserves the real principle.

Examples:

- 12 values before a 64800-bit interleaver
- a 3-variable parity equation before a real LDPC parity-check matrix
- 4 constellation points before high-order modulation
- a 4-element vector before AIE SIMD vectors
- a small pipeline before a complete hardware processing chain

After the toy example is understood, explicitly map it to the real system.

Do not leave the learner with only the toy model.

## 7. Include real worked examples

Do not only describe mechanisms verbally.

When appropriate, work through actual values step by step.

For example:

```text
Input:
0 1 2 3 4 5 6 7 8 9 10 11

Written column-wise:

0   4   8
1   5   9
2   6  10
3   7  11

Read row-wise:

0 4 8 1 5 9 2 6 10 3 7 11
```

For mathematics, substitute actual values.

For code, trace actual variables.

For algorithms, show important intermediate states.

A learner should be able to follow the mechanism manually.

## 8. Include formulas when they are part of real understanding

Do not remove formulas merely because the learner is a beginner.

If a formula is important:

1. establish the intuition
2. show the formula
3. define every symbol
4. explain what the formula means
5. explain why it has that form
6. derive it when the derivation materially improves understanding
7. work through an example

For example, when introducing:

\[
LLR(b)=\ln\frac{P(b=0|y)}{P(b=1|y)}
\]

explain:

- what `b` means
- what `y` means
- why probabilities are compared
- why a ratio is used
- why a logarithm is useful
- how sign and magnitude should be interpreted
- that sign conventions may differ between implementations

Do not present formulas as decoration.

## 9. Include derivations when useful

For central formulas, algorithms, or architectural rules, explain where they come from when the derivation is useful for understanding.

A derivation does not always need to be fully rigorous.

Choose an appropriate level:

- intuitive derivation
- algebraic derivation
- geometric derivation
- probability derivation
- algorithmic derivation

State clearly when steps rely on assumptions.

Do not skip key reasoning steps merely to shorten the document.

## 10. Explain underlying principles

Do not only teach procedures.

Explain why the procedure works.

For example, when teaching an Interleaver, do not stop at:

`write columns, read rows`

Also explain:

- what permutation is being applied
- why positions rather than values change
- how the inverse permutation restores order
- how modulation bit positions relate to the structure
- why the downstream decoder requires the restored ordering

Likewise, when teaching software or hardware, explain the underlying execution, memory, or data-flow model.

## 11. Explain terminology carefully

When an important term first appears, use:

`Chinese name（English Name, abbreviation）`

when Chinese is the primary document language.

Example:

`对数似然比（Log-Likelihood Ratio, LLR）`

Then explain the meaning immediately.

Do not introduce many unexplained acronyms in the same sentence.

After the initial explanation, the abbreviation may be used normally.

## 12. Explicitly compare easily confused concepts

Use direct comparisons for concepts that beginners commonly confuse.

Examples:

| Concept | Meaning |
|---|---|
| bit | information value, normally 0 or 1 |
| symbol | one modulation state that may encode multiple bits |

Or:

```text
Mapper:
bits → modulation symbol

Demapper:
received noisy symbol → bit likelihood information
```

Or:

```text
Interleaver:
applies a permutation

Deinterleaver:
applies the inverse permutation
```

Explain not only that they differ, but why the difference matters.

## 13. Use diagrams and visual structures

Use text diagrams, tables, matrices, state diagrams, data-flow diagrams, timelines, or memory layouts whenever they improve understanding.

For example:

```text
BCH Encoder
     ↓
LDPC Encoder
     ↓
Interleaver
     ↓
Mapper
     ↓
Channel
     ↓
Demapper
     ↓
Deinterleaver
     ↓
LDPC Decoder
     ↓
BCH Decoder
```

Visual structure should clarify the explanation, not merely decorate it.

## 14. Explain implementation after the principle

When code or implementation matters, use:

```text
problem
↓
concept
↓
algorithm
↓
pseudocode
↓
small implementation
↓
real implementation
↓
optimization
```

Do not jump directly from a definition to production code.

For code examples, explain:

- input
- output
- data structures
- indexing
- control flow
- important memory behavior
- important language syntax
- complexity or performance when relevant

## 15. Include mathematical and computational complexity where relevant

When appropriate, discuss:

- time complexity
- memory complexity
- computational cost
- latency
- throughput
- memory bandwidth
- data movement
- parallelism
- numerical precision

Do not add these mechanically to every topic.

Include them when they materially improve understanding.

## 16. Explain assumptions

If a result depends on assumptions, state them.

Examples:

- AWGN channel assumption
- independent bit assumptions
- fixed-point representation
- a specific modulation order
- a specific code rate
- a specific software version
- a specific hardware architecture

Do not present context-dependent behavior as universal truth.

## 17. Include edge cases and limitations

Once the core mechanism is understood, explain important situations where:

- the simple mental model stops being sufficient
- conventions differ
- implementations behave differently
- numerical problems occur
- performance assumptions break down
- a standard contains special cases

Do not overload the beginner section with edge cases.

Introduce them after the main model is stable.

## 18. Explain common misconceptions

For important topics, include likely misunderstandings.

For example:

`LLR is not itself a probability.`

`An FPGA does not simply execute software instructions in parallel like a CPU.`

`An Interleaver does not change the bit values; it changes their positions.`

Explain why the misconception is tempting and what the correct model is.

## 19. Use progressive depth

A major topic may be structured internally as:

### First intuition

What should a beginner understand first?

### More precise explanation

What is really happening?

### Formal model

What equations, definitions, or structures describe it?

### Worked example

How does it behave with actual values?

### Real system

How is it used in practice?

### Deeper understanding

What additional details become important later?

This layered structure is encouraged.

It allows accessibility and depth to coexist.

# Textbook structure

## 20. Organize large topics like chapters

For broad subjects, create a coherent textbook-like hierarchy.

A useful pattern is:

```text
# Topic

## 1. Introduction and motivation

## 2. Required foundations

## 3. Core concept A

### 3.1 Intuition
### 3.2 Formal definition
### 3.3 Formula or mechanism
### 3.4 Worked example
### 3.5 Common misunderstandings

## 4. Core concept B

...

## N. Putting everything together

## Summary

## Exercises

## Further reading
```

This is a guideline, not a mandatory template.

Adapt the structure to the subject.

Do not mechanically create empty subsections.

## 21. Allow substantial length

Do not artificially compress the document.

If a topic requires substantial explanation to build a good foundation, write the necessary material.

The user prefers completeness over brevity for systematic learning material.

However, length must come from useful teaching content rather than repetition.

Avoid:

- repeating the same explanation in different words
- unnecessary history
- filler
- excessive generic introductions
- copying specification prose

A long document should be long because it teaches deeply.

## 22. Include chapter summaries

At the end of substantial chapters, include a concise summary of the most important ideas.

For example:

```text
Key points:

- A bit represents binary information.
- A modulation symbol can encode multiple bits.
- 8PSK carries 3 bits per symbol.
- A Demapper can produce soft information rather than hard 0/1 decisions.
- LLR expresses both a preferred bit hypothesis and confidence.
```

Do not simply repeat the entire chapter.

## 23. Include exercises

For substantial learning documents, include exercises where appropriate.

Prefer a progression such as:

### Concept checks

Short reasoning questions.

### Worked-style exercises

Small calculations or traces.

### Application exercises

Apply the concept to a realistic situation.

### Advanced questions

Optional deeper problems.

Exercises should test understanding rather than memorization.

Do not provide solutions immediately unless useful.

When useful, place answers or solution outlines in a separate section after the exercises.

## 24. Include self-check questions

Important sections may include a few questions such as:

- Why is this mechanism needed?
- What would fail if it were removed?
- What is the input and output?
- What is preserved?
- What changes?
- How does it connect to the previous stage?

These should help the learner test their mental model.

Do not overuse them.

# Path and file handling

## 25. Resolve the requested path

Before editing, inspect the requested target path.

Example:

`docs/notes/procom/dvb-s2/DVB-S2.md`

### Existing file

If the file exists:

- read it before modifying it
- understand its current organization
- preserve useful material
- identify gaps
- avoid duplicate explanations
- expand it systematically

Do not blindly overwrite it.

### Clearly new file

If the file does not exist and the path appears intentional:

- create missing parent directories
- create the file automatically
- do not ask for confirmation merely because the path is new

### Suspected typo or missing directory

If the requested path does not exist and nearby repository structure strongly suggests a typo or omitted directory level:

- inspect similar directories and filenames
- identify the likely intended path
- ask for confirmation before creating anything

Examples include:

- one or two incorrect letters
- omitted directory level
- duplicated directory level
- nearly identical directory name
- nearly identical filename

Example:

Requested:

`docs/notes/procm/dvb-s2/DVB-S2.md`

Existing:

`docs/notes/procom/dvb-s2/`

Ask whether `procom` was intended.

Do not ask when there is no meaningful ambiguity.

## 26. Inspect surrounding documents

When possible, inspect nearby notes to understand repository conventions:

- language
- heading style
- terminology
- code-block conventions
- Markdown formatting
- formula style
- filename conventions

Preserve useful repository conventions.

However, the textbook-style learning quality required by this skill takes priority over matching an overly terse surrounding document.

# Sources and research

## 27. Research before writing when needed

For technical topics requiring factual precision, use reliable external sources before or during writing.

Do not rely entirely on memory for:

- standards
- changing hardware platforms
- software tools
- APIs
- current libraries
- protocol specifications
- version-dependent behavior

## 28. Source priority

Prefer:

1. official standards
2. official documentation
3. university courses and textbooks
4. academic papers
5. reputable technical tutorials
6. community articles and videos as supplementary explanation

Community material can be valuable for intuition but should not override authoritative technical sources.

## 29. Use sources as references, not writing style

Official standards and vendor manuals are often difficult for beginners.

Use them to verify facts.

Do not imitate their compressed style.

Rewrite the verified material into clear educational explanations.

## 30. Point to specific references

When useful, include a Further Reading or References section.

For large documents, prefer specific guidance such as:

`ETSI EN 302 307-1, Section 5.3.3: Bit Interleaver`

rather than merely listing:

`DVB-S2 standard`

If official documentation is large, identify the sections most relevant to the learner.

# Existing document maintenance

## 31. Improve incorrect or weak existing explanations

If an existing learning document contains material that is:

- incorrect
- misleading
- too shallow for the requested systematic learning goal
- badly ordered
- contradictory

improve it when directly relevant.

Preserve good existing material.

Do not rewrite unrelated sections merely for stylistic uniformity.

## 32. Avoid duplicate knowledge

Before adding a major explanation:

- search the existing document
- identify related sections
- integrate with them
- move or extend content when necessary

Do not create parallel explanations of the same concept in unrelated locations.

# Writing quality

## 33. Use natural explanatory language

Write like a technically strong teacher.

Prefer:

`为什么需要这个步骤？`

`这里会出现一个新的问题。`

`先不要急着看公式。`

`现在把前面的两个概念连起来。`

`这个例子只是缩小规模，真实系统使用的是相同原理。`

Natural language is encouraged when it improves readability.

However, maintain professional technical accuracy.

Do not use excessive conversational filler.

## 34. Avoid AI-report style

Avoid repeated patterns such as:

`Definition:`
`Purpose:`
`Advantages:`
`Applications:`

for every section unless the structure genuinely helps.

Avoid mechanically producing five bullets for every concept.

The document should read like coherent teaching, not templated content generation.

## 35. Preserve depth even when using simple language

Simple language does not mean omitting technical content.

Prefer:

`simple explanation + precise explanation`

instead of choosing only one.

For example:

First:

`You can think of the LLR magnitude as confidence.`

Then:

`More precisely, the LLR is the logarithm of a likelihood ratio...`

This pattern is encouraged throughout the document.

# Verification

## 36. Verify both clarity and depth

After writing, review the document from two perspectives.

### Beginner review

Check:

- Are prerequisites introduced before use?
- Are important terms explained?
- Are reasoning steps visible?
- Are examples sufficient?
- Are difficult transitions too abrupt?
- Can a beginner understand why each major concept exists?

### Technical review

Check:

- Are definitions accurate?
- Are equations correct?
- Are variables defined?
- Are derivations valid?
- Are standard-specific facts verified?
- Are simplifications clearly identified?
- Are important limitations included?
- Is the depth sufficient for the stated learning goal?

The document fails if it is easy to read but technically shallow.

The document also fails if it is technically complete but unnecessarily difficult to understand.

Both requirements must be satisfied.

## 37. Verify file integrity

Also check:

- Markdown heading hierarchy
- code fences
- formulas
- tables
- diagrams
- duplicate sections
- broken links when detectable
- logical ordering
- unrelated file changes

Inspect the diff when modifying an existing document.

# Final response

After successfully creating or updating the learning file, respond briefly.

State:

- which file was created or modified
- that the topic was developed as a systematic textbook-style learning document

Do not reproduce the entire document in the response.

Do not commit or push unless explicitly requested by the user.