#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../Project Implementation/Table.h"

TEST_CASE("Set and evaluate expressions")
{
	Table table;

	SECTION("Evaluable linear expressions")
	{
		table.set_table(1, 1);

		table.set_expression({ 0,0,"(5+2)*7" });
		REQUIRE(table.get_expression(0, 0) == "(5+2)*7");
		REQUIRE(table.get_value(0, 0) == 49);

		table.set_expression({ 0,0,"(10 - 2)/4" });
		REQUIRE(table.get_expression(0, 0) == "(10 - 2)/4");
		REQUIRE(table.get_value(0, 0) == 2);
	}

	SECTION("Evaluable boolean expressions")
	{
		table.set_table(1, 1);

		table.set_expression({ 0,0,"4 == 5" });
		REQUIRE(table.get_expression(0, 0) == "4 == 5");
		REQUIRE(table.get_value(0, 0) == 0);

		table.set_expression({ 0,0,"4 < 5" });
		REQUIRE(table.get_expression(0, 0) == "4 < 5");
		REQUIRE(table.get_value(0, 0) == 1);

		table.set_expression({ 0,0, "2 > 3 ? 2 : 3" });
		REQUIRE(table.get_expression(0, 0) == "2 > 3 ? 2 : 3");
		REQUIRE(table.get_value(0, 0) == 3);

		table.set_expression({ 0,0, "!0" });
		REQUIRE(table.get_expression(0, 0) == "!0");
		REQUIRE(table.get_value(0, 0) == 1);
	}

	SECTION("Negative and positive numbers")
	{
		table.set_table(1, 1);

		table.set_expression({ 0,0,"-2" });
		REQUIRE(table.get_expression(0, 0) == "-2");
		REQUIRE(table.get_value(0, 0) == -2);

		table.set_expression({ 0,0,"+5" });
		REQUIRE(table.get_expression(0, 0) == "+5");
		REQUIRE(table.get_value(0, 0) == 5);

		table.set_expression({ 0,0,"25*(-2)" });
		REQUIRE(table.get_expression(0, 0) == "25*(-2)");
		REQUIRE(table.get_value(0, 0) == -50);

		table.set_expression({ 0,0,"-(-2)" });
		REQUIRE(table.get_expression(0, 0) == "-(-2)");
		REQUIRE(table.get_value(0, 0) == 2);

		table.set_expression({ 0,0,"-(-5+4)" });
		REQUIRE(table.get_expression(0, 0) == "-(-5+4)");
		REQUIRE(table.get_value(0, 0) == 1);
	}

	SECTION("Empty expressions")
	{
		table.set_table(1, 1);

		table.set_expression({ 0,0,"" });
		REQUIRE(table.get_expression(0, 0) == "");
		REQUIRE(table.get_value(0, 0) == 0);
	}

	SECTION("Invalid operators")
	{
		table.set_table(1, 1);

		table.set_expression({ 0,0,"5^4" });
		REQUIRE(table.get_expression(0, 0) == "5^4");
		REQUIRE(table.get_value(0, 0) == 0);

		table.set_expression({ 0,0,"7=7" });
		REQUIRE(table.get_expression(0, 0) == "7=7");
		REQUIRE(table.get_value(0, 0) == 0);
	}

	SECTION("Non-evaluable expressions")
	{
		table.set_table(1, 1);

		table.set_expression({ 0,0,"abc" });
		REQUIRE(table.get_expression(0, 0) == "abc");
		REQUIRE(table.get_value(0, 0) == 0);

		table.set_expression({ 0,0,"q + v = qv" });
		REQUIRE(table.get_expression(0, 0) == "q + v = qv");
		REQUIRE(table.get_value(0, 0) == 0);
	}
}

