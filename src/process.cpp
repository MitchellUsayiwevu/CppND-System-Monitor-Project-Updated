#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int id):pid_(id){

}

// TODO: Return this process's ID
int Process::Pid() {

    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  float temp =LinuxParser::ActiveJiffies(pid_ );
//  std::cout<<"cpu ut"<< temp <<std::endl;
  return temp;


}

// TODO: Return the command that generated this process
string Process::Command() {

    return LinuxParser::Command(pid_) ;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  try{
    std::string kb_ram = LinuxParser::Ram( pid_) ;
    long mb_ram = stol(kb_ram)*0.0009765625;            //convert from KB to MB
    return std::to_string(mb_ram);
  }
  catch(...){

  }
  return {"0"};
}

// TODO: Return the user (name) that generated this process
string Process::User() {

  return  LinuxParser::User(pid_);

}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {

  return  LinuxParser::UpTime(pid_);

}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {

//  float CPU_a = LinuxParser::ActiveJiffies(a.pid_);
//  float CPU_b = LinuxParser::ActiveJiffies(this->pid_);


    return this->CpuUtilization() < a.CpuUtilization();
}