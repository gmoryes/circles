struct Point {
    double x, y;
    Point() {};
    Point (double x, double y) {
        this->x = x;
        this->y = y;
    }
    Point operator+ (Point b) {
        return Point(this->x + b.x, this->y + b.y);
    }
    Point operator- (Point b) {
        return Point(this->x - b.x, this->y - b.y);
    }  
    Point operator* (double t) {
        return Point(this->x * t, this->y * t);
    }
    Point operator/ (double t) {
        return Point(this->x / t, this->y / t);
    }
};

struct Sircle {
    Point centr;
    double r;
    double preArea;
    int start_x, start_y, end_x, end_y;
    Sircle() {};
    Sircle (Point a, double r) {
        this->centr = a;
        this->r = r;
        this->preArea = 0;
    }
};

double dist2(Point a, Point b) {
    return ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double dist(Point a, Point b) {
    return sqrt(dist2(a, b));
}

double mult_vector(Point a, Point b) {
    return (a.x * b.y - a.y * b.x);
}

double scalar_vector(Point a, Point b) {
	return (a.x * b.x + a.y * b.y);
}

pair<bool, pair <Point, Point> *> crossSircle(Sircle a, Sircle b, bool only_check) {
	double q = dist2(a.centr, b.centr);
	double q1 = a.r * a.r + b.r * b.r + 2.0 * a.r * b.r;
	double q3 = (a.r - b.r) * (a.r - b.r);
    if (dist2(a.centr, b.centr) >= a.r * a.r + b.r * b.r + 2.0 * a.r * b.r) {
        //if sircles (*)...(*)
        return make_pair(false, (pair<Point, Point> *) NULL);
    } else if (dist2(a.centr, b.centr) <= (a.r - b.r) * (a.r - b.r)) {
		//dist + r_min <= r_max, dist <= r_max - r_min, dist2 <= r_max*r_max - 2*r_max*r_min + r_min*r_min = (r_min - r_max)^2
        return make_pair(true, (pair<Point, Point> *) NULL);
    }
    if (only_check) return make_pair(false, (pair<Point, Point> *) NULL);
    double t = (a.r * a.r - b.r * b.r + dist2(a.centr, b.centr)) / (2 * dist(a.centr, b.centr));
    double h = sqrt(a.r * a.r - t * t);
    Point bet_centr = b.centr - a.centr;
    double len_centr = dist(Point(0, 0), bet_centr);
    bet_centr = bet_centr * (t / len_centr);
    Point middle = a.centr + bet_centr;
	if (fabs(t) < eps) bet_centr = b.centr - a.centr; //if (t == 0)
    Point height = Point(-bet_centr.y, bet_centr.x);
    double len_height = dist(Point(0, 0), height);
    height = height * (h / len_height);
    pair<Point, Point> * res = (pair<Point, Point> *) malloc(sizeof(pair<Point, Point>));
    res->first = middle + height;
    res->second = middle - height;
    return make_pair(true, res);
}