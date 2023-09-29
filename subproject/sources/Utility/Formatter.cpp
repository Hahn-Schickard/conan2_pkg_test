#include "Formatter.hpp"

CassUuid generateUUID() {
  CassUuid uuid;
  cass_uuid_from_string("550e8400-e29b-41d4-a716-446655440000", &uuid);
  return uuid;
}

std::string toString(const CassUuid& uuid) {
  char uuid_str[CASS_UUID_STRING_LENGTH];
  cass_uuid_string(uuid, uuid_str);
  return std::string(uuid_str);
}