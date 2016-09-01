#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct s {
    int val;
    s() {};
    s(int val) {
        this->val = val;
    }
    int func() {
        return this->val * this->val;
    }
};

int main(void) {
    s var = s(5);

    cout << var.func();
    return 0;
}
