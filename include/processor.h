#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    double user_;
    double nice_;
    double system_;
    double idle_;
    double iowait_;
    double  irq_;
    double  softirq_;
    double steal_;
    double guest_;
    double guest_nice_;
};

#endif