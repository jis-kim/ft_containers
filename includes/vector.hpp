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

#include "algorithm.hpp"
#include "reverse_iterator.hpp"
#include "type_traits.hpp"

// FIXME : vector_const_iterator
#include <iostream>

namespace ft {
// SECTION : vector iterator
// Random access iterator
// T is pointer of elements
template <typename Iter>
class vector_iterator
    : public iterator<typename iterator_traits<Iter>::iterator_category,
                      typename iterator_traits<Iter>::value_type,
                      typename iterator_traits<Iter>::difference_type,
                      typename iterator_traits<Iter>::pointer,
                      typename iterator_traits<Iter>::reference> {
 private:
  Iter _current;

 public:
  typedef iterator_traits<Iter> traits_type;

  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::pointer pointer;
  typedef typename traits_type::reference reference;

  typedef vector_iterator self;

  vector_iterator(void) : _current() {}
  vector_iterator(const Iter& current) : _current(current) {}

  template <typename Iter2>
  vector_iterator(const vector_iterator<Iter2>& other)
      : _current(other.base()) {}

  ~vector_iterator(void) {}

  reference operator*(void) const { return *_current; }
  pointer operator->(void) const { return _current; }

  self& operator++(void) {
    ++_current;
    return *this;
  }
  self operator++(int) { return vector_iterator(_current++); }

  self& operator--(void) {
    --_current;
    return *this;
  }
  self operator--(int) { return vector_iterator(_current--); }

  // NOTE : 이 경우는 iterator + n 의 경우고 n + iterator 도 가능해야 한다.
  self operator+(difference_type n) const {
    return vector_iterator(_current + n);
  }
  self operator-(difference_type n) const {
    return vector_iterator(_current - n);
  }

  self& operator+=(difference_type n) {
    _current += n;
    return *this;
  }
  self& operator-=(difference_type n) {
    _current -= n;
    return *this;
  }
  reference operator[](difference_type n) const { return _current[n]; }

  const Iter& base(void) const { return _current; }
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
  typedef Allocator allocator_type;
  typedef typename allocator_type::reference reference;              // T&
  typedef typename allocator_type::const_reference const_reference;  // const T&
  typedef typename allocator_type::pointer pointer;                  // T*
  typedef typename allocator_type::const_pointer const_pointer;      // const T*

  allocator_type _alloc;
  pointer _begin;
  pointer _end;
  pointer _end_cap;

  explicit vector_base(const allocator_type& allocator)
      : _alloc(allocator), _begin(NULL), _end(NULL), _end_cap(NULL) {}

  vector_base(const allocator_type& allocator,
              typename allocator_type::size_type n)
      : _alloc(allocator),
        _begin(_alloc.allocate(n)),
        _end(_begin),
        _end_cap(_begin + n) {}

  ~vector_base(void) {
    for (pointer tmp = _begin; tmp != _end; ++tmp) {
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

  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;

  // SECTION : public member functions
  // SECTION : constructor
  // STRONG
  // default
  explicit vector(const allocator_type& alloc = allocator_type())
      : base_(alloc) {}

  // fill
  explicit vector(size_type n, const value_type& val = value_type(),
                  const allocator_type& alloc = allocator_type())
      : base_(alloc, n) {
    std::uninitialized_fill(this->_begin, this->_begin + n, val);
    this->_end += n;
  }

  // range
  /**
   * @brief InputIterator range 로 새 vector 를 생성한다.
   *
   * @tparam InputIterator 한 번 이동하는 순간 기존 값에 다시 접근할 수 없다.
   * @param first
   * @param last
   * @param alloc
   */
  template <typename InputIterator>
  vector(InputIterator first,
         typename enable_if<is_input_iterator<InputIterator>::value &&
                                !is_forward_iterator<InputIterator>::value,
                            InputIterator>::type last,
         const allocator_type& alloc = allocator_type())
      : base_(alloc) {
    for (; first != last; ++first) {
      push_back(*first);
    }
  }

  /**
   * @brief ForwardIterator
   *
   * @tparam ForwardIterator
   * @param first
   * @param last
   * @param alloc
   */
  template <typename ForwardIterator>
  vector(ForwardIterator first,
         typename enable_if<is_forward_iterator<ForwardIterator>::value,
                            ForwardIterator>::type last,
         const allocator_type& alloc = allocator_type())
      : base_(alloc, std::distance(first, last)) {
    this->_end = std::uninitialized_copy(first, last, this->_begin);
  }

  // copy
  vector(const vector& x) : base_(x._alloc, x.size()) {
    this->_end = std::uninitialized_copy(x._begin, x._end, this->_begin);
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
    if (this != &x) {
      assign(x._begin, x._end);
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
  reverse_iterator rbegin(void) { return reverse_iterator(end()); }
  const_reverse_iterator rbegin(void) const {
    return const_reverse_iterator(end());
  }

  // NOTHROW
  /**
   * @brief 첫 element 의 바로 앞 element 를 가리키는 (reverse end) 이론적인
   * reverse random access iterator 를 반환한다.
   * @complexity O(1)
   *
   * @return reverse_iterator
   */
  reverse_iterator rend(void) { return reverse_iterator(begin()); }
  const_reverse_iterator rend(void) const {
    return const_reverse_iterator(begin());
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
    size_type _size = size();
    if (n > capacity()) {
      // STRONG
      // reallocation
      vector tmp;
      tmp._allocate(_get_alloc_size(n));
      tmp._end = std::uninitialized_copy(this->_begin, this->_end, tmp._begin);
      tmp._construct_at_end(n - _size, val);
      swap(tmp);
      return;
    }
    if (n > _size) {
      _construct_at_end(n - _size, val);
    } else if (n < _size) {
      _destroy_at_end(this->_begin + n);
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
   * n > capacity 이면 재할당이 필요하다.. 아니면 아무 일도 일어나지 않는다.
   * n > max size 이면 length_error throw.
   * 재할당이 필요할 경우 container 의 allocator를 이용한다.
   * @complexity 재할당이 일어나면 O(N) 에 가깝다.
   *
   * @param n 벡터의 최소 capacity
   */
  void reserve(size_type n) {
    if (n > capacity()) {
      vector tmp;
      tmp._allocate(_get_alloc_size(n));
      tmp._end = std::uninitialized_copy(this->_begin, this->_end, tmp._begin);
      swap(tmp);
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
  void assign(InputIterator first,
              typename enable_if<is_input_iterator<InputIterator>::value &&
                                     !is_forward_iterator<InputIterator>::value,
                                 InputIterator>::type last) {
    clear();
    for (; first != last; ++first) {
      push_back(*first);
    }
  }

  template <typename ForwardIterator>
  void assign(ForwardIterator first,
              typename enable_if<is_forward_iterator<ForwardIterator>::value,
                                 ForwardIterator>::type last) {
    size_type n = static_cast<size_type>(std::distance(first, last));
    if (capacity() < n) {
      vector(first, last).swap(*this);
    } else {
      // 0 ~ min(size, n) => copy
      size_type n = static_cast<size_type>(std::distance(first, last));
      size_type cur_size = size();
      size_type min_size = min(cur_size, n);
      for (size_type i = 0; i < min_size; ++i) {
        this->_begin[i] = *first;
        ++first;
      }
      if (cur_size < n) {
        for (size_type i = cur_size; i < n; ++i) {
          push_back(*first);
          ++first;
        }
      } else if (cur_size > n) {
        for (size_type i = n; i < cur_size; ++i) {
          pop_back();
        }
      }
    }
  }

  /**
   * @brief val 이 n개 할당된다. 기존 elements 는 destroy 된다.
   * @complecity O(N) n is size
   *
   * If allocator_traits::construct is not supported with the appropriate
   * arguments for the element constructions, or if the range specified by
   * [first,last) is not valid, it causes undefined behavior.
   *
   * @param n 할당할 element 의 개수
   * @param val 할당할 element 의 값
   */
  void assign(size_type n, const value_type& val) {
    if (capacity() < n) {
      vector(n, val).swap(*this);
    } else {
      size_type cur_size = size();
      // 공통되는 부분까진 n을 채운다.
      _fill_n_elements(this->_begin, min(cur_size, n), val);
      if (cur_size < n) {
        _construct_at_end(n - cur_size, val);
      } else if (cur_size > n) {
        _destroy_at_end(this->_begin + n);
      }
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
    if (this->_end >= this->_end_cap) {  // no more space
      // reallocation
      vector tmp;
      // system("leaks ft_containers >/dev/null");
      tmp._allocate(_get_alloc_size(size() + 1));
      tmp._end = std::uninitialized_copy(this->_begin, this->_end, tmp._begin);

      tmp._construct_at_end(1, val);
      swap(tmp);
    } else {
      _construct_at_end(1, val);
    }
  }

  // NOTHROW container is not empty
  // otherwise UB
  /**
   * @brief vector 의 마지막 element를 삭제해서 size를 1씩 줄인다.
   * 삭제된 element 는 destroy 된다.
   * @complexity O(1)
   *
   */
  void pop_back(void) { this->_alloc.destroy(--this->_end); }

  // STRONG 1. insert single element at the _end, no reallocations happen
  // 2. reallocation happens & elements copyable
  // BASIC otherwise
  /**
   * @brief position 앞 위치에 val 을 삽입한다.
   *
   * @param position val 이 삽입될 위치
   * @param val 삽입할 element 값
   * @return iterator 새로 삽입한 elements 의 맨 앞 위치
   */
  iterator insert(iterator position, const value_type& val) {
    pointer p = this->_begin + (position - begin());
    if (p == this->_end) {
      // STRONG
      push_back(val);
      return this->_end - 1;
    }
    if (size() + 1 > capacity()) {
      // STRONG
      vector tmp;
      tmp._allocate(_get_alloc_size(size() + 1));
      tmp._end = std::uninitialized_copy(this->_begin, p, tmp._begin);
      tmp._construct_at_end(1, val);
      tmp._end = std::uninitialized_copy(p, this->_end, tmp._end);
      swap(tmp);
    } else {
      // BASIC
      _construct_at_end(1, *(this->_end - 1));
      _copy_elements_backward(p, this->_end - 2, this->_end - 2);
      *p = val;
    }
    return iterator(p);
  }

  /**
   * @brief position 앞 위치에 n개의 val 을 삽입한다.
   * iff) 새 벡터의 사이즈 > capacity 일 경우 reallocation
   *
   * @param position val 이 삽입될 위치
   * @param n 삽입할 element 의 수
   * @param val 삽입할 element 값
   */
  void insert(iterator position, size_type n, const value_type& val) {
    if (n == 0) {
      return;
    }
    pointer p = this->_begin + (position - begin());
    if (size() + n > capacity()) {
      // STRONG
      vector tmp;
      tmp._allocate(_get_alloc_size(size() + n));
      tmp._end = std::uninitialized_copy(this->_begin, p, tmp._begin);
      std::uninitialized_fill_n(tmp._end, n, val);
      tmp._end = std::uninitialized_copy(p, this->_end, tmp._end + n);
      swap(tmp);
    } else {
      // BASIC
      if (p != this->_end) {
        pointer old_end = this->_end;
        this->_end = std::uninitialized_copy(old_end - n, old_end, old_end);
        _copy_elements_backward(p, old_end - n, old_end - 1);
        _fill_n_elements(p, n, val);
      } else {
        _construct_at_end(n, val);
      }
    }
  }

  /**
   * @brief position 앞 위치에 [first, last) 의 element 를 삽입한다.
   *
   * @tparam InputIterator
   * @param position
   * @param first
   * @param last
   */
  template <typename InputIterator>
  void insert(iterator position, InputIterator first,
              typename enable_if<is_input_iterator<InputIterator>::value &&
                                     !is_forward_iterator<InputIterator>::value,
                                 InputIterator>::type last) {
    if (first == last) {
      return;
    }
    difference_type offset = position - begin();
    difference_type n = offset;
    for (; first != last; ++first) {
      // 재할당이 일어날 수도 있다. 일어난다면 position 무효화됨.
      insert(this->_begin + n, *first);
      ++n;
    }
  }

  /**
   * @brief position 앞 위치에 [first, last) 의 element 를 삽입한다.
   * ForwardIterator 를 사용해서 미리 new size 를 알 수 있다.
   *
   * @tparam ForwardIterator
   * @param position
   * @param first
   * @param last
   */
  template <typename ForwardIterator>
  void insert(iterator position, ForwardIterator first,
              typename enable_if<is_forward_iterator<ForwardIterator>::value,
                                 ForwardIterator>::type last) {
    difference_type n = std::distance(first, last);
    pointer p = this->_begin + (position - begin());
    if (n == 0) {
      return;
    }
    if (size() + n > capacity()) {
      // STRONG
      vector tmp;
      std::cout << "tmp size : " << tmp.size() << std::endl;
      tmp._allocate(_get_alloc_size(size() + n));
      tmp._end = std::uninitialized_copy(this->_begin, p, tmp._begin);
      tmp._end = std::uninitialized_copy(first, last, tmp._end);
      tmp._end = std::uninitialized_copy(p, this->_end, tmp._end);
      swap(tmp);
    } else {
      // BASIC
      if (p != this->_end) {
        // [end - n, end) 까지를 end 에 construct (n개)
        pointer old_end = this->_end;
        this->_end = std::uninitialized_copy(old_end - n, old_end, old_end);
        // [position, end - n) 까지를 [end - n,  end) 까지로 copy (aps - n개)
        _copy_elements_backward(p, old_end - n, old_end);
        _copy_elements(first, last, p);
      } else {
        // position == end
        for (; first != last; ++first) {
          push_back(*first);
        }
      }
    }
  }

  // NOTHROW removed elements include the last element
  // BASIC otherwise
  // invalid position or range UB
  /**
   * @brief position 위치의 element 하나 삭제
   * 삭제한 position 뒤에 있던 elements 들은 앞으로 이동시켜줘야 한다.
   * @complexity O(N) 지울 element 의 수 + 뒤에 남은 element 의 수 (moved)
   *
   * @param position
   * @return iterator 함수 호출로 지워진 마지막 element 의 다음 위치
   */
  iterator erase(iterator position) {
    pointer p = this->_begin + (position - begin());
    if (p != this->_end - 1) {
      _copy_elements(p + 1, this->_end, p);
    }
    _destroy_at_end(this->_end - 1);
    return iterator(p);
  }

  /**
   * @brief [first, last) elements 삭제. 뒤에 남은 값들 move (contiguos 유지)
   * @complexity O(N) 지울 element 의 수 + 뒤에 남은 element 의 수 (moved)
   *
   * @param first 지울 첫번째 element
   * @param last  지울 마지막 element의 다음 위치
   * @return iterator following last removed element
   */
  iterator erase(iterator first, iterator last) {
    pointer first_p = this->_begin + (first - begin());
    pointer last_p = this->_begin + (last - begin());
    if (first_p != last_p) {
      _copy_elements(last_p, this->_end, first_p);
      _destroy_at_end(this->_end - (last_p - first_p));
    }
    return iterator(first_p);
  }

  // NOTHROW allocator in both vectors compare equal
  // otherwise UB
  /**
   * @brief 같은 T 타입을 가진 벡터를 swap 한다. 사이즈는 다를 수 있다.
   * @complexity O(1)
   *
   * @param x
   */
  void swap(vector& x) {
    _swap_pointer(this->_begin, x._begin);
    _swap_pointer(this->_end, x._end);
    _swap_pointer(this->_end_cap, x._end_cap);
  }

  // NOTHROW
  /**
   * @brief 모든 elements 를 삭제한다. size 를 0으로 설정한다.
   * @complexity O(N)
   */
  void clear(void) { _destroy_at_end(this->_begin); }

  // NOTHROW
  /**
   * @brief Get the allocator object
   * @complexity O(1)
   *
   * @return allocator_type
   */
  allocator_type get_allocator(void) const { return this->_alloc; }

  // SECTION : private functions
 private:
  typedef vector_base<T, Allocator> base_;

  /**
   * @brief 적절히 resize 할 크기를 리턴한다.
   * push_back(), resize(), assign() 등에서 사용
   *
   * @param new_size 새로 할당해야 하는 사이즈
   * @return size_type 실제로 할당할 사이즈
   */
  size_type _get_alloc_size(size_type new_size) const {
    const size_type _max_size = max_size();
    if (new_size > _max_size) {
      throw std::logic_error("ft::vector : reallocation size is too big");
    }
    const size_type cap = capacity();
    if (cap >= _max_size / 2) {
      return _max_size;
    }
    return max(2 * cap, new_size);
  }

  void _swap_pointer(pointer& p1, pointer& p2) {
    pointer tmp = p1;
    p1 = p2;
    p2 = tmp;
  }

  /**
   * @brief allocate 위치를 지정하기 어려우므로 초기에만 호출한다고 가정함.
   *
   * @param n
   */
  void _allocate(size_type n) {
    this->_end = this->_begin = this->_alloc.allocate(n);
    this->_end_cap = this->_begin + n;
    // std::cout << "end_cap " << *(this->_end_cap) << '\n';
  }

  /**
   * @brief [first, last) 를 end에 생성한다.
   *
   * @param first range 의 첫번째 element
   * @param last range 의 마지막 element의 다음 위치
   * @param dest 생성할 element 들의 시작 위치 (this)
   */
  void _construct_by_range(iterator first, iterator last) {
    for (; first != last; ++first) {
      _construct_element(this->_end, *first);
      ++this->_end;
    }
  }

  /**
   * @brief end() 위치에 n개의 element를 생성한다.
   *
   * @param n 생성할 element 의 개수
   * @param val 생성할 element 의 값
   */
  void _construct_at_end(size_type n, const value_type& val) {
    for (size_type idx = 0; idx < n; ++idx) {
      _construct_element(this->_end, val);
      ++this->_end;
    }
  }

  /**
   * @brief [pos, end) 까지의 element를 destroy. end를 pos로 대체. (size
   * 재설정) clear(), resize() 에서 사용한다.
   *
   * @param pos
   */
  void _destroy_at_end(pointer pos) {
    for (pointer tmp = pos; tmp < this->_end; ++tmp) {
      _destroy_element(tmp);
    }
    this->_end = pos;
  }

  /**
   * @brief allocator 캡슐화하는 construct 함수
   *
   * @param p construct 할 위치
   * @param val construct 할 값
   */
  void _construct_element(pointer p, const value_type& val) {
    this->_alloc.construct(p, val);
  }

  /**
   * @brief allocator 캡슐화하는 destroy 함수
   *
   * @param p element to destroy
   */
  void _destroy_element(pointer p) { this->_alloc.destroy(p); }

  /**
   * @brief copy of std::copy specialization for T*
   *
   * @param begin 복사하려는 범위의 첫번째 element
   * @param end 복사하려는 범위의 마지막 element의 다음 위치
   * @param dest 복사할 위치
   * @return pointer 복사한 마지막 위치의 다음 위치
   */
  template <typename InputIterator, typename OutputIterator>
  OutputIterator _copy_elements(InputIterator begin, InputIterator end,
                                OutputIterator dest) {
    for (InputIterator tmp = begin; tmp != end; ++tmp) {
      *dest = *tmp;
      ++dest;
    }
    return dest;
  }

  /**
   * @brief copy of std::copy_backward.. 근데 내 입맛대로를 곁들인
   * [begin, end)
   *
   * @param begin 복사하려는 범위의 마지막 element
   * @param end 복사하려는 범위의 첫번째 element의 이전 위치
   * @param dest 복사할 마지막 위치
   * @return pointer 복사한 첫번째 위치의 이전 위치
   */
  template <typename InputIterator, typename OutputIterator>
  OutputIterator _copy_elements_backward(InputIterator begin, InputIterator end,
                                         OutputIterator dest) {
    for (InputIterator tmp = end - 1; tmp >= begin; --tmp) {
      *dest = *tmp;
      --dest;
    }
    return dest;
  }

  /**
   * @brief copy of std::fill_n specialization for T
   *
   * @param dest element 를 넣을 위치
   * @param n 채울 element 의 수
   * @param val 채울 값
   * @return pointer 채운 마지막 위치의 다음 위치
   */
  pointer _fill_n_elements(pointer dest, size_type n, const value_type& val) {
    for (size_type idx = 0; idx < n; ++idx) {
      *dest = val;
      ++dest;
    }
    return dest;
  }

};  // !SECTION : vector

// SECTION : non-member function of vector operator
// SECTION : relational operators
// NOTHROW if the type of elements supports operations
template <typename T, typename Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return (lhs.size() == rhs.size()) &&
         equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                 rhs.end());
}

template <typename T, typename Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <typename T, typename Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return rhs < lhs;
}

template <typename T, typename Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs < rhs);
}

// NOTHROW allocator in both vectors compare equal
// otherwise UB
/**
 * @brief vector를 swap 한다. 두 벡터는 같은 타입이어야 한다.
 * <algorithm> 에 있는 std::swap 을 오버로딩해서
 *  x.swap(y) 가 불린 것 처럼 작동하도록 한다.
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
