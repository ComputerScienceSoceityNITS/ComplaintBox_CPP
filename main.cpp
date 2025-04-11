#include <bits/stdc++.h>
#include "ComplaintBox.h"
using namespace std;

int main() {
    ComplaintBox cb;
    string choice;
    int choiceNum = 0;

    do {
        // Double-line box top
        cout << BOLDVIOLET << u8"╔════════════════════════════════════════════════════╗\n";
        cout << u8"║                  🗂️  COMPLAINT BOX MENU                  ║\n";
        cout << u8"╠════════════════════════════════════════════════════╣\n";

        // Menu options with dividers
        cout << u8"║ 1. Register as User                                 ║\n";
        cout << u8"╟────────────────────────────────────────────────────╢\n";
        cout << u8"║ 2. Register as Admin                                ║\n";
        cout << u8"╟────────────────────────────────────────────────────╢\n";
        cout << u8"║ 3. Login as User                                    ║\n";
        cout << u8"╟────────────────────────────────────────────────────╢\n";
        cout << u8"║ 4. Login as Admin                                   ║\n";
        cout << u8"╟────────────────────────────────────────────────────╢\n";
        cout << u8"║ 5. File a Complaint                                 ║\n";
        cout << u8"╟────────────────────────────────────────────────────╢\n";
        cout << u8"║ 6. Export Complaints to CSV                         ║\n";
        cout << u8"╟────────────────────────────────────────────────────╢\n";
        cout << u8"║ 7. Search Complaints                                ║\n";
        cout << u8"╟────────────────────────────────────────────────────╢\n";
        cout << u8"║ 8. Exit                                             ║\n";
        cout << u8"╚════════════════════════════════════════════════════╝\n" << RESET;

        // Input prompt
        cout << WHITE << "\n👉 Enter your choice (1-8): " << RESET;
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
