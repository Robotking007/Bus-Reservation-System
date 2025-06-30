// ===== DOM ELEMENTS =====
const DOM = {
  // Navigation
  navbar: document.querySelector('.navbar'),
  navLinks: document.querySelector('.nav-links'),
  burgerMenu: document.querySelector('.burger-menu'),

  // Auth Forms
  loginForm: document.getElementById('loginForm'),
  registerForm: document.getElementById('registerForm'),
  forgotPasswordForm: document.getElementById('forgotPasswordForm'),

  // Modals
  modals: document.querySelectorAll('.modal'),
  modalTriggers: document.querySelectorAll('[data-modal]'),
  modalCloses: document.querySelectorAll('.modal-close'),

  // Dashboard
  sidebarLinks: document.querySelectorAll('.sidebar-link'),
  dashboardSections: document.querySelectorAll('.dashboard-section'),

  // Tables
  tables: document.querySelectorAll('.table'),
  tableSorters: document.querySelectorAll('[data-sort]'),

  // Other
  backToTop: document.getElementById('backToTop')
};

// ===== STATE MANAGEMENT =====
const state = {
  currentUser: null,
  buses: [],
  reservations: [],
  users: [],
  sortDirection: 'asc'
};

// ===== UTILITY FUNCTIONS =====
const utils = {
  // Show alert message
  showAlert: (message, type = 'success', duration = 3000) => {
    const alert = document.createElement('div');
    alert.className = `alert alert-${type}`;
    alert.textContent = message;
    document.body.prepend(alert);
    
    setTimeout(() => {
      alert.classList.add('fade-out');
      setTimeout(() => alert.remove(), 300);
    }, duration);
  },

  // Toggle modal visibility
  toggleModal: (modalId, show = true) => {
    const modal = document.getElementById(modalId);
    if (show) {
      modal.classList.add('active');
      document.body.style.overflow = 'hidden';
    } else {
      modal.classList.remove('active');
      document.body.style.overflow = '';
    }
  },

  // Format date
  formatDate: (dateString) => {
    const options = { year: 'numeric', month: 'short', day: 'numeric' };
    return new Date(dateString).toLocaleDateString(undefined, options);
  },

  // Format time
  formatTime: (timeString) => {
    const [hours, minutes] = timeString.split(':');
    const period = hours >= 12 ? 'PM' : 'AM';
    const formattedHours = hours % 12 || 12;
    return `${formattedHours}:${minutes} ${period}`;
  },

  // Validate email
  validateEmail: (email) => {
    const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return re.test(email);
  },

  // Debounce function
  debounce: (func, delay) => {
    let timeoutId;
    return (...args) => {
      clearTimeout(timeoutId);
      timeoutId = setTimeout(() => func.apply(this, args), delay);
    };
  }
};

// ===== API FUNCTIONS =====
const api = {
  // Fetch data from API
  fetchData: async (endpoint, method = 'GET', body = null) => {
    try {
      const options = {
        method,
        headers: {
          'Content-Type': 'application/json',
          'Authorization': state.currentUser?.token ? `Bearer ${state.currentUser.token}` : ''
        }
      };

      if (body) {
        options.body = JSON.stringify(body);
      }

      const response = await fetch(`/api/${endpoint}`, options);
      const data = await response.json();

      if (!response.ok) {
        throw new Error(data.message || 'Something went wrong');
      }

      return data;
    } catch (error) {
      console.error('API Error:', error);
      utils.showAlert(error.message, 'danger');
      throw error;
    }
  },

  // User authentication
  login: async (username, password) => {
    const data = await api.fetchData('auth/login', 'POST', { username, password });
    state.currentUser = data.user;
    localStorage.setItem('currentUser', JSON.stringify(data.user));
    return data;
  },

  register: async (userData) => {
    const data = await api.fetchData('auth/register', 'POST', userData);
    return data;
  },

  // Bus operations
  getBuses: async () => {
    const data = await api.fetchData('buses');
    state.buses = data;
    return data;
  },

  // Reservation operations
  getReservations: async () => {
    const data = await api.fetchData('reservations');
    state.reservations = data;
    return data;
  },

  createReservation: async (reservationData) => {
    const data = await api.fetchData('reservations', 'POST', reservationData);
    return data;
  }
};

