#include "unitTests.h"

struct gen_rand { 
    double range;
public:
    gen_rand(double r=1.0) : range(r) {}
    double operator()() { 
        return (rand()/(double)RAND_MAX) * range;
    }
};

template <size_t D>
std::unique_ptr<std::vector< Point<D, double, std::string> >> genRandStrPoints(
        int n, double range=1.0) {

    std::vector<double> tmp(D);

    std::unique_ptr<std::vector<Point<D, double, std::string>>> ret =
        make_unique<std::vector<Point<D, double, std::string>>>();
    
    for (int i=0; i<n; i++) {
        std::generate_n(tmp.begin(), D, gen_rand(range));
        Point<D, double, std::string> p(tmp, "");
        ret->push_back(p);
    }

    return ret;
}

void KDTreeTestCase::setUp () {
    
    //Test some stuff with the file
    testTreePoints = genRandStrPoints<8>(_N_TEST_POINTS, 50);
    testTree = make_unique<KDTree<8, double, std::string>>(*testTreePoints);
    emptyTree = make_unique<KDTree<2, double, std::string>>();
}

void KDTreeTestCase::checkSize() {
    CPPUNIT_ASSERT_EQUAL_MESSAGE("check size of empty tree",
            (size_t) 0, (size_t) emptyTree->size());

    // Make sure the tree parsed some data
    CPPUNIT_ASSERT_MESSAGE("check the points.in to make sure the points are being read property",
            testTreePoints->size() != 0);

    // Make sure the tree is not empty
    CPPUNIT_ASSERT_MESSAGE("check if tree is empty after given points",
            testTree->size() != 0);

    // make sure the tree parsed all the data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("small tree init size check fail",
            (size_t) _N_TEST_POINTS, testTree->size());

    // Make sure the number of points read in is the same as those added
    CPPUNIT_ASSERT_EQUAL_MESSAGE("check the number of elements in the saml tree vs points.in", 
            testTreePoints->size(), testTree->size());
}

void KDTreeTestCase::checkSearch() {
}

void KDTreeTestCase::checkInsert() {
    //test insert methods here
}

CppUnit::Test *suite() {
    CppUnit::TestFactoryRegistry &registry =
        CppUnit::TestFactoryRegistry::getRegistry();

  registry.registerFactory(
      &CppUnit::TestFactoryRegistry::getRegistry( "KDTreeTestCase" ) );
  return registry.makeTest();
}

//void ExampleTestCase::setUp () {
    //m_value1 = 2.0;
    //m_value2 = 3.0;
//}


//void ExampleTestCase::example () {
    //CPPUNIT_ASSERT (1 == 1);
//}


//void ExampleTestCase::anotherExample () {
    //CPPUNIT_ASSERT (2 == 2);
//}


//void ExampleTestCase::testEquals () {
    ////std::auto_ptr<long> l1 (new long (12));
    ////std::auto_ptr<long> l2 (new long (12));
    //std::unique_ptr<long> l1 = make_unique<long>(12);
    //std::unique_ptr<long> l2 = make_unique<long>(12);

    //CPPUNIT_ASSERT_DOUBLES_EQUAL (m_value1, 2.0, 0.01);
    //CPPUNIT_ASSERT_DOUBLES_EQUAL (m_value2, 3.0, 0.01);
    //CPPUNIT_ASSERT_EQUAL (12, 12);
    //CPPUNIT_ASSERT_EQUAL (12L, 12L);
    //CPPUNIT_ASSERT_EQUAL (*l1, *l2);

    //CPPUNIT_ASSERT(12L == 12L);
    //CPPUNIT_ASSERT_DOUBLES_EQUAL (12.0, 11.99, 0.5);
//}


//CppUnit::Test *suite() {
  //CppUnit::TestFactoryRegistry &registry =
                      //CppUnit::TestFactoryRegistry::getRegistry();

  //registry.registerFactory(
      //&CppUnit::TestFactoryRegistry::getRegistry( "ExampleTestCase" ) );
  //return registry.makeTest();
//}
