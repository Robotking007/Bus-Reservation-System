const express = require('express');
const router = express.Router();

// User login
router.post('/login', (req, res) => {
    const { username, password } = req.body;
    
    // Execute C program with login command
    executeCProgram(['login', username, password], (error, result) => {
        if (error) {
            return res.status(500).json({ success: false, message: 'Login failed' });
        }
        
        // Parse the result from C program
        if (result.includes('Login successful')) {
            const userData = parseUserData(result);
            return res.json({ 
                success: true, 
                user: userData,
                isAdmin: result.includes('admin') 
            });
        } else {
            return res.status(401).json({ success: false, message: 'Invalid credentials' });
        }
    });
});

// User registration
router.post('/register', (req, res) => {
    const { username, password, name, email, phone, address } = req.body;
    
    // Execute C program with registration command
    executeCProgram(['register', username, password, name, email, phone, address], (error, result) => {
        if (error) {
            return res.status(500).json({ success: false, message: 'Registration failed' });
        }
        
        if (result.includes('Registration successful')) {
            return res.json({ success: true, message: 'Registration successful' });
        } else if (result.includes('Username already exists')) {
            return res.status(400).json({ success: false, message: 'Username already exists' });
        } else {
            return res.status(500).json({ success: false, message: 'Registration failed' });
        }
    });
});

// Forgot password
router.post('/forgot-password', (req, res) => {
    const { username, email, newPassword } = req.body;
    
    // Execute C program with forgot password command
    executeCProgram(['forgot-password', username, email, newPassword], (error, result) => {
        if (error) {
            return res.status(500).json({ success: false, message: 'Password reset failed' });
        }
        
        if (result.includes('Password reset successful')) {
            return res.json({ success: true, message: 'Password reset successful' });
        } else if (result.includes('Email does not match')) {
            return res.status(400).json({ success: false, message: 'Email does not match our records' });
        } else if (result.includes('Username not found')) {
            return res.status(404).json({ success: false, message: 'Username not found' });
        } else {
            return res.status(500).json({ success: false, message: 'Password reset failed' });
        }
    });
});

// Helper function to parse user data from C program output
function parseUserData(output) {
    const lines = output.split('\n');
    const user = {};
    
    lines.forEach(line => {
        if (line.includes('Username:')) user.username = line.split(':')[1].trim();
        if (line.includes('Name:')) user.name = line.split(':')[1].trim();
        if (line.includes('Email:')) user.email = line.split(':')[1].trim();
        if (line.includes('Phone:')) user.phone = line.split(':')[1].trim();
        if (line.includes('Address:')) user.address = line.split(':')[1].trim();
    });
    
    return user;
}

module.exports = router;