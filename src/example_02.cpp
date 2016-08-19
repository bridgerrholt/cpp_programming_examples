// Example 01 Math Helper
// Covers functions, loops, exceptions, types, standard input, and more standard output.

#include <iostream> // `std::cout`, `std::cin`
#include <string> // `std::string`

using namespace std;

// Keyword `typedef` creates a type alias, letting you to use better names for complicated types.
// In this declaration, we declare our type "NumberType" to be an `unsigned long long`.
//
// In C++, there are multiple integer types, each without a hard-set size. A reason for this is
// so that programs can compile well for different architectures (for example, using 32-bit
// integers is ideal for a 32-bit OS, while 64-bit integers are ideal for a 64-bit OS).
// The primary integer type you should use for most things is `int`, and use `unsigned` if that
// integer should always be positive.
//
// We use `unsigned` for our prime number function because
// negative numbers cannot be prime and not having a sign means we can have numbers twice as large.
// Speaking of number size, we use `long long` to declare our type as the largest integer possible.
//
// It should be noted that the C++ standard already declares a large unsigned type, `std::size_t`,
// but it is important to know about `typedef` because it is used a lot in C++.
typedef unsigned long long NumberType;

// Functions in C++ typically have 4 parts, we'll disect the function "isPrime".
//
// First is the return value, `bool` (which means boolean);
// we're using a `bool` because a given number can be either prime or not.
//
// Second is the function name, "isPrime".
//
// Third is the parameter list (which is always between parenthesis);
// every variable in the list must have a declared type (our prime function only checks one value,
// which must be a "NumberType" object (which, remember, really means `unsigned long long`)).
//
// The final part is the body, which must be between curly braces ("{" and "}").
bool isPrime(NumberType n)
{
  if (n <= 1)
    return false;

  // Curly braces must be used if there are multiple statements.
  else {
    for (NumberType i { 2 }; // Once again using curly braces to initialize a variable.
         i < n / 2;          // Integer division causes automatic rounding down.
         ++i) {              // The pre-increment operator simply adds 1 to the variable.

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
    // space characters. This means you can type several commands in a series with spaces in
    // between and they will all be executed once you press enter/return.
    cin >> input;

    if (input == "quit")
      break;

    else {
      // In C++, when something unintended happens, an exception is usually thrown. A throw on its
      // own will cause the program to crash, but using `try/catch` blocks will allow the program
      // to continue running.
      // We use `try/catch` in this instance because there are a few errors that could occur due to
      // the user's input. If an error does occur, we simply don't do any processing of the
      // specific input that caused it.
      try {
        // `std::boolalpha` tells the stream to output `bool` values as "true" or "false" instead
        // of "1" or "0", respectively.
        // `std::stoll` converts a `std::string` to an `unsigned long long`
        // ("s" means "string", "to", "ull" means "unsigned long long").
        cout << boolalpha << isPrime(stoull(input)) << endl;
      }

      // If an exception is thrown within the `try` block, the `catch` block will recieve the
      // transmitted data, which usually contains an error message.
      // You can have multiple catch blocks but for standard functions all we need is
      // `std::exception`. The ampersand ("&") indicates that our variable "e" will be a reference
      // to an std::exception. You always want to reference exceptions because it allows many
      // kinds to be caught under one name.
      catch (std::exception & e) {
        // The `std::exception` class has a function `what` that returns the error message.
        cout << e.what() << endl;
      }
    }
  }
}