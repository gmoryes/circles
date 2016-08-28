#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

const int QtreeParametr = 10;
const double eps = 1e-10;
using namespace std;

struct Point {
    double x, y;
};

struct Rectangle {
    /*
    +-----p2
    |      | 
    |      |
    |      |
    p1-----+
    */ 
    Point p1, p2;
    int index;
};

bool IsRectEqual(Rectangle a, Rectangle b) {
    if (fabs(a.p1.x - b.p1.x) < eps && fabs(a.p1.y - b.p1.y) < eps &&
        fabs(a.p2.x - b.p2.x) < eps && fabs(a.p2.y - b.p2.y) < eps) return true;
    return false;
}

bool IsRectCross(Rectangle a, Rectangle b) {
    bool byXleft = false, byXright = false, byYdown = false, byYup = false;
    if (b.p1.x <= a.p2.x && a.p2.x <= b.p2.x) byXright = true;
    if (b.p1.x <= a.p1.x && a.p1.x <= b.p2.x) byXleft = true;
    if (b.p1.y <= a.p1.y && a.p1.y <= b.p2.y) byYdown = true;
    if (b.p1.y <= a.p2.y && a.p2.y <= b.p2.y) byYup = true;
    return ((byXleft || byXright) && (byYup || byYdown));
}

struct Vertex {
    Rectangle self;
    vector<Rectangle> cover;
    int size;
    Rectangle objects[QtreeParametr];
    Vertex() {
        this->size = 0;
    }
    Vertex(Rectangle a) {
        this->self = a;
        this->size = 0;
    }
};
typedef Vertex * pvertex;
void insert(pvertex root, Rectangle toAdd) {
    if (!IsRectCross(root->self, toAdd)) return;
    toAdd.p1.x = max(root->self.p1.x, toAdd.p1.x);
    toAdd.p1.y = max(root->self.p1.y, toAdd.p1.y);
    toAdd.p2.x = min(root->self.p2.x, toAdd.p2.x);
    toAdd.p2.y = min(root->self.p2.y, toAdd.p2.y);

    if (IsRectEqual(root->self, toAdd)) {
        root->cover.push_back(toAdd);
        return;
    }
    //если он содержится (не покрывает ее) в текущей области
    if (root->size < QtreeParametr) {
        root->objects[root->size] = toAdd;
        root->size += 1;
    } else {
        //help me god
    }

}

int main(void) {
    return 0;
}
