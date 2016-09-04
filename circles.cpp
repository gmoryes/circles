#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <time.h>

const int QtreeParametr = 10;
const int maxN = 1e6;
const double eps = 1e-10;
const double eps_min_rect = 1e-3;
using namespace std;

#include "circles.h";

////////////////////////

Circle circles[maxN];

//////////////////////

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

bool IsRectEqualRect(Rectangle a, Rectangle b) {
    if (fabs(a.p1.x - b.p1.x) < eps && fabs(a.p1.y - b.p1.y) < eps &&
        fabs(a.p2.x - b.p2.x) < eps && fabs(a.p2.y - b.p2.y) < eps) return true;
    return false;
}

bool IsSnippetCrossCircle(Point a, Point b, Circle s) {
	if (IsPointInsideCirle(s, a)) return true;
	if (IsPointInsideCirle(s, b)) return true;
	if (fabs(a.x - b.x) < eps) {
		if (a.y < b.y) swap(a, b);
		if (fabs(s.centr.x - a.x) < s.r) {
			if (b.y < s.centr.y && s.centr.y < a.y) return true;
		}
	} else {
		if (a.x < b.x) swap(a, b);
		if (fabs(s.centr.y - a.y) < s.r) {
			if (b.x < s.centr.x && s.centr.x <= a.x) return true;
		}
	}
	return false;
}

bool IsCircleInsideRect(Circle s, Rectangle a) {
	if (fabs(fabs(fabs(s.centr.x - a.p1.x) + fabs(s.centr.x - a.p2.x)) - fabs(a.p1.x - a.p2.x)) < eps) {
		if (fabs(fabs(fabs(s.centr.y - a.p1.y) + fabs(s.centr.y - a.p2.y)) - fabs(a.p1.y - a.p2.y)) < eps) {
			return true;
		}
	}
	return false;
}

