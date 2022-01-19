#include "ConsoleApp.h"

int main()
{
	Table table;
	table.set_table(2, 3);
	table.set_expression({ 1,2,"2+3" });
	table.set_expression({ 0,0,"R[+1]C[+2] > R0C2" });
	std::cout << table.get_value(0, 0);

}