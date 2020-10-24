/*******************************************************************************
 * Copyright (C) 2020 Dale Alleshouse (AKA Hideous Humpback Freak)
 *  dale@alleshouse.net https://hideoushumpbackfreak.com/
 *
 * This file is subject to the terms and conditions defined in the 'LICENSE'
 * file, which is part of this source code package.
 ******************************************************************************/
#include "bloom_filter.h"

#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "hash_functions.h"
#include "malloc_test_wrapper.h"
#include "test_helpers.h"

#define SUT(code_block)                                             \
  {                                                                 \
    BloomFilter* sut = NULL;                                        \
    ResultCode result_code = BloomFilter_Create(bits, funcs, &sut); \
    CU_ASSERT_EQUAL(result_code, kSuccess);                         \
    code_block;                                                     \
    BloomFilter_Destroy(sut);                                       \
  }

static const size_t bits = 512;
static const size_t funcs = 7;

static void BloomFilter_Create_failed_malloc() {
  FAILED_MALLOC_TEST({
    BloomFilter* sut = NULL;
    ResultCode result_code = BloomFilter_Create(bits, funcs, &sut);
    CU_ASSERT_EQUAL(result_code, kFailedMemoryAllocation);
    CU_ASSERT_PTR_NULL(sut);
  });
}

static void BloomFilter_Create_invalid_bits() {
  // Less than 8
  BloomFilter* sut = NULL;
  ResultCode result_code = BloomFilter_Create(4, funcs, &sut);
  CU_ASSERT_PTR_NULL(sut);
  CU_ASSERT_EQUAL(result_code, kArgumentOutOfRange);

  // Not a power of 2
  sut = NULL;
  result_code = BloomFilter_Create(1050, funcs, &sut);
  CU_ASSERT_PTR_NULL(sut);
  CU_ASSERT_EQUAL(result_code, kArgumentOutOfRange);
}

static void BloomFilter_Create_invalid_funcs() {
  // 0 hash functions
  BloomFilter* sut = NULL;
  ResultCode result_code = BloomFilter_Create(bits, 0, &sut);
  CU_ASSERT_PTR_NULL(sut);
  CU_ASSERT_EQUAL(result_code, kArgumentOutOfRange);

  // more than available
  sut = NULL;
  result_code = BloomFilter_Create(bits, hasher_count + 1, &sut);
  CU_ASSERT_PTR_NULL(sut);
  CU_ASSERT_EQUAL(result_code, kArgumentOutOfRange);
}

static void BloomFilter_Create_happy_path() {
  SUT({ CU_ASSERT_PTR_NOT_NULL(sut); });
}

static void BloomFilter_Insert_null_parameter() {
  char* key = "life is pain";

  Result result = BloomFilter_Insert(NULL, key);
  CU_ASSERT_EQUAL(kNullParameter, result);

  SUT({
    result = BloomFilter_Insert(sut, NULL);
    CU_ASSERT_EQUAL(kNullParameter, result);
  });
}

static void BloomFilter_Insert_happy_path() {
  char* key = "life is love";

  SUT({
    Result result = BloomFilter_Insert(sut, key);
    CU_ASSERT_EQUAL(kSuccess, result);
  });
}

static void BloomFilter_Lookup_null_parameter() {
  char* key = "life is beauty";

  bool result = false;
  Result result_code = BloomFilter_Lookup(NULL, key, &result);
  CU_ASSERT_EQUAL(result_code, kNullParameter);
  CU_ASSERT_FALSE(result);

  SUT({
    result_code = BloomFilter_Lookup(sut, NULL, &result);
    CU_ASSERT_EQUAL(result_code, kNullParameter);
    CU_ASSERT_FALSE(result);
  });
}

static void BloomFilter_Lookup_not_found() {
  SUT({
    bool result;
    ResultCode result_code = BloomFilter_Lookup(sut, "not found", &result);
    CU_ASSERT_EQUAL(result_code, kSuccess);
    CU_ASSERT_FALSE(result);
  });
}

static void BloomFilter_Lookup_found() {
  char* key = "life is an illusion";
  SUT({
    ResultCode result_code = BloomFilter_Insert(sut, (void*)key);
    CU_ASSERT_EQUAL(result_code, kSuccess);

    bool result;
    result_code = BloomFilter_Lookup(sut, (void*)key, &result);
    CU_ASSERT_EQUAL(result_code, kSuccess);
    CU_ASSERT_TRUE(result);
  });
}

static void BloomFilter_Lookup_happy_path() {
  const size_t n = 10;
  char* exists[] = {"life",    "is",     "pain", "love",        "an illusion",
                    "a quest", "beauty", "shit", "complicated", "code"};
  char* not_found[] = {"401",    "not found", "vacant",   "empty", "blank",
                       "barren", "hollow",    "desolate", "void",  "deserted"};

  SUT({
    for (size_t i = 0; i < n; i++) BloomFilter_Insert(sut, exists[i]);

    for (size_t i = 0; i < n; i++) {
      bool result;
      ResultCode result_code = BloomFilter_Lookup(sut, exists[i], &result);
      CU_ASSERT_EQUAL(result_code, kSuccess);
      CU_ASSERT_TRUE(result);

      result_code = BloomFilter_Lookup(sut, not_found[i], &result);
      CU_ASSERT_EQUAL(result_code, kSuccess);
      CU_ASSERT_FALSE(result);
    }
  });
}

static void BloomFilter_GetN_null_parameter() {
  size_t result = BloomFilter_GetN(NULL);
  CU_ASSERT_EQUAL(result, 0);
}

static void BloomFilter_GetN_happy_path() {
  SUT({
    CU_ASSERT_EQUAL(BloomFilter_GetN(sut), 0);
    BloomFilter_Insert(sut, "random key");
    CU_ASSERT_EQUAL(BloomFilter_GetN(sut), 1);
    BloomFilter_Insert(sut, "another random key");
    CU_ASSERT_EQUAL(BloomFilter_GetN(sut), 2);
  });
}

int RegisterBloomFilterTests() {
  CU_TestInfo hash_table_tests[] = {
      CU_TEST_INFO(BloomFilter_Create_failed_malloc),
      CU_TEST_INFO(BloomFilter_Create_invalid_bits),
      CU_TEST_INFO(BloomFilter_Create_happy_path),
      CU_TEST_INFO(BloomFilter_Create_invalid_funcs),
      CU_TEST_INFO(BloomFilter_Insert_null_parameter),
      CU_TEST_INFO(BloomFilter_Insert_happy_path),
      CU_TEST_INFO(BloomFilter_Lookup_null_parameter),
      CU_TEST_INFO(BloomFilter_Lookup_not_found),
      CU_TEST_INFO(BloomFilter_Lookup_found),
      CU_TEST_INFO(BloomFilter_Lookup_happy_path),
      CU_TEST_INFO(BloomFilter_GetN_null_parameter),
      CU_TEST_INFO(BloomFilter_GetN_happy_path),
      CU_TEST_INFO_NULL};

  CU_SuiteInfo suites[] = {{.pName = "Bloom Filters",
                            .pInitFunc = noop,
                            .pCleanupFunc = noop,
                            .pTests = hash_table_tests},
                           CU_SUITE_INFO_NULL};

  return CU_register_suites(suites);
}
