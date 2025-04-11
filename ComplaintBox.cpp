#include "ComplaintBox.h"
#include "bcrypt/BCrypt.hpp"
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

    // Hashing the password using bcrypt
    string hashedPassword = BCrypt::generateHash(pass);

    string table = isAdmin ? "adminusers" : "users";
    string sql = "INSERT INTO " + table + " (username, password) VALUES ('" + uname + "', '" + hashedPassword + "');";



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
    string sql = "SELECT password FROM " + table + " WHERE username = '" + uname + "';";
    string storedHash;
    bool found = false;

    sqlite3_exec(db, sql.c_str(), [](void *data, int argc, char **argv, char **) -> int {
        if (argc > 0 && argv[0]) {
            *((string*)data) = argv[0];
            return 0;
        }
        return 1;
    }, &storedHash, &errMsg);

    // Validating password using bcrypt
    bool success = BCrypt::validatePassword(pass, storedHash);


    if (success) {
        cout << GREEN << "Login successful!\n" << RESET;
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

    string sql = "INSERT INTO complaints (category, subCategory, message) VALUES ('" + category + "', '" + subCategory + "', '" + message + "');";
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        cout << RED << "Error: " << errMsg << RESET << endl;
        sqlite3_free(errMsg);
    } else {
        cout << BOLDGREEN << "Complaint filed successfully!\n" << RESET;
    }
}

void ComplaintBox::exportComplaintsToCSV() {
    ofstream file("complaints_export.csv");
    if (!file.is_open()) {
        cout << RED << "Failed to create CSV file.\n" << RESET;
        return;
    }
    
    file << "complaint_id,category,subCategory,message\n"; 
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

void ComplaintBox::searchComplaints() {
    string keyword;
    cout << YELLOW << "Enter keyword to search for: " << RESET;
    cin.ignore();
    getline(cin, keyword);

    string sql = "SELECT complaint_id, category, subCategory, message FROM complaints "
                 "WHERE category LIKE '%" + keyword + "%' OR "
                 "subCategory LIKE '%" + keyword + "%' OR "
                 "message LIKE '%" + keyword + "%';";
    
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
