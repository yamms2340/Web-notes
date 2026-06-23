# AgriSense

## Project Overview

### What is AgriSense?

AgriSense is an AI-powered agricultural decision support platform designed to help farmers make data-driven decisions throughout the farming lifecycle.

The platform combines:

* Soil Intelligence
* Crop Recommendation
* Weather Intelligence
* Risk Assessment
* Market Intelligence
* Expense Management

into a unified system.

Rather than relying solely on traditional farming practices or delayed government advisories, AgriSense leverages machine learning models, environmental data, and market analytics to provide actionable recommendations that improve productivity and profitability.

---

# Problem Statement

Agriculture is highly dependent on several unpredictable variables:

* Soil quality and composition
* Weather conditions
* Crop diseases and risks
* Market price fluctuations
* Expense management

Farmers often lack access to real-time information required to make informed decisions regarding crop selection, selling strategy, and financial planning.

AgriSense addresses this information gap by aggregating agricultural intelligence from multiple sources and transforming it into actionable insights.

---

# System Architecture

AgriSense follows a Service-Oriented Architecture (SOA).

```text
React Frontend

      ↓

Express Backend
(API Gateway)

      ↓

MongoDB

      ↓

FastAPI ML Service

      ↓

External APIs
```

---


### What is FastAPI?

FastAPI is a high-performance Python web framework used to expose machine learning models as REST APIs.

In AgriSense, FastAPI serves as a dedicated ML Inference Service.

---

### Why FastAPI?

Machine Learning inference is CPU-intensive.

Running PyTorch and Scikit-Learn models directly inside Node.js would block the event loop and degrade performance.

Therefore:

```text
Node.js
   ↓
Business Logic

FastAPI
   ↓
ML Inference
```

This separation provides:

* Independent scaling
* Fault isolation
* Better maintainability
* Technology specialization

---

### What is hosted inside FastAPI?

FastAPI hosts:

* Soil Classification Model
* Crop Recommendation Model
* Price Prediction Model

Endpoints:

```http
POST /predict/soil
POST /predict/crop
POST /predict/price
```

Models are loaded from:

```text
ml-service/models/
```

during application startup and kept in memory for inference.

---

# Technology Stack

**MERN Stack (MongoDB, Express.js, React.js, Node.js)**

- **React + Vite** – Frontend development, dashboards, forms, and user interactions.
- **Node.js + Express.js** – REST APIs, authentication, business logic, and external integrations.
- **MongoDB** – Storage of user data, expense records, crop plans, and inventory information.

**Machine Learning Stack**

- **FastAPI** – Dedicated ML microservice for model serving and inference.
- **PyTorch (EfficientNet-B0)** – Soil image classification.
- **Scikit-Learn (Random Forest)** – Crop recommendation.
- **Price Forecasting Model** – Commodity price prediction.

**External Services**

- **Open-Meteo API** – Weather forecasting.
- **ISRIC SoilGrids API** – GPS-based soil analysis.
- **data.gov.in API** – Live mandi and commodity prices.
- **OpenStreetMap Nominatim** – Geocoding for Best Mandi recommendations.

**Security**

- **JWT** – Authentication and authorization.
- **bcryptjs** – Password hashing.
---
# Feature 1: Soil Intelligence

## Purpose

Determine soil type and soil characteristics.

---

## Flow

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

Predicted Soil Type

        ↓

Reference API Lookup

        ↓

Soil Metadata

        ↓

Frontend Display
```

---

## How It Is Implemented

The SoilAnalysis page collects a soil image from the user.

The image is sent to the Express backend using multipart form data.

Express forwards the image to FastAPI.

Inside FastAPI:

* EfficientNet-B0 extracts image features.
* The model classifies soil type.

Example:

```json
{
  "soil_type": "Black Soil",
  "confidence": 0.93
}
```

The frontend also retrieves soil metadata through:

```http
GET /api/reference/soil-database
```

This contains:

* pH
* Water Retention
* Soil Texture
* Organic Matter
* Suitable Crops

The prediction is enriched using this dataset and presented to the farmer.

---

## Technologies Used

* React
* Express
* FastAPI
* PyTorch
* EfficientNet-B0
* SoilGrids API

---

# Feature 2: Crop Recommendation

## Purpose

Recommend the most suitable crops based on environmental conditions.

---

## Flow

```text
Soil Type

Temperature

Humidity

Rainfall

      ↓

React

      ↓

Express

      ↓

FastAPI

      ↓

Random Forest Model

      ↓

Top Crop Predictions

      ↓

Crop Metadata

      ↓

Final Recommendation
```

---

## How It Is Implemented

The frontend collects:

* Soil Type
* Temperature
* Humidity
* Rainfall

These values are submitted to:

```http
POST /api/ml/predict/crop
```

Express forwards the request to FastAPI.

A Random Forest model evaluates the environmental conditions and generates suitability scores for crops.

The prediction results are merged with crop metadata fetched from:

```http
GET /api/reference/crops
```

which includes:

* Crop Duration
* Water Requirements
* Yield Information
* Profitability Data

The final ranked recommendations are displayed.

---

## Technologies Used

* React
* Express
* FastAPI
* Scikit-Learn
* Random Forest

---

# Feature 3: Weather Intelligence

## Purpose

Provide environmental awareness and forecasting.

---

## Flow

```text
Location

    ↓

