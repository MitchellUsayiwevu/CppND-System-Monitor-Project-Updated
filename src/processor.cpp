#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> cpu_strings =  LinuxParser::CpuUtilization();

    double  user_ = std::stod(cpu_strings.at(0));
    double nice_ = std::stod(cpu_strings.at(1));
    double system_=std::stod(cpu_strings.at(2));
    double idle_ = std::stod(cpu_strings.at(3));
    double iowait_ = std::stod(cpu_strings.at(4));
    double irq_ = std::stod(cpu_strings.at(5));
    double softirq_ = std::stod(cpu_strings.at(6));
    double steal_ = std::stod(cpu_strings.at(7));
    double guest_ = std::stod(cpu_strings.at(8));
    double guest_nice_ = std::stod(cpu_strings.at(9));

    double Idle = idle_ + iowait_;
    double NonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
    double Total = Idle + NonIdle;

    return float((Total -Idle)/Total);
}