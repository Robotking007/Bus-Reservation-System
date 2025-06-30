#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_SEATS 50
#define MAX_BUSES 20
#define MAX_USERS 200
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

void format_time_12hr(const char* time24, char* time12, size_t size);
void initialize_system();
void main_menu();
void admin_menu();
void user_menu();
void user_registration();
int user_login();
void forgot_password();
void display_buses();
void display_seats(int bus_index);
int find_bus(int bus_number);
void reserve_seat();
void cancel_reservation();
void display_reservations();
void add_bus();
void edit_bus();
void delete_bus();
void view_all_reservations();
void generate_report();
void save_data();
void load_data();
void change_password();
void view_profile();
void edit_profile();
void print_ticket(int bus_index, int seat_number);
int validate_date(const char *date);
int validate_time(const char *time);
void clear_input_buffer();

typedef struct {
    int seat_number;
    int is_reserved;
    char passenger_name[50];
    char passenger_username[50];
    int passenger_age;
    char passenger_gender;
    char passenger_contact[15];
    char passenger_email[100];
    int is_discounted;
    time_t booking_time;
} Seat;

typedef struct {
    int bus_number;
    char source[50];
    char destination[50];
    char departure_time[10];
    char arrival_time[10];
    char date[15];
    float fare;
    float business_fare;
    int total_seats;
    int available_seats;
    int business_seats;
    int available_business_seats;
    Seat seats[MAX_SEATS];
    char bus_type[20];
    char amenities[100];
} Bus;

typedef struct {
    char username[50];
    char password[50];
    char name[50];
    char email[100];
    char phone[15];
    char address[100];
    int is_admin;
    int is_verified;
    int loyalty_points;
} User;

Bus buses[MAX_BUSES];
User users[MAX_USERS];
int bus_count = 0;
int user_count = 0;
User current_user;
int logged_in = 0;

void format_time_12hr(const char* time24, char* time12, size_t size) {
    int hour, minute;
    char am_pm[3] = "AM";
    if (sscanf(time24, "%d:%d", &hour, &minute) != 2) {
        strncpy(time12, time24, size);
        time12[size-1] = '\0';
        return;
    }
    if (hour >= 12) {
        if (hour > 12)
            hour -= 12;
        strcpy(am_pm, "PM");
    } else if (hour == 0) {
        hour = 12;
    }
    snprintf(time12, size, "%02d:%02d %s", hour, minute, am_pm);
}

int main() {
    initialize_system();
    load_data();
    main_menu();
    save_data();
    return 0;
}

void initialize_system() {
    strcpy(users[0].username, ADMIN_USERNAME);
    strcpy(users[0].password, ADMIN_PASSWORD);
    strcpy(users[0].name, "Administrator");
    strcpy(users[0].email, "admin@bussystem.com");
    strcpy(users[0].phone, "0000000000");
    strcpy(users[0].address, "System Headquarters");
    users[0].is_admin = 1;
    users[0].is_verified = 1;
    users[0].loyalty_points = 1000;
    user_count = 1;

    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char date[15];
    strftime(date, sizeof(date), "%d-%m-%Y", tm);

    buses[0].bus_number = 101;
    strcpy(buses[0].source, "New York");
    strcpy(buses[0].destination, "Boston");
    strcpy(buses[0].departure_time, "08:00");
    strcpy(buses[0].arrival_time, "12:00");
    strcpy(buses[0].date, date);
    buses[0].fare = 455.50f;
    buses[0].business_fare = 655.50f;
    buses[0].total_seats = MAX_SEATS;
    buses[0].available_seats = MAX_SEATS;
    buses[0].business_seats = 10;
    buses[0].available_business_seats = 10;
    strcpy(buses[0].bus_type, "Standard");
    strcpy(buses[0].amenities, "AC, WiFi, Restroom");

    buses[1].bus_number = 102;
    strcpy(buses[1].source, "Chicago");
    strcpy(buses[1].destination, "Detroit");
    strcpy(buses[1].departure_time, "09:30");
    strcpy(buses[1].arrival_time, "02:15");
    strcpy(buses[1].date, date);
    buses[1].fare = 375.75f;
    buses[1].business_fare = 557.75f;
    buses[1].total_seats = MAX_SEATS;
    buses[1].available_seats = MAX_SEATS;
    buses[1].business_seats = 8;
    buses[1].available_business_seats = 8;
    strcpy(buses[1].bus_type, "Express");
    strcpy(buses[1].amenities, "AC, WiFi, Restroom, Snacks");

    bus_count = 2;

    for (int i = 0; i < bus_count; i++) {
        for (int j = 0; j < buses[i].total_seats; j++) {
            buses[i].seats[j].seat_number = j + 1;
            buses[i].seats[j].is_reserved = 0;
            strcpy(buses[i].seats[j].passenger_name, "");
            strcpy(buses[i].seats[j].passenger_username, "");
            buses[i].seats[j].passenger_age = 0;
            buses[i].seats[j].passenger_gender = ' ';
            strcpy(buses[i].seats[j].passenger_contact, "");
            strcpy(buses[i].seats[j].passenger_email, "");
            buses[i].seats[j].is_discounted = 0;
            buses[i].seats[j].booking_time = 0;
        }
    }
}

