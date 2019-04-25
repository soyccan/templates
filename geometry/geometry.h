#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <cmath>
#include <limits>
#include <istream>
#include <ostream>
#include <vector>
#include <exception>

namespace geometry {

  enum LineRelation {INTERSECT, COINCIDENT, PARALLEL, SKEW};

  const long double EPS = 1e-10;
  const long double INF = std::numeric_limits<long double>::infinity();

  template<typename T>
  inline int sgn(T x) {
    if (x > EPS)
      return 1;
    else if (x < -EPS)
      return -1;
    else
      return 0;
  }


  template<typename T = long double>
  class _VectorBase {
  public:
    T x, y, z;
    _VectorBase(T x=0, T y=0, T z=0): x(x), y(y), z(z) {}

    _VectorBase& operator+() { return *this; }
    _VectorBase operator-() { return _VectorBase(-x, -y, -z); }

    _VectorBase& operator=(const _VectorBase& v) {
      x = v.x; y = v.y; z = v.z;
      return *this;
    }

    _VectorBase& operator+=(const _VectorBase& v) {
      x += v.x; y += v.y; z += v.z;
      return *this;
    }

    _VectorBase& operator-=(const _VectorBase& v) {
      x -= v.x; y -= v.y; z -= v.z;
      return *this;
    }

    _VectorBase& operator*=(T t) {
      x *= t; y *= t; z *= t;
      return *this;
    }

    _VectorBase& operator/=(T t) {
      x /= t; y /= t; z /= t;
      return *this;
    }

    friend _VectorBase
    operator+(_VectorBase u, const _VectorBase& v) { u += v; return u; }

    friend _VectorBase
    operator-(_VectorBase u, const _VectorBase& v) { u -= v; return u; }

    friend _VectorBase
    operator*(_VectorBase v, T t) { v *= t; return v; }

    friend _VectorBase
    operator*(T t, _VectorBase v) { v *= t; return v; }

    friend _VectorBase
    operator/(_VectorBase v, T t) { v /= t; return v; }

    friend std::ostream&
    operator<<(std::ostream& os, const _VectorBase& v) {
      os << '(' << v.x << ',' << v.y << ',' << v.z << ')';
      return os;
    }

    friend inline bool
    operator==(const _VectorBase& u, const _VectorBase& v) {
      return sgn(u.x - v.x) == 0
          && sgn(u.y - v.y) == 0
          && sgn(u.z - v.z) == 0;
    }

    friend inline bool
    operator!=(const _VectorBase& u, const _VectorBase& v) {
      return !(u == v);
    }
  };

  template<typename T = long double>
  class Vector3d : public _VectorBase<T> {
  public:
    Vector3d(T x=0, T y=0, T z=0): _VectorBase<T>(x, y, z) {}
  };

  template<typename T = long double>
  class Point3d : public _VectorBase<T> {
  public:
    Point3d(T x=0, T y=0, T z=0): _VectorBase<T>(x, y, z) {}
  };

  template<typename T = long double>
  class Line3d {
  public:
    Point3d<T> point;

    // direction vector
    // TODO: unit vector ?
    Vector3d<T> dir;

    Line3d() {}
    Line3d(const Point3d<T>& point,
           const Vector3d<T>& dir): point(point), dir(dir) {}
  };

  template<typename T = long double>
  class Segment3d {
  public:
    Point3d<T> left, right;
    Segment3d() {}
    Segment3d(const Point3d<T>& left,
              const Point3d<T>& right): left(left), right(right) {}
  };

  template<typename T = long double>
  class Polygon3d {
  public:
    Polygon3d(size_t count = 0) {
      __vertices.resize(count);
    }
    Point3d<T>& operator[](size_t i) {
      return __vertices[i];
    }
    size_t count() const {
      return __vertices.count();
    }

  private:
    std::vector<Point3d<T>> __vertices;
  };


