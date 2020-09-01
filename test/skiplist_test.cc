#include <cstdlib>
#include <fstream>
#include <functional>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <sys/time.h>

#include "skiplist.h"

using namespace std;
using namespace minidb;

TEST(Skiplist, insertInt) {
  google::InitGoogleLogging("SkipList_test");
  auto sk = SkipList<int>([](const int& a, const int& b) { return a - b; });
  int i = 3;
  sk.Insert(i);
  ASSERT_FALSE(sk.Contains(11));
  sk.Insert(8);
  sk.Insert(2);
  sk.Insert(11);
  ASSERT_TRUE(sk.Contains(3));
  ASSERT_TRUE(sk.Contains(8));
  ASSERT_TRUE(sk.Contains(2));
  ASSERT_TRUE(sk.Contains(11));
}

TEST(Skiplist, insertString) {
  auto sk = SkipList<string>([](const string& a, const string& b) {
    return int(hash<string>{}(a)-hash<string>{}(b));
  });
  ASSERT_FALSE(sk.Contains("cat"));
  sk.Insert("cat");
  sk.Insert("bee");
  sk.Insert("Ascii");
  sk.Insert("Z");
  ASSERT_TRUE(sk.Contains("cat"));
  ASSERT_TRUE(sk.Contains("bee"));
  ASSERT_TRUE(sk.Contains("Z"));
  ASSERT_TRUE(sk.Contains("Ascii"));
}

// TEST(Skiplist, benchmark) {
//   float time_use = 0;
//   struct timeval start;
//   struct timeval end;

//   auto sk = SkipList<int>([](const int& a, const int& b) { return a - b; });
//   srand(0x12334);
//   ofstream file("./result.csv");
//   if (file) {
//     for (long i = 0; i < 100000; i++) {
//       gettimeofday(&start, NULL); // gettimeofday(&start,
//       sk.Insert(rand());
//       gettimeofday(&end, NULL);
//       time_use = (end.tv_sec - start.tv_sec) * 1000000 +
//                  (end.tv_usec - start.tv_usec); //微秒
//       file << i << ", " << time_use << endl;
//     }
//   }
//   file.close();
// }