````md
# Realtime Communication Notes — TCP, HTTP, SSE, WebSocket, Socket.IO

# Internet Communication Layers

```text
Application Layer
   ├── HTTP
   ├── WebSocket

Transport Layer
   ├── TCP

Network Layer
   ├── IP
````

---

### IP (Internet Protocol)

<details>
<summary><strong>What is IP?</strong></summary>

IP is responsible for:

* identifying devices
* routing packets across networks

Example IP:

```text
142.250.183.14
```

IP is:

* connectionless
* unreliable

It does NOT guarantee:

* delivery
* ordering
* duplicate prevention

</details>


### TCP (Transmission Control Protocol)

<details>
<summary><strong>What is TCP?</strong></summary>

TCP is a transport-layer protocol that provides reliable communication between devices.

TCP handles:

* reliable delivery
* packet ordering
* retransmission
* error checking
* connection management

TCP is:

* connection-oriented
* reliable
* ordered
* full duplex

</details>


<details>
<summary><strong>TCP 3-Way Handshake</strong></summary>

```text
Client ---- SYN ----> Server
Client <-- SYN-ACK -- Server
Client ---- ACK ----> Server
```

### SYN

```text
"Can we establish connection?"
```

---

### SYN-ACK

```text
"Yes, accepted."
```

---

### ACK

```text
"Connection confirmed."
```

After this:

* TCP connection established
* reliable communication channel exists

</details>


## HTTP (HyperText Transfer Protocol)

HTTP is an application-layer protocol used for communication between:

* clients
* servers

HTTP runs over TCP.

Protocol stack:

```text
HTTP
TCP
IP
```


## HTTP Characteristics
### Stateless

Each request independent.

```text
Request 1 -> complete
Request 2 -> unrelated
```

---

### Unidirectional

The client starts the communication, and the server only responds. The server cannot independently push messages whenever it wants.
```text
Client ---> Server
```

---

### Half Duplex

One side communicates at a time.

```text
Client sends request
Server sends response
```



<details>
<summary><strong>HTTP Request-Response Flow</strong></summary>

```text
Client ---> HTTP Request ---> Server
Client <--- HTTP Response --- Server
```

After response:

* communication ends
* client must send another request for updates

</details>

---

<details>
<summary><strong>HTTP Request Example</strong></summary>

```http
GET /users HTTP/1.1
Host: example.com
```

</details>

---

<details>
<summary><strong>HTTP Response Example</strong></summary>

```http
HTTP/1.1 200 OK
Content-Type: application/json
```

</details>

---

<details>
<summary><strong>Common HTTP Methods</strong></summary>

| Method | Purpose         |
| ------ | --------------- |
| GET    | fetch data      |
| POST   | create data     |
| PUT    | update resource |
| PATCH  | partial update  |
| DELETE | remove data     |

</details>

---

<details>
<summary><strong>Common HTTP Status Codes</strong></summary>

| Code | Meaning      |
| ---- | ------------ |
| 200  | success      |
| 201  | created      |
| 400  | bad request  |
| 401  | unauthorized |
| 403  | forbidden    |
| 404  | not found    |
| 500  | server error |

</details>

---

# Problem With HTTP For Realtime Systems

<details>
<summary><strong>Why HTTP is inefficient for realtime apps</strong></summary>

Realtime apps need continuous updates:

* chat apps
* live notifications
* multiplayer games
* stock trading

But HTTP communication ends after response.

So client repeatedly asks server:

```text
"Any updates?"
```

This leads to:

* polling
* wasted bandwidth
* unnecessary requests
* latency

</details>

---

# Polling

<details>
<summary><strong>Short Polling</strong></summary>

Client repeatedly sends requests.

```js
setInterval(() => {
   fetch("/messages")
}, 1000)
```

Problems:

* high server load
* wasted requests
* latency
* bandwidth waste

</details>

---

<details>
<summary><strong>Long Polling</strong></summary>

Client sends request.

Server waits until data available.

```text
Client ---> Request ---> Server
          waits...
Client <--- Response --- Server
```

After response:

* client sends another request

Better than short polling but still:

* repeated HTTP requests
* not true duplex communication

</details>

---

# SSE (Server-Sent Events)

<details>
<summary><strong>What is SSE?</strong></summary>

SSE is a browser technology/API built on top of HTTP that allows servers to continuously stream updates to clients over a persistent HTTP connection.

Important:

* SSE is NOT a transport protocol
* SSE still uses HTTP underneath
* SSE is part of HTML5 event streaming

</details>

---

<details>
<summary><strong>SSE Characteristics</strong></summary>

* unidirectional
* server → client only
* persistent HTTP connection
* lightweight
* automatic reconnection
* uses `text/event-stream`

</details>

---

<details>
<summary><strong>SSE Flow</strong></summary>

```text
1. Client sends HTTP request
2. Server keeps connection open
3. Server continuously streams events
4. Browser continuously receives events
```

</details>

---

<details>
<summary><strong>SSE Example</strong></summary>

## Server

```js
app.get("/events", (req, res) => {

   res.setHeader(
      "Content-Type",
      "text/event-stream"
   );

   setInterval(() => {

      res.write(
         `data: ${new Date()}\n\n`
      );

   }, 1000);
});
```

---

## Client

```js
const eventSource =
   new EventSource("/events");

