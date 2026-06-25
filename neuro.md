# 🧠 NeuroProject: Comprehensive Project Documentation

Welcome to the complete project documentation for NeuroProject. This document outlines the architecture, tech stack, workflows, and engineering decisions behind the platform.

## 📑 Table of Contents
- [1. Project Overview & Problem Solved](#1-project-overview--problem-solved)
- [2. Tech Stack & Architectural Decisions](#2-tech-stack--architectural-decisions)
- [3. System Architecture & Ecosystem](#3-system-architecture--ecosystem)
- [4. Core Workflows](#4-core-workflows)
- [5. Machine Learning Integration & Flow](#5-machine-learning-integration--flow)
- [6. Authentication & OTP Flows](#6-authentication--otp-flows)
- [7. Development Challenges & Engineering Solutions](#7-development-challenges--engineering-solutions)
- [8. Common Interview / Cross Questions](#8-common-interview--cross-questions)

---

## 1. Project Overview & Problem Solved

**Project Overview:** 
NeuroProject is a comprehensive, full-stack web application designed to create an interactive and adaptive learning environment for children, accompanied by a Parent/Child monitoring dashboard. The platform features real-time study rooms, an AI-powered chatbot, and interactive educational games.

**The Target Audience & The Problem it Solves:** 
Traditional educational tools often use a "one-size-fits-all" approach, which is particularly ineffective for children with special educational needs. **NeuroProject is specially designed for children with Down Syndrome (Trisomy 21)**, focusing on those experiencing mild to moderate cognitive and developmental delays.

Children with Down Syndrome often benefit from highly visual, repetitive, and tailored learning experiences. NeuroProject solves this by providing **adaptive learning**. It monitors the child's game performance (accuracy, time taken, consistency) and sends this data to a Machine Learning model to calculate a dynamic cognitive/IQ score. Based on this score, the system automatically adapts the difficulty of future questions. It also provides parents with real-time analytics and visual graphs to track their child's developmental progress over time.

---

## 2. Tech Stack & Architectural Decisions

### ⚛️ Frontend: React.js (v19) & Tailwind CSS
*   **Why React.js instead of Vanilla JavaScript?**
    Vanilla JavaScript often requires manual and computationally expensive DOM manipulation, which can lead to slow performance in complex applications. React solves this by using a Virtual DOM—a lightweight in-memory representation of the actual web page. Instead of reloading the whole page, React intelligently calculates and updates only the specific components that changed. Furthermore, its component-based architecture allows us to write UI elements (like the Navbar or Study Rooms) once and reuse them across the application, significantly accelerating development and ensuring a seamless, native-app-like experience.

*   **Why Tailwind CSS instead of Bootstrap or Custom CSS?**
    Traditional CSS often results in massive, hard-to-maintain stylesheets with naming collisions. While Bootstrap solves this, it forces a rigid, templated look. Tailwind CSS is a utility-first framework that provides low-level styling classes directly within the React components. It grants total design freedom without context-switching between files, and its compiler automatically strips out unused CSS, guaranteeing a highly optimized final bundle.

### 🟢 Backend: Node.js & Express.js
*   **Why Node.js over Django, Spring, or Ruby on Rails?**
    By using Node.js, we maintain a unified JavaScript ecosystem across the entire stack, eliminating the mental overhead of context-switching between languages. More importantly, Node.js operates on a non-blocking, event-driven architecture. This single-threaded Event Loop makes it exceptionally efficient at handling thousands of concurrent I/O operations (like fetching data or routing), making it the optimal choice to power our Socket.IO real-time chat rooms with minimal latency.

### 🐍 Data Microservice: Python & Flask
*   **Why introduce Python if we already have Node.js?**
    While Node.js excels at asynchronous I/O, its single-threaded nature means it struggles with heavy CPU-bound tasks. Processing large datasets and rendering complex visual progress graphs would block the Node event loop, freezing the entire application and dropping live chat connections. We engineered a microservices approach by offloading the heavy mathematics to a lightweight Python Flask server. Python, being the industry standard for data science, utilizes `pandas` to process data instantly and `matplotlib` to render graphs, returning the finalized image to Node without disrupting core server operations.

### 🍃 Database: MongoDB & Mongoose
*   **Why a NoSQL database (MongoDB) over SQL (PostgreSQL/MySQL)?**
    Educational and user interaction data in our system is highly dynamic and unstructured. MongoDB’s JSON-like document structure perfectly accommodates this flexibility. Crucially, relational SQL databases require complex and computationally expensive `JOIN` queries to map a Parent to multiple Children. In MongoDB, we natively store an array of Child Object IDs directly inside the Parent document. Using Mongoose’s `.populate('children')` method, we execute what acts as an automated, highly efficient join, fetching complete child profiles in a single, clean command.

### 💬 Real-Time Communication: Socket.IO
*   **Why Socket.IO over standard HTTP?**
    Standard HTTP is a stateless, one-way protocol. To build a live chat, the React client would have to continuously "poll" the server every second to ask for new messages, creating massive network congestion. Socket.IO establishes a persistent, bi-directional WebSocket tunnel between the client and server. This empowers the server to instantly "push" messages directly to the users in specific virtual study rooms, resulting in true real-time communication with zero polling overhead.

---

## 3. System Architecture & Ecosystem

The application follows a **Microservices-inspired Architecture** to ensure separation of concerns and scalability.

1.  **The Client Layer:** The React Frontend (`localhost:3000`) acts as the user interface, sending requests via `axios` or `fetch`.
2.  **The Gateway/Router Layer:** Express Router acts as a traffic cop, directing requests to specific controllers (the "brains") based on the URL.
3.  **The Microservices Layer:** The backend is split into multiple specialized servers and micro-routers.
4.  **The Database Layer:** MongoDB stores user profiles, game datasets, and questions.

### ⚙️ Core Servers
*   **`Server.js` (Port 3016):** Main application server for daily operations and Socket.IO real-time chat rooms.
*   **`LoginPage.js` (Port 8080):** Authentication gateway handling login, OTPs, and user-data micro-routers.
*   **`modelserver.js`:** Specialized Node server dedicated to tracking live game scores and isolating game logic.
*   **`pyserver.py`:** Python microservice processing JSON data with Pandas, drawing graphs with Matplotlib, and returning images.

### 🔌 Micro-Routers
*   **`fetchUserDetails.js` / `getuserdataemail.js`:** Query MongoDB to return full user profiles based on cookies or email lookups.
*   **`getUserIQScore.js`:** Optimization controller that fetches *only* the `IQScore` to start a game, reducing network payload.
*   **`updateGameUserInsertionDataset.js`:** Receives finished game metrics and saves them into the user's `dataset` array.
*   **`updateGameUserDeletionDataset.js` (Memory Manager):** Prevents MongoDB document bloat by shifting old records when a user plays too many games.

---

## 4. Core Workflows

### 🌐 Tracing a Network Request
*Example: Fetching User Data when the app loads*
1.  **Trigger:** React's `useEffect` hook fires instantly when a page loads.
2.  **Axios:** Executes `axios.get("http://localhost:8080/api/get-user")`.
3.  **Router:** `LoginPage.js` intercepts the request and explicitly forwards it to the `fetchUserDetails.js` micro-router.
4.  **Database Lookup:** The controller uses the session cookie and runs `await User.findOne({ email })`.
5.  **Response:** The backend packages the user profile into JSON and sends it back to update the React UI.

### 💬 Socket.IO Real-Time Chat Workflow
1.  **Connection:** User's browser opens a WebSocket tunnel to `Server.js`.
2.  **Joining:** Client emits `join-room`, prompting the server to place them in a specific virtual Study Room.
3.  **Sending:** User types a message; client emits a `send-message` event.
4.  **Broadcasting:** Server executes `io.to(roomId).emit("receive-message")`, blasting the message only to users in that specific room without any page refresh.

---

## 5. Machine Learning Integration & Flow

NeuroProject leverages AI and ML in two distinct ways:

### 🤖 AI Chatbot
*   **Model Used:** `@google/generative-ai` (Gemini API)
*   **Purpose:** Powers the interactive AI Chatbot capabilities natively within the platform to assist students and parents.

### 🧠 Adaptive Game Play (Dynamic IQ Score)
*   **Algorithm & Logic:** 
    *   **Consistency Scoring:** The system calculates the Standard Deviation of a child's recent game accuracy and completion times. It uses this variance to calculate a normalized "Consistency Score" (`1 - ((sigmaAccuracy + sigmaTime) / 2)`).
    *   **Predictive ML Model:** A custom Python Machine Learning model processes the accuracy, time taken, and consistency score to predict and assign a dynamic IQ score.
*   **Where it is Hosted:** 
    *   The heavy Machine Learning script is run on an external Python environment and is securely exposed to the internet using an **Ngrok Tunnel**. The Node.js backend connects to it via a designated public Ngrok URL.
*   **The Exact ML Flow:**
    1. **Trigger & Storage:** A child finishes a game. React calculates metrics (Accuracy, Time). The Node backend saves these metrics to the user's MongoDB `dataset` array via `updateGameUserInsertionDataset.js`.
    2. **The Ping:** React triggers the `callmlmodel.js` endpoint. Node.js uses `axios.post()` to ping the external Ngrok URL, passing the `userId` in the body.
    3. **Analysis:** The Python ML model fetches the user's dataset, runs the predictive algorithm, calculates a new IQ Score, and updates the MongoDB database directly.
    4. **Adaptation Engine:** When the child plays again, the `giverandquesgame.js` router reads the newly assigned IQ Score. It scans `dataset.json` to find the exact matching IQ range bracket (e.g., "IQ 50-70"). It then shuffles the question bank for that specific level and serves 10 perfectly tailored questions to the frontend.

---

## 6. Authentication & OTP Flows

NeuroProject implements a custom authentication and verification flow using MongoDB and the Brevo email API.

### 📧 The OTP Verification Flow
1. **Request:** User enters their email to sign up. React triggers the `/send-parent-otp` route on `LoginPage.js`.
2. **Generation:** Node.js generates a secure 6-digit random code using `Math.floor(100000 + Math.random() * 900000)`.
3. **Database Storage:** Any existing OTP for that email is wiped. The new OTP is saved to the MongoDB `OtpModel` with a strict **10-minute expiration timer**.
4. **Email Delivery:** The backend uses the **Brevo (Sendinblue) Transactional Emails API** to dispatch the HTML email containing the OTP to the user.
5. **Verification:** The user submits the OTP. The `/verify-parent-otp` route checks MongoDB. If valid and unexpired, it approves the flow and deletes the OTP record.

### 🔐 The Authentication & Child Assignment Flow
1. **Parent Signup:** After OTP verification, a Parent document is created in MongoDB with `role: parent`. A session cookie (`res.cookie("user", email)`) is issued to the browser.
2. **Login:** A user submits their credentials. Node.js verifies the password. If successful, it uses Mongoose's `.populate("children")` to instantly fetch the dashboard data for all associated children in a single query. A session cookie is issued.
3. **Child Creation:** Only authenticated Parents can create children. When created, the backend sets `role: child` and explicitly links them by pushing the child's `_id` into the parent's `children` array, establishing a robust NoSQL relationship.
4. **Logout:** The `/logout` route clears the user session cookie and sets `isLogin: false` in the database.

---

## 7. Development Challenges & Engineering Solutions

### 📉 Challenge 1: Handling Heavy Math and Graphing
*   **Problem:** Node.js is single-threaded. Generating complex progress graphs for the parent dashboard blocked the Node event loop, freezing the server and real-time chat rooms.
*   **Solution:** Separated concerns using a Python Flask microservice. Node forwards raw JSON data to Python, which uses `pandas` and `matplotlib` to calculate trends and draw graphs. The image is saved in RAM and returned to React, entirely removing CPU load from Node.

### 🗄️ Challenge 2: Preventing Document Bloat in MongoDB
*   **Problem:** Pushing every game result into a user's MongoDB array caused document bloat and slowed down queries. The ML model only needed recent performance data.
*   **Solution:** Engineered a "Memory Manager" route (`updateGameUserDeletionDataset.js`) using a Sliding Window / FIFO queue pattern. It enforces a strict limit (e.g., keeping only the 8 most recent games) by using `array.shift()` to delete the oldest record before saving a new one, keeping database queries blazingly fast.

### 📡 Challenge 3: Real-time Communication Overhead
*   **Problem:** Early versions of the Study Rooms relied on HTTP polling, where the React client continually asked the server for new messages, creating massive network traffic.
*   **Solution:** Integrated `Socket.IO` to establish persistent WebSocket connections and built-in "Rooms". The server now instantly emits messages directly to clients, eliminating polling overhead.

### 👨‍👩‍👧‍👦 Challenge 4: Managing Complex Database Relationships
*   **Problem:** Linking one parent to multiple children in SQL requires complex, slow `JOIN` queries.
*   **Solution:** Chosen MongoDB handles arrays natively. Stored an array of Child Object IDs inside the Parent document. Using Mongoose's `.populate('children')` acts as an automated join, efficiently fetching the full profiles of all associated children in a single command.

---

## 8. Common Interview / Cross Questions

**Q: Why didn't you just use standard WebSockets instead of Socket.IO?**  
**A:** While raw WebSockets provide the same persistent connection, they require significant manual configuration to handle connection drops, fallbacks, and broadcasting. Socket.IO wraps WebSockets in a much simpler API and provides built-in fallback mechanisms (like long-polling if WebSockets fail). Crucially, Socket.IO provides a built-in "Rooms" feature, which made it incredibly easy to isolate chat messages to specific virtual Study Rooms without broadcasting to the entire server.

**Q: You mentioned your system calculates a consistency score. How exactly does that work?**  
**A:** The consistency score is determined by calculating the Standard Deviation of a child's recent accuracy and completion times. If a child answers quickly and correctly one day, but slowly and erratically the next, the standard deviation increases. The system normalizes this variance to yield a Consistency Score, which the predictive ML model uses to ensure the child isn't just randomly guessing or distracted.

**Q: Haven't you implemented secure password hashing (bcrypt) and JWT yet?**  
**A:** Currently, the codebase is in a rapid-prototyping phase. To quickly test the complex Parent/Child dashboard interactions and routing, I temporarily saved passwords as plain text and used raw email addresses for cookie sessions. However, I have recognized this as critical technical debt and already laid the groundwork for production. The immediate next task before deployment is to implement `bcrypt.hash()` for password security and generate cryptographically signed JSON Web Tokens (JWT) for the cookies to prevent tampering and XSS attacks.

**Q: What happens if your dataset in MongoDB grows too large?**  
**A:** I encountered this exact issue: pushing every single game result into a user's MongoDB array caused document bloat and slowed down queries. To solve it, I engineered a "Memory Manager" controller using a Sliding Window (FIFO queue) pattern. When a user completes a game, the server enforces a strict limit (e.g., the 8 most recent games). If the limit is reached, it executes an `array.shift()` to delete the oldest record before saving the new one, guaranteeing database queries remain blazingly fast.

**Q: How does the AI Chatbot differ from the ML Adaptive Game Model?**  
**A:** They serve completely different purposes and architectures. The AI Chatbot is an integration of the external `@google/generative-ai` (Gemini) API, used primarily for natural language processing and assisting parents/students with text-based queries natively in the app. The Adaptive Game Model is a custom-built predictive Python algorithm hosted securely via Ngrok that specifically crunches numerical game data (accuracy, time, variance) to dynamically calculate an IQ score and tailor educational content.
