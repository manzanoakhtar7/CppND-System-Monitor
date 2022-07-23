#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {

/**
 * @brief Convert elapsed time in seconds into a formatted string with the form
 * HH:MM:SS
 *
 * @param seconds elapsed time in seconds
 * @return std::string elapsed time formatted as HH:MM:SS
 */
std::string ElapsedTime(long seconds);

};  // namespace Format

#endif