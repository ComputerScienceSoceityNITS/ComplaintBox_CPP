#include <bits/stdc++.h>
#include "ComplaintBox.h"
using namespace std;

int main() {
    ComplaintBox cb;
    string choice;
    int choiceNum = 0;

    do {
        cout << BOLDVIOLET << "\n==== Complaint Box Menu ====\n" << RESET;
        cout << CYAN
             << "1. Register User\n"
             << "2. Register Admin\n"
             << "3. User Login\n"
             << "4. Admin Login\n"
             << "5. File Complaint\n"
             << "6. Export Complaints to CSV\n"
             << "7. Search Complaints\n"
             << "8. Update Complaint Status (Admin Only)\n"
             << "9. Exit\n"
             << RESET;

        cout << WHITE << "Choice: " << RESET;
        cin >> choice;

        try {
            choiceNum = stoi(choice);
            switch (choiceNum) {
                case 1:
                    cb.registerUser();
                    break;
                case 2:
                    cb.registerUser(true);
                    break;
                case 3:
                    cb.loginUser();
                    break;
                case 4:
                    cb.loginUser(true);
                    break;
                case 5:
                    cb.fileComplaint();
                    break;
                case 6:
                    cb.exportComplaintsToCSV();
                    break;
                case 7:
                    cb.searchComplaints();
                    break;
                case 8:
                    if (!cb.isAdminLoggedIn()) {
                        cout << RED << "Only admins can update complaint status. Please login as admin first.\n" << RESET;
                    } else {
                        int id;
                        string status;
                        cout << YELLOW << "Enter Complaint ID to update: " << RESET;
                        cin >> id;
                        cin.ignore();
                        cout << YELLOW << "Enter new status (Pending/In Progress/Resolved): " << RESET;
                        getline(cin, status);
                        cb.updateComplaintStatus(id, status);
                    }
                    break;
                case 9:
                    cout << BOLDGREEN << "Exiting..." << RESET << endl;
                    break;
                default:
                    cout << BOLDRED << "Invalid choice!\n" << RESET;
            }
        } catch (exception& e) {
            cout << RED << "Invalid input! Please enter a number.\n" << RESET;
        }

    } while (choiceNum != 9);

    return 0;
}
