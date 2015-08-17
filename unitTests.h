#include <memory>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "memoryHelper.hpp"

class ExampleTestCase : public CppUnit::TestCase {

    double m_value1;
    double m_value2;
    void example();
    void anotherExample();
    void testEquals();

    CPPUNIT_TEST_SUITE( ExampleTestCase );
    CPPUNIT_TEST( example );
    CPPUNIT_TEST( anotherExample );
    CPPUNIT_TEST( testEquals );
    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
};

CppUnit::Test *suite();

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ExampleTestCase, "ExampleTestCase" );
