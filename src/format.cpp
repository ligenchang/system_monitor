#include <string>

#include "format.h"
#include <sstream>
#include <iomanip>
#include "log.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

std::string to_format(const int number) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << number;
    return ss.str();
}
string Format::ElapsedTime(long seconds) { 
    int time = seconds;
	int hour = 0;
	int min = 0;
	int sec = 0;
    hour = time/3600;
	time = time%3600;
	min = time/60;
	time = time%60;
	sec = time;
    return to_format(hour) + ":"+ to_format(min) + ":" + to_format(sec);
 }

