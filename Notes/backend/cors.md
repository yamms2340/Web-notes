# CORS (Cross-Origin Resource Sharing)

### What is CORS?

CORS is a browser security mechanism that controls whether JavaScript from one origin can access resources from another origin.

---

### Origin

An origin is defined by:

- protocol
- domain
- port

Examples:

```txt
http://example.com
https://example.com
http://example.com:3000
```

All are different origins.

---

### Same-Origin Request

Frontend and backend have same:

- protocol
- domain
- port

Example:

```txt
http://localhost:5000
```

No CORS required.

---

### Cross-Origin Request

Frontend:

```txt
http://localhost:3000
```

Backend:

```txt
http://localhost:5000
```

Different ports → different origins.

Browser applies CORS rules.

---

### Why CORS Exists

Prevents malicious websites from reading sensitive data from another website using JavaScript.

Example:

```txt
evil.com trying to access bank.com data
```

---

### Important Points

- CORS protects the browser/client.
- CORS does NOT mainly protect the server.
- CORS mainly controls whether frontend JavaScript can read the response.
- It does NOT always stop request sending.
- CORS is enforced by browser.

---

### Browser Role

Browser:

- runs JavaScript
- stores cookies
- sends requests
- enforces CORS rules

Examples of browsers:

- Chrome
- Firefox
- Edge

---

### How CORS Works

Frontend sends request:

```js
fetch("http://localhost:5000/api")
```

Browser adds:

```http
Origin: http://localhost:3000
```

Backend responds:

```http
Access-Control-Allow-Origin: http://localhost:3000
```

Browser checks header and decides whether frontend JS can access response.

---

<details>
<summary><strong>Bank, Evil Website, and Browser Scenario</strong></summary>

### Step 1 — User Logs Into Bank Website

User opens:

```txt
https://bank.com
```

Browser downloads frontend files from bank backend.

User logs in.

Bank backend sends session cookie:

```http
Set-Cookie: session=abc123
```

Browser stores cookie.

---

### Step 2 — User Opens Evil Website

User opens:

```txt
https://evil.com
```

Browser downloads evil website JavaScript.

---

### Step 3 — Evil JavaScript Sends Request

```js
fetch("https://bank.com/profile")
```

Browser sees request going to:

```txt
bank.com
```

Browser may automatically attach stored bank cookie:

```http
Cookie: session=abc123
```

---

### Step 4 — Bank Backend Responds

Bank backend sends response:

```json
{
  "balance": 50000
}
```

---

### Step 5 — Browser Checks CORS

Browser checks:

```txt
Did bank.com allow evil.com?
```

If backend did NOT send:

```http
Access-Control-Allow-Origin: https://evil.com
```

browser blocks evil JavaScript from reading response.

---

### Important Understanding

- JavaScript runs inside browser.
- Browser acts like security guard.
- Browser enforces CORS.
- Backend only sends permission headers.

</details>

---

### Common CORS Headers

#### Access-Control-Allow-Origin

Specifies allowed origins.

Example:

```http
Access-Control-Allow-Origin: http://localhost:3000
```

---

#### Access-Control-Allow-Methods

Specifies allowed HTTP methods.

Example:

```http
Access-Control-Allow-Methods: GET, POST, PUT, DELETE
```

Methods:

- GET → fetch data
- POST → create/send data
- PUT → update data
- DELETE → remove data

---

#### Access-Control-Allow-Headers

Specifies allowed request headers.

Example:

```http
Access-Control-Allow-Headers: Content-Type, Authorization
```

Headers:

- Content-Type → data format
- Authorization → auth token/JWT

---

### Preflight Request

For complex cross-origin requests, browser first sends:

```http
OPTIONS
```

request to check permissions.

---

### Express.js Example

Install:

```bash
npm install cors
```

Use:

```js
const cors = require("cors");

app.use(cors());
```

---

### Restricted Example

```js
app.use(
  cors({
    origin: "http://localhost:3000",
    methods: ["GET", "POST"],
    allowedHeaders: ["Content-Type", "Authorization"]
  })
);
```

---








### Simple Definition

CORS is a browser-enforced security mechanism that determines whether JavaScript from one origin can access resources from another origin.