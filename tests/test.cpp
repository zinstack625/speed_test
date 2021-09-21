// Copyright 2021 Klimanov Anton <klimanov625@outlook.com>

#include <gtest/gtest.h>
#include <vector>
#include <thread>
#include <mutex>
#include "header.hpp"

TEST(SharedPtr, Construct) {
    auto shared_ptr = SharedPtr<int>();
    *shared_ptr = 0;
    EXPECT_EQ(*shared_ptr, 0);
    for (size_t i = 0; i < 1000; ++i) {
      *shared_ptr = i;
      EXPECT_EQ(*shared_ptr, i);
    }
    shared_ptr.reset();
}

TEST(SharedPtr, Copy_Move) {
    SharedPtr<int> shared_ptr(new int);
    *shared_ptr = 317289;
    EXPECT_EQ(*shared_ptr, 317289);

    auto copy(shared_ptr);
    auto copy2 = shared_ptr;
    auto moved = std::move(shared_ptr);

    EXPECT_EQ(shared_ptr.get(), nullptr);
    ASSERT_EQ(copy, moved);
    ASSERT_EQ(copy2, moved);
    EXPECT_EQ(moved.use_count(), 3);
    EXPECT_EQ(copy.use_count(), 3);
    EXPECT_EQ(copy2.use_count(), 3);
    copy.reset();
    EXPECT_FALSE(bool(copy));
    EXPECT_EQ(moved.use_count(), 2);
    EXPECT_EQ(copy2.use_count(), 2);
}

TEST(SharedPtr, Swap) {
  SharedPtr<double> somevalue(new double(2.0));
  SharedPtr<double> othervalue(new double(1.0));
  EXPECT_EQ(*somevalue, 2.0);
  EXPECT_EQ(*othervalue, 1.0);
  somevalue.swap(othervalue);
  EXPECT_EQ(*somevalue, 1.0);
  EXPECT_EQ(*othervalue, 2.0);
}

std::mutex mut;
void inc_counter(SharedPtr<size_t> counter) {
  mut.lock();
  ++(*counter);
  mut.unlock();
}

TEST(SharedPtr, Threads) {
  SharedPtr<size_t> counter(new size_t(0));
  std::vector<std::thread> threads;
  threads.reserve(1000);
  for (size_t i = 0; i < 1000; ++i) {
     threads.push_back(std::thread(inc_counter, counter));
  }
  for (size_t i = 0; i < 1000; ++i) {
    threads.at(i).join();
  }
  EXPECT_EQ(*counter, 1000);
}
