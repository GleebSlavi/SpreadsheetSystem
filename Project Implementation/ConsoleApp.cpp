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
			if (first == "SAVE" || first == "LOAD")
			{
				std::string route;
				std::getline(string_stream, route);
				second += route;
			}
		}
		else if (third.empty())
		{
			third = word;
		}
	}
}

void ConsoleApp::save(const std::string& file)
{
	std::ofstream out(file);
	if (out.is_open())
	{
		out << table;
		is_saved = true;
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
	std::cout << "HELP                                Opens help menu that shows all available commands" << std::endl; //W
	std::cout << "OPERATORS                           Opens operator menu that shows all operators that can be used" << std::endl; //W
	std::cout << "SET *address* *expression*          Adds (or changes if there is already) the expression at the entered address" << std::endl; //W
	std::cout << "PRINT VAL *address*                 Prints the value of the entered address" << std::endl; //W
	std::cout << "PRINT EXPR *address*                Prints the expression at the entered address" << std::endl; //w
	std::cout << "PRINT VAL ALL                       Prints the value of every cell in the table" << std::endl; //W
	std::cout << "PRINT EXPR ALL                      Prints the expression of every cell in the table" << std::endl; //W
	std::cout << "SAVE *csv file*                     Save the content of the table in csv file" << std::endl; // W
	std::cout << "LOAD *csv file*                     Loads the content of csv file in the table" << std::endl; //W
	std::cout << "++ *address*                        If there is an expression at that address, adds +1 to it" << std::endl; //W
	std::cout << "-- *address*                        If there is an expression at that address, adds -1 to it" << std::endl; //W
	std::cout << "SUM *address* *address*             Returns the sum of the cells in the rectangle formed by the addresses" << std::endl; //W
	std::cout << "COUNT *address* *address*           Returns the count of the not empty cells in the rectangle formed by the addresses" << std::endl; //W
	std::cout << "EXIT                                Exits the program" << std::endl; //W
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
	if (!is_saved)
	{
		char symbol;
		std::cout << "Table not saved! Do you want to save it? Enter Y/N: ";
		while (1)
		{
			std::cin >> symbol;
			std::cout << std::endl;
			if (symbol != 'Y' && symbol != 'N')
			{
				std::cout << "Wrong symbol! Please enter Y/N: ";
			}
		}
		
		std::string file;
		if (symbol == 'Y')
		{
			std::cout << "Enter file to save it to: ";
			std::getline(std::cin, file);
			save(file);
		}
	}
	std::cout << "Goodbye!" << std::endl;
}

bool ConsoleApp::csv_file_check(const std::string& file) const
{
	size_t size = file.size();
	return file[size - 1] == 'v' && file[size - 2] == 's' && file[size - 3] == 'c' && file[size - 4] == '.';
}

bool ConsoleApp::get_and_check_address(Data& data, const std::string& address) const
{
	size_t index = 0;
	if (address[index] != 'R' && (address[index + 1] < '0' || address[index + 1] > '9'))
	{
		return false;
	}
	++index;

	data.row = table.get_number(address, index);
	if (address[index] != 'C' && (address[index + 1] < '0' || address[index + 1] > '9'))
	{
		return false;
	}
	++index;
	data.column = table.get_number(address, index);

	if (data.row < 0 || data.row >= table.get_table_rows() || data.column < 0 || data.column >= table.get_table_columns())
	{
		return false;
	}
	return true;
}

void ConsoleApp::run_app()
{
	while (1)
	{
		std::string stream_command;
		std::cout << "> ";
		std::getline(std::cin, stream_command);

		std::string first, second, third;
		get_stream_strings(stream_command, first, second, third);
		
		Data data;
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
				if (get_and_check_address(data,second))
				{
					table.set_expression({ data.row, data.column, third });
				}
				else
				{
					std::cout << "Invalid address!" << std::endl;
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
					if (third == "ALL")
					{
						table.print_all_values();
					}
					else if (get_and_check_address(data, third))
					{
						std::cout << table.get_value(data.row, data.column) << std::endl;
					}
					else
					{
						std::cout << "Incorrect command or invalid address!" << std::endl;
					}
				}
				else if (second == "EXPR")
				{
					if (third == "ALL")
					{
						table.print_all_expressions();
					}
					else if (get_and_check_address(data, third))
					{
						std::string expression = table.get_expression(data.row, data.column);
						if (expression.empty())
						{

							std::cout << "There is no expression!" << std::endl;
						}
						else
						{
							std::cout << expression << std::endl;
						}
					}
					else
					{
						std::cout << "Wrong command or invalid address!" << std::endl;
					}
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
				if (csv_file_check(second))
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
			if (csv_file_check(second))
			{
				load(second);
			}
			else
			{
				std::cout << "Please enter .csv file!" << std::endl;
			}
		}
		else if (first == "++" || first == "--")
		{
			if (is_loaded)
			{
				if (get_and_check_address(data, second))
				{
					try
					{
						if (first == "++")
						{
							table.plus_or_minus_one(data.row, data.column, true);
							std::cout << "Added +1 to the expression!" << std::endl;
						}
						else
						{
							table.plus_or_minus_one(data.row, data.column, false);
							std::cout << "Added -1 to the expression!" << std::endl;
						}
					}
					catch (const std::logic_error& ex)
					{
						std::cout << ex.what() << std::endl;
					}
				}
				else
				{
					std::cout << "Invalid address!" << std::endl;
				}
			}
			else
			{
				std::cout << "Please load table!" << std::endl;
			}
		}
		else if (first == "SUM" || first == "COUNT")
		{
			if (is_loaded)
			{
				Data second_data;
				if (get_and_check_address(data, second) && get_and_check_address(second_data, third))
				{
					Data temp(second_data.row, second_data.column);
					second_data.set_rows_and_columns(std::max(temp.row, data.row), std::max(temp.column, data.column));
					data.set_rows_and_columns(std::min(temp.row, data.row), std::min(temp.column, data.column));

					if (first == "SUM")
					{
						std::cout << table.sum(data, second_data) << std::endl;
					}
					else
					{
						std::cout << table.count(data, second_data) << std::endl;
					}
				}
				else
				{
					std::cout << "Invalid addresses!" << std::endl;
				}
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
		std::cout << std::endl;
	}
}
