#include "ComplaintBox.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h> 
#include <sqlite3.h>

using namespace std;

ComplaintBox::ComplaintBox() {
    sqlite3_open("complaints.db", &db);
    createTables();
}

ComplaintBox::~ComplaintBox() {
    sqlite3_close(db);
}

void ComplaintBox::createTables() {
    const char* sqlUsers = "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT);";
    const char* sqlAdmins = "CREATE TABLE IF NOT EXISTS adminusers (username TEXT PRIMARY KEY, password TEXT);";
    const char* sqlComplaints = "CREATE TABLE IF NOT EXISTS complaints ("
                                "complaint_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "category TEXT, "
                                "subCategory TEXT, "
                                "message TEXT);";

    sqlite3_exec(db, sqlUsers, 0, 0, &errMsg);
    sqlite3_exec(db, sqlAdmins, 0, 0, &errMsg);
    sqlite3_exec(db, sqlComplaints, 0, 0, &errMsg);
}

void ComplaintBox::registerUser(bool isAdmin) {
    string uname, pass;
    cout << PURPLE << "Enter username: " << RESET;
    cin >> uname;

    sqlite3_stmt* stmt;
    string checkSql = "SELECT 1 FROM users WHERE username = ?";
    sqlite3_prepare_v2(db, checkSql.c_str(), -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, uname.c_str(), -1, SQLITE_STATIC);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) exists = true;
    sqlite3_finalize(stmt);

    if (exists) {
        cout << BOLDRED << "Username already exists. Please choose a different Username.\n" << RESET;
        return;
    }

    cout << PURPLE << "Enter password: " << RESET;
    cin >> pass;

    string table = isAdmin ? "adminusers" : "users";
    string insertSql = "INSERT INTO " + table + " (username, password) VALUES (?, ?);";

    sqlite3_prepare_v2(db, insertSql.c_str(), -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, uname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << RED << "Error: " << sqlite3_errmsg(db) << RESET << endl;
    } else {
        cout << BOLDGREEN << "Registration successful!\n" << RESET;
    }
    sqlite3_finalize(stmt);
}

bool ComplaintBox::loginUser(bool isAdmin) {
    string uname, pass;
    cout << CYAN << "Enter username: " << RESET;
    cin >> uname;
    cout << CYAN << "Enter password: " << RESET;
    cin >> pass;

    string table = isAdmin ? "adminusers" : "users";
    string sql = "SELECT 1 FROM " + table + " WHERE username = ? AND password = ?;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, uname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) success = true;
    sqlite3_finalize(stmt);

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

    sqlite3_stmt* stmt;
    string sql = "INSERT INTO complaints (category, subCategory, message) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, subCategory.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, message.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << RED << "Error: " << sqlite3_errmsg(db) << RESET << endl;
    } else {
        cout << BOLDGREEN << "Complaint filed successfully!\n" << RESET;
    }
    sqlite3_finalize(stmt);
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
                 "WHERE category LIKE ? OR subCategory LIKE ? OR message LIKE ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    string pattern = "%" + keyword + "%";
    for (int i = 1; i <= 3; ++i) {
        sqlite3_bind_text(stmt, i, pattern.c_str(), -1, SQLITE_STATIC);
    }

    cout << CYAN << "\nSearch Results:\n" << RESET;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "complaint_id: " << sqlite3_column_text(stmt, 0) << "  "
             << "category: " << sqlite3_column_text(stmt, 1) << "  "
             << "subCategory: " << sqlite3_column_text(stmt, 2) << "  "
             << "message: " << sqlite3_column_text(stmt, 3) << endl;
    }
    sqlite3_finalize(stmt);
}
