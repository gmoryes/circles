#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

const int QtreeParametr = 10;
const int maxN = 1e6;
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
    int color;
    Rectangle() {
        this->color = 0;
    }
    Rectangle(Point p1, Point p2, int color) {
        this->p1 = p1;
        this->p2 = p2;
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
    //если текущуя вершина никак не пересекается с данным прямоугольником
    if (!IsRectCrossRect(root->self, toAdd)) return;

    //в первспективе хранить тут массив цветов, в которые покаршен текущий прямоугольник
    if (root->self.color != 0) {
        //текущий прямоугольник уже покрашен во что-то
        return;
    }
    if (root->sqare() < eps_min_rect) {
        //если площадь текущего прямоугольника маленькая, забить хуй на него
        return;
    }
    if (UR == NULL) {
        //если еще детей нет
        if (IsRectInsideCircle(root->self, big_boss) && IsRectInsideCircle(root->self, cross_big_boss)) {
            //прямоугольник лежит внутри обеих окружностей
            root->color = cross_big_boss->index;   
            return;
        } else {
            //иначе надо хитрить с его детьми
            double half_width  = (root->self.p2.x - root->self.p1.x) / 2;
            double half_height = (root->self.p2.y - root->self.p1.y) / 2;
            root->UR = new Vertex(Rectangle( Point( root->self.p1.x + half_width, root->self.p1.y + half_height), root->self.p2), root->color);
            root->UL = new Vertex(Rectangle( Point( root->self.p1.x, root->self.p1.y + half_height), Point( root->self.p2.x - half_width, root->self.p2.y)), root->color);
            root->DR = new Vertex(Rectangle( Point( root->self.p1.x + half_width, root->self.p1.y), Point( root->self.p2.x, root->self.p1.x + half_height)), root->color);
            root->DL = new Vertex(Rectangle( root->self.p1, Point( root->self.p1.x + half_width, root->self.p1.y + half_height)), root->color); 
        }
    }
    //идем в детей
    insert(root->UR, toAdd);
    insert(root->UL, toAdd);
    insert(root->DR, toAdd);
    insert(root->DL, toAdd);
}

double findBorder(Sircle A, Sircle B, double l, double r, bool byX) {
    //бинпоиском ищем границу
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
    //ищет касающийся прямоугольник
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

////////////////////////

Sicle circles[maxN];

//////////////////////

int main(void) {
    FILE * circles_file = fopen("input", "r");
    FILE * config = fopen("config", "r");
    int is_debug = 0;
    if (config) {
        fscanf(config, "%d", &is_debug);
    }
    if (!circles_file) cerr << "No file \"input\"";
    double x, y, r;
    int n = 0;
    if (is_debug) {
        cout << "Start read" << endl;
    }
    while (fscanf(circles_file, "%lf %lf %lf", &x, &y, &r)) {
        circles[n] = Sircle( Point(x, y), r);
        n++;
    }
    if (is_debug) {
        cout << "Read end" << endl;
    }
    int id;
    //для разных типов запросов
    char type;
    pair<bool, pair <Point, Point> *> cross_res;
    if (is_debug) {
        cout << "Type id of circle" << endl;
    }
    double * borders;
    Rectangle toAdd;
    Vertex * color_tree;
    double zapac = 5;
    while (1) {
        scanf("%d", &id);
        big_boss = circles[id];
        color_tree = new Vertex( Rectangle( Point(circle[id].centr.x - circle[id].r - zapac, circle[id].centr.y - circle[id].r - zapac),
                                            Point(circle[id].centr.x + circle[id].r + zapac, circle[id].centr.y + circle[id].r + zapac)), 0); 
        for (int i = 0; i < n; i++) {
            if (i != id) {
                cross_res = crossSircle(circles[id], circles[i]);
                if (cross_res.first) {
                    cross_big_boss = circles[i];
                    if (cross_res.second) {
                        borders = findBorders(circles[id], circles[i], cross_res.second->first, cross_res.second->second);
                        toAdd = Rectangle( Point(borders[3], borders[2]), Point(borders[1], borders[0]));
                        insert(color_tree, toAdd);
                    } else {
                        borders = (double
                    }
                }
            }
        }
    }

    

    return 0;
}
