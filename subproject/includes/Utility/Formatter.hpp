#ifndef __CONAN_PACKAGE_EXAMPLE_FORMATTER_HPP
#define __CONAN_PACKAGE_EXAMPLE_FORMATTER_HPP

#include "cassandra.h"

#include <string>

CassUuid generateUUID();

std::string toString(const CassUuid& uuid);

#endif //__CONAN_PACKAGE_EXAMPLE_FORMATTER_HPP