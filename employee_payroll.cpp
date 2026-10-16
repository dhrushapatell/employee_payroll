#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

// Abstract base class
class Employee {
private:
    int id;
    string name;
    double hoursWorked;
    double taxRate;

protected:
    void printCommonDetails() const {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Hours Worked: " << hoursWorked << endl;
        cout << "Bonus: Rs. " << calculateBonus() << endl;
    }

public:
    static constexpr double REGULAR_HOURS = 160.0;
    static constexpr double BONUS_PER_EXTRA_HOUR = 10.0;

    Employee(int employeeId, const string& employeeName,
             double hours, double employeeTaxRate)
        : id(employeeId),
          name(employeeName),
          hoursWorked(hours),
          taxRate(employeeTaxRate) {}

    virtual ~Employee() {}

    string getName() const {
        return name;
    }

    double getHoursWorked() const {
        return hoursWorked;
    }

    double calculateBonus() const {
        if (hoursWorked > REGULAR_HOURS) {
            return (hoursWorked - REGULAR_HOURS)
                   * BONUS_PER_EXTRA_HOUR;
        }

        return 0;
    }

    double calculateTax() const {
        return calculateGrossPay() * taxRate;
    }

    double calculateNetPay() const {
        return calculateGrossPay() - calculateTax();
    }

    virtual double calculateGrossPay() const = 0;
    virtual void displayPaySlip() const = 0;
};

// Full-time employee
class FullTimeEmployee : public Employee {
private:
    double monthlySalary;

public:
    FullTimeEmployee(int id, const string& name,
                     double salary, double hours)
        : Employee(id, name, hours, 0.10),
          monthlySalary(salary) {}

    double calculateGrossPay() const override {
        return monthlySalary + calculateBonus();
    }

    void displayPaySlip() const override {
        cout << "\n========== Full-Time Employee ==========\n";

        printCommonDetails();

        cout << "Monthly Salary: Rs. " << monthlySalary << endl;
        cout << "Gross Pay: Rs. " << calculateGrossPay() << endl;
        cout << "Tax: Rs. " << calculateTax() << endl;
        cout << "Net Pay: Rs. " << calculateNetPay() << endl;
    }
};

// Part-time employee
class PartTimeEmployee : public Employee {
private:
    double hourlyRate;

public:
    PartTimeEmployee(int id, const string& name,
                     double hours, double rate)
        : Employee(id, name, hours, 0.05),
          hourlyRate(rate) {}

    double calculateGrossPay() const override {
        return (getHoursWorked() * hourlyRate) + calculateBonus();
    }

    void displayPaySlip() const override {
        cout << "\n========== Part-Time Employee ==========\n";

        printCommonDetails();

        cout << "Hourly Rate: Rs. " << hourlyRate << endl;
        cout << "Gross Pay: Rs. " << calculateGrossPay() << endl;
        cout << "Tax: Rs. " << calculateTax() << endl;
        cout << "Net Pay: Rs. " << calculateNetPay() << endl;
    }
};

// Payroll manager
class Payroll {
private:
    static const int MAX_EMPLOYEES = 100;

    Employee* employees[MAX_EMPLOYEES];
    int employeeCount;

public:
    Payroll() : employeeCount(0) {}

    ~Payroll() {
        for (int i = 0; i < employeeCount; i++) {
            delete employees[i];
        }
    }

    bool addEmployee(Employee* employee) {
        if (employeeCount >= MAX_EMPLOYEES) {
            delete employee;
            return false;
        }

        employees[employeeCount] = employee;
        employeeCount++;

        return true;
    }

    void displayAllPaySlips() const {
        if (employeeCount == 0) {
            cout << "\nNo employees found.\n";
            return;
        }

        for (int i = 0; i < employeeCount; i++) {
            employees[i]->displayPaySlip();
        }
    }

    void displayHoursAndBonuses() const {
        if (employeeCount == 0) {
            cout << "\nNo employees found.\n";
            return;
        }

        cout << "\n========== Hours and Bonuses ==========\n";

        for (int i = 0; i < employeeCount; i++) {
            cout << "Employee: " << employees[i]->getName() << endl;
            cout << "Hours Worked: "
                 << employees[i]->getHoursWorked() << endl;
            cout << "Bonus: Rs. "
                 << employees[i]->calculateBonus() << endl;
            cout << "---------------------------------------\n";
        }
    }
};

int main() {
    Payroll payroll;
    int choice;

    cout << fixed << setprecision(2);

    do {
        cout << "\n========== Employee Payroll System ==========\n";
        cout << "1. Add Full-Time Employee\n";
        cout << "2. Add Part-Time Employee\n";
        cout << "4. Display All Pay Slips\n";
        cout << "5. Display Hours Worked and Bonuses\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string name;
            double salary;
            double hours;

            cout << "Enter employee ID: ";
            cin >> id;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter employee name: ";
            getline(cin, name);

            cout << "Enter monthly salary in rupees: ";
            cin >> salary;

            cout << "Enter hours worked: ";
            cin >> hours;

            payroll.addEmployee(
                new FullTimeEmployee(id, name, salary, hours)
            );

            cout << "Full-time employee added successfully.\n";
        }
        else if (choice == 2) {
            int id;
            string name;
            double hours;
            double rate;

            cout << "Enter employee ID: ";
            cin >> id;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter employee name: ";
            getline(cin, name);

            cout << "Enter hours worked: ";
            cin >> hours;

            cout << "Enter hourly rate in rupees: ";
            cin >> rate;

            payroll.addEmployee(
                new PartTimeEmployee(id, name, hours, rate)
            );

            cout << "Part-time employee added successfully.\n";
        }
        else if (choice == 4) {
            payroll.displayAllPaySlips();
        }
        else if (choice == 5) {
            payroll.displayHoursAndBonuses();
        }
        else if (choice == 0) {
            cout << "Exiting program...\n";
        }
        else {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}