Open-Meteo API

    ↓

Current Weather

    ↓

Forecast Data

    ↓

Dashboard
```

---

## How It Is Implemented

The Dashboard directly calls Open-Meteo.

No backend layer is required.

Weather data includes:

* Temperature
* Rainfall
* Humidity
* Wind Speed

The information is rendered on the dashboard and reused by the Risk Assessment module.

---

## Technologies Used

* React
* Open-Meteo API

---

# Feature 4: Risk Assessment

## Purpose

Identify agricultural risks based on weather conditions.

---

## Flow

```text
Weather Forecast

      ↓

Express Risk Engine

      ↓

Risk Detection

      ↓

Flood

Drought

Heat Stress

Frost

      ↓

Safe Crop Recommendations
```

---

## How It Is Implemented

The frontend retrieves weather data from Open-Meteo.

The forecast is submitted to:

```http
POST /api/reference/compute-risks
```

A centralized rule engine evaluates agronomic thresholds.

Examples:

```text
High Rainfall
    ↓
Flood Risk
```

```text
High Temperature
    ↓
Heat Stress
```

Identified risks are sent to:

```http
POST /api/reference/safe-crops
```

which returns resilient crops.

---

## Technologies Used

* React
* Express
* Open-Meteo API
* Rule-Based Expert System

---

# Feature 5: Market Intelligence

---

## A. Live Price Tracking

### Purpose

Provide real-time commodity prices.

### Flow

```text
Commodity

    ↓

Market API

    ↓

data.gov.in

    ↓

Price Data
```

Fallback:

```text
API Failure

    ↓

Cheerio Scraper

    ↓

Alternative Source

    ↓

Normalized Response
```

### Implementation

The Market Controller first queries data.gov.in.

If unavailable, a Cheerio-based scraper retrieves data from alternative market websites.

All responses are normalized into a common schema.

---

## B. Best Mandi Finder

### Purpose

Identify the most profitable mandi.

### Flow

```text
Crop

Quantity

Location

      ↓

Geocoding

      ↓

Nearby Mandis

      ↓

Price Retrieval

      ↓

Revenue Calculation

      ↓

Ranking

      ↓

Best Mandi
```

### Implementation

Nominatim converts locations into coordinates.

Nearby mandis and prices are retrieved.

Revenue is calculated for each mandi.

Mandis are ranked and returned.

---

## C. Price Forecasting

### Purpose

Predict future commodity prices.

### Flow

```text
Historical Prices

      ↓

FastAPI

      ↓

Forecast Model

      ↓

Future Prices
```

### Implementation

Historical price data is passed to:

```http
POST /api/ml/predict/price
```

FastAPI performs forecasting and returns future price estimates.

---

## Technologies Used

* React
* Express
* FastAPI
* Cheerio
* data.gov.in API
* Nominatim API

---

# Feature 6: Expense Tracker

## Purpose

Digitize farm financial records.

---

## Flow

```text
Expense Entry

      ↓

Express API

      ↓

MongoDB

      ↓

Expense Dashboard
```

---

## How It Is Implemented

Users record:

* Seeds
* Fertilizers
* Labor
* Transport

Expenses are stored inside MongoDB using the ExpenseRecord collection.

The backend provides CRUD APIs for creating and retrieving records.

Future crop planning data is also persisted.

---

## Technologies Used

* React
* Express
* MongoDB
* Mongoose

---

# Authentication & Security

## Registration Flow

```text
Registration

      ↓

OTP Generation

      ↓

Memory Store

      ↓

Brevo Email

      ↓

Verification

      ↓

MongoDB User Creation
```

---

## Login Flow

```text
Credentials

      ↓

bcrypt Verification

      ↓

JWT Generation

      ↓

Authenticated Access
```

---

## Security Features

### JWT

Provides stateless authentication.

### bcryptjs

Hashes passwords before storage.

### express-rate-limit

Protects APIs against abuse and excessive requests.

### OTP Store

Implemented using:

```javascript
Map + setTimeout
```

OTPs automatically expire after 10 minutes.

---

# Key Design Decisions

## Why FastAPI?

To isolate CPU-intensive ML workloads from Node.js.

---

## Why MongoDB?

To support flexible agricultural records without JOIN-heavy relational schemas.

---

## Why SOA?

To separate:

* Presentation Layer
* Business Layer
* Data Layer
* ML Layer

making the system easier to scale and maintain.

---
## Node.js

Node.js is the runtime environment that executes backend JavaScript code outside the browser.

AgriSense performs a large number of I/O operations such as:

* Fetching weather information
* Querying MongoDB
* Calling market APIs
* Communicating with the FastAPI ML service

These operations spend most of their time waiting for external resources rather than performing computation.

Node.js uses an Event-Driven, Non-Blocking I/O architecture that allows a single thread to efficiently handle thousands of concurrent requests while waiting for these operations to complete.

**Why Node.js?**

The backend workload is predominantly I/O-bound rather than CPU-bound, making Node.js a natural fit.

---

## Express.js

Node.js provides only the runtime environment.

If we used only Node.js, we would need to manually handle:

* Routing
* Middleware execution
* Request parsing
* Response formatting
* Error handling

Express sits on top of Node.js and provides a structured framework for building web appliitcations.

In AgriSense, Express acts as the API Gateway and orchestration layer.

It is responsible for:

* Exposing REST APIs
* Authentication middleware
* Rate limiting
* Request validation
* Business logic execution
* Communication with MongoDB
* Communication with FastAPI

Example:

```text
React

    ↓

