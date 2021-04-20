const express = require("express");
const bodyParser = require("body-parser");
var http = require("http");
var path = require("path");
const INDEX = "./index.html";
const app = express();
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
const server = http.createServer(app);
const port = process.env.PORT || 3000;
const WebSocket = require("ws");
const s = new WebSocket.Server({ server });

// app.get("/main", async function (req, res) {
//   let msgMain = "Main page of aqi-server use child nodes for get requests";
//   res.status(200).send(msgMain);
// });

app.get("/", async function (req, res) {
  res.sendFile(INDEX);
});

s.on("connection", function (ws, req) {
  ws.on("message", function (message) {
    console.log("Recevied: " + message);
    s.clients.forEach(function (client) {
      if (client != ws && client.readyState) {
        client.send("broadcast :" + message);
      }
    });
  });
  ws.on("close", function () {
    console.log("lost one client");
  });
  console.log("new client connected");
});
server.listen(port);
// app.listen(port, () => {
//   console.log(`app listening at http://localhost:${port}`);
// });
