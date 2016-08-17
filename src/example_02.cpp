// Example 01 Math Helper
// Covers functions, loops, standard input, and more standard output.

#include <iostream> // `std::cout`, `std::cin`
#include <string> // `std::string`

using namespace std;

// Functions in C++ typically have 4 parts, we'll disect the function "isPrime".
//
// First is the return value, `bool` (which means boolean);
// we're using a `bool` because a given number can be either prime or not.
//
// Second is the function name, "isPrime".
//
// Third is the parameter list (which is always between parenthesis);
// every variable in the list must have a declared type (our prime function
// only checks one value, which must be an `int` (integer)).
//
// The final part is the body, which must be between curly braces ("{" and "}").
bool isPrime(int n)
{
  if (n <= 1)
    return false;

  // Curly braces must be used if there are multiple statements.
  else {
    for (int i { 2 }; // Once again using curly braces to initialize a variable.
         i < n / 2;   // Integer division causes automatic rounding down.
         ++i) {       // The pre-increment operator simply adds 1 to the variable.

      // If "n" is divisible by "i", then "n" is not prime.
      if (n % i == 0)
        return false;
    }

    // If no number was a factor, it must be prime.
    return true;
  }
}



int main()
{
  while (true) {
    string input;

    // The "extraction operator" pulls data from a source.
    // It should be noted that standard input's extraction operator is for formatted input,
    // that means it doesn't directly get line-by-line input. By default, it reads up to
    // space characters. This means, you can type several commands in a series with spaces in
    // between and they will all be executed once you press enter/return.
    cin >> input;

    if (input == "quit")
      break;

    else {
      // `std::boolalpha` tells the stream to output `bool` values as "true" or "false" instead
      // of "1" or "0", respectively.
      // `std::stoi` converts a `std::string` to an `int`.
      cout << boolalpha << isPrime(stoi(input)) << endl;
    }
  }
}