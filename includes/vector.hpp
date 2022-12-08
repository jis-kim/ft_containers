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

  // access to value
  const Iter& base(void) const { return current_; }
};  // !SECTION : vector_iterator

// SECTION : arithmetic operator
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

  allocator_type alloc_;
  pointer _begin;
  pointer _end;
  pointer _end_cap;

  vector_base(
      const allocator_type& allocator,
      typename allocator_type::size_type n = allocator_type::size_type())
      : alloc_(allocator),
        _begin(alloc_.allocate(n)),
        _end(_begin),
        _end_cap(_begin + n) {}

  ~vector_base(void) {
    for (_begin != _end; ++_begin) {
      alloc_.destroy(_begin);
    }
    alloc_.deallocate(_begin, _end - _begin);  // NOEXCEPT
  }

  /**
   * @brief allocate for last
   *
   * @param n
   */
  void allocate_last(typename allocator_type::size_type n) {
    alloc_.allocate(_end, n);
  }
};

// !SECTION : vector_base

// SECTION : vector
template <typename T, typename Allocator = std::allocator<T> >
class vector : private vector_base<T, Allocator> {
 private:
  typedef vector_base<T, Allocator> base_;

  /**
   * @brief [pos, end) 까지의 element를 destroy. end를 pos로 대체.
   * clear(), resize() 에서 사용한다.
   *
   * @param pos
   */
  // NOTHROW
  void _destroy_end_(pointer pos) {
    for (pos != _end; ++pos) {
      alloc_.destroy(pos);  // NOEXCEPT
    }
    end_ = pos;
  }

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
    std::uninitialized_fill(this->_begin, this->_end, val);
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
  vector(const vector& x) : base_(x.alloc_, x.size()) {
    std::uninitialized_copy(this->_begin, this->_end, x._begin);
  }

  // SECTION: destructor
  // NOTHROW
  /**
   * @brief Destroy the vector object
   * @complexity O(N) - N is size of vector
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
   * @brief vector 의 past-the-end element를 가리키는 random access 이터레이터를
   * 반환한다.
   * @complexity O(1)
   *
   * @return iterator
   */
  iterator end(void) { return iterator(this->_end); }
  const_iterator end(void) const { return const_iterator(this->_end); }

  // NOTHROW
  /**
   * @brief 첫 element 의 바로 앞 element 를 가리키는 (reverse end) 이론적인
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
   * @brief 시스템 또는 라이브러리에 따라 vector 가 가질 수 있는 element 의 최대
   * 개수
   *
   * @return size_type
   */
  size_type max_size(void) const { return this->alloc_.max_size(); }

  // NOTHROW n <= size
  // STRONG n > size and reallocation required, type of elements is copyable
  // BASIC otherwise
  /**
   * @brief n 개의 elements 를 포함하도록 컨테이너를 resize 한다.
   * n <= size 이면 처음 ~ n개의 elements 까지 잘리고 뒤에 남은 것들은 destroy.
   * n > size 이면 n에 도달할 때 까지 element 를 insert 한다.
   * @complexity O(N) N is number of elements inserted/erased
   *
   * @param n
   * @param val n이 사이즈보다 크고 val이 주어질 경우 val으로 초기화 한다.
   */
  void resize(size_type n, value_type val = value_type()) {
    size_type size = this->size();
    if (n > size) {
      // reallocation
      // append
    } else if (n < size) {
      _destroy_end(this->begin_ + n);
    }
    // else do nothing.
  }

  // NOTHROW
  size_type capacity(void) const { return this->_end_cap - this->_begin; }

  // NOTHROW
  bool empty(void) const { return this->_begin == this->_end; }

  // STRONG n > size and reallocation required, type of elements is copyable
  // BASIC otherwise
  void reserve(size_type n) {}

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
  }

  /**
   * @brief val 이 n개 할당된다. 기존 elements 는 파괴된다.
   *
   * @param n
   * @param val
   */
  void assign(size_type n, const value_type& val) {
    this->swap(vector(n, val));
  }

  // STRONG 1. no reallocation 2. reallocation happens & elements copyable
  // BASIC otherwise
  void push_back(const value_type& val) {}

  // NOTHROW container is not empty
  // otherwise UB
  void pop_back(void) { _destroy_end_(_end - 1); }

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
   * @brief [position, end) elements 삭제
   *
   * @param position
   * @return iterator
   */
  iterator erase(iterator position) { _destroy_end_(position.base()); }
  /**
   * @brief [first, last) elements 삭제. 뒤에 남은 값들 move (contiguos 유지)
   *
   * @param first
   * @param last
   * @return iterator
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
  void clear(void) {
    base_::_clear();
    this->_end = this->_begin;
  }

  // NOTHROW
  allocator_type get_allocator(void) const { return this->alloc_; }
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
