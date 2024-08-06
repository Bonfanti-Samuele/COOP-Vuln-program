#include <stdio.h>
#include<iostream>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <Windows.h>

class Iterated_class {
public:
    virtual void v_call() = 0;
};

class Main_loop_gadget {
private:
    Iterated_class** a;
    size_t size_a;
public:
    /* ... */
    virtual ~Main_loop_gadget() {
        for (size_t i = 0; i < size_a; i++)
            a[i]->v_call();
    }
};

class Arith_gadget {
private:
    size_t a, b, c;
public:
    size_t sum;
    virtual void arith_call() {
        sum = a + b + c;
    }
};

class Load_r64_gadget {
private:
    size_t a, b, c;
public:
    virtual float load_call() {
        return (float)(a * 5 + b * 3 + c * 2) / 10;
    }
};

struct Write_gadget {
    char* buffer;
    size_t len;
    /* ... */
    virtual void set(char* s) {
        strncpy(buffer, s, len);
    }
};

struct Read_gadget {  //this doesn't really matter
    char* buffer;
    size_t len;
    /* ... */
    virtual void set(char* s) {
        strncpy(s, buffer, len);
    }
};

class Tmp {};

class Write_cond_gadget {
private:
    std::list<Tmp> tmp;
public:
    virtual bool w_cond_call(Tmp &t) {
        if (tmp.empty()) return false;
        t = tmp.back();
        return true;
    }
};

class Odd_even_cond_gadget {
private:
    DWORD64 a;              //num tested
    DWORD64 pad = 0;        //inv_ret_gad
    DWORD64 pad2 = 0;       //vtable
    char* arg = 0;      //a
    DWORD64 pad3 = 0;       //callback
    char* addr = 0;     //new a (optional)

public:
    virtual void oe_cond_call() {
        if (a % 2) return;
        arg = addr;
        return;
    }
};

class Iterated_class2 {
public:
    virtual void call(int a) {};
};

class Main_loop_args_gadget {
private:
    Iterated_class2** a;
    size_t size_a;
    int id;
public:
    /* ... */
    virtual ~Main_loop_args_gadget() {
        for (size_t i = 0; i < size_a; i++)
            a[i]->call(id);
    }
};

class Invoked_gadget {
private:
    char* a = 0;
    int (*callback)(char* a) = 0;

public:
    virtual void trigger(char* a1) {
        callback(a);

    }
};

class Invoked_return_gadget {
private:
    WORD (*callback)() = 0;
    DWORD64 pad;
    DWORD64 a = 0;
public:
    virtual void trigger() {
        a = callback();
    }
};
