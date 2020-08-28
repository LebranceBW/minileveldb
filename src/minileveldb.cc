#include "minileveldb.h"

#include <map>
using std::make_pair;
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;

namespace minidb
{
  class db_impl : public db_interface
  {
    map<string, string> m;
    std::pair<GetRet, std::string> Get(const std::string &key) override
    {
      auto result = m.find(key);
      if (result == m.end())
      {
        return make_pair(GetRet::NoRecord, "");
      }
      return make_pair(GetRet::Success, result->second);
    }

    SetRet Set(const std::string &key, const std::string &value) override
    {
      m[key] = value;
      return SetRet::Success;
    }
  };

  shared_ptr<db_interface> open(const std::string &path)
  {
    return make_shared<db_impl>();
  }
} // namespace minidb
