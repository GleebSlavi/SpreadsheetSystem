#include "ConsoleApp.h"

int main()
{
	/*ConsoleApp app;
	app.run_app();*/

	Table table;
	table.set_table(2, 2);
	table.set_expression({ 0, 0, "1?1:0" });
	std::cout << table.get_value(0,0);

	return 0;
}