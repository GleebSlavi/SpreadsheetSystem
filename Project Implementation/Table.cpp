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

	if (operation == "?:")
	{
		if (numbers.empty())
		{
			throw std::logic_error("Incorrect expression");
		}

		int condition = numbers.top();
		numbers.pop();
		if (condition > 1)
		{
			numbers.push(left_number);
		}
		else
		{
			numbers.push(right_number);
		}

		return;
	}

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

const std::string Table::get_expression(int row, int column) const
{
	const Data* cell = get_cell(row, column);
	if (cell != nullptr)
	{
		return cell->expression;
	}
	return "-";
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
		else if (expression[i] == 'R')
		{
			numbers.push(get_value_from_address(expression, ++i));
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
			if (expression[i] == '?')
			{
				numbers.push(get_number(expression, ++i));
				operation += expression[i];
				
			}
			else if (is_operator(expression[i + 1]))
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

int Table::get_value_from_address(const std::string& expression, size_t& index) const
{
	if (!is_digit(expression[index]))
	{
		throw std::logic_error("Incorrect expression!");
	}

	int row = get_number(expression, index);
	if (expression[index++] != 'C')
	{
		throw std::logic_error("Incorrect expression!");
	}
	int column = get_number(expression, index);
	--index;

	return get_value(row, column);
}

int Table::get_sum_or_count(const Data& first, const Data& second, bool is_sum) const
{
	int result = 0;
	for (size_t i = first.row; i <= second.row; ++i)
	{
		size_t size = table[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			if (table[i][j].column >= first.column && table[i][j].column <= second.column)
			{
				if (is_sum)
				{
					result += shunting_yard(table[i][j].expression);
				}
				else
				{
					++result;
				}
			}
			
		}
	}
	return result;
}

const Table::Data* Table::get_cell(int row, int column) const
{
	size_t size = table[row].size();
	for (size_t i = 0; i < size; ++i)
	{
		if (column < table[row][i].column)
		{
			break;
		}
		else if (table[row][i].column == column)
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

int Table::sum(const Data& first, const Data& second) const
{
	return get_sum_or_count(first, second, true);
}

int Table::count(const Data& first, const Data& second) const
{
	return get_sum_or_count(first, second, false);
}

void Table::print_all_values() const
{
	print([&](int row, int column) {return get_value(row, column); });
}

void Table::print_all_expressions() const
{
	print([&](int row, int column) {return get_expression(row, column); });
}

void Table::plus_plus(int row, int column)
{
	plus_or_minus_one(row, column, [](std::string& string) {string += ")+1"; });
}

void Table::minus_minus(int row, int column)
{
	plus_or_minus_one(row, column, [](std::string& string) {string += ")-1"; });
}

template <class Predicate>
void Table::plus_or_minus_one(int row, int column, Predicate predicate)
{	Data* cell = get_cell(row, column);
	if (cell != nullptr)
	{
		cell->expression.insert(0, "(");
		predicate(cell->expression);
	}
	else
	{
		throw std::logic_error("There is no value in the cell!");
	}
}

void Table::set_rows()
{
	table.resize(max_rows);
}

void Table::clear_table()
{
	max_rows = 0;
	max_columns = 0;
	table.clear();
}

template<class Predicate>
void Table::print(Predicate predicate) const
{
	for (int i = 0; i < max_rows; ++i)
	{
		for (int j = 0; j < max_columns; ++j)
		{
			std::cout << predicate(i, j) << "\t";
		}
		std::cout << std::endl;
	}
}

std::ostream& operator<<(std::ostream& out, const Table& table)
{
	out << table.max_rows << ' ' << table.max_columns << std::endl;
	for (size_t i = 0; i < table.max_rows; ++i)
	{
		for (size_t j = 0; j < table.max_columns; ++j)
		{
			out << table.get_expression(i, j);
			if (j != table.max_columns - 1)
			{
				out << ',';
			}
		}
		std::cout << std::endl;
	}

	return out;
}

std::istream& operator>>(std::istream& in, Table& table)
{
	in >> table.max_rows >> table.max_columns;
	in.ignore();
	table.set_rows();

	std::string line;
	int rows = 0;
	while (std::getline(in, line))
	{
		std::stringstream stream(line);
		std::string word;

		int columns = 0;
		while (std::getline(stream, word, ','))
		{
			if (word != "-")
			{
				table.set_expression({ rows,columns,word });
			}
			++columns;
		}
		++rows;
	}

	return in;
}
