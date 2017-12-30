#ifndef KDNODE_H
#define KDNODE_H

#include <boost/serialization/list.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

#include "Point.hpp"
#include "memoryHelper.hpp"

/*
 * This is a node to hold each of the Points<N, V, E> inside of the
 * KDTree class.
 */
template <size_t N, typename V, typename E>
class KDNode {
    public:
        KDNode(Point<N, V, E> p);
        KDNode<N, V, E>(){};

        boost::shared_ptr<KDNode<N, V, E>> getLeft();
        boost::shared_ptr<KDNode<N, V, E>> getRight();

        void setLeft(boost::shared_ptr<KDNode<N, V, E>> node);
        void setRight(boost::shared_ptr<KDNode<N, V, E>> node);

        Point<N, V, E> getPoint() { return this->_point; };

        // returns Point[dim]
        V atDim(int dim);
        size_t getDim(){ return N; }

        // debugging method to print some node information
        void printNode();

        // operator overloading of [] for easy access of the
        // vector stored in _point
        V& operator[] (int x);

        //Everything serialization related
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive & ar, const unsigned int version)  {
            ar & _point & _left & _right;
        }

    private:
        Point<N, V, E> _point;
        boost::shared_ptr<KDNode<N, V, E>> _left;
        boost::shared_ptr<KDNode<N, V, E>> _right;
};

template <size_t N, typename V, typename E>
boost::shared_ptr<KDNode<N, V, E>> KDNode<N, V, E>::getLeft() {
    return this->_left;
}

template <size_t N, typename V, typename E>
boost::shared_ptr<KDNode<N, V, E>> KDNode<N, V, E>::getRight() {
    return this->_right;
}

template <size_t N, typename V, typename E>
void KDNode<N, V, E>::setLeft(boost::shared_ptr<KDNode<N, V, E>> node) {
    this->_left = node;
}

template <size_t N, typename V, typename E>
void KDNode<N, V, E>::setRight(boost::shared_ptr<KDNode<N, V, E>> node) {
    this->_right = node;
}

template <size_t N, typename V, typename E>
KDNode<N, V, E>::KDNode(Point<N, V, E> p) {
    this->_point = p;
}


template <size_t N, typename V, typename E>
V KDNode<N, V, E>::atDim(int dim) {
    return this->_point[dim];
}

template <size_t N, typename V, typename E>
void KDNode<N, V, E>::printNode() {
    for( unsigned int i=0; i<N; i++ ) {
        std::cout<<this->atDim(i)<<" ";
    }
    std::cout<<std::endl;
}

template <size_t N, typename V, typename E>
V& KDNode<N, V, E>::operator[] (int x) {
    return this->_point[x];
}
#endif
