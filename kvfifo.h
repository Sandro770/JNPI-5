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
    (*queue).push_back(std::make_pair(k, v));
    queue_t::iterator list_it = (*queue).end();
    list_it--;
    k_to_iterators_t::iterator map_it = (*k_to_iterators).find(k);

    if (map_it == (*k_to_iterators).end()) { //probably should dynamically allocate those and dealocate during pop
      std::deque<queue_t::iterator> new_element;
      new_element.push_back(list_it);
      (*k_to_iterators).insert(new_element);
    }
    else {
      (*map_it).push_back(list_it);
    }
  }

  void pop() {
    modify();
    if ((*queue).empty()) {
      throw std::invalid_argument();
    }

    queue_t::iterator list_it = (*queue).begin();
    k_to_iterators_t::iterator map_it = (*k_to_iterators).find((*list_it).first);
    //can add assertion/ error if map_it == end()
    (*map_it).erase((*map_it).begin());
    if ((*map_it).size() == 0) { //dealocate deque on map_it (?)
      (*k_to_iterators).erase(map_it);
    }
  }

  void pop(K const &) {
    modify();
    // remove from map
    // remove from queue
  }

  void move_to_back(K const &k) {
    modify();
    k_to_iterators_t::iterator map_it = (*k_to_iterators).find((*list_it).first);

    if (map_it == (*k_to_iterators).end()) {
      throw std::invalid_argument();
    }

    for (std::deque<queue_t::iterator>::iterator queue_it = (*map_it).begin();
        queue_it < (*map_it).end(); queue_it++) { //should work
      std::pair<K,V> moved = *queue_it;
      (*queue).erase(queue_it);
      (*queue).push_back(moved);
      queue_t::iterator list_it = (*queue).end();
      list_it--;
      *queue_it = list_it;
    }
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
  using k_to_iterators_t = std::map<K, std::deque<queue_t::iterator>>;

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
