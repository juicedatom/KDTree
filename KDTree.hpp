#ifndef KDTREE
#define KDTREE

#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <cmath>
#include <map>
#include <exception>
#include <boost/make_shared.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "KDNode.hpp"
#include "memoryHelper.hpp"
#include "Point.hpp"

#ifndef KDTREE_HELPERS
#define KDTREE_HELPERS
enum class SplitMethod {
    MEDIAN
};

class EmptyPointListException: public std::exception {
    virtual const char* what() const throw() {
        return "Empty List of Points, Nothing to do!";
    }
};

class InvalidSplitMethodException: public std::exception {
    virtual const char* what() const throw() {
        return "Invalid Split Method";
    }
};

#endif

template <size_t D, typename V, typename E>
class KDTree {
    public:
        //This one is for given points
        KDTree(std::vector< Point<D, V, E> >,
                SplitMethod sm = SplitMethod::MEDIAN);

        KDTree();

        void sayhi();
        size_t dim();
        size_t size();
        bool isEmpty();
        void insert(Point<D, V, E> point);
        void write(std::string fname);
        void read(std::string fname);

        std::unique_ptr<std::multimap<V, Point<D, V, E>>> search(
                Point<D, V, E> p,
                const unsigned int k,
                const bool bbf = false,
                const unsigned int maxlevel = 0);

        bool contains(Point<D, V, E> p);
    private:
        boost::shared_ptr<KDNode<D,V,E>> _head;
        size_t _size;

        boost::shared_ptr<KDNode<D,V,E>> buildTree(
                std::vector< Point<D,V,E>> points,
                const unsigned int depth,
                SplitMethod sm,
                V lastV);

        std::unique_ptr<std::multimap<V, Point<D, V, E>>> knnTraverse(
                boost::shared_ptr<KDNode<D, V, E>> cur,
                Point<D, V, E> p, std::unique_ptr<std::multimap<V, Point<D, V, E>>> pq,
                const unsigned int k,
                unsigned int level,
                const bool bbf = false,
                const unsigned int maxlevel = 0);

        void traverse(boost::shared_ptr<KDNode<D, V, E>>, std::string gg);
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(
                Archive & ar,
                const unsigned int version
                ) const {
            ar & _head & _size;
        }
};

template <size_t D, typename V, typename E>
KDTree<D, V, E>::KDTree() {
    this->_size = 0;
}

template <size_t D, typename V, typename E>
struct CompareByDim {
    CompareByDim(int d) {
        this-> d = d;
    }

    bool operator () (Point<D, V, E> i, Point<D, V, E> j) {
        return i[d] < j[d];
    }

    int d;
};

template <size_t D, typename V, typename E>
std::vector< Point<D, V, E> > sortByDim(
        std::vector<Point<D, V, E> > points,
        int d) {

    std::sort(points.begin(), points.end(), CompareByDim<D, V, E>(d));
    return points;
}


template <size_t D, typename V, typename E>
KDTree<D, V, E>::KDTree(std::vector<Point<D, V, E>> points,
        SplitMethod sm) {
    if (points.size() == 0) {
        EmptyPointListException a;
        throw a;
    }

    boost::shared_ptr<KDNode<D, V, E>> _head = buildTree(points, 0, sm, 0);
    this->_size = points.size();
    this->_head = _head;
}

