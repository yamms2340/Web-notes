# Traditional CSS vs Tailwind CSS

Both create the same final UI in the browser.

Difference is in the styling workflow.

---

# Traditional CSS

HTML:

```html
<button class="button">Click</button>
```

CSS:

```css
.button {
  padding: 10px;
  background: blue;
  color: white;
}
```

Here:

```html
class="button"
```

is only a reference name.

Browser searches CSS files for:

```css
.button
```

and applies those styles.

Flow:

```text
Custom class name
→ Separate CSS definition
→ Browser applies matching styles
```

Good for:

- large projects
- reusable design systems
- complex animations
- highly customized styling

Commonly used with:

- HTML
- React
- Angular
- Vue
- Next.js
- any frontend project

---

# Tailwind CSS

```html
<button class="px-4 py-2 bg-blue-500 text-white rounded">
  Click
</button>
```

Classes already contain predefined styling utilities.

Tailwind already provides CSS like:

```css
.px-4 { padding-left: 1rem; padding-right: 1rem; }
.bg-blue-500 { background-color: #3b82f6; }
```

Flow:

```text
Prebuilt utility classes
→ Combined directly in HTML/JSX
→ Browser applies styles
```

Good for:

- fast UI development
- React/Next.js apps
- responsive design
- modern component-based projects
- startups/MVPs/dashboard UIs

Commonly used with:

- React
- Next.js
- Vite
- Vue
- Nuxt
- Svelte

---