eventSource.onmessage = (event) => {
   console.log(event.data);
};
```

</details>

---

<details>
<summary><strong>When to use SSE</strong></summary>

Good for:

* dashboards
* notifications
* live feeds
* analytics

NOT good for:

* chat apps
* multiplayer games

because client cannot continuously send realtime data back.

</details>

---

# WebSocket

<details>
<summary><strong>What is WebSocket?</strong></summary>

WebSocket is a full-duplex communication protocol that provides persistent bidirectional realtime communication between client and server over a single TCP connection.

Important:

* WebSocket IS a protocol
* standardized protocol
* runs over TCP
* enables low-latency realtime communication

</details>

---

<details>
<summary><strong>WebSocket Characteristics</strong></summary>

* persistent connection
* bidirectional
* full duplex
* low latency
* stateful connection

</details>

---

<details>
<summary><strong>Bidirectional vs Full Duplex</strong></summary>

## Bidirectional

Messages travel both directions.

```text
Client ↔ Server
```

---

## Full Duplex

Both sides communicate simultaneously.

Like a phone call.

</details>

---

# WebSocket Runs Over TCP

```text
WebSocket
HTTP Handshake
TCP
IP
```

---

# Full WebSocket Connection Process

<details>
<summary><strong>Step 1 — TCP Handshake</strong></summary>

```text
Client ---- SYN ----> Server
Client <-- SYN-ACK -- Server
Client ---- ACK ----> Server
```

TCP connection established.

</details>

---

<details>
<summary><strong>Step 2 — HTTP Upgrade Request</strong></summary>

Client sends normal HTTP request:

```http
GET /chat HTTP/1.1
Upgrade: websocket
Connection: Upgrade
```

## `Upgrade: websocket`

```text
"Switch this connection to WebSocket protocol"
```

---

## `Connection: Upgrade`

```text
"This connection should change protocol"
```

</details>

---

<details>
<summary><strong>Step 3 — Server Response</strong></summary>

```http
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
```

## `101 Switching Protocols`

```text
"Switching from HTTP protocol to WebSocket protocol"
```

</details>

---

<details>
<summary><strong>Step 4 — Persistent Communication</strong></summary>

After handshake:

* HTTP communication ends
* same TCP connection stays open
* WebSocket messages continuously flow

</details>

---

# WebSocket URLs

<details>
<summary><strong>ws:// vs wss://</strong></summary>

## `ws://`

Non-secure WebSocket.

---

## `wss://`

Secure WebSocket over TLS.

Equivalent of HTTPS.

</details>

---

# Browser WebSocket API

<details>
<summary><strong>Native Browser API</strong></summary>

```js
const socket =
   new WebSocket("ws://localhost:3000");
```

Browsers already provide built-in WebSocket support.

</details>

---

# Raw WebSocket Using `ws`

<details>
<summary><strong>Installation</strong></summary>

```bash
npm install ws
```

</details>

---

<details>
<summary><strong>Complete Raw WebSocket Chat App</strong></summary>

## Server

```js
const WebSocket = require("ws");

// Create WebSocket server
const server = new WebSocket.Server({
    port: 3000
});

// Store clients manually
const clients = [];

// Runs when client connects
server.on("connection", (socket) => {

    console.log("User connected");

    // Save socket
    clients.push(socket);

    // Runs when message received
    socket.on("message", (message) => {

        // Convert JSON string -> object
        const data =
           JSON.parse(message);

        // Broadcast manually
        clients.forEach((client) => {

            if (
                client.readyState ===
                WebSocket.OPEN
            ) {

                client.send(
                    JSON.stringify({
                        type: "message",
                        text: data.text
                    })
                );
            }
        });
    });

    // Runs when client disconnects
    socket.on("close", () => {

        console.log("User disconnected");

        // Remove disconnected client
        const index =
           clients.indexOf(socket);

        if (index !== -1) {
            clients.splice(index, 1);
        }
    });
});
```

---

## Client

```html
<!DOCTYPE html>
<html>

<body>

<input id="input" />

<button onclick="sendMessage()">
Send
</button>

<ul id="messages"></ul>

<script>

// Create connection
const socket =
   new WebSocket("ws://localhost:3000");

// Receive messages
socket.onmessage = (event) => {

    const data =
       JSON.parse(event.data);

    const li =
       document.createElement("li");

    li.innerText = data.text;

    document
      .getElementById("messages")
      .appendChild(li);
};

// Send message
function sendMessage() {

    const input =
      document.getElementById("input");

    socket.send(
        JSON.stringify({
            type: "message",
            text: input.value
        })
    );

    input.value = "";
}

</script>

</body>
</html>
```

