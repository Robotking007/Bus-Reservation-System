const express = require('express');
const router = express.Router();
const fs = require('fs');
const path = require('path');

// Get all reservations
router.get('/', (req, res) => {
    const busesData = fs.readFileSync(path.join(__dirname, '../../backend/buses.dat'));
    const buses = JSON.parse(busesData.toString());
    
    const reservations = [];
    buses.forEach(bus => {
        bus.seats.forEach(seat => {
            if (seat.is_reserved) {
                reservations.push({
                    bus_number: bus.bus_number,
                    source: bus.source,
                    destination: bus.destination,
                    seat_number: seat.seat_number,
                    passenger_name: seat.passenger_name,
                    booking_time: seat.booking_time
                });
            }
        });
    });
    
    res.json(reservations);
});

// Create new reservation
router.post('/', (req, res) => {
    const { busId, seatNumber, passengerInfo } = req.body;
    
    const busesData = fs.readFileSync(path.join(__dirname, '../../backend/buses.dat'));
    const buses = JSON.parse(busesData.toString());
    
    const busIndex = buses.findIndex(b => b.bus_number === busId);
    if (busIndex === -1) {
        return res.status(404).json({ message: 'Bus not found' });
    }
    
    if (seatNumber < 1 || seatNumber > buses[busIndex].total_seats) {
        return res.status(400).json({ message: 'Invalid seat number' });
    }
    
    if (buses[busIndex].seats[seatNumber - 1].is_reserved) {
        return res.status(400).json({ message: 'Seat already reserved' });
    }
    
    // Update the seat reservation
    buses[busIndex].seats[seatNumber - 1] = {
        ...buses[busIndex].seats[seatNumber - 1],
        is_reserved: true,
        ...passengerInfo,
        booking_time: new Date().toISOString()
    };
    
    // Save back to file
    fs.writeFileSync(
        path.join(__dirname, '../../backend/buses.dat'),
        JSON.stringify(buses, null, 2)
    );
    
    res.status(201).json({
        message: 'Reservation successful',
        reservation: buses[busIndex].seats[seatNumber - 1]
    });
});

module.exports = router;