This system integrates IoT, cloud storage, machine learning, and a responsive web interface to create a real-time smart agricultural solution.

🧩 System Workflow

1. The ESP32 microcontroller collects real-time data from:
 - Soil Moisture Sensor (moisture level)
 - TCS3200 Color Sensor (RGB → converted to NPK values using normalized RGB formula)

2. NPK conversion formula:
 \`\`\`math
 Rnorm = R / (R + G + B)
 Gnorm = G / (R + G + B)
 Bnorm = B / (R + G + B)

 N (%) = (Rnorm × 100) – 10
 P (%) = (Gnorm × 100) – 5
 K (%) = (Bnorm × 100) – 8
 \`\`\`

3. Data is pushed to Firebase Realtime Database using HTTP and MQTT.

4. Live crop images are captured using an ESP32-CAM when the user clicks the capture button on the dashboard and are uploaded to Firebase for monitoring.

5. The L298N motor driver controls the irrigation pump:
 - In Manual Mode: The pump can be toggled ON/OFF from the dashboard.
 - In Auto Mode: The system schedules irrigation using crop-specific moisture values and real-time weather forecasts from OpenWeatherMap API to avoid unnecessary watering.

6. All data is visualized and controlled through a responsive HTML/CSS/JS dashboard.

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


