#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> 

#include "KDTree.hpp"
#include "Point.hpp"

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

template< size_t D >
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
std::vector< Point<D, double, std::string> > genRandStrPoints(int n) {
    std::vector<double> tmp(D);
    std::vector<Point<D, double, std::string>> ret;
    
    for (int i=0; i<n; i++) {
        std::generate_n(tmp.begin(), D, gen_rand());
        Point<D, double, std::string> p(tmp, "");
        ret.push_back(p);
    }

    return ret;
}

int main() {


    //Test some stuff with the file
    const std::string ifname = "points.in";
    std::vector<Point<2, double, std::string>> points = get_points<2>(ifname);
    KDTree<2, double, std::string> kd(points);

    std::vector<double> things{85, 97};
    std::vector<double> more{5, 6};

    //add some more points
    kd.insert(Point<2, double, std::string>(things, ""));
    kd.sayhi();

    Point<2, double, int> p(things, 0);
    for (int i = 0; i<2; i++ ) {
        std::cout<<p[i]<<" ";
    }

    //save 
    std::string ofname = "sometree.tree";
    kd.write(ofname);
    KDTree<2, double, int> saved;

    // Try a bigger dataset
    const int dim = 8;
    int n = 1000;
    std::vector<Point<dim, double, std::string>> pts = genRandStrPoints<dim>(n);
    KDTree<dim, double, std::string> kdb(pts);

    kdb.write(ofname);
    KDTree<dim, double, std::string> other;
    other.read(ofname);
    other.sayhi();
}
