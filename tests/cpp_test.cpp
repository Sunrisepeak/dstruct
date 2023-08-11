template <typename T>
class A {
public:
    using Type = T;
};

template <typename T>
struct B : public A<T> {
    B() {
        typename B::Type i;
    }
};

#include <iostream>
#include <map>
struct C {
    C() {
        std::cout << "C()" << std::endl;
    }

    ~C() {
        std::cout << "~C()" << std::endl;
    }
};

#include <iostream>

class Parent {
private:
    int privateData;

public:
    Parent(int data) : privateData(data) {}

    friend void FriendFunction(const Parent& obj);
};

/* ok
void FriendFunction(const Parent& obj) {
    std::cout << "Accessing privateData from FriendFunction: " << obj.privateData << std::endl;
}
*/

template <typename T>
void FriendFunction(const T& obj) {
    //std::cout << "Accessing privateData from FriendFunction: " << obj.privateData << std::endl;
}

class Child : public Parent {
public:
    Child(int data) : Parent(data) {}
};

int main() {

    std::cout << "\n\nTesting: " << __FILE__ << std::endl;
// test 1
    B<int> b;
// test 2
    using IntPtr = int *;
    int val = 1;
    const IntPtr ptr = &val;
    *ptr = 1;
// test 3: reference to dynamic obj
    C *cPtr = new C();
    {
        C &refC = *cPtr;
    } // refC lifetime end
    delete cPtr;
// test 4: cpp17 for
    std::map<int,int> tmp {{1,2},{3,4}};
    for(auto& x:tmp) {
        std::cout << x.first << x.second;
    }
// test 5: Parent friend function
    Child child(42);
    FriendFunction(child); //通过子类对象调用 friend 函数

// test6: const T &
{ // ok
    int val = 5;
    const int &data = val;
    std::cout << &val << " -- " << &data << std::endl;
}
    return 0;
}