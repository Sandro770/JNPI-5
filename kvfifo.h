#ifndef KVFIFO_H
#define KVFIFO_H

#include <concepts>
#include <deque>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <stdexcept>
#include <utility>

template <typename K, typename V> class kvfifo {
private:
  using queue_t = std::list<std::pair<const K *, V>>;
  using it_t = typename queue_t::iterator;
  using deque_it_t = std::deque<it_t>;
  using k_to_iterators_t = std::map<K, deque_it_t>;
public:
  class k_iterator {
  public:
    using difference_type =
        typename k_to_iterators_t::iterator::difference_type;
    using value_type = K;

    k_iterator() = default;

    k_iterator(typename k_to_iterators_t::iterator it) : it(it) {}

    k_iterator &operator++() {
      ++it;
      return *this;
    }

    k_iterator operator++(int) {
      k_iterator tmp(*this);
      operator++();
      return tmp;
    }

    k_iterator &operator--() {
      --it;
      return *this;
    }

    k_iterator operator--(int) {
      k_iterator tmp(*this);
      operator--();
      return tmp;
    }

    bool operator==(k_iterator const &other) const { return it == other.it; }

    bool operator!=(k_iterator const &other) const { return it != other.it; }

    K const &operator*() const { return it->first; }

  private:
    typename k_to_iterators_t::iterator it;
  };

  kvfifo() : data(std::make_shared<data_t>()) {}

  kvfifo(kvfifo const &other)
      : data(other.data->given_reference
                 ? std::make_shared<data_t>(*(other.data))
                 : other.data) {
    if (other.data->given_reference) {
      data->given_reference = false;
    }
  }

  kvfifo(kvfifo &&other) noexcept : data(std::move(other.data)) {
    other.data = empty_data;
  }

  kvfifo &operator=(kvfifo other) noexcept {
    data = std::move(other.data);
    return *this;
  }

  void push(K const &k, V const &v) {
    Guard g(data, true);
    g.data->push(k, v);
    g.accept();
  }

  void pop() {
    Guard g(data, true);
    g.data->pop();
    g.accept();
  }

  void pop(K const &k) {
    Guard g(data, true);
    g.data->pop(k);
    g.accept();
  }

  void move_to_back(K const &k) {
    Guard g(data, true);
    g.data->move_to_back(k);
    g.accept();
  }
  std::pair<K const &, V const &> front() const {
    if (data->queue.empty()) {
      throw std::invalid_argument("queue is empty");
    }

    auto &entry = data->queue.front();

    return std::pair<K const &, V const &>(*(entry.first), entry.second);
  }

  std::pair<K const &, V &> front() {
    if (data->queue.empty()) {
      throw std::invalid_argument("queue is empty");
    }

    Guard g(data, false);
    auto &entry = g.data->queue.front();
    g.data->given_reference = true;
    g.accept();

    return std::pair<K const &, V &>(*(entry.first), entry.second);
  }

  std::pair<K const &, V &> back() {
    if (data->queue.empty()) {
      throw std::invalid_argument("queue is empty");
    }

    Guard g(data, false);
    auto &entry = g.data->queue.back();
    g.data->given_reference = true;
    g.accept();

    return std::pair<K const &, V &>(*(entry.first), entry.second);
  }

  std::pair<K const &, V const &> back() const {
    if (data->queue.empty()) {
      throw std::invalid_argument("queue is empty");
    }

    auto &entry = data->queue.back();

    return std::pair<K const &, V const &>(*(entry.first), entry.second);
  }

  std::pair<K const &, V &> first(K const &key) {
    Guard g(data, false);

    auto queue_it = g.data->k_to_iterators.find(key);

    if (queue_it == g.data->k_to_iterators.end()) {
      throw std::invalid_argument("key not found");
    }

    K const &k = queue_it->first;
    V &v = (*(queue_it->second).begin())->second;

    g.data->given_reference = true;
    g.accept();

    return std::pair<K const &, V &>(k, v);
  }

  std::pair<K const &, V const &> first(K const &key) const {
    auto queue_it = data->k_to_iterators.find(key);

    if (queue_it == data->k_to_iterators.end()) {
      throw std::invalid_argument("key not found");
    }

    K const &k = queue_it->first;
    V const &v = (*(queue_it->second).begin())->second;

    return std::pair<K const &, V const &>(k, v);
  }

  std::pair<K const &, V &> last(K const &key) {
    Guard g(data, false);
    auto queue_it = g.data->k_to_iterators.find(key);

    if (queue_it == g.data->k_to_iterators.end()) {
      throw std::invalid_argument("key not found");
    }

    K const &k = queue_it->first;
    V &v = (queue_it->second).back()->second;

    g.data->given_reference = true;
    g.accept();

    return std::pair<K const &, V &>(k, v);
  }

  std::pair<K const &, V const &> last(K const &key) const {
    auto queue_it = data->k_to_iterators.find(key);

    if (queue_it == data->k_to_iterators.end()) {
      throw std::invalid_argument("key not found");
    }

    K const &k = queue_it->first;
    V &v = (queue_it->second).back()->second;

    return std::pair<K const &, V &>(k, v);
  }

  size_t size() const noexcept {
    return data.use_count() > 0 ? data->queue.size() : 0;
  }

  bool empty() const noexcept { return size() == 0; }

  size_t count(K const &k) const {
    auto queue_it = data->k_to_iterators.find(k);

    if (queue_it == data->k_to_iterators.end()) {
      return 0;
    }

    return queue_it->second.size();
  }

  void clear() { data = std::make_shared<data_t>(); }

  k_iterator k_begin() const noexcept {
    return k_iterator(data->k_to_iterators.begin());
  }

  k_iterator k_end() const noexcept {
    return k_iterator(data->k_to_iterators.end());
  }

private:
  static_assert(std::bidirectional_iterator<k_iterator>);

  struct data_t {
    queue_t queue;
    k_to_iterators_t k_to_iterators;
    bool given_reference = false;

    void push(K const &k, V const &v) {
      queue.push_back(std::pair<K *, V>(NULL, v));

      it_t list_it = std::prev(queue.end());

      try {
        typename k_to_iterators_t::iterator map_it = k_to_iterators.find(k);

        if (map_it == k_to_iterators.end()) {
          auto new_element =
              typename std::pair<K, std::deque<typename queue_t::iterator>>(
                  k, std::deque<typename queue_t::iterator>());
          new_element.second.push_back(list_it);
          list_it->first = &(k_to_iterators.insert(new_element).first->first);
        } else {
          map_it->second.push_back(list_it);
          list_it->first = &(map_it->first);
        }
      } catch (const std::exception &e) {
        queue.erase(list_it);
        throw e;
      }
    }

    void pop() {
      if (queue.empty()) {
        throw std::invalid_argument("queue is empty");
      }

      typename k_to_iterators_t::iterator map_it =
          k_to_iterators.find(*(queue.front().first));

      if (map_it == k_to_iterators.end()) {
        throw std::invalid_argument("problem with finding key in map");
      }

      it_t list_it = map_it->second.front();

      if (map_it->second.size() == 1) {
        k_to_iterators.erase(map_it);
      } else {
        map_it->second.erase(map_it->second.begin());
      }

      queue.erase(list_it);
    }

    void pop(K const &k) {
      typename k_to_iterators_t::iterator map_it = k_to_iterators.find(k);

      if (map_it == k_to_iterators.end()) {
        throw std::invalid_argument("this key doesn't exist");
      }

      it_t list_it = map_it->second.front();

      if (map_it->second.size() == 1) {
        k_to_iterators.erase(map_it);
      } else {
        map_it->second.erase(map_it->second.begin());
      }

      queue.erase(list_it);
    }

    void move_to_back(K const &k) {
      typename k_to_iterators_t::iterator map_it = k_to_iterators.find(k);

      if (map_it == k_to_iterators.end()) {
        throw std::invalid_argument("this key doesn't exist");
      }

      for (auto const &it_in_list : map_it->second) {
        queue.splice(queue.end(), queue, it_in_list);
      }
    }

    data_t() = default;

    data_t(data_t const &other) : given_reference(false) {
      for (auto const &it : other.queue) {
        this->push(*(it.first), it.second);
      }
    }
  };

  struct Guard {
    std::shared_ptr<data_t> data, &orig_data;
    bool modifies;

    Guard(std::shared_ptr<data_t> &data_ptr, bool modifies)
        : orig_data(data_ptr), modifies(modifies) {
      if (data_ptr.use_count() > 1) {
        data = std::make_shared<data_t>(*data_ptr);
      } else {
        data = data_ptr;
      }
    }

    void accept() {
      swap(data, orig_data);
      if (modifies) {
        data->given_reference = false;
      }
    }
  };
  std::shared_ptr<data_t> data, empty_data = std::make_shared<data_t>();
};

#endif
