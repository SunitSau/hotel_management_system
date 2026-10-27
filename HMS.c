#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 50
#define FILE_NAME "hotel_data.txt"

// Structure definition matching your exact schema
struct Room {
    int room_no;
    int is_occupied;        // 0 = Vacant, 1 = Booked
    int r_type;             // 1 = AC, 2 = Non-AC
    char name[30];
    int phone_num;
    char id_num[30];
    int days;
};

struct Room room[MAX_ROOMS];
int r_count = 0;

// ==========================================
// FILE HANDLING: LOAD RECORDS ON STARTUP
// ==========================================
void load_records() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        return; // File doesn't exist yet; start with 0 rooms
    }
    r_count = 0;
    while (r_count < MAX_ROOMS && 
           fscanf(fp, "%d %d %d %s %d %s %d",
                  &room[r_count].room_no,
                  &room[r_count].is_occupied,
                  &room[r_count].r_type,
                  room[r_count].name,
                  &room[r_count].phone_num,
                  room[r_count].id_num,
                  &room[r_count].days) == 7) {
        r_count++;
    }
    fclose(fp);
}

// ==========================================
// FILE HANDLING: SAVE RECORDS TO FILE
// ==========================================
void Saveexit() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp != NULL) {
        for (int i = 0; i < r_count; i++) {
            fprintf(fp, "%d %d %d %s %d %s %d\n",
                    room[i].room_no,
                    room[i].is_occupied,
                    room[i].r_type,
                    room[i].name,
                    room[i].phone_num,
                    room[i].id_num,
                    room[i].days);
        }
        fclose(fp);
    }
    printf("\nAll data successfully saved to '%s'.\n", FILE_NAME);
    printf("Thank you for using Hotel Management System. Goodbye!\n");
}

// ==========================================
// OPTION 1: CHECK-IN / ADD BOOKING
// ==========================================
void checkin() {
    printf("\n--- New booking ---\n");
    if (r_count == 0) {
        printf("No rooms available in the system! Add rooms first.\n");
        return;
    }

    printf("Available Rooms for booking:\n");
    int f = 0;
    for (int i = 0; i < r_count; i++) {
        if (room[i].is_occupied == 0) {
            f = 1;
            char type_str[10];
            if (room[i].r_type == 1) strcpy(type_str, "AC");
            else strcpy(type_str, "Non AC");

            printf("Room %d (%s) is vacant\n", room[i].room_no, type_str);
        }
    }

    if (f == 0) {
        printf("No Rooms available!!\n");
        return;
    }

    int r_no, book_i = -1;
    printf("\nEnter room no to book: ");
    scanf("%d", &r_no);

    for (int i = 0; i < r_count; i++) {
        if (room[i].room_no == r_no) {
            book_i = i;
            break;
        }
    }

    if (book_i == -1) {
        printf("Room not present!!\n");
        return;
    }

    if (room[book_i].is_occupied == 1) {
        printf("Room already booked!!\n");
        return;
    }

    printf("enter name: ");
    scanf("%s", room[book_i].name);

    printf("Enter phone number: ");
    scanf("%d", &room[book_i].phone_num);

    printf("Enter ID number: ");
    scanf("%s", room[book_i].id_num);

    printf("enter number of days of booking: ");
    scanf("%d", &room[book_i].days);

    room[book_i].is_occupied = 1;
    printf("Room %d successfully booked for %s!\n", r_no, room[book_i].name);
}

// ==========================================
// OPTION 2: VIEW ALL BOOKINGS AND ROOMS
// ==========================================
void view() {
    printf("\n---------- VIEW ROOM ----------\n");
    if (r_count == 0) {
        printf("No rooms created yet.\n");
        return;
    }

    printf("room no, status, type, name, number, id, days\n");
    for (int i = 0; i < r_count; i++) {
        char type[15];
        if (room[i].r_type == 1) {
            strcpy(type, "AC");
        } else {
            strcpy(type, "Non AC");
        }

        if (room[i].is_occupied == 1) {
            printf("%d, BOOKED, %s, %s, %d, %s, %d days\n",
                   room[i].room_no, type, room[i].name, room[i].phone_num, room[i].id_num, room[i].days);
        } else {
            printf("%d, vacant, %s, %s, %d, %s, %d days\n",
                   room[i].room_no, type, room[i].name, room[i].phone_num, room[i].id_num, room[i].days);
        }
    }
}