template <size_t D, typename V, typename E>
int getSplit(std::vector<Point<D, V, E>> points, SplitMethod sm, int axis) {
    int split = -1;
    switch (sm) {
        case SplitMethod::MEDIAN:
            split = points.size() / 2;
            break;
        default:
            InvalidSplitMethodException ex;
            throw ex;
            break;
    }

    return split;
}
template <size_t D, typename V, typename E>
boost::shared_ptr<KDNode<D, V, E>> KDTree<D, V, E>::buildTree(
        std::vector<Point<D, V, E>> points,
        unsigned int depth,
        SplitMethod sm,
        V lastV) {

    if (points.size() < 1) {
        return NULL;
    }

    int axis = depth % D;
    points = sortByDim(points, axis);
    int split = getSplit(points, sm, axis);

    boost::shared_ptr<KDNode<D, V, E>> node = boost::make_shared<KDNode<D, V, E>>(points[split]);

    std::vector<Point<D, V, E>> _left(points.begin(), points.begin() + split);
    std::vector<Point<D, V, E>> _right(points.begin() + split + 1, points.end());

    depth++;
    node->_left = buildTree(_left, depth, sm, points[split][axis]);
    node->_right = buildTree(_right, depth, sm, points[split][axis]);
    return node;
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::insert(Point<D, V, E> p) {
    int dim = 0;
    int left = 0;
    boost::shared_ptr<KDNode<D, V, E> > cur = this->_head;
    boost::shared_ptr<KDNode<D, V, E> > last = NULL;
    while(1) {

        if (cur == NULL) {
            break;
        }
        
        last = cur;
        if (p[dim] < cur->atDim(dim)) {
            cur = cur->getLeft();
            left = 1;
        } else {
            cur = cur->getRight();
            left = 0;
        }
        dim = (dim + 1) % D;
    }
    boost::shared_ptr<KDNode<D, V, E>> tmp = boost::make_shared<KDNode<D, V, E>>(p);
    if (left) {
        last->setLeft(tmp);
    } else {
        last->setRight(tmp);
    }
    this->_size++;
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::sayhi() {
    this->traverse(this->_head, "");
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::traverse(
        boost::shared_ptr<KDNode<D, V, E> > cur,
        std::string loc) {

    if (cur == NULL) {
        return;
    }
    cur->printNode();
    std::cout<<"dir: "<<loc<<std::endl;
    this->traverse(cur->getLeft(), loc + "l ");
    this->traverse(cur->getRight(), loc + "r ");
}

template <size_t D, typename V, typename E>
size_t KDTree<D, V, E>::dim() {
    return D;
}

template <size_t D, typename V, typename E>
size_t KDTree<D, V, E>::size() {
    return this->_size;
}

template <size_t D, typename V, typename E>
bool KDTree<D, V, E>::isEmpty() {
    return this->size() < 1;
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::write(std::string fname) {
    std::ofstream ofs(fname);
    boost::archive::binary_oarchive oa(ofs);
    oa & this->_head & this->_size;
}

template <size_t D, typename V, typename E>
std::unique_ptr<std::multimap<V, Point<D, V, E>>> KDTree<D, V, E>::knnTraverse(
        boost::shared_ptr<KDNode<D, V, E>> cur,
        Point<D, V, E> p,
        std::unique_ptr<std::multimap<V, Point<D, V, E>>> pq,
        const unsigned int k,
        unsigned int level,
        const bool bbf,
        const unsigned int maxlevel) {

    bool levelmaxReached = (maxlevel !=0) && (level > maxlevel);

    if (levelmaxReached || cur == NULL) {
        return pq;
    }

    V distance = cur->getPoint().eucl(p);
    pq->insert(std::pair<V, Point<D, V, E>>(distance, cur->getPoint()));

    if (pq->size() > k) {
        pq->erase(std::prev(pq->end()));
    }

    V axis = level % D;
    bool left = true;
    if (p[axis] < cur->atDim(axis)) {
        pq = knnTraverse(cur->getLeft(), p, std::move(pq), k, level++, bbf, maxlevel);
    } else {
        pq = knnTraverse(cur->getRight(), p, std::move(pq), k, level++, bbf, maxlevel);
        left = false;
    }

    if (!bbf) { 
        V best = pq->begin()->first;
        if (pq->size() < k || std::abs(cur->getPoint()[axis] - p[axis]) <= best) {
            if (left) {
                pq = knnTraverse(cur->getRight(), p,  std::move(pq), k, level++, bbf, maxlevel);
            } else {
                pq = knnTraverse(cur->getLeft(), p, std::move(pq), k, level++, bbf, maxlevel);
            }
        }
    }

   return pq;
}

template <size_t D, typename V, typename E>
using search_t = std::multimap<V, Point<D, V, E>>;

template <size_t D, typename V, typename E>
using search_ptr = std::unique_ptr<search_t<D, V, E>>;

template <size_t D, typename V, typename E>
using search_iter = typename search_t<D, V, E>::iterator;

template <size_t D, typename V, typename E>
std::unique_ptr<std::multimap<V, Point<D, V, E>>> KDTree<D, V, E>::search(
        Point<D, V, E> p,
        const unsigned int k,
        const bool bbf,
        const unsigned int maxlevel) {

    boost::shared_ptr<KDNode<D, V, E>> cur = this->_head;
    std::unique_ptr<std::multimap<V, Point<D, V, E>>> pq = make_unique<std::multimap<V, Point<D, V, E>>>();
    pq = this->knnTraverse(cur, p, std::move(pq), k, 0, bbf, maxlevel);
    return pq;
}

template <size_t D, typename V, typename E>
bool KDTree<D, V, E>::contains(Point<D, V, E> pt) {
    //check if I contain a point
    search_ptr<D, V, E> ret = this->search(pt, 1);

    return pt == ret->begin()->second;
}

template <size_t D, typename V, typename E>
void KDTree<D, V, E>::read(std::string fname) {
    std::ifstream ifs(fname);
    boost::archive::binary_iarchive ia(ifs);
    ia & this->_head & this->_size;
}
#endif
