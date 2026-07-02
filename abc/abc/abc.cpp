#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;
const int SUBJECTS = 6;
const int MAX_STUDENTS = 100;
const string FIXED_PASSWORD = "tehreem123";
/* ================= STRUCT ================= */
struct Student {
    int id;
    string name;
    int marks[SUBJECTS];
    float gpa;
    string performance;
    string status;
    string dues;
    string session;
    string field;
    string section;
};

/* ================= ARRAYS ================= */
string sessions[4] = { "2022","2023","2024","2025" };
string fields[4] = { "CS","SE","AI","IST" };
string sections[3] = { "A","B","C" };

/* ================= PROTOTYPES ================= */
float calculateGPA(int[]);
void calculatePerformance(Student&);

int loadClassFile(Student[], string);
void saveClassFile(Student[], int, string);

int selectSession();
int selectField();
int selectSection();

void addStudent();
void updateStudent();
void deleteStudent();
void searchStudent();
void searchStudentByName();
void viewAllStudents();
void sortStudents();
void rankStudents();
void classDashboard();
void generateReportCard();

void addAnnouncement();
void viewAnnouncements();
void deleteAnnouncement();
void updateDuesStatus();
void studentPanel();
void scholarshipStudents();

/* ================= GPA ================= */
float calculateGPA(int marks[]) {
    const int creditHours[SUBJECTS] = { 3, 3, 3, 3, 3, 3 }; // Credit hours for each subject
    float totalPoints = 0;
    int totalCredits = 0;

    for (int i = 0; i < SUBJECTS; i++) {
        float gradePoint;

        // Convert marks to grade point (4.0 scale)
        if (marks[i] >= 90) gradePoint = 4.0;
        else if (marks[i] >= 85) gradePoint = 3.7;
        else if (marks[i] >= 80) gradePoint = 3.3;
        else if (marks[i] >= 75) gradePoint = 3.0;
        else if (marks[i] >= 70) gradePoint = 2.7;
        else if (marks[i] >= 65) gradePoint = 2.3;
        else if (marks[i] >= 60) gradePoint = 2.0;
        else if (marks[i] >= 50) gradePoint = 1.0;
        else gradePoint = 0.0;

        totalPoints += gradePoint * creditHours[i];
        totalCredits += creditHours[i];
    }

    float gpa = totalPoints / totalCredits;

    // Round to 2 decimal places
    gpa = static_cast<int>(gpa * 100 + 0.5) / 100.0;

    return gpa;
}

/* ================= PERFORMANCE ================= */
void calculatePerformance(Student& s) {
    if (s.gpa < 1.9) {
        s.performance = "Fail";
        s.status = "Fail";
    }
    else if (s.gpa <= 2.3) {
        s.performance = "Academic Risk";
        s.status = "Pass";
    }
    else if (s.gpa <= 2.8) {
        s.performance = "Below Average";
        s.status = "Pass";
    }
    else if (s.gpa <= 3.5) {
        s.performance = "Average/Good";
        s.status = "Pass";
    }
    else {
        s.performance = "Excellent";
        s.status = "Pass";
    }
    s.dues = "Clear";
}

/* ================= FILE LOAD ================= */
int loadClassFile(Student students[], string filename) {
    ifstream fin(filename);
    int count = 0;

    while (fin >> students[count].id) {
        fin.ignore();
        getline(fin, students[count].name);

        for (int j = 0; j < SUBJECTS; j++)
            fin >> students[count].marks[j];

        fin >> students[count].gpa
            >> students[count].status
            >> students[count].performance
            >> students[count].dues
            >> students[count].session
            >> students[count].field
            >> students[count].section;

        count++;
    }
    fin.close();
    return count;
}

/* ================= FILE SAVE ================= */
void saveClassFile(Student students[], int count, string filename) {
    ofstream fout(filename);
    for (int i = 0; i < count; i++) {
        fout << students[i].id << endl;
        fout << students[i].name << endl;

        for (int j = 0; j < SUBJECTS; j++)
            fout << students[i].marks[j] << " ";
        fout << endl;

        fout << fixed << setprecision(2) << students[i].gpa << endl;
        fout << students[i].status << endl;
        fout << students[i].performance << endl;
        fout << students[i].dues << endl;
        fout << students[i].session << endl;
        fout << students[i].field << endl;
        fout << students[i].section << endl;
    }
    fout.close();
}

