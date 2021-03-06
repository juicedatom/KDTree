#ifndef KDTREE
#define KDTREE

#include <algorithm>
#include <cmath>
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/make_shared.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

#include "KDNode.hpp"
#include "Point.hpp"
#include "memoryHelper.hpp"

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

/**
 * KDTree class which can be used in many machine-learing applications
 * for faster N-dimensional point lookup.  Each Point stored in the KDTree
 * has a type V with a length of N and a cooresponding element E.
 */
template <size_t N, typename V, typename E>
class KDTree {
    public:

        // constructor for creating a new KDTree
        // Takes in an std::vector of Points and stores them
        // in the tree with a MEDIAN setting as the defalt
        KDTree(std::vector< Point<N, V, E> >,
                SplitMethod sm = SplitMethod::MEDIAN);

        // default constructor.  useful for creating trees which
        // need to load data.
        KDTree();

        // debugging method with prints the tree
        void sayhi();

        // returns the Point dimension D
        size_t dim();

        // returns the number of Points stored in the Tree
        size_t size();

        // checks if the tree is empty or not
        bool isEmpty();

        // Insert a new point into the tree
        void insert(Point<N, V, E> point);

        // read and write a tree to a file.
        //
        // //example
        //
        // //initialize a new tree with the default
        // // constructor
        //
        // KDTree<8, double, char> copiedTree;
        //
        // oldTree.write("sometree.tree");
        // copiedTree.read("sometree.tree");
        void write(std::string fname);
        void read(std::string fname);

        // This will search the tree for Point p and return an iterator
        // with the k nearest neighbors.  If bbf (best bin first)
        // is true, invoke a best bin first method which will skip
        // checking the other half of the tree and look at a maximum of
        // M nodes when traversing.
        //
        // // example, this returns the 3 nearest neighbors to
        // // Point pt in tree
        // 
        // search_ptr<8, double, std::string> result;
        // search_iter<8, double, std::string> pos;
        // 
        // ret = tree.search(pt, 3);
        // 
        // for (pos = ret->begin(); pos != ret->end(); os++) {
        //     std::cout<<"Key: "<<std::cout<<pos->first<<std::endl;
        //
        //     std::cout<<"Printing point"<<std::endl;
        //     pos->second.sayhi();
        //     std::cout<<std::endl;
        // }
        //
        std::unique_ptr<std::multimap<V, Point<N, V, E>>> search(
                Point<N, V, E> p,
                const unsigned int k = 1,
                const bool bbf = false,
                const unsigned int maxlevel = 0);

        // returns true if Point p is in the tree
        bool contains(Point<N, V, E> p);
    private:

        // head of the tree.  In hindsight I don't know why I didn't make
        // this a **, whatever... I digress.
        boost::shared_ptr<KDNode<N,V,E>> _head;

        // size of the tree
        size_t _size;

        // recursive method to help with the KDTree constructor
        boost::shared_ptr<KDNode<N,V,E>> buildTree(
                std::vector< Point<N,V,E>> points,
                const unsigned int depth,
                SplitMethod sm);

        // recursive method to help with the search method
        std::unique_ptr<std::multimap<V, Point<N, V, E>>> knnTraverse(
                boost::shared_ptr<KDNode<N, V, E>> cur,
                Point<N, V, E> p, std::unique_ptr<std::multimap<V, Point<N, V, E>>> pq,
                const unsigned int k,
                unsigned int level,
                const bool bbf = false,
                const unsigned int maxlevel = 0);

        // pre-order recursive traversal to help with the sayhi() method
        void traverse(boost::shared_ptr<KDNode<N, V, E>>, std::string gg);
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(
                Archive & ar,
                const unsigned int version
                ) const {
            ar & _head & _size;
        }
};

template <size_t N, typename V, typename E>
KDTree<N, V, E>::KDTree() {
    this->_size = 0;
    this->_head = NULL;
}

