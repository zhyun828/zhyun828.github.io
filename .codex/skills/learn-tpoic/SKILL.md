---
name: learn-topic
description: Create or systematically expand a structured learning note for a topic that the user wants to learn from zero. The skill writes directly to the specified file and is intended for comprehensive foundational learning rather than fragmented knowledge additions.
---

# Learn Topic

Use this skill when the user wants to learn a topic from zero and wants a systematic learning document generated directly in the repository.

This skill is file-oriented rather than conversation-oriented.

The goal is to create a coherent learning document that allows a beginner to build a solid foundation in the requested topic and progressively understand its important concepts, relationships, mechanisms, and applications.

## Input format

Primary format:

`$learn-topic notes <file> <topic>`

Example:

`$learn-topic notes docs/notes/procom/dvb-s2/DVB-S2.md DVB-S2`

Meaning:

- `notes`: generate or maintain a learning note
- `docs/notes/procom/dvb-s2/DVB-S2.md`: target file
- `DVB-S2`: topic to learn systematically

The topic may contain multiple words.

Examples:

`$learn-topic notes docs/notes/Cpp/templates.md C++ Templates`

`$learn-topic notes docs/notes/FPGA/versal-aie.md Versal AI Engine`

`$learn-topic notes docs/notes/math/linear-algebra.md Linear Algebra`

## Core purpose

`learn-topic` is designed for:

- learning a new subject from zero
- building a solid foundation
- creating systematic study material
- understanding the structure of a field
- learning prerequisite concepts in the correct order
- connecting related concepts into a coherent mental model

It should produce a document that can be read independently as study material.

It should not merely collect definitions or append isolated facts.

## Difference from add-knowledge

The distinction between `learn-topic` and `add-knowledge` is fundamental.

### learn-topic

Use when the user has little or no prior knowledge of the topic.

The document should be systematic and foundation-oriented.

Typical goal:

`I want to learn DVB-S2 from zero.`

Expected behavior:

- determine the important prerequisite concepts
- establish a logical learning order
- explain the topic progressively
- connect concepts together
- include examples where useful
- build a coherent learning document

### add-knowledge

Use when the user already has an existing knowledge base and wants to add, supplement, or complete one or more specific knowledge points.

Typical goal:

`Add LLR, Min-Sum decoding, and puncturing to my DVB-S2 notes.`

Expected behavior:

- inspect the existing document
- avoid duplication
- add only missing knowledge
- make minimal edits
- preserve the existing structure

In short:

`learn-topic = systematic learning from zero`

`add-knowledge = fragmented or targeted knowledge maintenance`

## 1. Resolve the target path

Before creating or editing the learning document, inspect the requested path.

Example:

`docs/notes/procom/dvb-s2/DVB-S2.md`

### Existing file

If the file already exists:

- read it first
- understand its current structure and style
- determine whether it already contains part of the requested learning topic
- preserve useful existing content
- systematically complete or reorganize only when necessary

Do not blindly overwrite an existing learning document.

### Clearly new file

If the target file does not exist but the path is reasonable and no similar conflicting path exists:

- create the required parent directories automatically
- create the target file
- do not ask for confirmation merely because the file or directory does not yet exist

For example, if neither of these exists:

`docs/notes/procom/dvb-s2/`

`docs/notes/procom/dvb-s2/DVB-S2.md`

but the surrounding repository structure makes the path reasonable, create them automatically.

### Suspected path mistake

If the requested path does not exist and there is strong evidence that the user may have made a typo or omitted an existing directory level, do not silently create a duplicate or incorrect directory tree.

Search nearby repository paths for likely intended locations.

Common suspicious cases include:

- one or two misspelled letters
- singular/plural mismatch
- capitalization mismatch when meaningful
- omitted directory level
- duplicated directory level
- a very similar existing directory
- a very similar existing filename

Example:

Requested:

`docs/notes/procm/dvb-s2/DVB-S2.md`

Existing:

`docs/notes/procom/dvb-s2/`

In this case, infer that `procm` is probably a typo for `procom` and ask the user to confirm the corrected path before editing.

Another example:

Requested:

`docs/notes/dvb-s2/DVB-S2.md`

Existing repository structure:

`docs/notes/procom/dvb-s2/`

If the surrounding structure strongly suggests that `procom` was accidentally omitted, ask for confirmation before proceeding.

Do not ask for confirmation when there is no meaningful ambiguity.

## 2. Inspect the surrounding note structure

Before writing a new file, inspect nearby notes and directories when available.

Use them to understand conventions such as:

- heading style
- filename conventions
- language
- Markdown formatting
- amount of detail
- code-block style
- use of tables
- terminology conventions

Match the repository's existing documentation style when practical.

Do not force an unrelated template onto the project.

## 3. Determine the learning scope

Treat the requested topic as a subject to be learned systematically.

Before writing, determine:

- what a beginner must understand first
- which prerequisites are necessary
- which concepts form the core of the subject
- which concepts depend on earlier concepts
- which advanced details can be postponed
- what level of depth is appropriate for a strong foundation

The default goal is:

`beginner → solid foundational understanding`

Do not assume that "beginner" means superficial.

The resulting note should establish correct mental models and enough depth for the learner to continue into more advanced material later.

## 4. Build a coherent knowledge structure

Organize the material according to the internal logic of the subject.

A common structure may be:

`motivation`
→ `basic concepts`
→ `terminology`
→ `core mechanisms`
→ `relationships between concepts`
→ `worked examples`
→ `applications`
→ `more advanced concepts`

This is only a guideline.

