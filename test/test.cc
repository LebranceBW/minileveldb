#include <gtest/gtest.h>

#include <iostream>

#include "minileveldb.h"

using namespace std;
using namespace minidb;

TEST(Basic, 1) {
  auto db = open("123");
  ASSERT_TRUE(db);
  db->Set("123", "456");
  ASSERT_EQ(db->Get("123").second, "456");
  switch(db->Get("123").first) {
    case GetRet::Success:
      break;
    }
}
