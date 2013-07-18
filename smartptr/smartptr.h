#ifndef smartptr_smartptr_h
#define smartptr_smartptr_h

// A super-simple smart pointer. This class is about as sophisticated as
// the original std::auto_ptr, which means that it has problems when you try
// to put it into vectors, assign one to another, and so forth.
template <class T> class smartptr
{
    T* ptr;
public:
    smartptr(T* p = 0) : ptr(p)  { }
    ~smartptr()                  { delete ptr; }
    T& operator*()               { return *ptr; }
    T* operator->()              { return ptr; }
    T* get()                     { return ptr; }
    operator bool() const        { return ptr != 0; }
};

#endif
