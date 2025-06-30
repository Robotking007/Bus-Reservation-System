const express = require('express');
const router = express.Router();
const fs = require('fs');
const path = require('path');

// Get all buses
router.get('/', (req, res) => {
    const busesData = fs.readFileSync(path.join(__dirname, '../../backend/buses.dat'));
    const buses = JSON.parse(busesData.toString());
    res.json(buses);
});

// Get single bus
router.get('/:id', (req, res) => {
    const busId = parseInt(req.params.id);
    const busesData = fs.readFileSync(path.join(__dirname, '../../backend/buses.dat'));
    const buses = JSON.parse(busesData.toString());
    
    const bus = buses.find(b => b.bus_number === busId);
    if (bus) {
        res.json(bus);
    } else {
        res.status(404).json({ message: 'Bus not found' });
    }
});

// Add new bus (admin only)
router.post('/', (req, res) => {
    // In a real app, you would add admin authentication here
    const newBus = req.body;
    
    const busesData = fs.readFileSync(path.join(__dirname, '../../backend/buses.dat'));
    const buses = JSON.parse(busesData.toString());
    
    buses.push(newBus);
    
    fs.writeFileSync(
        path.join(__dirname, '../../backend/buses.dat'),
        JSON.stringify(buses, null, 2)
    );
    
    res.status(201).json(newBus);
});

module.exports = router;