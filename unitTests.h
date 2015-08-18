#include <memory>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "memoryHelper.hpp"
#include "KDTree.hpp"
#include "genPoints.hpp"

#define _D_ 8
#define _N_TEST_POINTS 5000
#define _N_1_SEARCH_TEST 10
#define _MAX_PT_VAL 100
#define _TREE_FNAME "./mytree.tree"

class KDTreeTestCase : public CppUnit::TestCase {
    
    std::unique_ptr<KDTree<_D_, double, std::string>> tree;
    std::unique_ptr<KDTree<_D_, double, std::string>> emptyTree;
    std::unique_ptr<std::vector<Point<_D_, double, std::string>>> points;
    void checkSize();
    void singleSearchExact();
    void checkInsert();
    void saveAndLoad();
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
