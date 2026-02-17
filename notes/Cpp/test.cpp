#include <iostream>
using namespace std;
class Animal {
public:
    virtual void speak() {     // 虚函数
        cout << "Animal sound" << endl;
    }
};
class Dog : public Animal {
public:
    void speak() override {
        cout << "Dog: Woof" << endl;
    }
};
class Cat : public Animal {
public:
    void speak() override {
        cout << "Cat: Meow" << endl;
    }
};
int main(void){
    Animal* a1 = new Dog();
    Animal* a2 = new Cat();

    a1->speak();   // 调用 Dog 的版本
    a2->speak();   // 调用 Cat 的版本
    delete a1;
    delete a2;
    return 0;
}