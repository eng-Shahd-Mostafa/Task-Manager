#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <locale>

using namespace std;

class Process {
private:
    string Name;
    int PID;
    string SessionName;
    int SessionNum;
    long MemoryUsage;

public:
    Process(const string& name, int pid, const string& sessionName, int sessionNum, long memoryUsage)
        : Name(name), PID(pid), SessionName(sessionName), SessionNum(sessionNum), MemoryUsage(memoryUsage) {
    }

    string getName() const { return Name; }
    int getPID() const { return PID; }
    string getSessionName() const { return SessionName; }
    int getSessionNum() const { return SessionNum; }
    long getMemoryUsage() const { return MemoryUsage; }

    void display() const {

        cout << left << setw(50) << Name
            << setw(8) <<  PID
            << setw(14) << SessionName
            << setw(10) << SessionNum
            << fixed << setprecision(2) << MemoryUsage << " K" << '\n'; 
    }
};

class TaskManager {
private:
    vector<Process> processes;

public:
    void fetchProcesses() {
        processes.clear();

        system("tasklist /FO CSV > temp_tasklist.csv");

        ifstream file("temp_tasklist.csv");
        if (!file.is_open()) {
            cerr << "Failed to open the file temp_tasklist.csv\n";
            return;
        }

        string line;
        getline(file, line); // For neglect the first row

        while (getline(file, line)) {
            stringstream s(line);
            string Name, PID, sessionName, sessionNum, mem;

            getline(s, Name, ',');
            getline(s, PID, ',');
            getline(s, sessionName, ',');
            getline(s, sessionNum, ',');
            getline(s, mem);

            // For remove the Quotes 
            Name.erase(remove(Name.begin(), Name.end(), '\"'), Name.end());
            PID.erase(remove(PID.begin(), PID.end(), '\"'), PID.end());
            sessionName.erase(remove(sessionName.begin(), sessionName.end(), '\"'), sessionName.end());
            sessionNum.erase(remove(sessionNum.begin(), sessionNum.end(), '\"'), sessionNum.end());

            mem.erase(remove(mem.begin(), mem.end(), ','), mem.end());  
            mem.erase(remove(mem.begin(), mem.end(), '\"'), mem.end());
            mem.erase(remove(mem.begin(), mem.end(), 'K'), mem.end());

            int pidNum = PID.empty() ? 0 : stoi(PID);
            int sessionNumVal = sessionNum.empty() ? 0 : stoi(sessionNum);
            long memoryUsage = mem.empty() ? 0 : stol(mem);  

            processes.emplace_back(Name, pidNum, sessionName, sessionNumVal, memoryUsage);
        }

        file.close();
        remove("temp_tasklist.csv");
    }

    void sortProcesses(int choice) {
        if (choice == 1) {
            sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
                return a.getName() < b.getName();
                });
        }
        else if (choice == 2) {
            sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
                return a.getMemoryUsage() < b.getMemoryUsage();
                });
        }
        else if (choice == 3) {
            sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
                return a.getPID() < b.getPID();
                });
        }
    }

    void displayProcesses() const {
        locale loc("en_US.UTF-8");
        cout.imbue(loc);

        cout << string(47, ' ') << "Task Manager\n";
        cout << string(44, ' ') << string(18, '=') << "\n\n";
        cout << left
            << setw(50) << "Process Name"
            << setw(8) << "PID"
            << setw(14) << "Session Name"
            << setw(10) << "Session#"
            << setw(12) << "Memory Usage" << '\n';
        cout << string(100, '=') << '\n';

        for (const auto& Process : processes) {
            Process.display();
        }
        cout << "\n\n";
    }

    void showMenu() const {
        cout << "Please, Select how you want to sort the Task Manager:\n";
        cout << string(54, '=') << '\n';
        cout << "1. Sort by Process Name\n";
        cout << "2. Sort by Memory Usage\n";
        cout << "3. Sort by PID\n";
        cout << "4. Exit\n";
    }
};

int main() {
    TaskManager tmanager;

    tmanager.fetchProcesses();

    string input;
    int choice;

    while (true) {
        tmanager.showMenu();
        cout << "Please, Enter your choice.\n";
        cin >> input;

        if (!all_of(input.begin(), input.end(), ::isdigit)) {
            cout << "Invalid input, Please enter a number between 1 and 4.\n\n";
            continue;
        }

        choice = stoi(input);

        if (choice >= 1 && choice <= 3) {
            tmanager.sortProcesses(choice);
            tmanager.displayProcesses();
        }
        else if (choice == 4) {
            cout << "Exiting Task Manager.\n";
            break;
        }
        else {
            cout << "Invalid input, Please enter a number between 1 and 4.\n\n";
        }

    }

    return 0;
}
