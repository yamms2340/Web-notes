# Linting Notes 

Linting is static code analysis used to detect issues like syntax errors, bad practices, possible bugs, unused variables/imports, and style inconsistencies without running the program.

---

## Key Tool

ESLint is the most popular JavaScript/TypeScript linter and the current industry standard.

---

## Common Usage

```json
{
  "scripts": {
    "lint": "eslint ."
  }
}
```

Run using:

```bash
npm run lint
```

---

## Best Practice

Most common modern setup:

```txt
ESLint + Prettier
```
- ESLint (Linter) → checks code quality, rules, bugs, and best practices.
- Prettier (Formatter) → fixes code appearance, spacing, and formatting automatically.
Recommended for most React/Vite/Node.js projects.

---

## Modern Alternative

Biome is a newer, very fast Rust-based alternative that combines:

- linting
- formatting

But ESLint is still the most widely used in industry.


---

- VS Code ESLint extension shows lint errors live while coding.

- `npm run lint` is still used because it:
  - checks the entire project
  - works in CI/CD pipelines
  - validates code before push/deploy
  - is editor-independent

- VS Code ESLint = live developer convenience.
- `npm run lint` = official full-project validation.