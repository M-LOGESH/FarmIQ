This system integrates IoT, cloud storage, machine learning, and a responsive web interface to create a real-time smart agricultural solution.

🧩 System Workflow

1. The ESP32 microcontroller collects real-time data from connected sensors, including soil moisture readings.

2. Collected data is pushed to Firebase Realtime Database using HTTP and MQTT protocols for real-time storage and access.

3. The ESP32-CAM module captures live crop images when the user clicks the “Capture” button on the web dashboard. The image is uploaded to Firebase and displayed for remote field monitoring.

4. Irrigation is managed by an ESP32-controlled L298N motor driver that operates a water pump:
- Manual Mode: Users can directly switch the pump ON/OFF through the web dashboard.
- Auto Mode: The system uses crop-specific moisture thresholds and weather forecast data (via OpenWeatherMap API) to schedule irrigation. The pump turns off automatically when the optimal moisture level is reached or rain is expected.

5. All features are integrated into a responsive web dashboard (built with HTML, CSS, and JavaScript), which provides:
- Real-time sensor monitoring
- Motor control options
- Weather updates
- Crop prediction and insights


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


