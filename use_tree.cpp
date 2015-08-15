#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> 

#include "KDTree.h"

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

std::vector<std::vector<double> > get_points(std::string fname) {
    std::string line;
    std::ifstream myfile (fname);
    std::vector<std::vector<double> > points;
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            std::vector<double> vec = parse_doubles(line, 3);
            points.push_back(vec);
            std::cout<<std::endl;
        }
    }
    return points;
}

int main() {

    const std::string fname = "points.in";
    std::vector<std::vector<double> > points = get_points(fname);
    KDTree<3, int> kd(points);
    std::cout<<"size of points: " << points.size() << std::endl;
    kd.insert(points[0]);
    kd.sayhi();

}
