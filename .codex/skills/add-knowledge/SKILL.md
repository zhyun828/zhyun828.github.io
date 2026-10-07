---
name: add-knowledge
description: Add or complete a knowledge topic in an existing project note while matching that document's existing style, avoiding duplicate content, and inserting the topic at the most appropriate location.
---

# Add Knowledge

Use this skill when the user wants to add, supplement, or record a knowledge point in an existing note or documentation file in this repository.

The goal is not merely to append text. The goal is to maintain the document as if the new knowledge had originally been written as part of it.

## Input

Typical requests:

`$add-knowledge <topic> [--requirement <additional instructions>]`

`$add-knowledge <document> <topic> [--requirement <additional instructions>]`

Examples:

`$add-knowledge explicit`

`$add-knowledge basics.md explicit`

`$add-knowledge docs/notes/Cpp/basics.md 构造函数`

`$add-knowledge docs\notes\Cpp\basics.md 构造函数`

If the target document is already clear from the current task or conversation, use it.

If several documents are plausible and the target cannot be determined reliably, ask the user which document should be modified.

### Additional requirements

Use the optional `--requirement` marker after the document and knowledge points. Treat all text after the marker as additional instructions, not as more knowledge points or a document path. The text may be quoted or unquoted and may contain multiple sentences.

Examples:

`$add-knowledge docs/notes/Cpp/basics.md explicit --requirement 用中文解释，并补充一个隐式转换的反例`

`$add-knowledge basics.md 构造函数 析构函数 --requirement 对比调用时机，各给一个简短代码示例`

Requirements may specify depth, language, examples, comparisons, formulas, emphasis, exclusions, or placement. Apply them to all requested knowledge points unless the user limits a requirement to a specific point.

- Evaluate existing coverage against both the requested knowledge points and the additional requirements. A topic already present may still need an edit to satisfy a requested example or explanation.
- Explicit requirements take precedence over this skill's default choices about depth, style, and placement. Keep the changes focused on the requested knowledge and preserve unrelated content.
- Make the smallest edit that satisfies the requirements; do not omit requested detail merely because the surrounding note is concise.
- If `--requirement` is absent, retain the normal workflow and honor any clear requirements expressed in natural language.
- If the marker has no following text, ask what requirement the user intended rather than inventing one. If requirements conflict and the intended result cannot be inferred, clarify the conflict before making dependent edits.

### Multiple knowledge points

A request may contain one or multiple knowledge points.

When multiple knowledge points are requested:

- treat each knowledge point independently
- search the target document for each knowledge point separately
- evaluate whether each knowledge point is already sufficiently covered, including any applicable additional requirements
- some knowledge points may require no edit while others may require an edit
- place each addition at its own most appropriate location
- do not force unrelated knowledge points into the same section
- apply the minimal-edit principle independently to each knowledge point
- preserve the logical organization of the document
- report which knowledge points were already covered and which were modified

## Workflow

### 1. Read the target document

Before editing, inspect enough of the target document to understand:

- heading hierarchy
- language
- tone
- explanation depth
- typical section length
- code example style
- use of tables, bullets, paragraphs, and code blocks
- terminology conventions
- how related knowledge is organized

Do not impose a new writing style on the document.

### 2. Search for existing coverage

Search the whole target document for:

- the exact topic
- common aliases
- English and Chinese names when relevant
- closely related headings
- explanations that already cover the same concept without using the same title

Do not decide that a topic is absent based only on an exact keyword search.

### 3. Evaluate existing coverage

Classify the topic into one of three cases.

#### Case A: Already sufficiently covered

If the document already explains the requested knowledge sufficiently for the document's existing level of detail and satisfies any applicable additional requirements:

- do not modify the file
- tell the user that the knowledge point already exists
- identify the relevant existing section
- briefly state why no modification was necessary

Do not add duplicate wording merely because the user requested the topic again.

Judge whether a topic is sufficiently covered relative to the document's existing level of detail and the user's explicit requirements, not relative to everything that could possibly be said about the topic.

A concise beginner-oriented note should not be expanded with advanced details unless those details are necessary to match the surrounding document or meet the user's explicit requirements.
#### Case B: Present but incomplete

If the topic already exists but important information is missing:

- update the existing section
- add only the missing knowledge
- integrate the new material naturally into the existing explanation
- avoid creating a second duplicate section for the same concept

Preserve useful existing wording whenever possible.

#### Case C: Not present

If the topic is not covered:

