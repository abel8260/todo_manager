#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class TodoList {
private:
    std::string filename;
    std::vector<std::string> tasks;

    void loadFromFile() {
        std::ifstream file(filename);
        if (file) {
            tasks.clear();
            std::string line;
            bool inTasks = false;

            while (std::getline(file, line)) {
                // Trim spaces
                line.erase(0, line.find_first_not_of(" \t"));
                line.erase(line.find_last_not_of(" \t") + 1);

                if (line == "\"tasks\": [") {
                    inTasks = true; // Start reading tasks
                    continue;
                } 
                if (line == "]") {
                    inTasks = false; // Stop reading tasks
                    continue;
                }

                if (inTasks && !line.empty()) {
                    if (line.back() == ',') {
                        line.pop_back(); // Remove trailing comma
                    }
                    tasks.push_back(line.substr(1, line.size() - 2)); // Remove quotes
                }
            }
        }
        file.close();
    }

    void saveToFile() {
        std::ofstream outFile(filename);
        outFile << "{\n  \"tasks\": [\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            outFile << "    \"" << tasks[i] << "\"";
            if (i != tasks.size() - 1) {  // Add a comma after all but the last item
                outFile << ",";
            }
            outFile << "\n";
        }
        outFile << "  ]\n}";
        outFile.close();
    }

public:
    TodoList(const std::string &file) : filename(file) {
        loadFromFile();
    }

    void readTasks() {
        std::cout << "Your TODO List:\n";
        for (const auto &task : tasks) {
            std::cout << "\"" << task << "\"\n";
        }
    }

    void addTask(const std::string &task) {
        tasks.push_back(task);
        saveToFile();
        std::cout << "Task added successfully!\n";
    }

    void updateTask(const std::string &oldTask, const std::string &newTask) {
        for (auto &task : tasks) {
            if (task == oldTask) {
                task = newTask;
                saveToFile();
                std::cout << "Task updated successfully!\n";
                return;
            }
        }
        std::cerr << "Task not found. No update made.\n";
    }

    void deleteTask(const std::string &taskToDelete) {
        bool found = false;
        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
            if (*it == taskToDelete) {
                tasks.erase(it);
                found = true;
                break;  // Stop after deleting the first occurrence
            }
        }

        if (found) {
            saveToFile();
            std::cout << "Task deleted successfully!\n";
        } else {
            std::cerr << "Task not found. No deletion made.\n";
        }
    }
};

int main() {
    TodoList todoList("todolist.json");
    int choice;
    std::string task, oldTask;

    std::cout << "Choose your option:\n";
    std::cout << "1. List all TODOs\n";
    std::cout << "2. Add a TODO\n";
    std::cout << "3. Update a TODO\n";
    std::cout << "4. Delete a TODO\n";
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1:
            todoList.readTasks();
            break;
        case 2:
            std::cout << "Enter a new task: ";
            std::getline(std::cin, task);
            todoList.addTask(task);
            break;
        case 3:
            std::cout << "Enter the task to update: ";
            std::getline(std::cin, oldTask);
            std::cout << "Enter the new task: ";
            std::getline(std::cin, task);
            todoList.updateTask(oldTask, task);
            break;
        case 4:
            std::cout << "Enter the task to delete: ";
            std::getline(std::cin, task);
            todoList.deleteTask(task);
            break;
        default:
            std::cout << "Invalid choice!\n";
    }

    return 0;
}
