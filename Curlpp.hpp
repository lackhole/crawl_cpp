//
// Created by YongGyu Lee on 2020/12/23.
//

#ifndef CRAWL__CURLPP_HPP_
#define CRAWL__CURLPP_HPP_

#include <cstdint>

class Curlpp {
 public:
  class Impl;

  Curlpp();
  ~Curlpp();

  bool init();

  bool request(const char* url);

  void get(char* buffer, std::uint_fast32_t max_size);

  std::uint_fast32_t bufferSize() const;

 private:
  Impl* pImpl;
};

#endif //CRAWL__CURLPP_HPP_
