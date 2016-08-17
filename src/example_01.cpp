// Example 01 Hello World
// Covers standard output.

#include <iostream> // `std::cout`

// Not recommended in actual projects but will be used for simplicity.
// Makes you not have to type "std::" before every standard name.
using namespace std;

// Functions in C++ typically have 4 parts,
// we'll disect the function definition `int main() { }`.
// First is the return value, `int` (which means integer).
// Second is the function name, "main".
// Third is the parameter list
// Every C++ program must have the function "main" and it must return "int".
int main()
{
  // Newlines are not automatic with the standard out stream.
  cout << "Hello, world!\n";
  
  // The insertion operator ("<<") simply strings together output.
  cout << "Hello, " << "again!\n";

  // If "\n" is too ugly, `std::endl` can be used instead.
  // It also flushes the stream, which guarantees that
  // anything set to output will output right now.
  cout << "Goodbye, world!" << endl;

  // This line is not required, the main function
  // automatically returns 0 (no other function has auto-return).
  return 0;
}