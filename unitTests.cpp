#include "unitTests.h"

std::vector<double> parse_doubles(std::string str, int n) {
    std::vector<double> ret;
    std::stringstream stream(str);
    for( int i = 0; i < n; i++ ) {
        double val;
        stream >> val;
        ret.push_back(val);
    }
    return ret;
}

template <size_t D>
std::vector<Point<D, double, std::string>> get_points(std::string ifname) {
    std::string line;
    std::ifstream myfile (ifname);
    std::vector<Point<D, double, std::string>> points;
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            std::vector<double> vec = parse_doubles(line, D);
            Point<D, double, std::string> p(vec, "");
            points.push_back(p);
        }
    }
    return points;
}

struct gen_rand { 
    double range;
public:
    gen_rand(double r=1.0) : range(r) {}
    double operator()() { 
        return (rand()/(double)RAND_MAX) * range;
    }
};

template <size_t D>
std::vector< Point<D, double, std::string> > genRandStrPoints(int n, double range=1.0) {
    std::vector<double> tmp(D);
    std::vector<Point<D, double, std::string>> ret;
    
    for (int i=0; i<n; i++) {
        std::generate_n(tmp.begin(), D, gen_rand(range));
        Point<D, double, std::string> p(tmp, "");
        ret.push_back(p);
    }

    return ret;
}

void KDTreeTestCase::setUp () {
    
    //Test some stuff with the file
    const std::string ifname = "points.in";
    std::vector<Point<2, double, std::string>> points = get_points<2>(ifname);

    smallTree = make_unique<KDTree<2, double, std::string>>(points);
    //bigTree = make_unique<KDTree<8, double, int>> bigTree;
}

void KDTreeTestCase::checkSize() {
    CPPUNIT_ASSERT (1 == 1);
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
