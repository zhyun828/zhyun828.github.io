---
name: site-maintain
description: Incrementally check a MkDocs notes website. Default to read-only static checks; explicit site-maintain build runs a strict external build, then commits and pushes validated source changes by default.
---

# Site Maintain

Inspect newly added or currently modified notes and their directly affected dependencies. Preserve user work and the existing website conventions. Do not scan historical notes beyond what is needed to verify a changed link target, asset, or navigation entry.

## Modes and authorization

| Invocation | Behavior | Files generated |
| --- | --- | --- |
| `site-maintain` | Static checks only | None |
| `site-maintain build` | Static checks, strict MkDocs build, then commit and push validated source changes | One fixed external build tree |

The `$` prefix used to invoke a skill does not change the mode. Only the explicit `build` argument enables full build verification. A request to check anchors, inspect HTML, resolve uncertainty, or maintain a site must not silently activate a build.

For compatibility, `site-maintain check` is a static-only alias; `site-maintain links` narrows the static check to changed links, anchors, and images. `site-maintain links full` permits a broader link audit only because the user explicitly requested it. None of these aliases runs a build or writes reports.

Both primary modes are read-only with respect to formal notes, final image assets, configuration, and other user files unless the task separately and explicitly authorizes modifications. Report safe fixes instead of applying them when the request is only a check. An explicitly requested fix does not automatically authorize a build.

An explicit `site-maintain build` invocation authorizes staging, committing, and pushing validated source changes to the current branch and its configured upstream by default. It also authorizes fetching to check remote state. Honor explicit restrictions such as "do not commit" or "do not push" without asking again. Static-only modes do not authorize Git mutations. Never discard uncommitted changes or use force-push, `git reset --hard`, or `git clean -fd` as maintenance shortcuts.

## Default mode: static checks without file generation

Run this workflow for `site-maintain`:

1. Inspect Git status and diffs to determine the incremental scope. Include relevant staged, unstaged, new, moved, and deleted documentation files; do not mistake temporary output for publishable notes.
2. Read affected Markdown and assets, and the configuration needed to understand their navigation and rendering rules.
3. Check Markdown structure, internal links and anchors, image paths, navigation, and related file references using the rules below.
4. Report findings directly in the response. Do not create or save a report.

Use read-only operations and in-memory processing. Do not run `mkdocs build`, `mkdocs serve`, a documentation-generating command, or a helper that writes output. Do not create temporary files, build directories, logs, JSON inventories, screenshots, compiled examples, caches, or Markdown/HTML reports. Pass small check programs through stdin instead of saving them as scripts; for Python checks use `python -B` to avoid writing bytecode caches.

Parse configuration without running build hooks or plugins that could write files. Markdown can be rendered to an in-memory string with the relevant configured extensions when this is safe; do not write that HTML to disk. If an extension or plugin cannot be reproduced safely in memory, report the verification limitation instead of generating output or assuming an anchor is valid.

If there are no changed documentation files, report that there is no incremental documentation scope. Do not respond by scanning the entire historical site or building it. Skill files can be checked separately when the user's task is to update the skill.

### Markdown structure

Check affected headings and their hierarchy, code fences, lists, tables, and newly introduced raw HTML. Use the surrounding document's style; do not reformat unrelated sections. Detect repeated heading names and malformed links or table syntax introduced by the change.

For new pages, confirm that they are in the intended documentation tree and statically reachable through the site's existing navigation or index conventions. Static mode confirms source organization and navigation, not that MkDocs has generated a page.

### Internal links and anchors

Check changed links such as:

```markdown
[Text](#anchor)
[Text](other.md)
[Text](other.md#anchor)
```

Also inspect equivalent HTML references and reference-style Markdown links. Ignore examples inside code fences when identifying actual links. Resolve relative paths from the note, account for URL escaping, and verify that target files and explicit anchors exist. Inspect only the required target when it is an unchanged historical note.

VS Code and MkDocs can produce different heading IDs. Pay particular attention to Chinese headings, punctuation, inline code, `C++`, `std::move()`, `&&`, and duplicate headings. Verify automatic IDs using safe in-memory rendering consistent with `mkdocs.yml` rather than guessing. If plugin-specific IDs remain uncertain, state that they require explicit build verification; do not trigger a build automatically.

Stable explicit IDs may be proposed for fragile anchors when the existing configuration supports them. Only add IDs or change extensions such as `attr_list` when the task explicitly authorizes a fix; otherwise report the issue.

### Images and related resources

For newly added, referenced, modified, or relocated assets, verify existence, relative paths, capitalization, and publication location. Check that new final assets will be included by Git rather than ignored; do not stage them merely to perform a check.

Watch for local absolute paths such as `C:\Users\...`, `D:\...`, or `E:\...` in published references. Check image readability using existing files and read-only viewers when available. Do not generate previews in static mode. Report unclear or missing assets without inventing a replacement path.

### Navigation and configuration

Determine whether the site uses explicit `nav:`, automatic navigation, or index pages. Follow that convention and preserve ordering and naming. Confirm that explicit navigation paths exist and that new normal notes have an appropriate entry where required. Do not blindly add every Markdown file or reorganize the site.

Only inspect unchanged configuration as needed to interpret affected content. Do not change the theme, dependencies, `.gitignore`, or unrelated navigation. A necessary `.gitignore` change requires a clear task-related reason and disclosure; ordinary checks do not justify it.

