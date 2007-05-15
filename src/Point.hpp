#ifndef _POINT_HPP_
#define _POINT_HPP_


/** 
 * Simple class representation of a point. 
 * 
 * Etc.. more info
 *
 */
class Point {

    private:
    
    int x;
    int y;
    
    public:

    Point();
    Point(int x, int y);

    /** 
     * Getter for the x value.
     *
     * @return the x value
     */
    int getX();
    
    /** 
     * Getter for the y value.
     *
     * @return the y value
     */    
    int getY();
    
    void setX(int x);
    void setY(int y);

    void translate(int x, int y);
    void translate(Point p);

    static void fix_points(Point &a, Point &b);

};

#endif

