// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_ARENA_H_
#define STORAGE_LEVELDB_UTIL_ARENA_H_

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace leveldb {

class Arena {
 public:
  Arena();

  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;

  // Arena 隶属于 Memtable，当 MemTable 结束生命周期时
  // 析构函数会释放内存池
  ~Arena();

  // Return a pointer to a newly allocated memory block of "bytes" bytes.
  char* Allocate(size_t bytes);

  // Allocate memory with the normal alignment guarantees provided by malloc.
  // 分配偶数大小的内存，主要是 skiplist 节点时，目的是加快访问
  char* AllocateAligned(size_t bytes);

  // Returns an estimate of the total memory usage of data allocated
  // by the arena.
  size_t MemoryUsage() const {
    return memory_usage_.load(std::memory_order_relaxed);
  }

 private:
  // 按需分配内存，可能会有内存浪费
  char* AllocateFallback(size_t bytes);
  char* AllocateNewBlock(size_t block_bytes);

  // Allocation state
  // 当前已使用内存的指针
  char* alloc_ptr_;
  // 剩余内存的字节数
  size_t alloc_bytes_remaining_;

  // Array of new[] allocated memory blocks
  // 实际分配的内存池
  std::vector<char*> blocks_;

  // Total memory usage of the arena.
  //
  // TODO(costan): This member is accessed via atomics, but the others are
  //               accessed without any locking. Is this OK?
  // 记录内存的使用情况
  std::atomic<size_t> memory_usage_;
};

inline char* Arena::Allocate(size_t bytes) {
  // The semantics of what to return are a bit messy if we allow
  // 0-byte allocations, so we disallow them here (we don't need
  // them for our internal use).
  // 这种习惯不好，assert 是在运行期，若命中该断言会直接 kill 掉
  // 建议使用 static_assert
  assert(bytes > 0);
  // 优先判断当前容量是否够
  if (bytes <= alloc_bytes_remaining_) {
    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
  }
  return AllocateFallback(bytes);
}

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_ARENA_H_
