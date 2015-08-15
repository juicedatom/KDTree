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

        int sayhi();

        size_t dim();
        size_t size();
        bool isEmpty();

    private:
        std::shared_ptr<KDNode<D, ele>> buildTree(std::vector< std::vector<double> > points, int dim);
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
    std::shared_ptr<KDNode<D, ele>> hey = buildTree(points, 0);
}

//This is bad because sorting un-needed things, re-do if time
template <size_t D, typename ele>
std::shared_ptr<KDNode<D, ele>> KDTree<D, ele>::buildTree(std::vector< std::vector<double> > points, int dim) {
    if (points.size() < 1) {
        return NULL;
    }
    points = sortByDim(points, dim);
    int median = points.size() / 2;
    std::shared_ptr<KDNode<D, ele>> tmp(new KDNode<D, ele>(points[median], 0));

    std::cout<<"this is adding"<<std::endl;
    for (int i=0; i<points[median].size(); i++) {
        std::cout<<points[median][i]<<" ";
    }
    std::cout<<std::endl;

    std::vector< std::vector<double> > _left(points.begin(), points.begin() + median);
    std::vector< std::vector<double> > _right(points.begin() + median + 1, points.end());

    tmp->_left = buildTree(_left, (dim+1)%D);
    std::cout<<"here"<<std::endl;
    tmp->_right = buildTree(_right, (dim+1)%D);
    return tmp;
}

template <size_t D, typename ele>
int KDTree<D, ele>::sayhi() {
    std::cout<<"hi";
    return 0;
}

template <size_t D, typename ele>
size_t KDTree<D, ele>::dim() {
    return D;
}
#endif