// ===== EVENT HANDLERS =====
const handlers = {
  // Navigation
  handleBurgerMenu: () => {
    DOM.navLinks.classList.toggle('active');
    DOM.burgerMenu.classList.toggle('active');
  },

  // Auth forms
  handleLogin: async (e) => {
    e.preventDefault();
    const username = DOM.loginForm.querySelector('#username').value;
    const password = DOM.loginForm.querySelector('#password').value;

    try {
      const { user } = await api.login(username, password);
      utils.showAlert(`Welcome back, ${user.name}!`);
      setTimeout(() => {
        window.location.href = user.isAdmin ? 'admin.html' : 'user.html';
      }, 1000);
    } catch (error) {
      console.error('Login failed:', error);
    }
  },

  handleRegister: async (e) => {
    e.preventDefault();
    const formData = new FormData(DOM.registerForm);
    const userData = Object.fromEntries(formData.entries());

    if (userData.password !== userData.confirmPassword) {
      return utils.showAlert('Passwords do not match', 'danger');
    }

    try {
      await api.register(userData);
      utils.showAlert('Registration successful! Please login.');
      setTimeout(() => {
        window.location.href = 'login.html';
      }, 1500);
    } catch (error) {
      console.error('Registration failed:', error);
    }
  },

  // Modal handling
  handleModalOpen: (e) => {
    e.preventDefault();
    const modalId = e.currentTarget.getAttribute('data-modal');
    utils.toggleModal(modalId, true);
  },

  handleModalClose: (e) => {
    e.preventDefault();
    const modal = e.currentTarget.closest('.modal');
    utils.toggleModal(modal.id, false);
  },

  // Dashboard
  handleSidebarLink: (e) => {
    e.preventDefault();
    const sectionId = e.currentTarget.getAttribute('data-section');
    
    // Update active state
    DOM.sidebarLinks.forEach(link => link.classList.remove('active'));
    e.currentTarget.classList.add('active');
    
    // Show corresponding section
    DOM.dashboardSections.forEach(section => {
      section.classList.toggle('active', section.id === `${sectionId}-section`);
    });
  },

  // Table sorting
  handleTableSort: (e) => {
    const column = e.currentTarget.getAttribute('data-sort');
    const table = e.currentTarget.closest('table');
    const tbody = table.querySelector('tbody');
    const rows = Array.from(tbody.querySelectorAll('tr'));

    // Toggle sort direction
    state.sortDirection = state.sortDirection === 'asc' ? 'desc' : 'asc';

    // Sort rows
    rows.sort((a, b) => {
      const aValue = a.querySelector(`td[data-column="${column}"]`).textContent;
      const bValue = b.querySelector(`td[data-column="${column}"]`).textContent;
      
      if (state.sortDirection === 'asc') {
        return aValue.localeCompare(bValue);
      } else {
        return bValue.localeCompare(aValue);
      }
    });

    // Re-append sorted rows
    rows.forEach(row => tbody.appendChild(row));
  },

  // Back to top
  handleBackToTop: () => {
    window.scrollTo({
      top: 0,
      behavior: 'smooth'
    });
  },

  // Window scroll
  handleWindowScroll: utils.debounce(() => {
    if (window.scrollY > 300) {
      DOM.backToTop?.classList.add('show');
    } else {
      DOM.backToTop?.classList.remove('show');
    }
  }, 100)
};

// ===== INITIALIZATION =====
const init = {
  // Initialize all event listeners
  eventListeners: () => {
    // Navigation
    DOM.burgerMenu?.addEventListener('click', handlers.handleBurgerMenu);

    // Auth forms
    DOM.loginForm?.addEventListener('submit', handlers.handleLogin);
    DOM.registerForm?.addEventListener('submit', handlers.handleRegister);

    // Modals
    DOM.modalTriggers.forEach(trigger => {
      trigger.addEventListener('click', handlers.handleModalOpen);
    });

    DOM.modalCloses.forEach(close => {
      close.addEventListener('click', handlers.handleModalClose);
    });

    // Dashboard
    DOM.sidebarLinks.forEach(link => {
      link.addEventListener('click', handlers.handleSidebarLink);
    });

    // Tables
    DOM.tableSorters?.forEach(sorter => {
      sorter.addEventListener('click', handlers.handleTableSort);
    });

    // Back to top
    DOM.backToTop?.addEventListener('click', handlers.handleBackToTop);
    window.addEventListener('scroll', handlers.handleWindowScroll);
  },

  // Load initial data
  loadData: async () => {
    try {
      // Check if user is logged in
      const storedUser = localStorage.getItem('currentUser');
      if (storedUser) {
        state.currentUser = JSON.parse(storedUser);
      }

      // Load data based on current page
      if (window.location.pathname.includes('buses.html')) {
        await api.getBuses();
        renderBuses();
      } else if (window.location.pathname.includes('reservations.html')) {
        await api.getReservations();
        renderReservations();
      } else if (window.location.pathname.includes('admin.html') || 
                 window.location.pathname.includes('user.html')) {
        await Promise.all([api.getBuses(), api.getReservations()]);
        renderDashboard();
      }
    } catch (error) {
      console.error('Initialization error:', error);
    }
  }
};

