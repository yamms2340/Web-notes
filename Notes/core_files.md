# Project Structure Basics

Important files and folders commonly found in modern web projects.

---

# `.env`

Stores environment variables.

Example:

```env
PORT=5000
DB_URL=mongodb://localhost:27017
API_KEY=abc123
```

<details>
<summary><strong>Why `.env` is used</strong></summary>

Sensitive values should not be hardcoded.

Bad:

```js
const apiKey = "abc123";
```

Better:

```js
const apiKey = process.env.API_KEY;
```

Used for:

- API keys
- database URLs
- secret tokens
- environment-specific configuration

</details>

---

# `.gitignore`

Tells Git which files/folders should not be tracked.

Example:

```gitignore
node_modules
.env
dist
```

<details>
<summary><strong>Why `.gitignore` is important</strong></summary>

Some files should not be pushed to GitHub.

| File/Folder | Reason |
|---|---|
| `node_modules` | very large, can be regenerated |
| `.env` | contains secrets |
| `dist` | generated build files |

</details>

---

# `node_modules/`

Contains installed packages and their dependencies.

Created during:

```bash
npm install
```

Example:

```txt
node_modules/
├── express/
├── react/
├── vite/
└── ...
```

<details>
<summary><strong>Why `node_modules` becomes huge</strong></summary>

Installing one package may internally install many dependent packages.

Example:

```txt
express
 ├── body-parser
 ├── cookie
 └── mime
```

Some dependencies may also have their own dependencies.

This creates a dependency tree.

</details>

<details>
<summary><strong>Why `node_modules` is not pushed to GitHub</strong></summary>

`node_modules` can be recreated using:

```bash
npm install
```

So projects usually ignore it using:

```gitignore
node_modules
```

</details>

---

# `src/`

Main source code folder.

Example:

```txt
src/
├── components/
├── pages/
├── App.jsx
└── main.jsx
```

Application logic is usually written here.

---

# `public/`

Stores static files.

Examples:

- images
- icons
- static assets

Example:

```txt
public/
├── logo.png
├── favicon.ico
└── robots.txt
```

Files inside `public` are served directly by the browser.

---

# `dist/`

Generated production build folder.

Created after:

```bash
npm run build
```

Example:

```txt
dist/
├── assets/
├── index.html
└── ...
```

Contains optimized files ready for deployment.