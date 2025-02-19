#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>

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
  std::unordered_set<std::string> builtins = {"echo","exit","type"};

std::string input;
  // std::getline(std::cin, input);
  // std::cout << input << ": command not found" << std::endl;

  
  while (true)
  {
    std::cout << "$ ";
    std::getline(std::cin, input);

    if (input == "exit 0")
    {
      break; // Exit the loop
    }

    if (input.starts_with("echo "))
    { 
      std::cout << std::string_view(input).substr(5) << '\n';
      continue;
    }

    if(input.starts_with("type "))
    {
      std::string cmd = input.substr(5);
      if(builtins.find(cmd) != builtins.end()){
        std::cout << cmd << " is a shell builtin\n";
      }
      else{
        std::cout << cmd << ": not found\n";
      }
      continue;
    }

    std::cout << input << ": command not found\n";
  }

  return 0;
}