// ===== RENDER FUNCTIONS =====
const render = {
  // Render buses list
  buses: () => {
    const busesContainer = document.querySelector('.buses-list');
    if (!busesContainer) return;

    busesContainer.innerHTML = state.buses.length > 0 ? 
      state.buses.map(bus => `
        <div class="card bus-card" data-id="${bus.id}">
          <div class="bus-header">
            <h3>Bus #${bus.bus_number}</h3>
            <span class="badge">${bus.available_seats} seats available</span>
          </div>
          <div class="bus-body">
            <div class="bus-info">
              <div class="info-row">
                <span class="info-label">Route:</span>
                <span>${bus.source} to ${bus.destination}</span>
              </div>
              <div class="info-row">
                <span class="info-label">Departure:</span>
                <span>${utils.formatTime(bus.departure_time)} on ${utils.formatDate(bus.date)}</span>
              </div>
              <div class="info-row">
                <span class="info-label">Fare:</span>
                <span>₹${bus.fare.toFixed(2)}</span>
              </div>
            </div>
            <div class="bus-actions">
              <button class="btn btn-primary" data-modal="reservation-modal">Book Now</button>
            </div>
          </div>
        </div>
      `).join('') :
      `<div class="no-buses">
        <i class="fas fa-bus"></i>
        <h3>No Buses Available</h3>
        <p>Check back later for new schedules.</p>
      </div>`;
  },

  // Render reservations
  reservations: () => {
    const reservationsContainer = document.querySelector('.reservations-list');
    if (!reservationsContainer) return;

    reservationsContainer.innerHTML = state.reservations.length > 0 ? 
      state.reservations.map(reservation => `
        <div class="card reservation-card" data-id="${reservation.id}">
          <div class="reservation-info">
            <h3>Bus #${reservation.bus_number}</h3>
            <p>${reservation.source} to ${reservation.destination}</p>
            <p>Seat: ${reservation.seat_number} • ${utils.formatDate(reservation.date)}</p>
            <span class="status-badge ${reservation.status}">${reservation.status}</span>
          </div>
          <div class="reservation-actions">
            <button class="btn btn-outline" data-modal="ticket-modal">
              <i class="fas fa-ticket-alt"></i> View Ticket
            </button>
            ${reservation.status === 'upcoming' ? `
              <button class="btn btn-danger">
                <i class="fas fa-times"></i> Cancel
              </button>
            ` : ''}
          </div>
        </div>
      `).join('') :
      `<div class="no-reservations">
        <i class="fas fa-ticket-alt"></i>
        <h3>No Reservations Found</h3>
        <p>You haven't made any reservations yet. <a href="buses.html">Book your first trip now!</a></p>
      </div>`;
  },

  // Render dashboard
  dashboard: () => {
    // Update user greeting
    const greeting = document.getElementById('user-greeting');
    if (greeting && state.currentUser) {
      greeting.textContent = state.currentUser.name.split(' ')[0];
    }

    // Update stats
    const upcomingTrips = state.reservations.filter(r => r.status === 'upcoming').length;
    document.getElementById('upcoming-trips')?.textContent = upcomingTrips;
    document.getElementById('total-trips')?.textContent = state.reservations.length;
    document.getElementById('loyalty-points')?.textContent = state.currentUser?.loyalty_points || 0;

    // Render upcoming trips
    const tripsContainer = document.querySelector('.trip-cards');
    if (tripsContainer) {
      const upcoming = state.reservations.filter(r => r.status === 'upcoming').slice(0, 3);
      
      tripsContainer.innerHTML = upcoming.length > 0 ? 
        upcoming.map(trip => `
          <div class="card trip-card" data-id="${trip.id}">
            <div class="trip-info">
              <h3>${trip.source} to ${trip.destination}</h3>
              <p>Bus #${trip.bus_number} • Seat ${trip.seat_number}</p>
              <p>${utils.formatTime(trip.departure_time)} on ${utils.formatDate(trip.date)}</p>
            </div>
            <div class="trip-actions">
              <button class="btn btn-outline" data-modal="ticket-modal">
                <i class="fas fa-ticket-alt"></i> Ticket
              </button>
            </div>
          </div>
        `).join('') :
        `<div class="no-trips">
          <i class="fas fa-ticket-alt"></i>
          <h3>No Upcoming Trips</h3>
          <p>You don't have any upcoming trips. <a href="buses.html">Book your next adventure now!</a></p>
        </div>`;
    }
  }
};

// ===== MAIN INITIALIZATION =====
document.addEventListener('DOMContentLoaded', () => {
  init.eventListeners();
  init.loadData();
});