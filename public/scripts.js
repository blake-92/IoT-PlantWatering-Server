var socket = io();

socket.on("data", function (data) {

    console.log(data);

    document.getElementById("temperature").innerHTML = data.Temperatura;
    document.getElementById("humidity").innerHTML = data.Humedad;
    document.getElementById("humidity_ground").innerHTML = data.Hume_suelo;
    document.getElementById("water").innerHTML = data.Agua;
});

socket.on("date", function (date) {
    document.getElementById("time").innerHTML = date.date
})