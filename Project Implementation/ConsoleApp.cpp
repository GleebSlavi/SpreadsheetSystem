#include "ConsoleApp.h"

void ConsoleApp::help() const
{
	std::cout << "                                         Available commands!" << std::endl;
	std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "HELP                                Opens help menu that shows all available commands" << std::endl;
	std::cout << "OPERATORS                           Opens operator menu that shows all operators that can be used" << std::endl;
	std::cout << "SET *address* *expression*          Adds (or changes if there is already) the expression at the entered address" << std::endl;
	std::cout << "PRINT VAL *address*                 Prints the value of the entered address" << std::endl;
	std::cout << "PRINT EXPR *address*                Prints the expression at the entered address" << std::endl;
	std::cout << "PRINT VAL ALL                       Prints the value of every cell in the table" << std::endl;
	std::cout << "PRINT EXPR ALL                      Prints the expression of every cell in the table" << std::endl;
	std::cout << "SAVE *csv file*                     Save the content of the table in csv file" << std::endl;
	std::cout << "LOAD *csv file*                     Loads the content of csv file in the table" << std::endl;
	std::cout << "++ *address*                        If there is an expression at that address, adds +1 to it" << std::endl;
	std::cout << "-- *address*                        If there is an expression at that address, adds -1 to it" << std::endl;
	std::cout << "EXIT                                Exits the program" << std::endl;
	std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
}

void ConsoleApp::operators() const
{
	std::cout << "                                         Available operators!" << std::endl;
	std::cout << "-------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "*number1* + *number2*                          returns the sum of two numbers" << std::endl;
	std::cout << "*number1* - *number2*                          returns the difference of two numbers" << std::endl;
	std::cout << "*number1* * *number2*                          returns the product of two numbers" << std::endl;
	std::cout << "*number1* / *number2*                          returns the  " << std::endl;
	std::cout << "*number1* == *number2*                         checks if the 2 numbers are equal" << std::endl;
	std::cout << "*number1* != *number2*                         checks if the 2 numbers are ot equal" << std::endl;
	std::cout << "*number1* >= *number2*                         check if the first number is bigger than or equal to the second number" << std::endl;
	std::cout << "*number1* <= *number2*                         check if the first number is smaller than or equal to the second number" << std::endl;
	std::cout << "*number1* > *number2*                          check if the first number is bigger than the second number" << std::endl;
	std::cout << "*number1* < *number2*                          check if the first smaller is bigger than or equal to the second number" << std::endl;
	std::cout << "*condition* ? *number1* : *number2*            if condition is true return first number, else returns second" << std::endl;
	std::cout << "!*number*                                      if number is true returns false, else the oposite" << std::endl;
	std::cout << "*number1* && *number2*                         returns true if first and second numbers return true, else false" << std::endl;
	std::cout << "*number1* || *number2*                         returns true if first or second numbers return true, else false" << std::endl;
	std::cout << "-------------------------------------------------------------------------------------------------------------------------" << std::endl;
}