/* ================= SELECT ================= */
int selectSession() {
    int ch;
    cout << "Select Session:\n1.2022 2.2023 3.2024 4.2025\nChoice: ";
    cin >> ch;
    return ch - 1;
}
int selectField() {
    int ch;
    cout << "Select Field:\n1.CS 2.SE 3.AI 4.IST\nChoice: ";
    cin >> ch;
    return ch - 1;
}
int selectSection() {
    int ch;
    cout << "Select Section:\n1.A 2.B 3.C\nChoice: ";
    cin >> ch;
    return ch - 1;
}

/* ================= ADD STUDENT ================= */
void addStudent() {
    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";
    string backup = "backup_" + filename;

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    int n;
    cout << "Enter number of students: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "ID: ";
        cin >> students[count].id;
        cin.ignore();

       


        // NEW FEATURE: Duplicate ID check
        bool duplicate = false;

        for (int k = 0; k < count; k++) {
            if (students[k].id == students[count].id) {
                duplicate = true;
                break;
            }
        }

        if (duplicate) {
            cout << "Student ID already exists! Enter another ID.\n";
            i--;
            continue;
        }

        cout << "Name: ";
        getline(cin, students[count].name);

        for (int j = 0; j < SUBJECTS; j++) {
            do {
                cout << "Marks " << j + 1 << ": ";
                cin >> students[count].marks[j];
            } while (students[count].marks[j] < 0 || students[count].marks[j] > 100);
        }

        students[count].gpa = calculateGPA(students[count].marks);
        calculatePerformance(students[count]);

        students[count].session = sessions[sess];
        students[count].field = fields[fld];
        students[count].section = sections[sec];

        count++;
    }

    saveClassFile(students, count, filename);
    saveClassFile(students, count, backup);

    cout << "Students added successfully!\n";
}

/* ================= UPDATE ================= */
void updateStudent() {
    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";
    string backup = "backup_" + filename;

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    int id;
    cout << "Enter Student ID: ";
    cin >> id;

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            for (int j = 0; j < SUBJECTS; j++) {
                do {
                    cout << "New Marks " << j + 1 << ": ";
                    cin >> students[i].marks[j];
                } while (students[i].marks[j] < 0 || students[i].marks[j] > 100);
            }
            students[i].gpa = calculateGPA(students[i].marks);
            calculatePerformance(students[i]);

            saveClassFile(students, count, filename);
            saveClassFile(students, count, backup);
            cout << "Student updated successfully!\n";
            return;
        }
    }
    cout << "ID not found\n";
}

/* ================= DELETE ================= */
void deleteStudent() {
    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";
    string backup = "backup_" + filename;

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    int id;
    cout << "Enter Student ID: ";
    cin >> id;

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            for (int j = i; j < count - 1; j++)
                students[j] = students[j + 1];
            count--;

            saveClassFile(students, count, filename);
            saveClassFile(students, count, backup);
            cout << "Student deleted successfully!\n";
            return;
        }
    }
    cout << "ID not found\n";
}

/* ================= SEARCH ================= */
void searchStudent() {
    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    int id;
    cout << "Enter Student ID: ";
    cin >> id;

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            cout << "ID: " << students[i].id
                << " Name: " << students[i].name
                << " GPA: " << fixed << setprecision(2) << students[i].gpa
                << " Status: " << students[i].status << endl;
            return;
        }
    }
    cout << "ID not found\n";
}

// search by name
void searchStudentByName() {

    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    string name;
    cin.ignore();

    cout << "Enter student name: ";
    getline(cin, name);

    for (int i = 0; i < count; i++) {

        if (students[i].name == name) {

            cout << "ID: " << students[i].id
                << "\nName: " << students[i].name
                << "\nGPA: " << students[i].gpa
                << "\nStatus: " << students[i].status << endl;

            return;
        }
    }

    cout << "Student not found\n";
}


/* ================= VIEW ALL ================= */
void viewAllStudents() {
    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    cout << "ID\tName\tGPA\tStatus\tPerformance\n";
    for (int i = 0; i < count; i++) {
        cout << students[i].id << "\t"
            << students[i].name << "\t"
            << fixed << setprecision(2) << students[i].gpa << "\t"
            << students[i].status << "\t"
            << students[i].performance << endl;
    }
}

