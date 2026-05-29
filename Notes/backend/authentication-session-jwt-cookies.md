# Authentication Notes: Stateless HTTP, Sessions, Cookies, and JWT

## The Problem

### HTTP is Stateless

HTTP does not automatically remember previous requests.

```text
Request 1 ---> Complete

Request 2 ---> Independent
```

Example:

```http
POST /login
```

Server verifies the user.

A minute later:

```http
GET /profile
```

The server does not automatically know:

* Who is making this request?
* Did they log in previously?
* What happened in earlier requests?

Every request is treated independently.

---

## What Problem Are We Solving?

After a user logs in:

```text
User
  ↓
Authenticated
```

We need a way to identify the same user in future requests.

Example:

```text
Login
  ↓
Profile Page
  ↓
Add Expense
  ↓
View Orders
```

The server must know:

```text
These requests belong to the same user.
```

---

## Two Main Authentication Strategies

### 1. Session-Based Authentication

### 2. Token-Based Authentication (JWT)

Cookies are **not** a third authentication strategy.

A cookie is simply a storage/transport mechanism often used by both approaches.

---

## Cookie

### What is a Cookie?

A cookie is a small piece of data stored by the browser.

```text
Browser
└── Cookie
```

The browser automatically sends cookies with future requests.

Example:

```http
Cookie: sessionId=abc123
```

A cookie is only:

```text
Browser Storage
```

It can store:

* Session ID
* JWT
* Theme Preference
* Language Preference
* Cart ID

---

## Session-Based Authentication

Store user information on the server.

Store only a session identifier in the browser.

```text
Browser
   ↓
Session ID
   ↓
Server
   ↓
User Data
```

---

## Session Login Flow

### Step 1

User logs in.

```http
POST /login
```

### Step 2

Server verifies credentials.

### Step 3

Server creates a session.

Example:

```text
sessionId = abc123
```

Server stores:

```text
abc123
   ↓
{
  userId: 10,
  name: "Ammu",
  role: "Student"
}
```

### Step 4

Server sends:

```http
Set-Cookie: sessionId=abc123
```

Browser stores:

```text
sessionId=abc123
```

---

## Session Request Flow (Future Requests)

Browser automatically sends:

```http
Cookie: sessionId=abc123
```

Server:

```text
Receive Session ID
        ↓
Find Session In Store
        ↓
Get User Data
        ↓
Authenticate User
```

---
## Session Advantages & Disadvantages

| Advantages | Why? |
|------------|------|
| Very small cookie | Browser stores only a session ID (e.g., `abc123`) |
| Easy logout | Server deletes the session; all future requests immediately fail |
| Easy revocation | Server can invalidate any session at any time |
| Sensitive user data stays on server | Browser never receives actual user data |
| Permissions can change instantly | Server always reads the latest user data from its store |

| Disadvantages | Why? |
|--------------|------|
| Server must store session data | Requires memory/database for every active user |
| Extra lookup on every request | Session ID → Session Store → User Data |
| Harder horizontal scaling | Session data must be available to all servers |
| Often requires Redis/shared store | Multiple servers need access to the same sessions |

---
## Scaling Problem

Suppose:

```text
         Load Balancer
              |
      -----------------
      |       |       |
     S1      S2      S3
```

User logs in through:

```text
S1
```

Session stored on:

```text
S1
```

Later request reaches:

```text
S3
```

S3 cannot find the session.

---

## Solution: Shared Session Store

Example:

```text
         Load Balancer
              |
      -----------------
      |       |       |
     S1      S2      S3
              |
              v
            Redis
```

All servers share session data.

---

# JWT (JSON Web Token)

JWT is a token-based authentication strategy.

Instead of storing user information on the server:

Store user information inside a signed token.

```text
User Data
     ↓
JWT
     ↓
Browser
```

---

## JWT Login Flow

### Step 1

User logs in.

```http
POST /login
```

### Step 2

Server verifies credentials.

### Step 3

Server creates JWT.

Payload:

```json
{
  "userId": 10,
  "role": "student"
}
```

Server signs it.

### Step 4

Server sends token.

Example:

```text
eyJhbGc...
```

Browser stores it.

Usually:

```text
Cookie
```

or

```text
Local Storage
```

---

## JWT Request Flow (Future Requests)

Browser sends:

```http
Authorization: Bearer <jwt>
```

or

```http
Cookie: token=<jwt>
```

Server:

```text
Receive JWT
      ↓
Verify Signature
      ↓
Extract User Data
      ↓
Authenticate User
```

No session lookup required.

---



## JWT Advantages & Disadvantages

| Advantages | Why? |
|------------|------|
| No server-side session storage | User information is inside the token |
| No lookup on each request | Server only verifies the token signature |
| Easy horizontal scaling | Any server can validate the JWT independently |
| No Redis/shared session store required | Servers do not share authentication state |
| Good for APIs and microservices | Services can verify tokens without centralized session storage |

| Disadvantages | Why? |
|--------------|------|
| Logout is harder | Already-issued tokens remain valid until expiry |
| Revocation is harder | Server does not track issued tokens by default |
| Larger payload | JWT contains user data and signature, not just an ID |
| Permission changes may be delayed | Existing tokens may still contain old permissions until refreshed |
| Token leakage is more impactful | Possession of a valid JWT is often enough to authenticate |