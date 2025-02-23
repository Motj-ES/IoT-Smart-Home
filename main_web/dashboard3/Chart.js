//THAI
const firebaseConfig = {
  apiKey: "AIzaSyAA-137QmrJf86fkHeHdhZfbFD4Ys5eF4I",
  authDomain: "smarthome-4d2d1.firebaseapp.com",
  databaseURL: "https://smarthome-4d2d1-default-rtdb.firebaseio.com",
  projectId: "smarthome-4d2d1",
  storageBucket: "smarthome-4d2d1.appspot.com",
  messagingSenderId: "844490387446",
  appId: "1:844490387446:web:b322a58d4b051a172b61d7",
  measurementId: "G-E54Y1EP3XP",
};

//init var
firebase.initializeApp(firebaseConfig);

// func open sideBar
document.getElementById("mySidebar").style.display = "block";

const updateInterval = 6000; // 10 mins(600000 milliseconds)
// Chart Temperature
const TempChart = document.getElementById("myChartTemp").getContext("2d");
let TempData = JSON.parse(localStorage.getItem("TempData")) || [];
let timeLabels_Temp = JSON.parse(localStorage.getItem("timeLabels_Temp")) || [];
const MAX_DATA_POINTS_Temp = 20; // Limit the data points

const Temp_Chart = new Chart(TempChart, {
  type: "line",
  data: {
    labels: timeLabels_Temp,
    datasets: [
      {
        label: "Nhiệt Độ",
        data: TempData,
        borderColor: "red",
        borderWidth: 2,
      },
    ],
  },
  options: {
    scales: {
      y: {
        beginAtZero: true,
        ticks: {
          //Print °C next to numbers
          callback: function (value) {
            return value + " °C";
          },
        },
      },
    },
  },
});

// Chart Humid
const HumidChart = document.getElementById("myChartHumid").getContext("2d");
let HumidData = JSON.parse(localStorage.getItem("HumidData")) || [];
let timeLabels_Humid =
  JSON.parse(localStorage.getItem("timeLabels_Humid")) || [];
const MAX_DATA_POINTS_Humid = 20; // Limit the data points

const Humid_Chart = new Chart(HumidChart, {
  type: "line",
  data: {
    labels: timeLabels_Humid,
    datasets: [
      {
        label: "Độ ẩm",
        data: HumidData,
        borderColor: "blue",
        borderWidth: 2,
      },
    ],
  },
  options: {
    scales: {
      y: {
        beginAtZero: true,
        ticks: {
          //Print % next to numbers
          callback: function (value) {
            return value + " %";
          },
        },
      },
    },
  },
});

function updateData() {
  // Update data from Firebase
  firebase
    .database()
    .ref("/Temperature")
    .once("value", (snap) => {
      //Use .once to only take data from firebase once
      const TempValue = snap.val();
      const currentTime_Temp = new Date().toLocaleTimeString();

      console.log(TempValue); // Check state of data

      if (TempValue !== null) {
        const currentTime = Date.now();
        // Add new data to array (LocalStorage)
        TempData.push(TempValue);
        timeLabels_Temp.push(currentTime_Temp);

        // Check the limit of data points
        if (TempData.length > MAX_DATA_POINTS_Temp) {
          TempData.shift(); // Delete the first data points
          timeLabels_Temp.shift(); // Delete the first time points
        }
        Temp_Chart.update(); // update chart with new data

        // Save data to Localstorage
        localStorage.setItem("TempData", JSON.stringify(TempData));
        localStorage.setItem(
          "timeLabels_Temp",
          JSON.stringify(timeLabels_Temp)
        );
      }
    });
  firebase
    .database()
    .ref("/Humidity")
    .once("value", (snap) => {
      //Use .once to only take data from firebase once
      const HumidValue = snap.val();
      const currentTime_Humid = new Date().toLocaleTimeString();

      console.log(HumidValue); // Check state of data

      if (HumidValue !== null) {
        const currentTime = Date.now();
        // Add new data to array (LocalStorage)
        HumidData.push(HumidValue);
        timeLabels_Humid.push(currentTime_Humid);

        // Check the limit of data points
        if (HumidData.length > MAX_DATA_POINTS_Humid) {
          HumidData.shift(); // Delete the first data points
          timeLabels_Humid.shift(); // Delete the first time points
        }
        Humid_Chart.update(); // update chart with new data

        // Save data to Localstorage
        localStorage.setItem("HumidData", JSON.stringify(HumidData));
        localStorage.setItem(
          "timeLabels_Humid",
          JSON.stringify(timeLabels_Humid)
        );
      }
    });
}
// Call updateData every 10 minute
setInterval(updateData, updateInterval);