  // _VectorBase
    template<typename T>
    T abs(const _VectorBase<T>& v) {
      return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    template<typename T>
    T dot(const _VectorBase<T>& u, const _VectorBase<T>& v) {
      return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    template<typename T>
    _VectorBase<T> cross(const _VectorBase<T>& u, const _VectorBase<T>& v) {
      return _VectorBase<T>(u.y * v.z - v.y * u.z,
                            u.z * v.x - v.z * u.x,
                            u.x * v.y - v.x * u.y);
    }

    template<typename T>
    T det(const _VectorBase<T>& u,
          const _VectorBase<T>& v,
          const _VectorBase<T>& w) {
      return dot(u, cross(v, w));
    }

    // template<typename T>
    // Vector2d<T> rotate(_VectorBase<T> v, T ang) {
    //   T _x = v.x;
    //   T _y = v.y;
    //   v.x = _x * cos(ang) - _y * sin(ang);
    //   v.y = _x * sin(ang) + _y * cos(ang);
    //   return v;
    // }

    template<typename T>
    _VectorBase<T> project(const _VectorBase<T>& u, const _VectorBase<T>& v) {
      // u onto v
      return v * (dot(u, v) / (v.x*v.x + v.y*v.y + v.z*v.z));
    }
  // end _VectorBase

  // Line3d
    template<typename T>
    LineRelation relation(const Line3d<T>& l, const Line3d<T>& m) {
      if (sgn(abs(cross(l.dir, m.dir))) == 0) {
        if (sgn(abs(cross(l.dir, m.point - l.point))) == 0)
          return COINCIDENT;
        else
          return PARALLEL;
      }
      else {
        if (sgn(det(l.dir, m.dir, m.point - l.point)) == 0)
          return INTERSECT;
        else
          return SKEW;
      }
    }

    template<typename T>
    Point3d<T> get_inter(const Line3d<T>& l, const Line3d<T>& m) {
      if (relation(l, m) != LineRelation::INTERSECT)
        throw std::exception("Lines may not intersect at one point");
      return l.point + l.dir * (cross(m.point - l.point, m.dir) / cross(l.dir, m.dir));
    }
  // end Line3d

  // Segment3d
  // [TODO]
    // template<typename T>
    // bool _point_on_collinear_seg(const Point3d<T>& p, const Segment3d<T>& s) {
    //   return sgn(p.x - s.base.x) * sgn(p.x - s.end.x) <= 0
    //       && sgn(p.y - s.base.y) * sgn(p.y - s.end.y) <= 0;
    // }

    // template<typename T>
    // bool point_on_seg(const Point3d<T>& p, const Segment3d<T>& s) {
    //   return sgn(cross(s.base-p, s.end-p)) == 0
    //       && sgn(dot(s.base-p, s.end-p)) <= 0;
    // }

    // template<typename T>
    // bool _collinear_seg_inter(const Segment3d<T>& s, const Segment3d<T>& t) {
    //   return point_on_seg(t.base, s)
    //       || point_on_seg(t.end, s)
    //       || point_on_seg(s.base, t)
    //       || point_on_seg(s.end, t);
    // }

    // template<typename T>
    // size_t count_inter(const Segment3d<T>& s, const Segment3d<T>& t) {
    //   T c1 = cross(s.end-s.base, t.base-s.base);
    //   T c2 = cross(s.end-s.base, t.end-s.base);
    //   T c3 = cross(t.end-t.base, s.base-t.base);
    //   T c4 = cross(t.end-t.base, s.end-t.base);
    //   if ((sgn(c1) == 0 && sgn(c2) == 0 && _collinear_seg_inter(s, t))
    //    || (sgn(c3) == 0 && sgn(c4) == 0 && _collinear_seg_inter(s, t)))
    //     return COUNTLESS;
    //   else if ((sgn(c1) * sgn(c2) < 0 && sgn(c3) * sgn(c4) < 0) // cross
    //         || (sgn(c1) == 0 && point_on_seg(t.base, s))
    //         || (sgn(c2) == 0 && point_on_seg(t.end, s))
    //         || (sgn(c3) == 0 && point_on_seg(s.base, t))
    //         || (sgn(c4) == 0 && point_on_seg(s.end, t)))
    //     return 1;
    //   else
    //     return 0;
    // }

    // template<typename T>
    // Point3d<T> get_inter(const Segment3d<T>& s, const Segment3d<T>& t) {
    //   Line3d<T> l(s.base, s.end-s.base);
    //   Line3d<T> m(t.base, t.end-t.base);
    //   if (count_inter(s, t) == 1)
    //     return get_inter(l, m);
    //   else
    //     return NO_POINT;
    // }
  // end Segment3d

  // Polygon3d
    template<typename T>
    T area(const Polygon3d<T> polygon) {
      T res = 0;
      for (size_t i = 0; i < polygon.count(); ++i)
        res += cross(polygon[i], polygon[(i+1) % polygon.count()]);
      return res / 2;
    }

    template<typename T>
    bool inside(const Point3d<T>& p, const Polygon3d<T>& pg) {
      size_t n = pg.size();
      int wn = 0;
      for (int i=0; i<n; i++) {
        const Point3d<T>& s1 = pg[i];
        const Point3d<T>& s2 = pg[(i+1)%n];
        if (point_on_seg(p, Segment3d<T>(s1, s2)))
          return -1; // on side
        int k = cross(s2-s1 , p-s1);
        int d1 = s1.y - p.y;
        int d2 = s2.y - p.y;
        if (k > 0 && d1 <= 0 && d2 > 0)
          wn++; // 逆時針穿過，繞數+1
        if (k < 0 && d1 > 0 && d2 <= 0)
          wn--; // 順時針穿過，繞數-1
      }
      return wn != 0;
    }

    // template<typename T>
    // Polygon3d<T> scale(Polygon3d<T> pg, const Point3d<T>& origin, T t) {}
  // end Polygon3d
};

#endif // GEOMETRY_H
