# `package.json`

`package.json` is the main configuration file of a Node.js project.

It acts as a manifest that stores:

- project metadata
- dependencies
- scripts
- version information
- entry point
- license details

Without it:

- dependencies cannot be tracked
- project setup becomes difficult
- scripts cannot be managed properly

Every npm-based project typically contains a `package.json`.

---


### Example

```json
{
  "name": "backend-app",
  "version": "1.0.0",
  "main": "index.js",

  "scripts": {
    "start": "node index.js",
    "dev": "nodemon index.js"
  },

  "dependencies": {
    "express": "^4.18.3"
  },

  "devDependencies": {
    "nodemon": "^3.0.0"
  },

  "license": "MIT"
}
```

---

# Important Fields

## `name`

Project/package name.

```json
"name": "my-app"
```

---

## `version`

Uses Semantic Versioning.

```txt
MAJOR.MINOR.PATCH
1    .2    .3
```

| Change Type | Example |
|---|---|
| Bug Fix | `1.2.3 → 1.2.4` |
| New Feature | `1.2.3 → 1.3.0` |
| Breaking Change | `1.2.3 → 2.0.0` |

### Version Symbols

| Symbol | Meaning |
|---|---|
| `^4.18.3` | Allows minor + patch updates |
| `~4.18.3` | Allows only patch updates |

---

## `main`

Application entry point.

```json
"main": "index.js"
```

Node starts execution from this file.

---

# `scripts`

<summary><strong>What are scripts?</strong></summary>

`scripts` lets you create custom terminal commands inside `package.json`.

Instead of repeatedly typing long commands manually:

```bash
node index.js
```

you can create shortcuts:

```json
"scripts": {
  "start": "node index.js"
}
```

and run:

```bash
npm start
```



<details>
<summary><strong>What happens internally?</strong></summary>

When you run:

```bash
npm run dev
```

npm:

- opens `package.json`
- checks the `scripts` section
- finds the `dev` command
- executes the mapped terminal command

Example:

```json
"scripts": {
  "dev": "nodemon index.js"
}
```

Running:

```bash
npm run dev
```

actually executes:

```bash
nodemon index.js
```

</details>


<details>
<summary><strong>Common scripts</strong></summary>

```json
"scripts": {
  "start": "node index.js",
  "dev": "nodemon index.js",
  "build": "vite build",
  "test": "jest"
}
```

| Script | Purpose |
|---|---|
| `start` | Run production app |
| `dev` | Run development server |
| `build` | Create optimized build |
| `test` | Run tests |

</details>


<summary><strong>Special scripts</strong></summary>

These can run without `run`:

```bash
npm start
npm test
npm stop
```

Equivalent to:

```bash
npm run start
npm run test
npm run stop
```
---

## `dependencies`

Packages required while running the application.

```json
"dependencies": {
  "express": "^4.18.3"
}
```

Install:

```bash
npm install express
```

npm automatically updates `package.json`.

---

## `devDependencies`

Packages needed only during development.

Examples:

- testing libraries
- bundlers
- linters
- formatters
- dev tools

```json
"devDependencies": {
  "nodemon": "^3.0.0"
}
```

Install:

```bash
npm install nodemon --save-dev
```

Short form:

```bash
npm i nodemon -D
```

---

## `license`

Defines legal usage permissions.

```json
"license": "MIT"
```

| License | Meaning |
|---|---|
| `MIT` | Free use, modification, distribution |
| `ISC` | Simple permissive open-source license |
| `GPL` | Modified shared versions must remain open source |

---

# Creating `package.json`

## Interactive Setup

```bash
npm init
```

Prompts for:

```txt
package name
version
description
entry point
author
license
```

---

## Quick Setup

```bash
npm init -y
```

Creates the file instantly with default values.

---
