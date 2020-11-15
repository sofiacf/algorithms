/*******************************************************************************
 * Copyright (C) 2020 Dale Alleshouse (AKA Hideous Humpback Freak)
 *  dale@alleshouse.net https://hideoushumpbackfreak.com/
 *
 * This file is subject to the terms and conditions defined in the 'LICENSE'
 * file, which is part of this source code package.
 ******************************************************************************/
#include "quick_sort.h"

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "sorting_test_helpers.h"
#include "test_helpers.h"

/************* quick sort ****************/
size_t comparisons = 0;

STANDARD_SORTING_TESTS(QuickSort)

static void QuickSort_large() { TestIntArray(100000, rev_arr); }

static void TestPivot(ChoosePivot choose_pivot) {
  int* arr = GenerateTestData();
  int* arr_cpy = Duplicate(arr, sizeof(int) * test_data_n);

  int result = QuickSortPivot(test_data_n, sizeof(int), arr, PIntComparator,
                              choose_pivot);

  qsort(arr_cpy, test_data_n, sizeof(int), PIntComparator);

  CU_ASSERT_EQUAL_FATAL(0, result);
  arrays_are_equal(test_data_n, sizeof(int), arr_cpy, arr);
  free(arr);
  free(arr_cpy);
}

static void QuickSort_pivot_on_zero_matches_c() { TestPivot(PivotOnZero); }

static void QuickSort_pivot_on_random_matches_c() { TestPivot(PivotOnRandom); }

static void QuickSort_pivot_on_last_matches_c() { TestPivot(PivotOnLast); }

static void QuickSort_pivot_on_median_matches_c() { TestPivot(PivotOnMedian); }

static void QuickSort_pivot_on_median_finds_first() {
  const int n = 6;
  const int expected = 0;
  const int arr[] = {3, 1, 2, 4, 5, 6};

  const int result = PivotOnMedian(n, sizeof(int), arr, PIntComparator);

  CU_ASSERT_EQUAL(expected, result);
}

static void QuickSort_pivot_on_median_finds_last() {
  const int n = 6;
  const int expected = 5;
  const int arr[] = {6, 1, 2, 4, 5, 3};

  const int result = PivotOnMedian(n, sizeof(int), arr, PIntComparator);

  CU_ASSERT_EQUAL(expected, result);
}

static void QuickSort_pivot_on_median_finds_middle() {
  const int n = 6;
  const int expected = 2;
  const int arr[] = {6, 1, 3, 4, 5, 2};

  const int result = PivotOnMedian(n, sizeof(int), arr, PIntComparator);

  CU_ASSERT_EQUAL(expected, result);
}

int RegisterExtraQuickSortTests() {
  CU_TestInfo quick_sort_tests[] = {
      CU_TEST_INFO(QuickSort_large),
      CU_TEST_INFO(QuickSort_pivot_on_zero_matches_c),
      CU_TEST_INFO(QuickSort_pivot_on_random_matches_c),
      CU_TEST_INFO(QuickSort_pivot_on_last_matches_c),
      CU_TEST_INFO(QuickSort_pivot_on_median_matches_c),
      CU_TEST_INFO(QuickSort_pivot_on_median_finds_first),
      CU_TEST_INFO(QuickSort_pivot_on_median_finds_middle),
      CU_TEST_INFO(QuickSort_pivot_on_median_finds_last),
      CU_TEST_INFO_NULL};

  CU_SuiteInfo suites[] = {{.pName = "QuickSort Extra",
                            .pInitFunc = noop,
                            .pCleanupFunc = noop,
                            .pTests = quick_sort_tests},
                           CU_SUITE_INFO_NULL};

  return CU_register_suites(suites);
}

/************* Partition ****************/
static void Partition_null_parameter() {
  ResultCode result_code = Partition(0, 0, NULL, NULL, NULL);
  CU_ASSERT_EQUAL(result_code, kNullParameter);
}

static void Partition_invalid_parameter() {
  int arr[] = {1, 2, 3, 4, 5, 6};
  size_t pivot_index;

  ResultCode result_code = Partition(0, 0, arr, PIntComparator, &pivot_index);
  CU_ASSERT_EQUAL(result_code, kArgumentOutOfRange);
}

static void Partition_does_not_chage_sorted() {
  const int n = 6;
  const int expected[] = {1, 2, 3, 4, 5, 6};
  int arr[] = {1, 2, 3, 4, 5, 6};
  size_t pivot_index;

  ResultCode result =
      Partition(n, sizeof(int), arr, PIntComparator, &pivot_index);

  CU_ASSERT_EQUAL(result, kSuccess);
  CU_ASSERT_EQUAL(pivot_index, 0);
  arrays_are_equal(n, sizeof(expected[0]), expected, arr);
}

static void Partition_QuickSort_pivot_on_center() {
  const int n = 6;
  const int expected[] = {1, 2, 3, 4, 5, 6};
  int arr[] = {4, 2, 3, 1, 5, 6};
  size_t pivot_index;

  int result = Partition(n, sizeof(int), arr, PIntComparator, &pivot_index);

  CU_ASSERT_EQUAL(result, 0);
  CU_ASSERT_EQUAL(pivot_index, 3);
  arrays_are_equal(n, sizeof(expected[0]), expected, arr);
}

static void Partition_reverse_sorted() {
  const int n = 6;
  const int expected[] = {1, 5, 4, 3, 2, 6};

  int arr[] = {6, 5, 4, 3, 2, 1};
  size_t pivot_index;

  int result = Partition(n, sizeof(int), arr, PIntComparator, &pivot_index);

  CU_ASSERT_EQUAL(result, 0);
  CU_ASSERT_EQUAL(pivot_index, 5);
  arrays_are_equal(n, sizeof(expected[0]), expected, arr);
}

static void Partition_first_in_correct_position() {
  const int n = 5;
  const int expected[] = {1, 5, 4, 3, 2};

  int arr[] = {1, 5, 4, 3, 2};
  size_t pivot_index;

  int result = Partition(n, sizeof(int), arr, PIntComparator, &pivot_index);

  CU_ASSERT_EQUAL(result, 0);
  CU_ASSERT_EQUAL(pivot_index, 0);
  arrays_are_equal(n, sizeof(expected[0]), expected, arr);
}

int RegisterPartitionTests() {
  CU_TestInfo partition_tests[] = {
      CU_TEST_INFO(Partition_null_parameter),
      CU_TEST_INFO(Partition_invalid_parameter),
      CU_TEST_INFO(Partition_does_not_chage_sorted),
      CU_TEST_INFO(Partition_QuickSort_pivot_on_center),
      CU_TEST_INFO(Partition_reverse_sorted),
      CU_TEST_INFO(Partition_first_in_correct_position),
      CU_TEST_INFO_NULL};

  CU_SuiteInfo suites[] = {{.pName = "Partition tests",
                            .pInitFunc = noop,
                            .pCleanupFunc = noop,
                            .pTests = partition_tests},
                           CU_SUITE_INFO_NULL};

  return CU_register_suites(suites);
}
