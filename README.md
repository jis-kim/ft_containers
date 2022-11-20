

# ft_containers
C++ containers, easy mode
# 구현사항
## vector
## map
## stack
- vector 기반 구현
## set (bonus)
- RB tree

## else
- `iterator_traits`
- `reverse_iterator`
- `enable_if (c++11)`
- `is_integral`
- `equal` and/or `lexicographical_compare`
- `std::pair`
- `std::make_pair`

## Exception-safety Guarantees
1.  **Basic guarantee for all operations**
    - stl의 기본 invariants 가 유지되고 memory 와 같은 리소스의 누수가 발생하지 않는다.
2. **Strong guarantee for key operations**
   - basic guarantee + operation 이 성공하거나 효과가 없다. key library operation에 제공된다. ( `push_back()`, `list`의 single element `insert()`, `uninitialized_copy()` )
3. **Nothrow guarantee for some operations**
   - basic guarantee + `exception` 을 던지지 않는다. 간단한 `swap(), pop_back()` 같은 operation 들에 제공된다.
