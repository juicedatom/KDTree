#include "unitTests.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( KDTreeTestCase, "KDTreeTestCase");

void KDTreeTestCase::setUp () {
    //Test some stuff with the file
    points = genRandStrPoints<_D_>(_N_TEST_POINTS, _MAX_PT_VAL);
    tree = make_unique<KDTree<_D_, double, std::string>>(*points);
    emptyTree = make_unique<KDTree<_D_, double, std::string>>();
}

void KDTreeTestCase::checkSize() {
    CPPUNIT_ASSERT_EQUAL_MESSAGE("check size of empty tree",
            (size_t) 0, (size_t) emptyTree->size());

    // Make sure the tree parsed some data
    CPPUNIT_ASSERT_MESSAGE("check the points.in to make sure the points are being read property",
            points->size() != 0);

    // Make sure the tree is not empty
    CPPUNIT_ASSERT_MESSAGE("check if tree is empty after given points",
            tree->size() != 0);

    // make sure the tree parsed all the data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("small tree init size check fail",
            (size_t) _N_TEST_POINTS, tree->size());

    // Make sure the number of points read in is the same as those added
    CPPUNIT_ASSERT_EQUAL_MESSAGE("check the number of elements in the tree vs points.in", 
            points->size(), tree->size());
}

void KDTreeTestCase::singleSearchExact() {
    
    //make sure we can compare points accuratly
    double a[3] = {1,2,3};
    double b[3] = {3,2,1};

    //Technically i could do 16 different tests but for now
    //I'm going to test the two that are important to this
    //test.  If i were to keep going, i would have separate
    //tests for points
    Point<3, double, std::string> aa(a, "hello");
    Point<3, double, std::string> aa_2(a, "hello");
    Point<3, double, std::string> ba(b, "goodbye");

    CPPUNIT_ASSERT_MESSAGE("make sure points can be compared with similiar points",
            aa == aa_2);

    CPPUNIT_ASSERT_MESSAGE("make sure points can be compared with different points",
            aa != ba);
  
    Point<_D_, double, std::string> pt;
    search_ptr<_D_, double, std::string> ret;
    // Grab a bunch of random known point and look for it in the tree
    for (int test = 0; test < _N_1_SEARCH_TEST; test++) {
        pt = points->at(rand() % _N_TEST_POINTS);

        ret = tree->search(pt);

        search_iter<_D_, double, std::string> pos; 
        for( pos = ret->begin(); pos != ret->end(); pos++) {
            CPPUNIT_ASSERT_MESSAGE("checking if point transferred ok",
                pos->second == pt);
        }
    }
}

void KDTreeTestCase::checkInsert() {

    //this vector can't possibly be in the array because it
    //has a value above the _MAX_PT_VAL
    double arr[_D_];
    arr[0] = _MAX_PT_VAL * 2;
    for (int i=1; i<_D_; i++) {
        arr[i] = i;
    }

    Point<_D_, double, std::string> pt(arr, "sup");

    size_t n = tree->size() + 1;
    tree->insert(pt);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("check if size increased when adding a point",
            n, tree->size());

    CPPUNIT_ASSERT_MESSAGE("check if tree can insert point",
            tree->contains(pt));

    Point<_D_, double, std::string> pt_b(arr, "sup");
    emptyTree->insert(pt);

    CPPUNIT_ASSERT_MESSAGE("check inserting point into an empty tree",
            emptyTree->contains(pt));
}

void KDTreeTestCase::saveAndLoad() {
    
    tree->write(_TREE_FNAME);

    std::unique_ptr<KDTree<_D_, double, std::string>> treeCopy =
        make_unique<KDTree<_D_, double, std::string>>();

    treeCopy->read(_TREE_FNAME);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("check if loaded tree has same size as original",
            tree->size(), treeCopy->size());

    for (size_t i=0; i<points->size(); i++ ){
        search_ptr<_D_, double, std::string> ret;
        ret = tree->search(points->at(i), 1);
        CPPUNIT_ASSERT_MESSAGE("checking if point transferred ok",
                treeCopy->contains(points->at(i)));
    }
}

void KDTreeTestCase::nearestNeighbor() {
    
    // Because these two points both contain a point outside the max
    // value they must be the ones closest to each other
    double arr_a[_D_];
    arr_a[0] = _MAX_PT_VAL * + 3;
    Point<_D_, double, std::string> pt_a(arr_a, "sup");

    double arr_b[_D_];
    arr_b[0] = _MAX_PT_VAL * + 4;
    Point<_D_, double, std::string> pt_b(arr_b, "brah");

    double arr_c[_D_];
    arr_c[0] = _MAX_PT_VAL * + 5;
    Point<_D_, double, std::string> pt_c(arr_c, "im");

    double arr_d[_D_];
    arr_d[0] = _MAX_PT_VAL * + 6;
    Point<_D_, double, std::string> pt_d(arr_d, "fine");

    std::vector<Point<_D_, double, std::string>> vec;
    vec.push_back(pt_b);
    vec.push_back(pt_c);
    vec.push_back(pt_d);

    for (size_t i=0; i<vec.size(); i++) {
        tree->insert(vec[i]);
    }

    search_ptr<_D_, double, std::string> ret;
    ret = tree->search(pt_a);

    search_iter<_D_, double, std::string> pos; 
    int i=0;
    for( pos = ret->begin(); pos != ret->end(); pos++) {
        CPPUNIT_ASSERT_MESSAGE("checking if point transferred ok",
            pos->second == vec[i++]);
    }

}

CppUnit::Test *suite() {
    CppUnit::TestFactoryRegistry &registry =
        CppUnit::TestFactoryRegistry::getRegistry();

    registry.registerFactory(
      &CppUnit::TestFactoryRegistry::getRegistry( "KDTreeTestCase" ) );

    return registry.makeTest();
}

