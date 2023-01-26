

# ft_containers
C++ containers, easy mode

<br>

# 구현사항
## containers
- [`vector`](https://evening-cushion-319.notion.site/vector-f53ef526e55a4dc0a62144cd257584aa)
- [`stack`](https://evening-cushion-319.notion.site/stack-fba591344894433d9a1b0befacb89bb3)
- [`map`](https://evening-cushion-319.notion.site/map-4df850e49fef4511a868d5730fbeecc2)
- `set` (bonus)

## else
- `reverse_iterator`
- `enable_if (c++11)`
- `is_integral`
- `equal` and/or `lexicographical_compare`
- `std::pair`
- `std::make_pair`

---

## Exception-safety Guarantees
[Standard-Libray Exception Safety](https://www.stroustrup.com/3rd_safe.pdf)

C++ standard library 에서는 exception safety 를 보장하기 위해 3가지 guarantee 를 제공한다.

1.  **Basic guarantee for all operations**
    - stl의 기본 invariants 가 유지되고 memory 와 같은 리소스의 누수가 발생하지 않는다.
2. **Strong guarantee for key operations**
   - basic guarantee + operation 이 성공하거나 효과가 없다. key library operation에 제공된다. ( `push_back()`, `list`의 single element `insert()`, `uninitialized_copy()` )
3. **Nothrow guarantee for some operations**
   - basic guarantee + `exception` 을 던지지 않는다. 간단한 `swap(), pop_back()` 같은 operation 들에 제공된다.

<br>

## Exception-safe implementation techniques
```cpp
// warning: naive implementation
template <class T, class A>
vector<T, A>::vector(size_type n, const T& val, const A& a)
  :alloc(a) // copy the allocator
{
  v = alloc.allocate(n); // get memory for elements (§19.4.1)
  space = last = v + n;
  for(T* p= v; p != last; ++p)
    a.construct(p, val) ; // construct copy of val in *p (§19.4.1)
}
```
- 위와 같이 생성자를 구현했을 경우, 세 부분에서 exception 이 throw 될 수 있다.

1. `allocate()` 에서 메모리 할당 실패할 경우.
2. `alloc(a)` 에서 복사 생성자가 throw 할 경우.
3. `T` 의 복사 생성자가 `val` 을 복사할 수 없을 경우.

- 3번의 경우, 일부 리소스만 할당받은 채로 exception 이 throw 되어 적절한 처리를 해주지 않는다면 memory leak 이 발생할 수 있다.
이를 추적해서 파괴해야 Basic guarantee 를 보장할 수 있다.

### 1. try-catch block
```cpp
template<class T, class A>
vector<T,A>::vector(size_type n, const T& val, const A& a) // messy implementation
  :alloc(a) // copy the allocator
{
  v = alloc.allocate(n); // get memory for elements
  iterator p;
  try {
    iterator end = v + n;
    for(p = v; p != end; ++p)
      alloc.construct(p,val); //  construct element (§19.4.1)
    last = space = p;
  } catch(...) {
    for(iterator q = v; q != p; ++q)
      alloc.destroy(q) ; // destroy constructed elements
    alloc.deallocate(v, n) ; // free memory
    throw; // re-throw
  }
}
```
- `try` 블록에서 예외가 발생하면 `catch` 블록에서 예외를 처리하고 라이브러리의 사용자들에게 문제가 발생했음을 알리기 위해 re-throw 해 준다.

### 2. **RAII (Resource Acquisition Is Initialization)**
- RAII 는 리소스를 얻는 것을 객체의 생성자에서, 해제하는 것을 객체의 소멸자에서 처리하는 것을 말한다.
- try-catch 의 대안으로 사용해서 작성해야 하는 코드의 양을 줄이고 좀 더 예쁜 코드를 만들 수 있다!
```cpp
template<class T, class A= allocator<T> >
struct vector_base{
  A alloc; // allocator
  T* v; // start of allocation
  T* space; // end of element sequence, start of space allocated for possible expansion
  T* last; // end of allocated space

  vector_base(const A& a, typename A::size_type n)
    : alloc(a) , v(a.allocate(n)) , space(v + n) , last(v + n) {}
  ~vector_base() { alloc.deallocate(v, last - v) ; }
};
```
- `vector` 의 핵심 리소스는 element 들을 담을 메모리 이므로 메모리만 다루는 `vector_base` 클래스를 사용한다.

```cpp
template<class T, class A = allocator<T> >
class vector : private vector_base<T, A> {
	void destroy_elements() { for(T* p = v; p != space; ++p) p->~T() ; } // §10.4.11
public:
  explicit vector(size_type n, const T& val = T() , const A& = A());
  vector(const vector& a) ; // copy constructor
  vector& operator=(const vector& a) ; // copy assignment

  ~vector() { destroy_elements(); }
   // ...
};
```
- `vector_base` 의 생성자에서 `allocate()` 하고, `vector` 의 생성자에서 `construct()` 한다.
- `vector` 의 소멸자에서 `destroy_elements()` 를 호출해서 element 들을 파괴하고, `vector_base` 의 소멸자에서 `deallocate()` 로 메모리를 해제한다.

이로 인해 exception 이 throw 되었을 때 간단한 방식으로 처리할 수 있다.

1. `allocate` 할 때 exception throw 되었을 경우

    ⇒ `vector_base` 도, `vector` 도 아직 생성되지 않은 상태이다. 공간 할당도 이루어지지 않았으므로 leak 이 나지 않는다.

2. `construct` 할 때 exception throw 되었을 경우

    ⇒ 베이스 클래스인 `vector_base` 에서 `allocate()` 하고, `construct()` 시점엔 `vector_base` 의 sub object 가 생성된 상태이므로 `construct()` 중간에 throw 되어도 스코프(생성자) 를 벗어나면 sub object 의 생명주기가 끝나서 안전하게 `deallocate()` 및 `destroy()` 된다.

# vector
# stack

# red-black tree (map, set)
# _rb_tree 구현

## 1. structs

### _rb_tree_node_base

```cpp
struct _rb_tree_node_base {
  typedef _rb_tree_node_base* base_ptr;
  typedef const _rb_tree_node_base* const_base_ptr;

  _rb_tree_color color;
  base_ptr parent;
  base_ptr left;
  base_ptr right;

  void _node_init(base_ptr p) {
    color = RED;
    parent = p;
    left = NULL;
    right = NULL;
  }
};
```

- `value` 없이 `pointer` 만 가지는 base class 여서 template 을 사용하지 않고 구현할 수 있다.
- red-black tree 에서 이루어지는 연산들은 `value` 정보가 없어도 되는 경우가 많기 때문에 노드의 `parent`, `left`, `right` 를 가리키는 포인터를 `_rb_tree_node_base` 타입으로 구현해서 함수의 독립성을 높이고 불필요한 템플릿의 사용을 줄인다.

### _rb_tree_node

```cpp
template <typename Val>
struct _rb_tree_node : public _rb_tree_node_base {
  typedef _rb_tree_node<Val>* link_type;
  typedef Val value_type;

  value_type value;

  _rb_tree_node(const value_type& value = value_type()) : value(value) {}
};
```

- 실제 `value` 를 저장하는 node struct. 이 타입으로 `allocate` 하여 트리를 만든다.
- `_rb_tree_node_base` 를 상속받아 구현한다.

### _rb_tree_header

```cpp
struct _rb_tree_header {
  _rb_tree_node_base _header;
  size_t _node_count;

  _rb_tree_header(void) {
    _header.color = RED;
    _reset();
  }

  /**
   * @brief tree 가 empty 일 때 header 를 초기화.
   * 초기 상태 : root 는 NULL, leftmost, rightmost 는 header 자신 (end)

   */
  void _reset(void) {
    _header.parent = NULL;
    _header.left = &_header;
    _header.right = &_header;
    _node_count = 0;
  }

	//  ...

};
```

- tree 의 정보를 저장하는 `_header` 더미 노드와 노드의 개수`_node_count` 를 저장한다.
- `_header` 의 `parent` 에는 root, `left` 에는 left-most, `right` 에는 right-most 를 저장해서 각 노드로의 빠른 접근을 가능하게 한다.

### _rb_tree_impl

```cpp
template <typename Compare>
struct _rb_tree_impl : public _rb_tree_header, _rb_tree_key_compare<Compare> {
  typedef _rb_tree_key_compare<Compare> _base_key_compare;

  _rb_tree_impl(void) : _rb_tree_header(), _base_key_compare() {}
  _rb_tree_impl(const _rb_tree_impl& src)
      : _rb_tree_header(), _base_key_compare(src._compare) {}
  explicit _rb_tree_impl(const Compare& comp)
      : _rb_tree_header(), _base_key_compare(comp) {}
};
```

- `_rb_tree_header`, `_rb_tree_key_compare` (Compare 객체를 저장하는 helper class) 를 상속받아 tree 의 데이터를 가지고 있는 struct 를 구현한다. 내부 객체 초기화는 base class 의 생성자에서 이루어지므로 캡슐화된다.

실제로 tree 가 멤버 변수로 가지는 객체는 `_rb_tree_impl` 하나로 해결할 수 있다.

### _rb_tree

```cpp
template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc = std::allocator<Val> >
class _rb_tree {
	// typedefs ..

 private:
	typedef typename Alloc::template rebind<_rb_tree_node<value_type> >::other
    node_allocator;

  _rb_tree_impl<Compare> _impl;
  node_allocator _alloc;

	// member functions
	// ...
};
```

- `_impl` 에 있는 `_header` 노드에 모든 트리의 정보를 저장하고 함수만 선언한다.

## 2. tree 주요 멤버함수

- `map`과 `set` 에서 호출하기 위한 함수들이 주를 이룬다. 해당 컨테이너들의 명세를 참고하면서 exception safety 와 complexity 를 고려하여 구현한다.

### copy constructor & assignment operator (operator=)

```cpp
_rb_tree(const _rb_tree& x) : _impl(x._impl), _alloc(x._alloc) {
  if (x._root() != NULL) {
    _copy_tree(x);
  }
}
```

```cpp
_rb_tree& operator=(const _rb_tree& x) {
  if (this != &x) {
    clear();
    _impl._compare = x._impl._compare;
    if (x._root() != NULL) {
      _copy_tree(x);
    }
  }
  return *this;
}
```

- `map` 의 명세를 보면 [복사생성자](https://cplusplus.com/reference/map/map/map/) 및 [할당연산자](https://cplusplus.com/reference/map/map/operator=/)에 sorted 된 element 가 들어오는 경우 O(nlogn) 이 아니라 O(n) 의 시간복잡도를 가진다.

    따라서 불필요한 rotation 없이 주어진 트리를 일반 이진트리처럼 그대로 복사하는 `_copy_tree`함수를 내부에서 호출하여 구현한다.


```cpp
// copy constructor, assignment operator 에서 호출
void _copy_tree(const _rb_tree& x) {
  _impl._header.parent = _copy_nodes(x._root(), _root());
  _impl._header.left = _get_subtree_min(_impl._header.parent);
  _impl._header.right = _get_subtree_max(_impl._header.parent);
  _impl._header.parent->parent = _get_end();
  _impl._node_count = x._impl._node_count;
}

/**
 * @brief 완성된 tree 를 그대로 복사한다.
 * tree 가 정렬된 것이 확실하면 _insert_rebalance 를 사용하지 않고 더 빠르게
 * 복사할 수 있다.
 *
 * @param x 복사할 node
 * @param p x를 달 parent
 * @return link_type 복사된 node 의 포인터
 */
link_type _copy_nodes(link_type x, base_ptr p) {
  // x 는 NULL 이 아닌 것이 보장되어 있다.
  link_type top = _clone_node(x);
  top->parent = p;

  try {
		// top 이 서브트리 root 이므로 자식 연결만 해준다.
    if (x->right) {
      top->right = _copy_nodes(_right(x), top); // 우측 트리는 재귀호출로 복사
    }
    p = top;      // 부모 노드 p를 새로 생성된 노드로 update
    x = _left(x); // 복사할 노드x 를 x->left로 update

    while (x != NULL) {
      link_type y = _clone_node(x);
      p->left = y;
      y->parent = p;
      if (x->right) {
        y->right = _copy_nodes(_right(x), y);
      }
      p = y;
      x = _left(x);  // 왼쪽 subtree 가 없을 때 까지 반복
    }
  } catch (...) {
    _erase_all(top);  // rollback
    throw;
  }
  return top;
}
```

- STRONG guarantee 가 보장되어야 하기 때문에 `_copy_nodes` 함수에서 exception 이 났을 경우 `_erase_all` 으로 생성된 모든 node 를 destroy 해준다.

### insert

```cpp
// 1. single element
pair<iterator, bool> insert(const value_type& val) {
  pair<base_ptr, base_ptr> res = _find_insert_pos(KeyOfValue()(val));
  if (res.second) {
    return pair<iterator, bool>(_insert(res.first, res.second, val), true);
  }
  return pair<iterator, bool>(iterator(res.first), false);
}

// 2. with hint
/**
 * @brief insert with hint
 *
 * @param position hint position
 * @param val
 * @return iterator 성공 -> 새 element, 실패 -> 기존 element
 */
iterator insert(iterator position, const value_type& val) {
  pair<base_ptr, base_ptr> res =
      _find_insert_hint_pos(position, KeyOfValue()(val));
  if (res.second) {
    return _insert(res.first, res.second, val);
  }
  return iterator(res.first);
}

// range
template <typename InputIterator>
void insert(InputIterator first, InputIterator last) {
  for (iterator end = end(); first != last; ++first) {
    insert(end, *first);
  }
}
```

- single element 와 hint `insert` 는 노드를 삽입할 적절한 위치를 찾은 뒤, 실제로 노드 생성, 삽입, 재정렬 (red-black 성질 복구) 를 수행하는  `_insert` 함수를 호출한다.
- `map` 의 range constructor 에서 range `insert` 를 호출하는데, 이 때 range 가 정렬된 값일 경우 O(n) 의 시간복잡도(n : distance of range) 를 가져야 한다. 따라서 `end()` 를 hint 로 하는 `insert` 를 내부에서 호출하여 탐색을 O(1) 로 줄인다. 삽입을 n 번 반복하므로 O(n) 이 된다.

hint 없이 key 만 아는 single element insert - `find_insert_pos`

```cpp
/**
 * @brief find parent's position to insert new node
 *
 * @param key
 * @return pair<base_ptr, base_ptr>
 * success - first : null, second : parent
 * fail - first : equal node, second : null
 */
pair<base_ptr, base_ptr> _find_insert_pos(const key_type& key) {
  typedef pair<base_ptr, base_ptr> pair_type;

  base_ptr x = _get_root();
  base_ptr y = &_impl._header;
  bool comp = true;

  while (x != NULL) { // 가장 key 와 유사한 노드를 찾는다.
    y = x;
    comp = _impl._compare(key, _get_key(x));
    x = comp ? x->left : x->right;
		// comp 가 true 이면 x 값이 더 크므로 더 작은 node 를 찾기위해 left. (key < x.key)
		// x가 같거나 작으면 right. (key >= x.key)
  }
	// loop 를 탈출한 상황이라면 유사한 노드를 찾고 y가 말단 노드를 가리키고 있다.
  iterator tmp(y);
  if (comp) {              // key < y.key - 마지막 이동이 left - y 와 같지 않음.
    if (tmp == begin()) {  // leftmost 이면 왼쪽으로만 왔으므로 같을 일이 없다.
      return pair_type(x, y);
    }
    --tmp; // 직전 노드를 찾아 key 와 비교
  }
	// comp == true) tmp < key < y
	// comp == false) y < key ( y 가 부모 )
  if (_impl._compare(KeyOfValue()(*tmp), key)) {
    return pair_type(x, y);
  }
  // tmp == key
  return pair_type(tmp._node, NULL);
}
```

- 트리의 높이만큼 연산하므로 O(log n).

hint 가 주어진 with hint insert - `find_insert_hint_pos`

```cpp
/**
 * @brief position 과 최대한 가까운 위치에 있는 node 를 찾아서 반환.
 *
 * @param position
 * @param key
 * @return pair<base_ptr, base_ptr>
 * success -  만약 왼쪽 삽입이 확실하면 first 가 parent, 아니면 NULL.
 * fail - first : equal node, second : null
 */
pair<base_ptr, base_ptr> _find_insert_hint_pos(const_iterator position,
                                               const key_type& key) {
  typedef pair<base_ptr, base_ptr> pair_type;
  iterator pos = position._const_cast();

  if (pos._node == &_impl._header) {  // end
    if (size() > 0 && _impl._compare(_get_key(_get_right_most()), key)) {
      // key가  rightmost 보다 크다면 rightmost 뒤에 삽입
      return pair_type(NULL, _get_right_most());
    } else {
      // 아니면 허위사실이므로 새로 찾아야 함
      return _find_insert_pos(key);
    }
  } else if (_impl._compare(key, _get_key(pos._node))) {
    // key < pos -> pos 의 왼쪽에 삽입
    iterator before = pos;
    if (pos._node == _get_left_most()) {  // begin()
      // pos 가 left most 이면 left most 앞에 삽입.
      return pair_type(_get_left_most(), _get_left_most());
    } else if (_impl._compare(_get_key((--before)._node), key)) {
      // before < key < pos
      if (_right(before._node) == NULL) {
        // before 의 right 가 없으면 before 자식으로 삽입 (오른쪽)
        return pair_type(NULL, before._node);
      } else {
        // before 의 right 가 있으므로 pos 가 before 의 right. (확실함..)
        // before 가 pos-- 이고 만약 pos 의 왼쪽 자식이 있었으면 before 가
        // pos->left 였을 것이다.
        // pos._node != NULL 이므로 pos 의 왼편에 들어갈 것이다.
        return pair_type(pos._node, pos._node);
      }
    } else {
      return _find_insert_pos(key);
    }
  } else if (_impl._compare(_get_key(pos._node), key)) {
		// 위와 방향 제외 거의 동일하다.
		// ...
  } else {
    // pos == key
    return pair_type(pos._node, NULL);
  }
}
```

- right most 뒤에 삽입하는 경우, left most 앞에 삽입하는 경우는 위치 찾는 시간이 O(1) 으로 감소한다.  hint의 바로 왼쪽 혹은 오른쪽에 들어갈 경우 pos 를  -- 하거나 ++ 하는 과정에서의 수행 시간은 상수 ~ log n 이기 때문에 pos 가 적절히 들어오면 시간복잡도는 amortized constant 이다.
- hint 가 도움이 되지 않는 경우 `_find_insert_pos`로 single element `insert` 와 동일한 방법으로 위치를 찾는다.

```cpp
void _insert_rebalance(bool left, _rb_tree_node_base* x, _rb_tree_node_base* p,
                       _rb_tree_node_base& header) {
  _rb_tree_node_base*& root = header.parent;
  x->_node_init(p);

  // insert
  // root, leftmost, rightmost 를 갱신해준다.
  // tree 가 empty 라면 left 이다.
  if (left) {
    p->left = x;         // also makes leftmost = x when p == header
    if (p == &header) {  // root 자리에 들어갈 경우
      header.parent = x;
      header.right = x;
      x->color = BLACK;
    } else if (p == header.left) {  // leftmost update
      header.left = x;
    }
  } else {
    p->right = x;
    if (p == header.right) {  // rightmost update
      header.right = x;
    }
  }
  // rotate
  while (x != root && x->parent->color == RED) {  // header -> no pp
    _rb_tree_node_base* const xpp = x->parent->parent;
    if (x->parent == xpp->left) {                 // 부모가 왼쪽 자식임
      _rb_tree_node_base* const xu = xpp->right;  // uncle
      if (xu && xu->color == RED) {
        // case 1
        // 삼촌과 부모가 빨간색, 나는 왼쪽 자식 -> color 교체만 해주면 됨
        // 이미 할아버지는 BLACK 인 게 확실함 (rb tree 의 성질)
        x->parent->color = BLACK;
        xu->color = BLACK;
        xpp->color = RED;
        x = xpp;  // 다시 확인할 노드를 할아버지 (xpp) 로 바꿔줌
      } else {  // 부모가 왼쪽, 삼촌이 없거나 (이것도 BLACK) 삼촌이 검은색
        if (x == x->parent->right) {  // 나 오른쪽 자식
                                      // case 2
          x = x->parent;
          // 부모 기준으로 왼쪽 회전 후 case 3으로 넘어감
          _rotate_left(x, root);
        }
        // case 3
        // 부모와 할아버지의 색을 바꾸고 할아버지 기준으로 오른쪽 회전.
        x->parent->color = BLACK;
        xpp->color = RED;
        _rotate_right(xpp, root);
      }
    } else {
      _rb_tree_node_base* const xu = xpp->left;
      if (xu && xu->color == RED) {
        x->parent->color = BLACK;
        xu->color = BLACK;
        xpp->color = RED;
        x = xpp;
      } else {
        if (x == x->parent->left) {
          x = x->parent;
          _rotate_right(x, root);
        }
        x->parent->color = BLACK;
        xpp->color = RED;
        _rotate_left(xpp, root);
      }
    }
  }
  root->color = BLACK;  // while 을 바로 빠져나온 경우를 위해 setting.
}
```

- 삽입된 노드 `x` 가 RED 이므로 `x->parent` 가 RED 이면 4번 규칙 (RED 노드는 연속해서 올 수 없음) 을 위반하게 된다.
- `x`, 의 부모 `xp`, 의 할아버지 `xpp`, 의 삼촌 `xu` 을 확인해서 recoloring & rotating 으로 red-black 트리의 속성을 지키도록 트리를 변경한다.

### erase

```cpp
void erase(iterator position) { _erase(position._node); }

size_type erase(const key_type& key) {
  pair<iterator, iterator> range = equal_range(key);
  size_type count = 0;
  while (range.first != range.second) {
    erase(range.first++);
    ++count;
  }
  return count;
}
```

- `erase` 후 iterator가 무효화 되는 것에 유의해야 한다.
- while loop 안의 `erase` 가 호출되기 전 먼저 iterator 의 후위 ++ 연산자를 호출해서 옮기고 옮기기 전의 리턴값으로 `erase` 를 호출하여 iterator 를 이동시키면서 삭제할 수 있다.

```cpp
_rb_tree_node_base* _rebalance_for_erase(_rb_tree_node_base* const z,
                                         _rb_tree_node_base& header) {
  _rb_tree_node_base*& root = header.parent;
  _rb_tree_node_base*& leftmost = header.left;
  _rb_tree_node_base*& rightmost = header.right;

  _rb_tree_node_base* y = z;
  _rb_tree_node_base* x = NULL;
  _rb_tree_node_base* xp = NULL;

  // 자식이 0 또는 1개인 경우 y 는 z 이고, 2개인 경우 y 는 z 의 successor
  if (y->left == NULL) {  // z 왼쪽 자식 없음. 오른쪽 자식은 모름
    x = y->right;         // NULL 일 수도 있다.
  } else if (y->right == NULL) {  // z 왼쪽 자식 있음, 오른쪽 없음.
    x = y->left;
  } else {  // z 의 자식이 둘 다 있음
    y = _get_subtree_min(z->right);
    x = y->right;
  }

  // z 가 leaf 이거나 오른쪽만 있으면 x 가 null 이다.
  // 이 경우 y 는 z 와 같다.

  // z 없앨 준비 하는 과정
  if (y != z) {
    // y가 z의 successor 인 경우 relink y in place of z.
    z->left->parent = y;
    y->left = z->left;  // y의 왼쪽 자식을 z의 왼쪽 자식으로 바꿔줌
    if (y != z->right) {
      // x 가 y 자리에 갈 것이므로 xp를 y 의 부모로 세팅해야 한다.
      xp = y->parent;  // y의 부모를 xp에 저장 -> 어차피 y 자리에 x를 넣을 것
      if (x != NULL) {
        x->parent = y->parent;
      }
      y->parent->left = x;
      y->right = z->right;
      z->right->parent = y;
    } else {
      // z 가 y 의 부모인 경우 x.p 가 z 이면 유감이므로 y 를 x.p 로 세팅.
      // 어차피 결과적으로 y는 x의 부모가 된다.
      xp = y;
    }
    // 부모와 연결
    if (root == z) {
      root = y;
    } else if (z->parent->left == z) {
      z->parent->left = y;
    } else {
      z->parent->right = y;
    }
    y->parent = z->parent;

    swap(y->color, z->color);  // y 가 z 색 상속
    y = z;                     // 실제 없어질 노드 -> z
  } else {                     // y == z
    // z의 자식이 0 또는 1개인 경우 -> x 가 z 자리 바로 대체함.
    xp = z->parent;
    if (x != NULL) {
      x->parent = z->parent;
    }
    if (root == z) {
      root = x;
    } else if (z->parent->left == z) {
      z->parent->left = x;
    } else {
      z->parent->right = x;
    }
    // 자식이 있으면 이럴 일이 없다.
    if (leftmost == z) {
      if (z->right == NULL) {
        leftmost = z->parent;
      } else {
        leftmost = _get_subtree_min(x);
      }
    }
    if (rightmost == z) {
      if (z->left == NULL) {
        rightmost = z->parent;
      } else {
        rightmost = _get_subtree_max(x);
      }
    }
  }
	// black 을 삭제하게 되면 black 노드 개수가 하나 사라지므로 rb tree 특성이 깨진다.
	// 따라서 x 노드에 black 을 부여하여 해결한다.

  // doubly black 해결 과정
  if (y->color != RED) {  // 삭제될 node 의 color 판별
    while (x != root && (x == NULL || x->color == BLACK)) {
      // red -> black 제거해서 black 설정하면 됨.
      // root -> extra black 제거하면 됨.
      // x is always doubly black node which is non-root
      if (x == xp->left) {
        _rb_tree_node_base* w = xp->right;  // sibling of x
        if (w->color == RED) {              // case 1
          w->color = BLACK;
          xp->color = RED;
          _rotate_left(xp, root);
          w = xp->right;  // update sibling to new sibling after rotate
        }
        if ((w->left == NULL || w->left->color == BLACK) &&
            (w->right == NULL || w->right->color == BLACK)) {
          // case 2 (both children of w are black)
          w->color = RED;
          x = xp;  // 대상 노드 (extra black 이 추가된 black node)를 부모로 갱신
          xp = xp->parent;
        } else {
          // case 3, 4
          if (w->right == NULL || w->right->color == BLACK) {
            // case 3
            // w->left 와 w 의 색을 바꾼다.
            w->left->color = BLACK;
            w->color = RED;
            _rotate_right(w, root);
            w = xp->right;  // w 새로 설정. (rotate 했기 때문)
            // case 4로 이어짐
          }
          // case 4
          w->color = xp->color;
          xp->color = BLACK;
          if (w->right != NULL) {
            w->right->color = BLACK;
          }
          _rotate_left(xp, root);
          break;
        }
      } else {
				// 방향만 다르고 위와 똑같다.
        _rb_tree_node_base* w = xp->left;
        if (w->color == RED) {
          w->color = BLACK;
          xp->color = RED;
          _rotate_right(xp, root);
          w = xp->left;
        }
        if ((w->right == NULL || w->right->color == BLACK) &&
            (w->left == NULL || w->left->color == BLACK)) {
          w->color = RED;
          x = xp;
          xp = xp->parent;
        } else {
          if (w->left == NULL || w->left->color == BLACK) {
            w->right->color = BLACK;
            w->color = RED;
            _rotate_left(w, root);
            w = xp->left;
          }
          w->color = xp->color;
          xp->color = BLACK;
          if (w->left != NULL) {
            w->left->color = BLACK;
          }
          _rotate_right(xp, root);
          break;
        }
      }
    }
    if (x != NULL) {
      x->color = BLACK;  // red-and-black or root
    }
  }
  return y;
}
```

### find

```cpp
/**
 * @brief key 에 해당하는 노드를 찾는다.
 *
 * @param k key
 * @return iterator 찾으면 찾은 iterator, 못찾으면 end.
 */
iterator find(const key_type& key) {
  iterator gte = lower_bound(key);
  // 1. gte 가 end 이면 크거나 같은 키가 없음.
  // 2. 1 에서 || 이므로 key <= gte.
  // 거기서 key < gte 가 true 이면 같은 것이 아니라 더 큰 것이므로 end return.
  return (gte == end() || _impl._compare(key, KeyOfValue()(*gte))) ? end()
                                                                   : gte;
}
```

---

## iterator

map 과 set 의 iterator 는 bidirectional 이므로 bidirectional iterator 를 구현한다.

[C++ named requirements: LegacyBidirectionalIterator](https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator)

[iterator & reverse_iterator](https://www.notion.so/iterator-reverse_iterator-a298e284f797453fa2fb56d46d071892)

1. operator ++
    - rb tree + 1 -> 오른쪽 서브트리에서 가장 작은 값
    - 오른쪽 서브트리가 없으면
        - 내가 부모의 왼쪽 자식 -> 부모
        - 내가 부모의 오른쪽 자식 -> 왼쪽 자식이 될 때 까지 부모 찾기

    ```jsx
    _rb_tree_node_base* _rb_tree_increment(_rb_tree_node_base* x) {

      if (x->right != 0) {
        return _rb_tree_subtree_min(x->right);
      } else {
        _rb_tree_node_base* xp = x->parent;
        while (x == xp->right) {  // x가 xp의 right child
          x = xp;
          xp = xp->parent;
        }
        // 왼쪽 자식이 될 때 까지 트리 등반. y가 부모, x가 왼쪽 자식
        // 종료 조건이 x != xp->right 이므로 두 가지가 있다.
        // 1. x 가 xp의 왼쪽 자식
        // 2. x 가 root->parent(header) (더 이상 자식이 없음)
        // x 가 root 였다면 xp 는 header 가 된다.
        // header 의 right 는 right most 이므로 노드가 하나 뿐인 tree 에서
        // x (root) == xp->right (right most : root) 가 true 가 되어
        // 반복문에서 x 가 header, xp가 header->parent (root) 가 된다.
        // root 를 리턴하면 큰일나므로 header 를 리턴해준다.

        // x-> right 가 header->right (right most), xp 가 root => root.
        if (x->right != xp) x = xp;
      }
      return x;
    }
    ```

2. opeartor--

    rb tree + 1 -> 왼쪽 서브트리에서 가장 큰 값

    - 왼쪽 서브트리가 없으면
        - 내가 부모의 오른쪽 자식 -> 부모
        - 내가 부모의 왼쪽 자식 -> 오른쪽 자식이 될 때 까지 부모 찾기

    ```cpp
    _rb_tree_node_base* _rb_tree_decrement(_rb_tree_node_base* x) {
      if (x->color == RED && x->parent->parent == x) {  // header node (end node)
        x = x->right;                                   // 바로 right most return
      } else if (x->left != 0) {
        return _rb_tree_subtree_max(x->left);  // max of left subtree
      } else {
        _rb_tree_node_base* xp = x->parent;
        while (x == xp->left) {
          x = xp;
          xp = xp->parent;
        }
        x = xp;
      }
      return x;
    }
    ```

    - Header node (end) 일 경우는 right most 인 게 확실하므로 right 리턴.
