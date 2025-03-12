#include <iostream>
#include <vector>
#include <algorithm> 
#include <cctype>    
#include <limits>   
#include <string>  

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"


class Employee {
private:  
    string ID;
    string name;
    double salary;

public:
    Employee(string empID, string empName, double empSalary)
        : ID(empID), name(empName), salary(empSalary) {}

    virtual double calculateSalary() = 0; 
    string getID() { return ID; }
    string getName() { return name; }
    double getSalary() { return salary; }

    virtual void displayDetails() = 0; 
};

// *******************************Full-Time Employee*******************************************************************
class FullTime : public Employee {
public:
    FullTime(string empID, string empName, double empSalary)
        : Employee(empID, empName, empSalary) {}

    double calculateSalary() override {
        return getSalary();
    }

    void displayDetails() override {
        cout << MAGENTA << "Employee: " << getName() << " (ID: " << getID() << ")\n" << RESET;
        cout << YELLOW << "Fixed Monthly Salary: $" << getSalary() << "\n\n" << RESET;
    }
};

// *************************************Part-Time Employee*********************************************************
class PartTime : public Employee {
private:
    int hoursWorked;

public:
    PartTime(string empID, string empName, double empSalary, int empHoursWorked)
        : Employee(empID, empName, empSalary), hoursWorked(empHoursWorked) {}

    double calculateSalary() override {
        return getSalary() * hoursWorked;
    }

    void displayDetails() override {
        cout << MAGENTA << "Employee: " << getName() << " (ID: " << getID() << ")\n" << RESET;
        cout << "Hourly Wage: $" << getSalary() << "\n";
        cout << "Hours Worked: " << hoursWorked << "\n";
        cout << YELLOW << "Total Salary: $" << calculateSalary() << "\n\n" << RESET;
    }
};

// ************************************Contractual Employee*******************************************
class Contractual : public Employee {
private:
    int numProjects;

public:
    Contractual(string empID, string empName, double empSalary, int empNumProjects)
        : Employee(empID, empName, empSalary), numProjects(empNumProjects) {}

    double calculateSalary() override {
        return getSalary() * numProjects;
    }

    void displayDetails() override {
        cout << MAGENTA << "Employee: " << getName() << " (ID: " << getID() << ")\n" << RESET;
        cout << "Contract Payment Per Project: $" << getSalary() << "\n";
        cout << "Projects Completed: " << numProjects << "\n";
        cout << YELLOW << "Total Salary: $" << calculateSalary() << "\n\n" << RESET;
    }
};



string toUpperCase(const string &str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}


bool isUniqueID(const string &id, const vector<Employee *> &employees) {
    string upperID = toUpperCase(id); 

    for (size_t i = 0; i < employees.size(); i++) {
        if (toUpperCase(employees[i]->getID()) == upperID) { 
            return false;
        }
    }
    return true;
}


bool isValidName(const string &name) {
    bool hasLetter = false; 

    for (char ch : name) {
        if (isalpha(ch)) {
            hasLetter = true; 
        } 
        else if (ch != ' ') { 
            return false; 
        }
    }

    return hasLetter; 
}


bool isValidID(const string &id) {
    for (char ch : id) {
        if (!isalnum(ch)) { 
            return false;
        }
    }
    return true;
}


double getValidDoubleInput(string prompt) {
    string input;
    double value;
    bool validInput = false;  

    while (!validInput) {
        cout << prompt;
        getline(cin, input);  

        bool isValid = !input.empty();
        int dotCount = 0;

        for (char c : input) {
            if (!isdigit(c)) {
                if (c == '.' && dotCount == 0) {
                    dotCount++;  
                } else {
                    isValid = false; 
                    break;
                }
            }
        }


        if (isValid) {
            try {
                size_t pos;
                value = stod(input, &pos);

                if (pos == input.length() && value > 0) {
                    validInput = true;
                } else {
                    isValid = false;
                }
            } catch (...) {
                isValid = false;
            }
        }

        if (!validInput) {
            cout << RED << "Invalid input! Please enter a valid positive number.\n" << RESET;
        }
    }

    return value;
}






