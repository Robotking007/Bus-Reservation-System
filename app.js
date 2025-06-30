const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const path = require('path');
const fs = require('fs');

const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Serve static files from the frontend
app.use(express.static(path.join(__dirname, '../frontend')));

// API Routes
app.use('/api/auth', require('./routes/auth'));
app.use('/api/buses', require('./routes/buses'));
app.use('/api/reservations', require('./routes/reservations'));

// Execute C program function
function executeCProgram(args, callback) {
    const cProgramPath = path.join(__dirname, '../backend/TheTrioSoftware');
    
    // Compile the C program if not already compiled
    if (!fs.existsSync(cProgramPath)) {
        exec('gcc ../backend/TheTrioSoftware.c -o ../backend/TheTrioSoftware', (error, stdout, stderr) => {
            if (error) {
                console.error(`Compilation error: ${error}`);
                return callback(error, null);
            }
            runProgram(args, callback);
        });
    } else {
        runProgram(args, callback);
    }
    
    function runProgram(args, callback) {
        exec(`${cProgramPath} ${args.join(' ')}`, (error, stdout, stderr) => {
            if (error) {
                console.error(`Execution error: ${error}`);
                return callback(error, null);
            }
            if (stderr) {
                console.error(`stderr: ${stderr}`);
                return callback(new Error(stderr), null);
            }
            callback(null, stdout);
        });
    }
}

// Start server
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});