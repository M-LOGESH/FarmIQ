import gradio as gr
import joblib
import pandas as pd

# Load your model and label encoder
model = joblib.load("crop_prediction_model.pkl")
label_encoder = joblib.load("label_encoder.pkl")

# Prediction function
def predict_crop(N, P, K, temperature, humidity, season_encoded):
    input_data = pd.DataFrame([[N, P, K, temperature, humidity, season_encoded]],
                              columns=['N', 'P', 'K', 'temperature', 'humidity', 'season_encoded'])
    
    probabilities = model.predict_proba(input_data)[0]
    top_indices = probabilities.argsort()[-5:][::-1]
    top_crops = label_encoder.inverse_transform(top_indices)

    # Return dictionary of crop and probability
    return {top_crops[i]: round(probabilities[top_indices[i]] * 100, 2) for i in range(5)}

# Create Gradio interface
demo = gr.Interface(
    fn=predict_crop,
    inputs=[
        gr.Number(label="Nitrogen (N)"),
        gr.Number(label="Phosphorus (P)"),
        gr.Number(label="Potassium (K)"),
        gr.Number(label="Temperature (°C)"),
        gr.Number(label="Humidity (%)"),
        gr.Number(label="Season Encoded (0–3)")
    ],
    outputs=gr.Label(num_top_classes=5),
    title="Crop Prediction Model",
    description="Predict the top 5 most suitable crops based on soil nutrients and weather conditions."
)

demo.launch()
