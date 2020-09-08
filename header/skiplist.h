#pragma once

#include "random.h"

#include <glog/logging.h>
#include <memory>
namespace minidb {
template <class DataType> class SkipList {
public:
  static constexpr int MAX_HEIGHT = 12;
  using Comparator = int (*)(const DataType&, const DataType&);
  struct SkipBar {
    DataType content;
    SkipBar* pieces[MAX_HEIGHT];
    unsigned int pieces_num;
    SkipBar* Next(int level) {
      if (level >= pieces_num || level < 0) {
        LOG(ERROR) << "Invaid";
        return nullptr;
      }
      return pieces[level];
    }
    void setNext(int level, SkipBar* node) {
      pieces[level] = node;
    }
  };

  Comparator comp_;
  leveldb::Random rnd_;
  SkipBar* head_;
  unsigned int height_;

  SkipList(const Comparator& comp);

  ~SkipList();

  void Insert(const DataType& content);

  bool Contains(const DataType& content);

private:
  SkipBar* FindInsertionPoint(const DataType& content, SkipBar** prev = nullptr);

  int RandomHeight();
};

template <class DataType>
SkipList<DataType>::SkipList(const Comparator& comp)
    : rnd_(0xfeedbac1), comp_(comp), head_(new SkipBar()), height_(1) {
  head_->pieces_num = MAX_HEIGHT;
  for (int i = 0; i < MAX_HEIGHT; i++)
    head_->pieces[i] = nullptr;
}

template <class DataType> int SkipList<DataType>::RandomHeight() {
  static const unsigned int kBranching = 4;
  int height = 1;
  while (height < MAX_HEIGHT && ((rnd_.Next() % kBranching) == 0))
    height++;
  if (height <= 0 || height > MAX_HEIGHT)
    LOG(ERROR) << "Error Height: " << height;
  return height;
}

template <class DataType> SkipList<DataType>::~SkipList() {
  // TODO
}

template <class DataType> void SkipList<DataType>::Insert(const DataType& content) {
  SkipBar* prev[MAX_HEIGHT];
  SkipBar* insert_point = FindInsertionPoint(content, prev);
  int height = RandomHeight();
  // height_ = max(height, height_);
  if (height > height_) {
    for (int i = height_; i < height; i++) {
      prev[i] = head_;
    }
  }

  SkipBar* newBar = new SkipBar();
  newBar->content = content;
  newBar->pieces_num = height;
  for (int i = 0; i < height; i++) {
    newBar->setNext(i, prev[i]->Next(i));
    prev[i]->setNext(i, newBar);
  }
}

template <class DataType>
typename SkipList<DataType>::SkipBar*
SkipList<DataType>::FindInsertionPoint(const DataType& content, SkipBar* prev[]) {
  SkipBar* node = head_;
  unsigned int level = height_ - 1;
  while (true) {
    SkipBar* next = node->Next(level);
    if ((next != nullptr) && comp_(next->content, content) < 0)
      node = next;
    else {
      if (prev != nullptr)
        prev[level] = node;
      if (level > 0)
        level--;
      else
        return next;
    }
  }
}

template <class DataType> bool SkipList<DataType>::Contains(const DataType& content) {
  SkipBar* result = FindInsertionPoint(content);
  return result != nullptr && comp_(result->content, content) == 0;
}
} // namespace minidb