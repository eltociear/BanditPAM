/**
 * @file kmeds_pywrapper.cpp
 * @date 2020-06-10
 *
 * This file creates the Python bindings for the C++ code that
 * allows it to be called in Python.
 *
 */
#include <armadillo>
#include <carma/carma.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "kmedoids_ucb.hpp"

namespace py = pybind11;

/**
 *  \brief Python wrapper for KMedoids class.
 *
 *  KMedsWrapper class. Is the Python wrapper generated by Pybind that allows for calling
 *  the C++ code in Python3.
 */
class KMedsWrapper : public KMedoids {
public:
  using KMedoids::KMedoids;

  /**
   * This is the main function of the KMedoids module: this finds the build and swap
   * medoids for the desired data
   *
   * @param inputData Input data to find the medoids of
   * @param loss The loss function used during medoid computation
   * @param k The number of medoids to compute
   * @param logFilename The name of the outputted log file
   */
  void fitPython(py::array_t<double> inputData, std::string loss, int k, std::string logFilename) {
    KMedoids::setNMedoids(k);
    KMedoids::setLogFilename(logFilename);
    KMedoids::fit(carma::arr_to_mat<double>(inputData), loss);
  }

  /**
   * This function returns the final medoids for a KMedoids object.
   */
  py::array_t<double> getMedoidsFinalPython() {
    return carma::row_to_arr<double>(KMedoids::getMedoidsFinal()).squeeze();
  }

  /**
   * This function returns the build medoids for a KMedoids object.
   */
  py::array_t<double> getMedoidsBuildPython() {
    return carma::row_to_arr<double>(KMedoids::getMedoidsBuild()).squeeze();
  }

  /**
   * This function returns the labels/medoids assignments for each datapoint after the final
   * medoids are identified.
   */
  py::array_t<double> getLabelsPython() {
    return carma::row_to_arr<double>(KMedoids::getLabels()).squeeze();
  }

  /**
   * This function returns the number of swap steps that took place during the computation
   */
  int getStepsPython() {
    return KMedoids::getSteps();
  }
};

PYBIND11_MODULE(BanditPAM, m) {
  m.doc() = "BanditPAM Python library, implemented in C++";
  py::class_<KMedsWrapper>(m, "KMedoids")
      .def(py::init<int, std::string, int, int, std::string>(),
        py::arg("nMedoids") = 5,
        py::arg("algorithm") = "BanditPAM",
        py::arg("verbosity") = 0,
        py::arg("maxIter") = 1000,
        py::arg("logFilename") = "KMedoidsLogfile"
      )
      .def_property("nMedoids", &KMedsWrapper::getNMedoids, &KMedsWrapper::setNMedoids)
      .def_property("algorithm", &KMedsWrapper::getAlgorithm, &KMedsWrapper::setAlgorithm)
      .def_property("verbosity", &KMedsWrapper::getVerbosity, &KMedsWrapper::setVerbosity)
      .def_property("maxIter", &KMedsWrapper::getMaxIter, &KMedsWrapper::setMaxIter)
      .def_property("logFilename", &KMedsWrapper::getLogfileName, &KMedsWrapper::setLogFilename)
      .def_property_readonly("finalMedoids", &KMedsWrapper::getMedoidsFinalPython)
      .def_property_readonly("buildMedoids", &KMedsWrapper::getMedoidsBuildPython)
      .def_property_readonly("labels", &KMedsWrapper::getLabelsPython)
      .def_property_readonly("steps", &KMedsWrapper::getStepsPython)
      .def("fit", &KMedsWrapper::fitPython);
}
