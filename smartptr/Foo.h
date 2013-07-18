#ifndef smartptr_Foo_h
#define smartptr_Foo_h

// Make a super-simple class that we can use for testing.
class Foo {
    int id;
public:
    Foo(int id);
    ~Foo();
    
    int get_id() const; // Make it easy to see which Foo we're working with.
    
    Foo(Foo const & rhs); // Standard copy ctor.
    Foo & operator =(Foo const & rhs); // Standard copy assignment operator.
    
    // Move ctor. (This won't compile unless your compiler supports C++-11-style
    // "move semantics". It is necessary for some std::unique_ptr optimizations.)
    Foo(Foo && rhs);
    
    // Move assignment operator. (This won't compile unless your compiler supports C++-11-style
    // "move semantics". It is necessary for some std::unique_ptr optimizations.)
    Foo & operator =(Foo && rhs);
};

#endif
