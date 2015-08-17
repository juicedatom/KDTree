#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> 

#include "KDTree.hpp"
#include "Point.hpp"
#include "unitTests.h"

int main() {

    //Unit test stuff
    // if command line contains "-selftest" then this is the post build check
    // => the output must be in the compiler error format.
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( suite() );   // Add the top suite to the test runner

    // Run the test.
    runner.run("");

    //Test some stuff with the file
    //const std::string ifname = "points.in";
    //std::vector<Point<2, double, std::string>> points = get_points<2>(ifname);
    //KDTree<2, double, std::string> kd(points);

    //std::vector<double> things{85, 97};
    //std::vector<double> more{-1, 10};

    //Point<2, double, std::string> a(more, "");
    //Point<2, double, std::string> b(things, "");

    //add some more points
    //kd.insert(Point<2, double, std::string>(things, ""));
    //kd.sayhi();

    //Point<2, double, int> p(things, 0);
    //for (int i = 0; i<2; i++ ) {
        //std::cout<<p[i]<<" ";
    //}

    //save 
    std::string ofname = "sometree.tree";
    //kd.write(ofname);
    //KDTree<2, double, std::string> saved;
    //std::cout<<"hi!"<<std::endl;
    //saved.read(ofname);
    //saved.sayhi();

    //Look for a point in the saved dataset
    //saved.search(a, 2);
    //Point<2, double, std::string> nn = saved.search(2, a);
    //nn.sayhi();
    

    // Try a bigger dataset
    //const int dim = 8;
    //int n = std::pow(2, 10);
    //double max = 80;

    //std::cout<<"generating points..."<<std::endl;
    //std::vector<Point<dim, double, std::string>> pts = genRandStrPoints<dim>(n, max);

    //std::cout<<"creating tree..."<<std::endl;
    //KDTree<dim, double, std::string> kdb(pts, SplitMethod::MEAN);

    //std::cout<<"writing tree to a file..."<<std::endl;
    //kdb.write(ofname);

    //std::cout<<"reading tree from file..."<<std::endl;
    //KDTree<dim, double, std::string> other;
    //other.read(ofname);

    //std::cout<<"looking for things..."<<std::endl;
    //std::vector<double> hey {40,40,40,40,40,40,40,40};
    //Point<8, double, std::string> tofind(hey, "me!");
    //std::unique_ptr<std::multimap<double, Point<8, double, std::string>>> ret;

    //ret = other.search(tofind, 1);

    //typedef std::multimap<double, Point<8, double, std::string>>::iterator it_type;
    //for (it_type pos = ret->begin(); pos != ret->end(); pos++) {
        //std::cout<<pos->first<<std::endl;
        //Point<8, double, std::string> gg = pos->second;
        //for (int i=0; i<8; i++) {
            //std::cout<<gg[i]<<" ";
        //}
        //std::cout<<std::endl;
    //}
    //other.sayhi();
}
