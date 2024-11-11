const express = require('express');
const { exec } = require('child_process');
const cors = require('cors');
const path = require('path');
const fs = require('fs');
const app = express();

app.use(cors());
app.use(express.json());

// POST endpoint to add a new train
app.post('/addTrain', (req, res) => {
    const { 
        trainNo, 
        trainName, 
        availableSeats, 
        ticketPrice, 
        fromLocation, 
        toLocation, 
        arrivalTime, 
        departureTime 
    } = req.body;

    // Validate input fields
    if (!trainNo || !trainName || !availableSeats || !ticketPrice || !fromLocation || !toLocation || !arrivalTime || !departureTime) {
        return res.status(400).send({ error: 'Missing required fields' });
    }

    const isWindows = process.platform === 'win32';
    const trainManagerPath = isWindows ? path.join(__dirname, 'trainManager.exe') : path.join(__dirname, 'trainManager');

    // Check if the executable already exists
    if (!fs.existsSync(trainManagerPath)) {
        // Compile the C++ file if necessary
        const compileCommand = `g++ -o ${trainManagerPath} trainManager.cpp`;

        exec(compileCommand, (compileErr, compileStdout, compileStderr) => {
            if (compileErr) {
                console.error('Compilation error:', compileErr);
                return res.status(500).send({ error: 'Compilation failed.', details: compileStderr });
            }

            if (compileStderr) {
                console.error('Compilation stderr:', compileStderr);
                return res.status(500).send({ error: compileStderr });
            }

            runTrainManager();
        });
    } else {
        // Run the compiled executable
        runTrainManager();
    }

    function runTrainManager() {
        const executeCommand = `${trainManagerPath} addTrain "${trainNo}" "${trainName}" "${availableSeats}" "${ticketPrice}" "${fromLocation}" "${toLocation}" "${arrivalTime}" "${departureTime}"`;

        exec(executeCommand, (execErr, execStdout, execStderr) => {
            if (execErr) {
                console.error('Execution error:', execErr);
                return res.status(500).send({ error: 'Execution failed.', details: execStderr });
            }

            if (execStderr) {
                console.error('Execution stderr:', execStderr);
                return res.status(500).send({ error: execStderr });
            }

            res.send({ message: 'Train added successfully!', output: execStdout.trim() });
        });
    }
});

// POST endpoint to book a ticket
app.post('/bookTicket', (req, res) => {
    const { trainNo, passengerName } = req.body;

    // Validate input fields
    if (!trainNo || !passengerName) {
        return res.status(400).send({ error: 'Missing required fields' });
    }

    const isWindows = process.platform === 'win32';
    const trainManagerPath = isWindows ? path.join(__dirname, 'trainManager.exe') : path.join(__dirname, 'trainManager');

    const executeCommand = `${trainManagerPath} book "${trainNo}" "${passengerName}"`;

    exec(executeCommand, (err, stdout, stderr) => {
        if (err || stderr) {
            console.error('Booking error:', err || stderr);
            return res.status(500).send({ error: 'Booking failed.', details: err || stderr });
        }
        res.send({ message: stdout.trim() });
    });
});

// POST endpoint to cancel a ticket
app.post('/cancelTicket', (req, res) => {
    const { trainNo, passengerName } = req.body;

    // Validate input fields
    if (!trainNo || !passengerName) {
        return res.status(400).send({ error: 'Missing required fields' });
    }

    const isWindows = process.platform === 'win32';
    const trainManagerPath = isWindows ? path.join(__dirname, 'trainManager.exe') : path.join(__dirname, 'trainManager');

    const executeCommand = `${trainManagerPath} cancel "${trainNo}" "${passengerName}"`;

    exec(executeCommand, (err, stdout, stderr) => {
        if (err || stderr) {
            console.error('Cancellation error:', err || stderr);
            return res.status(500).send({ error: 'Cancellation failed.', details: err || stderr });
        }
        res.send({ message: stdout.trim() });
    });
});

// Optional GET endpoint to fetch all trains
app.get('/getTrains', (req, res) => {
    res.send({ trains: [] });
});

// Starting the server on port 3000
app.listen(3000, () => {
    console.log('Server is running on http://localhost:3000');
});
