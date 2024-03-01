#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>
#include <iostream>
#include <string>

struct BaseConfig {
    std::string sensorPath;
    uint8_t entityId;
    std::string shownName;
    uint8_t sensorType;
};

struct BaseConfigCompare {
    bool operator()(const BaseConfig &a, const BaseConfig &b) const { return a.sensorPath < b.sensorPath; }
};

using NumMap = boost::bimap<int, boost::bimaps::set_of<BaseConfig, BaseConfigCompare>>;

int main() {
    NumMap numMap;
    numMap.insert(NumMap::value_type(777, BaseConfig{"Inlet_Temp", 4, "Inlet Temp", 5}));

    std::cout << numMap.left.at(777).sensorPath << std::endl;
    std::cout << numMap.right.at(BaseConfig{"Inlet_Temp"}) << std::endl;

    return 0;
}