Express

    ↓

MongoDB
FastAPI
External APIs
```

Without Express, building and maintaining dozens of API endpoints would become significantly more complex.

---

## FastAPI

The machine learning ecosystem is heavily centered around Python libraries such as:

* PyTorch
* Scikit-Learn
* Prophet

Although Node.js excels at I/O operations, it is not ideal for CPU-intensive inference workloads.

Running neural networks directly inside Node.js would block the event loop and reduce API responsiveness.

FastAPI solves this problem by exposing machine learning models as independent HTTP services.

In AgriSense:

```text
React

   ↓

Express

   ↓

FastAPI

   ↓

ML Models
```

FastAPI hosts:

* Soil Classification Model
* Crop Recommendation Model
* Price Forecasting Model

and exposes them through REST endpoints.

This architecture allows the ML layer and backend layer to scale independently.

---

## MongoDB

Agricultural data does not naturally fit into rigid relational schemas.

Consider expense records:

One farmer may record:

```json
{
  "category": "Seeds",
  "amount": 5000
}
```

while another may include:

```json
{
  "category": "Seeds",
  "amount": 5000,
  "vendor": "ABC Agro",
  "quantity": 20
}
```

Relational databases often require schema modifications and migrations for such changes.

MongoDB stores data as flexible BSON documents, allowing the schema to evolve naturally.

In AgriSense, MongoDB stores:

* Users
* Expense Records
* Crop Plans
* Inventory Data

This flexibility accelerated development and reduced schema maintenance overhead.

---

## Mongoose

MongoDB itself provides raw database operations.

Writing database queries directly everywhere can become difficult to manage as the project grows.

Mongoose introduces an Object Document Mapper (ODM) layer.

It provides:

* Schema definitions
* Validation
* Middleware hooks
* Model abstraction

Instead of interacting directly with collections, the application works with domain models such as:

```js
User
ExpenseRecord
InputInventory
```

This improves maintainability and enforces data consistency.

---

## Axios

AgriSense consists of multiple communicating services.

The frontend communicates with Express.

Express communicates with FastAPI.

A reliable HTTP client is therefore required.

Axios provides:

* Promise-based requests
* Request interceptors
* Response interceptors
* Automatic JSON transformation
* Better error handling

It acts as the communication layer between system components.

---

## Open-Meteo

The weather intelligence module depends on real-time environmental information.

Open-Meteo provides:

* Current temperature
* Rainfall
* Humidity
* Wind speed
* Multi-day forecasts

This data serves as the foundation for:

* Weather Dashboard
* Risk Assessment
* Crop Planning

Rather than maintaining proprietary weather infrastructure, the platform consumes high-quality meteorological data through Open-Meteo.

---

## SoilGrids

Soil analysis traditionally requires laboratory testing.

Many farmers do not have access to such facilities.

SoilGrids provides scientifically estimated soil properties using geographic coordinates.

It supplies:

* Soil pH
* Organic Carbon
* Nitrogen Content
* Soil Texture

This enables GPS-based soil intelligence even when image uploads are unavailable.

---

## Cheerio

The market intelligence module depends on external commodity pricing sources.

Government APIs occasionally experience downtime or rate limits.

Cheerio provides server-side HTML parsing capabilities.

When the primary data source becomes unavailable:

```text
Government API

      ↓

Failure

      ↓

Cheerio

      ↓

Alternative Website

      ↓

Price Extraction
```

This fallback mechanism improves system reliability and availability.

---

## Random Forest

Crop recommendation is fundamentally a tabular prediction problem.

Inputs:

* Soil Type
* Temperature
* Humidity
* Rainfall

Unlike image or NLP tasks, deep learning is often unnecessary for structured agricultural datasets.

Random Forest combines predictions from multiple decision trees to improve accuracy and reduce overfitting.

It was selected because it performs well on structured environmental data while remaining interpretable and computationally efficient.

---

## EfficientNet-B0

The soil classification problem involves image recognition.

Traditional machine learning algorithms cannot directly understand image patterns.

EfficientNet-B0 is a Convolutional Neural Network designed for efficient image classification.

It automatically learns visual features such as:

* Color distributions
* Texture patterns
* Surface characteristics

allowing soil images to be mapped to soil categories with high accuracy while maintaining fast inference speed.
