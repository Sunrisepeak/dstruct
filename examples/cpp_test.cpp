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
    B<int> b;
    return 0;
}