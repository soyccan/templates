#include <cassert>
#include <iostream>
#include "geometry.h"
#define FOR(i,st,en) for (int i=st; i<=en; ++i)
#define RFOR(i,st,en) for (int i=st; i>=en; --i)
using namespace std;
using namespace geometry;
main() {
	Vector3d<int> a(4,8,7), b(9,1,7), c(8,6,0);
	cout<<det(a,b,c);
	// Vector v1(5,7), v2(2,5);
	// assert(v1 + v2 == Vector(7,12));
	// assert(v1 - v2 == Vector(3,2));
	// assert(v2 - v1 == -Vector(3,2));
	// assert(dot(v1, v2)==45);
	// assert(cross(v1, v2)==11);

	// Point p1(5,7), p2(2,5);
	// // p1.x = 6;
	// assert(p2 - p1 == Vector(-3,-2));

	// Line l1(p1, v2), l2(Point(3,2), Point(5, -1));
	// assert(count_inter(l1,l2)==1 && get_inter(l1,l2)==Point(3,2));

	// // cout<<rotate(Vector(1,0), acos(-1.0l)/2);
	// assert(rotate(Vector(1,0), acos(-1.0l)/2) == Vector(0,1));
	// assert(rotate(Point(1,0), Point(0,0), acos(-1.0l)/2) == Point(0,1));

	// Polygon pg(3);
	// pg[0] = Point(1,3);
	// pg[1] = Point(5,4);
	// pg[2] = Point(7,10);
	// rotate(pg, Point(0,0), 3.14l);
	// assert(round(pg.area()) == 11);

	system("pause");

    return 0;
}
