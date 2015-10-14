#include <string>

#include <boost/python.hpp>
#include "getfeature.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(RBML)
{
    class_<GetFeature>("GetFeature", init<const std::string &>())
            .def(init<const std::string &>())
            .def("get_feature", &GetFeature::get_feature);
}
