# AgriSense Backend Notes — `backend/server.js` (and how everything connects)

These notes explain how the **Express backend** starts, which **middleware** runs, how **routes are mounted**, and how **routers → middleware → controllers → models** connect.

> Tip: Most sections are collapsible. Click the ▶ to expand.

---

## 1) What is `server.js`?

`backend/server.js` is the **entry point** of the backend. Its job is **wiring**, not business logic:

- load environment variables
- connect to MongoDB
- create the Express app
- register global middleware
- mount route modules under `/api/*`
- start listening on a port

---

## 2) The file itself (annotated)

<details>
<summary><strong>Imports (what is loaded at the top)</strong></summary>

```js
const express   = require('express');
const dotenv    = require('dotenv');
const cors      = require('cors');
const helmet    = require('helmet');
const morgan    = require('morgan');
const rateLimit = require('express-rate-limit');

const connectDB = require('./config/db');

const marketRoutes = require("./routes/market");
const expenseRoutes = require("./routes/expenses");
const inputAdvisorRoutes = require("./routes/inputAdvisor");
const referenceRoutes = require("./routes/reference");
const mlRoutes = require("./routes/ml");
```

- **Packages** (`express`, `helmet`, `cors`, `morgan`, `express-rate-limit`) are loaded from `node_modules`.
- **Local modules** (`./config/db`, `./routes/*`) are your own files.

</details>

<details>
<summary><strong>Boot phase: dotenv + DB connect</strong></summary>

```js
dotenv.config();
connectDB();
```

- **`dotenv.config()`** loads variables from `backend/.env` into `process.env`.
- **`connectDB()`** uses `process.env.MONGO_URI` and connects Mongoose.

Why order matters:
- `connectDB()` must run **after** `dotenv.config()`, otherwise `process.env.MONGO_URI` might be `undefined`.

</details>

<details>
<summary><strong>Create the Express app</strong></summary>

```js
const app = express();
```

`app` is your server instance. You attach middleware and routes onto it.

</details>

<details>
<summary><strong>Global middleware (runs before route handlers)</strong></summary>

```js
app.use(helmet());

app.use(cors({
  origin: "http://localhost:5173",
  credentials: true
}));

app.use(morgan('dev'));
app.use(express.json());
```

### (a) `helmet()`
- Adds security-related HTTP headers.
- Helps reduce common attacks by setting safer defaults.

### (b) `cors(...)`
- **CORS** = browser rule for cross-origin requests.
- This config allows the frontend dev server (`http://localhost:5173`) to call the backend (`http://localhost:5000`).

### (c) `morgan('dev')`
- Logs requests like: method, URL, status code, response time.
- Mainly for debugging during development.

### (d) `express.json()`
<details>
<summary><strong>detail</strong></summary>

# `app.use(express.json())`

Frontend usually sends data like:

```json
{
  "email": "ammu@gmail.com",
  "password": "1234"
}
```

Without:

```js
app.use(express.json());
```

```js
req.body
```

becomes:

```js
undefined
```

because Express does not parse JSON automatically.

---

```js
app.use(express.json());
```

`express.json()` is a built-in Express middleware that:

- only works for JSON requests

```http
Content-Type: application/json
```

- reads incoming JSON request bodies
- converts JSON → JavaScript object
- stores parsed data inside:

```js
req.body
```
<details>
<summary><strong>Example</strong></summary>

```js
const express = require("express");

const app = express();

app.use(express.json());

app.post("/login", (req, res) => {
  console.log(req.body);

  res.send("done");
});
```

Request Body:

```json
{
  "email": "ammu@gmail.com",
  "password": "1234"
}
```

Console Output:

```js
{
  email: "ammu@gmail.com",
  password: "1234"
}
```

Now JSON is converted into a JavaScript object.

</details>

---


### Working of `app.use(express.json())`

```js
app.use(express.json());
```


```js
express.json()
```
runs once when server starts.
It creates and returns a middleware function.



```js
app.use(...)
```
stores that middleware inside Express middleware stack.



Whenever a request comes:

```txt
Request
   ↓
stored middleware executes
```

Middleware:

- reads request body
- checks `Content-Type: application/json`
- converts JSON text → JavaScript object
- stores result in `req.body`
- calls `next()`


Request moves to next middleware/route handler.

```txt
Middleware
   ↓
next()  → tells Express to continue to next matching middleware/route
   ↓
Next middleware / Route Handler executes
```


</details>



---


</details>

<details>
<summary><strong>Rate limiting middleware (scoped to /api)</strong></summary>

```js
const limiter = rateLimit({
  windowMs: 15 * 60 * 1000,
  max: 100,
  message: { message: 'Too many requests, please try again after 15 minutes' }
});
app.use('/api/', limiter);
```

- Applies only to paths starting with `/api/`.
- Helps prevent abuse (too many requests from one client/IP in a short time).

</details>

<details>
<summary><strong>Mounting routers (how URLs map to route files)</strong></summary>