## Build mode: explicit opt-in only

For `site-maintain build`, first complete the same static checks. Report any findings; do not silently fix formal content. Then perform strict build verification unless an unsafe output target or other external blocker prevents it. After static checks, the strict build, and affected-output verification pass, commit and push the validated source changes using the Git workflow below. Do not commit or push if validation fails, and do not report success while errors remain unresolved.

Always use this exact output directory and command on Windows:

```powershell
mkdocs build --strict --site-dir E:\Temp\mkdocs-site-check
```

`E:\Temp\mkdocs-site-check` is the user-designated external temporary build directory. Use this explicit path even if the current `%TEMP%` points elsewhere; do not change `%TEMP%` or other system environment settings.

Before the first use in a run:

- Resolve the repository and output paths and ensure the output is outside the repository and exactly the designated directory. Do not traverse an unexpected symlink or reparse point.
- If the directory already exists, confirm that it is dedicated generated MkDocs output. If it contains formal notes, user documents, configuration, or other unrelated files, stop before MkDocs cleans it; preserve those files and report the conflict.
- If the drive or directory is unavailable or unwritable, report the limitation. Do not fall back to a repository-local directory or create another output location.

Do not use `--dirty`. Reuse this same directory for every build and rebuild, relying on MkDocs' normal cleanup to replace the previous generated output. Do not create per-topic, per-run, or timestamped copies. Do not use repository-local `tmp/site-check`, `site/`, or another build folder.

MkDocs' normal cleanup preserves hidden entries whose names begin with `.`. Do not store hidden scratch files or folders inside the build directory or assume they will be removed by a build.

After building, inspect generated HTML and copied assets only for affected pages or directly referenced targets. Confirm generated pages, important IDs, fragment links, and images. The build may process the whole site; the content audit must remain incremental. Do not start unrelated cleanup of historical warnings unless they block the requested validation; report any such blocker.

If an explicitly authorized fix changes a build-relevant source, repeat static checks and rebuild into the same external directory. Keep at most this one generated build tree after success, allowing the next build to replace it. The fixed result is allowed to remain; do not attempt a separate recursive deletion merely to clean a successful build.

## Extra temporary artifacts

Default static checks generate no artifacts. PDF previews, UML rendering, C++ compilation tests, and similar extra work are not implicit parts of a static site check. If a separately authorized task actually requires them, prefer system temporary storage and record the paths created by that run. Do not change `%TEMP%` or put disposable artifacts among formal notes.

After such work, attempt safe cleanup of this run's extra disposable files, including after failure when the files are no longer needed. Verify resolved paths and ownership before deletion. On Windows, use native PowerShell operations within the verified target and check for reparse points before recursive removal.

Never delete formal notes, final images, configuration, or other user files. Do not sweep pre-existing temporary folders as part of routine maintenance; historical cleanup needs explicit authorization. Retain only diagnostics still needed for an unresolved issue or artifacts the user asked to keep, and report their locations.

If cleanup returns `blocked by policy`, stop trying to delete those artifacts. Do not retry through another command, language, tool, indirect execution, or security-setting change. Report the residual paths and the stated reason. Do not promise that cleanup succeeded or will always be permitted.

Do not modify `.gitignore` merely to hide temporary output. Do not stage or commit generated results.

## Source fixes and Git workflow

Apply only the authorized, minimal fixes: incorrect paths or anchors, missing required navigation entries, or equivalent obvious current-change problems. Preserve unrelated content and assets. Recheck fixes statically; only the explicit `build` mode enables a build.

For build mode, after validation succeeds (or when Git synchronization is separately requested):

1. Review the source diff and select only the validated notes, assets, configuration, and relevant skill changes. Preserve unrelated work. Exclude temporary files, caches, editor backups, and generated build output, including tracked `site/` changes; leave excluded working-tree changes untouched.
2. Inspect the current branch, configured remote/upstream, staged files, and existing unpushed commits. Fetch the relevant remote to check divergence. Do not include unrelated pre-staged changes in the commit. If the destination is ambiguous or the remote has diverged, stop and explain rather than guessing or rewriting history.
3. Stage the selected paths explicitly and create a descriptive commit. Do not create an empty commit or duplicate existing unpushed commits; when no new validated changes exist, push any already validated pending commits instead.
4. Push the current branch to its configured upstream. If no upstream exists but the repository has one unambiguous remote, push the same branch name there with `--set-upstream`. Verify the push result and report the commit and destination. Never force-push or publish a known broken build.

If separately requested integration changes build-relevant sources, repeat the checks and strict build before pushing. If a conflict cannot be resolved confidently while preserving user content, stop and explain.

## Final report

Respond briefly with the selected mode, affected files, findings, and verification limits. Report only counts for changed content, not unchanged historical notes. Do not save a report file.

For default mode, explicitly state that the check was static, no build was run, and no files were generated. Do not claim that HTML output or a full build was verified.

For build mode, report the static-check result, build success or failure, the fixed output path `E:\Temp\mkdocs-site-check`, and the commit/push outcome (commit ID and destination, or why either step was skipped or blocked). Confirm reuse on repeated builds when relevant. Mention any residual extra temporary files and why they remain.

Only report a commit or push as successful after it actually completes. A successful push can trigger the repository's deployment workflow; distinguish push success from deployment completion.
