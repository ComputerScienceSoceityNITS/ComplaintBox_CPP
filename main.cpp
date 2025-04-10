#include <bits/stdc++.h>
#include "ComplaintBox.h"
using namespace std;

int main() {
    ComplaintBox cb;
    string choice;
    int choiceNum = 0;

    do {
        cout << BOLDVIOLET << "\n==== Complaint Box Menu ====\n" << RESET;
        cout << CYAN << "1. Register User\n"
             << "2. Register Admin\n"
             << "3. User Login\n"
             << "4. Admin Login\n"
             << "5. File Complaint\n"
             << "6. Export Complaints to CSV\n"
             << "7. Search Complaints\n"
             << "8. Exit\n" << RESET;
            
        cout << WHITE << "Choice: " << RESET;
        cin >> choice;

// <<<<<<< Fix/crash-main-menu
        try {
            choiceNum = stoi(choice); 
        switch (choiceNum)
        {
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
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } catch (exception& e) {  // stoi throw an exception when input is non-numeric string
        cout << "Invalid input! Please enter a number."<<endl; 
    }
}while (choiceNum != 6);
// =======
//         switch (choice) {
//             case 1:
//                 cb.registerUser();
//                 break;
//             case 2:
//                 cb.registerUser(true);
//                 break;
//             case 3:
//                 cb.loginUser();
//                 break;
//             case 4:
//                 cb.loginUser(true);
//                 break;
//             case 5:
//                 cb.fileComplaint();
//                 break;
//             case 6:
//                 cb.exportComplaintsToCSV();
//                 break;
//             case 7:
//                 cb.searchComplaints();
//                 break;
//             case 8:
//                 cout << BOLDGREEN << "Exiting..." << RESET << endl;
//                 break;
//             default:
//                 cout << BOLDRED << "Invalid choice!\n" << RESET;
//         }
//     } while (choice != 8);
// >>>>>>> main

    return 0;
}