- determine the most logical position based on the document structure and prerequisite relationships
- insert the new section there
- do not simply append it to the end unless the end is genuinely the best location

For example, a topic about copy constructors should normally be placed near constructors, object copying, or memory management rather than in an unrelated final section.

### Minimal-edit principle

Always make the smallest edit necessary to cover the requested knowledge and satisfy any applicable additional requirements.

A requested knowledge point does not require creating a new section.

If the surrounding section already covers most of the concept and only one relationship, clarification, syntax rule, or important detail is missing, add only one sentence or a few sentences at the most natural location.

Prefer, in this order:

1. No edit, if the knowledge is already sufficiently covered.
2. Add or adjust one sentence.
3. Add a few sentences or a small example.
4. Extend an existing subsection.
5. Create a new subsection only when the concept genuinely needs its own section.

Do not create a new heading merely because the user named a new knowledge point.
Do not expand a concise document unnecessarily.

### 4. Match the document's style

The added content must match the surrounding document unless the user's explicit requirements call for a different style or depth in the requested content.

Match, where applicable:

- Chinese/English language usage
- heading level
- paragraph length
- amount of explanation
- terminology
- code formatting
- comment style
- table style
- bullet style
- use of bold text
- level of technical detail

If surrounding sections are concise, stay concise.

If surrounding sections explain concepts for beginners, explain the new concept at the same level.

Do not turn a concise note into a textbook chapter unless the user explicitly requests that depth for the requested knowledge.

### 5. Content quality

Add enough information for the knowledge point to be useful and understandable in the context of the document.

Prefer:

- definition
- purpose
- important syntax
- key behavior
- a small example when it genuinely helps
- important pitfalls when directly relevant

Do not add unrelated advanced material merely to make the section look comprehensive.

Do not duplicate explanations already present elsewhere in the same document. If another existing section already explains a prerequisite, refer to the concept naturally instead of rewriting it.

### Images and local storage

When an image helps explain the requested knowledge, create it yourself or download a suitable image from the web. Save every new image in the `assert` folder directly beside the target note, using this exact folder name. Create the folder automatically when needed; do not create an empty folder when no image is needed.

For example, images for `docs/notes/Cpp/basics.md` belong in `docs/notes/Cpp/assert/` and are embedded with relative Markdown paths such as `![对象生命周期示意图](assert/object-lifetime.svg)`. Use descriptive filenames, avoid overwriting unrelated images, and include useful alt text and a caption when needed. Do not hotlink remote images or embed temporary or absolute local paths.

For downloaded images, use official or authoritative original sources and identify the source near the figure with a qualifying link as described below. Inspect created or downloaded images for readability and technical correctness before using them.

### Sources and reference links

When research or external links are needed, use only official standards, official documentation, or other authoritative original materials. These include standards bodies, vendor or project documentation, original academic papers, and university or publisher-hosted original teaching materials.

Do not cite or link to CSDN, Zhihu, personal blogs, community posts, unofficial tutorials, reposts, or other non-authoritative secondary pages. This applies to inline references, further reading, and image-source links. A well-known website alone does not make its content an authoritative original source.

Verify the source and link directly to the relevant original document, section, or paper rather than a search-results page or unofficial mirror. If a suitable original link cannot be found, do not invent a link or substitute a prohibited source. Explain any material verification gap briefly. Relative links to local images and internal note sections remain allowed.

### 6. Preserve the rest of the document

Do not rewrite, reformat, reorder, or "improve" unrelated sections.

Keep unrelated content unchanged unless a very small adjustment is necessary to integrate the new knowledge correctly.

Do not perform broad cleanup while completing this task.

### 7. Verify after editing

After editing:

- inspect the relevant surrounding section
- inspect the diff
- ensure no unrelated content changed
- ensure Markdown syntax and code fences remain valid
- ensure the new content does not duplicate existing material
- ensure every applicable additional requirement is satisfied
- ensure new images are saved in the target note's adjacent `assert` folder, their relative paths resolve, and they are readable and technically correct
- ensure all added external reference and image-source links point to official or authoritative original materials

If the edit created unnecessary duplication, fix it before finishing.

## Final response

If no edit was needed, respond briefly with:

- the topic is already covered
- where it is covered
- no file was modified

If an edit was made, respond briefly with:

- which file was modified
- whether an existing section was supplemented or a new section was inserted
- where it was placed

Do not provide a long summary of the knowledge point unless the user asks for one.

Do not commit or push changes unless the user explicitly requests it.
