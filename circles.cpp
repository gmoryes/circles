#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

const int QtreeParametr = 10;
const double eps = 1e-10;
const double eps_min_rect = 1e-2;
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
    int color;
    Rectangle() {
        this->color = 0;
    }
    Rectangle(Point p1, Point p2, int index, int color) {
        this->p1 = p1;
        this->p2 = p2;
        this->index = index;
        this->color = color;
    }
    double sqare() {
       return fabs(p2.x - p1.x) * fabs(p2.y - p1.y); 
    }
};
bool IsPointInsideCirle (Sirlce a, Point p) {
    return dist2(a.centr, p) < a.r * a.r;
}
bool IsRectEqual(Rectangle a, Rectangle b) {
    if (fabs(a.p1.x - b.p1.x) < eps && fabs(a.p1.y - b.p1.y) < eps &&
        fabs(a.p2.x - b.p2.x) < eps && fabs(a.p2.y - b.p2.y) < eps) return true;
    return false;
}

bool IsRectCrossRect(Rectangle a, Rectangle b) {
    bool byXleft = false, byXright = false, byYdown = false, byYup = false;
    if (b.p1.x <= a.p2.x && a.p2.x <= b.p2.x) byXright = true;
    if (b.p1.x <= a.p1.x && a.p1.x <= b.p2.x) byXleft = true;
    if (b.p1.y <= a.p1.y && a.p1.y <= b.p2.y) byYdown = true;
    if (b.p1.y <= a.p2.y && a.p2.y <= b.p2.y) byYup = true;
    return ((byXleft || byXright) && (byYup || byYdown));
}
bool IsRectInsideCirlce (Rectangle rec, Sirlce a) {
    Point p = rec.p1;
    if (!IsPointInsideCirle(a, p)) return false;
    p.x = rec.p2.x;
    if (!IsPointInsideCirle(a, p)) return false;
    p = rec.p2;
    if (!IsPointInsideCirle(a, p)) return false;
    p.x = rec.p1.x;
    if (!IsPointInsideCirle(a, p)) return false;
    return true;
}
struct Vertex {
    Rectangle self;
    vector<Rectangle> cover;
    int size;
    Rectangle objects[QtreeParametr];
    Vertex * UR; //up and right and so on
    Vertex * UL;
    Vertex * DR;
    Vertex * DL;
    Vertex() {
        this->size = 0;
    }
    Vertex(Rectangle a) {
        this->self = a;
        this->size = 0;
    }
};
typedef Vertex * pvertex;
Sircle big_boss;
Sircle cross_big_boss;
void insert(pvertex root, Rectangle toAdd) {
    if (!IsRectCrossRect(root->self, toAdd)) return;

    if (root->self.color != 0) {
        //текущий прямоугольник уже покрашен во что-то
        return;
    }
    if (root->sqare() < eps_min_rect) {
        //если площадь текущего прямоугольника маленькая, забить хуй на нее
        return;
    }
    if (UR == NULL) {
        //если еще детей нет
        if (IsRectInsideCircle(root->self, big_boss) && IsRectInsideCircle(root->self, cross_big_boss)) {
            //прямоугольник лежит внутри обеих окружностей
            root->color = cross_big_boss->index;   
        }
    }

}

double findBorder(Sircle A, Sircle B, double l, double r, bool byX) {
    double mid;
    while (fabs(r - l) > eps) {
        mid = (r + l) / 2;
        bool cond = byX ? (fabs(A.centr.x - mid) < A.r && fabs(B.centr.x - mid) < B.r) :
                          (fabs(A.centr.y - mid) < A.r && fabs(B.centr.y - mid) < B.r);
        if (cond) {
            r = mid;
        } else {
            l = mid;
        }
    }
    return r;
}

double * findBorders(Sircle A, Sircle B, Point p1, Point p2) {
    double * res = (double*) malloc(sizeof(double) * 4);
    /*
    +----0----+
    |         |
    3         1
    |         |
    +----2----+
    */
    Point centr_point = (p1 + p2) / 2;
    res[0] = findBorder(A, B, centr_point.y + max(A.r, B.r), centr_point.y, false);
    res[1] = findBorder(A, B, centr_point.x + max(A.r, B.r), centr_point.x, true);
    res[2] = findBorder(A, B, centr_point.y - max(A.r, B.r), centr_point.y, false);
    res[3] = findBorder(A, B, centr_point.x - max(A.r, B.r), centr_point.x, true);
    return res;
}

int main(void) {
    return 0;
}
