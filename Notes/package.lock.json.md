### Problems with Only `package.json`

`package.json` stores dependency rules, not exact installed versions.

Example:

```json
"dependencies": {
  "express": "^4.18.3"
}
```

The `^` allows newer compatible versions.

So different developers may install different versions.

Example:

```txt
Developer A → 4.18.3
Developer B → 4.18.5
Developer C → 4.19.0
```

This can cause:

- inconsistent behavior
- unexpected bugs
- "works on my machine" problems

---

### Another Problem

Packages depend on other packages internally.

Example:

```txt
express
 ├── body-parser
 ├── cookie
 └── mime
```

Those dependencies may also install different versions across systems.

This makes installations non-reproducible.

---

### Solution — `package-lock.json`

package-lock.json is automatically created by npm.

`package-lock.json` stores:

- exact installed versions
- complete dependency tree

Example:

```txt
express → 4.18.3
body-parser → 1.20.2
cookie → 0.5.0
```

Now every developer installs identical versions.

This ensures:

- stable installations
- consistent behavior
- reproducible builds