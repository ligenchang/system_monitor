#include "processor.h"
#include <string>
#include <vector>
#include "log.h"
#include "linux_parser.h"
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  string line;
  string key;
  string value;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guest_nice;

  float cpu_user;
  float cpu_nice;
  float cpu_system;
  float cpu_idle;
  float cpu_iowait;
  float cpu_irq;
  float cpu_softirq;
  float cpu_steal;



  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> user >> nice >> system >>idle >> iowait >>irq >>softirq >>steal >>guest >>guest_nice) {

        if (key == "cpu") {
          cpu_user=stoi(user);
          cpu_nice=stoi(nice);
          cpu_system=stoi(system);
          cpu_idle=stoi(idle);
          cpu_iowait=stoi(iowait);
          cpu_irq=stoi(irq);
          cpu_softirq=stoi(softirq);
          cpu_steal=stoi(steal);
        }

      }
    }
  }
  float total_cpu_since_boot = cpu_user+cpu_nice+cpu_system+cpu_idle+cpu_iowait+cpu_irq+cpu_softirq+cpu_steal;
  float total_cpu_idle_boot =  cpu_idle + cpu_iowait;
  TOTAL= total_cpu_since_boot;
  IDLE= total_cpu_idle_boot;

  float diff_total=TOTAL-PREV_TOTAL;
  float diff_idle=IDLE-PREV_IDLE;
  
  float diff_usage=(1000*(diff_total-diff_idle)/diff_total+5)/10/100;

  PREV_TOTAL=TOTAL;
  PREV_IDLE=IDLE;


  return diff_usage;

}