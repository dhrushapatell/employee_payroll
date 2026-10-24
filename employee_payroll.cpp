#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// Abstraction and encapsulation
class Employee {
    static int nextId;
    int id;
    string name;
    double hours;

protected:
    double workedHours() const { return hours; }
    virtual double basePay() const = 0;
    void commonDetails() const {
        cout << "ID: " << id << "\nName: " << name << "\nHours Worked: " << hours
             << "\nBonus Rate: " << bonusRate() * 100 << "%\nBonus: Rs. " << bonus() << '\n';
    }

public:
    Employee(const string& name, double hours) : id(nextId++), name(name), hours(hours) {}
    virtual ~Employee() {}

    string getName() const { return name; }
    double getHours() const { return hours; }
    double bonusRate() const { return hours <= 15 ? .05 : hours <= 31 ? .10 : hours <= 47 ? .15 : hours <= 63 ? .20 : .25; }
    double bonus() const { return basePay() * bonusRate(); }

    virtual double grossPay() const = 0;
    virtual void displayPaySlip() const = 0;
};

int Employee::nextId = 1001;

// Inheritance
class FullTimeEmployee : public Employee {
    double salary;

public:
    FullTimeEmployee(const string& name, double hours, double salary)
        : Employee(name, hours), salary(salary) {}

    double basePay() const override { return salary; }
    double grossPay() const override { return basePay() + bonus(); }
    void displayPaySlip() const override {
        cout << "\n========== Full-Time Employee ==========\n";
        commonDetails();
        cout << "Monthly Salary: Rs. " << salary << "\nFinal Pay: Rs. " << grossPay() << '\n';
    }
};

class PartTimeEmployee : public Employee {
    double rate;

public:
    PartTimeEmployee(const string& name, double hours, double rate)
        : Employee(name, hours), rate(rate) {}

    double basePay() const override { return workedHours() * rate; }
    double grossPay() const override { return basePay() + bonus(); }
    void displayPaySlip() const override {
        cout << "\n========== Part-Time Employee ==========\n";
        commonDetails();
        cout << "Hourly Rate: Rs. " << rate << "\nFinal Pay: Rs. " << grossPay() << '\n';
    }
};

// Polymorphism through Employee pointers
class Payroll {
    static const int MAX_EMPLOYEES = 100;
    Employee* employees[MAX_EMPLOYEES];
    int count;

public:
    Payroll() : count(0) {}
    ~Payroll() { for (int i = 0; i < count; ++i) delete employees[i]; }

    bool add(Employee* employee) {
        if (count == MAX_EMPLOYEES) { delete employee; return false; }
        employees[count++] = employee;
        return true;
    }

    void displayPaySlips() const {
        if (!count) { cout << "\nNo employees found.\n"; return; }
        for (int i = 0; i < count; ++i) employees[i]->displayPaySlip();
    }

    void displayHoursAndBonuses() const {
        if (!count) { cout << "\nNo employees found.\n"; return; }
        cout << "\n========== Hours and Bonuses ==========\n";
        for (int i = 0; i < count; ++i)
            cout << employees[i]->getName() << " | Hours: " << employees[i]->getHours()
                 << " | Bonus: Rs. " << employees[i]->bonus() << '\n';
    }
};

void menu() {
    cout << "\n========== Employee Payroll System =========="
         << "\n1. Add Full-Time Employee\n2. Add Part-Time Employee"
         << "\n3. Display All Pay Slips\n4. Display Hours Worked and Bonuses\n5. Exit\n";
}

int main() {
    Payroll payroll;
    cout << fixed << setprecision(2);

    for (int choice; ; ) {
        menu();
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 5) break;

        if (choice == 1 || choice == 2) {
            string name; double hours, pay;
            cout << "Name: "; getline(cin >> ws, name);
            cout << "Hours worked: "; cin >> hours;
            cout << (choice == 1 ? "Monthly salary (Rs.): " : "Hourly rate (Rs.): "); cin >> pay;

            bool added = choice == 1
                ? payroll.add(new FullTimeEmployee(name, hours, pay))
                : payroll.add(new PartTimeEmployee(name, hours, pay));
            cout << (added ? "Employee added successfully.\n" : "Employee limit reached.\n");
        } else if (choice == 3) {
            payroll.displayPaySlips();
        } else if (choice == 4) {
            payroll.displayHoursAndBonuses();
        } else {
            cout << "Invalid choice.\n";
        }
    }
    cout << "Exiting program...\n";
}
