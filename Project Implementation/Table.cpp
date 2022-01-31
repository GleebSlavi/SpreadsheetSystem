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

int Table::get_operator_priority(const std::string& operation) const
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
	else if (operation == "!")
	{
		return 8;
	}
	return 0;
}

bool Table::apply_operator(std::stack<int>& numbers, const std::string& operation) const
{
	if (numbers.empty())
	{
		return false;
	}
	int right_number = numbers.top();
	numbers.pop();

	if (operation == "!")
	{
		numbers.push(!right_number);
		return true;
	}
	int left_number = numbers.top();
	numbers.pop();

	if (operation == "?:")
	{
		if (numbers.empty())
		{
			return false;
		}

		int condition = numbers.top();
		numbers.pop();
		if (condition > 0)
		{
			numbers.push(left_number);
		}
		else
		{
			numbers.push(right_number);
		}

		return true;
	}
	else if (operation == "/" && right_number == 0)
	{
		return false;
	}

	numbers.push(apply(left_number, right_number, operation));
	return true;
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
	else
	{
		return left || right;
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

bool Table::is_valid_address(const Data& expr_cell, size_t& index, Data& address) const
{
	if (expr_cell.expression[index] != '[' && !is_digit(expr_cell.expression[index]))
	{
		return false;
	}

	if (expr_cell.expression[index] == '[')
	{
		if (!is_relative_address(expr_cell, ++index, address))
		{
			return false;
		}
	}
	else if (is_digit(expr_cell.expression[index]))
	{
		if (!is_absolute_address(expr_cell.expression, index, address))
		{
			return false;
		}
		--index;
		--address.row;
		--address.column;
	}

	if ((address.row >= 0 && address.row < max_rows) && (address.column >= 0 && address.column < max_columns))
	{
		return true;
	}
	return false;
}

bool Table::is_relative_address(const Data& expr_cell, size_t& index, Data& address) const
{
	if (!is_relative(expr_cell.expression[index]))
	{
		return false;
	}

	char symbol = ' ';
	if (expr_cell.expression[index] != '0')
	{
		symbol = expr_cell.expression[index];
		++index;
	}

	if (!is_digit(expr_cell.expression[index]))
	{
		return false;
	}
	address.row = get_number(expr_cell.expression, index);

	if (symbol == '+')
	{
		address.row = expr_cell.row + address.row;
	}
	else
	{
		address.row = expr_cell.row - address.row;
	}

	if (expr_cell.expression[index] != ']' || expr_cell.expression[index + 1] != 'C'
		|| expr_cell.expression[index + 2] != '[' || !is_relative(expr_cell.expression[index + 3]))
	{
		return false;
	}
	index += 3;

	if (expr_cell.expression[index] != '0')
	{
		symbol = expr_cell.expression[index];
		++index;
	}

	if (!is_digit(expr_cell.expression[index]))
	{
		return false;
	}
	address.column = get_number(expr_cell.expression, index);

	if (symbol == '+')
	{
		address.column = expr_cell.column + address.column;
	}
	else
	{
		address.column = expr_cell.column - address.column;
	}

	if (expr_cell.expression[index] != ']')
	{
		return false;
	}
	return true;
}

bool Table::is_relative(char symbol) const
{
	return symbol == '+' || symbol == '-' || symbol == '0';
}

bool Table::is_absolute_address(const std::string& expression, size_t& index, Data& address) const
{
	address.row = get_number(expression, index);

	if (expression[index] != 'C' || !is_digit(expression[index + 1]))
	{
		return false;
	}
	address.column = get_number(expression, ++index);

	return true;
}

void Table::set_expression(const Data& data)
{
	if (table[data.row].empty() && !data.expression.empty())
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
				if (data.expression.empty())
				{
					table[data.row].erase(table[data.row].begin() + i);
					return;
				}
				table[data.row][i].expression = data.expression;
				return;
			}
			else if (data.column < table[data.row][i].column && !data.expression.empty())
			{
				table[data.row].insert(table[data.row].begin() + i, data);
				return;
			}

			if (i == size - 1)
			{
				if (!data.expression.empty())
				{
					table[data.row].push_back(data);
				}
			}
		}
	}
}

