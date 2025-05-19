# Cloud Integrated IoT System for Soil Monitoring and Crop Management

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

### Homepage
A central dashboard displaying real-time soil moisture, nutrient levels (NPK), crop irrigation status, and live crop camera feed. Designed for intuitive navigation and rapid smart farm monitoring.

<p align="center">
  <img src="https://github.com/user-attachments/assets/3aef7c0d-9283-4880-928b-427203209d25" alt="Image" width="650">
</p>


### Crop Monitoring
Live Crop Monitoring provides real-time visual surveillance of the field using ESP32-CAM, enabling early detection of issues and better crop management.

<p align="center">
  <img src="https://github.com/user-attachments/assets/9106e771-117f-4596-81de-9c385a04d0b3" alt="Image" width="700">
</p>


### Analytical Dashboard
Combines live NPK sensor readings and weather data with user inputs to predict suitable crops. Features real-time visualizations, crop recommendations, and a prediction probability chart.

<p align="center">
  <img src="https://github.com/user-attachments/assets/2c4ac729-e30d-488e-b4e8-10aca7e8cdba" alt="Image" width="700">
</p>

Allows manual entry of NPK, temperature, humidity, and season to get AI-based crop recommendations. Uses a Flask-hosted machine learning model to display the top crops with probabilities.


### Irrigation Control Dashboard
Displays soil moisture level, pump status, and date/time while allowing manual or automatic irrigation control. Integrates weather forecasts to make intelligent irrigation decisions.

<p align="center">
  <img src="https://github.com/user-attachments/assets/872db852-2147-4e9a-a8cd-c7fca63be1b4" alt="Image" width="700">
</p>


### Crop Irrigation Customization
Lets users define crop-specific irrigation schedules including moisture thresholds and time slots. Crop data is stored and updated dynamically using Firestore for persistent and synced control.

<p align="center">
  <img src="https://github.com/user-attachments/assets/1d761029-9161-4e78-9f52-03dae50e4d25" alt="Image" width="700">
</p>



This project successfully integrates IoT, cloud services, and machine learning to enable smart, data-driven agriculture. It provides real-time monitoring, intelligent crop recommendations, and automated irrigation control through a user-friendly dashboard. The system enhances productivity, conserves resources, and empowers farmers with modern precision farming tools.

---









