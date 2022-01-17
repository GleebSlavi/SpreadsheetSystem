#include "ConsoleApp.h"

void ConsoleApp::get_stream_strings(const std::string& stream, std::string& first, std::string& second, std::string& third) const
{
	std::stringstream string_stream(stream);
	std::string word;

	while (string_stream >> word)
	{
		if (first.empty())
		{
			first = word;
		}
		else if (second.empty())
		{
			second = word;
		}
		else if (third.empty())
		{
			third = word;
		}
	}
}

bool ConsoleApp::row_and_column_check(int& number, const std::string& address, size_t& index) const
{
	if (address[index] != 'R' || address[index] != 'C')
	{
		return false;
	}
	++index;

	while (address[index] >= '0' && address[index] <= '9')
	{
		number = number * 10 + (address[index] - '0');
		++index;
	}
	return true;
}

template <class FirstFunc, class SecondFunc>
bool ConsoleApp::print_command(const std::string& third, FirstFunc all, SecondFunc single) const
{
	size_t index = 0;
	int row, column;
	if (third == "ALL")
	{
		all;
		std::cout << std::endl;
	}
	else if (valid_address_check(row, column, third, index))
	{
		std::cout << single(row, column) << std::endl;
	}
	else
	{
		std::cout << "Invalid command!" << std::endl;
	}
}

void ConsoleApp::save(const std::string& file) const
{
	std::ofstream out(file);
	if (out.is_open())
	{
		out << table;
		std::cout << "File saved successfully!" << std::endl;
	}
	else
	{
		std::cout << "Couldn't open file!" << std::endl;
	}
}

void ConsoleApp::load(const std::string& file)
{
	std::ifstream in(file);
	if (in.is_open())
	{
		table.clear_table();
		in >> table;
		is_loaded = true;
		std::cout << "Table loaded successfully!" << std::endl;
	}
	else
	{
		std::cout << "Couln't open file!" << std::endl;
	}

}

void ConsoleApp::help() const
{
	std::cout << "                                         Available commands!" << std::endl;
	std::cout << "---------------------------------------------------------------------------------------------------------------------" << std::endl;
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
	std::cout << "SUM *address* *address*             Returns the sum of the cells in the rectangle formed by the addresses" << std::endl;
	std::cout << "COUNT *address* *address*           Returns the count of the not empty cells in the rectangle formed by the addresses" << std::endl;
	std::cout << "EXIT                                Exits the program" << std::endl;
	std::cout << "---------------------------------------------------------------------------------------------------------------------" << std::endl;
}

void ConsoleApp::operators() const
{
	std::cout << "                                         Available operators!" << std::endl;
	std::cout << "-------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "*number* + *number*                          returns the sum of two numbers" << std::endl;
	std::cout << "*number* - *number*                          returns the difference of two numbers" << std::endl;
	std::cout << "*number* * *number*                          returns the product of two numbers" << std::endl;
	std::cout << "*number* / *number*                          returns the  " << std::endl;
	std::cout << "*number* == *number*                         checks if the 2 numbers are equal" << std::endl;
	std::cout << "*number* != *number*                         checks if the 2 numbers are ot equal" << std::endl;
	std::cout << "*number* >= *number*                         check if the first number is bigger than or equal to the second number" << std::endl;
	std::cout << "*number* <= *number*                         check if the first number is smaller than or equal to the second number" << std::endl;
	std::cout << "*number* > *number*                          check if the first number is bigger than the second number" << std::endl;
	std::cout << "*number* < *number*                          check if the first smaller is bigger than or equal to the second number" << std::endl;
	std::cout << "*condition* ? *number* : *number*            if condition is true return first number, else returns second" << std::endl;
	std::cout << "!*number*                                      if number is true returns false, else the oposite" << std::endl;
	std::cout << "*number* && *number*                         returns true if first and second numbers return true, else false" << std::endl;
	std::cout << "*number* || *number*                         returns true if first or second numbers return true, else false" << std::endl;
	std::cout << "-------------------------------------------------------------------------------------------------------------------------" << std::endl;
}

