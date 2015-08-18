#ifndef GENPOINTS_H
#define GENPOINTS_H

// function object to be used by std::generate_n
// to populate a vector<double> with a bunch of
// random numbers
struct gen_rand { 
    double range;
public:
    gen_rand(double r=1.0) : range(r) {}
    double operator()() { 
        return (rand()/(double)RAND_MAX) * range;
    }
};

// Randomly generates a n Points<D, double, str::string> and fills them with
// and empty string, ""
template <size_t D>
std::unique_ptr<std::vector< Point<D, double, std::string> >> genRandStrPoints(
        int n, double range=1.0) {

    std::vector<double> tmp(D);

    std::unique_ptr<std::vector<Point<D, double, std::string>>> ret =
        make_unique<std::vector<Point<D, double, std::string>>>();
    
    for (int i=0; i<n; i++) {
        std::generate_n(tmp.begin(), D, gen_rand(range));
        Point<D, double, std::string> p(tmp, "<DEFAULT>");
        ret->push_back(p);
    }

    return ret;
}

#endif
