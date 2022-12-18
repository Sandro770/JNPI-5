#ifndef KVFIFO_H
#define KVFIFO_H

#include <concepts>
#include <memory>
#include <map>
#include <queue>
#include <deque>
#include <utility>
#include <list>
#include <functional>
#include <stdexcept>

using namespace std;

template <typename K, typename V> 
class kvfifo{
public:
  using queue_t = std::list<std::pair<K&, V>>;
  using k_iterator = queue_t::const_iterator;
  using it_t = queue_t::iterator;
  using deque_it_t = std::deque<it_t>;
  using k_to_iterators_t = std::map<K, deque_it_t>;
  
  kvfifo() : data(std::make_shared<data_t>()) { }

  kvfifo(kvfifo const &other) : data(other.data) { }

  kvfifo(kvfifo &&other) noexcept : data(std::move(other.data))  { }

  kvfifo& operator=(kvfifo other) noexcept {
    data = std::move(other.data);
    return *this;
  }

  void push(K const &k, V const &v) {
    try {
      size_t size = data->queue.size();
      data->queue.push_back(pair<K, V>(k, v));
      if (size == data->queue.size()) {
        throw std::exception("addition to list failed"); //think which exception to use
      }
    }
    catch (const std::exception& e) {
      throw e;
    }

    it_t list_it = std::prev(data->queue.end()); // throws only when container is empty which we have a guarantee it isn't i think
    std::deque<queue_t::iterator>* new_element = NULL;
    try {
      k_to_iterators_t::iterator map_it = data->k_to_iterators.find(k);

      if (map_it == data->k_to_iterators.end()) {
        new_element = new std::deque<queue_t::iterator>();
        if (new_element == NULL) {
          throw std::exception("creation of new element of map failed"); //think if needed
        }
        (*new_element).push_back(list_it);
        if ((*new_element).size() != 1) {
          throw std::exception("addition to map element failed");
        }
        data->k_to_iterators.insert(*new_element);
        delete(new_element);
      }
      else {
        size_t size = (*map_it).size();
        (*map_it).push_back(list_it);
        if (size != (*map_it).size()) {
          throw std::exception("addition to map element failed");
        }
      }
    }
    catch (const std::exception& e) {
      if (new_element != NULL) {
        delete(new_element);
      }
      data->queue.erase(list_it);
      throw e;
    }
  }

  void pop() {
    if (data->queue.empty()) {
      throw std::invalid_argument("queue is empty");
    }
    try {
      it_t list_it = data->k_to_iterators[data->queue.front()].front();
      if (data->k_to_iterators[data->queue.front()].size() == 1) {
        data->k_to_iterators.erase(data->queue.front());
      }
      else {
        data->k_to_iterators[data->queue.front()].erase(data->k_to_iterators[data->queue.front()].begin());
      }
      data->queue.erase(list_it);

    }
    catch (const std::exception& e) {
      throw e;
    }
  }

  void pop(K const &k) {
    try {
      k_to_iterators_t::iterator map_it = data->k_to_iterators.find(k);
      if (map_it == data->k_to_iterators.end()) {
        throw std::invalid_argument();
      }
      it_t list_it = (*map_it).front();
      if ((*map_it).size() == 1) {
        data->k_to_iterators.erase(map_it);
      }
      else {
        (*map_it).erase((*map_it).begin());
      }
      data->queue.erase(list_it); // no exception
    }
    catch (const std::exception& e) {
      throw e;
    }
  }

  void move_to_back(K const &k) {
    // modify();
    // k_to_iterators_t::iterator map_it = (*k_to_iterators).find((*list_it).first);

    // if (map_it == (*k_to_iterators).end()) {
    //   throw std::invalid_argument("key not found");
    // }

    // for (std::deque<queue_t::iterator>::iterator queue_it = (*map_it).begin();
    //     queue_it < (*map_it).end(); queue_it++) { //should work
    //   std::pair<K,V> moved = *queue_it;
    //   (*queue).erase(queue_it);
    //   (*queue).push_back(moved);
    //   queue_t::iterator list_it = (*queue).end();
    //   list_it--;
    //   *queue_it = list_it;
    // }
  }

  std::pair<K const &, V &> front() {
    if (data->queue.empty()) {
      throw std::invalid_argument("queue is empty");
    }
    return data->queue.front();
  }

  std::pair<K const &, V const &> front() const {
    if (data->queue.empty()) {
      throw std::invalid_argument("queue is empty");
    }
    return data->queue.front();
  }

  std::pair<K const &, V &> back() {
    if (data->queue.empty()) {
      throw std::invalid_argument("queue is empty");
    }
    return data->queue.back();
  }

  std::pair<K const &, V const &> back() const {
    if (data->queue.empty()) {
      throw std::invalid_argument("queue is empty");
    }
    return data->queue.back();
  }
  
  std::pair<K const &, V &> first(K const &key) {
    auto queue_it = data->k_to_iterators->find(key);
    if (queue_it == data->k_to_iterators->end()) {
      throw std::invalid_argument("key not found");
    }
    return queue_it->front();
  }

  std::pair<K const &, V const &> first(K const &key) const {
    auto queue_it = data->k_to_iterators->find(key);
    if (queue_it == data->k_to_iterators->end()) {
      throw std::invalid_argument("key not found");
    }
    return queue_it->front();
  }
  std::pair<K const &, V &> last(K const &key) {
    auto queue_it = data->k_to_iterators->find(key);
    if (queue_it == data->k_to_iterators->end()) {
      throw std::invalid_argument("key not found");
    }
    return queue_it->back();
  }

  std::pair<K const &, V const &> last(K const &key) const {
    auto queue_it = data->k_to_iterators->find(key);
    if (queue_it == data->k_to_iterators->end()) {
      throw std::invalid_argument("key not found");
    }
    return queue_it->back();
  }
  
  size_t size() const noexcept {
    return data->queue.size();
  }
  
  bool empty() const noexcept {
    return size() == 0;
  }
  
  size_t count(K const &k) const {
    auto queue_it = data->k_to_iterators->find(k);
    if (queue_it == data->k_to_iterators->end()) {
      return 0;
    }
    return queue_it->size();
  }

  void clear() {
    data = std::make_shared<data_t>();
  }

  k_iterator k_begin() const noexcept {
    return data->queue->begin();
  } 

  k_iterator k_end() const noexcept {
    return data->queue->end();
  }
private:
  
  
  static_assert(std::bidirectional_iterator<k_iterator>);

  struct data_t {
    queue_t queue;
    k_to_iterators_t k_to_iterators;
  };

  std::shared_ptr<data_t> data;

  void modify() {
    // if (queue.use_count() == 0) {
    //   queue = std::make_shared<queue_t>();
    // }
    // if (k_to_iterators.use_count() == 0) {
    //   k_to_iterators = std::make_shared<k_to_iterators_t>();
    // }
    // if (queue.use_count() > 1 || k_to_iterators.use_count() > 1) { /// create a copy
    //   queue = std::make_shared<queue_t>(queue_t(*queue));
    //   k_to_iterators =
    //       std::make_shared<k_to_iterators_t>(k_to_iterators_t(*k_to_iterators));
    // }
  }
};

#endif
