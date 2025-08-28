// import logo from './logo.svg';
// import './App.css';

// function App() {
//   return (
//     <div className="App">
//       <header className="App-header">
//         <img src={logo} className="App-logo" alt="logo" />
//         <p>
//           Edit <code>src/App.js</code> and save to reload.
//         </p>
//         <a
//           className="App-link"
//           href="https://reactjs.org"
//           target="_blank"
//           rel="noopener noreferrer"
//         >
//           Learn React
//         </a>
//       </header>
//     </div>
//   );
// }

// export default App;
// import { useState } from "react";

// function App() {
//   const [source, setSource] = useState("");
//   const [destination, setDestination] = useState("");
//   const [result, setResult] = useState(null);

//   const getPath = async () => {
//     const res = await fetch("http://localhost:3000/getPath", {
//       method: "POST",
//       headers: { "Content-Type": "application/json" },
//       body: JSON.stringify({ source, destination }),
//     });
//     const data = await res.json();
//     setResult(data);
//   };

//   return (
//     <div style={{ padding: "20px" }}>
//       <h1>Campus Path Finder</h1>
//       <input placeholder="Source" value={source} onChange={(e) => setSource(e.target.value)} />
//       <input placeholder="Destination" value={destination} onChange={(e) => setDestination(e.target.value)} />
//       <button onClick={getPath}>Find Path</button>

//       {result && (
//         <div>
//           <h3>Distance: {result.distance} km</h3>
//           <p>Path: {result.path.join(" -> ")}</p>
//         </div>
//       )}
//     </div>
//   );
// }

// export default App;

import { useState } from "react";

function App() {
  const [source, setSource] = useState("");
  const [destination, setDestination] = useState("");
  const [result, setResult] = useState(null);

  const fetchPath = async () => {
    try {
      const response = await fetch(
        `http://localhost:3000/api/path?source=${source}&destination=${destination}`
      );
      const data = await response.json();
      setResult(data);
    } catch (error) {
      console.error("Error fetching path:", error);
    }
  };

  return (
    <div style={{ textAlign: "center", marginTop: "50px" }}>
      <h1>Campus Path Finder</h1>
      <input
        type="text"
        placeholder="Enter source"
        value={source}
        onChange={(e) => setSource(e.target.value)}
      />
      <input
        type="text"
        placeholder="Enter destination"
        value={destination}
        onChange={(e) => setDestination(e.target.value)}
      />
      <button onClick={fetchPath}>Find Path</button>

      {result && (
        <div style={{ marginTop: "20px" }}>
          <h2>Shortest Path</h2>
          <p>Path: {result.path.join(" → ")}</p>
          <p>Distance: {result.distance}</p>
        </div>
      )}
    </div>
  );
}

export default App;
