# Project Structure Basics

## `.env`

A .env (short for environment) file is a simple text file used in software development to store sensitive data and configuration settings as key-value pairs

```env
KEY=value
```
One variable per line.


```env
PORT=5000
DB_URL=mongodb://localhost:27017
API_KEY=abc123
```

### What is `process.env`?

In Node, `process` is the running program. `process.env` is an object of environment variables.

After loading `.env`, you read:

```js
process.env.PORT        // "5000" (string)
```

If a key is missing → `undefined`.


### How `.env` gets loaded into `process.env` — `dotenv`

`dotenv` is a Node.js package that loads variables from a `.env` file into `process.env`.

Install:

```bash
npm install dotenv
```
In `server.js`:

```js
const dotenv = require('dotenv');

dotenv.config();

connectDB();
```
<details>
<summary><strong>explnation</strong></summary>


If `connectDB()` runs before `dotenv.config()`, `process.env.MONGO_URI` may be `undefined`, causing the MongoDB connection to fail.

## `require('dotenv')`

```js
const dotenv = require('dotenv');
```

Imports the `dotenv` package into the project.

## `dotenv.config()`

```js
dotenv.config();
```

`config()` is a function provided by `dotenv`.

It:

- reads the `.env` file
- parses all variables
- loads them into:

```js
process.env
```

## `connectDB()`

```js
connectDB();
```

Calls a function that connects the backend to MongoDB.

Inside `connectDB()`:

```js
await mongoose.connect(process.env.MONGO_URI);
```
- `mongoose.connect(...)`
  → connects to MongoDB

- `process.env.MONGO_URI`
  → gets the database URL from `.env`

- `await`
  → waits until the connection is completed

</details>

---
## `.gitignore`

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

## `node_modules/`

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
<summary><strong>Why is node modules not pushed</strong></summary>

- very large folder
- contains thousand of generated files(cuz installing one package may internally install many dependent packages)
- can be recreated anytime using:

Dependencies already exist in:

```json
package.json
package-lock.json
```

Anyone can restore `node_modules` using:

```bash
npm install
```

Keeping `node_modules` out of Git:

- keeps repo small
- makes Git faster
- avoids OS-specific conflicts

</details>

---

## `src/`

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

## `public/`

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

## `dist/`

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