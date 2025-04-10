#include <bits/stdc++.h>
#include "sqlite3.h"
using namespace std;

// ANSI escape color codes
#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define PURPLE      "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BOLDVIOLET  "\033[1;35m"
#define BOLDGREEN   "\033[1;32m"
#define BOLDRED     "\033[1;31m"

class ComplaintBox {
public:
    ComplaintBox() {
        sqlite3_open("complaints.db", &db);
        createTables();
    }

    ~ComplaintBox() {
        sqlite3_close(db);
    }

    void registerUser(bool isAdmin = false) {
        string uname, pass;
        cout << PURPLE << "Enter username: " << RESET;
        cin >> uname;

        string checkSql = "SELECT * FROM users WHERE username='" + uname + "';";
        bool exists = false;

        sqlite3_exec(db, checkSql.c_str(), [](void *data, int, char **, char **) -> int {
            *(bool *)data = true;
            return 0;
        }, &exists, &errMsg);

        if (exists) {
            cout << BOLDRED << "Username already exists. Please choose a different Username.\n" << RESET;
            return;
        }

        cout << PURPLE << "Enter password: " << RESET;
        cin >> pass;

        string table = isAdmin ? "adminusers" : "users";
        string sql = "INSERT INTO " + table + " (username, password) VALUES ('" + uname + "', '" + pass + "');";

        if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
            cout << RED << "Error: " << errMsg << RESET << endl;
            sqlite3_free(errMsg);
        } else {
            cout << BOLDGREEN << "Registration successful!\n" << RESET;
        }
    }

    bool loginUser(bool isAdmin = false) {
        string uname, pass;
        cout << CYAN << "Enter username: " << RESET;
        cin >> uname;
        cout << CYAN << "Enter password: " << RESET;
        cin >> pass;

        string table = isAdmin ? "adminusers" : "users";
        string sql = "SELECT * FROM " + table + " WHERE username = '" + uname + "' AND password = '" + pass + "';";
        bool success = false;

        sqlite3_exec(db, sql.c_str(), [](void *successPtr, int, char **, char **) -> int {
            *(bool*)successPtr = true;
            return 0;
        }, &success, &errMsg);

        if (success) {
            cout << GREEN << "Login successful!\n" << RESET;
            return true;
        } else {
            cout << RED << "Invalid credentials!\n" << RESET;
            return false;
        }
    }

    void fileComplaint() {
        string category, subCategory, message;
        cout << YELLOW << "Enter category: " << RESET;
        cin.ignore();
        getline(cin, category);
        cout << YELLOW << "Enter sub-category: " << RESET;
        getline(cin, subCategory);
        cout << YELLOW << "Enter complaint message: " << RESET;
        getline(cin, message);

        string sql = "INSERT INTO complaints (category, subCategory, message) VALUES ('" + category + "', '" + subCategory + "', '" + message + "');";
        if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
            cout << RED << "Error: " << errMsg << RESET << endl;
            sqlite3_free(errMsg);
        } else {
            cout << BOLDGREEN << "Complaint filed successfully!\n" << RESET;
        }
    }

    void exportComplaintsToCSV() {
        ofstream file("complaints_export.csv");
        if (!file.is_open()) {
            cout << RED << "Failed to create CSV file.\n" << RESET;
            return;
        }
    
        // Write header with the column names in order
        file << "complaint_id,category,subCategory,message\n";
    
        // Explicitly specify the columns to get the correct order.
        string sql = "SELECT complaint_id, category, subCategory, message FROM complaints;";
        auto callback = [](void *data, int argc, char **argv, char **colName) -> int {
            ofstream *f = static_cast<ofstream *>(data);
            for (int i = 0; i < argc; i++) {
                *f << (argv[i] ? argv[i] : "") << (i < argc - 1 ? "," : "\n");
            }
            return 0;
        };
    
        if (sqlite3_exec(db, sql.c_str(), callback, &file, &errMsg) != SQLITE_OK) {
            cout << RED << "Export failed: " << errMsg << RESET << endl;
            sqlite3_free(errMsg);
        } else {
            cout << BOLDGREEN << "Complaints exported to 'complaints_export.csv'!\n" << RESET;
        }
    
        file.close();
    }
    
    

private:
    sqlite3 *db;
    char *errMsg;

    void createTables() {
        string sqlUsers = "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT);";
        string sqlAdmins = "CREATE TABLE IF NOT EXISTS adminusers (username TEXT PRIMARY KEY, password TEXT);";
        string sqlComplaints = "CREATE TABLE IF NOT EXISTS complaints ("
        "complaint_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "category TEXT, "
        "subCategory TEXT, "
        "message TEXT);";

        sqlite3_exec(db, sqlUsers.c_str(), 0, 0, &errMsg);
        sqlite3_exec(db, sqlAdmins.c_str(), 0, 0, &errMsg);
        sqlite3_exec(db, sqlComplaints.c_str(), 0, 0, &errMsg);
    }
};

int main() {
    ComplaintBox cb;
    int choice;

    do {
        cout << BOLDVIOLET << "\n==== Complaint Box Menu ====\n" << RESET;
        cout << CYAN << "1. Register User\n"
             << "2. Register Admin\n"
             << "3. User Login\n"
             << "4. Admin Login\n"
             << "5. File Complaint\n"
             << "6. Export Complaints to CSV\n"
             << "7. Exit\n" << RESET;
            
        cout << WHITE << "Choice: " << RESET;
        cin >> choice;

        switch (choice) {
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
                cout << BOLDGREEN << "Exiting..." << RESET << endl;
                break;
            default:
                cout << BOLDRED << "Invalid choice!\n" << RESET;
        }
    } while (choice != 7);

    return 0;
}
