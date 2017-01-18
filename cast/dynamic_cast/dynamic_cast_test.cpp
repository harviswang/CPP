/*
 * dynamic_cast用于动态转型
 * 用法:
 * T1 obj;
 * T2* pObj   = dynamic_cast<T2*>(&obj); 转换为T2指针, 失败返回NULL
 * T2& refObj = dynamic_cast<T2&)(obj);  转换为T2引用,失败抛出bad_cast异常
 * 要求:
 * 被转换对象obj的列型必须是多态类型(polymorphic), 即T1必须公有继承自其它类, 
 * 或者T1拥有虚函数(继承或自定义)
 * 
 * 在多态类型之间转换, 分为3中类型
 * 1. 子类向基类的向上转型(Up Cast)
 * 2. 基类向子类的向下转型(Down Cast)
 * 3. 横向转型(Cross Cast)
 * 
 * 向上转型是多态的基础, 不需要借助任何的特殊的方法, 只用将子类的指针或引用赋给基类的指针
 * 或引用即可.
 */
#include <stdio.h>

static void up_cast_test();
static void down_cast_test();
static void cross_cast_test();
static void cross_cast2_test();

int main(int argc, char *argv[])
{
    up_cast_test();
    down_cast_test();
    cross_cast_test();
    cross_cast2_test();
    return 0;
}

static void up_cast_test()
{
    class watch {
    public:
        watch(int price, int color): price(price), color(color) {}
        void dump() {
            printf("price: %d\n", price);
            printf("color: %d\n", color);
        }
    public:
        int price;
        int color;
    };
    
    class paywatch : public watch {
    public:
        paywatch(int price, int color): watch(price, color) {}
        void pay(int money) { printf("pay money: %d\n", money); }
    };
    
    paywatch f1(999, 0);
    watch& w = dynamic_cast<watch&>(f1);
    w.dump();
    watch *w2 = dynamic_cast<watch*>(&f1);
    w2->dump();
    watch *w3 = &f1;
    w3->dump();
    watch w4 = f1;
    w4.dump();
}

static void down_cast_test()
{
    class Animal {
    public:
        void running() { printf("running()\n"); }
    };
    
    class Fish : public Animal {
    public:
        Fish() { age = 1000; }
        void swimming() { printf("swim()\n"); }
        int age;
    };
    
    Animal animal;
    Fish* fish = (Fish*)(&animal);
    printf("&animal = %p\n", &animal);
    printf("fish = %p\n", fish);
    fish->running();
    fish->swimming();
    printf("fish->age = %d\n", fish->age); // 结果不确定
}

static void cross_cast_test()
{
    class Shape {
    public:
        Shape() {}
        virtual ~Shape() {}
        virtual void draw() const = 0;
    };
    
    class Rollable {
    public:
        virtual ~Rollable() {}
        virtual void roll() = 0;
    };
    
    class Circle : public Shape, public Rollable {
    public:
        void draw() const {}
        void roll() {}
    };
    
    class Square : public Shape {
    public:
        void draw() const {}
    };
    
    Shape *pShape = new Square(); // 基类指针指向子类, 不需要强制类型转换
    Rollable *pRollable = dynamic_cast<Rollable*>(pShape);
    printf("pShape = %p\n", pShape);
    printf("pRollable1 = %p\n", pRollable);
    
    Circle *pCircle = new Circle();
    Shape *pShape2 = pCircle;
    Rollable *pRollable2 = dynamic_cast<Rollable*>(pShape2);
    printf("pCircle = %p\n", pCircle);
    printf("pShape2 = %p\n", pShape2);
    printf("pRollable2 = %p\n", pRollable2);
}

class QObject {

};

class IInteraction {
public:
    virtual int start() = 0;
    static IInteraction* GetInstance()
    {
        return s_instance ? s_instance : NULL;
    }

protected:
    IInteraction() { }
    ~IInteraction() { }
    static IInteraction *s_instance;

private:
    IInteraction(const IInteraction&);
    IInteraction& operator=(const IInteraction&);
};

class StdInteraction : public QObject, public IInteraction {
public:
    int start() { printf("StdInteraction::start()\n"); return 0; }
    static IInteraction* GetInstance() {
        if (!IInteraction::s_instance) {
            IInteraction::s_instance = new StdInteraction();
        }
        return IInteraction::s_instance; 
    }
};

class SocketInteraction : public QObject, public IInteraction {
public:
    int start() { printf("SocketInteraction::start()\n"); return 0; }
    static IInteraction* GetInstance() {
        if (!IInteraction::s_instance) {
            s_instance = new StdInteraction();
        }
        return s_instance;
    }
};

class Burner : public QObject {
public:
    IInteraction *ia;
    StdInteraction *std_ia;
    SocketInteraction *soc_ia;
};
 
static void cross_cast2_test()
{
    Burner core_burner;
//    core_burner.ia = StdInteraction::GetInstance();
    (void)core_burner;
//    core_burner.ia->start();
//    printf("%p\n", dynamic_cast<SocketInteraction *>(core_burner.ia));
//    SocketInteraction si;
//    si.start();
//    printf("IInteraction::s_instance = %p\n", IInteraction::GetInstance());
}