// ==========================================
// OPTION 3: CHECKOUT & BILL GENERATION
// ==========================================
void checkout() {
    printf("\n--- CHECKOUT ---\n");
    if (r_count == 0) {
        printf("No rooms registered in the system.\n");
        return;
    }

    int r_no, found_i = -1;
    printf("Enter room number to checkout: ");
    scanf("%d", &r_no);

    for (int i = 0; i < r_count; i++) {
        if (room[i].room_no == r_no) {
            found_i = i;
            break;
        }
    }

    if (found_i == -1) {
        printf("Room %d not present!!\n", r_no);
        return;
    }

    if (room[found_i].is_occupied == 0) {
        printf("Room %d is already vacant!\n", r_no);
        return;
    }

    // Tariff calculation: AC = 2000/day, Non-AC = 1200/day
    float rate = (room[found_i].r_type == 1) ? 2000.0f : 1200.0f;
    float total_bill = room[found_i].days * rate;

    printf("\n================ BILL INVOICE ================\n");
    printf("Guest Name   : %s\n", room[found_i].name);
    printf("Room Number  : %d (%s)\n", room[found_i].room_no, (room[found_i].r_type == 1) ? "AC" : "Non AC");
    printf("Contact No   : %d\n", room[found_i].phone_num);
    printf("ID Number    : %s\n", room[found_i].id_num);
    printf("Days Stayed  : %d\n", room[found_i].days);
    printf("Daily Rate   : Rs. %.2f\n", rate);
    printf("Total Amount : Rs. %.2f\n", total_bill);
    printf("==============================================\n");

    // Reset room fields back to default vacant status
    room[found_i].is_occupied = 0;
    strcpy(room[found_i].name, "NA");
    room[found_i].phone_num = 0;
    strcpy(room[found_i].id_num, "NA");
    room[found_i].days = 0;

    printf("Room %d has been checked out and is now vacant.\n", r_no);
}

// ==========================================
// OPTION 4: UPDATE ROOM DETAILS (SUBMENU)
// ==========================================
void update() {
    int up_ch;
    while (1) {
        printf("\n------- UPDATE ROOM DETAILS -------\n");
        printf("1) add new room\n");
        printf("2) delete room\n");
        printf("3) update rooms details\n");
        printf("4) back to main menu\n");
        printf("choose from 1-4: ");
        
        if (scanf("%d", &up_ch) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Try again.\n");
            continue;
        }

        // Return to main menu loop
        if (up_ch == 4) {
            return;
        }

        // Sub-option 1: Add room
        if (up_ch == 1) {
            if (r_count >= MAX_ROOMS) {
                printf("Error: Max limit reached (50 rooms).\n");
                continue;
            }

            int rn, rt, duplicate = 0;
            printf("Enter new room number: ");
            scanf("%d", &rn);

            for (int i = 0; i < r_count; i++) {
                if (rn == room[i].room_no) {
                    duplicate = 1;
                    break;
                }
            }

            if (duplicate == 0) {
                printf("enter room type: 1)AC 2)Non AC: ");
                scanf("%d", &rt);

                room[r_count].room_no = rn;
                room[r_count].is_occupied = 0;
                room[r_count].r_type = rt;
                strcpy(room[r_count].name, "NA");
                room[r_count].phone_num = 0;
                strcpy(room[r_count].id_num, "NA");
                room[r_count].days = 0;

                r_count++;
                printf("room added\n");
            } else {
                printf("Room number already added!!\n");
            }
        }
        // Sub-option 2: Delete room
        else if (up_ch == 2) {
            if (r_count == 0) {
                printf("No rooms available to delete.\n");
                continue;
            }

            int del_no, del_index = -1;
            printf("Enter room number to delete: ");
            scanf("%d", &del_no);

            for (int i = 0; i < r_count; i++) {
                if (room[i].room_no == del_no) {
                    del_index = i;
                    break;
                }
            }

            if (del_index == -1) {
                printf("Room %d not present!!\n", del_no);
                continue;
            }

            if (room[del_index].is_occupied == 1) {
                printf("Cannot delete: Room %d is currently occupied by %s.\n", del_no, room[del_index].name);
                continue;
            }

            // Shift left
            for (int i = del_index; i < r_count - 1; i++) {
                room[i] = room[i + 1];
            }
            r_count--;
            printf("Room %d successfully deleted.\n", del_no);
        }
        // Sub-option 3: Update room type
        else if (up_ch == 3) {
            int mod_no, mod_index = -1;
            printf("Enter room number to modify: ");
            scanf("%d", &mod_no);

            for (int i = 0; i < r_count; i++) {
                if (room[i].room_no == mod_no) {
                    mod_index = i;
                    break;
                }
            }

            if (mod_index == -1) {
                printf("Room %d not present!!\n", mod_no);
                continue;
            }

            printf("Current type: %s\n", (room[mod_index].r_type == 1) ? "AC" : "Non AC");
            printf("Enter new type (1: AC, 2: Non AC): ");
            scanf("%d", &room[mod_index].r_type);
            printf("Room %d updated.\n", mod_no);
        }
        else {
            printf("Invalid selection! Choose between 1 and 4.\n");
        }
    }
}

// ==========================================
// MAIN FUNCTION & CONTROLLER LOOP
// ==========================================
int main() {
    int ch = 0;
    load_records(); // Read previously saved data from disk

    while (ch != 5) {
        printf("\n========= HOTEL MANAGEMENT SYSTEM =========\n");
        printf("1) Add booking\n");
        printf("2) view all booking and rooms\n");
        printf("3) Checkout\n");
        printf("4) update room details\n");
        printf("5) Save & exit\n");
        printf("\nEnter choice 1-5: ");

        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            printf("\n***** Enter Valid number *****\n");
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
            printf("\n***** Enter Valid number *****\n");
        }
    }
    return 0;
}