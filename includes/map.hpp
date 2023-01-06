/**
 * @file map.hpp
 * @author jiskim
 * @brief
 * @date 2023-01-02
 *
 * @copyright Copyright (c) 2023
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include <functional>  // std::less
#include <memory>      //std::allocator

#include "_rb_tree.hpp"

// TODO : std::pair to ft::pair

namespace ft {

template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<std::pair<const Key, T>>>
class map {
 private:
  typedef _rb_tree<Key, T, std::pair<const Key, T>, Compare, Alloc> _rep_type;

  _rep_type _tree;  // structure

 public:
  typedef Key key_type;
  typedef T mapped_type;
  typedef std::pair<const Key, T> value_type;  // FIXME ft::pair
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef Compare key_compare;
  typedef Alloc allocator_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;

  typedef typename _rep_type::iterator iterator;
  typedef typename _rep_type::const_iterator const_iterator;
  typedef typename _rep_type::reverse_iterator reverse_iterator;
  typedef typename _rep_type::const_reverse_iterator const_reverse_iterator;

  // SECTION : constructor
  // STRONG
  // allocator_traits::construct 가 적절한 인자를 받을 수 없거나
  // [first, last) 가 invalid 하면 UB
  /**
   * @brief create empty map
   * @complexity O(1)
   *
   */
  map(void) {}
  explicit map(const Compare& comp, const Alloc alloc = Alloc()) {}

  /**
   * @brief Range constructor. 범위 안에 key 값이 같은 element 가 있다면 어떤
   * element 가 삽입될 지 특정할 수 없다.
   * @complexity 삽입할 범위가 정력되어 있으면 O(N), 아니면 O(N log N).
   *             N is distance
   *
   * @tparam InputIterator
   * @param first
   * @param last
   * @param comp
   * @param alloc
   */
  template <typename InputIterator>
  map(InputIterator first, InputIterator last, const Compare& comp = Compare(),
      const Alloc& alloc = Alloc()) {}

  /**
   * @brief x의 allocator 와 comparison object 의 복사본을 사용하는 container
   * 생성.
   *
   * @param x Key, T, Compare, Alloc 이 같은 map 객체.
   */
  map(const map& x) {}

  // NOTHROW
  ~map(void) {}

  // BASIC
  // allocator_traits::construct 가 적절한 인자를 받을 수 없거나
  // value_type 이 copy assignable 하지 않으면 UB.
  map& operator=(const map& x) {}

  // SECTION : iterators
  // NOTHROW
  /**
   * @brief map 컨테이너는 항상 정렬되어 있으므로 begin 은 가장 앞에 정렬된
   * element 를 가리킨다.
   * @complexity O(1)
   *
   * @return iterator
   */
  iterator begin(void) {}
  const_iterator begin(void) const {}

  // NOTHROW
  /**
   * @brief
   * @complexity O(1)
   *
   * @return iterator past-the-end 를 가리키는 iterator (bidirectional)
   */
  iterator end(void) {}
  const_iterator end(void) const {}

  // NOTHROW
  /**
   * @brief rbegin 은 end 의 앞을 가리킨다.
   * @complexity O(1)
   *
   * @return reverse_iterator 컨테이너의 마지막 element 를 가리키는 reverse
   * iterator (bidirectioinal)
   */
  reverse_iterator rbegin(void) {}
  const_reverse_iterator rbegin(void) const {}

  // NOTHROW
  /**
   * @brief rend 는 begin 의 앞을 가리킨다.
   * @complexity O(1)
   *
   * @return reverse_iterator 컨테이너의 reverse end 를 가리키는 reverse
   * iterator (bidirectional)
   */
  reverse_iterator rend(void) {}
  const_reverse_iterator rend(void) const {}
  // !SECTION : iterators

  // SECTION : capacity

  // NOTHROW
  /**
   * @brief map이 비어 있는지 여부를 리턴한다. 컨테이너를 변경하지는 않는다.
   * @complexity O(1)
   *
   * @return true size is 0
   * @return false otherwise
   */
  bool empty(void) const {}

  // NOTHROW
  /**
   * @brief 컨테이너 사이즈를 반환한다.
   * @complexity O(1)
   *
   * @return size_type the number of elements in the container
   */
  size_type size(void) const {}

  // NOTHROW
  /**
   * @brief map 컨테이너가 가질 수 있는 최대 element 수를 반환한다.
   * @complecity O(1)
   *
   * @return size_type
   */
  size_type max_size(void) const {}

  // !SECTION : capacity

  // SECTION : element access

  // STRONG
  // new element 가 insert 될 떄 allocator_traits::construct 가 k와 기본
  // 생성되는 mapped_type 으로 construct 할 수 없다면 (혹은 mapped type 이
  // default constructible 하지 않다면) UB
  /**
   * @brief k 가 container 의 element 중 하나와 일치한다면 그 mapped value 의
   * 레퍼런스를 리턴한다. 그렇지 않다면 새로운 element 를 생성하고 그 레퍼런스를
   * 리턴한다.
   * (*((this->insert(make_pair(k,mapped_type()))).first)). 와 동일하다.
   *
   * @complexity O(log N) N is size
   *
   * @param k
   * @return mapped_type&
   */
  mapped_type& operator[](const key_type& k) {}

  // STRONG
  /**
   * @brief key 값 k 로 element 를 찾아서 mapped value reference 를 리턴한다.
   * element 가 존재하지 않는다면 out_of_range exception throw.
   * @complexity O(log N) N is size
   *
   * @param k
   * @return mapped_type&
   */
  mapped_type& at(const key_type& k) {}
  const mapped_type& at(const key_type& k) const {}

  // !SECTION:  element access

  // SECTION : modifiers

  // STRONG single element 가 삽입되는 경우
  // BASIC otherwise
  // allocator_traits::construct 가 적절한 인자를 받을 수 없거나
  // position 이 유효하지 않다면 UB
  /**
   * @brief
   * @complexity O(log N) N is size
   *
   * @param val
   * @return std::pair<iterator, bool> 새로 삽입에 성공했다면 first 는 삽입한
   * element 를 가리키는 iterator, second 는 true.
   * 삽입에 실패했다면 first 는 이미 존재하는 element 를 가리키는 iterator,
   * second 는 false.
   */
  std::pair<iterator, bool> insert(const value_type& val) {}

  /**
   * @brief
   * @complexity hint (position) 이 최적의 값일 경우 amortized O(1)
   * worst case O(log N) N is size
   *
   * @param position element 가 삽입될 수 있는 위치에 대한 hint 이다.
   * @param val
   * @return iterator 삽입에 성공했다면 삽입된 element 를 가리키는 iterator
   * 실패했다면 key 가 같은 이미 존재하는 element 를 가리키는 iterator.
   */
  iterator insert(iterator position, const value_type& val) {}

  /**
   * @brief
   * @complexity O(N log(size + N)) 이 일반적이지만
   * [first, last) 가 container 의 기준과 동일하게 정렬되어 있으면 O(size + N).
   * N is distance between first and last
   *
   * @tparam InputIterator
   * @param first
   * @param last
   */
  template <typename InputIterator>
  void insert(InputIterator first, InputIterator last) {}

  // NOTHROW 컨테이너의 comparison object 가 throw 하지 않는 이상 nothrow.
  // STRONG single element 가 지워지는 경우
  // BASIC otherwise
  // position 이나 range [first, last) 가 유효하지 않다면 UB
  /**
   * @brief
   * @complexity amortized O(1)
   *
   * @param position
   */
  void erase(iterator position) {}

  /**
   * @brief
   * @complexity O(log N) N is size
   *
   * @param k
   * @return size_type 지워진 element 의 개수
   */
  size_type erase(const key_type& k) {}

  /**
   * @brief [first, last) 를 삭제한다.
   * @complexity O(N) N is distance between first and last
   *
   * @param first
   * @param last
   */
  void erase(iterator first, iterator last) {}

  // NOTHROW 두 컨테이너의 compare 가 같거나 allocator traits 가 allocator 가
  // 전파되어야 함을 알 경우(? 먼소리)
  // otherwise UB
  void swap(map& x) {}

  // NOTHROW
  /**
   * @brief 모든 element 들을 지운다. container 의 size 는 0 이 된다.
   * @complexity O(N) N is size - destruction
   *
   */
  void clear(void) {}

  // !SECTION : modifiers

  // SECTION : observers

  // STRONG 아니 이게 뭐라고 strong?
  /**
   * @brief key 를 비교하는 comparison object 를 리턴한다. (기본값
   * std::less<key_type>)
   * @complexity O(1)
   *
   * @return key_compare
   */
  key_compare key_comp(void) const {}

  // STRONG
  /**
   * @brief key 에 따라 element 들을 비교하는 comparison object 를 리턴한다.
   * value_compare 의 생성자는 public 이 아니므로 밖에서 직접 생성할 수 없다.
   *
   * @return value_compare (nested class type)
   */
  value_compare value_comp(void) const {}

  // !SECTION : observers

  // SECTION : operations

  // STRONG
  /**
   * @brief
   * @complexity O(log N) N is size
   *
   * @param k
   * @return k 라는 key 가 있으면 그 element 를 가리키는 iterator, 없으면 end
   */
  iterator find(const key_type& k) {}
  const_iterator find(const key_type& k) const {}

  // STRONG
  /**
   * @brief k 라는 key 를 가진 element 의 개수.
   * map 의 key 는 unique 하므로 0 또는 1 이다.
   * @complexity O(log N) N is size
   *
   * @param k
   * @return size_type k 라는 key 를 가진 element 가 있으면 1, 없으면 0
   */
  size_type count(const key_type& k) const {}

  // STRONG
  /**
   * @brief k 보다 앞에 있지 않은 (같거나 뒤에 있는) 첫번째 element 를 가리키는
   * iterator 를 리턴한다.
   *
   * @param k
   * @return iterator
   */
  iterator lower_bound(const key_type& k) {}
  const_iterator lower_bound(const key_type& k) const {}

  // STRONG
  /**
   * @brief
   *
   * @param k
   * @return iterator
   */
  iterator upper_bound(const key_type& k) {}
  const_iterator upper_bound(const key_type& k) const {}

  // STRONG
  /**
   * @brief k 와 같은 key 를 가진 element 들의 range 를 리턴한다.
   * map 은 unique key 를 가지므로 range 의 길이는 0 또는 1 이다.
   *
   * @param k
   * @return std::pair<iterator, iterator> pair::first 는 lower_bound,
   * pair::second 는 upper_bound
   */
  std::pair<iterator, iterator> equal_range(const key_type& k) {}
  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& k) const {}

  // !SECTION : operations

  // SECTION : allocator

  // NOTHROW
  /**
   * @brief Get the allocator object
   *
   * @return allocator_type map 의 allocator
   */
  allocator_type get_allocator(void) const {}
};

}  // namespace ft

#endif  // MAP_HPP_
