const express = require("express");
const bodyParser = require("body-parser");
const fs = require("fs");

const app = express();
app.use(bodyParser.json());

const data = JSON.parse(fs.readFileSync("appsettings.json", "utf8"));
const n = data.places.length + 1;

// Graph, dist, parent
let graph = Array.from({ length: n }, () => Array(n).fill(-1));
let dist = Array.from({ length: n }, () => Array(n).fill(Infinity));
let parent = Array.from({ length: n }, () => Array(n).fill(-1));

let nodes_name = [""];
let mpp = {};

// ---- Step 1: Load Data ----
for (let place of data.places) {
  nodes_name[place.id] = place.name.toUpperCase();
  mpp[place.name.toUpperCase()] = place.id;
}

for (let i = 1; i < n; i++) {
  if (data[i]) {
    for (let edge of data[i]) {
      graph[i][edge.node] = edge.distance;
    }
  }
}

// ---- Step 2: Floyd–Warshall Setup ----
for (let i = 1; i < n; i++) {
  for (let j = 1; j < n; j++) {
    if (graph[i][j] === -1) {
      dist[i][j] = Infinity;
    } else {
      dist[i][j] = graph[i][j];
      if (i !== j && graph[i][j] !== 0) {
        parent[i][j] = i;
      }
    }
  }
}

// ---- Step 3: Run Floyd–Warshall ----
for (let via = 1; via < n; via++) {
  for (let i = 1; i < n; i++) {
    for (let j = 1; j < n; j++) {
      if (dist[i][via] + dist[via][j] < dist[i][j]) {
        dist[i][j] = dist[i][via] + dist[via][j];
        parent[i][j] = via;
      }
    }
  }
}

// ---- Step 4: Find Path ----
function findPath(src, dest) {
  let srcNum = mpp[src.toUpperCase()];
  let destNum = mpp[dest.toUpperCase()];
  if (!srcNum || !destNum) return { error: "Invalid nodes" };

  let path = [];
  let stack = [destNum];

  let node = destNum;
  while (parent[srcNum][node] !== srcNum && parent[srcNum][node] !== -1) {
    node = parent[srcNum][node];
    stack.push(node);
  }
  stack.push(srcNum);

  while (stack.length > 0) {
    path.push(nodes_name[stack.pop()]);
  }

  return { distance: dist[srcNum][destNum], path };
}

// ---- Step 5: API Endpoint ----
app.post("/getPath", (req, res) => {
  const { source, destination } = req.body;
  const result = findPath(source, destination);
  res.json(result);
});

// Start server
app.listen(5000, () => console.log("Server running on http://localhost:5000"));
