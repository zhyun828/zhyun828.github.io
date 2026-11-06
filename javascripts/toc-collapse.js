(function () {
  "use strict";

  let content = null;
  let dispose = [];
  let sequence = 0;

  function init() {
    const nextContent = document.querySelector(".md-content__inner");
    if (content === nextContent) return;
    dispose.forEach(cleanup => cleanup());
    dispose = [];
    content = nextContent;

    // Material renders separate desktop and mobile TOCs. Leave primary navigation alone.
    document.querySelectorAll(".md-nav--secondary").forEach(toc => {
      const controls = new Map();

      function expand(branch, expanded) {
        const button = controls.get(branch);
        if (!button) return;
        branch.hidden = !expanded;
        button.setAttribute("aria-expanded", String(expanded));
        const label = `${expanded ? "收起" : "展开"}：${button.dataset.heading}`;
        button.setAttribute("aria-label", label);
        button.title = label;
      }

      toc.querySelectorAll(".md-nav__item").forEach(item => {
        const branch = item.querySelector(":scope > nav.md-nav");
        const link = item.querySelector(":scope > a.md-nav__link");
        if (!branch || !link) return;

        const button = document.createElement("button");
        button.type = "button";
        button.className = "toc-collapse__toggle md-icon";
        button.dataset.heading = link.textContent.trim();
        button.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true" focusable="false"><path d="M9.29 6.71 10.71 5.29 17.41 12 10.71 18.71 9.29 17.29 14.59 12z"/></svg>';
        branch.id = `toc-branch-${++sequence}`;
        branch.classList.add("toc-collapse__branch");
        button.setAttribute("aria-controls", branch.id);
        item.classList.add("toc-collapse__item");
        item.insertBefore(button, link);
        controls.set(branch, button);
        expand(branch, false);
      });

      function reveal(link) {
        // Open ancestors only: a current H2 should not expand all its children at startup.
        let branch = link?.closest(".toc-collapse__branch");
        while (branch) {
          expand(branch, true);
          branch = branch.parentElement.closest(".toc-collapse__branch");
        }
      }

      function revealHash() {
        if (!location.hash) return;
        const link = Array.from(toc.querySelectorAll("a.md-nav__link")).find(
          anchor => anchor.hash === location.hash
        );
        reveal(link);
      }

      let lastActive = null;
      function revealActive() {
        const link = toc.querySelector("a.md-nav__link--active");
        if (link === lastActive) return;
        lastActive = link;
        reveal(link);
      }

      function onClick(event) {
        const button = event.target.closest(".toc-collapse__toggle");
        if (!button || !toc.contains(button)) return;
        const branch = button.parentElement.querySelector(":scope > .toc-collapse__branch");
        expand(branch, button.getAttribute("aria-expanded") !== "true");
      }

      // Keep Material's scroll tracking/highlight; observe its active link instead of replacing it.
      const observer = new MutationObserver(revealActive);
      observer.observe(toc, { subtree: true, attributes: true, attributeFilter: ["class"] });
      toc.addEventListener("click", onClick);
      window.addEventListener("hashchange", revealHash);
      revealHash();
      revealActive();
      dispose.push(() => {
        observer.disconnect();
        toc.removeEventListener("click", onClick);
        window.removeEventListener("hashchange", revealHash);
      });
    });
  }

  if (typeof document$ !== "undefined") {
    document$.subscribe(init);
  } else if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", init, { once: true });
  } else {
    init();
  }
})();
