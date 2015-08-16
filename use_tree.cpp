#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> 

#include "KDTree.hpp"

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

std::vector<std::vector<double> > get_points(std::string fname, int dim) {
    std::string line;
    std::ifstream myfile (fname);
    std::vector<std::vector<double> > points;
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            std::vector<double> vec = parse_doubles(line, dim);
            points.push_back(vec);
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

std::vector< std::vector<double> > genRandPoints(int n, int dim) {
    std::vector< std::vector<double> > ret;
    std::vector<double> x(dim);

    for(int i=0; i<n; i++ ){
        std::generate_n(x.begin(), dim, gen_rand());
        ret.push_back(x);
    }

    return ret;
}

int main() {

    //Test some stuff with the file
    const std::string fname = "points.in";
    std::vector<std::vector<double> > points = get_points(fname, 2);
    std::vector<double> things{-3, 14};
    //std::vector<double> more{5,6,2};
    KDTree<2, int> kd(points);
    kd.insert(things);
    //kd.insert(more);

    std::string ggname = "sometree.tree";
    kd.write(ggname);
    KDTree<2, int> saved;
    saved.read(ggname);
    saved.sayhi();

    //make some points
    //const int dim = 8;
    //int n = 1000;
    //std::vector< std::vector<double> > pts = genRandPoints(n, dim);
    //KDTree<dim, int> kdb(pts);
}
