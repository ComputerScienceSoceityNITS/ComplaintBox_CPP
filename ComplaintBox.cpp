#include "ComplaintBox.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h> 

using namespace std;

ComplaintBox::ComplaintBox() {
    sqlite3_open("complaints.db", &db);
    createTables();
}

ComplaintBox::~ComplaintBox() {
    sqlite3_close(db);
}

void ComplaintBox::createTables() {
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

void ComplaintBox::registerUser(bool isAdmin) {
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

bool ComplaintBox::loginUser(bool isAdmin) {
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
        admin_logged_in = isAdmin;  
        return true;
    } else {
        cout << RED << "Invalid credentials!\n" << RESET;
        return false;
    }
}


void ComplaintBox::fileComplaint() {
    string category, subCategory, message;
    cout << YELLOW << "Enter category: " << RESET;
    cin.ignore();
    getline(cin, category);
    cout << YELLOW << "Enter sub-category: " << RESET;
    getline(cin, subCategory);
    cout << YELLOW << "Enter complaint message: " << RESET;
    getline(cin, message);

    string sql = "INSERT INTO complaints (category, subCategory, message, status) VALUES ('" 
                + category + "', '" + subCategory + "', '" + message + "', 'Pending');";

    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        cout << RED << "Error: " << errMsg << RESET << endl;
        sqlite3_free(errMsg);
    } else {
        cout << BOLDGREEN << "Complaint filed successfully with status 'Pending'!\n" << RESET;
    }
}

void ComplaintBox::exportComplaintsToCSV() {
    ofstream file("complaints_export.csv");
    if (!file.is_open()) {
        cout << RED << "Failed to create CSV file.\n" << RESET;
        return;
    }
    
    file << "complaint_id,category,subCategory,message,status\n"; 
    string sql = "SELECT complaint_id, category, subCategory, message, status FROM complaints;";
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

void ComplaintBox::searchComplaints() {
    string keyword;
    cout << YELLOW << "Enter keyword to search for: " << RESET;
    cin.ignore();
    getline(cin, keyword);

    string sql = "SELECT complaint_id, category, subCategory, message, status FROM complaints "
                 "WHERE category LIKE '%" + keyword + "%' OR "
                 "subCategory LIKE '%" + keyword + "%' OR "
                 "message LIKE '%" + keyword + "%' OR "
                 "status LIKE '%" + keyword + "%';";  
    
    cout << CYAN << "\nSearch Results:\n" << RESET;
    auto callback = [](void *data, int argc, char **argv, char **colName) -> int {
        for (int i = 0; i < argc; i++) {
            cout << (colName[i] ? colName[i] : "") << ": " << (argv[i] ? argv[i] : "NULL") << "  ";
        }
        cout << "\n";
        return 0;
    };

    if (sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg) != SQLITE_OK) {
        cout << RED << "Search failed: " << errMsg << RESET << endl;
        sqlite3_free(errMsg);
    }
}


void ComplaintBox::updateComplaintStatus(int complaint_id, const std::string& new_status) {
    if (!admin_logged_in) {
        cout << RED << "Only admins can update complaint status.\n" << RESET;
        return;
    }

    sqlite3_stmt* stmt;
    string sql = "UPDATE complaints SET status = ? WHERE complaint_id = ?";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, new_status.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, complaint_id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            cout << GREEN << "Status updated successfully.\n" << RESET;
        } else {
            cerr << RED << "Failed to update status.\n" << RESET;
        }
    } else {
        cerr << RED << "SQL Prepare Failed: " << sqlite3_errmsg(db) << "\n" << RESET;
    }

    sqlite3_finalize(stmt);
}