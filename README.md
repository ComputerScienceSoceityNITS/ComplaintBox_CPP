# 🛠️ Complaint Box System (C++ + SQLite + Qt)

A lightweight and extensible complaint management system built in C++ with SQLite for the backend. Includes CLI and planned Qt GUI support. Built for learning, collaboration, and open-source contribution.

---

## 📦 Getting Started

### ⚙️ Build Instructions

#### 🔧 Option 1: Using Makefile

```bash
make
./complaintbox
```

#### 🔧 Option 2: Using g++

```bash
g++ -o complaintbox main.cpp -lsqlite3
./complaintbox
```

---

## 🗃️ Database Setup

You do not need to set up the database manually.  
The database (`complaints.db`) is created automatically on first run, along with all required tables.

✅ Auto-generates:

- Users Table  
- Admins Table  
- Complaints Table

`.db` file is excluded from Git using `.gitignore` for security and clean versioning.

---

## 🧩 Project Structure

```
📁 ComplaintBox
├── main.cpp               # Entry point
├── .gitignore             # Ignored files
├── README.md              # Project documentation
├── ISSUES.md              # Open source task tracker
└── LICENSE                # Open source license
```

---

## 🔧 How to Contribute

We welcome all contributions!

- Fork this repository  
- Clone your fork:

  ```bash
  git clone https://github.com/yourusername/complaint-box.git
  ```

- Create a new branch:

  ```bash
  git checkout -b feature-name
  ```

- Make your changes and commit:

  ```bash
  git commit -m "Added feature: XYZ"
  ```

- Push your branch:

  ```bash
  git push origin feature-name
  ```

- Open a Pull Request and link related issues
