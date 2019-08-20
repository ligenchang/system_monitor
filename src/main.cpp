#include "ncurses_display.h"
#include "system.h"
#include "log.h"
#include <fstream>

int main() {

  System system;
  NCursesDisplay::Display(system);
  
}