#ifndef POINT_H
#define POINT_H

#include <vector>
#include <string>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <cmath>

template <size_t D, typename V, typename E>
class Point {
    private:
        E _element;
        std::vector<V> _vec;
    public:
        Point(){};
        Point(std::vector<V> points, E element);
        Point(V arr[], E element);

        V& operator[] (int x) {
            return _vec[x];
        }
        bool operator==(const Point<D, V, E>& other) const;
        bool operator!=(const Point<D, V, E>& other) const;

        E getEle() const { return this->_element; };
        size_t getDim() const { return D; }
        V at(int x){ return _vec[x]; }
        V dist(Point<D, V, E> p);
        std::vector<V> getVec() const { return this->_vec; };
        void sayhi();

         //Everything serialization related
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)  {
            ar & this->_vec & this->_element;
        }
};

template <size_t D, typename V, typename E>
Point<D, V, E>::Point(std::vector<V> vec, E element) {
    this->_element = element;
    this->_vec = vec;
}

template <size_t D, typename V, typename E>
Point<D, V, E>::Point(V arr[], E element) {
    this->_element = element;
    std::vector<V> vec(arr, arr + sizeof(arr) / sizeof(arr[0]) );
    this->_vec = vec;
}

template <size_t D, typename V, typename E>
V Point<D, V, E>::dist(Point<D, V, E> p) {
    V sum = 0;
    for (size_t i=0; i<D; i++) {
        V a = p[i] - _vec[i];
        sum += a * a;
    }
    return sum*sum;
}

template <size_t D, typename V, typename E>
bool Point<D, V, E>::operator==(const Point<D, V, E>& other) const {
    return this->getVec() == other.getVec() && 
        this->getEle() == other.getEle();
}

template <size_t D, typename V, typename E>
bool Point<D, V, E>::operator!=(const Point<D, V, E>& other) const {
    return ! ((*this) == other);
}

template <size_t D, typename V, typename E>
void Point<D, V, E>::sayhi() {
    for(size_t i=0; i<D; i++) {
        std::cout<<this->_vec[i]<<" ";
    }
}
#endif
