#include"gogogo++.h"
using std::runtime_error;
using std::cout;
using std::cerr;
using std::endl;
using std::cin;
class abc
{
private:
	int num = 0;
public:
	abc addabc(abc& nihao);
};
abc abc::addabc(abc& nihao)
{
	cout << "hello" << endl;
	return nihao;
}
int main()
{
	return 0;
}