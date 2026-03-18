#include"gogogo++.h"
using std::runtime_error;
using std::cout;
using std::cerr;
using std::endl;
using std::cin;
double divide(int x, int y)
{
	if (y == 0)
	{
		throw runtime_error("balabala");
	}
	return x / y;
}

int main_wrong()
{
	std::cerr << "hello" << std::endl;
	int x;
	int y;
	cin >> x >> y;
	try
	{
		double value = divide(x, y);
		cout << "两个数的商为:" << value;
	}
	catch (const runtime_error& e)
	{
		cerr << e.what() << endl;
	}
	return 0;
}