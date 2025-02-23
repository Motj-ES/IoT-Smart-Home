//thai
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
  } else {
    tempElements.forEach((element) => {
      element.textContent = "__";
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
  } else {
    huElements.forEach((element) => {
      element.textContent = "__"; // Hiển thị nếu không có dữ liệu
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

// function to on/off sideBar
const btnOn = document.getElementById("sb_bt");
const btnOff = document.getElementById("btn-off");
// btn is get tag have id: sb_bt

// func open sideBar
document.getElementById("mySidebar").style.display = "block";
//btnOn.style.display = "none";
// btnOff.style.display = "block";

// Select room elements from the DOM
const rooms = {
  "living-room-btn": document.getElementById("living-room"),
  "bedroom-btn": document.getElementById("bedroom"),
  "kitchen-btn": document.getElementById("kitchen"),
  "bathroom-btn": document.getElementById("bathroom"),
};

// Select device checkboxes
const devices = {
  window: document.getElementById("window"),
  fan: document.getElementById("fan"),
  light: document.getElementById("light"),
};

// Function to hide all rooms and show the selected room
function showRoom(selectedRoomId) {
  // Hide all rooms
  for (let key in rooms) {
    rooms[key].classList.remove("active");
  }
  // Show the selected room
  rooms[selectedRoomId].classList.add("active");

  // cheked, if 'selectedRoomId' == 'bathroom-btn', hide device have name id: 'device-window'
  let deviceWindow = document.getElementById("device-window");
  if (selectedRoomId == "bathroom-btn") {
    deviceWindow.style.display = "none";
  } else {
    deviceWindow.style.display = "block";
  }
}

//Select room information

//get all selector is room-info
const roomInfos = document.querySelectorAll(".room-info");

// loop through all selectors
roomInfos.forEach((room) => {
  // Remove the 'active' class from all room-info elements
  room.addEventListener("click", () => {
    roomInfos.forEach((info) => info.classList.remove("active"));

    // Add the 'active' property to the tag you click on.
    room.classList.add("active");
  });
});

//comment
document
  .getElementById("living-room-btn")
  .addEventListener("click", function () {
    showRoom("living-room-btn");
    updateRoomInfo("living");
  });

document.getElementById("bedroom-btn").addEventListener("click", function () {
  showRoom("bedroom-btn");
  updateRoomInfo("bedroom");
});

document.getElementById("kitchen-btn").addEventListener("click", function () {
  showRoom("kitchen-btn");
  updateRoomInfo("kitchen");
});

document.getElementById("bathroom-btn").addEventListener("click", function () {
  showRoom("bathroom-btn");
  updateRoomInfo("bathroom");
});

//comment
var dropdown = document.getElementsByClassName("dropdown");
var i;

//comment
for (i = 0; i < dropdown.length; i++) {
  dropdown[i].addEventListener("click", function () {
    this.classList.toggle("active");
    var dropdownContent = this.nextElementSibling;
    if (dropdownContent.style.display === "block") {
      dropdownContent.style.display = "none";
    } else {
      dropdownContent.style.display = "block";
    }
  });
}

// test;

function updateRoomInfo(room) {
  console.log("Room", room); // can see in console on web by ctrl + shift + i

  // delete listener before add new data
  database.ref(`/Devices/${room}`).off();

  // listen a change of date from database
  database.ref(`/Devices/${room}`).on("value", function (snapshot) {
    const deviceStates = snapshot.val();

    if (deviceStates) {
      // update status base on data from Firebase
      document.getElementById("fan").checked = deviceStates.Fan === "ON";
      document.getElementById("light").checked = deviceStates.Light === "ON";
      document.getElementById("window").checked = deviceStates.Window === "ON";
      document.getElementById("door").checked = deviceStates.Door === "ON";

      // update image base on data from Firebase
      document.getElementById("fan_img").src =
        deviceStates.Fan === "ON" ? "image/fan_on.gif" : "image/fan_off.png";
      document.getElementById("light_img").src =
        deviceStates.Light === "ON"
          ? "image/light_on.png"
          : "image/light_off.png";
      document.getElementById("wd_img").src =
        deviceStates.Window === "ON"
          ? "image/window-open.png"
          : "image/window-closed.png";
      document.getElementById("door_img").src =
        deviceStates.Door === "ON"
          ? "image/door_open.png"
          : "image/door_closed.png";
    }
  });

  // delete a old event and add a new event for room
  setupDeviceEventListeners(room);
}

function setupDeviceEventListeners(room) {
  // fan
  const fan = document.getElementById("fan_img");
  const fan_bt = document.getElementById("fan");
  fan_bt.replaceWith(fan_bt.cloneNode(true));
  const newFanButton = document.getElementById("fan");
  newFanButton.addEventListener("click", function () {
    var newState = newFanButton.checked ? "ON" : "OFF";
    // if 'newFanButton.checked' is true then add value 'ON' for 'newState'
    fan.src = newFanButton.checked ? "image/fan_on.gif" : "image/fan_off.png";
    database
      .ref(`/Devices/${room}`)
      .update({ Fan: newState }, function (error) {
        if (error) {
          // check error message with update data to firebase
          console.error("Không thể cập nhật Firebase:", error);
        } else {
          console.log("Cập nhật trạng thái Fan thành công");
        }
      });
  });

  // light
  const light = document.getElementById("light_img");
  const light_bt = document.getElementById("light");
  light_bt.replaceWith(light_bt.cloneNode(true));
  const newLightButton = document.getElementById("light");
  newLightButton.addEventListener("click", function () {
    var newState = newLightButton.checked ? "ON" : "OFF";
    light.src = newLightButton.checked
      ? "image/light_on.png"
      : "image/light_off.png";
    database
      .ref(`/Devices/${room}`)
      .update({ Light: newState }, function (error) {
        if (error) {
          console.error("Không thể cập nhật Firebase:", error);
        } else {
          console.log("Cập nhật trạng thái Light thành công");
        }
      });
  });

  // window
  const windowImg = document.getElementById("wd_img");
  const window_bt = document.getElementById("window");
  window_bt.replaceWith(window_bt.cloneNode(true));
  const newWindowButton = document.getElementById("window");
  newWindowButton.addEventListener("click", function () {
    var newState = newWindowButton.checked ? "ON" : "OFF";
    windowImg.src = newWindowButton.checked
      ? "image/window-open.png"
      : "image/window-closed.png";
    database
      .ref(`/Devices/${room}`)
      .update({ Window: newState }, function (error) {
        if (error) {
          console.error("Không thể cập nhật Firebase:", error);
        } else {
          console.log("Cập nhật trạng thái Window thành công");
        }
      });
  });

  // door
  const door = document.getElementById("door_img");
  const door_bt = document.getElementById("door");
  door_bt.replaceWith(door_bt.cloneNode(true));
  const newDoorButton = document.getElementById("door");
  newDoorButton.addEventListener("click", function () {
    var newState = newDoorButton.checked ? "ON" : "OFF";
    door.src = newDoorButton.checked
      ? "image/door_open.png"
      : "image/door_closed.png";
    database
      .ref(`/Devices/${room}`)
      .update({ Door: newState }, function (error) {
        if (error) {
          console.error("Không thể cập nhật Firebase:", error);
        } else {
          console.log("Cập nhật trạng thái Door thành công");
        }
      });
  });
}