/* ================= SORT ================= */
void sortStudents() {
    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    int ch;
    cout << "Sort by: 1.ID 2.Name\nChoice: ";
    cin >> ch;

    for (int i = 0; i < count - 1; i++)
        for (int j = 0; j < count - i - 1; j++)
            if ((ch == 1 && students[j].id > students[j + 1].id) ||
                (ch == 2 && students[j].name > students[j + 1].name))
                swap(students[j], students[j + 1]);

    for (int i = 0; i < count; i++)
        cout << students[i].id << " " << students[i].name << " " << students[i].gpa << endl;
}

/* ================= RANK ================= */
void rankStudents() {
    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    for (int i = 0; i < count - 1; i++)
        for (int j = 0; j < count - i - 1; j++)
            if (students[j].gpa < students[j + 1].gpa)
                swap(students[j], students[j + 1]);

    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << students[i].name << " " << students[i].gpa << endl;
}

/* ================= DASHBOARD ================= */
void classDashboard() {
    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    int pass = 0, fail = 0;
    float total = 0;

    for (int i = 0; i < count; i++) {
        if (students[i].gpa < 1.9) fail++;
        else pass++;
        total += students[i].gpa;
    }
    cout << "Total: " << count
        << "\nPass: " << pass
        << "\nFail: " << fail
        << "\nAverage GPA: " << (count ? total / count : 0) << endl;
    float maxGPA = 0;
    string topper;

    for (int i = 0; i < count; i++) {

        if (students[i].gpa > maxGPA) {

            maxGPA = students[i].gpa;
            topper = students[i].name;
        }
    }
    cout << "Class Topper: " << topper << " GPA: " << maxGPA << endl;
}

/* ================= ANNOUNCEMENTS ================= */
void addAnnouncement() {
    cin.ignore();
    ofstream fout("announcements.txt", ios::app);
    string line;
    cout << "Enter announcement: ";
    getline(cin, line);
    fout << line << endl;
    fout.close();
}

void viewAnnouncements() {
    ifstream fin("announcements.txt");
    string line;
    cout << "\n--- ANNOUNCEMENTS ---\n";
    while (getline(fin, line)) cout << line << endl;
    fin.close();
}

void deleteAnnouncement() {
    ifstream fin("announcements.txt");
    string lines[100];
    int count = 0;

    while (getline(fin, lines[count])) count++;
    fin.close();

    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << lines[i] << endl;

    int del;
    cout << "Enter number to delete: ";
    cin >> del;

    ofstream fout("announcements.txt");
    for (int i = 0; i < count; i++)
        if (i != del - 1) fout << lines[i] << endl;
    fout.close();
}
 //new feature dues check 

void updateDuesStatus() {

    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    int id;
    cout << "Enter student ID: ";
    cin >> id;

    for (int i = 0; i < count; i++) {

        if (students[i].id == id) {

            int ch;

            cout << "1. Clear\n2. Pending\nChoice: ";
            cin >> ch;

            if (ch == 1)
                students[i].dues = "Clear";
            else
                students[i].dues = "Pending";

            saveClassFile(students, count, filename);

            cout << "Dues updated successfully\n";

            return;
        }
    }

    cout << "Student not found\n";
}
//new fewature scholarship eligibility 
void scholarshipStudents() {

    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    cout << "\nScholarship Eligible Students:\n";

    for (int i = 0; i < count; i++) {

        if (students[i].gpa > 3.5) {

            cout << students[i].name
                << " GPA: "
                << students[i].gpa << endl;
        }
    }
}
/* ================= STUDENT PANEL ================= */
void studentPanel() {
    int sid;
    cout << "Enter Student ID: ";
    cin >> sid;

    Student s;
    bool found = false;

    for (int si = 0; si < 4 && !found; si++)
        for (int fi = 0; fi < 4 && !found; fi++)
            for (int se = 0; se < 3 && !found; se++) {
                string filename = sessions[si] + "_" + fields[fi] + "_" + sections[se] + ".txt";
                Student students[MAX_STUDENTS];
                int count = loadClassFile(students, filename);
                for (int i = 0; i < count; i++)
                    if (students[i].id == sid) {
                        s = students[i];
                        found = true;
                        break;
                    }
            }

    if (!found) {
        cout << "ID not found!\n";
        return;
    }

    cout << "Name: " << s.name
        << "\nGPA: " << s.gpa
        << "\nStatus: " << s.status
        << "\nPerformance: " << s.performance << endl;
    cout << "\nMarks:\n";

    for (int i = 0; i < SUBJECTS; i++) {

        cout << "Subject " << i + 1 << ": " << s.marks[i] << endl;
    }
}

