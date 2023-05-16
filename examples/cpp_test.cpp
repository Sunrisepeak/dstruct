template<typename T>
class A {
public:
    using Type = T;
};

template<typename T>
struct B : public A<T> {
    B() {
        typename B::Type i;
    }
};

int main() {
// test 1
    B<int> b;
// test 2
    using IntPtr = int *;
    int val = 1;
    const IntPtr ptr = &val;
    *ptr = 1;
    return 0;
}