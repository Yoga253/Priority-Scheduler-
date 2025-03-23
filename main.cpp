#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <iomanip>
using namespace std;

// Task Type
enum class TaskType { Fixed, Floating };

// Color Categories
enum class Color { Red, Orange, Blue };

// Task Struct
struct Task {
    string name;
    TaskType type;
    Color color;
    string startTime, endTime;

    // Constructor
    Task(string taskName, TaskType taskType, Color taskColor, string start = "", string end = "")
        : name(taskName), type(taskType), color(taskColor), startTime(start), endTime(end) {}

    // Custom comparator
    bool operator<(const Task &other) const {
        if (color != other.color) return color > other.color; // Higher priority color first
        if (type != other.type) return type < other.type; // Fixed before Floating
        if (type == TaskType::Fixed) return startTime > other.startTime; // Earlier time first
        return false;
    }
};

// Function to get color as a string
string getColor(Color color) {
    switch (color) {
        case Color::Red: return "Red";
        case Color::Orange: return "Orange";
        case Color::Blue: return "Blue";
        default: return "Unknown";
    }
}

void printBoxedHeader(const string& title) {
    int width = 50;
    cout << string(width, '=') << endl;
    cout << setw((width + title.length()) / 2) << title << endl;
    cout << string(width, '=') << endl;
}

int main() {
    priority_queue<Task> pq;
    unordered_set<string> fixedTimes;
    int n;

    printBoxedHeader("Task Scheduler");

    cout << "Enter number of tasks: ";
    cin >> n;
    cin.ignore();

    cout << "\nEnter tasks in the format: task_name task_type color [start_time] [end_time]\n";
    cout << "(Example: Meeting fx 0 08:00 OR Workout fx 1 06:00 07:30 OR Study fl 2)\n\n";

    int validCount = 0;
    while (validCount < n) {
        string line, taskName, taskTypeStr, startTime = "", endTime = "";
        int colorInput;
        TaskType taskType;
        Color taskColor;

        cout << "Enter task " << (validCount + 1) << ": ";
        getline(cin, line);
        stringstream ss(line);

        if (!(ss >> taskName >> taskTypeStr >> colorInput)) {
            cout << "Invalid input format. Please enter again.\n";
            continue;  // Retry input
        }

        if (colorInput == 0) taskColor = Color::Red;
        else if (colorInput == 1) taskColor = Color::Orange;
        else taskColor = Color::Blue;

        if (taskTypeStr == "fx") {
            taskType = TaskType::Fixed;
            if (!(ss >> startTime) || (ss >> endTime && endTime.empty())) {
                cout << "Invalid fixed task time format. Please enter again.\n";
                continue;  // Retry input
            }
        } else if (taskTypeStr == "fl") {
            taskType = TaskType::Floating;
        } else {
            cout << "Invalid task type. Use 'fx' for Fixed or 'fl' for Floating. Please enter again.\n";
            continue;  // Retry input
        }

        if (taskType == TaskType::Fixed && fixedTimes.count(startTime)) {
            cout << "Error: Multiple tasks with start time " << startTime << ". Please enter again.\n";
            continue;  // Retry input
        }

        pq.push(Task(taskName, taskType, taskColor, startTime, endTime));
        if (taskType == TaskType::Fixed) fixedTimes.insert(startTime);
        validCount++;
    }

    printBoxedHeader("Tasks in Priority Order");

    if (pq.empty()) {
        cout << "No valid tasks entered.\n";
        return 0;
    }

    while (!pq.empty()) {
        Task task = pq.top();
        pq.pop();

        cout << "| Task: " << setw(15) << left << task.name 
             << " | Color: " << setw(6) << left << getColor(task.color) 
             << " | Type: " << (task.type == TaskType::Fixed ? "Fixed" : "Floating");

        if (task.type == TaskType::Fixed) 
            cout << " (" << task.startTime << (task.endTime.empty() ? "" : " - " + task.endTime) << ")";
        
        cout << " |\n";
    }

    cout << string(50, '=') << endl;
    return 0;
}
