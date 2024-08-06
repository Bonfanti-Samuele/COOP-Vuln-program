#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <strsafe.h>
#include <iostream>
#include "vfGadget.cpp"


class Base {};
class Child : public Base {
public:
    virtual void call();
};

void* print_stack_pointer() {
    DWORD64* p = NULL;
    return (DWORD64*)&p;
}

BOOL hexstring_parse(const char* string, BYTE* dest, int size) {

    int len = (int)strlen(string);
    if (len / 4 > size)
        return false;

    for (int i = 0; i < len / 4; i++) {
        int v;
        if (sscanf_s(string + (i * 4 + 2), "%2x", &v) != 1)
            break;
        dest[i] = (unsigned char)v;
    }

    return true;
}

int main(int argc, char* argv[]) {
    printf("\n[-] COOP Vulnerable Application\n");

    //vfgadget import
    Invoked_gadget inv_g;
    Main_loop_gadget main_g;
    Invoked_return_gadget inv_r_g;
    Odd_even_cond_gadget od_cond_g;
    //Write_cond_gadget cond_g;

    //allocating local buffer
    char* coopbuf = (char*)VirtualAlloc(NULL, 0x8000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    DWORD64 coop = (DWORD64)(coopbuf);
    if (coop == NULL) {
        printf("\n[!] COOP buffer generation failed");
        exit(1);
    }

    Child* child2 = static_cast<Child*>((void*)coopbuf);

    //leaking buffer address
    printf("\n[*] COOP buffer at: 0x%p", coopbuf);

    //getting coop chain from input
    printf("\n[-] Insert the buffer size:");
    int size;
    scanf("%d", &size);
    char input_buff[1000];
    BYTE in_b[999];
    /*if (size > 999) {
        printf("Too long");
        return 0;
    }
    for (int i = 0; i < size; i++) {
        std::cin >> std::hex >> in_b[i];
    }
    */

    printf("\n[-] Insert the buffer:");
    scanf("%999s", &input_buff);

    hexstring_parse(input_buff, in_b, size);

    memcpy((DWORD64*)coopbuf, (DWORD64*)in_b, size);

    // triggering type confusion
    printf("\t\t\t[*] hijacking flow control: ");
    child2->call();
    printf("\tOK\n");
    return 0;
}

//A019004001000000 00622daffa7f0000 "cmd.exe /C calc"
//7ffaaf270000 + 66200