int Table::get_value(int row, int column) const
{
	const Data* cell = get_cell(row, column);
	int value;

	if (cell != nullptr && shunting_yard({ cell->row,cell->column,cell->expression }, value))
	{
		return value;
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
	return "";
}

bool Table::shunting_yard(const Data& cell, int& value) const
{
	std::stack<int> numbers;
	std::stack<std::string> operators;

	size_t size = cell.expression.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (cell.expression[i] == ' ')
		{
			continue;
		}
		else if (is_digit(cell.expression[i]))
		{
			numbers.push(get_number(cell.expression, i));
			--i;
		}
		else if (cell.expression[i] == 'R')
		{
			Data data;
			if (!is_valid_address(cell, ++i, data))
			{
				return false;
			}
			numbers.push(get_value(data.row, data.column));
		}
		else if (is_open_bracket(cell.expression[i]))
		{
			std::string bracket(1, cell.expression[i]);
			operators.push(bracket);
		}
		else if (is_close_bracket(cell.expression[i]))
		{
			while (!operators.empty() && !is_open_bracket(operators.top()[0]))
			{
				apply_operator(numbers, operators.top());
				operators.pop();
			}

			if (operators.empty() || !matching_brackets(operators.top()[0], cell.expression[i]))
			{
				return false;
			}
			operators.pop();
		}
		else
		{
			std::string operation(1, cell.expression[i]);
			int number;

			if (operation == "?")
			{
				if (!if_else_operator(cell, i, number))
				{
					return false;
				}
				operation += cell.expression[i];
			}
			else if (operation == "-" || operation == "+")
			{
				if (plus_or_minus_number(cell, i, numbers))
				{
					continue;
				}
			}
			else if (is_operator(cell.expression[i + 1]) && cell.expression[i+1] != '+' && cell.expression[i + 1] != '-')
			{
				operation += cell.expression[i++];
			}

			if (get_operator_priority(operation) == 0)
			{
				return false;
			}

			while (!operators.empty() && get_operator_priority(operators.top()) > get_operator_priority(operation))
			{
				apply_operator(numbers, operators.top());
				operators.pop();
			}

			if (operation == "?:")
			{
				numbers.push(number);
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
		return false;
	}
	value = numbers.top();
	numbers.pop();

	if (!numbers.empty())
	{
		return false;
	}
	return true;
}

bool Table::if_else_operator(const Data& expr_data, size_t& index, int& int_number) const
{
	std::string number;
	int stopper = expr_data.expression.size() - 1;

	do
	{
		++index;
		number += expr_data.expression[index];

	} while (index != stopper && expr_data.expression[index] != ':');
	number.pop_back();

	if (index == stopper || !shunting_yard({ expr_data.row,expr_data.column,number }, int_number))
	{
		return false;
	}
	return true;
}

bool Table::plus_or_minus_number(const Data& data, size_t& index, std::stack<int>& numbers) const
{
	char symbol = data.expression[index];
	size_t back_index = index;

	while (back_index != 0 && data.expression[back_index - 1] == ' ')
	{
		--back_index;
	}

	int number;
	if(back_index != 0)
	{
		--back_index;
		if (is_close_bracket(data.expression[back_index]) || is_digit(data.expression[back_index]))
		{
			return false;
		}
		number = get_number(data.expression, ++index);
		--index;
	}
	else if (is_open_bracket(data.expression[index + 1]))
	{
		++index;
		char bracket = data.expression[index];
		std::string expression;
		do
		{
			expression += data.expression[index];
			++index;
		} while (!matching_brackets(bracket, data.expression[index]));
		expression += data.expression[index];

		if (!shunting_yard({ data.row,data.column,expression }, number))
		{
			return false;
		}
	}
	else
	{
		number = get_number(data.expression, ++index);
		--index;
	}

	if (symbol == '-')
	{
		number *= (-1);
	}

	numbers.push(number);
	return true;
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
					result += get_value(i,j);
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

const Data* Table::get_cell(int row, int column) const
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

Data* Table::get_cell(int row, int column)
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

void Table::plus_plus(int row, int column)
{
	plus_or_minus_one(row, column, [&](std::string& expression) {expression += ")+1"; });
}

void Table::minus_minus(int row, int column)
{
	plus_or_minus_one(row, column, [&](std::string& expression) {expression += ")-1"; });
}

void Table::print_all_values() const
{
	print([&](int row, int column) {return get_value(row, column); });
}

void Table::print_all_expressions() const
{
	print([&](int row, int column) {return get_expression(row, column); });
}

void Table::set_table(int row, int column)
{
	max_rows = row;
	max_columns = column;
	table.resize(row);
}

template <class Predicate>
void Table::plus_or_minus_one(int row, int column, Predicate predicate)
{
	Data* cell = get_cell(row, column);
	int number;

	if (cell != nullptr && shunting_yard({ cell->row,cell->column,cell->expression }, number))
	{
		cell->expression.insert(0, "(");
		predicate(cell->expression);
	}
	else
	{
		throw std::logic_error("Expression can't be turned into number!");
	}
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
			std::cout<< predicate(i, j) << ", ";
		
		}
		std::cout << std::endl;
	}
}

std::ostream& operator<<(std::ostream& out, const Table& table)
{
	out << table.max_rows << ' ' << table.max_columns;
	out << std::endl;

	for (size_t i = 0; i < table.max_rows; ++i)
	{
		for (size_t j = 0; j < table.max_columns; ++j)
		{
			std::string expression = table.get_expression(i, j);
			out << expression;
			if (!expression.empty())
			{
				out << ';';
			}
			if (j < table.max_columns - 1)
			{
				out << ",";
			}
		}
		out << std::endl;
	}

	return out;
}

std::istream& operator>>(std::istream& in, Table& table)
{
	in >> table.max_rows >> table.max_columns;
	in.ignore();
	table.table.resize(table.max_rows);

	std::string line;
	int row = 0;
	while (std::getline(in, line))
	{
		std::stringstream stream(line);
		std::string expression;

		int column = 0;
		while (std::getline(stream, expression, ','))
		{
			if (!expression.empty())
			{
				expression.pop_back();
				table.set_expression({ row,column,expression });
			}
			++column;
		}
		++row;
	}

	return in;
}
