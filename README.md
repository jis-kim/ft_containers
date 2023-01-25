

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
