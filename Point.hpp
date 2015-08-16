#ifndef POINT_H
#define POINT_H

#include <vector>
#include <string>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>

template <size_t D, typename ele>
class Point {
    private:
        ele _element;
        std::vector<double> _vec;
    public:
        Point(){};
        Point(std::vector<double> points, ele element);
        double& operator[] (int x) {
            return _vec[x];
        }
        ele getEle() { return this->_element; };
        std::vector<double> getVec() { return this->_vec; };

         //Everything serialization related
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)  {
            ar & this->_vec & this->_element;
        }
};

template <size_t D, typename ele>
Point<D, ele>::Point(std::vector<double> vec, ele element) {
    this->_element = element;
    this->_vec = vec;
}

#endif
