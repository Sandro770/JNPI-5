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
  using it_t = queue_t::iterator;
  using deque_it_t = std::deque<it_t>;
  using k_to_iterators_t = std::map<K, deque_it_t>;
  
  // Create wrapper iterator for k_to_iterators_t iterator
  

  class k_iterator {
  public:


    k_iterator(k_to_iterators_t::iterator it) : it(it) { }
    k_iterator& operator++() {
      ++it;
      return *this;
    }

    k_iterator operator++(int) {
      k_iterator tmp(*this);
      operator++();
      return tmp;
    }

    k_iterator& operator--() {
      --it;
      return *this;
    }

    k_iterator operator--(int) {
      k_iterator tmp(*this);
      operator--();
      return tmp;
    }
    
    bool operator==(k_iterator const &other) const {
      return it == other.it;
    }
    
    bool operator!=(k_iterator const &other) const {
      return it != other.it;
    }
    
    K const & operator*() const {
      return it->first;
    }
  private:
    k_to_iterators_t::iterator it;
  };

  kvfifo() : data(std::make_shared<data_t>()) { }

  kvfifo(kvfifo const &other) : data(other.data->given_reference ? std::make_shared<data_t> (*(other.data)) : other.data) { data->given_reference = false; }

  kvfifo(kvfifo &&other) noexcept : data(std::move(other.data))  { }

  kvfifo& operator=(kvfifo other) noexcept {
    data = std::move(other.data);
    return *this;
  }

  void push(K const &k, V const &v) {
    // try {
    //   size_t size = data->queue.size();
    //   data->queue.push_back(pair<K, V>(k, v));
    //   if (size == data->queue.size()) {
    //     throw std::exception("addition to list failed"); //think which exception to use
    //   }
    // }
    // catch (const std::exception& e) {
    //   throw e;
    // }

    // it_t list_it = std::prev(data->queue.end()); // throws only when container is empty which we have a guarantee it isn't i think
    
    // try {
    //   k_to_iterators_t::iterator map_it = data->k_to_iterators.find(k);

    //   if (map_it == data->k_to_iterators.end()) {
    //     std::deque<queue_t::iterator> new_element;
    //     if (new_element == NULL) {
    //       throw std::exception("creation of new element of map failed"); //think if needed
    //     }
    //     (*new_element).push_back(list_it);
    //     if ((*new_element).size() != 1) {
    //       throw std::exception("addition to map element failed");
    //     }
    //     data->k_to_iterators.insert(*new_element);
    //   }
    //   else {
    //     size_t size = (*map_it).size();
    //     (*map_it).push_back(list_it);
    //     if (size != (*map_it).size()) {
    //       throw std::exception("addition to map element failed");
    //     }
    //   }
    // }
    // catch (const std::exception& e) {
    //   if (new_element != NULL) {
    //   }
    //   data->queue.erase(list_it);
    //   throw e;
    // }
    // modify_and_rollback();
  }

  void pop() {
    // if (data->queue.empty()) {
    //   throw std::invalid_argument("queue is empty");
    // }
    // try {
    //   k_to_iterators_t::iterator map_it = data->k_to_iterators.find(data->queue.front());
    //   modify()
    //   if (map_it == data->k_to_iterators.end()) {
    //     throw std::invalid_argument();
    //   }
    //   it_t list_it = (*map_it).front();
    //   if ((*map_it).size() == 1) {
    //     data->k_to_iterators.erase(map_it);
    //   }
    //   else {
    //     (*map_it).erase((*map_it).begin());
    //   }
    //   data->queue.erase(list_it);

    // }
    // catch (const std::exception& e) {
    //   throw e;
    // }
  }

  void pop(K const &k) {
    // try {
    //   k_to_iterators_t::iterator map_it = data->k_to_iterators.find(k);
    //   if (map_it == data->k_to_iterators.end()) {
    //     throw std::invalid_argument();
    //   }
    //   modify();
    //   it_t list_it = (*map_it).front();
    //   if ((*map_it).size() == 1) {
    //     data->k_to_iterators.erase(map_it);
    //   }
    //   else {
    //     (*map_it).erase((*map_it).begin());
    //   }
    //   data->queue.erase(list_it); // no exception
    // }
    // catch (const std::exception& e) {
    //   throw e;
    // }
  }

  void move_to_back(K const &k) {
    // try {
    //   k_to_iterators_t::iterator map_it = (*k_to_iterators).find(k);
    // }
    // catch (const std::exception& e) {
    //   throw e;
    // }
    // size_t size = (*map_it).size();
    // size_t moved = 0;
    // deque_it_t::iterator deq_it = (*map_it).begin();
    // try {
    //   std::deque<queue_t::iterator> new_element;
    //   k_iterator adding_helper = data->queue.end();
    //   std::prev(adding_helper);
    //   while (moved + 1 < size) {
    //     data->queue.push_back(*deq_it);
    //     moved++;
    //     deq_it = std::next(deq_it);
    //     adding_helper = std::next(adding_helper);
    //     new_element.push_back(adding_helper);
    //   }
    // }
    // catch (const std::exception& e) {
    //   k_iterator removing_helper = data->queue.end();
    //   std::prev(removing_helper);
    //   while (moved > 0) {
    //     data->queue.erase(removing_helper);
    //     removing_helper = std::prev(removing_helper);
    //     moved--;
    //   }
    //   throw e;
    // }
    // modify_and_goback(); 
    // std::swap(*map_it, new_element);
    // std::deque<queue_t::iterator>::iterator helper = new_element.begin();
    // for (size_t i = 0; i < new_element.size(); i++) {
    //   data->queue.erase(helper);
    //   helper = std::next(helper);
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
    Guard g(data);

    auto queue_it = g.data->k_to_iterators->find(key);
    if (queue_it == g.data->k_to_iterators->end()) {
      throw std::invalid_argument("key not found");
    }

    g.data->given_reference = true;
    g.accept();
    
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
    Guard g(data);

    auto queue_it = g.data->k_to_iterators->find(key);
    if (queue_it == g.data->k_to_iterators->end()) {
      throw std::invalid_argument("key not found");
    }

    g.data->given_reference = true;
    g.accept();

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

  
  // static_assert(std::bidirectional_iterator<k_iterator>);

  struct data_t {
    queue_t queue;
    k_to_iterators_t k_to_iterators;
    bool given_reference = false;

    void push(K const &k, V const &v) {
    
    }

    void pop() {
    
    }

    void pop(K const &k) {
    
    }

    void move_to_back(K const &k) {
      
    }
  };

  // implement k_iterator
  

  struct Guard {
    std::shared_ptr<data_t> data;
    std::shared_ptr<data_t> &orig_data;

    Guard(std::shared_ptr<data_t> &data_ptr) : orig_data(data_ptr) {
      if (data_ptr->use_count() > 1) {
        data = make_shared(*data_ptr);
      } else {
        data = data_ptr;
      }
    }

    void accept() {
      swap(data, orig_data);
    }
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
