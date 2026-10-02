(function () {
  "use strict";

  const selectors = {
    content: ".md-content__inner",
    toolbar: ".page-search",
    mark: "mark.page-search__mark"
  };

  let searchState = null;

  function clearHighlights(content) {
    for (const mark of content.querySelectorAll(selectors.mark)) {
      mark.replaceWith(document.createTextNode(mark.textContent || ""));
    }
    content.normalize();
  }

  function collectTextGroups(content) {
    const groups = new Map();
    const blockSelector = [
      "p", "pre", "li", "td", "th", "dt", "dd", "figcaption",
      "h1", "h2", "h3", "h4", "h5", "h6"
    ].join(", ");
    const walker = document.createTreeWalker(
      content,
      NodeFilter.SHOW_TEXT,
      {
        acceptNode(node) {
          const parent = node.parentElement;
          if (!parent || !node.nodeValue) {
            return NodeFilter.FILTER_REJECT;
          }
          if (
            parent.closest(selectors.toolbar) ||
            parent.closest(
              "script, style, noscript, textarea, button, .md-clipboard, .headerlink"
            )
          ) {
            return NodeFilter.FILTER_REJECT;
          }
          if (!node.nodeValue.trim() && !parent.closest(blockSelector)) {
            return NodeFilter.FILTER_REJECT;
          }
          return NodeFilter.FILTER_ACCEPT;
        }
      }
    );

    while (walker.nextNode()) {
      const node = walker.currentNode;
      const parent = node.parentElement;
      const root = parent.closest(blockSelector) || parent;
      if (!groups.has(root)) groups.set(root, []);
      groups.get(root).push(node);
    }
    return groups.values();
  }

  function findRanges(text, query) {
    const ranges = [];
    const source = text.toLowerCase();
    const needle = query.toLowerCase();
    let start = 0;

    while (start <= source.length - needle.length) {
      const index = source.indexOf(needle, start);
      if (index === -1) break;
      ranges.push([index, index + needle.length]);
      start = index + needle.length;
    }
    return ranges;
  }

  function highlightTextNode(node, ranges) {
    const text = node.nodeValue || "";
    const fragment = document.createDocumentFragment();
    let offset = 0;

    for (const { start, end, match } of ranges) {
      fragment.append(document.createTextNode(text.slice(offset, start)));
      const mark = document.createElement("mark");
      mark.className = "page-search__mark";
      mark.dataset.pageSearchMatch = String(match);
      mark.textContent = text.slice(start, end);
      fragment.append(mark);
      offset = end;
    }

    fragment.append(document.createTextNode(text.slice(offset)));
    node.replaceWith(fragment);
  }

  function setCurrentMatch(state, index, shouldScroll) {
    const { counter, matches, previous, next } = state;
    for (const marks of matches) {
      for (const mark of marks) mark.classList.remove("page-search__mark--current");
    }

    if (!matches.length) {
      state.current = -1;
      counter.textContent = "0 / 0";
      previous.disabled = true;
      next.disabled = true;
      return;
    }

    state.current = (index + matches.length) % matches.length;
    const active = matches[state.current];
    for (const mark of active) mark.classList.add("page-search__mark--current");
    counter.textContent = `${state.current + 1} / ${matches.length}`;
    previous.disabled = false;
    next.disabled = false;

    if (shouldScroll) {
      active[0].scrollIntoView({ behavior: "smooth", block: "center" });
    }
  }

  function runSearch(state, shouldScroll) {
    clearHighlights(state.content);
    state.matches = [];
    state.current = -1;

    const query = state.input.value;
    if (!query) {
      setCurrentMatch(state, -1, false);
      return;
    }

    let matchCount = 0;
    for (const nodes of collectTextGroups(state.content)) {
      const mapping = [];
      let text = "";

      for (const node of nodes) {
        const start = text.length;
        text += node.nodeValue || "";
        mapping.push({ node, start, end: text.length });
      }

      const ranges = findRanges(text, query).map(([start, end]) => ({
        start,
        end,
        match: matchCount++
      }));
      if (!ranges.length) continue;

      for (const item of mapping) {
        const local = [];
        for (const range of ranges) {
          const start = Math.max(range.start, item.start);
          const end = Math.min(range.end, item.end);
          if (start < end) {
            local.push({
              start: start - item.start,
              end: end - item.start,
              match: range.match
            });
          }
        }
        if (local.length) highlightTextNode(item.node, local);
      }
    }

    state.matches = Array.from({ length: matchCount }, () => []);
    for (const mark of state.content.querySelectorAll(selectors.mark)) {
      state.matches[Number(mark.dataset.pageSearchMatch)].push(mark);
    }
    setCurrentMatch(state, 0, shouldScroll);
  }

  function move(state, step) {
    if (!state.matches.length) return;
    setCurrentMatch(state, state.current + step, true);
  }

  function createToolbar() {
    const toolbar = document.createElement("div");
    toolbar.className = "page-search";
    toolbar.setAttribute("role", "search");
    toolbar.setAttribute("aria-label", "本页搜索");
    toolbar.innerHTML = `
      <label class="page-search__field">
        <span class="page-search__icon" aria-hidden="true">
          <svg viewBox="0 0 24 24"><path d="M9.5 3a6.5 6.5 0 1 1 0 13 6.5 6.5 0 0 1 0-13m0 2a4.5 4.5 0 1 0 0 9 4.5 4.5 0 0 0 0-9m5.23 9.32L21 20.59 19.59 22l-6.27-6.27z"/></svg>
        </span>
        <input class="page-search__input" type="search" placeholder="本页搜索" autocomplete="off" spellcheck="false" aria-label="搜索当前页面正文">
      </label>
      <output class="page-search__counter" aria-live="polite">0 / 0</output>
      <button class="page-search__button page-search__previous" type="button" title="上一个（Shift+Enter）" aria-label="上一个匹配项" disabled>
        <svg viewBox="0 0 24 24" aria-hidden="true"><path d="m7.41 14.59 4.59-4.58 4.59 4.58L18 13.18l-6-6-6 6z"/></svg>
      </button>
      <button class="page-search__button page-search__next" type="button" title="下一个（Enter）" aria-label="下一个匹配项" disabled>
        <svg viewBox="0 0 24 24" aria-hidden="true"><path d="M7.41 8.59 12 13.17l4.59-4.58L18 10l-6 6-6-6z"/></svg>
      </button>
    `;
    return toolbar;
  }

  function initPageSearch() {
    if (searchState?.content?.isConnected) {
      clearHighlights(searchState.content);
    }

    const content = document.querySelector(selectors.content);
    if (!content || content.querySelector(selectors.toolbar)) return;

    const toolbar = createToolbar();
    content.prepend(toolbar);

    const state = {
      content,
      toolbar,
      input: toolbar.querySelector(".page-search__input"),
      counter: toolbar.querySelector(".page-search__counter"),
      previous: toolbar.querySelector(".page-search__previous"),
      next: toolbar.querySelector(".page-search__next"),
      matches: [],
      current: -1
    };
    searchState = state;

    state.input.addEventListener("input", () => runSearch(state, false));
    state.input.addEventListener("keydown", event => {
      if (event.key === "Enter") {
        event.preventDefault();
        move(state, event.shiftKey ? -1 : 1);
      } else if (event.key === "Escape") {
        event.preventDefault();
        state.input.value = "";
        runSearch(state, false);
        state.input.blur();
      }
    });
    state.previous.addEventListener("click", () => move(state, -1));
    state.next.addEventListener("click", () => move(state, 1));
  }

  if (typeof document$ !== "undefined") {
    document$.subscribe(initPageSearch);
  } else if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", initPageSearch);
  } else {
    initPageSearch();
  }
})();
