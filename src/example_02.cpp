// Example 01 Math Helper
// Covers functions, loops, exceptions, types, standard input, and more standard output.

#include <iostream> // `std::cout`, `std::cin`
#include <string> // `std::string`

using namespace std;

// The keyword `using` creates an alias, letting you to use better names for complicated types.
// In this declaration we declare our type, `NumberType`, to be a `long long int`.
//
// In C++ there are multiple integer types, each described by a minimum number of bits.
// The primary integer type you should use for most things is `int` (which has at least 16 bits,
// but for every platform you build for it probably has at least 32).
//
// We use the `long long` modifier to declare our type as the largest integer available, it is
// guaranteed to have at least 64 bits. If we were to use the `long` modifier, it would be
// guaranteed to have 32 bits.
//
// It should be noted that in the C++ standard header `<cstdint>`, the type `intmax_t` is defined
// as the largest integer available, so we are reinventing the wheel here.
// However, using this alias makes it much easier in the future to change the type we're working
// with if we ever had to.
using NumberType = long long int;

// Plain functions in C++ typically have 4 parts, we'll disect the function `isPrime`.
//
// First is the return value, `bool` (which means boolean);
// we're using a `bool` because a given number can be either prime or not.
//
// Second is the function name, "isPrime".
//
// Third is the parameter list (which is always between parenthesis);
// every variable in the list must have a declared type (our prime function only checks one value,
// which must be a `NumberType` object (which, remember, really means `long long int`)).
//
// The final part is the body, which must be between curly braces ("{" and "}").
bool isPrime(NumberType n)
{
  // Prime numbers must be greater than 1.
  if (n > 1) { // Curly braces must be used if there are multiple statements.

    for (NumberType i { 2 }; // Once again using curly braces to initialize a variable.
         i < n / 2;          // Integer division causes automatic rounding down.
         ++i) {              // The pre-increment operator simply adds 1 to the variable.

      // If `n` is divisible by `i`, then `n` is not prime.
      if (n % i == 0)
        return false;
    }

    // If no number was a factor, it must be prime.
    return true;
  }

  // It was not greater than 1.
  else
    return false;
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
        // `std::stoll` converts a `std::string` to a `long long int`
        // ("s" means "string", "to", "ll" means "long long").
        cout << boolalpha << isPrime(stoll(input)) << endl;
      }

      // If an exception is thrown within the `try` block, the `catch` block will recieve the
      // transmitted data, which usually contains an error message.
      // You can have multiple catch blocks but for standard functions all we need is
      // `std::exception`.
      // The ampersand ("&") indicates that our variable `e` will be a reference
      // to an `std::exception`. The `const` keyword means we are referencing a "constant" object,
      // meaning we cannot modify it. You always want to catch exceptions by reference because it
      // allows many kinds to be caught under one name, and you almost always want to catch them
      // as a constant.
      catch (std::exception const & e) {
        // The `std::exception` class has a function `what` that returns the error message.
        cout << e.what() << endl;
      }
    }
  }
}
