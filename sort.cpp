#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <iterator>

/******************************************************************************/
/*                             vector operations                              */
/******************************************************************************/

std::ostream& operator<<(std::ostream& os, std::vector<int> vec) {
  os << "[";
  for (int elt : vec) {
    os << " " << elt;
  }
  os << " ]";
  return os;
}

std::vector<int> operator+(const std::vector<int> &lhs, const std::vector<int> &rhs) {
  std::vector<int> result = lhs;
  std::copy(rhs.begin(), rhs.end(), std::back_inserter(result));
  return result;
}

std::vector<int> operator+(const std::vector<int> &lhs, int rhs) {
  std::vector<int> result = lhs;
  result.push_back(rhs);
  return result;
}

/******************************************************************************/
/*                               selection sort                               */
/******************************************************************************/

size_t indice_min(const std::vector<int> &L, size_t index) {
  if (index >= L.size())
    throw std::out_of_range("Error (indice_min): index out of bounds.");

  int min = L[index];
  int i_min = index;

  for (size_t i = index + 1; i < L.size(); ++i) {
    if (L[i] < min) {
      min = L[i];
      i_min = i;
    }
  }

  return i_min;
}

std::vector<int> tri_sel(std::vector<int> L) {
  size_t i_min;
  int tmp;

  for (size_t i = 0; i < L.size(); ++i) {
    i_min = indice_min(L, i);
    tmp = L[i_min];
    L[i_min] = L[i];
    L[i] = tmp;
  }
  return L;
}

void test_selection_sort() {
  std::vector<int> TestVec = { 2, 9, 0, 6, 1, 4 };

  std::cout << "test_selection_sort -------------------------------------------"
    << std::endl;

  // Index min
  std::cout << "indice_min (2): " << indice_min(TestVec, 2) << std::endl;
  std::cout << "indice_min (3): " << indice_min(TestVec, 3) << std::endl;

  // sort
  tri_sel(TestVec);
  std::cout << "tri_sel: " << TestVec << std::endl;
}

/******************************************************************************/
/*                               insertion sort                               */
/******************************************************************************/

void insert_i(int x, std::vector<int> &L, size_t index) {
  if (index > L.size())
    throw std::out_of_range("Error (insert_i): index out of bounds.");

  L.push_back(0);
  std::copy(L.begin() + index, L.end() - 1, L.begin() + index + 1);
  L[index] = x;
}

void insert(int x, std::vector<int> &L_triee) {
  size_t position = L_triee.size(), i = 0;

  // finding the position
  while (i < L_triee.size() && position >= L_triee.size()) {
    if (L_triee[i] > x) {
      position = i;
    }
    ++i;
  }
  // insertion
  insert_i(x, L_triee, position);
}

/**
 * Insert x int L_triee after start.
 * NOTE: here L_triee[start:] is a sorted list and x = L_triee[start - 1]
 */
void insert(int x, std::vector<int> &L_triee, size_t start) {
  if (start > L_triee.size())
    throw std::out_of_range("Error (insert): start out of bounds.");

  size_t position = L_triee.size(), i = start;

  // finding the position
  while (i < L_triee.size() && position >= L_triee.size()) {
    if (L_triee[i] > x) {
      position = i;
    }
    ++i;
  }
  // insertion
  // L[start - 1] is the current position of x, so we move all the value to the
  // left to insert x.
  for (i = start - 1; i < position - 1; ++i) {
    L_triee[i] = L_triee[i + 1];
  }
  L_triee[position - 1] = x;
}

/**
 * simple version using a temporary array
 */
std::vector<int> tri_insert(std::vector<int> L) {
  std::vector<int> tmp;

  for (size_t i = 0; i < L.size(); ++i) {
    insert(L[i], tmp);
  }
  std::copy(tmp.begin(), tmp.end(), L.begin());
  return L;
}

/**
 * same as tri_insert but this time, we don't use a temporary array
 */
std::vector<int> tri_insert_inplace(std::vector<int> L) {
  for (size_t i = L.size() - 1; i > 0; --i) {
    insert(L[i - 1], L, i);
  }
  return L;
}

void test_insertion_sort() {
  std::vector<int> TestVec = { 2, 9, 0, 6, 1, 4 };
  std::vector<int> TestVec2 = { 2, 9, 0, 6, 1, 4, 4, 17, 12 };
  std::vector<int> SortedTestVec = { 1, 4, 5, 9, 11 };

  std::cout << "test_insertion_sort -------------------------------------------"
    << std::endl;

  insert_i(15, TestVec, TestVec.size());
  std::cout << "test insert_i: " << TestVec << std::endl;
  insert(7, SortedTestVec);
  std::cout << "test insert: " << SortedTestVec << std::endl;
  tri_insert(TestVec);
  std::cout << "test tri_insert: " << TestVec << std::endl;
  tri_insert_inplace(TestVec2);
  std::cout << "test tri_insert_inplace: " << TestVec2 << std::endl;
}

/******************************************************************************/
/*                                 quick sort                                 */
/******************************************************************************/

std::vector<int> quicksort(std::vector<int> L) {
  int pivot;
  std::vector<int> inferiors;
  std::vector<int> superiors;

  if (L.size() == 0) {
    return L;
  } else {
    pivot = L.front();
    std::copy_if(L.begin() + 1, L.end(), std::back_inserter(inferiors), [pivot](int a) { return a < pivot; });
    std::copy_if(L.begin() + 1, L.end(), std::back_inserter(superiors), [pivot](int a) { return a >= pivot; });
    return quicksort(inferiors) + pivot + quicksort(superiors);
  }
}

void test_quicksort() {
  std::vector<int> TestVec2 = { 2, 9, 0, 6, 1, 4, 4, 17, 12 };

  std::cout << "test_quicksort ------------------------------------------------"
    << std::endl;
  std::cout << "quicksort: " << quicksort(TestVec2) << std::endl;
}

/******************************************************************************/
/*                                  measures                                  */
/******************************************************************************/

void generate_random_vector(std::vector<int> &out, int size) {
  srand(0);
  for (int i = 0; i < size; ++i) {
    out.push_back(rand() % 1000);
  }
}

unsigned long measure(std::function<std::vector<int>(std::vector<int>)> sort, std::vector<int> &L) {
  std::chrono::steady_clock::time_point begin;
  std::chrono::steady_clock::time_point end;

  begin = std::chrono::steady_clock::now();
  sort(L);
  end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}

/******************************************************************************/
/*                                    main                                    */
/******************************************************************************/

int main(int, char**)
{
  std::vector<int> TestVec;


  std::cout << "================================================== basic tests:"
    << std::endl;
  test_selection_sort();
  test_insertion_sort();
  test_quicksort();

  std::cout << "=============================================== measures tests:"
    << std::endl;

  generate_random_vector(TestVec, 1000000);

  std::cout << "time for tri_sel: " << measure(tri_sel, TestVec) / 1000000.0 <<  "s" << std::endl;
  std::cout << "time for tri_insert: " << measure(tri_sel, TestVec) / 1000000.0 <<  "s" << std::endl;
  std::cout << "time for tri_insert_inplace: " << measure(tri_sel, TestVec) / 1000000.0 << "s" << std::endl;
  std::cout << "time for quicksort: " << measure(tri_sel, TestVec) / 1000000.0 <<  "s" << std::endl;
  return 0;
}
