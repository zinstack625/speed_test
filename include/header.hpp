// Copyright 2021 Klimanov Anton <klimanov625@outlook.com>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <atomic>
#include <cstddef>
#include <utility>

template <class T>
class SharedPtr {
 public:
  SharedPtr() : data(new T()), ref_cnt(new std::atomic<size_t>(1)) {}
  explicit SharedPtr(T* ptr) : data(ptr), ref_cnt(new std::atomic<size_t>(1)) {}
  SharedPtr(const SharedPtr& r) : data(r.data), ref_cnt(r.ref_cnt) {
    ++(*r.ref_cnt);
  }
  SharedPtr(SharedPtr&& r) {
    data = r.data;
    ref_cnt = r.ref_cnt;
    r.data = nullptr;
    r.ref_cnt = new std::atomic<size_t>(1);
  }
  ~SharedPtr() {
    if (--(*ref_cnt) == 0) {
      delete data;
      delete ref_cnt;
    }
    data = nullptr;
    ref_cnt = nullptr;
  }
  auto operator=(const SharedPtr& r) -> SharedPtr& {
    data = r.data;
    ref_cnt = r.ref_cnt;
    ++(*ref_cnt);
    return *this;
  }
  auto operator=(SharedPtr&& r) -> SharedPtr& {
    data = std::move(r.data);
    *ref_cnt = *r.ref_cnt;
    return *this;
  }

  // проверяет, указывает ли указатель на объект
  operator bool() const { return data; }
  auto operator*() const -> T& { return *data; }
  auto operator->() const -> T* { return data; }

  auto get() -> T* { return data; }
  void reset() {
    if (--(*ref_cnt) == 0) {
      delete data;
      delete ref_cnt;
    }
    data = nullptr;
    ref_cnt = new std::atomic<size_t>(1);
  }
  void reset(T* ptr) { data = ptr; }
  void swap(SharedPtr& r) {
    std::swap(data, r.data);
    std::swap(ref_cnt, r.ref_cnt);
  }
  // возвращает количество объектов SharedPtr, которые ссылаются на тот же
  // управляемый объект
  auto use_count() const -> size_t { return *ref_cnt; }

 private:
  T* data;
  std::atomic<size_t>* ref_cnt;
};

#endif  // INCLUDE_HEADER_HPP_
