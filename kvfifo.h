#ifndef KVFIFO_H
#define KVFIFO_H

#include <concepts>

template <typename K, typename V> class kvfifo{
  struct k_iterator requires std::bidirectional_iterator<kvfifo<K,V>> {
  
  };
  kvfifo();
  kvfifo(kvfifo const &);
  kvfifo(kvfifo &&);
  kvfifo& operator=(kvfifo other);
  void push(K const &k, V const &v);
  void pop();
  void pop(K const &);
  void move_to_back(K const &k);
  std::pair<K const &, V &> front();
  std::pair<K const &, V const &> front() const;
  std::pair<K const &, V &> back();
  std::pair<K const &, V const &> back() const;
  std::pair<K const &, V &> first(K const &key);
  std::pair<K const &, V const &> first(K const &key) const;
  std::pair<K const &, V &> last(K const &key);
  std::pair<K const &, V const &> last(K const &key) const;
  size_t size() const;
  bool empty() const;
  size_t count(K const &) const;
  void clear();
  k_iterator k_begin();
  k_iterator k_end();
};



#endif