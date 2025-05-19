 
import { initializeApp } from "https://www.gstatic.com/firebasejs/11.2.0/firebase-app.js";
import { getDatabase, ref, onValue, set, push, get } from "https://www.gstatic.com/firebasejs/11.2.0/firebase-database.js";
import { getFirestore, collection, getDocs } from "https://www.gstatic.com/firebasejs/11.2.0/firebase-firestore.js";
      
    const firebaseConfig = {
        apiKey: "API KEY",
        authDomain: "AuthDomain",
        databaseURL: "Firebase URL",
        projectId: "Project ID",
        storageBucket: "Storage Bucket",
        messagingSenderId: "Messaging Sender ID",
        appId: "App ID"
    };

    const app = initializeApp(firebaseConfig);
    const database = getDatabase(app);
    const firestore = getFirestore(app);

    function updateUI(elementId, value) {
        document.getElementById(elementId).innerText = value;
    }

    function updateProgress(value) {
        const circle = document.querySelector(".progress-bar");
        const radius = 45;
        const circumference = 2 * Math.PI * radius;
        const progress = (value / 100) * circumference;
        circle.style.strokeDasharray = `${progress}, ${circumference}`;
    }

    const moistureRef = ref(database, "soil_moisture/wet_percentage");
        onValue(moistureRef, (snapshot) => {
            let moistureValue = snapshot.val();
            if (moistureValue !== null) {
                updateUI("moisture_value", moistureValue);
                updateProgress(moistureValue);
            } else {
                updateUI("moisture_value", "Loading...");
            }
        });


    const nRef = ref(database, "/soil/N");
    const pRef = ref(database, "/soil/P");
    const kRef = ref(database, "/soil/K");

    onValue(nRef, (snapshot) => {
        updateUI("n_value", snapshot.val() !== null ? snapshot.val() : "Loading...");
    });
    onValue(pRef, (snapshot) => {
        updateUI("p_value", snapshot.val() !== null ? snapshot.val() : "Loading...");
    });
    onValue(kRef, (snapshot) => {
        updateUI("k_value", snapshot.val() !== null ? snapshot.val() : "Loading...");
    });


// Firebase references
const toggleRef = ref(database, "motor/mode");
const alarmRef = ref(database, "alarms");
const selectedCropRef = ref(database, "selected_crop"); // ✅ Corrected path

// Crop Dropdown Elements
const cropDropdownDisplay = document.getElementById("cropDropdownDisplay");
const cropDropdownOptions = document.getElementById("cropDropdownOptions");

// Fetch and display crops from Firestore
async function fetchCrops() {
    try {
        const cropsCollection = collection(firestore, "crops");
        const cropSnapshot = await getDocs(cropsCollection);

        cropDropdownOptions.innerHTML = "";
        cropDropdownDisplay.textContent = "Select Crop";

        cropSnapshot.forEach(doc => {
            const cropData = doc.data();
            const cropname = doc.id;
            const option = document.createElement("div");
            option.classList.add("dropdown-option");
            option.textContent = cropname;

            option.dataset.alarms = JSON.stringify(cropData.alarms || []);
            option.dataset.moisture = cropData.optimal_moisture || "--";

            option.addEventListener("click", async () => {
                const cropName = option.textContent;
                const alarmTimes = JSON.parse(option.dataset.alarms);
                const optimalMoisture = option.dataset.moisture;

                console.log(`Selected Crop: ${cropName}, Alarms:`, alarmTimes);
                console.log(`Optimal Moisture: ${optimalMoisture}%`);

                cropDropdownDisplay.textContent = cropName;
                cropDropdownOptions.classList.remove("show-options");

                // ✅ Save selected crop to correct Firebase path
                await set(selectedCropRef, {
                    cropName: cropName,
                    optimalMoisture: optimalMoisture
                });

                await addCropAlarms(alarmTimes);
            });

            cropDropdownOptions.appendChild(option);
        });

        // ✅ Restore previously selected crop from Firebase
        const snapshot = await get(selectedCropRef);
        if (snapshot.exists()) {
            const { cropName, optimalMoisture } = snapshot.val();
            cropDropdownDisplay.textContent = cropName;
            console.log(`Restored Crop from Firebase: ${cropName}, Optimal Moisture: ${optimalMoisture}%`);
        }

    } catch (error) {
        console.error("Error fetching crops:", error);
    }
}

// Add crop-specific alarms to Firebase
async function addCropAlarms(alarmTimes) {
    try {
        await set(alarmRef, {});
        console.log("Cleared all existing alarms.");

        if (!alarmTimes.length) {
            console.log("No alarms found for the selected crop.");
            return;
        }

        for (const time of alarmTimes) {
            const newAlarmRef = push(alarmRef);
            await set(newAlarmRef, { time, enabled: true });
            console.log(`Added new alarm at ${time}`);
        }
    } catch (error) {
        console.error("Error updating alarms:", error);
    }
}

