#include <memory>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "memoryHelper.hpp"
#include "KDTree.hpp"

#define _N_TEST_POINTS 1000

class KDTreeTestCase : public CppUnit::TestCase {
    
    std::unique_ptr<KDTree<8, double, std::string>> testTree;
    std::unique_ptr<KDTree<2, double, std::string>> emptyTree;
    std::unique_ptr<std::vector<Point<8, double, std::string>>> testTreePoints;
    void checkSize();
    void checkSearch();
    void checkInsert();

    CPPUNIT_TEST_SUITE( KDTreeTestCase );
    CPPUNIT_TEST( checkSize   );
    CPPUNIT_TEST( checkSearch );
    CPPUNIT_TEST( checkInsert );
    CPPUNIT_TEST_SUITE_END();

    public: 
        void setUp();
};

CppUnit::Test *suite();
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( KDTreeTestCase, "KDTreeTestCase" );

struct gen_rand;
std::vector<double> parse_doubles(std::string, int);
