#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
#include <unistd.h>

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {

    float MemTotal{0};
    float MemFree{0};
    float MemAvailable{0};
    float Buffers{0};
    float Total_used_memory{0};
    float Non_cache_memory{0};


    std::string line;
    std::ifstream mem_utiliz_stream(kProcDirectory + kMeminfoFilename);
    std::string a,b;
    std::vector<float> memory_values;
    if (mem_utiliz_stream.is_open()){
        for (int i=0; i<4; i++){
            std::getline(mem_utiliz_stream, line);
            std::istringstream linestream(line);
            linestream >> a>>b;
            memory_values.emplace_back(std::stof(b));
        }
        MemTotal = memory_values.at(0);
        MemFree = memory_values.at(1);
        MemAvailable = memory_values.at(2);
        Buffers = memory_values.at(3);

    }
    Total_used_memory = MemTotal - MemFree;
    Non_cache_memory = Total_used_memory - Buffers;  //green

    return (Non_cache_memory/MemAvailable);

}


// TODO: Read and return the system uptime
long LinuxParser::UpTime() {

    long uptime{0};
    long idle_time{0};
    std::string line, uptime_s, idle_time_s;

    std::ifstream Timestream (kProcDirectory + kUptimeFilename);

    if(Timestream.is_open()){
        std::getline(Timestream, line);
        std::istringstream linestream(line);
        linestream>>uptime_s>>idle_time_s;
        uptime = std::stol(uptime_s);
        idle_time = std::stol(idle_time_s);
    }

    return uptime;

}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {

  return (IdleJiffies() + ActiveJiffies());

}

// TODO: Read and return the number of active jiffies for a PID

long LinuxParser::ActiveJiffies(int pid ){

  std::string pid_line, val;
  long jiffies_1,jiffies_2;
  std::ifstream pid_jiffies_stream(kProcDirectory + std::to_string(pid) + kStatFilename );
  if(pid_jiffies_stream.is_open()){
    std::getline(pid_jiffies_stream, pid_line);
    std::istringstream mystream(pid_line);

    for(int i=0; i<17; i++){
        mystream>>val;
        if(i==13){
          jiffies_1 = stol(val);
        }
        if(i==16){
          jiffies_2 = stol(val);
        }
    }

    return jiffies_1 + jiffies_2 ;

  }

  return 0;

}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {

  auto jiffies = CpuUtilization();
  long num = stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_])+stol(jiffies[CPUStates::kSystem_])+stol(jiffies[CPUStates::kIRQ_])+
             stol(jiffies[CPUStates::kSoftIRQ_])+stol(jiffies[CPUStates::kSteal_]);
  return num;

}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {

  auto jiffies = CpuUtilization();
  long num = stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);

  return num;

}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {

    std::string first_line;
    std::vector<std::string> string_nums(10);
    std::string el0, el1, el2, el3, el4, el5, el6, el7, el8, el9, el10;

    std::ifstream stream(kProcDirectory + kStatFilename);

    if(stream.is_open()){

        std::getline(stream, first_line);
        std::istringstream linestream(first_line);

        linestream >> el0>>el1>>el2>>el3>>el4>>el5>>el6>>el7>>el8>>el9>>el10;

        string_nums = {el1,el2,el3,el4,el5,el6,el7,el8,el9,el10};

        return string_nums;

    }

    return string_nums;

}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {

    std::string line, first_element, second_element;
    std::ifstream processes_stream(kProcDirectory+ kStatFilename);
    if (processes_stream.is_open()){

        while(std::getline(processes_stream, line)){

            std::istringstream  linestream(line);
            linestream >> first_element>> second_element;
            if (first_element=="processes"){
                return std::stoi(second_element);
            }

        }
    }

    return 0;

}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {

    std::string line, first_element, second_element;
    std::ifstream processes_stream(kProcDirectory+ kStatFilename);
    if (processes_stream.is_open()){

        while(std::getline(processes_stream, line)){

            std::istringstream  linestream(line);
            linestream >> first_element>> second_element;
            if (first_element=="procs_running"){
                return std::stoi(second_element);
            }

        }
    }

    return 0;

}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
//  /proc/[pid]/cmdline
  std::string cmd{};
  std::ifstream command_stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(command_stream.is_open()){
    std::getline(command_stream, cmd);
    return cmd;
  }

  return cmd;

}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {

  std::ifstream  memory_stream(kProcDirectory + std::to_string(pid)+ kStatusFilename);
  std::string line;
  std::string num{};
  std::string str;
  if(memory_stream.is_open()){
    while(std::getline(memory_stream, line)){
      std::istringstream mystream(line);
      mystream>>str>>num;
      if (str=="VmSize:"){
        return num;
      }
    }
  }
  return num;

}

// TODO: Read and return the user ID associated with a process

string LinuxParser::Uid(int pid){

  std::ifstream user_stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, str;
  std::string num{} ;
  if(user_stream.is_open()){
    while(std::getline(user_stream, line)){
      std::istringstream user_id_stream(line);
      user_id_stream>>str>>num;
      if(str =="Uid:"){
        return num;
      }
    }
  }
  return num;
}

// TODO: Read and return the user associated with a process

string LinuxParser::User(int pid) {

  std::string username{};
  std::string line;
  std::ifstream username_file( kPasswordPath );
  if(username_file.is_open()){
    while(std::getline(username_file , line) ) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream username_stream(line);
      username_stream>>username;
    }
    return username ;
  }
  return username;

}

// TODO: Read and return the uptime of a process

long LinuxParser::UpTime(int pid) {
//  /proc/[pid]/stat.
    std::string uptime_line;
    std::string l0,l1,l2,l3,l4,l5,l6,l7,l8,l9,l10,l11,l12,l13,l14,l15,l16,l17,l18,l19,l20,l21,l22;
    std::ifstream uptime_file(kProcDirectory + std::to_string(pid) + kStatFilename  );

    if (uptime_file.is_open()){

      std::getline(uptime_file, uptime_line);
      std::istringstream mystream(uptime_line);
      mystream>>l0>>l1>>l2>>l3>>l4>>l5>>l6>>l7>>l8>>l9>>l10>>l11>>l12>>l13>>l14>>l15>>l16>>l17>>l18>>l19>>l20>>l21;
      long clock_ticks = stol(l21);
      long time_var = clock_ticks / sysconf(_SC_CLK_TCK);

      return  time_var;

    }

    return 0;

}
