#include <cassert>
#include <iostream>
#include <ostream>
#include "leveldb/db.h"

int main() {
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    if (status.ok()) {
      std::cout << "open successfully" << std::endl;
    } else {
      std::cout << status.ToString() << std::endl;
    }

    delete db;

    return 0;
}