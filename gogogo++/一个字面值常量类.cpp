#include <iostream>

// 定义一个字面值常量类
class Point {
public:
    int x;
    int y;

    // 1. constexpr 构造函数：允许在编译期创建对象
    // 成员必须在初始化列表中初始化
    constexpr Point(int xVal, int yVal) : x(xVal), y(yVal) {}

    // 2. constexpr 成员函数：允许在编译期调用
    constexpr int sum() const {
        return x + y;
    }

    // 析构函数必须是默认的（隐式或显式 = default）
    ~Point() = default;
};

int main_already() {
    // ✅ 合法：在编译期创建一个 Point 对象
    constexpr Point p1(10, 20);

    // ✅ 合法：在编译期调用成员函数，结果也是常量
    constexpr int s = p1.sum(); // s 在编译期就是 30

    // ✅ 合法：用作数组大小（需要常量表达式）
    int arr[s];

    std::cout << "Sum: " << s << std::endl;

    // ❌ 错误示例：如果构造函数不是 constexpr
    // constexpr Point p2(1, 2); // 会报错

    return 0;
}