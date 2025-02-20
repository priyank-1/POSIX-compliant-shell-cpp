#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <cstdlib>  // For getenv()
#include <unistd.h> // For fork(), execvp(), access()
#include <sys/wait.h> // For waitpid()
// #include <direct.h> // For getcwd() = windows only

int main() {
    // Optimize I/O
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // Built-in commands
    std::unordered_set<std::string> builtins = {"echo", "exit", "type","pwd"};

    std::string input;

    while (true) {
      std::cout << "$ " << std::flush;
        std::getline(std::cin, input);

        if (input == "exit 0") {
            break;
        }

        if (input.starts_with("echo ")) {
            std::cout << std::string_view(input).substr(5) << '\n';
            continue;
        }

        if (input.starts_with("type ")) {
            std::string cmd = input.substr(5);
            if (builtins.find(cmd) != builtins.end()) {
                std::cout << cmd << " is a shell builtin\n";
                continue;
            }
            const char* path = std::getenv("PATH"); // Getting PATH environment variable
            if (path != nullptr) { // If PATH exists
                std::string path_str(path);
                std::stringstream ss(path_str);
                std::string token;
                bool found = false;
                while (std::getline(ss, token, ':')) { // Tokenize PATH directories
                    std::string cmd_path = token + "/" + cmd;
                    if (access(cmd_path.c_str(), X_OK) == 0) { // Check if executable
                        std::cout << cmd << " is " << cmd_path << std::endl;
                        found = true;
                        break; // Stop after finding first match
                    }
                }
                if (!found) std::cout << cmd << ": not found\n";
            } else {
                std::cout << cmd << ": not found\n";
            }
            continue;
        }

        if(input == "pwd"){
            const size_t size = 1024;
            char buffer[size];
            if(getcwd(buffer,size)!=nullptr){
                std::cout<<buffer<<std::endl;
            }
            else {
                std::cerr<<"Error: Failed to get current working directory\n";
            }
            continue;
        }

        if(input.starts_with("cd ")){
            // std::string path = input.substr(3);
            std::string current_path(input.substr(3));

            if(current_path.starts_with("..")){
                std::string temp_path(current_path.substr(3));
                if(chdir(temp_path.c_str())!=0){
                    std::cout<<"cd: "<<temp_path<<": No such file or directory\n";
                }
                continue;
            }
            else if(current_path = "~") continue;
            else{
                current_path = current_path.starts_with(".") ? current_path.substr(2) : current_path;
                if(access(current_path.c_str(),F_OK)==0)
                 chdir(current_path.c_str());
                 else 
                 std::cout << "cd: " << current_path << ": No such file or directory\n";
                 continue;
            }
           
        }


        // Handle external commands (program execution)
        std::stringstream ss(input);
        std::vector<std::string> args;
        std::string arg;
        
        while (ss >> arg) { 
            args.push_back(arg);
        }
        
        if (args.empty()) continue; 
        
        std::string cmd = args[0];
        
        const char* path = std::getenv("PATH");
        if (path == nullptr) {
            std::cout << cmd << ": not found\n";
            continue;
        }

        std::string executablePath;
        std::stringstream path_ss(path);
        std::string dir;
        bool found = false;

        while (std::getline(path_ss, dir, ':')) { 
            std::string full_path = dir + "/" + cmd;
            if (access(full_path.c_str(), X_OK) == 0) {
                executablePath = full_path;
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << cmd << ": not found\n";
            continue;
        }

        // logic for fork and child process execution
        std::vector<char*> argv;
        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str())); 
        }
        argv.push_back(nullptr); 
       
        pid_t pid = fork();

        if (pid < 0) { 
            std::cerr << "Error: Failed to fork process\n";
            continue;
        }

        if (pid == 0) { 
            execvp(executablePath.c_str(), argv.data());
            std::cerr << cmd << ": command not found\n"; 
            exit(1);
        } else { 
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