void main_menu() {
    int choice;
    do {
        printf("\n*********************************************\n");
        printf("\n              THE TRIO SOFTWARE              \n");
        printf("\n*********************************************\n");
        printf("1. User Login\n");
        printf("2. User Registration\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                if (user_login()) {
                    user_menu();
                }
                break;
            case 2:
                user_registration();
                break;
            case 3:
                if (user_login()) {
                    if (current_user.is_admin) {
                        admin_menu();
                    } else {
                        printf("You are not authorized as admin!\n");
                    }
                }
                break;
            case 4:
                printf("\n*********************************************************************\n");
                printf("\n                THANK YOU FOR USING THE TRIO SOFTWARE.               \n");
                printf("***********************************************************************\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);
}

void user_menu() {
    int choice;
    do {
        printf("\n**********************************************************************************\n");
        printf("\n                        WELCOME TO MENU OF THE TRIO SOFTWARE                      \n");
        printf("\n**********************************************************************************\n");
        printf("1. View Available Buses\n");
        printf("2. Reserve a Seat\n");
        printf("3. Cancel Reservation\n");
        printf("4. View My Reservations\n");
        printf("5. View Profile\n");
        printf("6. Edit Profile\n");
        printf("7. Change Password\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                display_buses();
                break;
            case 2:
                reserve_seat();
                break;
            case 3:
                cancel_reservation();
                break;
            case 4:
                display_reservations();
                break;
            case 5:
                view_profile();
                break;
            case 6:
                edit_profile();
                break;
            case 7:
                change_password();
                break;
            case 8:
                printf("\n***************************************************************************************\n");
                printf("\n                      GOODBYE! USER GOOD TO SEE YOU AGAIN                              \n");
                printf("\n***************************************************************************************\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 8);
}

void admin_menu() {
    int choice;
    do {
        printf("\n********************************************************************************************\n");
        printf("\n                           WELCOME TO ADMIN MENU OF THE TRIO SOFTWARE                       \n");
        printf("\n********************************************************************************************\n");

        printf("1. Add New Bus\n");
        printf("2. Edit Bus\n");
        printf("3. Delete Bus\n");
        printf("4. View All Buses\n");
        printf("5. View All Reservations\n");
        printf("6. Generate Report\n");
        printf("7. Change Password\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                add_bus();
                break;
            case 2:
                edit_bus();
                break;
            case 3:
                delete_bus();
                break;
            case 4:
                display_buses();
                break;
            case 5:
                view_all_reservations();
                break;
            case 6:
                generate_report();
                break;
            case 7:
                change_password();
                break;
            case 8:
                printf("\n*********************************************************************************\n");
                printf("\n                     GOODBYE! ADMIN GOOD TO SEE YOU AGAIN                        \n");
                printf("\n*********************************************************************************\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 8);
}

void user_registration() {
    if (user_count >= MAX_USERS) {
        printf("Maximum user limit reached!\n");
        return;
    }

    User new_user;
    new_user.is_admin = 0;
    new_user.is_verified = 0;
    new_user.loyalty_points = 0;

    printf("\n===== USER REGISTRATION =====\n");
    printf("Enter Username: ");
    scanf("%49s", new_user.username);
    clear_input_buffer();

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, new_user.username) == 0) {
            printf("Username already exists!\n");
            return;
        }
    }

    printf("Enter Password: ");
    scanf("%49s", new_user.password);
    clear_input_buffer();

    printf("Enter Full Name: ");
    fgets(new_user.name, sizeof(new_user.name), stdin);
    new_user.name[strcspn(new_user.name, "\n")] = '\0';

    printf("Enter Email: ");
    scanf("%99s", new_user.email);
    clear_input_buffer();

    printf("Enter Phone Number: ");
    scanf("%14s", new_user.phone);
    clear_input_buffer();

    printf("Enter Address: ");
    fgets(new_user.address, sizeof(new_user.address), stdin);
    new_user.address[strcspn(new_user.address, "\n")] = '\0';

    users[user_count++] = new_user;
    printf("\nRegistration successful! You can now login.\n");
}

void forgot_password() {
    char username[50];
    char email[100];
    int found = 0;

    printf("\n===== FORGOT PASSWORD =====\n");
    printf("Enter your username: ");
    scanf("%s", username);
    clear_input_buffer();

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Enter your registered email: ");
            scanf("%s", email);
            clear_input_buffer();
            if (strcmp(users[i].email, email) == 0) {
                char new_password[50], confirm_password[50];
                printf("Enter new password: ");
                scanf("%s", new_password);
                clear_input_buffer();
                printf("Confirm new password: ");
                scanf("%49s", confirm_password);
                clear_input_buffer();
                if (strcmp(new_password, confirm_password) == 0) {
                    strcpy(users[i].password, new_password);
                    printf("Password reset successful. You can now login with your new password.\n");
                    found = 1;
                } else {
                    printf("Passwords do not match. Password reset failed.\n");
                    found = 1;
                }
            } else {
                printf("Email does not match our records.\n");
                found = 1;
            }
            break;
        }
    }
    if (!found) {
        printf("Username not found.\n");
    }
}

int user_login() {
    char username[50];
    char password[50];

    printf("\n===== LOGIN =====\n");
    printf("Username: ");
    scanf("%49s", username);
    clear_input_buffer();

    printf("Password: ");
    scanf("%49s", password);
    clear_input_buffer();

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            current_user = users[i];
            printf("\nLogin successful! Welcome %s.\n", current_user.name);
            return 1;
        }
    }

    printf("Invalid username or password!\n");
    char choice;
    printf("Forgot password? (y/n): ");
    scanf(" %c", &choice);
    clear_input_buffer();
    if (choice == 'y' || choice == 'Y') {
        forgot_password();
    }
    return 0;
}

void display_buses() {
    printf("\nAvailable Buses:\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("Bus No. Source          Destination    Departure   Arrival     Date       Fare (INR)\n");
    printf("-----------------------------------------------------------------------------------\n");

    for (int i = 0; i < bus_count; i++) {
        char dep_12[12], arr_12[12];
        format_time_12hr(buses[i].departure_time, dep_12, sizeof(dep_12));
        format_time_12hr(buses[i].arrival_time, arr_12, sizeof(arr_12));
        printf("%-7d %-15s %-15s %-11s %-10s %-10s ₹%.2f\n",
               buses[i].bus_number,
               buses[i].source,
               buses[i].destination,
               dep_12,
               arr_12,
               buses[i].date,
               buses[i].fare);
    }
    printf("-----------------------------------------------------------------------------------\n");
}

void display_seats(int bus_index) {
    int columns = 10;
    printf("\nSeat Status for Bus %d (%s to %s):\n",
           buses[bus_index].bus_number,
           buses[bus_index].source,
           buses[bus_index].destination);
    printf("--------------------------------------------------\n");
    for (int i = 0; i < buses[bus_index].total_seats; i++) {
        printf("%3d:%-6s ", i + 1, buses[bus_index].seats[i].is_reserved ? "Booked" : "Free");
        if ((i + 1) % columns == 0) printf("\n");
    }
    printf("\n--------------------------------------------------\n");
}

int find_bus(int bus_number) {
    for (int i = 0; i < bus_count; i++) {
        if (buses[i].bus_number == bus_number) {
            return i;
        }
    }
    return -1;
}

void reserve_seat() {
    int bus_number, seat_number, bus_index;
    char name[50], contact[15], gender;
    int age;

    display_buses();

    printf("\nEnter Bus Number: ");
    scanf("%d", &bus_number);
    clear_input_buffer();

    bus_index = find_bus(bus_number);
    if (bus_index == -1) {
        printf("Invalid Bus Number!\n");
        return;
    }

    if (buses[bus_index].available_seats <= 0) {
        printf("Sorry, no seats available on this bus!\n");
        return;
    }

    display_seats(bus_index);

    printf("\nEnter Seat Number to reserve (1-%d): ", buses[bus_index].total_seats);
    scanf("%d", &seat_number);
    clear_input_buffer();

    if (seat_number < 1 || seat_number > buses[bus_index].total_seats) {
        printf("Invalid Seat Number!\n");
        return;
    }

    if (buses[bus_index].seats[seat_number - 1].is_reserved) {
        printf("Seat already reserved!\n");
        return;
    }

    printf("Enter Passenger Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter Passenger Age: ");
    scanf("%d", &age);
    clear_input_buffer();

    printf("Enter Passenger Gender (M/F/O): ");
    scanf("%c", &gender);
    clear_input_buffer();
    gender = toupper(gender);

    printf("Enter Contact Number: ");
    scanf("%14s", contact);
    clear_input_buffer();

    buses[bus_index].seats[seat_number - 1].is_reserved = 1;
    strcpy(buses[bus_index].seats[seat_number - 1].passenger_name, name);
    strcpy(buses[bus_index].seats[seat_number - 1].passenger_username, current_user.username);
    buses[bus_index].seats[seat_number - 1].passenger_age = age;
    buses[bus_index].seats[seat_number - 1].passenger_gender = gender;
    strcpy(buses[bus_index].seats[seat_number - 1].passenger_contact, contact);
    strcpy(buses[bus_index].seats[seat_number - 1].passenger_email, current_user.email);
    buses[bus_index].seats[seat_number - 1].booking_time = time(NULL);
    buses[bus_index].available_seats--;

    printf("\nReservation Successful!\n");
    print_ticket(bus_index, seat_number - 1);
}

void cancel_reservation() {
    int bus_number, seat_number, bus_index;

    display_buses();

    printf("\nEnter Bus Number: ");
    scanf("%d", &bus_number);
    clear_input_buffer();

    bus_index = find_bus(bus_number);
    if (bus_index == -1) {
        printf("Invalid Bus Number!\n");
        return;
    }

    display_seats(bus_index);

    printf("\nEnter Seat Number to cancel (1-%d): ", buses[bus_index].total_seats);
    scanf("%d", &seat_number);
    clear_input_buffer();

    if (seat_number < 1 || seat_number > buses[bus_index].total_seats) {
        printf("Invalid Seat Number!\n");
        return;
    }

    if (!buses[bus_index].seats[seat_number - 1].is_reserved) {
        printf("Seat not reserved!\n");
        return;
    }

    if (strcmp(buses[bus_index].seats[seat_number - 1].passenger_username, current_user.username) != 0 &&
        !current_user.is_admin) {
        printf("You can only cancel your own reservations!\n");
        return;
    }

    printf("\nCanceling reservation for:\n");
    printf("Bus: %d (%s to %s)\n", buses[bus_index].bus_number,
           buses[bus_index].source, buses[bus_index].destination);
    printf("Seat: %d\n", seat_number);
    printf("Passenger: %s (Age: %d)\n",
           buses[bus_index].seats[seat_number - 1].passenger_name,
           buses[bus_index].seats[seat_number - 1].passenger_age);

    buses[bus_index].seats[seat_number - 1].is_reserved = 0;
    strcpy(buses[bus_index].seats[seat_number - 1].passenger_name, "");
    strcpy(buses[bus_index].seats[seat_number - 1].passenger_username, "");
    buses[bus_index].seats[seat_number - 1].passenger_age = 0;
    buses[bus_index].seats[seat_number - 1].passenger_gender = ' ';
    strcpy(buses[bus_index].seats[seat_number - 1].passenger_contact, "");
    strcpy(buses[bus_index].seats[seat_number - 1].passenger_email, "");
    buses[bus_index].seats[seat_number - 1].booking_time = 0;
    buses[bus_index].available_seats++;

    printf("Reservation canceled successfully!\n");
}

void display_reservations() {
    printf("\nYour Reservations:\n");
    printf("----------------------------------------------------------------------------\n");
    printf("Bus No. Source      Destination  Seat  Departure  Date       Passenger\n");
    printf("----------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < bus_count; i++) {
        for (int j = 0; j < buses[i].total_seats; j++) {
            if (buses[i].seats[j].is_reserved &&
                strcmp(buses[i].seats[j].passenger_username, current_user.username) == 0) {
                char dep_12[12];
                format_time_12hr(buses[i].departure_time, dep_12, sizeof(dep_12));
                printf("%-7d %-12s %-12s %-5d %-10s %-10s %s\n",
                       buses[i].bus_number,
                       buses[i].source,
                       buses[i].destination,
                       buses[i].seats[j].seat_number,
                       dep_12,
                       buses[i].date,
                       buses[i].seats[j].passenger_name);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No reservations found for %s.\n", current_user.username);
    }
    printf("----------------------------------------------------------------------------\n");
}

void add_bus() {
    if (bus_count >= MAX_BUSES) {
        printf("Maximum number of buses reached!\n");
        return;
    }

    Bus new_bus;

    printf("\n===== ADD NEW BUS =====\n");

    printf("Enter Bus Number: ");
    scanf("%d", &new_bus.bus_number);
    clear_input_buffer();

    if (find_bus(new_bus.bus_number) != -1) {
        printf("Bus number already exists!\n");
        return;
    }

    printf("Enter Source: ");
    fgets(new_bus.source, sizeof(new_bus.source), stdin);
    new_bus.source[strcspn(new_bus.source, "\n")] = '\0';

    printf("Enter Destination: ");
    fgets(new_bus.destination, sizeof(new_bus.destination), stdin);
    new_bus.destination[strcspn(new_bus.destination, "\n")] = '\0';

    printf("Enter Departure Time (HH:MM): ");
    scanf("%9s", new_bus.departure_time);
    clear_input_buffer();

    if (!validate_time(new_bus.departure_time)) {
        printf("Invalid time format! Use HH:MM\n");
        return;
    }

    printf("Enter Arrival Time (HH:MM): ");
    scanf("%9s", new_bus.arrival_time);
    clear_input_buffer();

    if (!validate_time(new_bus.arrival_time)) {
        printf("Invalid time format! Use HH:MM\n");
        return;
    }

    printf("Enter Date (DD-MM-YYYY): ");
    scanf("%14s", new_bus.date);
    clear_input_buffer();

    if (!validate_date(new_bus.date)) {
        printf("Invalid date format! Use DD-MM-YYYY\n");
        return;
    }

    printf("Enter Fare (INR): ");
    scanf("%f", &new_bus.fare);
    clear_input_buffer();

    new_bus.business_fare = new_bus.fare + 200.0f;
    strcpy(new_bus.bus_type, "Standard");
    strcpy(new_bus.amenities, "AC, WiFi, Restroom");
    new_bus.business_seats = 8;
    new_bus.available_business_seats = 8;

    new_bus.total_seats = MAX_SEATS;
    new_bus.available_seats = MAX_SEATS;
    for (int i = 0; i < MAX_SEATS; i++) {
        new_bus.seats[i].seat_number = i + 1;
        new_bus.seats[i].is_reserved = 0;
        strcpy(new_bus.seats[i].passenger_name, "");
        strcpy(new_bus.seats[i].passenger_username, "");
        new_bus.seats[i].passenger_age = 0;
        new_bus.seats[i].passenger_gender = ' ';
        strcpy(new_bus.seats[i].passenger_contact, "");
        strcpy(new_bus.seats[i].passenger_email, "");
        new_bus.seats[i].is_discounted = 0;
        new_bus.seats[i].booking_time = 0;
    }
    buses[bus_count++] = new_bus;
    printf("\nBus added successfully!\n");
}

void edit_bus() {
    int bus_number, bus_index, new_bus_number;
    display_buses();

    printf("\nEnter Bus Number to edit: ");
    scanf("%d", &bus_number);
    clear_input_buffer();

    bus_index = find_bus(bus_number);
    if (bus_index == -1) {
        printf("Bus not found!\n");
        return;
    }

    printf("\nEditing Bus %d (%s to %s)\n",
           buses[bus_index].bus_number,
           buses[bus_index].source,
           buses[bus_index].destination);

    printf("Current Bus Number: %d\n", buses[bus_index].bus_number);
    printf("Enter new Bus Number (or enter the same number to skip): ");
    scanf("%d", &new_bus_number);
    clear_input_buffer();

    if (new_bus_number != buses[bus_index].bus_number) {
        if (find_bus(new_bus_number) != -1) {
            printf("Bus number already exists, cannot update to this number!\n");
            return;
        }
        buses[bus_index].bus_number = new_bus_number;
        printf("Bus number updated successfully!\n");
    }

    printf("Enter new Source (current: %s): ", buses[bus_index].source);
    fgets(buses[bus_index].source, sizeof(buses[bus_index].source), stdin);
    buses[bus_index].source[strcspn(buses[bus_index].source, "\n")] = '\0';

    printf("Enter new Destination (current: %s): ", buses[bus_index].destination);
    fgets(buses[bus_index].destination, sizeof(buses[bus_index].destination), stdin);
    buses[bus_index].destination[strcspn(buses[bus_index].destination, "\n")] = '\0';

    printf("Enter new Departure Time (current: %s): ", buses[bus_index].departure_time);
    scanf("%9s", buses[bus_index].departure_time);
    clear_input_buffer();

    if (!validate_time(buses[bus_index].departure_time)) {
        printf("Invalid time format! Use HH:MM\n");
        return;
    }

    printf("Enter new Arrival Time (current: %s): ", buses[bus_index].arrival_time);
    scanf("%9s", buses[bus_index].arrival_time);
    clear_input_buffer();

    if (!validate_time(buses[bus_index].arrival_time)) {
        printf("Invalid time format! Use HH:MM\n");
        return;
    }

    printf("Enter new Date (current: %s): ", buses[bus_index].date);
    scanf("%14s", buses[bus_index].date);
    clear_input_buffer();

    if (!validate_date(buses[bus_index].date)) {
        printf("Invalid date format! Use DD-MM-YYYY\n");
        return;
    }

    printf("Enter new Fare (current: %.2f): ", buses[bus_index].fare);
    scanf("%f", &buses[bus_index].fare);
    clear_input_buffer();

    printf("\nBus details updated successfully!\n");
}

void delete_bus() {
    int bus_number, bus_index;

    display_buses();

    printf("\nEnter Bus Number to delete: ");
    scanf("%d", &bus_number);
    clear_input_buffer();

    bus_index = find_bus(bus_number);
    if (bus_index == -1) {
        printf("Bus not found!\n");
        return;
    }

    for (int i = 0; i < buses[bus_index].total_seats; i++) {
        if (buses[bus_index].seats[i].is_reserved) {
            printf("Cannot delete bus with active reservations!\n");
            return;
        }
    }

    for (int i = bus_index; i < bus_count - 1; i++) {
        buses[i] = buses[i + 1];
    }
    bus_count--;
    printf("\nBus deleted successfully!\n");
}

void view_all_reservations() {
    printf("\nAll Reservations:\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("Bus No. Source      Destination  Seat  Passenger          User          Booking Time\n");
    printf("------------------------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < bus_count; i++) {
        for (int j = 0; j < buses[i].total_seats; j++) {
            if (buses[i].seats[j].is_reserved) {
                char time_str[20];
                strftime(time_str, sizeof(time_str), "%Y-%m-%d %I:%M %p",
                        localtime(&buses[i].seats[j].booking_time));

                printf("%-7d %-12s %-12s %-5d %-16s %-12s %s\n",
                       buses[i].bus_number,
                       buses[i].source,
                       buses[i].destination,
                       buses[i].seats[j].seat_number,
                       buses[i].seats[j].passenger_name,
                       buses[i].seats[j].passenger_username,
                       time_str);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No reservations found.\n");
    }
    printf("------------------------------------------------------------------------------------------------\n");
}

void generate_report() {
    printf("\n===== SYSTEM REPORT =====\n");
    printf("\nBus Statistics:\n");
    printf("Total buses: %d\n", bus_count);
    int total_reservations = 0;
    float total_revenue = 0;
    for (int i = 0; i < bus_count; i++) {
        int bus_reservations = 0;
        for (int j = 0; j < buses[i].total_seats; j++) {
            if (buses[i].seats[j].is_reserved) {
                bus_reservations++;
                total_revenue += buses[i].fare;
            }
        }
        total_reservations += bus_reservations;
        printf("\nBus %d (%s to %s):\n", buses[i].bus_number, buses[i].source, buses[i].destination);
        printf("Reservations: %d/%d (%.1f%% occupancy)\n",
               bus_reservations, buses[i].total_seats,
               (float)bus_reservations / buses[i].total_seats * 100);
        printf("Revenue: ₹%.2f\n", bus_reservations * buses[i].fare);
    }
    printf("\nTotal Reservations: %d\n", total_reservations);
    printf("Total Revenue: ₹%.2f\n", total_revenue);
    printf("\nUser Statistics:\n");
    printf("Total users: %d\n", user_count);
    printf("Admin users: %d\n", 1);
    printf("Regular users: %d\n", user_count - 1);
}

void view_profile() {
    printf("\n===== YOUR PROFILE =====\n");
    printf("Username: %s\n", current_user.username);
    printf("Name: %s\n", current_user.name);
    printf("Email: %s\n", current_user.email);
    printf("Phone: %s\n", current_user.phone);
    printf("Address: %s\n", current_user.address);
    printf("Account Type: %s\n", current_user.is_admin ? "Administrator" : "Regular User");
}

void edit_profile() {
    printf("\n===== EDIT PROFILE =====\n");
    printf("Current Name: %s\nEnter new Name: ", current_user.name);
    fgets(current_user.name, sizeof(current_user.name), stdin);
    current_user.name[strcspn(current_user.name, "\n")] = '\0';
    printf("Current Email: %s\nEnter new Email: ", current_user.email);
    scanf("%99s", current_user.email);
    clear_input_buffer();
    printf("Current Phone: %s\nEnter new Phone: ", current_user.phone);
    scanf("%14s", current_user.phone);
    clear_input_buffer();
    printf("Current Address: %s\nEnter new Address: ", current_user.address);
    fgets(current_user.address, sizeof(current_user.address), stdin);
    current_user.address[strcspn(current_user.address, "\n")] = '\0';
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, current_user.username) == 0) {
            users[i] = current_user;
            break;
        }
    }
    printf("\nProfile updated successfully!\n");
}

void change_password() {
    char current_password[50], new_password[50], confirm_password[50];
    printf("\n===== CHANGE PASSWORD =====\n");
    printf("Enter current password: ");
    scanf("%49s", current_password);
    clear_input_buffer();
    if (strcmp(current_password, current_user.password) != 0) {
        printf("Incorrect current password!\n");
        return;
    }
    printf("Enter new password: ");
    scanf("%49s", new_password);
    clear_input_buffer();
    printf("Confirm new password: ");
    scanf("%49s", confirm_password);
    clear_input_buffer();
    if (strcmp(new_password, confirm_password) != 0) {
        printf("Passwords don't match!\n");
        return;
    }
    strcpy(current_user.password, new_password);
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, current_user.username) == 0) {
            strcpy(users[i].password, new_password);
            break;
        }
    }
    printf("\nPassword changed successfully!\n");
}

void print_ticket(int bus_index, int seat_index) {
    printf("\n===== TICKET =====\n");
    printf("Bus Number: %d\n", buses[bus_index].bus_number);
    printf("Route: %s to %s\n", buses[bus_index].source, buses[bus_index].destination);

    char dep_12[12], arr_12[12];
    format_time_12hr(buses[bus_index].departure_time, dep_12, sizeof(dep_12));
    format_time_12hr(buses[bus_index].arrival_time, arr_12, sizeof(arr_12));

    printf("Departure: %s on %s\n", dep_12, buses[bus_index].date);
    printf("Arrival: %s\n", arr_12);
    printf("Seat Number: %d\n", buses[bus_index].seats[seat_index].seat_number);
    printf("Passenger: %s\n", buses[bus_index].seats[seat_index].passenger_name);
    printf("Age: %d, Gender: %c\n", buses[bus_index].seats[seat_index].passenger_age,
           buses[bus_index].seats[seat_index].passenger_gender);
    printf("Contact: %s\n", buses[bus_index].seats[seat_index].passenger_contact);
    printf("Email: %s\n", buses[bus_index].seats[seat_index].passenger_email);
    printf("Fare: ₹%.2f\n", buses[bus_index].fare);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %I:%M %p",
            localtime(&buses[bus_index].seats[seat_index].booking_time));
    printf("Booking Time: %s\n", time_str);
    printf("Booked By: %s\n", buses[bus_index].seats[seat_index].passenger_username);
    printf("==================\n");
}

