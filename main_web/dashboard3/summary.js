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
var database = firebase.database();
//Processing for temperature and humidity
//Temp
const tempElements = document.querySelectorAll(".temp");

database.ref("/Temperature").on("value", function (snapshot) {
  let t = snapshot.val();

  if (t !== null) {
    tempElements.forEach((element) => {
      element.textContent = t + "°C";
    });
  }
});
//Humidity
const huElements = document.querySelectorAll(".hu");

database.ref("/Humidity").on("value", function (snapshot) {
  let humidity = snapshot.val();

  // Kiểm tra giá trị humidity có tồn tại không
  if (humidity !== null) {
    huElements.forEach((element) => {
      element.textContent = humidity + "%"; // Cập nhật với textContent cho tất cả các phần tử
    });
  }
});

// sumary js setup for firebase

const temp_status = document.querySelectorAll(".temp-status");
const humid_status = document.querySelectorAll(".humid-status");
const smoke_status = document.querySelectorAll(".smoke-status");
const fire_status = document.querySelectorAll(".fire-status");
const light_status = document.querySelectorAll(".light-status");
const hc_status = document.querySelectorAll(".hc-status");

database.ref("/SMOKE").on("value", function (snapshot) {
  let smoke = snapshot.val();

  //checked avaliable of value
  if (smoke !== null) {
    smoke_status.forEach((element) => {
      element.textContent = smoke;
    });
  }
});

database.ref("/Fire").on("value", function (snapshot) {
  let fire = snapshot.val();

  if (fire !== null) {
    fire_status.forEach((element) => {
      element.textContent = fire;
    });
  }
});

database.ref("/Light").on("value", function (snapshot) {
  let light = snapshot.val();

  if (light !== null) {
    light_status.forEach((element) => {
      element.textContent = light;
    });
  }
});

database.ref("/HC").on("value", function (snapshot) {
  let hc = snapshot.val();

  if (hc !== null) {
    hc_status.forEach((element) => {
      element.textContent = hc;
    });
  }
});

database.ref("/Temperature").on("value", function (snapshot) {
  let t = snapshot.val();

  if (t !== null) {
    temp_status.forEach((element) => {
      element.textContent = t + "°C";
    });
  }
});
//Humidity

database.ref("/Humidity").on("value", function (snapshot) {
  let humidity = snapshot.val();

  if (humidity !== null) {
    humid_status.forEach((element) => {
      element.textContent = humidity + "%";
    });
  }
});

// Function to update date and time every second
function updateDateTime() {
  const now = new Date();
  const dateTimeStr = now.toLocaleString(); // Date and time in string format
  document.getElementById("datetime").innerHTML = dateTimeStr; // Update time in HTML
}
setInterval(updateDateTime, 1000); // Refresh every second

// set block for sidebar
document.getElementById("mySidebar").style.display = "block";