bool IsRectInsideCircle (Rectangle rec, Circle a) {
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

bool IsRectCrossCircle(Rectangle a, Circle s) {
	if (IsCircleInsideRect(s, a)) return true;
	if (IsRectInsideCircle(a,s)) return true;
	if (IsSnippetCrossCircle(a.p1, Point(a.p2.x, a.p1.y), s)) return true;
	if (IsSnippetCrossCircle(Point(a.p2.x, a.p1.y), a.p2, s)) return true;
	if (IsSnippetCrossCircle(a.p2, Point(a.p1.x, a.p2.y), s)) return true;
	if (IsSnippetCrossCircle(Point(a.p1.x, a.p2.y), a.p1, s)) return true;
	return false;
}

bool IsRectCrossRect(Rectangle a, Rectangle b) {
    bool byX = false, byY = false;
    if (b.p1.x <= a.p1.x && a.p1.x <= b.p2.x) byX = true;
	if (b.p1.x <= a.p2.x && a.p2.x <= b.p2.x) byX = true;
	if (b.p1.y <= a.p1.y && a.p1.y <= b.p2.y) byY = true;
	if (b.p1.y <= a.p2.y && a.p2.y <= b.p2.y) byY = true;
	if(!(byX && byY)) {
		swap(a, b);
		byX = false; byY = false;
		if (b.p1.x <= a.p1.x && a.p1.x <= b.p2.x) byX = true;
		if (b.p1.x <= a.p2.x && a.p2.x <= b.p2.x) byX = true;
		if (b.p1.y <= a.p1.y && a.p1.y <= b.p2.y) byY = true;
		if (b.p1.y <= a.p2.y && a.p2.y <= b.p2.y) byY = true;
		return byX && byY;
	} else return true;
}

struct Vertex {
    Rectangle self;
    vector<Rectangle> cover;
    int size;
    //Rectangle objects[QtreeParametr];
    Vertex * UR; //up and right and so on
    Vertex * UL;
    Vertex * DR;
    Vertex * DL;
    Vertex() {
        this->size = 0;
		this->UR = NULL;
		this->UL = NULL;
		this->DR = NULL;
		this->DL = NULL;
    }
    Vertex(Rectangle a) {
        this->self = a;
        this->size = 0;
		this->UR = NULL;
		this->UL = NULL;
		this->DR = NULL;
		this->DL = NULL;
    }
};

int big_boss;
int cross_big_boss;
int cnt = 0;
int insert(Vertex * root, Rectangle toAdd) {
    //если текущуя вершина никак не пересекается с данным прямоугольником
	if (!IsRectCrossRect(root->self, toAdd)) return root->self.color;

    //в первспективе хранить тут массив цветов, в которые покаршен текущий прямоугольник
	if (root->self.color != -1 && root->self.color != -2) {
        //текущий прямоугольник уже покрашен во что-то
		return root->self.color;
    }
    if (root->self.sqare() < eps_min_rect) {
        //если площадь текущего прямоугольника маленькая, забить хуй на него
		return root->self.color;
    }
    if (root->UR == NULL) {
        //если еще детей нет
		if (IsRectInsideCircle(root->self, circles[big_boss]) && IsRectInsideCircle(root->self, circles[cross_big_boss])) {
            //прямоугольник лежит внутри обеих окружностей
            root->self.color = cross_big_boss;   
			return root->self.color;
        } else {
            //иначе надо хитрить с его детьми
            double half_width  = (root->self.p2.x - root->self.p1.x) / 2;
            double half_height = (root->self.p2.y - root->self.p1.y) / 2;
			cnt += 4;
            root->UR = new Vertex(Rectangle( Point( root->self.p1.x + half_width, root->self.p1.y + half_height), root->self.p2, root->self.color));
            root->UL = new Vertex(Rectangle( Point( root->self.p1.x, root->self.p1.y + half_height), Point( root->self.p2.x - half_width, root->self.p2.y), root->self.color));
            root->DR = new Vertex(Rectangle( Point( root->self.p1.x + half_width, root->self.p1.y), Point( root->self.p2.x, root->self.p1.y + half_height), root->self.color));
            root->DL = new Vertex(Rectangle( root->self.p1, Point( root->self.p1.x + half_width, root->self.p1.y + half_height), root->self.color)); 
        }
    }
    //идем в детей
    int color = insert(root->UR, toAdd);
	if (color != root->self.color) root->self.color = -2;
    color = insert(root->UL, toAdd);
	if (color != root->self.color) root->self.color = -2;
    color = insert(root->DR, toAdd);
	if (color != root->self.color) root->self.color = -2;
    color = insert(root->DL, toAdd);
	if (color != root->self.color) root->self.color = -2;
	return root->self.color;
}

double findBorder(Circle A, Circle B, double l, double r, bool byX) {
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


//double * findBorders(Circle A, Circle B, Point p1, Point p2) {
//    //ищет касающийся прямоугольник
//    double * res = (double*) malloc(sizeof(double) * 4);
//    /*
//    +----0----+
//    |         |
//    3         1
//    |         |
//    +----2----+
//    */
//    Point centr_point = (p1 + p2) / 2;
//    res[0] = findBorder(A, B, centr_point.y + max(A.r, B.r), centr_point.y, false);
//    res[1] = findBorder(A, B, centr_point.x + max(A.r, B.r), centr_point.x, true);
//    res[2] = findBorder(A, B, centr_point.y - max(A.r, B.r), centr_point.y, false);
//    res[3] = findBorder(A, B, centr_point.x - max(A.r, B.r), centr_point.x, true);
//    return res;
//}

void findBorders(double * res, Circle A, Circle B, Point p1, Point p2) {
	/*
    +----0----+
    |         |
    3         1
    |         |
    +----2----+
    */
	Point R = Point(A.centr.x + A.r, A.centr.y);
	Point L = Point(A.centr.x - A.r, A.centr.y);
	Point D = Point(A.centr.x, A.centr.y - A.r);
	Point U = Point(A.centr.x, A.centr.y + A.r);
	if (!IsPointInCircle(B, U)) {
		U = Point(B.centr.x, B.centr.y + B.r);
		if (!IsPointInCircle(A, U)) U = (p1.y > p2.y) ? p1 : p2;
	}
	if (!IsPointInCircle(B, D)) {
		D = Point(B.centr.x, B.centr.y - B.r);
		if (!IsPointInCircle(A, D)) D = (p1.y < p2.y) ? p1 : p2;
	}
	if (!IsPointInCircle(B, R)) {
		R = Point(B.centr.x + B.r, B.centr.y);
		if (!IsPointInCircle(A, R)) R = (p1.x > p2.x) ? p1 : p2;
	}
	if (!IsPointInCircle(B, L)) {
		L = Point(B.centr.x - B.r, B.centr.y);
		if (!IsPointInCircle(A, L)) L = (p1.x < p2.x) ? p1 : p2;
	}
	res[0] = U.y;
	res[1] = R.x;
	res[2] = D.y;
	res[3] = L.x;
}
double ans = 0;

double findZeroColor(Vertex * root) {
	if (!IsRectCrossCircle(root->self, circles[big_boss])) return 0;
	if (root->self.sqare() < eps_min_rect) return 0;
	if (root->self.color >= 0) return 0; //значит полностью покрашен уже в другой свет
	if (root->self.color == -1 && IsRectInsideCircle(root->self, circles[big_boss])) {
		ans += root->self.sqare();
		return root->self.sqare();
	}
	else if (root->UR) {
		double a1 = findZeroColor(root->UR), 
			a2 = findZeroColor(root->UL), 
			a3 = findZeroColor(root->DR), 
			a4 = findZeroColor(root->DL);
		double a5 = a1 + a2 + a3 + a4;
		if (a5 != 0) {
			ans += a5;
		}
		return a1 + a2 + a3 + a4;
	} else {
		//иначе надо хитрить с его детьми
        double half_width  = (root->self.p2.x - root->self.p1.x) / 2;
        double half_height = (root->self.p2.y - root->self.p1.y) / 2;
		cnt += 4;
        root->UR = new Vertex(Rectangle( Point( root->self.p1.x + half_width, root->self.p1.y + half_height), root->self.p2, root->self.color));
        root->UL = new Vertex(Rectangle( Point( root->self.p1.x, root->self.p1.y + half_height), Point( root->self.p2.x - half_width, root->self.p2.y), root->self.color));
        root->DR = new Vertex(Rectangle( Point( root->self.p1.x + half_width, root->self.p1.y), Point( root->self.p2.x, root->self.p1.y + half_height), root->self.color));
        root->DL = new Vertex(Rectangle( root->self.p1, Point( root->self.p1.x + half_width, root->self.p1.y + half_height), root->self.color));
		double a1 = findZeroColor(root->UR), 
			a2 = findZeroColor(root->UL), 
			a3 = findZeroColor(root->DR), 
			a4 = findZeroColor(root->DL);
		double a5 = a1 + a2 + a3 + a4;
		if (a5 != 0) {
			ans += a5;
		}
		return a1 + a2 + a3 + a4;
	}
}



int main(void) {
    FILE * circles_file = fopen("input", "r");
    FILE * config = fopen("config_simple_fast", "r");
    int is_debug = 0;
    if (config) {
        fscanf(config, "%d", &is_debug);
    }
    if (!circles_file) {
		cerr << "No file \"input\"";
		return 0;
	}
    double x, y, r;
    int n = 0;
    if (is_debug) {
        cout << "Start read" << endl;
    }
    while (fscanf(circles_file, "%lf %lf %lf", &x, &y, &r) == 3) {
        circles[n] = Circle( Point(x, y), r);
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
        cout << "Type id of circle(from 0)" << endl;
    }
    double borders[4];
    Rectangle toAdd;
    Vertex * color_tree;
    double zapac = 0;
    while (1) {
        scanf("%d", &id);
        //id = 0;
		big_boss = id;
        color_tree = new Vertex( Rectangle( Point(circles[id].centr.x - circles[id].r - zapac, circles[id].centr.y - circles[id].r - zapac),
                                            Point(circles[id].centr.x + circles[id].r + zapac, circles[id].centr.y + circles[id].r + zapac), -1)); 
        for (int i = 0; i < n; i++) {
            if (i != id) {
                cross_res = crossCircle(circles[id], circles[i], false);
                if (cross_res.first) {
                    cross_big_boss = i;
                    if (cross_res.second) {
                        findBorders(borders, circles[id], circles[i], cross_res.second->first, cross_res.second->second);
                    } else {
						int ind = (circles[i].r < circles[id].r) ? i : id;
						borders[3] = circles[ind].centr.x - circles[ind].r;
						borders[2] = circles[ind].centr.y - circles[ind].r;
						borders[1] = circles[ind].centr.x + circles[ind].r;
						borders[0] = circles[ind].centr.y + circles[ind].r;
                    }
					toAdd = Rectangle( Point(borders[3], borders[2]), Point(borders[1], borders[0]), i);
					insert(color_tree, toAdd);
                }
            }
        }
		cout << findZeroColor(color_tree) << endl;
		cout << cnt << endl;
    }

    

    return 0;
}
