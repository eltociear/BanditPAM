/**
 * @file main.cpp
 * @date 2020-06-10
 *
 * Defines a command line program that can be used
 * to run the BanditPAM KMedoids algorithm.
 *
 * Usage (from home repo directory):
 * ./cluster -f [path/to/input] -k [number of clusters]
 */

#include "kmedoids_ucb.hpp"
#include <armadillo>
#include <chrono>
#include <fstream>
#include <unistd.h>

int main(int argc, char* argv[])
{
    std::string input_name;
    int k;
    int opt;
    int verbosity = 0;
    std::string loss = "L2";

    while ((opt = getopt(argc, argv, "f:k:v:")) != -1) {
        switch (opt) {
            // path to the data file to be read in
            case 'f':
                input_name = optarg;
                break;
            // number of clusters to create
            case 'k':
                k = std::stoi(optarg);
                break;
            // type of loss/distance function to use
            case 'l':
                loss = optarg;
            // set the verbosity of the algorithm
            case 'v':
            	verbosity = std::stoi(optarg);
            	break;
            case ':':
                printf("option needs a value\n");
                return 1;
            case '?':
                printf("unknown option: %c\n", optopt);
                return 1;
        }
    }
    arma::mat data;
    data.load(input_name);
    arma::uword n = data.n_cols;
    arma::uword d = data.n_rows;

    KMedoids kmed;
    kmed.fit(data, loss);
    arma::rowvec meds = kmed.getMedoidsFinal();

    kmed.setNMedoids(10);
    std::cout << kmed.n_medoids << std::endl;

    kmed.setLogFilename("hellologfile");
    std::cout << kmed.logFilename << std::endl;


    std::cout << "Medoids:";
    for (size_t i = 0; i < k; i++) {
      std::cout << meds(i) << ", ";
    }
    std::cout << std::endl;
    std::cout << "Steps:" << kmed.getSteps() << std::endl;
}
