#ifndef KDTREE
#define KDTREE

#include <vector>
#include <algorithm>
#include <memory>

#include "KDNode.h"

template <size_t D, typename ele>
class KDTree {
    public:
        //This one is for given points
        KDTree(std::vector<std::vector<double> >);

        //This one is for active learning
        KDTree();

        //TODO DESTRUCTOR
        ~KDTree(){}

        void sayhi();

        size_t dim();
        size_t size();
        bool isEmpty();

        void insert(std::vector<double>);

    private:
        std::shared_ptr<KDNode<D, ele>> _head;
        std::shared_ptr<KDNode<D, ele>> buildTree(std::vector< std::vector<double> > points, int dim);
        void traverse(std::shared_ptr<KDNode<D, ele>>);
};

struct CompareByDim {
    CompareByDim(int d) {
        this-> d = d;
    }

    bool operator () (std::vector<double> i, std::vector<double> j) {
        return i[d] < j[d];
    }

    int d;
};

std::vector< std::vector<double> > sortByDim(std::vector<std::vector<double> > points, int d) {
    std::sort(points.begin(), points.end(), CompareByDim(d));
    return points;
}

template <size_t D, typename ele>
KDTree<D, ele>::KDTree() {}

template <size_t D, typename ele>
KDTree<D, ele>::KDTree(std::vector<std::vector<double> > points) {
    std::shared_ptr<KDNode<D, ele>> _head = buildTree(points, 0);
    this->_head = std::move(_head);
}

//This is bad because sorting un-needed things, re-do if time
template <size_t D, typename ele>
std::shared_ptr<KDNode<D, ele>> KDTree<D, ele>::buildTree(std::vector< std::vector<double> > points, int dim) {
    if (points.size() < 1) {
        return NULL;
    }
    points = sortByDim(points, dim);
    int median = points.size() / 2;
    std::shared_ptr<KDNode<D, ele>> node(new KDNode<D, ele>(points[median], 0));

    std::vector< std::vector<double> > _left(points.begin(), points.begin() + median);
    std::vector< std::vector<double> > _right(points.begin() + median + 1, points.end());

    node->_left = buildTree(_left, (dim+1)%D);
    node->_right = buildTree(_right, (dim+1)%D);
    return node;
}

template <size_t D, typename ele>
void KDTree<D, ele>::insert(std::vector<double> vec) {
    //this is a thing
    int dim = 0;
    int left = 0;
    std::shared_ptr<KDNode<D, ele> > cur = _head;
    std::shared_ptr<KDNode<D, ele> > last = NULL;
    
    while(1) {

        if (cur == NULL) {
            break;
        }
        
        last = cur;
        if (vec[dim] < cur->atDim(dim)) {
            cur = cur->_left;
            left = 0;
        } else {
            cur = cur->_right;
            left = 1;
        }
        dim = (dim + 1) % D;
    }
    std::shared_ptr<KDNode<D, ele> > tmp(new KDNode<D, ele>(vec, 0));
    if (left) {
        last->_left = tmp;
    } else {
        last->_right = tmp;
    }
}

template <size_t D, typename ele>
void KDTree<D, ele>::sayhi() {
    this->traverse(this->_head);
}

template <size_t D, typename ele>
void KDTree<D, ele>::traverse(std::shared_ptr<KDNode<D, ele> > cur) {
    if (cur == NULL) {
        return;
    }
    for (int i=0; i<cur->getDim(); i++) {
        std::cout<<cur->atDim(i)<<" ";
    }
    std::cout<<std::endl;
    this->traverse(cur->_left);
    this->traverse(cur->_right);
}

template <size_t D, typename ele>
size_t KDTree<D, ele>::dim() {
    return D;
}

#endif
