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
        V& operator[] (int x) {
            return _vec[x];
        }
        E getEle() { return this->_element; };
        V eucl(Point<D, V, E> p);
        std::vector<V> getVec() { return this->_vec; };
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
V Point<D, V, E>::eucl(Point<D, V, E> p) {
    V sum = 0;
    for (int i=0; i<D; i++) {
        V a = p[i] - _vec[i];
        sum += a * a;
    }
    return std::sqrt(sum);
}

template <size_t D, typename V, typename E>
void Point<D, V, E>::sayhi() {
    for(int i=0; i<D; i++) {
        std::cout<<this->_vec[i]<<" ";
    }
}
#endif
