#include <iostream>
#include "Foo.h"

using std::cout;
using std::endl;

Foo::Foo(int id) : id(id) {
    cout << "Standard ctor for Foo " << id << endl;
}

Foo::~Foo() {
    cout << "Dtor for Foo " << id << endl;    
}

int Foo::get_id() const {
    return id;
}
    
Foo::Foo(Foo const & rhs) : id(rhs.id) {
    cout << "Copy ctor for Foo, using existing id " << id << endl;
}

Foo & Foo::operator =(Foo const & rhs) {
    cout << "Standard assignment operator for Foo, using existing id " << rhs.id << endl;
    id = rhs.id;
    return *this;
}

Foo::Foo(Foo && rhs) : id(std::move(rhs.id)) {
    cout << "Move ctor for Foo, using existing id " << id << endl;
}

Foo & Foo::operator =(Foo && rhs) {
    cout << "Move assignment operator for Foo, using existing id " << rhs.id << endl;
    id = std::move(rhs.id);
    return *this;
}
