#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class User {
protected:
    string username, password;
public:
    virtual void registerUser() = 0;
    virtual bool loginUser() = 0;
};

class Student : public User {
public:
    void registerUser() override {
        cout << "\nRegister as Student";
        cout << "\nEnter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        ofstream file("student_data.txt", ios::app);
        file << username << " " << password << endl;
        file.close();
        cout << "Registration successful!\n";
    }
    
    bool loginUser() override {
        string user, pass;
        cout << "\nLogin as Student";
        cout << "\nEnter username: ";
        cin >> user;
        cout << "Enter password: ";
        cin >> pass;
        ifstream file("student_data.txt");
        string u, p;
        while (file >> u >> p) {
            if (u == user && p == pass) {
                cout << "Login successful!\n";
                return true;
            }
        }
        cout << "Invalid credentials!\n";
        return false;
    }
};

class Employee : public User {
public:
    void registerUser() override {
        cout << "\nRegister as Employee";
        cout << "\nEnter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        ofstream file("employee_data.txt", ios::app);
        file << username << " " << password << endl;
        file.close();
        cout << "Registration successful!\n";
    }
    
    bool loginUser() override {
        string user, pass;
        cout << "\nLogin as Employee";
        cout << "\nEnter username: ";
        cin >> user;
        cout << "Enter password: ";
        cin >> pass;
        ifstream file("employee_data.txt");
        string u, p;
        while (file >> u >> p) {
            if (u == user && p == pass) {
                cout << "Login successful!\n";
                return true;
            }
        }
        cout << "Invalid credentials!\n";
        return false;
    }
};

int main() {
    int choice, action;
    cout << "Welcome!\nSelect user type:";
    cout << "\n1. Student\n2. Administrative Employee\nChoice: ";
    cin >> choice;

    User* user = nullptr;
    if (choice == 1)
        user = new Student();
    else if (choice == 2)
        user = new Employee();
    else {
        cout << "Invalid choice!";
        return 0;
    }

    cout << "\n1. Register\n2. Login\nChoice: ";
    cin >> action;
    if (action == 1)
        user->registerUser();
    else if (action == 2)
        user->loginUser();
    else
        cout << "Invalid action!";

    delete user;
    return 0;
}