#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

const int MAX_ROOMS = 50;
const string FILE_NAME = "Hotel_data.txt";

// ----------------------------------------------------
// STRUCTURE DEFINITION
// ----------------------------------------------------
struct Room {
    int room_no;
    int is_occupied;        // 0 = Vacant, 1 = Booked
    int r_type;             // 1 = AC, 2 = Non-AC
    string name;
    int phone_num;
    string id_num;
    int days;
};

Room room[MAX_ROOMS];
int r_count = 0;

// ----------------------------------------------------
// FILE I/O: LOAD PERSISTENT DATA AT STARTUP
// ----------------------------------------------------
void load_records() {
    ifstream fp(FILE_NAME);
    if (!fp.is_open()) {
        return; // File does not exist yet; starts with 0 rooms
    }
    r_count = 0;
    while (r_count < MAX_ROOMS && 
           (fp >> room[r_count].room_no 
               >> room[r_count].is_occupied 
               >> room[r_count].r_type 
               >> room[r_count].name 
               >> room[r_count].phone_num 
               >> room[r_count].id_num 
               >> room[r_count].days)) {
        r_count++;
    }
    fp.close();
}

// ----------------------------------------------------
// FILE I/O: SAVE DATA TO FILE UPON EXIT
// ----------------------------------------------------
void Saveexit() {
    ofstream fp(FILE_NAME);
    if (fp.is_open()) {
        for (int i = 0; i < r_count; i++) {
            fp << room[i].room_no << " "
               << room[i].is_occupied << " "
               << room[i].r_type << " "
               << room[i].name << " "
               << room[i].phone_num << " "
               << room[i].id_num << " "
               << room[i].days << "\n";
        }
        fp.close();
    }
    cout << "\nAll data successfully saved to '" << FILE_NAME << "'.\n";
    cout << "Exiting system. Have a great day!\n";
}

// ----------------------------------------------------
// OPTION 1: ADD BOOKING (CHECK-IN)
// ----------------------------------------------------
void checkin() {
    cout << "\n--- New booking ---\n";
    if (r_count == 0) {
        cout << "No rooms available in the system! Please add rooms first.\n";
        return;
    }

    cout << "Available Rooms for booking:\n";
    int f = 0;
    for (int i = 0; i < r_count; i++) {
        if (room[i].is_occupied == 0) {
            f = 1;
            string type_str;
            if (room[i].r_type == 1) {
                type_str = "AC";
            } else {
                type_str = "Non AC";
            }
            cout << "Room " << room[i].room_no << " (" << type_str << ") is vacant\n";
        }
    }

    if (f == 0) {
        cout << "No Rooms available!!\n";
        return;
    }

    int r_no, book_i = -1;
    cout << "\nEnter room no to book: ";
    cin >> r_no;

    for (int i = 0; i < r_count; i++) {
        if (room[i].room_no == r_no) {
            book_i = i;
            break;
        }
    }

    if (book_i == -1) {
        cout << "Room not present!!\n";
        return;
    }

    if (room[book_i].is_occupied == 1) {
        cout << "Room already booked!!\n";
        return;
    }

    cout << "enter name: ";
    cin >> room[book_i].name;

    cout << "Enter phone number: ";
    cin >> room[book_i].phone_num;

    cout << "Enter ID number: ";
    cin >> room[book_i].id_num;

    cout << "enter number of days of booking: ";
    cin >> room[book_i].days;

    room[book_i].is_occupied = 1;
    cout << "Room " << r_no << " successfully booked for " << room[book_i].name << "!\n";
}

// ----------------------------------------------------
// OPTION 2: VIEW ALL ROOMS AND BOOKINGS
// ----------------------------------------------------
void view() {
    cout << "\n---------- VIEW ROOM ----------\n";
    if (r_count == 0) {
        cout << "No rooms created yet.\n";
        return;
    }

    cout << "room no, status, type, name, number, id, days\n";
    for (int i = 0; i < r_count; i++) {
        string type;
        if (room[i].r_type == 1) {
            type = "AC";
        } else {
            type = "Non AC";
        }

        if (room[i].is_occupied == 1) {
            cout << room[i].room_no << ", BOOKED, " << type << ", "
                 << room[i].name << ", " << room[i].phone_num << ", "
                 << room[i].id_num << ", " << room[i].days << " days\n";
        } else {
            cout << room[i].room_no << ", vacant, " << type << ", "
                 << room[i].name << ", " << room[i].phone_num << ", "
                 << room[i].id_num << ", " << room[i].days << " days\n";
        }
    }
}

