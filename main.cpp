#include<bits/stdc++.h>
#include "sqlite3.h"
using namespace std;

class ComplaintBox
{
public:
    ComplaintBox()
    {
        sqlite3_open("complaints.db", &db);
        createTables();
    }

    ~ComplaintBox()
    {
        sqlite3_close(db);
    }

    void registerUser(bool isAdmin = false)
    {
        string uname, pass;
        cout << "Enter username: ";
        cin >> uname;

        // --To check duplicate Username while Registering--

        string checkSql = "SELECT * FROM users WHERE username='" + uname + "';";
        bool exists = false;

        sqlite3_exec(db, checkSql.c_str(), [](void *data, int, char **, char **) -> int
                     {
            *(bool *)data = true;    // same func as in loginUser
            return 0; }, &exists, &errMsg);
        
        if (exists)
        {
            cout << "Username already exists. Please choose a different Username" << endl;
            return;
        }
        
        // if not exists then continue
        cout << "Enter password: ";
        cin >> pass;

        string table = isAdmin ? "adminusers" : "users";
        string sql = "INSERT INTO " + table + " (username, password) VALUES ('" + uname + "', '" + pass + "');";

        if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK)
        {
            cout << "Error: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
        else
        {
            cout << "Registration successful!\n";
        }
    }

    bool loginUser(bool isAdmin = false)
    {
        string uname, pass;
        cout << "Enter username: ";
        cin >> uname;
        cout << "Enter password: ";
        cin >> pass;

        string table = isAdmin ? "adminusers" : "users";
        string sql = "SELECT * FROM " + table + " WHERE username = '" + uname + "' AND password = '" + pass + "';";
        bool success = false;

        sqlite3_exec(db, sql.c_str(), [](void *successPtr, int argc, char **argv, char **colName) -> int
                     {
            *(bool*)successPtr = true;
            return 0; }, &success, &errMsg);

        if (success)
        {
            cout << "Login successful!\n";
            return true;
        }
        else
        {
            cout << "Invalid credentials!\n";
            return false;
        }
    }

    void fileComplaint()
    {
        string category, subCategory, message;
        cout << "Enter category: ";
        cin.ignore();
        getline(cin, category);
        cout << "Enter sub-category: ";
        getline(cin, subCategory);
        cout << "Enter complaint message: ";
        getline(cin, message);

        string sql = "INSERT INTO complaints (category, subCategory, message) VALUES ('" + category + "', '" + subCategory + "', '" + message + "');";
        if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK)
        {
            cout << "Error: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
        else
        {
            cout << "Complaint filed successfully!\n";
        }
    }

private:
    sqlite3 *db;
    char *errMsg;

    void createTables()
    {
        string sqlUsers = "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT);";
        string sqlAdmins = "CREATE TABLE IF NOT EXISTS adminusers (username TEXT PRIMARY KEY, password TEXT);";
        string sqlComplaints = "CREATE TABLE IF NOT EXISTS complaints (category TEXT, subCategory TEXT, message TEXT);";

        sqlite3_exec(db, sqlUsers.c_str(), 0, 0, &errMsg);
        sqlite3_exec(db, sqlAdmins.c_str(), 0, 0, &errMsg);
        sqlite3_exec(db, sqlComplaints.c_str(), 0, 0, &errMsg);
    }
};

int main()
{
    ComplaintBox cb;
    int choice;

    do
    {
        cout << "\n1. Register User\n2. Register Admin\n3. User Login\n4. Admin Login\n5. File Complaint\n6. Exit\nChoice: ";
        cin >> choice;

        switch (choice)
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
    } while (choice != 6);

    return 0;
}
