//
//  main.cpp
//  smartptr
//
//  Created by Daniel Hardman on 7/17/13.
//  Copyright (c) 2013 Daniel Hardman. All rights reserved.
//

#include <iostream>
#include <memory>
#include <vector>
#include "Foo.h"
#include "smartptr.h"

using std::cout;
using std::endl;

// Prototypes for functions we'll use in our tests.
void do_something_innocuous();
std::unique_ptr<Foo> make_a_foo(int id);
std::vector<std::unique_ptr<Foo>> make_many_foos(int start_id);


int main(int argc, const char * argv[])
{
    // Construct a Foo on the stack.
    Foo foo(1);

    // Construct a Foo the traditional C++ way. This ptr is not deleted,
    // so it's a memory leak.
    Foo * foo2 = new Foo(2);
    cout << "The id of foo2 is " << foo2->get_id() << endl;

    // Construct a Foo using hand-written smartptr class.
    smartptr<Foo> foo3 = new Foo(3);
    if (foo3) {
        cout << "Smartptr foo3 is not null" << endl;
        cout << "Smartptr foo3 has id " << foo3->get_id() << endl;
    } else {
        cout << "Smartptr foo3 is null" << endl;
    }
    
    // Demonstrate what happens when smartptr contains null.
    smartptr<Foo> foo4 = nullptr;
    if (foo4) {
        cout << "Smartptr foo4 is not null" << endl;
        cout << "Smartptr foo4 has id " << foo4->get_id() << endl;
    } else {
        cout << "Smartptr foo4 is null" << endl;
    }
    
    // Demonstrate what happens when smartptr is misused by lying about
    // ownership. smartptr isn't built to handle shared ownership, so
    // by creating 2 smartptr's that both think they own the same pointer,
    // we're going to cause a double free.
    smartptr<Foo> foo5 = new Foo(5);
    //smartptr<Foo> foo6 = foo5.get();  // UNCOMMENT FOR SEG FAULT
    
    // gotos are evil; they can make delete never get called
    Foo * foo6 = new Foo(6);
    goto afterFoo6;
    delete foo6; // never called; foo6 is leaked
afterFoo6:
    
    {
        Foo foo7(7);
        goto afterFoo7;
    } // Did you know that goto can't get around destructors? foo7 ISN'T leaked.
afterFoo7:
    
    // Which means that gotos can't get around smart ptrs, either...
    {
        smartptr<Foo> foo8 = new Foo(8);
        goto afterFoo8;
    }
afterFoo8:
    
    // How about exceptions?
    try {
        Foo * foo8 = new Foo(8);
        do_something_innocuous();
        delete foo8;
    } catch (...) {
        cout << "Caught exception, but foo8 wasn't destroyed" << endl;
    }
    
    // smartptr isn't quite good enough. We need something a bit fancier to do
    // stuff like this...
    
    // Pass a baton. Foo was created inside a subroutine; now it's owned by foo9
    // inside of main().
    std::unique_ptr<Foo> foo9 = make_a_foo(9);
    std::unique_ptr<Foo> foo10(foo9.release()); // pass the baton again
    cout << "After passing baton, foo9 is "
        << (foo9 ? "not null" : "null")
        << " and foo10 is "
        << (foo10 ? "not null" : "null")
        << endl;
    foo10 = nullptr; // causes Foo * to be released; Foo destructor will fire
    cout << "After reassigning, foo10 is " << (foo9 ? "not null" : "null") << endl;
    
    { // artificial scope to make vectors go away when we want
        // Vectors of smart pointers. In C++ 11 with move semantics
        std::vector<std::unique_ptr<Foo>> vec1;
        vec1.push_back(std::unique_ptr<Foo>(new Foo(11)));
        vec1.push_back(std::unique_ptr<Foo>(new Foo(12)));
        vec1.push_back(std::unique_ptr<Foo>(new Foo(13)));
        vec1.erase(vec1.begin()); // Foo 11's dtor should fire
    
        std::vector<std::unique_ptr<Foo>> vec2 = make_many_foos(14);
        cout << "We've copied a vector of smart ptrs. If move semantics worked, we shouldn't see any destructors for foo 14-" << 14 + vec2.size() - 1 << " yet." << endl;
    } // now destructors should fire
    
    // Demonstrate ref-counted shared_ptr<T>.
    std::shared_ptr<Foo> foo500_a(new Foo(500));
    std::shared_ptr<Foo> foo500_b(foo500_a);
    std::shared_ptr<Foo> foo500_c(foo500_b);
    
    // Simulate random release order on multiple threads by releasing in a
    // different order depending on current time of day.
    time_t now = time(nullptr);
    if (now % 3 == 0) {
        foo500_a.reset();
        foo500_b.reset();
    } else if (now % 3 == 1) {
        foo500_b.reset();
        foo500_c.reset();
    } else {
        foo500_c.reset();
        foo500_a.reset();
    }
    cout << "At this point, 2 of the 3 shared_ptr objects have released, but we shouldn't have seen the destructor for Foo 500." << endl;
        
    return 0;
}

// You never know when something that looks harmless will actually misbehave!
void do_something_innocuous() {
    throw 25;
}

// Demonstrate returning a smart ptr from a function.
std::unique_ptr<Foo> make_a_foo(int id) {
    return std::unique_ptr<Foo>(new Foo(id));
}

// Demonstrate returning a vector of smart pointers by *value*.
std::vector<std::unique_ptr<Foo>> make_many_foos(int start_id) {
    std::vector<std::unique_ptr<Foo>> vec;
    for (int id = start_id; id < start_id + 300; ++id) {
        vec.push_back(std::unique_ptr<Foo>(new Foo(id)));
    }
    // In C++98, this was horribly inefficient. A new vector was created in the
    // memory space of the caller. The copy constructor of the vector was then
    // called, which iterated through the entire vector and called the copy
    // constructor of every element in the old vector, to populate the new one.
    // In C++11, this is super cheap. Because the compiler can tell that vec
    // will not be used by anyone once this statement executes, and because
    // std::vector<T> includes a "move" constructor, the memory from the old
    // vector is simply moved into the new one. Expense is equivalent to
    // returning a single pointer, but now you have exception safety, memory
    // mgmt, and so forth.
    return vec;
}


