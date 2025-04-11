#ifndef COMPLAINTBOX_H
#define COMPLAINTBOX_H

#include <string>
#include "sqlite3.h"
using namespace std;

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
    ComplaintBox();
    ~ComplaintBox();

    void registerUser(bool isAdmin = false);
    bool loginUser(bool isAdmin = false);
    void fileComplaint();
    void exportComplaintsToCSV();
    void searchComplaints();
    void updateComplaintStatus(int complaint_id, const string& new_status);
    bool isAdminLoggedIn() const { return admin_logged_in; }

private:
    sqlite3 *db;
    char *errMsg;
    bool admin_logged_in = false;
    void createTables();
};

#endif 
