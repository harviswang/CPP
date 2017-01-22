#include <set>
#include <iostream>

static bool set_empty_test(void);
static bool set_size_test(void);
static bool set_max_size_test(void);
static bool set_insert_test(void);
static bool set_erase_test(void);
static bool set_swap_test(void);
static bool set_clear_test(void);
static bool set_copy_constructor_test(void);
template <typename T>
static bool set_dump(std::set<T>& s);

int main(int argc, char *argv[])
{
    std::cout << "set test..." << std::endl;
    set_empty_test();
    set_size_test();
    set_max_size_test();
    set_insert_test();
    set_erase_test();
    set_erase_test();
    set_swap_test();
    set_clear_test();
    set_copy_constructor_test();

    return 0;
}

static bool set_empty_test()
{
    std::set<int> s;
    if (s.empty()) {
        std::cout << "set<int> s; s is empty" << std::endl;
    }
    set_dump(s);
    return true;
}

static bool set_size_test()
{
    std::set<char> digital_set;
    std::cout << digital_set.size() << std::endl;
    set_dump(digital_set);
    return true;
}

template <typename T>
static bool set_dump(std::set<T>& s)
{
    std::cout << "set address:" << &s << std::endl;
    std::cout << "empty():" << s.empty() << std::endl;
    std::cout << "size():" << s.size() << std::endl;
    std::cout << "max_size():" << s.max_size() << std::endl;

    std::cout << std::endl;
    return true;
}

static bool set_max_size_test()
{
    std::set<bool> check;
    std::cout << check.max_size() << std::endl;
    return true;
}

static bool set_insert_test()
{
    std::set<std::string> week;
    week.insert("Sunday");
    week.insert("Monday");
    week.insert("Tuesday");
    week.insert("Wednesday");
    week.insert("Thursday");
    week.insert("Friday");
    week.insert("Saturday");

    set_dump(week);
    return true;
}

static bool set_erase_test(void)
{
    std::set<int> day;
    std::set<int>::size_type size, size_new;
    for (int i = 0; i < 365; i++) {
        day.insert(i);
    }
    size = day.size();

    day.erase(10);

    size_new = day.size();
    if (size == size_new + 1) {
        set_dump(day);
    }
    return true;
}

static bool set_swap_test()
{
    std::set<int> seven;
    std::set<int> week;
    std::set<int>::iterator itor;
    for (int i = 0; i < 7; i++) {
        seven.insert(i);
        week.insert(i * i);
    }

    seven.swap(week); // == week.swap(seven);
    for (itor = seven.begin(); itor != seven.end(); itor++) {
        std::cout << *itor << std::endl;
    }

    for (itor = week.begin(); itor != week.end(); itor++) {
        std::cout << *itor << std::endl;
    }

    return true;
}

//
// std;;set::clear() clear the content of set
//

static bool set_clear_test()
{
    std::set<int> pencilBox;
    set_dump(pencilBox);
    for (int i = 1; i < 100; i++) {
        pencilBox.insert(i);
    }
    pencilBox.clear();
    set_dump(pencilBox);

    return true;
}

static bool set_copy_constructor_test()
{
    std::set<int> box;
    for (int i = 99; i > 50; i--) {
        box.insert(i);
    }
    set_dump(box);
    std::set<int> box2 = box;
    set_dump(box2);
    return true;
}