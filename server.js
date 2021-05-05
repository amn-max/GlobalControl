const express = require("express");
const bodyParser = require("body-parser");
var http = require("http");
var path = require("path");
const INDEX = "./index.ejs";
const app = express();
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, "public")));
app.set("views", "./views");
app.set("view engine", "ejs");
const server = http.createServer(app);
const port = process.env.PORT || 3000;
const WebSocket = require("ws");
const s = new WebSocket.Server({ server });
var fs = require("fs");
const readLastLines = require("read-last-lines");
// app.get("/main", async function (req, res) {
//   let msgMain = "Main page of aqi-server use child nodes for get requests";
//   res.status(200).send(msgMain);
// });

app.get("/", async function (req, res) {
  var lines = [];
  res.render(INDEX, { lines: lines });
});

s.on("connection", function (ws, req) {
  ws.on("message", function (message) {
    console.log("Recevied: " + message);
    // if (message.substr(0, 7) === "Arduino") {
    //   fs.writeFile(
    //     "./log.txt",
    //     "\n" + new Date().toDateString() + " " + message,
    //     { flag: "a+" },
    //     (err, data) => {
    //       if (err) throw err;
    //     }
    //   );
    // }
    s.clients.forEach(function (client) {
      if (client != ws && client.readyState) {
        client.send(message);
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
