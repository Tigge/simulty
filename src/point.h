#ifndef _POINT_H_
#define _POINT_H_

class point {

    public:

    int x;
    int y;

    point();
    point(int nx, int ny);

    void translate(int x, int y);
    void translate(point p);

    static void fix_points(point &a, point &b);

};

#endif
