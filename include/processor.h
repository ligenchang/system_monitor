#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 float PREV_TOTAL=0;
 float PREV_IDLE=0;
 float TOTAL=0;
 float IDLE;
};

#endif