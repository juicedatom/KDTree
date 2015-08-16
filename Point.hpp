#ifndef POINT_H
#define POINT_H

#include <vector>
#include <string>

template <size_t D, typename ele>
class Point {
    private:
        ele _element;
        std::vector<double> _vec;
    public:
        Point(std::vector<double> points, ele element);
        double& operator[] (int x) {
            return _vec[x];
        }
};

template <size_t D, typename ele>
Point<D, ele>::Point(std::vector<double> vec, ele element) {
    this->_element = element;
    this->_vec = vec;
}

#endif
