#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long _lastActiveJiffies{-1};
  long _lastTotalJiffies{-1};
};

#endif