TEST_CASE("Addressing")
{
	Table table;

	SECTION("Expressions with absolute addresses")
	{
		table.set_table(2, 2);

		table.set_expression({ 0,0,"5 + 2" });
		table.set_expression({ 0,1,"R1C1 - 12" });
		REQUIRE(table.get_expression(0, 1) == "R1C1 - 12");
		REQUIRE(table.get_value(0, 1) == -5);

		table.set_expression({ 1,0, "R1C1 + R1C2" });
		REQUIRE(table.get_expression(1, 0) == "R1C1 + R1C2");
		REQUIRE(table.get_value(1, 0) == 2);
	}

	SECTION("Expressions with relative addresses")
	{
		table.set_table(2, 2);

		table.set_expression({ 0,0,"R[+1]C[0] == 5" });
		table.set_expression({ 1,0,"5" });
		REQUIRE(table.get_expression(0, 0) == "R[+1]C[0] == 5");
		REQUIRE(table.get_value(0, 0) == 1);

		table.set_expression({ 0,0,"R[+1]C[0] + R[0]C[+1]" });
		table.set_expression({ 0,1,"2 > 1 ? 1 : 0" });
		REQUIRE(table.get_expression(0, 0) == "R[+1]C[0] + R[0]C[+1]");
		REQUIRE(table.get_value(0, 0) == 6);
	}

	SECTION("Invalid addresses")
	{
		table.set_table(1, 1);

		table.set_expression({ 0,0,"R+1C0" });
		REQUIRE(table.get_expression(0, 0) == "R+1C0");
		REQUIRE(table.get_value(0, 0) == 0);

		table.set_expression({ 0,0,"R[+1C0]" });
		REQUIRE(table.get_expression(0, 0) == "R[+1C0]");
		REQUIRE(table.get_value(0, 0) == 0);

		table.set_expression({ 0,0,"R2A0" });
		REQUIRE(table.get_expression(0, 0) == "R2A0");
		REQUIRE(table.get_value(0, 0) == 0);

		table.set_expression({ 0,0,"25C0" });
		REQUIRE(table.get_expression(0, 0) == "25C0");
		REQUIRE(table.get_value(0, 0) == 0);

		table.set_expression({ 0,0,"R100000A0" });
		REQUIRE(table.get_expression(0, 0) == "R100000A0");
		REQUIRE(table.get_value(0, 0) == 0);
	}
}

TEST_CASE("Add plus and minus one")
{
	Table table;

	SECTION("Plus or minus one")
	{
		table.set_table(1, 1);

		table.set_expression({ 0,0,"2" });
		table.plus_plus(0, 0);
		REQUIRE(table.get_expression(0, 0) == "(2)+1");
		REQUIRE(table.get_value(0, 0) == 3);

		table.set_expression({ 0,0,"4*5" });
		table.plus_plus(0, 0);
		REQUIRE(table.get_expression(0, 0) == "(4*5)+1");
		REQUIRE(table.get_value(0, 0) == 21);

		table.set_expression({ 0,0,"1" });
		table.minus_minus(0, 0);
		REQUIRE(table.get_expression(0, 0) == "(1)-1");
		REQUIRE(table.get_value(0, 0) == 0);

		table.set_expression({ 0,0,"-5" });
		table.minus_minus(0, 0);
		table.plus_plus(0, 0);
		REQUIRE(table.get_expression(0, 0) == "((-5)-1)+1");
		REQUIRE(table.get_value(0, 0) == -5);
	}

	SECTION("Non-evaluable expression")
	{
		table.set_table(1, 1);

		table.set_expression({ 0,0,"abv" });
		REQUIRE_THROWS(table.plus_plus(0, 0));

		table.set_expression({ 0,0,"R+1C0 + 5" });
		REQUIRE_THROWS(table.minus_minus(0, 0));
	}
}

TEST_CASE("Sum and count")
{
	Table table;

	SECTION("Sum")
	{
		table.set_table(2, 2);

		table.set_expression({ 0,0,"+5" });
		table.set_expression({ 1,0,"15*2" });
		table.set_expression({ 0,1,"abv" });
		REQUIRE(table.sum({ 0,0 }, { 1,1 }) == 35);

		table.set_expression({ 1,1,"!4 ? 6 : -6" });
		REQUIRE(table.sum({ 0,0 }, { 1,1 }) == 29);
	}

	SECTION("Count")
	{
		table.set_table(2, 2);

		table.set_expression({ 0,0,"+5" });
		table.set_expression({ 1,0,"" });
		table.set_expression({ 0,1,"abv" });
		REQUIRE(table.count({ 0,0 }, { 1,1 }) == 2);

		table.set_expression({ 0,0,"" });
		REQUIRE(table.count({ 0,0 }, { 1,1 }) == 1);
	}
}