#pragma once
#include <memory>
#include <optional>
#include <string>

namespace minidb {
// using DBStruct = struct {};
enum class GetRet { Success, NoRecord };
enum class SetRet { Success, NoRecord };

class db_interface {
 public:
  virtual ~db_interface() {};
  virtual std::pair<GetRet, std::string> Get(const std::string& key) = 0;
  virtual SetRet Set(const std::string& key, const std::string& value) = 0;
};
std::shared_ptr<db_interface> open(const std::string& path);
}  // namespace minidb