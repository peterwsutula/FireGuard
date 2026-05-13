/**
 * FireGuard PRO — PC Bridge (v3)
 * Serial → HTTP for all sensor data: motion, temp, humidity, AQI
 *
 * Setup:
 *   cd Hackathon/bridge && npm install && node server.js
 *
 * ⚠️  Change COM_PORT to match your Elegoo MEGA's port.
 *     Arduino IDE → Tools → Port to find it.
 */

const { SerialPort, ReadlineParser } = require('serialport');
const express = require('express');
const cors = require('cors');

const COM_PORT = 'COM3';   // ← CHANGE THIS
const BAUD_RATE = 9600;
const HTTP_PORT = 8080;

let latestData = {
  mayday: false,
  moving: true,
  still_seconds: 0,
  temp: null,
  co2: null,
  nh4: null,
  nh3: null,
  toluene: null,
  connected: false,
  ts: null,
};

const port = new SerialPort({ path: COM_PORT, baudRate: BAUD_RATE });
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

port.on('open', () => {
  console.log(`✅ ${COM_PORT} opened`);
  latestData.connected = true;
});

port.on('error', err => {
  console.error('❌ Serial error:', err.message);
  latestData.connected = false;
});

// ── Parse incoming hackathon lines ─────────────────────────
// Expected: "Data Received: 25.00,1,400.00,0.50,0.20,0.10"
parser.on('data', line => {
  line = line.trim();
  if (line === '') return;

  const prefix = "Data Received: ";
  if (line.includes(prefix)) {
    const csv = line.substring(line.indexOf(prefix) + prefix.length);
    const parts = csv.split(',');
    
    if (parts.length >= 6) {
      latestData.temp    = parseFloat(parts[0]);
      let isMoving       = (parts[1] === '1' || parts[1] === 'true');
      latestData.co2     = parseFloat(parts[2]);
      latestData.nh4     = parseFloat(parts[3]);
      latestData.nh3     = parseFloat(parts[4]);
      latestData.toluene = parseFloat(parts[5]);
      
      latestData.moving = isMoving;
      if (isMoving) {
        latestData.still_seconds = 0;
        latestData.mayday = false;
      } else {
        // Assume messages arrive roughly every 2 seconds
        latestData.still_seconds += 2;
        if (latestData.still_seconds >= 20) {
          latestData.mayday = true;
        }
      }

      latestData.connected = true;
      latestData.ts = new Date().toISOString();

      const flag = latestData.mayday ? '🚨 MAYDAY' : (latestData.moving ? '🟢 OK' : '🟡 STILL');
      console.log(`[${latestData.ts}] ${flag} | still=${latestData.still_seconds}s | temp=${latestData.temp}°C | CO2=${latestData.co2} | NH3=${latestData.nh3}`);
    } else {
      console.log(`📡 Serial: ${line}`);
    }
  } else {
    console.log(`📡 Serial: ${line}`);
    if (line.includes('Fireguard Receiver')) latestData.connected = true;
  }
});

const app = express();
app.use(cors());

app.get('/data', (req, res) => res.json(latestData));
app.get('/status', (req, res) => res.json({ bridge: 'FireGuard v3', port: COM_PORT, ...latestData }));

app.listen(HTTP_PORT, () => {
  console.log(`\n🚀 Bridge → http://localhost:${HTTP_PORT}/data`);
  console.log(`   Open Hackathon/index.html in your browser\n`);
});
