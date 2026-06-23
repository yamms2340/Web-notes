# 🌾 AgriSense: Comprehensive Interview Script & Deep Dive

## 📑 Quick Links
- [1. Project Overview & Problem Statement](#1-project-overview--problem-statement)
- [2. System Architecture & Definitions](#2-system-architecture--definitions)
- [3. Feature Workflows (Data Flow)](#3-feature-workflows-data-flow)
- [4. Technology Stack Deep Dive](#4-technology-stack-deep-dive)
- [5. Authentication & Security](#5-authentication--security)
- [6. Development Challenges & Engineering Decisions](#6-development-challenges--engineering-decisions)

---

## 1. Project Overview & Problem Statement

### What is AgriSense?
AgriSense is an AI-powered agricultural decision support platform designed to help farmers make data-driven decisions throughout the farming lifecycle. It unifies Soil Intelligence, Crop Recommendation, Weather Intelligence, Risk Assessment, Market Intelligence, and Expense Management into a unified system.

Rather than relying solely on traditional farming practices or delayed government advisories, AgriSense leverages machine learning models, environmental data, and market analytics to provide actionable recommendations that improve productivity and profitability.

### Problem Statement
Agriculture is highly dependent on several unpredictable variables: soil quality and composition, weather conditions, crop diseases and risks, market price fluctuations, and expense management.

Farmers often lack access to real-time information required to make informed decisions regarding crop selection, selling strategy, and financial planning. AgriSense addresses this information gap by aggregating agricultural intelligence from multiple sources and transforming it into actionable insights.

---

## 2. System Architecture & Definitions

### Architecture Diagram
AgriSense follows a Service-Oriented Architecture (SOA).
```text
React Frontend (UI Layer)
      ↓
Express Backend (API Gateway & Business Logic)
      ↓
      ├──→ MongoDB (Data Persistence)
      ├──→ External APIs (Weather, GPS, Email)
      └──→ FastAPI ML Service (Inference Layer)
```

### Key Definitions
* **SOA (Service-Oriented Architecture):** Separates the presentation layer, business layer, data layer, and ML layer into independent services, making the system easier to scale and maintain.
* **API Gateway Pattern:** Express acts as the single entry point handling authentication, validation, routing, and orchestration.
* **Microservice Architecture:** FastAPI runs as an independent ML microservice allowing separate deployment, fault isolation, independent scaling, better maintainability, and technology specialization.
* **ODM (Object Document Mapper):** Mongoose acts as the ODM, providing schema definitions, validation, and middleware for MongoDB.

### What is hosted inside FastAPI?
FastAPI serves as a dedicated ML Inference Service. Machine Learning inference is CPU-intensive. Running PyTorch and Scikit-Learn models directly inside Node.js would block the event loop and degrade performance.
It hosts:
* **Soil Classification Model** (`POST /predict/soil`)
* **Crop Recommendation Model** (`POST /predict/crop`)
* **Price Prediction Model** (`POST /predict/price`)

Models are loaded from `ml-service/models/` during application startup and kept in memory for fast inference.

---

## 3. Feature Workflows (Data Flow)

### Feature 1: Soil Intelligence
Determine soil type and soil characteristics.
```text
User Uploads Soil Image
        ↓
React Frontend
        ↓
POST /api/ml/predict/soil
        ↓
Express Backend
        ↓
FastAPI ML Service
        ↓
EfficientNet-B0
        ↓
Predicted Soil Type (e.g., {"soil_type": "Black Soil", "confidence": 0.93})
        ↓
Reference API Lookup (GET /api/reference/soil-database)
        ↓
Soil Metadata (pH, Water Retention, Texture, Organic Matter, Suitable Crops)
        ↓
Frontend Display
```
*(Note: If image upload is unavailable, the system uses SoilGrids API for GPS-based soil intelligence.)*

### Feature 2: Crop Recommendation
Recommend the most suitable crops based on environmental conditions.
```text
Soil Type, Temp, Humidity, Rainfall
        ↓
React
        ↓
POST /api/ml/predict/crop
        ↓
Express
        ↓
FastAPI
        ↓
Random Forest Model
        ↓
Top Crop Predictions
        ↓
Crop Metadata Lookup (GET /api/reference/crops)
        ↓
Final Recommendation (Crop Duration, Water Requirements, Yield, Profitability)
```

### Feature 3: Weather Intelligence
Provide environmental awareness and forecasting.
```text
Location
        ↓
Open-Meteo API
        ↓
Current Weather & Forecast Data
        ↓
Dashboard
```
*(Note: The Dashboard directly calls Open-Meteo. No backend layer is required. Weather data includes Temperature, Rainfall, Humidity, Wind Speed.)*

### Feature 4: Risk Assessment
Identify agricultural risks based on weather conditions.
```text
Weather Forecast
        ↓
POST /api/reference/compute-risks
        ↓
Express Risk Engine (Centralized Rule Engine)
        ↓
Risk Detection (e.g., High Rainfall → Flood Risk, High Temp → Heat Stress)
        ↓
POST /api/reference/safe-crops
        ↓
Safe Crop Recommendations
```

### Feature 5: Market Intelligence

**A. Live Price Tracking**
Provide real-time commodity prices.
```text
Commodity → Market API (data.gov.in) → Price Data
(If API Fails) → Cheerio Scraper (Alternative Website) → Normalized Price Response
```

**B. Best Mandi Finder Flow**
Identify the most profitable mandi.
```text
Crop, Quantity, Location → Nominatim Geocoding → Nearby Mandis & Prices → Revenue Calculation → Ranking → Best Mandi
```

**C. Price Forecasting**
Predict future commodity prices.
```text
Historical Prices → FastAPI (POST /api/ml/predict/price) → Forecast Model → Future Prices
```

### Feature 6: Expense Tracker
Digitize farm financial records.
```text
Expense Entry (Seeds, Fertilizers, Labor, Transport)
        ↓
Express API
        ↓
MongoDB (ExpenseRecord collection)
        ↓
Expense Dashboard
```

---

## 4. Technology Stack Deep Dive

### Node.js
* **What is it?** Node.js is the runtime environment that executes backend JavaScript code outside the browser.
* **Why was it needed?** AgriSense performs a large number of I/O operations (fetching weather info, querying MongoDB, calling market APIs, communicating with FastAPI). These operations spend most of their time waiting for external resources. Node's event-driven, non-blocking I/O architecture allows a single thread to efficiently handle thousands of concurrent requests. The backend workload is predominantly I/O-bound rather than CPU-bound, making Node.js a natural fit.

### Express.js
* **What is it?** A structured web framework built on top of Node.js.
* **Why not plain Node?** Node.js provides only the runtime environment. Without Express, building and maintaining dozens of API endpoints would require manually handling routing, middleware, request parsing, response formatting, and error handling. Express acts as our API Gateway and orchestration layer.

### FastAPI
* **What is it?** A high-performance Python web framework for exposing machine learning models as REST APIs.
* **Why was it needed?** The ML ecosystem is heavily centered around Python (PyTorch, Scikit-Learn). Running CPU-intensive neural networks directly in Node.js would block the event loop and reduce API responsiveness. FastAPI acts as a dedicated ML inference service, providing independent scaling, fault isolation, better maintainability, and technology specialization.

### MongoDB
* **What is it?** A NoSQL database storing data as flexible BSON documents.
* **Why not MySQL?** Agricultural data does not naturally fit rigid relational schemas. For example, one expense record might be `{"category": "Seeds", "amount": 5000}` while another includes `{"category": "Seeds", "amount": 5000, "vendor": "ABC Agro", "quantity": 20}`. Relational databases often require schema modifications and migrations for such changes. MongoDB allows the schema to evolve naturally, eliminating slow `JOIN` queries.

### Mongoose
* **What is it?** ODM (Object Document Mapper) for MongoDB.
* **Why needed?** Writing database queries directly everywhere can become difficult to manage. Mongoose provides schema definitions, validation, middleware hooks, and model abstraction (e.g., `User`, `ExpenseRecord`, `InputInventory`), which improves maintainability and enforces data consistency.

### React.js & Vite
* **What are they?** React is a component-based UI library. Vite is a fast frontend build tool.
* **Why used?** React provides reusable components and the Virtual DOM for fast updates on dynamic dashboards. Vite provides instant Hot Module Replacement (HMR) and extremely fast builds.

### Axios
* **What is it?** Promise-based HTTP client.
* **Why used?** It acts as the communication layer between system components (React → Express, Express → FastAPI). It provides request/response interceptors, automatic JSON transformation, and better error handling than native `fetch`.

### PyTorch & EfficientNet-B0
* **What is it?** PyTorch is a deep learning framework. EfficientNet-B0 is a Convolutional Neural Network (CNN) designed for efficient image classification.
* **Why used?** The soil classification problem involves image recognition. Traditional ML algorithms cannot directly understand image patterns. EfficientNet-B0 automatically learns visual features (color distributions, texture patterns, surface characteristics) to map soil images to categories with high accuracy and fast inference speed.

### Scikit-Learn & Random Forest
* **What is it?** Machine learning library and ensemble algorithm built from multiple decision trees.
* **Why used?** Crop recommendation is fundamentally a tabular prediction problem (Soil Type, Temp, Humidity, Rainfall). Deep learning is often unnecessary for structured agricultural datasets. Random Forest combines predictions from multiple decision trees to improve accuracy, reduce overfitting, handle nonlinear relations, and remain interpretable and computationally efficient.

### External APIs & Scrapers
* **Open-Meteo:** Rather than maintaining proprietary weather infrastructure, the platform consumes high-quality meteorological data (temp, rainfall, humidity, wind) through Open-Meteo.
* **SoilGrids:** Soil analysis traditionally requires laboratory testing, which many farmers cannot access. SoilGrids provides scientifically estimated soil properties using geographic coordinates. This enables GPS-based soil intelligence even when image uploads are unavailable.
* **Nominatim:** Geocoding service used in the Best Mandi Finder (Location Name → Coordinates).
* **data.gov.in:** Government API providing live mandi and commodity prices.
* **Cheerio:** Server-side HTML parsing library used as a fallback scraper. When the primary data source becomes unavailable, Cheerio retrieves data from alternative market websites, improving system reliability.

---

## 5. Authentication & Security

### Registration Flow
```text
Registration
      ↓
OTP Generation (expires in 10 mins)
      ↓
Memory Store (Map + setTimeout)
      ↓
Brevo Email Validation
      ↓
MongoDB User Creation
```

### Login Flow
```text
Credentials
      ↓
bcryptjs Verification
      ↓
JWT Generation
      ↓
Authenticated Access
```

### Security Tooling
* **JWT (JSON Web Token):** Provides stateless authentication.
* **bcryptjs:** Hashes passwords before database storage.
* **express-rate-limit:** Rate limiting middleware that protects APIs against abuse and excessive requests.

---

## 6. Development Challenges & Engineering Decisions

### Challenge 1: The Single-Threaded Bottleneck
Node.js is incredibly fast for I/O, but its single-threaded nature means a heavy CPU-bound task like a PyTorch model would block the event loop and crash the server. **Solution:** Decoupled the ML logic entirely into a standalone Python FastAPI microservice. Express acts as an asynchronous proxy, offloading the work and keeping the main server blazingly fast.

### Challenge 2: Unreliable Third-Party APIs
The official government market API was prone to unexpected downtimes. **Solution:** Integrated **Cheerio** into Node.js to build a robust fallback web scraper. If the official API fails, the backend silently scrapes live HTML tables from an alternative site, ensuring high availability.

### Challenge 3: Managing OTPs Without Database Bloat
During registration, saving unverified OTPs to MongoDB opens the database up to bot spam and garbage documents. **Solution:** Since OTPs are transient, they are stored in an in-memory `Map` combined with `setTimeout`. They auto-delete after 10 minutes, and MongoDB is only written to *after* the user verifies.

### Challenge 4: Handling Unstructured User Data
A farmer's expense ledger varies wildly based on what they buy (seeds, labor, fuel). Using a relational SQL approach would require multiple tables and slow `JOIN` queries. **Solution:** Leveraged MongoDB and Mongoose to design an `ExpenseRecord` schema where the entire ledger is embedded inside a single document, accommodating dynamic data requirements and reducing API response times.
