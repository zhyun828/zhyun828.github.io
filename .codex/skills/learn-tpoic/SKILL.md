---
name: learn-topic
description: Create or systematically expand a textbook-style learning document for a topic the user wants to learn from zero. Combine beginner-friendly explanations, intuition, worked examples, formal definitions, formulas, derivations, underlying principles, implementation details, and authoritative references. Integrate learning points into the relevant explanations rather than assigning exercises. Write directly to the specified file and prioritize both clarity and technical depth.
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

`$learn-topic notes docs/notes/procom/dvb-s2/DVB-S2.md DVB-S2 --requirements include derivations, worked examples, diagrams, and sufficient mathematical depth`

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
- worked calculations and reasoning walkthroughs
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

### Zero-background prerequisite closure

When teaching from zero, do not assume prior mastery of the mathematics, probability, signal processing, computer architecture, programming, or other foundations on which the subject depends. Use a background the user explicitly establishes when available; otherwise establish the foundations needed for the requested learning path.

Before introducing a concept, formula, derivation, algorithm, or code example, inspect the prerequisites it actually uses. If any have not yet been explained, build the necessary understanding before using them. A name, translation, glossary entry, or link alone does not establish that understanding.

The learning chain should be:

```text
A is understood
↓
use A to explain B
↓
B is understood
↓
use A and B to explain C
```

Do not use several unexplained foundations together to derive a new result. This applies within paragraphs and individual derivation steps, not only to chapter ordering. Avoid circular explanations in which two unfamiliar concepts are used to define each other.

Close prerequisite gaps only to the depth needed for the next explanation. Establish a usable meaning, a small example when helpful, and the specific property that will be used; then return to the main subject. Reuse earlier explanations with a brief reminder or precise section reference instead of teaching them again.

### Prerequisite bridges

When a transition introduces unfamiliar foundations, place a short bridge immediately before their first substantive use, or extend an appropriate earlier section. For example:

- `### 在继续之前：什么是概率密度？`
- `### 在继续之前：为什么会出现 exp？`
- `### 在继续之前：复数的模平方是什么？`
- `### 在继续之前：指针与对象生命周期有什么关系？`

Explain why the bridge is needed and reconnect it to the next step. These are optional teaching shapes, not required headings for every concept or separate full courses.

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

When formal mathematics is involved, continue through three layers:

`intuitive understanding → more accurate meaning → formal mathematical expression`

For example, "closer to a candidate constellation point means more plausible" is an intuition. More precisely, under the stated AWGN model, the conditional density of the received value given that candidate decreases exponentially with squared distance. Establish the density and exponential concepts before expressing this mathematically. Inferring which candidate was sent also requires stating the applicable prior assumptions; a likelihood is not automatically a posterior probability.

Carry the assumptions and limits of the intuitive example into the formal model explicitly. Do not leave the learner at an analogy or silently change the meaning of a quantity.

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

### No unexplained mathematical objects

Before first substantive use, explain any important mathematical object, symbol, or operation that a true beginner may not know. Do not classify it as common knowledge merely because it is basic to the author.

Examples include probability, probability density, conditional probability, likelihood, random variables, expectation, variance, Gaussian distributions, logarithms, exponential functions, complex numbers, magnitudes or norms, Euclidean distance, proportionality (`∝`), summation, and vector or matrix notation.

Give the meaning and the behavior needed for the current explanation, rather than only naming a symbol. For instance, define `exp(x)` as the exponential function `e^x` and establish its relevant growth or decay behavior before using it; explain what a squared magnitude measures before manipulating it.

### Formula introduction protocol

For an important formula, develop the explanation in this order where applicable:

1. establish the problem and why a mathematical description is needed
2. identify and explain the prerequisites used by the formula
3. describe the relationship in ordinary language
4. build intuition with a diagram, numerical comparison, or small one-dimensional example when helpful
5. define the mathematical objects and notation, including the type of quantity the left side represents
6. present the formula and explain each term and operation
7. explain its origin and derive the key steps from already-understood concepts
8. substitute concrete numbers and show the calculation
9. interpret the result physically or algorithmically and connect it back to the real system

This is a teaching progression, not a requirement to create nine labeled subsections. Adapt it to the subject without dropping the conceptual bridges. Do not use "formula first, variable list afterward" as a substitute for building understanding.

### Explain what kind of quantity the formula represents

At first introduction, state what the formula computes: for example, a probability, a probability density, a log ratio, energy, power, variance, or throughput. Explain its interpretation, relevant units or normalization, and how to read the result. A symbol definition alone is insufficient.

For `p(y|s)`, establish whether the received variable is discrete or continuous and what conditioning on `s` means. For a continuous observation in the usual Gaussian model, this notation represents a conditional probability density, not the probability of one exact received value. Explain that interval or region probabilities come from accumulating density over that interval or region; an exact point has zero probability in this model, and a density value can exceed one. Introduce integration before actually using it in a derivation, rather than requiring a full calculus course to establish the density intuition.

When calling a received value "likely" informally, connect that language to its precise density interpretation. Explain that likelihood treats the observation as fixed and compares candidate model parameters or symbols; it is not by itself the probability that a candidate was transmitted.

For example, when introducing:

\[
LLR(b)=\ln\frac{P(b=0|y)}{P(b=1|y)}
\]

explain:

- the probability, conditioning, ratio, and logarithm concepts before using this expression in the learning document
- what `b` means
- what `y` means
- why probabilities are compared
- why a ratio is used
- why a logarithm is useful
- how sign and magnitude should be interpreted
- that sign conventions may differ between implementations

