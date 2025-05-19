This system integrates IoT, cloud storage, machine learning, and a responsive web interface to create a real-time smart agricultural solution.

## 🧩 System Workflow

* The ESP32 microcontroller collects real-time data from connected sensors, primarily for soil moisture monitoring.

* Sensor data is transmitted to Firebase Realtime Database using HTTP and MQTT protocols for cloud storage and real-time access.

* The ESP32-CAM captures live crop field images upon request. When the user clicks the "Capture" button on the dashboard, the image is uploaded to Firebase and displayed for remote inspection.

* The irrigation system uses an L298N motor driver controlled by an ESP32:

  * In Manual Mode: Users can directly control the pump from the web dashboard.
  * In Auto Mode: The system automatically manages irrigation based on crop-specific moisture thresholds and weather forecasts from the OpenWeatherMap API, skipping watering during expected rainfall.

* The system integrates a machine learning-based crop recommendation engine:

  * The frontend collects NPK, temperature, humidity, and season data.
  * These inputs are sent to a Flask API that hosts a Random Forest model.
  * The model returns the top 5 suitable crops with confidence scores, which are visualized as a bar chart on the dashboard.

* All monitoring, prediction, and control functionalities are presented through a user-friendly, responsive HTML/CSS/JavaScript web dashboard.

* The architecture is modular and scalable for future enhancements like AI-based disease detection, historical yield analysis, and multi-farm support.

---


### 🤖 Machine Learning Crop Recommendation

An ML engine built using RandomForestClassifier helps suggest suitable crops based on soil and climate conditions.

* Developed in Google Colab
* Hosted via a Flask REST API
* Deployed model is serialized using Joblib (.pkl)

🧠 Input Features:

* Nitrogen (N), Phosphorus (P), Potassium (K)
* Temperature (°C)
* Humidity (%)
* Season (Summer, Monsoon, Winter → encoded)

🪴 Output:

* Top 5 crops with confidence scores
* Displayed as a list and a probability bar chart on the website

🔧 ML Stack:

| Tool         | Purpose                        |
| ------------ | ------------------------------ |
| Flask        | Backend API to serve model     |
| Flask-CORS   | Enable frontend-backend comms  |
| Pandas       | Data preprocessing             |
| Scikit-learn | Model training (Random Forest) |
| Joblib       | Model serialization (.pkl)     |
| Google Colab | Cloud training environment     |

🧪 Dataset Processing:

* Missing values and outliers handled statistically
* Categorical variables (e.g., crop, season) encoded
* Scaled and normalized for consistency
* 80/20 train-test split

🌲 Random Forest Config:

* n\_estimators: 100
* max\_depth: None
* criterion: gini
* random\_state: 42

🎯 Model Performance:

* Handles nonlinear, real-world agricultural data
* Robust against noise, missing inputs, and imbalance
* Feature importance helps track critical soil and weather features

---

![Home Page]([https://user-images.githubusercontent.com/12345678/your-image.png](https://github.com/M-LOGESH/FarmIQ/issues/1#issue-3072931229))



