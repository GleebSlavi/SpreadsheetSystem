#include "Table.h"

bool Table::is_digit(char symbol) const
{
	return symbol >= '0' && symbol <= '9';
}

int Table::get_number(const std::string& expression, size_t& index) const
{
	int number = 0;
	while (is_digit(expression[index]))
	{
		number = number * 10 + (expression[index] - '0');
		++index;
	}
	return number;
}

int Table::get_operation_priority(const std::string& operation) const
{
	if (operation == "?:")
	{
		return 1;
	}
	else if (operation == "||")
	{
		return 2;
	}
	else if (operation == "&&")
	{
		return 3;
	}
	else if (operation == "==" || operation == "!=")
	{
		return 4;
	}
	else if (operation == ">" || operation == ">=" || operation == "<" || operation == "<=")
	{
		return 5;
	}
	else if (operation == "+" || operation == "-")
	{
		return 6;
	}
	else if (operation == "*" || operation == "/")
	{
		return 7;
	}
	return 0;
}

void Table::apply_operator(std::stack<int>& numbers, const std::string& operation) const
{
	if (numbers.empty())
	{
		throw std::logic_error("Incorrect expression");
	}
	int right_number = numbers.top();
	numbers.pop();

	if (operation == "!")
	{
		numbers.push(!right_number);
		return;
	}
	
	if (numbers.empty())
	{
		throw std::logic_error("Incorrect expression");
	}
	int left_number = numbers.top();
	numbers.pop();

	numbers.push(apply(left_number, right_number, operation));
}

int Table::apply(int left, int right, const std::string& operation) const
{
	if (operation == "+")
	{
		return left + right;
	}
	else if (operation == "-")
	{
		return left - right;
	}
	else if (operation == "*")
	{
		return left * right;
	}
	else if (operation == "/")
	{
		return left / right;
	}
	else if (operation == ">")
	{
		return left > right;
	}
	else if (operation == "<")
	{
		return left < right;
	}
	else if (operation == "!=")
	{
		return left != right;
	}
	else if (operation == "==")
	{
		return left == right;
	}
	else if (operation == ">=")
	{
		return left >= right;
	}
	else if (operation == "<=")
	{
		return left <= right;
	}
	else if (operation == "&&")
	{
		return left && right;
	}
	else if (operation == "||")
	{
		return left || right;
	}
	else
	{
		throw std::logic_error("Incorrect operator");
	}
}

bool Table::is_open_bracket(char symbol) const
{
	return symbol == '(' || symbol == '[' || symbol == '{';
}

bool Table::is_close_bracket(char symbol) const
{
	return symbol == ')' || symbol == ']' || symbol == '}';
}

bool Table::is_bool_operator(char symbol) const
{
	return symbol == '!' || symbol == '=' || symbol == '&' || symbol == '|' ||
		symbol == '>' || symbol == '<' || symbol == '?' || symbol == ':';
}

bool Table::is_arithmetic_operator(char symbol) const
{
	return symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/';
}

bool Table::is_operator(char symbol) const
{
	return is_bool_operator(symbol) || is_arithmetic_operator(symbol);
}

bool Table::matching_brackets(char left, char right) const
{
	return left == '(' && right == ')' || left == '[' && right == ']' || left == '{' && right == '}';
}

Table::Table(int rows, int columns)
{
	max_rows = rows;
	max_columns = columns;
	table.resize(rows);
}

void Table::set_expression(const Data& data)
{
	if (table[data.row].empty())
	{
		table[data.row].push_back(data);
	}
	else
	{
		size_t size = table[data.row].size();
		for (size_t i = 0; i < size; ++i)
		{
			if (data.column == table[data.row][i].column)
			{
				table[data.row][i].expression = data.expression;
				return;
			}
			else if (data.column < table[data.row][i].column)
			{
				table[data.row].insert(table[data.row].begin() + i, data);
				return;
			}
		}
		table[data.row].push_back(data);
	}
}

int Table::get_value(int row, int column) const
{
	const Data* cell = get_cell(row, column);
	if (cell != nullptr)
	{
		return shunting_yard(cell->expression);
	}
	return 0;
}

int Table::shunting_yard(const std::string& expression) const
{
	std::stack<int> numbers;
	std::stack<std::string> operators;

	size_t size = expression.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (expression[i] == ' ')
		{
			continue;
		}
		else if (is_digit(expression[i]))
		{
			numbers.push(get_number(expression, i));
			--i;
		}
		else if (is_open_bracket(expression[i]))
		{
			std::string bracket(1, expression[i]);
			operators.push(bracket);
		}
		else if (is_close_bracket(expression[i]))
		{
			while (!operators.empty() && !is_open_bracket(operators.top()[0]))
			{
				apply_operator(numbers, operators.top());
				operators.pop();
			}

			if (operators.empty() || !matching_brackets(operators.top()[0], expression[i]))
			{
				throw std::logic_error("Incorrect expression");
			}
			operators.pop();
		}
		else
		{
			std::string operation(1, expression[i]);
			if (is_operator(expression[i + 1]))
			{
				operation += expression[i + 1];
				++i;
			}

			if (get_operation_priority(operation) == 0)
			{
				throw std::logic_error("Incorrect operator");
			}
			
			while (!operators.empty() && get_operation_priority(operators.top()) > get_operation_priority(operation))
			{
				apply_operator(numbers, operators.top());
				operators.pop();
			}
			operators.push(operation);
		}
	}

	while (!operators.empty())
	{
		apply_operator(numbers, operators.top());
		operators.pop();
	}

	if (numbers.empty())
	{
		throw std::logic_error("Incorrect expression");
	}
	int value = numbers.top();
	numbers.pop();

	if (!numbers.empty())
	{
		throw std::logic_error("Incorrect expression");
	}

	return value;
}

const Table::Data* Table::get_cell(int row, int column) const
{
	size_t size = table[row].size();
	for (size_t i = 0; i < size; ++i)
	{
		if (table[row][i].column == column)
		{
			return &table[row][i];
		}
	}
	return nullptr;
}

Table::Data* Table::get_cell(int row, int column)
{
	return const_cast<Data*>(std::as_const(*this).get_cell(row, column));
}

int Table::get_table_rows() const
{
	return max_rows;
}

int Table::get_table_columns() const
{
	return max_columns;
}

void Table::plus_or_minus_one(int row, int column, char symbol)
{	Data* cell = get_cell(row, column);
	if (cell != nullptr)
	{
		cell->expression.insert(0, "(");
		if (symbol == '+')
		{
			cell->expression += ")+1";
		}
		else
		{
			cell->expression += ")-1";
		}
	}
	std::cout << "Cell with row " << row << " and column " << column << " has no expression!" << std::endl;
}