State whether the expression uses posterior bit probabilities or conditional-observation likelihoods, and explain their relationship and prior assumptions before switching between them. The LLR itself is a dimensionless log ratio, not a probability.

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

Build central formulas from concepts established earlier. "According to the Gaussian distribution" is not an explanation if that distribution and its relevant density have not been introduced. Likewise, "obviously" or "it follows easily" must not hide a prerequisite or a meaningful intermediate step.

When a full rigorous derivation exceeds the learning goal, provide an intuitive derivation and the key mathematical steps. Identify the omitted proof or approximation and explain its role; do not omit the reasoning needed to understand where the result comes from.

For example, a complex AWGN likelihood needs a visible chain such as:

```text
one-dimensional noise and a random variable
↓
Gaussian density, its shape, and variance as noise spread
↓
why Gaussian noise is a useful model here, and its assumptions and limits
↓
complex numbers and I/Q components
↓
two independent zero-mean real Gaussian components, each with variance σ²
↓
independence makes their joint density the product of their densities
↓
y = s + n, so n = y - s for a fixed transmitted symbol s
↓
nI² + nQ² = |y-s|²: squared Euclidean distance in the I/Q plane
↓
under the stated convention, N₀ = E[|n|²] = 2σ²
↓
p(y|s) = 1/(πN₀) · exp(-|y-s|²/N₀)
```

Explain expectation before using `E`, and explain each density, operation, and independence assumption before its step. Explain why aggregating many small noise contributions can motivate a Gaussian approximation; if invoking a theorem to justify it, establish the part of that theorem needed here rather than merely naming it.

State the noise normalization and the convention relating the sample variance to the noise spectral-density parameter; `N₀ = 2σ²` is not a universal equality for every real/complex model, filtering choice, or sample scaling. When replacing the equality by `∝`, explain that only a factor independent of the candidates being compared is omitted under the fixed-noise model, and why this preserves that comparison. Work through a small numerical example after establishing this chain.

This is an example of prerequisite closure, not a mandatory communication chapter for other subjects. Apply the same reasoning to mathematical proofs, C++ semantics, FPGA data paths, operating-system mechanisms, and other learning topics.

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

Expand mathematical foundations on demand. If density intuition and a basic definition suffice now, stop there; introduce integration, Bayes' theorem, logarithm properties, or other machinery before a later step actually depends on them. Avoid both a large unrelated foundation course at the beginning and a formula that requires foundations promised only for later.

Preserve the full learning destination: formal definitions, formulas, derivations, underlying principles, complete examples, real system parameters, implementation details, misconceptions, and edge cases remain required where relevant. Integrate knowledge that would otherwise be taught through exercises into the relevant explanations and worked examples. Add the missing steps on the way to that depth instead of removing the depth itself.

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

## 23. Integrate exercise knowledge into the explanation

Do not assign exercises, quizzes, homework, or problems for the learner to solve. Do not create exercise sections or separate answer keys.

Teach the knowledge that an exercise would have tested directly at its natural place in the learning sequence:

- explain conceptual distinctions alongside the relevant definitions
- show calculations and variable traces as fully worked examples after the relevant formula or mechanism
- explain realistic applications where the concept connects to the real system
- integrate deeper reasoning and boundary cases into the appropriate advanced discussion

Include the reasoning, intermediate steps, result, and interpretation rather than leaving them for the learner to discover. Preserve the useful knowledge and technical depth without turning the document into a question bank or collecting that knowledge at the end.

When expanding an existing document, integrate relevant exercise-only knowledge into the corresponding sections, avoiding duplication, and remove the exercise framing for that material. Keep unrelated content outside the requested scope unchanged.

## 24. Explain conceptual checks directly

Explain the points that self-check questions would otherwise test:

- why the mechanism is needed
- what would fail if it were removed
- its input and output
- what is preserved and what changes
- how it connects to the previous stage

Use these explanations where they support understanding rather than repeating a checklist in every section. A rhetorical question may introduce an explanation if it is answered immediately; do not leave unanswered self-check prompts or ask the learner to complete a derivation or calculation.

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
- dependent on unexplained prerequisites or missing intermediate reasoning
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

Read paragraph by paragraph from the perspective of a learner with genuinely zero background, rather than only checking whether the wording sounds simple.

Check:

- Are prerequisites established before use, including foundations outside the main subject?
- Are important terms, symbols, operations, and mathematical objects explained before their first substantive use?
- Are reasoning steps visible?
- Are examples sufficient?
- Are difficult transitions too abrupt?
- Can a beginner understand why each major concept exists?
- Is exercise-related knowledge explained in the appropriate sections, with no assigned exercises, unanswered self-check prompts, or separate answer keys?

### Hidden prerequisite jump review

Trace each central formula, derivation, algorithm, and transition back to the concepts already established. Look specifically for:

- a formula using unexplained mathematics, notation, or distributions
- "obviously," "easily obtained," or "according to ..." concealing non-obvious steps
- probability and probability density being conflated
- one advanced concept explained through another when neither has been established
- an abrupt jump from a small intuitive example to the formal or real-system model
- a worked calculation that teaches how to compute but not why that computation is appropriate
- a result whose origin remains unexplained even though all variables are listed
- a forward reference or external link standing in for a foundation needed immediately

Repair these gaps before completing the document: insert a concise prerequisite bridge, extend an earlier explanation, show the missing steps, or reorder dependent content. Recheck the affected learning chain after the repair. Do not wait for the learner to discover the gap and ask a follow-up question.

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