int validate_date(const char *date) {
    if (strlen(date) != 10) return 0;
    if (date[2] != '-' || date[5] != '-') return 0;
    int day, month, year;
    if (sscanf(date, "%d-%d-%d", &day, &month, &year) != 3) return 0;
    if (day < 1 || day > 31) return 0;
    if (month < 1 || month > 12) return 0;
    if (year < 2020 || year > 2100) return 0;
    return 1;
}

int validate_time(const char *time) {
    if (strlen(time) != 5) return 0;
    if (time[2] != ':') return 0;
    int hour, minute;
    if (sscanf(time, "%d:%d", &hour, &minute) != 2) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    return 1;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void save_data() {
    FILE *bus_file = fopen("buses.dat", "wb");
    FILE *user_file = fopen("users.dat", "wb");
    if (bus_file) {
        fwrite(&bus_count, sizeof(int), 1, bus_file);
        fwrite(buses, sizeof(Bus), bus_count, bus_file);
        fclose(bus_file);
    } else {
        printf("Error saving bus data!\n");
    }
    if (user_file) {
        fwrite(&user_count, sizeof(int), 1, user_file);
        fwrite(users, sizeof(User), user_count, user_file);
        fclose(user_file);
    } else {
        printf("Error saving user data!\n");
    }
}

void load_data() {
    FILE *bus_file = fopen("buses.dat", "rb");
    FILE *user_file = fopen("users.dat", "rb");
    if (bus_file) {
        fread(&bus_count, sizeof(int), 1, bus_file);
        fread(buses, sizeof(Bus), bus_count, bus_file);
        fclose(bus_file);
        for (int i = 0; i < bus_count; i++) {
            for (int j = 0; j < buses[i].total_seats; j++) {
                buses[i].seats[j].seat_number = j + 1;
            }
        }
    }
    if (user_file) {
        fread(&user_count, sizeof(int), 1, user_file);
        fread(users, sizeof(User), user_count, user_file);
        fclose(user_file);
    }
}