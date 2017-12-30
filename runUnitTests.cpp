#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector> 

#include "KDTree.hpp"
#include "Point.hpp"
#include "unitTests.h"

int main() {

    //Run unit tests
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( suite() );   // Add the top suite to the test runner

    // Run the test.
    runner.run("");
}