```js
app.use('/api/auth', require('./routes/auth'));
app.use("/api/market", marketRoutes);
app.use("/api/expenses", expenseRoutes);
app.use("/api/input-advisor", inputAdvisorRoutes);
app.use("/api/reference", referenceRoutes);
app.use("/api/ml", mlRoutes);
```

### What “mounting” means

Mounting means: **“For any URL that starts with this prefix, send it to that router.”**

Example:
- mount prefix: `/api/auth`
- inside `routes/auth.js`: `router.post("/login", login)`
- full URL becomes: `POST /api/auth/login`

</details>

<details>
<summary><strong>Health route and listen (server starts accepting requests)</strong></summary>

```js
app.get('/', (req, res) => res.json({ status: 'AgriSense Backend Running' }));

const PORT = process.env.PORT || 5000;
app.listen(PORT, () => console.log(`🚀 Server running on port ${PORT}`));
```

- `GET /` is a simple “server is alive” check.
- `PORT` comes from `.env` or defaults to 5000.
- `listen()` is the moment your backend becomes reachable.

</details>

---

## 3) Router files vs controller files vs middleware files (roles)

<details>
<summary><strong>Router files (routes)</strong></summary>

**Where:** `backend/routes/*.js`

Router File:
- defines API endpoints (URLs)
- defines HTTP methods
- runs middleware
- maps requests to controller functions



Typical format:

```js
const express = require("express");
const auth = require("../middleware/auth");
const {
  getExpenses,
  updatePlan,
  addExpense,
  deleteExpense,
  clearExpenses,
} = require("../controllers/expensesController");

const router = express.Router();

router.get("/", auth, getExpenses);
router.put("/plan", auth, updatePlan);
router.post("/", auth, addExpense);

- `router.put()`     → handles PUT requests
- `router.delete()` → handles DELETE requests


HTTP Method : DELETE
URL         : /api/expenses/:id

Example:
DELETE /api/expenses/123

→ auth middleware runs
→ deleteExpense() controller runs
*/
router.delete("/:id", auth, deleteExpense);


router.delete("/", auth, clearExpenses);

module.exports = router;
 
```

</details>

<details>
<summary><strong>Controller files</strong></summary>

**Where:** `backend/controllers/*.js`

**Job:** the “real logic” for an endpoint:
- validate input (`req.body`, `req.params`, `req.query`)
- call MongoDB models and/or external APIs
- return a JSON response

Typical format (handler/controller function):

```js
const handler = async (req, res) => {
  // read input
  // do work
  return res.json({ ok: true });
};
```

Controller/handler functions usually have signature:
- `(req, res)` (no `next`) because they end the request by responding.

</details>

<details>
<summary><strong>Middleware files</strong></summary>

**Where:** `backend/middleware/*.js`

**Job:** run before the handler to do cross-cutting tasks (auth, validation, parsing, etc.).

Middleware signature:
- `(req, res, next)`

Key rule:
- If it calls `next()`, the request continues.
- If it sends a response (e.g., `res.status(401).json(...)`), it stops the chain.

</details>

---

## 4) How `middleware/auth.js` connects (JWT protection)

<details>
<summary><strong>What it does</strong></summary>

`backend/middleware/auth.js` protects routes by requiring a valid JWT token:

- expects header: `Authorization: Bearer <token>`
- verifies token with `process.env.JWT_SECRET`
- attaches decoded payload to `req.user` (example: `{ id, email }`)

</details>

<details>
<summary><strong>How it is used in routes</strong></summary>

Example (Expenses):

```js
router.get("/", auth, getExpenses);
```

Meaning:
1. run `auth(req,res,next)`
2. if token ok → `next()` → run `getExpenses(req,res)`
3. if token missing/invalid → respond 401 and stop

</details>

---

## 5) “Full flow”: one request end-to-end

<details>
<summary><strong>Example A: Public login (no auth middleware)</strong></summary>

Request:

```http
POST /api/auth/login
Content-Type: application/json

{ "email": "...", "password": "..." }
```

Flow:
1. `server.js` global middleware (`helmet` → `cors` → `morgan` → `express.json` → rate limit)
2. mount match: `/api/auth` → `routes/auth.js`
3. route match: `POST /login` → `authController.login`
4. controller checks DB + bcrypt → returns `{ token, user }`

</details>

<details>
<summary><strong>Example B: Protected expenses (uses auth middleware)</strong></summary>

Request:

```http
GET /api/expenses
Authorization: Bearer <jwt>
```

Flow:
1. `server.js` global middleware
2. mount match: `/api/expenses` → `routes/expenses.js`
3. route match: `GET /` → middleware `auth`
4. `auth` verifies JWT and sets `req.user`
5. controller uses `req.user.id` to read/write the user’s `ExpenseRecord`

</details>

---

## 6) Response format & return patterns you’ll keep seeing

<details>
<summary><strong>Common response patterns</strong></summary>

### Success

```js
return res.json({ ... });
// or
return res.status(201).json({ ... });
```

### Client error (bad input)

```js
return res.status(400).json({ message: "..." });
```

### Auth error

```js
return res.status(401).json({ message: "..." });
```

### Server error (unexpected)

```js
console.error(err);
return res.status(500).json({ message: "..." });
```

</details>

