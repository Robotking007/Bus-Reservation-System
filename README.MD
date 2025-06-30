# 🚌 Bus Reservation System - The Trio Software

A complete bus ticket booking system with C backend, Node.js API bridge, and modern web frontend.

![System Architecture](assets/images/system-architecture.png) *(optional diagram)*

## 🌟 Features

- **User Types**:
  - Admin: Manage buses, view all reservations
  - Customers: Book/cancel tickets, view bookings
- **Core Functionality**:
  - Real-time seat availability
  - Booking history
  - Ticket printing
  - User profiles

## 🛠️ Tech Stack

| Component       | Technology |
|-----------------|------------|
| Backend (Core)  | C          |
| API Bridge      | Node.js    |
| Frontend        | HTML5/CSS3/JS |
| Data Storage    | Binary files (.dat) |

## 🚀 Quick Start

### Prerequisites
- GCC Compiler (for C)
- Node.js v16+
- Modern browser

### Installation

1. **Backend Setup**:
   ```bash
   cd backend
   gcc "The Trio Software.c" -o TrioSoftware
   ```

2. **API Setup**:
   ```bash
   cd ../api
   npm install
   ```

3. **Frontend**:
   - Open `frontend/index.html` in browser

### Running the System

1. Start C backend:
   ```bash
   cd backend
   ./TrioSoftware
   ```

2. Start Node.js API (in separate terminal):
   ```bash
   cd api
   node app.js
   ```

3. Access web interface at:
   ```
   http://localhost:3000
   ```

## 🔑 Default Credentials

| Role    | Username | Password  |
|---------|----------|-----------|
| Admin   | admin    | admin123  |
| User    | (Register new account) |

## 📂 Project Structure

```
bus-reservation-system/
├── backend/          # C program and data files
├── api/              # Node.js API server
├── frontend/         # Web interface
│   ├── assets/       # CSS, JS, images
│   ├── *.html        # All pages
└── README.md
```

## 🐛 Troubleshooting

**Common Issues**:
1. GCC compilation errors:
   - Ensure filename has no spaces or use quotes
   - Verify MinGW is installed correctly

2. Node.js server not starting:
   ```bash
   npm cache clean --force
   rm -rf node_modules
   npm install
   ```

3. Frontend not connecting:
   - Check API is running
   - Enable CORS in browser if testing locally

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

💡 **Pro Tip**: For development, use VS Code with these extensions:
- C/C++ (Microsoft)
- Live Server
- ESLint