// Function to get valid positive integer input
int getValidIntInput(string prompt) {
    string input;
    int value;
    bool validInput = false;

    while (!validInput) {
        cout << prompt;
        getline(cin, input);


        if (input.find(' ') != string::npos) {
            cout << RED << "Invalid input! No spaces allowed.\n" << RESET;
            continue;
        }

        bool isValid = all_of(input.begin(), input.end(), ::isdigit);

        if (isValid) {
            try {
                value = stoi(input);
                if (value > 0) {
                    validInput = true; 
                }
            } catch (...) {
                isValid = false;
            }
        }

        if (!validInput) {
            cout << RED << "Invalid input! Please enter a positive whole number.\n" << RESET;
        }
    }

    return value;
}



// Function to add an employee
void addEmployee(vector<Employee *> &employees, int type) {
    char choice;
    
    do {
        string ID, name;
        double salary;
        int hours, projects;

        // Validate employee ID
        do {
            cout << "Enter Employee ID (letters and numbers only): ";
            cin >> ID;

            if (!isValidID(ID)) {
                cout << RED << "Invalid ID! The ID should only contain letters and numbers.\n" << RESET;
            } else if (!isUniqueID(ID, employees)) {
                cout << RED << "Duplicate ID! Enter a unique ID.\n" << RESET;
            }

        } while (!isValidID(ID) || !isUniqueID(ID, employees));

        // Validate employee name
        cin.ignore();
        do {
            cout << "Enter Employee Name (letters only): ";
            getline(cin, name);

            if (!isValidName(name)) {
                cout << RED << "Invalid Name! The name should only contain letters and spaces.\n" << RESET;
            }

        } while (!isValidName(name));

        salary = getValidDoubleInput("Enter Salary: ");

        switch (type) {
            case 1:
                employees.push_back(new FullTime(ID, name, salary));
                break;
            case 2:
                hours = getValidIntInput("Enter Hours Worked: ");
                employees.push_back(new PartTime(ID, name, salary, hours));
                break;
            case 3:
                projects = getValidIntInput("Enter Number of Projects: ");
                employees.push_back(new Contractual(ID, name, salary, projects));
                break;
        }

        // Ask user if they want to add another employee of the same type
        do {
            cout << "Do you want to add another employee of the same type? (Y/N): ";
            cin >> choice;
            choice = toupper(choice);

            if (choice != 'Y' && choice != 'N') {
                cout << RED << "Invalid input! Please enter 'Y' for Yes or 'N' for No.\n" << RESET;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        } while (choice != 'Y' && choice != 'N');

    } while (choice == 'Y'); // Repeat if the user chooses 'Y'
}
	

// Function to display payroll report
void displayPayroll(const vector<Employee *> &employees) {
    if (employees.empty()) {
        cout << "No employees to display!\n";
        return;
    }

    cout << "\n------ " << RED << "Employee Payroll Report" << RESET << " ------\n\n";

    for (size_t i = 0; i < employees.size(); i++) {
        employees[i]->displayDetails();
    }
}

// Function to delete dynamically allocated memory
void cleanup(vector<Employee *> &employees) {
    for (size_t i = 0; i < employees.size(); i++) {
        delete employees[i];
    }
    employees.clear();
}

// Function to validate and return a valid integer input for the menu
int getMenuChoice() {
    string input;
    int choice;
    bool validInput = false;

    while (!validInput) {
        cout << "\nMenu\n"
             << "1 - Full-time Employee\n"
             << "2 - Part-time Employee\n"
             << "3 - Contractual Employee\n"
             << "4 - Display Payroll Report\n"
             << "5 - Exit\n"
             << "Enter your choice: ";
        
        getline(cin, input);

        bool isValid = true;
        for (char c : input) {
            if (!isdigit(c)) { 
                isValid = false;
                break;
            }
        }

        if (isValid && input.size() == 1) {
            choice = input[0] - '0'; 
            if (choice >= 1 && choice <= 5) {
                validInput = true; 
            }
        }

        if (!validInput) {
            cout << RED << "Invalid input! Please enter a number between 1 and 5.\n" << RESET;
        }
    }

    return choice;
}


// Main function with menu
int main() {
    vector<Employee *> employees;
    int choice;

    do {
        choice = getMenuChoice();

        switch (choice) {
        case 1:
        case 2:
        case 3:
            addEmployee(employees, choice);
            break;
        case 4:
            displayPayroll(employees);
            break;
        case 5:
            cout << "Exiting program...\n";
            break;
        default:
            break;
        }
    } while (choice != 5);

    cleanup(employees);
    return 0;
}
