#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_set>
#include <unistd.h>
#include <cstdlib>

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  // Uncomment this block to pass the first stage
  // std::cout << "$ ";
  std::unordered_set<std::string> builtins = {"echo", "exit", "type"};

  std::string input;
  // std::getline(std::cin, input);
  // std::cout << input << ": command not found" << std::endl;

  while (true)
  {
    std::cout << "$ ";
    std::getline(std::cin, input);

    if (input == "exit 0")
    {
      break;
    }

    if (input.starts_with("echo "))
    {
      std::cout << std::string_view(input).substr(5) << '\n';
      continue;
    }

    if (input.starts_with("type "))
    {
      std::string cmd = input.substr(5);
      if (builtins.find(cmd) != builtins.end())
      {
        std::cout << cmd << " is a shell builtin\n";
        continue;
      }
      const char *path = std::getenv("PATH");
      if (path != nullptr)
      {
        std::string path_str(path);
        std::stringstream ss(path_str);
        std::string token;
        bool found = false;
        while (std::getline(ss, token, ':'))
        {
          std::string cmd_path = token + "/" + cmd;
          if (access(cmd_path.c_str(), X_OK) == 0)
          {
            std::cout << cmd << " is" << cmd_path << std::endl;
            found = true;
            break; // break to find only first executable path
          }
        }

        if(!found) std::cout << cmd << ": not found\n";
      }
      else
      {
        std::cout << cmd << ": not found\n";
      }
      continue;
    }

    std::cout << input << ": command not found\n";
  }

  return 0;
}
