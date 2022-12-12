/**
 * @file vector.hpp
 * @author jiskim
 * @brief vector container
 * @date 2022-11-12
 *
 * @copyright Copyright (c) 2022
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <algorithm>  // swap, fill_n
#include <iostream>   // FIXME : remove
#include <memory>

#include "iterator.hpp"
#include "reverse_iterator.hpp"

namespace ft {

// SECTION : vector iterator
// Random access iterator
// T is pointer of elements
template <typename Iter>
class vector_iterator {
 private:
  Iter current_;  // pointer of elements

 public:
  typedef iterator_traits<Iter> traits_type;

  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::value_type value_type;
  typedef typename traits_type::pointer pointer;
  typedef typename traits_type::reference reference;
  typedef typename traits_type::iterator_category iterator_category;

  typedef vector_iterator self;

  vector_iterator(void) {}
  vector_iterator(const Iter& current) : current_(current) {}

  ~vector_iterator(void) {}

  reference operator*(void) const { return *current_; }
  pointer operator->(void) const { return current_; }

  self& operator++(void) {
    ++current_;
    return *this;
  }
  self operator++(int) { return vector_iterator(current_++); }

  self& operator--(void) {
    --current_;
    return *this;
  }
  self operator--(int) { return vector_iterator(current_--); }

  // NOTE : 이 경우는 iterator + n 의 경우고 n + iterator 도 가능해야 한다.
  self operator+(difference_type n) const {
    return vector_iterator(current_ + n);
  }
  self operator-(difference_type n) const {
    return vector_iterator(current_ - n);
  }

  self& operator+=(difference_type n) {
    current_ += n;
    return *this;
  }
  self& operator-=(difference_type n) {
    current_ -= n;
    return *this;
  }
  reference operator[](difference_type n) const { return current_[n]; }

  const Iter& base(void) const { return current_; }
};  // !SECTION : vector_iterator

// SECTION : arithmetic operators
template <typename T>
vector_iterator<T> operator+(
    const typename vector_iterator<T>::difference_type n,
    const vector_iterator<T>& iter) {
  return vector_iterator<T>(iter + n);
}

template <typename T, typename U>
typename vector_iterator<T>::difference_type operator-(
    const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() - rhs.base();
}
// !SECTION : arithmetic operators

// SECTION : comparison operators
template <typename T, typename U>
bool operator==(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() == rhs.base();
}

template <typename T, typename U>
bool operator!=(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() != rhs.base();
}

template <typename T, typename U>
bool operator<(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() < rhs.base();
}

template <typename T, typename U>
bool operator>(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() > rhs.base();
}

template <typename T, typename U>
bool operator<=(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() <= rhs.base();
}

template <typename T, typename U>
bool operator>=(const vector_iterator<T>& lhs, const vector_iterator<U>& rhs) {
  return lhs.base() >= rhs.base();
}
// !SECTION : comparision operators

// SECTION : vector base
template <typename T, typename Allocator = std::allocator<T> >
class vector_base {
 protected:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename allocator_type::reference reference;              // T&
  typedef typename allocator_type::const_reference const_reference;  // const T&
  typedef typename allocator_type::pointer pointer;                  // T*
  typedef typename allocator_type::const_pointer const_pointer;      // const T*

  allocator_type _alloc;
  pointer _begin;
  pointer _end;
  pointer _end_cap;

  vector_base(
      const allocator_type& allocator,
      typename allocator_type::size_type n = allocator_type::size_type())
      : _alloc(allocator),
        _begin(_alloc.allocate(n)),
        _end(_begin),
        _end_cap(_begin + n) {}

  ~vector_base(void) {
    for (pointer tmp = _begin; tmp != _end; ++tmp) {
      std::cout << tmp - _begin << '\n';
      _alloc.destroy(tmp);
    }
    _alloc.deallocate(_begin, _end_cap - _begin);  // deallocate 는 capacity 로
  }
};  // !SECTION : vector_base

// SECTION : vector
template <typename T, typename Allocator = std::allocator<T> >
class vector : private vector_base<T, Allocator> {
 public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;

  typedef vector_iterator<pointer> iterator;
  typedef vector_iterator<const_pointer> const_iterator;

  // TODO: reverse iterator custom implement
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;

  // SECTION : public member functions
  // SECTION : constructor
  // STRONG
  // default
  explicit vector(const allocator_type& alloc = allocator_type())
      : base_(alloc, size_type()) {}

  // fill
  explicit vector(size_type n, const value_type& val = value_type(),
                  const allocator_type& alloc = allocator_type())
      : base_(alloc, n) {
    std::uninitialized_fill(this->_begin, this->_begin + n, val);
    this->_end += n;
  }

  // TODO : consider SFINAE
  // range
  template <typename InputIterator>
  vector(InputIterator first, InputIterator last,
         const allocator_type& alloc = allocator_type())
      : base_(alloc) {
    // push_back()
  }

  // copy
  vector(const vector& x) : base_(x._alloc, x.size()) {
    this->_end = std::uninitialized_copy(this->_begin, this->_begin + x.size(),
                                         x._begin);
  }

  // SECTION: destructor
  // NOTHROW
  /**
   * @brief Destroy the vector object
   * @complexity O(N) - N is size of vector
   * automatically call ~vector_base()
   */
  ~vector(void) {}

  // BASIC
  // allocator_traits::construct 가 element 생성에 적절한 대응을 하지 못하거나
  // value_type 이 copy_assignable 하지 못하면 UB.
  /**
   * @brief assign content
   * 새 content 할당, 현 내용 대체, size 변경
   * @complexity O(size)
   *
   * @param x 복사할 vector
   * @return vector& 현재 벡터
   */
  vector& operator=(const vector& x) {
    if (this == &x) {
      return *this;
    }
    size_type size = this->size();
    size_type x_size = x.size();
    if (this->capacity() < x_size) {
      swap(vector(x), *this);
    } else if (size <= x_size) {
      // throws if element assignment or iterator operation throws
      std::copy(x._begin, x._end, this->_begin);
      this->_end = this->_begin + x_size;
    } else {  // size >= x_size, reallocation needed.
    }
    return *this;
  }

  // SECTION : iterator
  // NOTHROW
  /**
   * @brief vector 의 첫 번째 element를 가리키는 random access 이터레이터를
   * 반환한다.
   * @complexity O(1)
   *
   * @return iterator
   */
  iterator begin(void) { return iterator(this->_begin); }
  const_iterator begin(void) const { return const_iterator(this->_begin); }

  // NOTHROW
  /**
   * @brief vector 의 past-the-end element를 가리키는 random access
   * 이터레이터를 반환한다.
   * @complexity O(1)
   *
   * @return iterator
   */
  iterator end(void) { return iterator(this->_end); }
  const_iterator end(void) const { return const_iterator(this->_end); }

  // NOTHROW
  /**
   * @brief vector의 마지막 element 를 가리키는 (reverse begin)
   * reverse random access iterator 를 반환한다.
   * @complexity O(1)
   *
   * @return reverse_iterator
   */
  reverse_iterator rbegin(void) { return reverse_iterator(this->_begin); }
  const_reverse_iterator rbegin(void) const {
    return const_reverse_iterator(this->_begin);
  }

  // NOTHROW
  /**
   * @brief 첫 element 의 바로 앞 element 를 가리키는 (reverse end) 이론적인
   * reverse random access iterator 를 반환한다.
   * @complexity O(1)
   *
   * @return reverse_iterator
   */
  reverse_iterator rend(void) { return reverse_iterator(this->_end); }
  const_reverse_iterator rend(void) const {
    return const_reverse_iterator(this->_end);
  }

  // SECTION : capacity

  // NOTHROW
  /**
   * @brief 실제로 갖고 있는 element 의 개수 리턴.
   *
   * @return size_type
   */
  size_type size(void) const { return this->_end - this->_begin; }

  // NOTHROW
  /**
   * @brief 시스템 또는 라이브러리에 따라 vector 가 가질 수 있는 element 의
   * 최대 개수
   *
   * @return size_type
   */
  size_type max_size(void) const { return this->_alloc.max_size(); }

  // NOTHROW n <= size
  // STRONG n > size and reallocation required, type of elements is copyable
  // BASIC otherwise
  /**
   * @brief n 개의 elements 를 포함하도록 컨테이너를 resize 한다.
   * n <= size 이면 처음 ~ n개의 elements 까지 잘리고 뒤에 남은 것들은
   * destroy.
   * n > size 이면 n에 도달할 때 까지 element 를 insert 한다.
   * @complexity O(N) N is number of elements inserted/erased
   *
   * @param n
   * @param val n이 사이즈보다 크고 val이 주어질 경우 val으로 초기화 한다.
   */
  void resize(size_type n, value_type val = value_type()) {
    size_type size = this->size();
    if (n > capacity()) {
      // reallocation
    }
    if (n > size) {
      // append
      _append(n, val);
    } else if (n < size) {
      _destroy_end(this->_begin + n);
    }
    // else do nothing.
  }

  // NOTHROW
  size_type capacity(void) const { return this->_end_cap - this->_begin; }

  // NOTHROW
  bool empty(void) const { return this->_begin == this->_end; }

  // STRONG n > capacity and reallocation required, type of elements is copyable
  // BASIC otherwise
  /**
   * @brief request to change capacity
   * n >= capacity 이면 재할당이 필요하다.. 아니면 아무 일도 일어나지 않는다.
   * n >= max size 이면 length_error throw.
   * 재할당이 필요할 경우 container 의 allocator를 이용한다.
   * std::allocator는 bad_alloc throw.
   * @
   *
   * @param n 벡터의 최소 capacity
   */
  void reserve(size_type n) {
    if (n > capacity()) {
      size_type new_capacity = _get_alloc_size(n);
      vector tmp(new_capacity);
      std::uninitialized_copy(this->_begin, this->_end, tmp._begin);
      tmp._end = tmp._begin + n;
      tmp._end_cap = tmp._begin + new_capacity;
      std::swap(tmp, *this);
    }
  }

  // SECTION : element access

  // NOTHROW size > n
  // otherwise UB
  /**
   * @brief
   *
   * @param n
   * @return reference
   */
  reference operator[](size_type n) { return this->_begin[n]; }
  const_reference operator[](size_type n) const { return this->_begin[n]; }

  // STRONG
  // It throws out_of_range if n is out of bounds.
  /**
   * @brief operator[] 이 범위를 체크하지 않는 반면, at 은 체크한다.
   * n >= size 이면 throw out_of_range
   * @complexity O(1)
   *
   * @param n
   * @return reference
   */
  reference at(size_type n) {
    if (n >= this->size()) {
      throw std::out_of_range("ft::vector::at n is out of range.");
    }
    return (*this)[n];
  }

  const_reference at(size_type n) const {
    if (n >= this->size()) {
      throw std::out_of_range("ft::vector::at n is out of range.");
    }
    return (*this)[n];
  }

  // NOTHROW container is not empty
  // otherwise UB
  reference front(void) { return *(this->_begin); }
  const_reference front(void) const { return *(this->_begin); }

  // NOTHROW container is not empty
  // otherwise UB
  reference back(void) { return *(this->_end - 1); }
  const_reference back(void) const { return *(this->_end - 1); }

  // NOTHROW
  value_type* data(void) { return this->_begin; }
  const value_type* data(void) const { return this->_begin; }

  // SECTION : modifiers
  // BASIC
  // if [first, last) is not valid UB
  /**
   * @brief first ~ last elements 들이 같은 순서로 할당된다.
   * 기존 elements 는 파괴된다.
   * >>> (reallocate iff capacity < new size) <<<
   * @complexity construct(final size), destruct(initial size) 에 대해 각 O(N)
   *
   * @tparam InputIterator
   * @param first
   * @param last
   */
  template <typename InputIterator>
  void assign(InputIterator first, InputIterator last) {
    this->swap(vector(first, last));
    // reallocation 여부, iterator 에 따라 나눈다.
  }

  /**
   * @brief val 이 n개 할당된다. 기존 elements 는 destroy 된다.
   * @complecity O(N) n is size
   *
   * If allocator_traits::construct is not supported with the appropriate
   * arguments for the element constructions, or if the range specified by
   * [first,last) is not valid, it causes undefined behavior.
   * constructor 가 실패하면 UB 이므로.. ㄱㅊ지
   * 않을지.................
   *
   * @param n
   * @param val
   */
  void assign(size_type n, const value_type& val) {
    if (capacity() >= n) {
      size_type cur_size = size();
      // 공통되는 부분까진 n 채운다.
      std::fill_n(this->_begin, std::min(cur_size, n), val);
      // 중간에 실패한다면? 세그폴트 날 것 같은데 소멸자에서?.. -> ㄱㅊㄱㅊ;;
      if (n > cur_size) {
        std::cout << "n is " << n << ", cur_size is " << cur_size << '\n';
        _construct_at_end(n - cur_size, val);
      } else {
        std::cout << "n is " << n << ", cur_size is " << cur_size << '\n';
        _destroy_at_end_(this->_begin + n);
      }
    } else {
      // realloc
      // swap(vector(n, val));
    }
  }

  // STRONG 1. no reallocation 2. reallocation happens & elements copyable
  // BASIC otherwise 사실상 98에서 없을 것 같음
  /**
   * @brief val 을 복사하여 벡터의 끝에 삽입한다.
   * size를 효과적으로 1씩 늘린다.
   * 삽입 후의 사이즈가 capacity 보다 클 때만 (iff) reallocation.
   * reallocation 났을 때만 iterator 보장 불가.
   * @complexity amortized(reallocation happened) O(1)
   *
   * @param val 복사되어 삽입될 element
   */
  void push_back(const value_type& val) {
    if (this->_end < this->_end_cap) {  // size != capacity
      // construct only
      this->_alloc.construct(this->_end, val);
      ++this->_end;
    } else {
      // reallocation
      size_type new_capacity = _get_alloc_size(size() + 1);
      vector tmp(new_capacity);
      // copy
      std::uninitialized_copy(this->_begin, this->_end, tmp.begin());
      tmp._alloc.construct(tmp._begin, val);  // bad_alloc
      tmp._end = tmp._begin + size() + 1;
      tmp._end_cap = tmp.begin + new_capacity;

      std::swap(tmp, *this);
    }
  }

  // NOTHROW container is not empty
  // otherwise UB
  /**
   * @brief vector 의 마지막 element를 삭제해서 size를 1씩 줄인다.
   * 삭제된 element 는 destroy 된다.
   * 삭제된 element 이외를 가리키는 iterator 들은 그대로 사용할 수 있다.
   * @complexity O(1)
   *
   */
  void pop_back(void) { this->_alloc.destory(--this->_end); }

  // STRONG 1. insert single element at the _end, no reallocations happen
  // 2. reallocation happens & elements copyable
  // BASIC otherwise
  iterator insert(iterator position, const value_type& val) {}
  void insert(iterator position, size_type n, const value_type& val) {}
  template <typename InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last) {}

  // NOTHROW removed elements include the last element
  // BASIC otherwise
  // invalid position or range UB
  /**
   * @brief position 위치의 element 하나 삭제
   * 삭제한 position 뒤에 있던 elements 들은 앞으로 이동시켜줘야 한다.
   *
   * @param position
   * @return iterator
   */
  iterator erase(iterator position) { this->_alloc.destroy(position.base()); }
  /**
   * @brief [first, last) elements 삭제. 뒤에 남은 값들 move (contiguos 유지)
   *
   * @param first
   * @param last
   * @return iterator following last removed element
   */
  iterator erase(iterator first, iterator last) {
    // first~last 지우고 뒤에 남은 애들 다시 붙여줘야 됨.....
  }

  // NOTHROW allocator in both vectors compare equal
  // otherwise UB
  void swap(vector& x) {
    std::swap(this->_begin, x._begin);
    std::swap(this->_end, x._end);
    std::swap(this->_end_cap, x._end_cap);
  }

  // NOTHROW
  /**
   * @brief 모든 elements 를 삭제한다. size 를 0으로 설정한다.
   * @complexity O(N)
   */
  void clear(void) { _destroy_at_end_(this->_begin); }

  // NOTHROW
  /**
   * @brief Get the allocator object
   *
   * @return allocator_type
   */
  allocator_type get_allocator(void) const { return this->_alloc; }

 private:
  typedef vector_base<T, Allocator> base_;

  void _construct_at_end(size_type n, const value_type& val) {
    for (size_type i = 0; i < n; ++i) {
      _construct_element(this->_end, val);
      // 여기서 exception throw 되면 어떡함..? -> 내 책임 아님
      this->_end++;
    }
    // this->_end += n;
  }

  /**
   * @brief [pos, end) 까지의 element를 destroy. end를 pos로 대체. (size
   * 재설정) clear(), resize() 에서 사용한다.
   *
   * @param pos
   */
  void _destroy_at_end_(pointer pos) {
    for (pointer tmp = pos; tmp != this->_end; ++tmp) {
      _destroy_element(tmp);
    }
    this->_end = pos;
  }

  /**
   * @brief 새 element 들을 삽입한다.
   * 아직 미사용..
   *
   * @param n
   * @param val
   */
  void _append(size_type n, value_type val) {
    if (this->_end_cap - this->_end >= n) {  // 추가할당공간 충분!
      _construct_at_end(n, val);
      // for (; n > 0; ++n) {
      //  this->_alloc.construct(this->_end, val);
      //++this->_end;
      //}
    } else {
    }
  }

  /**
   * @brief 적절히 resize 할 크기를 리턴한다.
   * _append() 에서 호출.
   * push_back(), resize(), assign() 등에서 사용하면 될 듯?
   *
   * @param new_size 새로 할당해야 하는 사이즈
   * @return size_type 실제로 할당할 사이즈
   */
  size_type _get_alloc_size(size_type new_size) const {
    const size_type ms = max_size();
    if (new_size > ms) {
      throw std::logic_error("ft::vector : reallocation size is too big");
    }
    const size_type cap = capacity();
    if (cap >= ms / 2) {
      return ms;
    }
    return std::max(2 * cap, new_size);
    // 그치 굳이 계속 2*cap 할 필요가 없지..걍 할당 시점에 더 큰 거
    // 리턴하면..되는 거지.. 이걸 몰랐네......
  }

  void _construct_element(pointer p, const value_type& val) {
    this->_alloc.construct(p, val);
  }

  /**
   * @brief allocator 감싸기용 destroy 함수
   *
   * @param p element to destroy
   */
  void _destroy_element(pointer p) { this->_alloc.destroy(p); }

};  // !SECTION : vector

// SECTION : non-member function of vector operator
// SECTION : relational operators
// NOTHROW if the type of elements supports operations
template <typename T, typename Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

template <typename T, typename Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {}

// NOTHROW allocator in both vectors compare equal
// otherwise UB
/**
 * @brief std::swap 에서 이걸 부름
 *
 * @tparam T
 * @tparam Alloc
 * @param x
 * @param y
 */
template <typename T, typename Alloc>
void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
  x.swap(y);
}

}  // namespace ft

#endif  // VECTOR_HPP