/* ================= MAIN ================= */
int main() {
    int choice;
    do {
        cout << "\n========================================";
        cout << "\n        STUDENT MANAGEMENT SYSTEM";
        cout << "\n========================================";

        cout << "\n\n-------------- MAIN MENU --------------";

        cout << "\n  1. Admin Login";
        cout << "\n  2. Student Login";
        cout << "\n  0. Exit";

        cout << "\n---------------------------------------";

        cout << "\nEnter Your Choice: ";
        cin >> choice;

        if (choice == 1) {
            string password;
            int attempts = 3;   // NEW FEATURE: Admin password attempts
            bool access = false;

            while (attempts--) {
                cout << "Password: ";
                cin >> password;

                if (password == FIXED_PASSWORD) {
                    access = true;
                    break;
                }
                cout << "Wrong password! Attempts left: " << attempts << endl;
            }

            if (!access) {
                cout << "Access denied. Returning to main menu.\n";
                continue;
            }

            int ch;
            do {
                cout << "\n========================================";
                cout << "\n            ADMIN CONTROL PANEL         ";
                cout << "\n========================================";

                cout << "\n\n--- Student Management ---";
                cout << "\n 1. Add New Student";
                cout << "\n 2. Update Student Record";
                cout << "\n 3. Delete Student Record";
                cout << "\n 4. Search Student";
                cout << "\n 5. Search Student by Name";
                cout << "\n 6. View All Students";

                cout << "\n\n--- Academic Tools ---";
                cout << "\n 7. Dashboard Overview";
                cout << "\n 8. Sort Students (Marks/Name)";
                cout << "\n 9. Generate Class Ranking";
                cout << "\n 10. Generate Report Card";

                cout << "\n\n--- Communication ---";
                cout << "\n11. Post Announcements";

                cout << "\n\n--- Dues Section  ---";
                cout << "\n12. Update Dues Status";
                cout << "\n13. Scholarship Eligible Students";

                cout << "\n\n----------------------------------------";
                cout << "\n 0. Logout";
                cout << "\n========================================";
                cout << "\nEnter Your Choice: ";
                cin >> ch;

                if (ch == 1) addStudent();
                else if (ch == 2) updateStudent();
                else if (ch == 3) deleteStudent();
                else if (ch == 4) searchStudent();
                else if (ch == 5) searchStudentByName();
                else if (ch == 6) viewAllStudents();
                else if (ch == 7) classDashboard();
                else if (ch == 8) sortStudents();
                else if (ch == 9) rankStudents();
                else if (ch == 10) generateReportCard();
                else if (ch == 11) {
                    int a;
                    cout << "\n================ ANNOUNCEMENTS MENU ================\n";
                    cout << " 1. Add Announcement\n";
                    cout << " 2. View Announcements\n";
                    cout << " 3. Delete Announcement\n";
                    cout << " 0. Back to Admin Panel\n";
                    cout << "====================================================\n";
                    cout << "Enter your choice: ";
                    cin >> a;
                    if (a == 1) addAnnouncement();
                    else if (a == 2) viewAnnouncements();
                    else if (a == 3) deleteAnnouncement();
                }
                else if (ch == 12) updateDuesStatus();
                else if (ch == 13) scholarshipStudents();

            } while (ch != 0);
        }
        else if (choice == 2) studentPanel();

    } while (choice != 0);

    return 0;
}

/* ================= REPORT CARD ================= */
void generateReportCard() {
    int sess = selectSession();
    int fld = selectField();
    int sec = selectSection();

    string filename = sessions[sess] + "_" + fields[fld] + "_" + sections[sec] + ".txt";

    Student students[MAX_STUDENTS];
    int count = loadClassFile(students, filename);

    int id;
    cout << "Enter Student ID: ";
    cin >> id;

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            string rep = "report_" + to_string(id) + ".txt";
            ofstream fout(rep);

            fout << "Name: " << students[i].name << endl;
            fout << "Session: " << students[i].session << endl;
            fout << "Field: " << students[i].field << endl;
            fout << "Section: " << students[i].section << endl;
            fout << "GPA: " << students[i].gpa << endl;
            fout << "Status: " << students[i].status << endl;
            fout << "Performance: " << students[i].performance << endl;

            fout.close();
            cout << "Report card saved!\n";
            return;
        }
    }
    cout << "ID not found\n";
}