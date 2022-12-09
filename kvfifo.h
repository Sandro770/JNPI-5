#ifndef KVFIFO_H
#define KVFIFO_H

#include <concepts>

template <typename K, typename V> class kvfifo{
public:
  struct k_iterator requires std::bidirectional_iterator<kvfifo<K, V>>;
  kvfifo() {
    queue = std::make_shared<queue_t>();
    k_to_iterators = std::make_shared<k_to_iterators_t>();
  }

  kvfifo(kvfifo const &other) {
    queue = other.queue;
    k_to_iterators = other.k_to_iterators;
  }

  kvfifo(kvfifo &&other) {
    queue = std::move(other.queue);
    k_to_iterators = std::move(other.k_to_iterators);
  }

  kvfifo& operator=(kvfifo other) { 
    queue = std::move(other.queue);
    k_to_iterators = std::move(other.k_to_iterators);
    return *this;
  }

  void push(K const &k, V const &v) {
    modify();

  }

  void pop() {
    modify();

  }

  void pop(K const &) {
    modify();

  }

  void move_to_back(K const &k) {
    modify();

  }

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
private:
  using queue_t = std::list<std::pair<K, V>>;
  using k_to_iterators_t = std::map<K, std::queue<queue_t::iterator>>;

  std::shared_ptr<queue_t> queue;
  std::shared_ptr<k_to_iterators_t> k_to_iterators;

  void modify() {
    if (queue.use_count() > 1 || k_to_iterators.use_count() > 1) { /// create a copy
      queue = std::make_shared<queue_t>(queue_t(*queue));
      k_to_iterators =
          std::make_shared<k_to_iterators_t>(k_to_iterators_t(*k_to_iterators));
    }
  }
};



#endif
