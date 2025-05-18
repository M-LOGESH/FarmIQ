from flask import Flask, request, jsonify
import joblib
import pandas as pd
from flask_cors import CORS


# Create Flask application
app = Flask(__name__)
CORS(app)  # This will allow CORS for all routes

# Load the trained machine learning model
model = joblib.load("C:/FarmIQ/ML_model/crop_prediction_model.pkl")


# Load the label encoder (used to decode the predicted labels)
label_encoder = joblib.load("C:/FarmIQ/ML_model/label_encoder.pkl")

@app.route('/predict', methods=['POST'])
def predict():
    data = request.get_json()

    try:
        # Convert and validate all inputs
        N = float(data['N'])
        P = float(data['P'])
        K = float(data['K'])
        temperature = float(data['temperature'])
        humidity = float(data['humidity'])
        season_encoded = int(data['season_encoded'])
    except (ValueError, TypeError, KeyError):
        return jsonify({'error': 'Invalid or missing input values'}), 400

    input_data = pd.DataFrame([[
        N, P, K, temperature, humidity, season_encoded
    ]], columns=['N', 'P', 'K', 'temperature', 'humidity', 'season_encoded'])

    # Get prediction probabilities
    probabilities = model.predict_proba(input_data)[0]

    # Get indices of top 3 predictions
    top_indices = probabilities.argsort()[-5:][::-1]

    # Decode top predicted crops
    top_crops = label_encoder.inverse_transform(top_indices)

    # Prepare response with probabilities
    result = [{'crop': crop, 'probability': round(probabilities[idx]*100, 2)} for crop, idx in zip(top_crops, top_indices)]

    return jsonify({'predicted_crops': result})


if __name__ == '__main__':
    app.run(debug=True)