// Restore crop if alarms still valid
async function checkAndRestoreSelectedCrop() {
    const storedSnapshot = await get(selectedCropRef);
    if (!storedSnapshot.exists()) return;

    const { cropName } = storedSnapshot.val();
    if (!cropName) return;

    const existingAlarmsSnapshot = await get(alarmRef);
    const existingAlarms = existingAlarmsSnapshot.val() || {};
    const existingAlarmTimes = new Set(Object.values(existingAlarms).map(alarm => alarm.time));

    const cropsCollection = collection(firestore, "crops");
    const cropSnapshot = await getDocs(cropsCollection);

    for (const doc of cropSnapshot.docs) {
        const cropData = doc.data();
        const cropname = doc.id;
        if (cropname === cropName) {
            const storedAlarms = cropData.alarms || [];

            if (storedAlarms.some(alarm => !existingAlarmTimes.has(alarm))) {
                await set(selectedCropRef, null); // clear invalid selection
                cropDropdownDisplay.textContent = "Select Crop";
                return;
            }

            cropDropdownDisplay.textContent = cropName;
            return;
        }
    }
}

// Toggle dropdown
cropDropdownDisplay.addEventListener("click", () => {
    cropDropdownOptions.classList.toggle("show-options");
});

// Hide dropdown if clicked outside
document.addEventListener("click", (e) => {
    if (!document.querySelector(".dropdown-container").contains(e.target)) {
        cropDropdownOptions.classList.remove("show-options");
    }
});

// Sync toggle switch with Firebase
const toggleSwitch = document.getElementById("color_mode");

onValue(toggleRef, (snapshot) => {
    const mode = snapshot.val();
    toggleSwitch.checked = (mode === "manual");
});

toggleSwitch.addEventListener("change", () => {
    const newMode = toggleSwitch.checked ? "manual" : "auto";
    set(toggleRef, newMode);
});

// On page load
fetchCrops();
checkAndRestoreSelectedCrop();


const motorRef = ref(database, "motor/state");
const motorToggle = document.getElementById("motorToggle");
const motorContainer = document.getElementById("motorContainer");

onValue(motorRef, (snapshot) => {
    const isOn = snapshot.val() === "on";
    motorToggle.classList.toggle("on", isOn);
    motorContainer.classList.toggle("on", isOn);
    motorContainer.classList.toggle("off", !isOn);
});

motorToggle.addEventListener("click", async () => {
    try {
        const snapshot = await get(motorRef);
        await set(motorRef, snapshot.val() === "on" ? "off" : "on");
    } catch (error) {
        console.error("Error toggling motor state:", error);
    }
});
  

setInterval(async () => {
    const now = new Date();
    const currentHour = now.getHours();
    const currentMinute = now.getMinutes();
    const currentPeriod = currentHour >= 12 ? "PM" : "AM";
    const formattedHour = ((currentHour + 11) % 12 + 1).toString().padStart(2, "0");
    const formattedMinute = currentMinute.toString().padStart(2, "0");
    const currentTime = `${formattedHour}:${formattedMinute} ${currentPeriod}`;

    try {
        const [soilMoistureSnapshot, motorStateSnapshot, motorModeSnapshot, alarmSnapshot, selectedCropSnapshot] = await Promise.all([
            get(ref(database, "/soil_moisture/wet_percentage")),
            get(ref(database, "/motor/state")),
            get(ref(database, "/motor/mode")),
            get(alarmRef),
            get(selectedCropRef)
        ]);

        const soilMoisture = soilMoistureSnapshot.val() || 0;
        const motorState = motorStateSnapshot.val();
        const motorMode = motorModeSnapshot.val()?.toLowerCase() || "auto";
        const optimalMoisture = parseInt(selectedCropSnapshot.val()?.optimalMoisture || "100");

        if (motorMode === "auto") {
            let motorShouldBeOn = false;
            let alarmIsEnabled = false;

            alarmSnapshot.forEach(child => {
                const alarmData = child.val();
                if (alarmData.enabled) {
                    alarmIsEnabled = true;
                    if (alarmData.time === currentTime && soilMoisture < optimalMoisture) {
                        motorShouldBeOn = true;
                    }
                }
            });

            if (motorState === "off" && motorShouldBeOn) {
                set(ref(database, "/motor/state"), "on");
            } else if (motorState === "on" && (!alarmIsEnabled || soilMoisture >= optimalMoisture)) {
                set(ref(database, "/motor/state"), "off");
            }
        }
    } catch (error) {
        console.error("Error fetching data:", error);
    }
}, 10000);
