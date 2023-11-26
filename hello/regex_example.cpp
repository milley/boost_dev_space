#include <boost/regex.hpp>
#include <iostream>
#include <string>

// c++ -I /opt/homebrew/Cellar/boost/1.83.0/include ./regex_example.cpp -o
// regex_example -std=c++20 -L /opt/homebrew/Cellar/boost/1.83.0/lib
// -lboost_regex

int main() {
  std::string line;
  boost::regex pat("^Subject: (Re: |Aw: )*(.*)");

  while (std::cin) {
    std::getline(std::cin, line);
    boost::smatch matches;
    if (boost::regex_match(line, matches, pat))
      std::cout << matches << std::endl;
  }
}