// Helper function to sort a vector of points by a specific
// dimension
template <size_t N, typename V, typename E>
std::vector< Point<N, V, E> > sortByDim(
        std::vector<Point<N, V, E> > points,
        int d) {

    std::sort(points.begin(), points.end(), ComparePtByDim<N, V, E>(d));
    return points;
}

template <size_t N, typename V, typename E>
KDTree<N, V, E>::KDTree(std::vector<Point<N, V, E>> points,
        SplitMethod sm) {
    if (points.size() == 0) {
        EmptyPointListException a;
        throw a;
    }

    boost::shared_ptr<KDNode<N, V, E>> _head = buildTree(points, 0, sm);
    this->_size = points.size();
    this->_head = _head;
}

// helper method to decide how the tree should be broken up.  currently only
// the median is implemented but this could easily be extended to other methods
// for example, mean.
template <size_t N, typename V, typename E>
int getSplit(std::vector<Point<N, V, E>> points, SplitMethod sm, int axis) {
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

template <size_t N, typename V, typename E>
boost::shared_ptr<KDNode<N, V, E>> KDTree<N, V, E>::buildTree(
        std::vector<Point<N, V, E>> points,
        unsigned int depth,
        SplitMethod sm) {

    // we have reached the end of a leaf node
    if (points.size() < 1) {
        return NULL;
    }

    // get the current axis to sort on, sort the points
    // based on that and extract the median
    int axis = depth++ % N;
    points = sortByDim(points, axis);
    int split = getSplit(points, sm, axis);

    boost::shared_ptr<KDNode<N, V, E>> node = boost::make_shared<KDNode<N, V, E>>(points[split]);

    // create the new left and right nodes around the median
    std::vector<Point<N, V, E>> _left(points.begin(), points.begin() + split);
    std::vector<Point<N, V, E>> _right(points.begin() + split + 1, points.end());

    // recurse
    node->setLeft(buildTree(_left, depth, sm));
    node->setRight(buildTree(_right, depth, sm));
    return node;
}

template <size_t N, typename V, typename E>
void KDTree<N, V, E>::insert(Point<N, V, E> p) {

    int level = 0;
    int left = 0;
    boost::shared_ptr<KDNode<N, V, E> > cur = this->_head;
    boost::shared_ptr<KDNode<N, V, E> > last = NULL;
    while(1) {
        //update our current axis and then increment the level
        int axis = level++ % N;

        if (cur == NULL) {
            break;
        }
        
        // go left  if current value at axis is less,
        // go right if current value at axis is more.
        last = cur;
        if (p[axis] < cur->atDim(axis)) {
            cur = cur->getLeft();
            left = 1;
        } else {
            cur = cur->getRight();
            left = 0;
        }
    }
    boost::shared_ptr<KDNode<N, V, E>> tmp = boost::make_shared<KDNode<N, V, E>>(p);
    
    // if the last node is NULL then the tree must
    // be empty.
    if (last) {

        // set the parent's pointer to the correct
        // side, left or right.
        if (left) {
            last->setLeft(tmp);
        } else {
            last->setRight(tmp);
        }
    } else {
        this->_head = tmp;
    }
    this->_size++;
}

template <size_t N, typename V, typename E>
void KDTree<N, V, E>::sayhi() {
    this->traverse(this->_head, "");
}

template <size_t N, typename V, typename E>
void KDTree<N, V, E>::traverse(
        boost::shared_ptr<KDNode<N, V, E> > cur,
        std::string loc) {

    if (cur == NULL) {
        return;
    }
    cur->printNode();
    std::cout<<"dir: "<<loc<<std::endl;
    this->traverse(cur->getLeft(), loc + "l ");
    this->traverse(cur->getRight(), loc + "r ");
}

template <size_t N, typename V, typename E>
size_t KDTree<N, V, E>::dim() {
    return N;
}

template <size_t N, typename V, typename E>
size_t KDTree<N, V, E>::size() {
    return this->_size;
}

template <size_t N, typename V, typename E>
bool KDTree<N, V, E>::isEmpty() {
    return this->size() < 1;
}

// recursive method which performs the majority of the search() functionality
template <size_t N, typename V, typename E>
std::unique_ptr<std::multimap<V, Point<N, V, E>>> KDTree<N, V, E>::knnTraverse(
        boost::shared_ptr<KDNode<N, V, E>> cur,
        Point<N, V, E> p,
        std::unique_ptr<std::multimap<V, Point<N, V, E>>> pq,
        const unsigned int k,
        unsigned int level,
        const bool bbf,
        const unsigned int maxlevel) {

    // checks to see if we have reached our maximum level.  When bbf is true
    // max level cooresponds to number of nodes looked at because bbf doesn't
    // ever backtrack.
    bool levelmaxReached = (maxlevel !=0) && (level > maxlevel);

    if (levelmaxReached || cur == NULL) {
        return pq;
    }

    // get the current distance to the main point
    V distance = cur->getPoint().dist(p);

    // add that distance as a key to the pq, the value being the Point.
    // pq is implemented with std::map because std::map is ordered and
    // you can conviniently remove from the bottom.  unlike std::priority_queue
    // where it only holds a single object and you can only pop from the top.
    pq->insert(std::pair<V, Point<N, V, E>>(distance, cur->getPoint()));

    // if the pq gets larger than k, remove the last element
    if (pq->size() > k) {
        pq->erase(std::prev(pq->end()));
    }

    V axis = level++ % N;
    bool left = true;

    // if the value at the axis of the current node is less than p, go left. else go right
    if (p[axis] < cur->atDim(axis)) {
        pq = knnTraverse(cur->getLeft(), p, std::move(pq), k, level, bbf, maxlevel);
    } else {
        pq = knnTraverse(cur->getRight(), p, std::move(pq), k, level, bbf, maxlevel);
        left = false;
    }

    // don't backtrack if we are doing bbf
    if (!bbf) { 

        // if we are within the hypersphere of the point, backtrack
        V best = pq->begin()->first;
        //V best = pq->begin()->second[axis];
        if (pq->size() < k || std::abs(cur->getPoint()[axis] - p[axis]) <= best) {
            if (left) {
                pq = knnTraverse(cur->getRight(), p, std::move(pq), k, level, bbf, maxlevel);
            } else {
                pq = knnTraverse(cur->getLeft(), p, std::move(pq), k, level, bbf, maxlevel);
            }
        }
    }

   return pq;
}

template <size_t N, typename V, typename E>
using search_t = std::multimap<V, Point<N, V, E>>;

template <size_t N, typename V, typename E>
using search_ptr = std::unique_ptr<search_t<N, V, E>>;

template <size_t N, typename V, typename E>
using search_iter = typename search_t<N, V, E>::iterator;

template <size_t N, typename V, typename E>
std::unique_ptr<std::multimap<V, Point<N, V, E>>> KDTree<N, V, E>::search(
        Point<N, V, E> p,
        const unsigned int k,
        const bool bbf,
        const unsigned int maxlevel) {

    boost::shared_ptr<KDNode<N, V, E>> cur = this->_head;
    std::unique_ptr<search_t<N, V, E>> pq = make_unique<search_t<N, V, E>>();
    pq = this->knnTraverse(cur, p, std::move(pq), k, 0, bbf, maxlevel);
    return pq;
}

template <size_t N, typename V, typename E>
bool KDTree<N, V, E>::contains(Point<N, V, E> pt) {
    search_ptr<N, V, E> ret = this->search(pt, 1);

    return pt == ret->begin()->second;
}

// serialize into a binary archive, in the future
// this could be replaced with a choice to go with
// a text archive so the class is more portable.
template <size_t N, typename V, typename E>
void KDTree<N, V, E>::write(std::string fname) {
    std::ofstream ofs(fname);
    boost::archive::binary_oarchive oa(ofs);
    oa & this->_head & this->_size;
}

// read from binary archive.  same comment regarding
// text archives applies here as above.
template <size_t N, typename V, typename E>
void KDTree<N, V, E>::read(std::string fname) {
    std::ifstream ifs(fname);
    boost::archive::binary_iarchive ia(ifs);
    ia & this->_head & this->_size;
}
#endif
