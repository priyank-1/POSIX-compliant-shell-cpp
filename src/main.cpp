#include <iostream>
#include <string>
#include <sstream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  std::cout << "$ ";

  std::string input;
  std::getline(std::cin, input);
  std::cout << input << ": command not found" << std::endl;

  while(true){
    std::cout << "$ ";
    std::getline(std::cin,input);
    if(input == "exit 0") exit(0);
    if(input.find("echo ") == 0){
      std::cout << input.substr(5) << std::endl;
      continue;
    }
    std::cout << input << ": command not found" << std::endl;
  }

 
  // return 0;
}