</details>

---

# Problems With Raw WebSocket

<details>
<summary><strong>Manual work required</strong></summary>

You manually handle:

* event system
* broadcasting
* reconnection
* rooms
* client management
* JSON parsing/stringifying

</details>

---

# Socket.IO

<details>
<summary><strong>What is Socket.IO?</strong></summary>

Socket.IO is a realtime communication library/framework built on top of WebSocket.

Important:

* Socket.IO is NOT a protocol
* Socket.IO is NOT pure WebSocket
* usually uses WebSocket internally
* can fallback to polling

</details>

---

<details>
<summary><strong>Socket.IO Features</strong></summary>

* named events
* rooms
* broadcasting helpers
* namespaces
* auto reconnection
* heartbeat management
* polling fallback

</details>

---

# Internal Stack

```text
Socket.IO
Engine.IO
WebSocket
TCP
IP
```

---

<details>
<summary><strong>WebSocket vs Socket.IO</strong></summary>

| Feature          | WebSocket | Socket.IO |
| ---------------- | --------- | --------- |
| Protocol         | Yes       | No        |
| Event System     | No        | Yes       |
| Rooms            | No        | Yes       |
| Auto Reconnect   | No        | Yes       |
| Polling Fallback | No        | Yes       |
| Easier APIs      | Less      | More      |

</details>

---

# Install Socket.IO

<details>
<summary><strong>Installation</strong></summary>

## Server

```bash
npm install socket.io
```

---

## Client

```bash
npm install socket.io-client
```

</details>

---

<details>
<summary><strong>Complete Socket.IO Chat App</strong></summary>

## Server

```js
const express = require("express");
const http = require("http");
const { Server } = require("socket.io");

// Create Express app
const app = express();

// Create HTTP server
const server =
   http.createServer(app);

// Attach Socket.IO to server
const io = new Server(server, {

    cors: {
        origin: "*"
    }
});

// Runs when client connects
io.on("connection", (socket) => {

    console.log("User connected");

    // Listen for message event
    socket.on("message", (text) => {

        console.log(text);

        // Send to ALL clients
        io.emit("message", text);
    });

    // Runs on disconnect
    socket.on("disconnect", () => {

        console.log("User disconnected");
    });
});

// Start server
server.listen(3000, () => {

    console.log("Server running");
});
```

---

## Client

```html
<!DOCTYPE html>
<html>

<body>

<input id="input" />

<button onclick="sendMessage()">
Send
</button>

<ul id="messages"></ul>

<script src=
"https://cdn.socket.io/4.7.5/socket.io.min.js">
</script>

<script>

// Connect to server
const socket =
   io("http://localhost:3000");

// Listen for messages
socket.on("message", (text) => {

    const li =
       document.createElement("li");

    li.innerText = text;

    document
      .getElementById("messages")
      .appendChild(li);
});

// Send message
function sendMessage() {

    const input =
      document.getElementById("input");

    socket.emit(
       "message",
       input.value
    );

    input.value = "";
}

</script>

</body>
</html>
```

</details>

---

# `emit`

<details>
<summary><strong>What is emit?</strong></summary>

Used to send event.

```js
socket.emit("message", "hello")
```

</details>

---

# `on`

<details>
<summary><strong>What is on?</strong></summary>

Used to listen for event.

```js
socket.on("message", callback)
```

</details>

---

# `socket.emit()` vs `io.emit()`

<details>
<summary><strong>Difference</strong></summary>

## `socket.emit()`

Send to ONE client/socket.

---

## `io.emit()`

Send to ALL connected clients.

---

## `socket.broadcast.emit()`

Send to everyone except sender.

</details>

---

# Rooms

<details>
<summary><strong>Rooms Example</strong></summary>

```js
socket.join("room1");

io.to("room1").emit(
   "message",
   "Hello room"
);
```

Used for:

* private chats
* multiplayer lobbies
* group communication

</details>

---

# Namespaces

<details>
<summary><strong>Namespaces</strong></summary>

Examples:

```text
/chat
/admin
/game
```

Separate communication channels.

</details>

---

# When To Use SSE

<details>
<summary><strong>Use Cases</strong></summary>

Use when:

* only server pushes updates
* lightweight streaming enough

Examples:

* dashboards
* feeds
* analytics

</details>

---

# When To Use WebSocket

<details>
<summary><strong>Use Cases</strong></summary>

Use when:

* true bidirectional communication needed
* low latency required

Examples:

* chat apps
* multiplayer games
* collaborative editors

</details>

---

# Interview Questions

<details>
<summary><strong>Important Interview Questions</strong></summary>

## What problem do WebSockets solve?

Persistent low-latency bidirectional realtime communication.

---

## Difference between HTTP and WebSocket?

HTTP:

* request-response
* stateless
* half duplex

WebSocket:

* persistent
* bidirectional
* full duplex



