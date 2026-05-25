# What Happens During `npm install`

Example:

```bash
npm install express
```

npm will:

1. connect to the npm registry
2. download the package
3. download its dependencies
4. check for `node_modules`

   - if `node_modules` does not exist → npm creates it
   - if it already exists → npm reuses/updates it

5. install packages inside `node_modules`
6. update `package.json`
7. create/update `package-lock.json`

---

# What Happens During `npm install`

Example:

```bash
npm install express
```

---

### Step 1 — npm Connects to npm Registry

npm first connects to the npm registry.

```txt
https://registry.npmjs.org/
```

The npm registry is an online database that stores Node.js packages.

npm searches for:

```txt
express
```

---

### Step 2 — npm Downloads the Package

npm downloads:

```txt
express
```

But packages usually depend on other packages internally.

So npm also downloads dependencies of `express`.

Example:

```txt
express
 ├── body-parser
 ├── cookie
 └── mime
```

Some of those packages may also have their own dependencies.

This creates a dependency tree.

Example:

```txt
express
 ├── body-parser
 │    └── bytes
 │
 ├── cookie
 └── mime
```

So installing one package may internally install many packages.

---

### Step 3 — npm Checks `node_modules`

npm checks whether:

```txt
node_modules/
```

already exists.

### If it does not exist

npm creates it.

### If it already exists

npm reuses the folder and adds/updates only required packages.

---

### Step 4 — Packages Get Installed

All downloaded packages are installed inside:

```txt
node_modules/
```

Example:

```txt
project/
├── node_modules/
│   ├── express/
│   ├── body-parser/
│   ├── bytes/
│   ├── cookie/
│   └── mime/
│
├── package.json
└── index.js
```

This folder contains actual package source code.

---

### Step 5 — `package.json` Gets Updated

npm adds the package inside:

```json
"dependencies": {
  "express": "^4.18.3"
}
```

This tracks project dependencies.

---

### Step 6 — `package-lock.json` Gets Created/Updated

npm stores exact installed versions.

Example:

```txt
express → 4.18.3
body-parser → 1.20.2
bytes → 3.1.2
```

This ensures all developers install identical versions.

---

### Final Structure

```txt
project/
├── node_modules/
├── package.json
├── package-lock.json
└── index.js
```