# Dynamic Array

Data structure to represent a dynamic array of non-negative integer numbers.

To use the data structure, it is necessary to copy both the codes from the `src/sms_mo.cpp` and `src/dyn_array.cpp`.

The complexity analysis will assume that the size of the array is `n` and the largest value of the array is `N`.

## Declaration

Since Dynamic Array is a template, to declare it you have to specify its type. 

```c++
dyn_array<int> a;
```

It is also possible to initialize a Dynamic Array with `std::initializer_list`

```c++
template<typename T, typename SIZE_T = int, typename SET = sms_mo<T, true, SIZE_T>>
struct dyn_array;

// Example:
dyn_array<int> a = {3, 1, 2};
```

Note that the data structure **only works with integral types**, such as `int`, `char`, `long long`, `unsigned short`, etc.

It is also possible, through the second template parameter, to specify the type to use to store the size of the array. This is useful to save memory or to represent very large arrays (with size greater than 2^31 - 1).

## Simple operations

### swap

Swaps the contents of two Dynamic Arrays.

```c++
void swap(dyn_array&, dyn_array&);

// Example:
dyn_array<int> a, b;
swap(a, b);
```

**Complexity**: `O(1)`

### size

Returns the size (number of elements) of the array.

```c++
SIZE_T dyn_array::size();

// Example:
dyn_array<int> a = {3, 1, 2};
int sz = a.size(); // sz = 3
```

**Complexity**: `O(1)`

### clear

Removes all elements from the array.

```c++
void dyn_array::clear();

// Example:
dyn_array<int> a;
a.clear();
```

**Complexity**: `O(n)` 

### push_back

Appends an element to the end of the array. By using the second argument, it is possible to push an arbitrary number of copies of the same value to the end of the array.

**If the value is negative or the frequency is not positive, the behaviour is undefined.**

```c++
void dyn_array::push_back(T, SIZE_T = 1);

// Example:
dyn_array<int> a;
a.push_back(2); // now a = {2}
a.push_back(5, 3); // now a = {2, 5, 5, 5}
```

**Complexity**: `O(log(n))`

### [] operator

Access a given index of the array.

**This doesn't allow for changing the value. If the index is not in the range `[0, size())`, the behaviour is undefined.**

```c++
const T dyn_array::operator[](SIZE_T);

// Example:
dyn_array<int> a = {3, 1, 2};
int val = a[0]; // val = 3
```

**Complexity**: `O(log(n) + log(N))`

### update

Changes the value at a given index of the array.

**If the index is not in the range `[0, size())`, the behaviour is undefined.**

```c++
void dyn_array::update(SIZE_T, T);

// Example:
dyn_array<int> a = {3, 1, 2};
a.update(1, 5);
int val = a[1]; // val = 5
```

**Complexity**: `O(log(n) + log(N))`

### min

Returns the minimum value of the array.

**If the array is empty, the behaviour is undefined.**

```c++
T dyn_array::min();

// Example:
dyn_array<int> a = {3, 1, 2};
int mi = a.min(); // mi = 1
```

**Complexity**: `O(1)`

## Range operations

### split

Splits at a given index. The given Dynamic Array gets cleared, and after the operation it will contain the indices smaller than the given index. The remaining elements stay in the original array.

```c++
void dyn_array::split(SIZE_T, dyn_array&);

// Example:
dyn_array<int> a = {3, 1, 2, 5, 6, 2}, b;
a.split(3, b); // b = {3, 1, 2} and a = {5, 6, 2}
```

**Complexity**: `O(log(n) + log(N))`

### concat

Concatenates two arrays. The given array gets cleared, and the original array is extended to contain the elements of the given array.

```c++
void dyn_array::concat(dyn_array&);

// Example:
dyn_array<int> a = {3, 1, 2}, b = {5, 6, 2};
a.concat(b); // a = {3, 1, 2, 5, 6, 2} and b = {}
```

**Complexity**: `O(log(n))`

### reverse

Reverses the given sub-array.

**If the first index is greater than the second index, the behaviour is undefined.**

```c++
void dyn_array::reverse(SIZE_T, SIZE_T);

// Example:
dyn_array<int> a = {3, 1, 2, 5, 6, 2};
a.reverse(1, 3); // a = {3, 5, 2, 1, 6, 2}
```

**Complexity**: `O(log(n) + log(N))`

If the range is not specified, the entire array gets reversed.

```c++
void dyn_array::reverse();

// Example:
dyn_array<int> a = {3, 1, 2, 5, 6, 2};
a.reverse(); // a = {2, 6, 5, 2, 1, 3}
```

**Complexity**: `O(1)`

### sort

Sorts the given sub-array, in non-decreasing order. If the last parameter is passed as `true`, the sub-array is sorted in non-increasing order.

**If the first index is greater than the second index, the behaviour is undefined.**

```c++
void dyn_array::sort(SIZE_T, SIZE_T, bool = false);

// Example:
dyn_array<int> a = {3, 1, 2, 5, 6, 2};
a.sort(0, 3); // a = {1, 2, 3, 5, 6, 2}
a.sort(2, 5, true); // a = {1, 2, 6, 5, 3, 2}
```

**Complexity**: `O(log(n))` **amortized**

If the range is not specified, the entire array gets sorted, in non-decreasing order. If the last parameter is passed as `true`, the array is sorted in non-increasing order.

```c++
void dyn_array::sort(bool = false);

// Example:
dyn_array<int> a = {3, 1, 2, 5, 6, 2};
a.sort(); // a = {1, 2, 2, 3, 5, 6}
```

**Complexity**: `O(1)` **amortized**

### rmq

Returns the minimum over the values of the given sub-array.

**If the first index is greater than the second index, the behaviour is undefined.**

```c++
T dyn_array::rmq(SIZE_T, SIZE_T);

// Example:
dyn_array<int> a = {3, 1, 2, 5, 6, 2};
int mi = a.rmq(0, 3); // mi = 1
```

**Complexity**: `O(log(n) + log(N))`

## Copying

Copying the array costs time linear in the size of the array.

## Memory Consumption

At any point, a Dynamic Array of size `n` uses `O(n)` memory.