Do not force every subject into exactly the same outline.

For a large topic, create a sensible hierarchy of sections and subsections.

The reader should be able to understand:

- where they are in the subject
- why the current concept matters
- what previous concepts it depends on
- what concept logically comes next

## 5. Explain prerequisites when necessary

If the requested topic depends on concepts that a true beginner may not know, include the necessary prerequisites in the document.

Do not assume unexplained prerequisite knowledge.

However, avoid expanding prerequisites into unrelated full courses.

Only explain them to the depth needed to understand the main topic.

For example, a beginner DVB-S2 note may need to introduce:

- bits and symbols
- digital modulation
- channel noise
- forward error correction

before explaining concepts such as:

- Demapper
- LLR
- Interleaver
- LDPC

But it does not necessarily need a complete communications engineering curriculum.

## 6. Prefer understanding over definition collection

Do not create a glossary-like document consisting mainly of isolated definitions.

For important concepts, explain when useful:

- what it is
- why it exists
- what problem it solves
- how it works
- what goes in
- what comes out
- how it relates to surrounding concepts
- what common misunderstanding should be avoided

The note should teach relationships, not merely terminology.

## 7. Use intuitive explanation before unnecessary formalism

For difficult new concepts, prefer:

1. motivation
2. intuitive mental model
3. small example
4. precise terminology
5. formal rule or equation
6. realistic use

when this order is appropriate.

Do not begin with dense mathematics merely for rigor.

At the same time, do not avoid formulas when they are necessary for correct understanding.

When using an important formula:

- explain every important variable
- explain what the formula means
- explain why it is used
- provide an example when useful

## 8. Use examples strategically

Use small examples to make abstract mechanisms concrete.

For example, before showing a real interleaver operating on tens of thousands of values, it may be better to explain the same mechanism using 12 values.

Small examples should simplify scale without changing the actual principle.

Avoid excessive examples that make the document unnecessarily long.

## 9. Connect concepts

Explicitly explain important relationships such as:

- input/output relationships
- cause and effect
- prerequisite relationships
- inverse operations
- similarities and differences
- abstraction levels
- trade-offs
- how one stage feeds another

A systematic learning note should make these connections visible.

## 10. Distinguish core and advanced material

Prioritize foundational knowledge.

Advanced details may be included when they help complete the conceptual structure, but clearly separate them from material that a beginner must understand first.

Do not let advanced details obscure the main learning path.

If a topic can safely be postponed, state that briefly rather than fully expanding it.

## 11. Use authoritative sources when needed

For technical topics, verify information using reliable sources when appropriate.

Prefer:

1. official standards
2. official documentation
3. textbooks and university material
4. academic publications
5. high-quality technical tutorials
6. community material only as supplementary explanation

For technologies, APIs, standards, libraries, or hardware platforms that may change over time, use current sources.

When a specific standard defines behavior, prefer the standard over blogs.

Do not fill the document with a long bibliography unless useful.

References should support correctness and further study.

## 12. Language

Follow the language implied by the user and surrounding notes.

If the user is Chinese and no conflicting document convention exists, prefer clear Chinese explanations while retaining important English technical terminology.

A useful format is:

`对数似然比（Log-Likelihood Ratio, LLR）`

Use the full form when introducing an important abbreviation for the first time.

Do not repeatedly expand the same abbreviation unnecessarily.

## 13. Existing file behavior

If the learning file already exists, treat it as an evolving systematic learning document.

Before editing:

- read the whole document or enough of it to understand its structure
- search for existing coverage
- identify gaps
- avoid duplicate sections

If the file already has a reasonable structure, extend it rather than replacing it.

If the structure is clearly incomplete for systematic learning, make only the structural changes necessary to turn it into a coherent learning document.

Do not rewrite unrelated high-quality content merely to match a preferred style.

## 14. Correct existing misconceptions

If an existing learning file contains an explanation that is clearly incorrect or misleading and directly affects the topic being developed:

- correct it
- preserve useful surrounding content
- avoid leaving contradictory explanations in different parts of the file

The final document should represent the best current understanding of the topic.

## 15. File content should be study material, not process logs

Do not write:

- the user's raw questions
- conversation history
- planning commentary
- internal reasoning
- "today we learned..."
- temporary uncertainty
- unresolved guesses

Write only the resulting organized knowledge.

The file should read as if it were intentionally written as a study chapter or set of structured notes.

## 16. Scope control

Do not make the note unnecessarily encyclopedic.

The target is a strong foundation, not exhaustive coverage of everything ever written about the subject.

Include enough depth that the learner:

- understands the core concepts
- understands how they connect
- can read more advanced material afterward
- has a useful reference document

Avoid excessive historical detail, obscure edge cases, and advanced research material unless directly useful.

## 17. Verify after writing

After creating or modifying the file:

- inspect the resulting structure
- inspect the relevant content
- check Markdown syntax
- check heading hierarchy
- check code fences
- check formulas when present
- check for duplicate explanations
- check that concepts appear in a logical learning order
- check that prerequisite concepts appear before concepts that depend on them
- inspect the diff when modifying an existing file
- ensure unrelated files were not changed

## 18. Do not commit or push automatically

Creating or modifying the learning file does not imply Git commit or push.

Do not commit, push, or otherwise publish changes unless the user explicitly requests it.

## Final response

After successfully creating or updating the learning file, respond briefly.

State:

- which file was created or modified
- that the topic was organized for systematic learning from zero
- optionally mention the major sections created

Do not reproduce the entire note in the response.

If path ambiguity was detected, ask only for confirmation of the likely corrected path before making changes.