// ----------------------------------------------------
// OPTION 3: CHECKOUT & INVOICE GENERATION
// ----------------------------------------------------
void checkout() {
    cout << "\n--- CHECKOUT ---\n";
    if (r_count == 0) {
        cout << "No rooms registered in the system.\n";
        return;
    }

    int r_no, found_i = -1;
    cout << "Enter room number to checkout: ";
    cin >> r_no;

    for (int i = 0; i < r_count; i++) {
        if (room[i].room_no == r_no) {
            found_i = i;
            break;
        }
    }

    if (found_i == -1) {
        cout << "Room " << r_no << " not present!!\n";
        return;
    }

    if (room[found_i].is_occupied == 0) {
        cout << "Room " << r_no << " is already vacant!\n";
        return;
    }

    // Tariff: AC = Rs. 2000/day, Non-AC = Rs. 1200/day
    float rate = (room[found_i].r_type == 1) ? 2000.0f : 1200.0f;
    float total_bill = room[found_i].days * rate;

    cout << "\n================ BILL INVOICE ================\n";
    cout << "Guest Name   : " << room[found_i].name << "\n";
    cout << "Room Number  : " << room[found_i].room_no << " (" << ((room[found_i].r_type == 1) ? "AC" : "Non AC") << ")\n";
    cout << "Contact No   : " << room[found_i].phone_num << "\n";
    cout << "ID Number    : " << room[found_i].id_num << "\n";
    cout << "Days Stayed  : " << room[found_i].days << "\n";
    cout << "Daily Tariff : Rs. " << rate << "\n";
    cout << "Total Bill   : Rs. " << total_bill << "\n";
    cout << "==============================================\n";

    // Reset room fields back to default vacant state
    room[found_i].is_occupied = 0;
    room[found_i].name = "NA";
    room[found_i].phone_num = 0;
    room[found_i].id_num = "NA";
    room[found_i].days = 0;

    cout << "Room " << r_no << " has been checked out successfully.\n";
}

// ----------------------------------------------------
// OPTION 4: UPDATE ROOM DETAILS SUB-MENU
// ----------------------------------------------------
void update() {
    int up_ch;
    while (true) {
        cout << "\n------- UPDATE ROOM DETAILS -------\n";
        cout << "1) add new room\n";
        cout << "2) delete room\n";
        cout << "3) update rooms details\n";
        cout << "4) back to main menu\n";
        cout << "choose from 1-4: ";

        if (!(cin >> up_ch)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (up_ch == 4) {
            return;
        }

        // Sub-option 1: Add room
        if (up_ch == 1) {
            if (r_count >= MAX_ROOMS) {
                cout << "Error: Maximum limit reached (50 rooms).\n";
                continue;
            }

            int rn, rt, duplicate = 0;
            cout << "Enter new room number: ";
            cin >> rn;

            for (int i = 0; i < r_count; i++) {
                if (rn == room[i].room_no) {
                    duplicate = 1;
                    break;
                }
            }

            if (duplicate == 0) {
                cout << "enter room type: 1)AC 2)Non AC: ";
                cin >> rt;

                room[r_count].room_no = rn;
                room[r_count].is_occupied = 0;
                room[r_count].r_type = rt;
                room[r_count].name = "NA";
                room[r_count].phone_num = 0;
                room[r_count].id_num = "NA";
                room[r_count].days = 0;

                r_count++;
                cout << "room added\n";
            } else {
                cout << "Room number already added!!\n";
            }
        }
        // Sub-option 2: Delete room
        else if (up_ch == 2) {
            if (r_count == 0) {
                cout << "No rooms available to delete.\n";
                continue;
            }

            int del_no, del_index = -1;
            cout << "Enter room number to delete: ";
            cin >> del_no;

            for (int i = 0; i < r_count; i++) {
                if (room[i].room_no == del_no) {
                    del_index = i;
                    break;
                }
            }

            if (del_index == -1) {
                cout << "Room " << del_no << " not present!!\n";
                continue;
            }

            if (room[del_index].is_occupied == 1) {
                cout << "Cannot delete: Room " << del_no << " is currently occupied by " << room[del_index].name << ".\n";
                continue;
            }

            for (int i = del_index; i < r_count - 1; i++) {
                room[i] = room[i + 1];
            }
            r_count--;
            cout << "Room " << del_no << " successfully deleted.\n";
        }
        // Sub-option 3: Modify room type
        else if (up_ch == 3) {
            int mod_no, mod_index = -1;
            cout << "Enter room number to modify: ";
            cin >> mod_no;

            for (int i = 0; i < r_count; i++) {
                if (room[i].room_no == mod_no) {
                    mod_index = i;
                    break;
                }
            }

            if (mod_index == -1) {
                cout << "Room " << mod_no << " not present!!\n";
                continue;
            }

            cout << "Current type: " << ((room[mod_index].r_type == 1) ? "AC" : "Non AC") << "\n";
            cout << "Enter new type (1: AC, 2: Non AC): ";
            cin >> room[mod_index].r_type;
            cout << "Room " << mod_no << " updated.\n";
        }
        else {
            cout << "Invalid selection! Choose between 1 and 4.\n";
        }
    }
}

// ----------------------------------------------------
// MAIN MENU CONTROLLER
// ----------------------------------------------------
int main() {
    int ch = 0;
    load_records();

    while (ch != 5) {
        cout << "\n========= HOTEL MANAGEMENT SYSTEM =========\n";
        cout << "1) Add booking\n";
        cout << "2) view all booking and rooms\n";
        cout << "3) Checkout\n";
        cout << "4) update room details\n";
        cout << "5) Save & exit\n";
        cout << "\nEnter choice 1-5: ";

        if (!(cin >> ch)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n***** Enter Valid number *****\n";
            continue;
        }

        if (ch == 1) {
            checkin();
        } else if (ch == 2) {
            view();
        } else if (ch == 3) {
            checkout();
        } else if (ch == 4) {
            update();
        } else if (ch == 5) {
            Saveexit();
        } else {
            cout << "\n***** Enter Valid number *****\n";
        }
    }
    return 0;
}