#include <memory>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "memoryHelper.hpp"
#include "KDTree.hpp"
#include "genPoints.hpp"

// how many dimensions we want to run our tests in
#define _D_ 8

// number of points to generate for our test tree
#define _N_TEST_POINTS 1000

//maximum value for the points in the randomly generated
//test tree.
#define _MAX_PT_VAL 100

//number of points to check for the checkInsert() test
//see below...
#define _N_1_SEARCH_TEST 10

//filename to save the tree during the saveLoadTest()
#define _TREE_FNAME "./mytree.tree"

class KDTreeTestCase : public CppUnit::TestCase {
   
    std::unique_ptr<KDTree<_D_, double, std::string>> tree;
    std::unique_ptr<KDTree<_D_, double, std::string>> emptyTree;
    std::unique_ptr<std::vector<Point<_D_, double, std::string>>> points;

    /**
     * Checks the size of various operations on trees
     *
     * 1. Makes sure an empty tree has 0 size
     * 2. Make sure a populated tree has a size
     * 3. Make sure the number of test points
     *    matches the size of the tree.
     */
    void checkSize();

    /**
     * Checks to make sure we can get exact values
     * out of trees.
     *
     * 1. verify points can be compared to equal points
     * 2. make sure points are recognized to be different
     * 3. grab a random point from the points vector and
     *    make sure it is in the tree
     * 4. repeat step 3 _N_1_TREE_SEARCH_TEST times.
     */
    void singleSearchExact();


    /**
     * Make sure we can insert a point succesfully
     *
     * 1. Insert point p, make sure the size of the
     *    tree increased by one.
     * 2. check if tree contains inserted point
     * 3. Insert point into empty tree, make sure
     *    that point exists in empty tree.
     */
    void checkInsert();

    /**
     * writes a tree to a file, reads from it and make sure
     * the contents exist.
     *
     * 1. save tree, load tree into a copy and make sure it
     *    has the same size as the original tree.
     * 2. for each point in original tree, make sure it exists
     *    in the copied tree
     */
    void saveAndLoad();

    /**
     * A very basic knn test  for k == 3
     *
     * 1. Generate four random points which are all in increasing 
     *    distance from the previous point to other points.
     * 2. Insert the latter three of those points into the tree and seach for
     *    the three nearest points to the first point created.
     * 3. run a search knn search (k == 3) for the first point
     *    and make sure the other three points appear in order.
     */
    void nearestNeighbor();

    CPPUNIT_TEST_SUITE( KDTreeTestCase  );
    CPPUNIT_TEST( checkSize             );
    CPPUNIT_TEST( singleSearchExact     );
    CPPUNIT_TEST( checkInsert           );
    CPPUNIT_TEST( saveAndLoad           );
    CPPUNIT_TEST( nearestNeighbor       );
    CPPUNIT_TEST_SUITE_END();

    public: 
        void setUp();
};

CppUnit::Test *suite();

struct gen_rand;
std::vector<double> parse_doubles(std::string, int);