void ConsoleApp::exit()
{
	char symbol;
	std::cout << "Do you want to save the table? Enter Y/N: ";
	std::cin >> symbol;

	if (symbol == 'Y' || symbol == 'y')
	{
		std::string file;
		std::cout << "Enter file to save to: ";
		std::cin >> file;

		if (csv_file_check(file))
		{
			save(file);
		}
		else
		{
			std::cout << "You entered other file than .csv file, so the changes won't be saved!" << std::endl;
		}
	}
	else if (symbol != 'N' || symbol != 'n')
	{
		std::cout << "You entered wrong symbol, so the changes won't be saved!" << std::endl;
	}
	std::cout << "Goodbye!" << std::endl;
}

bool ConsoleApp::valid_address_check(int& row, int& column, const std::string& address, size_t index)
{
	return row_and_column_check(row, address, index) && row_and_column_check(column, address, index) &&
		row >= 0 && row < table.get_table_rows() && column >= 0 && column < table.get_table_columns();
}

bool ConsoleApp::csv_file_check(const std::string& file) const
{
	size_t size = file.size();
	return file[size - 1] == 'v' && file[size - 2] == 's' && file[size - 3] == 'c' && file[size - 4] == '.';
}

void ConsoleApp::app()
{
	while (1)
	{
		std::string stream_command;
		std::cout << "Enter command: ";
		std::getline(std::cin, stream_command);

		std::string first, second, third;
		get_stream_strings(stream_command, first, second, third);
		
		size_t index = 0;
		int row, column;
		if (first == "HELP")
		{
			help();
		}
		else if (first == "OPERATORS")
		{
			operators();
		}
		else if (first == "SET")
		{
			if (is_loaded)
			{
				if (valid_address_check(row, column, second, index))
				{
					table.set_expression({ row, column, third });
				}
				else
				{
					std::cout << "Incorrect address!" << std::endl;
				}
			}
			else
			{
				std::cout << "Please load table!" << std::endl;
			}
		}
		else if (first == "PRINT")
		{
			if (is_loaded)
			{
				if (second == "VAL")
				{
					print_command(third, [&]() {table.print_all_values(); }, [&](int row, int column) {return table.get_value(row, column); });
				}
				else if (second == "EXPR")
				{
					print_command(third, [&]() {table.print_all_expressions(); }, [&](int row, int column) {return table.get_expression(row, column); });
				}
				else
				{
					std::cout << "Wrong command!" << std::endl;
				}
			}
			else
			{
				std::cout << "Please load table!" << std::endl;
			}
		}
		else if (first == "SAVE")
		{
			if (is_loaded)
			{
				if (!csv_file_check(second))
				{
					save(second);
				}
				else
				{
					std::cout << "Please enter .csv file!" << std::endl;
				}
			}
			else
			{
				std::cout << "Please load table!" << std::endl;
			}
		}
		else if (first == "LOAD")
		{
			if (!csv_file_check(second))
			{
				load(second);
			}
			else
			{
				std::cout << "Please enter .csv file!" << std::endl;
			}
		}
		else if (first == "++")
		{
			if (is_loaded)
			{
				if (valid_address_check(row, column, second, index))
				{
					table.plus_plus(row, column);
				}
				else
				{
					std::cout << "Incorrect address!" << std::endl;
				}
			}
			else
			{
				std::cout << "Please load table!" << std::endl;
			}
		}
		else if (first == "--")
		{
			if (is_loaded)
			{
				if (valid_address_check(row, column, second, index))
				{
					table.minus_minus(row, column);
				}
				else
				{
					std::cout << "Incorrect address!" << std::endl;
				}
			}
		}
		else if (first == "SUM" || first == "COUNT")
		{
			if (is_loaded)
			{
			
			}
			else
			{
				std::cout << "Please load table!" << std::endl;
			}
		}
		else if (first == "EXIT")
		{
			exit();
			return;
		}
		else
		{
			std::cout << "Wrong command!" << std::endl;
		}
	}
}

void ConsoleApp::print()
{
	table.print_all_values();
}
