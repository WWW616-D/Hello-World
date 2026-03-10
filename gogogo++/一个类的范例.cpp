#include "gogogo++.h"
using namespace std;
int add(int a,GameCharacter people)
{
    people.health += a;
}
// ============ 1. 最简单的类 ============
class GameCharacter {
    friend int add(int a, GameCharacter people);
    // 2. 访问控制：private和public
private:
    // 3. 成员变量（属性）
    string name;
    int level;
    int health;
    int attack;
    int defense;
    int experience;

public:
    // ============ 4. 构造函数和析构函数 ============
    // 默认构造函数
    GameCharacter() :
        name("无名氏"),
        level(1),
        health(100),
        attack(10),
        defense(5),
        experience(0)
    {
        cout << name << " 被创建了！" << endl;
    }

    // 带参数的构造函数
    GameCharacter(string charName, int startLevel = 1) {
        name = charName;
        level = startLevel;
        health = 100 * level;
        attack = 10 * level;
        defense = 5 * level;
        experience = 0;
        cout << name << "(Lv." << level << ") 诞生了！" << endl;
    }
    GameCharacter() = default;
    // 析构函数
    ~GameCharacter() {
        cout << name << " 离开了游戏..." << endl;
    }

    // ============ 5. 成员函数（方法） ============
    // 显示状态
    void showStatus() {
        cout << "\n=== " << name << " 的状态 ===" << endl;
        cout << "等级: Lv." << level << endl;
        cout << "生命: " << health << "/" << (100 * level) << endl;
        cout << "攻击: " << attack << endl;
        cout << "防御: " << defense << endl;
        cout << "经验: " << experience << "/" << (level * 100) << endl;
    }

    // 攻击敌人
    void attackEnemy(GameCharacter& enemy) {
        cout << "\n" << name << " 攻击 " << enemy.name << "！" << endl;

        int damage = attack - enemy.defense;
        if (damage < 5) damage = 5;  // 最小伤害

        enemy.takeDamage(damage);

        if (!enemy.isAlive()) {
            gainExperience(50);
        }
    }

    // 受到伤害（私有成员只能通过公有函数访问）
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;

        cout << name << " 受到 " << damage << " 点伤害" << endl;
        cout << "剩余生命: " << health << endl;

        if (!isAlive()) {
            cout << "💀 " << name << " 倒下了！" << endl;
        }
    }

    // 治疗
    void heal(int amount) {
        int maxHealth = 100 * level;
        health += amount;
        if (health > maxHealth) health = maxHealth;

        cout << name << " 恢复了 " << amount << " 点生命" << endl;
        cout << "当前生命: " << health << "/" << maxHealth << endl;
    }

    // 获得经验
    void gainExperience(int exp) {
        experience += exp;
        cout << name << " 获得 " << exp << " 点经验" << endl;

        // 检查是否升级
        while (experience >= level * 100) {
            levelUp();
        }
    }

    // 检查是否存活
    bool isAlive() {
        return health > 0;
    }

    // ============ 6. Getter和Setter ============
    // 获取名字（const成员函数，不修改对象）
    string getName() const {
        return name;
    }

    // 设置名字
    void setName(string newName) {
        cout << name << " 改名为 " << newName << endl;
        name = newName;
    }

    // 获取等级
    int getLevel() const {
        return level;
    }

    // 获取经验
    int getExperience() const {
        return experience;
    }

private:
    // ============ 7. 私有辅助函数 ============
    // 升级（只能在类内部调用）
    void levelUp() {
        level++;
        int oldMaxHealth = 100 * (level - 1);
        int newMaxHealth = 100 * level;

        health += (newMaxHealth - oldMaxHealth);  // 升级回血
        attack += 5;
        defense += 3;
        experience = 0;  // 经验重置

        cout << "\n🎉 " << name << " 升级了！" << endl;
        cout << "现在是 Lv." << level << "！" << endl;
        cout << "攻击力+" << 5 << "，防御力+" << 3 << endl;
    }
};
class king:public GameCharacter
{
public:
    king():GameCharacter(),money(100)
    {
        cout << "一个王降临了" << endl;
    }
    int getLevel() const {
        return 100;
    }
private:
    int money;
};
// ============ 8. 使用类的示例 ============
int main_111() {
    cout << "=== 游戏开始 ===" << endl;

    // 创建角色（调用构造函数）
    GameCharacter hero("勇者", 1);
    GameCharacter monster("哥布林", 1);

    // 显示初始状态
    hero.showStatus();
    monster.showStatus();

    // 战斗！
    cout << "\n=== 战斗开始 ===" << endl;

    hero.attackEnemy(monster);  // 勇者攻击哥布林

    if (monster.isAlive()) {
        monster.attackEnemy(hero);  // 哥布林反击
    }

    // 治疗
    cout << "\n=== 治疗时间 ===" << endl;
    hero.heal(30);

    // 再战！
    cout << "\n=== 第二回合 ===" << endl;
    hero.attackEnemy(monster);

    // 显示最终状态
    cout << "\n=== 战斗结束 ===" << endl;
    hero.showStatus();
    monster.showStatus();

    // 使用Getter

    cout << "\n英雄信息：" << endl;
    cout << "名字: " << hero.getName() << endl;
    cout << "等级: " << hero.getLevel() << endl;
    cout << "经验: " << hero.getExperience() << endl;

    // 使用Setter
    hero.setName("传奇勇者");
    cout << "新名字: " << hero.getName() << endl;

    // 创建更多角色
    cout << "\n=== 新角色登场 ===" << endl; 
    GameCharacter mage("法师", 3);
    mage.showStatus();

    // 尝试直接访问私有成员（会编译错误！）
    // hero.health = 9999;  // ❌ 错误：'health'是私有的
    // hero.levelUp();      // ❌ 错误：'levelUp'是私有的

    cout << "\n=== 游戏结束 ===" << endl;
    // 离开作用域时自动调用析构函数

    return 0;
}