#include <memory>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "memoryHelper.hpp"
#include "KDTree.hpp"

class KDTreeTestCase : public CppUnit::TestCase {
    
    std::unique_ptr<KDTree<2, double, std::string>> smallTree;
    //std::unique_ptr<KDTree<8, double, int>> bigTree;
    void checkSize();

    CPPUNIT_TEST_SUITE( KDTreeTestCase );
    CPPUNIT_TEST( checkSize );
    CPPUNIT_TEST_SUITE_END();

    public: 
        void setUp();
};

CppUnit::Test *suite();
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( KDTreeTestCase, "KDTreeTestCase" );

struct gen_rand;
std::vector<double> parse_doubles(std::string, int);

//class ExampleTestCase : public CppUnit::TestCase {

    //double m_value1;
    //double m_value2;
    //void example();
    //void anotherExample();
    //void testEquals();

    //CPPUNIT_TEST_SUITE( ExampleTestCase );
    //CPPUNIT_TEST( example );
    //CPPUNIT_TEST( anotherExample );
    //CPPUNIT_TEST( testEquals );
    //CPPUNIT_TEST_SUITE_END();

    //public:
        //void setUp();
//};

//CppUnit::Test *suite();

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ExampleTestCase, "ExampleTestCase" );
