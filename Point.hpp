#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <string>
#include <vector>

#include <boost/serialization/list.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

/** 
 * This class holds information regarding a Point where a vector of
 * type V and length N represent a point in space.  The Point holds
 * an element of type E
*/
template <size_t N, typename V, typename E>
class Point {
    private:
        E _element;
        std::vector<V> _vec;
    public:

        // Constructors for default, using a vector, and using
        // an array.  The vector constructor makes it easy to write
        // tests
        Point(){};
        Point(std::vector<V> points, E element);

        // allow for indexing the vector with []
        V& operator[] (int x) {
            return _vec[x];
        }

        // equality overloading
        bool operator==(const Point<N, V, E>& other) const;
        bool operator!=(const Point<N, V, E>& other) const;

        E getEle() const { return this->_element; };
        size_t getDim() const { return N; }

        // useful when you need to access a Point based
        // on a memory location.  ex) a->at(4)
        V at(int x){ return _vec[x]; }
        V dist(Point<N, V, E> p);
        std::vector<V> getVec() const { return this->_vec; };

        // debugging statement to print the Point vec
        void sayhi();

         //Everything serialization related
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)  {
            ar & this->_vec & this->_element;
        }
};

template <size_t N, typename V, typename E>
Point<N, V, E>::Point(std::vector<V> vec, E element) {
    this->_element = element;
    this->_vec = vec;
}

template <size_t N, typename V, typename E>
V Point<N, V, E>::dist(Point<N, V, E> p) {
    V sum = 0;
    for (size_t i=0; i<N; i++) {
        V a = p[i] - _vec[i];
        sum += a * a;
    }
    return sum;
}

template <size_t N, typename V, typename E>
bool Point<N, V, E>::operator==(const Point<N, V, E>& other) const {
    return this->getVec() == other.getVec() && 
        this->getEle() == other.getEle();
}

template <size_t N, typename V, typename E>
bool Point<N, V, E>::operator!=(const Point<N, V, E>& other) const {
    return ! ((*this) == other);
}

template <size_t N, typename V, typename E>
void Point<N, V, E>::sayhi() {
    for(size_t i=0; i<N; i++) {
        std::cout<<this->_vec[i]<<" ";
    }
}


template <size_t N, typename V, typename E>
struct ComparePtByDim {
    ComparePtByDim(int d) {
        this-> d = d;
    }

    bool operator () (Point<N, V, E> i, Point<N, V, E> j) {
        return i[d] < j[d];
    }

    int d;
};
#endif
