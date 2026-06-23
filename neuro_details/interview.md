# 🧠 NeuroProject: Comprehensive Interview Script & Q&A

This document contains detailed answers to common interview questions based on the architecture, technologies, and challenges of the NeuroProject. It is designed so you can read the narratives naturally, covering both deep technical details and your engineering thought process.

## 📑 Quick Links
- [1. Explain the project and the problem it solves](#1-explain-the-project-and-the-problem-it-solves)
- [2. Tech Stack Deep Dive: What, Why, and Cross-Questions](#2-tech-stack-deep-dive-what-why-and-cross-questions)
- [3. Can you explain the overall architecture and workflow of the application?](#3-can-you-explain-the-overall-architecture-and-workflow-of-the-application)
- [4. Development Challenges & Engineering Decisions](#4-development-challenges--engineering-decisions)
- [5. Comprehensive Glossary & Architecture Concepts](#5-comprehensive-glossary--architecture-concepts)

---

## 1. Explain the project and the problem it solves

**Project Overview:** 
NeuroProject is a comprehensive, full-stack web application designed to create an interactive and adaptive learning environment for children, accompanied by a Parent/Child monitoring dashboard. The platform features real-time study rooms, an AI-powered chatbot, and interactive educational games.

**The Target Audience & The Problem it Solves:** 
Traditional educational tools often use a "one-size-fits-all" approach, which is particularly ineffective for children with special educational needs. **NeuroProject is specially designed for children with Down Syndrome (Trisomy 21)**, focusing on those experiencing mild to moderate cognitive and developmental delays.

Children with Down Syndrome often benefit from highly visual, repetitive, and tailored learning experiences. NeuroProject solves this by providing **adaptive learning**. It monitors the child's game performance (accuracy, time taken, consistency) and sends this data to a Machine Learning model to calculate a dynamic cognitive/IQ score. Based on this score, the system automatically adapts the difficulty of future questions—ensuring the child is always appropriately challenged without becoming frustrated. Additionally, it provides parents with real-time analytics and visual graphs to track their child's developmental progress over time.

---

## 2. Tech Stack Deep Dive: What, Why, and Cross-Questions

### ⚛️ React.js (Frontend)
"If asked about React, I explain that it is an open-source, front-end JavaScript library developed by Facebook for building user interfaces. It provides a way to build complex, interactive web applications by breaking them down into small, isolated, and reusable pieces of code called components. I chose React over Vanilla JavaScript because it utilizes a 'Virtual DOM'—a lightweight, in-memory representation of the actual web page. Instead of the browser expensively reloading the entire page every time the user navigates or data changes, React intelligently calculates the most efficient way to update *only* the specific components that changed. This makes the app feel incredibly fast and seamless, much like a native mobile app. A great example of this in my project is component reusability: in my `App.js` entry point, I placed the `<Navbar />` and `<Footer />` components completely outside the routing block. This means I wrote them exactly once, and they remain glued to the screen across every single page without ever reloading."

### 🎨 Tailwind CSS (Styling)
"Tailwind is a 'utility-first' CSS framework. Unlike traditional CSS where you write custom class names and constantly switch between HTML and CSS files, Tailwind provides a massive set of low-level, pre-defined utility classes—like `flex` for flexbox, `p-4` for padding, or `min-h-screen` for height—that let you style elements directly inside your JavaScript component. What it provides is extreme development speed, design consistency, and a highly optimized final CSS file, because it automatically strips out any unused classes during the build process. I chose it over standard CSS files because standard CSS often leads to massive, hard-to-maintain stylesheets with naming conflicts. I also chose it over Bootstrap because Bootstrap forces your site to look very generic, whereas Tailwind gave me complete freedom to do rapid, custom UI development without ever leaving my React files."

### 🟢 Node.js & Express.js (Main Backend)
"For my main backend, I used Node.js and Express. Node.js is a cross-platform runtime environment built on Chrome's V8 JavaScript engine. What it provides is the ability to execute JavaScript code outside of a web browser, directly on the server. Its defining feature is its non-blocking, event-driven architecture. Because it operates on a single thread with an 'Event Loop', it is incredibly efficient at handling thousands of concurrent I/O operations—making it the absolute perfect choice for my real-time Socket.IO chat feature.
Express.js is the most popular web application framework built on top of Node.js. While Node provides raw HTTP server capabilities, building an API from scratch with raw Node is tedious. Express provides a robust, lightweight set of tools to easily set up RESTful API routes (GET, POST, PUT), parse incoming data, and manage middleware (like CORS). I chose this combination over frameworks like Python Django or Java Spring because it allowed me to use JavaScript across the entire stack—from frontend to backend—avoiding constant context-switching."

### 🐍 Python & Flask (Data Microservice)
"Flask is a lightweight web server framework for Python. I introduced Python into my stack—even though I already had a Node backend—because of a fundamental limitation in Node.js: it is single-threaded. While Node is fantastic for handling I/O operations like chat messages or database fetches, it completely freezes if you ask it to perform heavy CPU tasks. Drawing visual progress graphs and processing large datasets would have blocked my Node server, crashing the study rooms. Because Python is the industry standard for data science, I built a Flask microservice specifically for the heavy lifting. Node just hands off the data, and Python uses `pandas` and `matplotlib` to generate the graphs without disrupting the main server."

### 🍃 MongoDB & Mongoose (Database)
"MongoDB is a NoSQL database that stores data in flexible JSON-like documents, and Mongoose is the Object Data Modeling library I used to define my schemas. I chose MongoDB over a relational SQL database like PostgreSQL because educational data can be highly unstructured and evolve rapidly, which maps perfectly to flexible JSON documents. More importantly, in a SQL database, linking a Parent to multiple Children requires complex, expensive `JOIN` tables. In Mongoose, I was able to simply store an array of Child Object IDs directly inside the Parent document and use the `.populate('children')` method to fetch all the related data instantly and efficiently in a single command."

### 🧠 External ML Server (Ngrok) & Dynamic Adaptation
"To adapt the difficulty of the educational games, I deployed a custom Machine Learning model. The ML model is hosted on a completely separate external Python server, which is securely exposed to the internet via Ngrok. When a child finishes a game, React triggers a bridge endpoint (`callmlmodel.js`) on my Node server. Node then uses Axios to ping the Ngrok URL of the ML server. The ML server processes the game metrics, calculates a new dynamic IQ score, and updates the database. The next time the child plays, the game automatically pulls questions matching that new difficulty level."

### 💬 Socket.IO & 🤖 Gemini API
"Socket.IO is a library that enables persistent, real-time, bi-directional communication. I used it for the Study Rooms because standard HTTP 'polling'—where the app asks the server for new messages every second—is far too slow and resource-intensive. Socket.IO creates a persistent WebSocket tunnel, allowing the server to instantly push messages directly to clients. Additionally, I integrated the `@google/generative-ai` Gemini API to power the AI Chatbot capabilities natively within the platform."

---

## 3. Can you explain the overall architecture and workflow of the application?

The application follows a **Microservices-inspired Architecture** to ensure separation of concerns and scalability:
1.  **The Client Layer:** The React Frontend (`localhost:3000`) acts as the user interface, sending HTTP requests via `axios` or `fetch`.
2.  **The Gateway/Router Layer:** The Express backend (`localhost:3016`, `localhost:3017`, `localhost:8080`) intercepts these requests. The Express Router acts as a traffic cop, directing requests to specific controllers (the "brains") based on the URL.
3.  **The Microservices Layer:**
    *   **Main Server (`Server.js`):** Handles daily operations like Calendar, Tasks, and Socket.IO real-time chat.
    *   **Auth Gateway (`LoginPage.js`):** Handles authentication and OTPs.
    *   **Game Server (`modelserver.js`):** Isolates game logic and score tracking.
    *   **Analytics Server (`pyserver.py`):** Handles heavy math and draws graphs.
    *   **External ML Server:** An ML model hosted via Ngrok that calculates dynamic IQ scores.
4.  **The Database Layer:** MongoDB stores user profiles, game datasets, and questions.

**Workflow Example (Adaptive Game Play):**
1.  **Trigger:** A child plays a game on the React frontend. React calculates metrics (Accuracy, Time) and sends them to the Node.js backend.
2.  **Storage:** The Node backend (`updateGameUserInsertionDataset.js`) saves these metrics into the user's MongoDB dataset.
3.  **ML Bridge:** React triggers a bridge endpoint (`callmlmodel.js`), which pings the external Python ML server.
4.  **Analysis:** The ML server calculates a new dynamic IQ Score and updates the database.
5.  **Adaptation:** The next time the child plays, the game router (`giverandquesgame.js`) reads this new score and automatically pulls appropriately difficult questions from the `dataset.json` question bank.

---

## 4. Development Challenges & Engineering Decisions

### Challenge 1: Handling Heavy Math and Graphing
"When I was building the parent dashboard, I needed to process large datasets and generate visual progress graphs. Initially, I tried doing this directly in Node.js. The problem I quickly encountered was that Node is single-threaded. Because generating graphs is a CPU-intensive task, it completely blocked the Node event loop—meaning the server froze, and real-time features like the chat rooms stopped working. 

My thought process was that I couldn't block the main thread, and while I considered Node's worker threads, Node still isn't built for heavy data manipulation. I asked myself what the industry standard for data science was, which led me to Python. 

I realized the best approach was to separate these concerns using a microservices architecture. I built a standalone Python Flask microservice specifically to handle the heavy lifting. Now, the Node backend simply forwards the raw JSON data to Python. Python leverages powerful libraries like `pandas` to process the data and `matplotlib` to draw the graph. It saves the graph in RAM and sends it back to React as an image file. This entirely removed the CPU load from Node, keeping the main server blazingly fast."

### Challenge 2: Preventing Document Bloat in MongoDB
"As children played multiple games on the platform, I was pushing every single game result into an array directly inside their MongoDB user document. I quickly realized these user documents were growing endlessly. While MongoDB itself has massive storage, individual documents have strict size limits, and endlessly growing arrays drastically slow down database queries when fetching user profiles.

My thought process was to evaluate what data the Machine Learning model actually needed. It didn't need a lifetime history of every single game; it only needed recent performance to adapt the difficulty. I needed a way to cap the size of the dataset array without having to write complex, periodic database cleanup scripts.

Thinking back to core data structures, I was inspired by the concept of a 'Sliding Window' or a FIFO (First In, First Out) queue. I decided to implement this logic directly in my game metrics API routes. 

I engineered a specific 'Memory Manager' route (`updateGameUserDeletionDataset.js`) that enforces a strict limit, keeping only the most recent 8 games in the array. When a 9th game is played, the Node server uses an `array.shift()` operation to delete the oldest game before pushing the new one into MongoDB. This simple data structure solution completely eliminated the document bloat and kept my database queries incredibly fast."

### Challenge 3: Rapid Prototyping Security Debt vs. Production Readiness
"To rapidly build and test the complex interactions of the Parent/Child dashboard, I took a shortcut: I temporarily bypassed strict security, comparing plain text passwords and using raw email addresses for cookie sessions. 

The problem was obvious—this was a critical security vulnerability. My thought process was that while prototyping speed is important, I couldn't deploy this to production. I needed a robust authentication flow that wouldn't require me to rewrite my entire backend architecture from scratch.

I researched industry standards for Node.js authentication and found that `bcrypt` and JSON Web Tokens (JWT) were the ideal combination for secure, stateless sessions. 

I treated the prototype as known technical debt and immediately laid the groundwork for production. I've installed the necessary libraries, and my defined next step prior to deployment is to replace the plain text comparisons with `bcrypt.hash()` and to generate signed JWTs for the cookies. This will fully secure the app against tampering, session hijacking, and XSS attacks."

### Challenge 4: Real-time Communication Overhead
"When I first built the Study Rooms, I relied on standard HTTP requests. This meant the React client had to continuously 'poll' the server every second to ask if there were any new messages. This created massive, unnecessary network traffic and overhead.

My thought process was that HTTP is a stateless, one-way street, and I desperately needed a two-way connection where the server could push data to the client independently, without the client having to ask. 

I looked into raw WebSockets, but managing reconnections and fallbacks manually can be complex. My research led me to `Socket.IO`, which not only wraps WebSockets in a much easier API but also includes a built-in 'Rooms' feature.

This was the perfect solution. I integrated Socket.IO to establish persistent WebSocket connections. Now, instead of polling, the server instantly `emits` messages directly to specific virtual rooms. This completely eliminated the polling overhead and created a truly instantaneous chat experience."

### Challenge 5: Managing Complex Database Relationships
"When designing the database for the Parent/Child dashboard, I had to figure out how to link one parent to multiple children. In a traditional SQL database, this requires setting up foreign keys and writing complex, slow `JOIN` queries just to load the basic dashboard.

My thought process was that I wanted to avoid writing huge SQL queries and needed a database that handled related, nested data more naturally. 

This inspired my choice of MongoDB. Because it's a NoSQL document database, it handles arrays natively. I realized I didn't need a separate relational table; I could just put an array of references directly inside the parent document.

My solution was to utilize Mongoose schemas to store an array of Child Object IDs directly inside the Parent document. When fetching the dashboard, I simply use Mongoose's `.populate('children')` method. This acts as an incredibly efficient, automated join that fetches the full profiles of all associated children in a single, clean command."

---

## 5. Comprehensive Glossary & Architecture Concepts

*If an interviewer drills down into specific terms or workflows, use this section as your cheat sheet.*

### 📑 Section 5 Quick Links
- [5.1 Core Frontend Concepts](#51-core-frontend-concepts)
- [5.2 Core Backend Concepts](#52-core-backend-concepts)
- [5.3 Python & Data Science Stack](#53-python--data-science-stack)
- [5.4 Security Concepts](#54-security-concepts)
- [5.5 Ecosystem File Architecture & Purpose](#55-ecosystem-file-architecture--purpose)
- [5.6 Key Workflows](#56-key-workflows)

### 5.1 Core Frontend Concepts

*   **`useState` (React Hook)**
    *   **What it is:** A built-in React function that allows components to create and manage their own internal memory (state).
    *   **How it works:** It provides a variable to hold data and a setter function to update it. When the setter function is called, React detects the change and automatically triggers a UI re-render.
    *   **Project Example:** `const [tasks, setTasks] = useState([]);` When a user adds a new task to their planner, calling `setTasks(newTasksArray)` instantly updates the screen to display the new task without a page refresh.

*   **`useEffect` (React Hook)**
    *   **What it is:** A Hook used to perform "side effects"—operations that reach outside the React component, such as fetching data from an API, setting up subscriptions, or manually changing the DOM.
    *   **How it works:** It runs automatically after the component renders on the screen. You can control *when* it runs using a dependency array (e.g., run once on load, or run every time a specific variable changes).
    *   **Project Example:** When a user navigates to the Dashboard, a `useEffect` block instantly fires an Axios request to the Node backend to fetch the user's profile data so the screen populates immediately upon loading.

*   **React Router DOM**
    *   **What it is:** The standard library for routing in React applications.
    *   **How it works:** It enables "client-side routing." Instead of the browser asking the server for a brand-new HTML page when a user clicks a link, React Router intercepts the URL change and instantly swaps the visible component.
    *   **Project Example:** When a parent clicks from the "Home" tab to the "Analytics" tab, React Router simply unmounts the `<Home />` component and mounts the `<Analytics />` component, providing the lightning-fast feel of a native mobile app.

*   **Axios vs. Fetch API**
    *   **What they are:** Both are methods used to send HTTP requests (like GET or POST) from the frontend to the backend API.
    *   **The Difference:** `fetch` is natively built into modern browsers but is relatively bare-bones; you have to manually parse the response into JSON (e.g., `res.json()`). **Axios** is a third-party library that automatically parses JSON data, handles network errors more gracefully, and simplifies sending headers or cookies.
    *   **Project Example:** I used Axios for data fetching. For example, `axios.post('/api/login', { email, password })` cleanly sends the login credentials as a JSON payload in a single, readable line of code.

### 5.2 Core Backend Concepts

*   **Express.js (Routing & Middleware)**
    *   **What it is:** A minimal and highly flexible web application framework built on top of Node.js.
    *   **How it works:** Raw Node.js can handle HTTP requests, but it requires writing tedious, low-level code. Express abstracts this away by providing two main tools: **Routers** (to cleanly map URLs to functions) and **Middleware** (functions that execute during the lifecycle of a request, such as checking user authentication before allowing access to a route).
    *   **Project Example:** I used Express to quickly spin up my API endpoints (`app.get('/api/user', controllerFunction)`), which allowed me to focus on business logic rather than server configuration.

*   **Router vs. Controller Pattern**
    *   **What it is:** An architectural design pattern used to keep backend code organized and maintainable by separating navigation from business logic.
    *   **Router (The Traffic Cop):** Simply listens for specific URLs (e.g., `/api/fetch-score`) and directs the request to the correct hallway. It contains zero business logic.
    *   **Controller (The Brains):** The function at the end of the hallway. It receives the request, queries the database, processes the data, and sends the response.
    *   **Project Example:** My Express route file says `router.get('/score', getScoreController)`. The `getScoreController` lives in a separate file, making the code highly modular.

*   **Request (`req`) & Response (`res`) Flow**
    *   **What it is:** The fundamental communication cycle of the web. 
    *   **How it works:** The Client (React) builds an HTTP **Request (`req`)** containing parameters, body data, and headers, and sends it across the network. The Server (Express) catches it, processes it, and formats an HTTP **Response (`res`)**—usually a JSON object or an error code—and sends it back.
    *   **Project Example:** When saving a game, React sends a `req` with `{ accuracy: 80 }` in the body. Express processes it and replies with `res.json({ success: true })`.

*   **CORS (Cross-Origin Resource Sharing)**
    *   **What it is:** A strict security mechanism built into modern web browsers.
    *   **How it works:** By default, a browser will block a frontend running on `localhost:3000` from requesting data from a backend running on `localhost:3016` because they are on different "origins" (ports). 
    *   **Project Example:** I had to explicitly configure the `cors` middleware in my Express `Server.js` file to whitelist `http://localhost:3000`, thereby telling the browser that my frontend is "safe" and allowed to communicate with my backend.

### 5.3 Python & Data Science Stack

*   **Python (The Language Choice)**
    *   **What it is & What it provides:** Python is the industry-standard language for data science and machine learning. What it provides is access to highly optimized, massive data libraries that simply don't exist (or are very slow) in the Node.js ecosystem. 
    *   **Why we chose it over Node.js:** Node.js is single-threaded. While it is fantastic for handling thousands of fast chat messages, it completely freezes if forced to do heavy math or draw images. We used Python specifically to offload this heavy CPU processing so the main Node server wouldn't crash.

*   **Pandas**
    *   **What it is & What it provides:** A highly optimized data manipulation library for Python. It provides the "DataFrame"—a 2D table of data (like an Excel spreadsheet in code) that allows for incredibly fast filtering, grouping, and statistical calculations on massive datasets.
    *   **How it connects in the flow:** When Node sends hundreds of raw game scores as JSON to our Python server, Pandas is the very first thing to touch it. It instantly organizes that raw JSON into a DataFrame, calculating the moving averages and performance trends required for the parent dashboard. Because we *needed* Pandas to do this math efficiently, we had to use Python instead of Node.

*   **Matplotlib**
    *   **What it is & What it provides:** A comprehensive library for creating visual graphs and charts in Python. It provides the mathematical plotting functions needed to turn raw numbers into visual UI elements.
    *   **How it connects in the flow:** After Pandas calculates the trends, Matplotlib takes those numbers and physically draws the line graph. It plots the X and Y axes, draws the data points, and outputs the final visual graph as a PNG image file directly into RAM. It is because of Matplotlib's powerful rendering capabilities that we routed this entire workflow through Python.

*   **Flask**
    *   **What it is & What it provides:** A lightweight "micro" web server framework written in Python. Since our data logic was written in Python (for Pandas/Matplotlib), we needed a way for our Node.js server to talk to our Python code over the network. Flask provides exactly that: a simple way to expose Python functions as HTTP API endpoints.
    *   **How it connects in the flow:** Node.js cannot run Python code directly. So, we run Flask on a separate port. When a parent requests the analytics dashboard, React hits Node. Node then sends an HTTP POST request to our Flask endpoint (`/generate-graph`). Flask receives the data, triggers the Pandas/Matplotlib script, grabs the resulting PNG image, and returns it to Node as an HTTP response.

*   **Ngrok**
    *   **What it is & What it provides:** A cross-platform application that creates a secure tunnel from the public internet directly to your local laptop. **It is important to note that Ngrok does NOT host your code.** It merely provides a public, temporary URL (e.g., `https://abc.ngrok.io`) that forwards internet traffic to a server you are already running locally on your machine.
    *   **How it connects in the flow:** Our Machine Learning model is heavy, so instead of paying expensive cloud costs to host it on AWS, **we physically run the Python ML script locally on our own laptop.** We then use Ngrok to generate a public URL that points to our laptop. When a child finishes a game, the Node backend uses Axios to ping this Ngrok URL over the internet. The internet traffic travels through the Ngrok tunnel, hits the ML script running on our laptop, calculates the new IQ score, and sends a success response back through the tunnel.

### 5.4 Security Concepts

*   **bcrypt**
    *   **What it is:** A widely-used password-hashing function designed to be computationally slow, protecting against brute-force attacks.
    *   **How it works:** It takes a plain-text password and mathematically transforms it into an irreversible string of characters (a hash). It also adds a unique "salt" to every password so that two users with the same password have completely different hashes.
    *   **Project Example:** If a parent creates the password `apple123`, `bcrypt` hashes it to `$2b$10$wI8...`. If the database is compromised, hackers cannot reverse the hash to see the original password.

*   **JWT (JSON Web Token)**
    *   **What it is:** An open standard for securely transmitting information between parties as a JSON object.
    *   **How it works:** After a user logs in successfully, the server creates a cryptographically signed token and sends it to the browser. The browser stores it (usually in an `HttpOnly` cookie) and automatically sends it with every future request. 
    *   **Project Example:** Because the token is signed with a secret key on the server, the Express backend can instantly verify the user's identity on every request without having to constantly query the database for session data (this is known as stateless authentication).

### 5.5 Ecosystem File Architecture & Purpose

*This is the architectural map of the backend ecosystem.*

*   **`Server.js` (The Main Hub):** Initializes Express, mounts everyday routes (Calendar, Tasks), configures CORS, and initializes the Socket.IO server for real-time chat.
*   **`LoginPage.js` (Auth Gateway):** Specifically handles user logins, One-Time Passwords (OTPs) via Brevo, and mounts all the micro-routers related to fetching user profiles.
*   **`modelserver.js` (Game Tracker):** A specialized Node server dedicated entirely to tracking live game scores and isolating game logic from standard chat/task operations.
*   **`pyserver.py` (Analytics Engine):** The Python microservice. It receives JSON data from Node, processes it using Pandas, draws graphs using Matplotlib, and returns the image back to the user.
*   **`fetchUserDetails.js` & `getuserdataemail.js`:** Micro-controllers designed specifically to query MongoDB and return full user profiles based on cookie data or email lookups.
*   **`getUserIQScore.js` (Performance Optimization):** A hyper-focused controller. Instead of downloading a massive user profile just to start a game, this route *only* queries and returns the `IQScore` integer, drastically reducing network payload.
*   **`updateGameUserInsertionDataset.js`:** The controller responsible for receiving finished game metrics (accuracy, time) and saving them into the `dataset` array inside the user's MongoDB document.
*   **`updateGameUserDeletionDataset.js` (Memory Manager):** Prevents MongoDB document bloat. If a user plays too many games, this script uses `.shift()` to delete the oldest game record before adding the new one, keeping database queries fast.
*   **`dataset.json` & `questions.json`:** The static data files. They act as the master question banks, containing all the educational content strictly categorized by IQ range and difficulty levels.

### 5.6 Key Workflows

*   **Socket.IO Real-Time Chat Workflow:** 
    1. **Connection:** A user's browser opens a WebSocket tunnel to `Server.js`. 
    2. **Joining:** The client emits a `join-room` event, telling the server to place them in a specific virtual Study Room. 
    3. **Sending:** When the user types a message, the client emits a `send-message` event containing the text. 
    4. **Broadcasting:** The server receives it and executes `io.to(roomId).emit("receive-message")`. This instantly blasts the message down the WebSocket tunnel *only* to other users currently inside that specific room, without them needing to refresh the page.

*   **Machine Learning (ML) Interaction Flow:** 
    1. **The Trigger:** A child finishes a game on the React frontend. React calculates their Accuracy and Time metrics.
    2. **The Bridge:** React sends an HTTP request to the `callmlmodel.js` endpoint on the Node backend. 
    3. **The Ping:** The Node server uses Axios to send a request to the external Ngrok URL hosting the Python ML model. 
    4. **Analysis & Update:** The Python ML model analyzes the metrics, calculates a new dynamic cognitive score, and updates the child's record in MongoDB. 
    5. **Adaptation:** The next time the child plays, React hits `giverandquesgame.js`, which reads the new score from the database and automatically fetches appropriately harder or easier questions from the `dataset.json` question bank.
