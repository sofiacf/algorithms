#include "./dijkstra.h"

#include <limits.h>
#include <stdlib.h>

#include "../utils/malloc_test_wrapper.h"
#include "../utils/test_helpers.h"
#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"

static const double kEpsilon = 0.00000000000001;

const int small_n = 5;
const char* small_path = "src/shortest_path/test_data/small.txt";

const int med_n = 9;
const char* med_path = "src/shortest_path/test_data/medium.txt";

const int big_n = 201;
const char* big_path = "src/shortest_path/test_data/big.txt";

/************************ Helpers *********************************************/
static double Distance(Vertex* v) {
  if (v == NULL || v->data == NULL) return INFINITY;

  ShortestPath* sp = v->data;
  return sp->distance;
}

static void NodeHasDistance(Vertex* v, double expected) {
  if (v == NULL || v->data == NULL) {
    CU_FAIL("v is NULL");
    return;
  }

  ShortestPath* sp = v->data;
  CU_ASSERT_DOUBLE_EQUAL(expected, sp->distance, kEpsilon);
}

Graph* sut;

#if !defined(NDEBUG)
#define SHORTEST_PATH_TESTS(ALGO)                                    \
  static void ALGO##_null_parameter() {                              \
    GraphResult result = ALGO(NULL, 1);                              \
    CU_ASSERT_EQUAL(Graph_kNullParameter, result);                   \
  }                                                                  \
                                                                     \
  static void ALGO##_invalid_vertex() {                              \
    Graph* sut = Graph_Create(5);                                    \
                                                                     \
    GraphResult result = ALGO(sut, 6);                               \
    CU_ASSERT_EQUAL(Graph_InvalidVertexId, result);                  \
                                                                     \
    result = ALGO(sut, 6);                                           \
    CU_ASSERT_EQUAL(Graph_InvalidVertexId, result);                  \
                                                                     \
    Graph_Destroy(sut, NULL);                                        \
  }                                                                  \
                                                                     \
  static void ALGO##_malloc_tester(void) {                           \
    GraphResult result = ALGO(sut, 1);                               \
    CU_ASSERT_EQUAL(Graph_kFailedMemoryAllocation, result);          \
  }                                                                  \
                                                                     \
  static void ALGO##_failed_malloc() {                               \
    sut = Graph_Create(5);                                           \
    FAILED_MALLOC_TEST({ ALGO##_malloc_tester(); });                 \
    Graph_Destroy(sut, NULL);                                        \
  }                                                                  \
                                                                     \
  static void ALGO##_small() {                                       \
    Graph* sut = Graph_WeightedFromFile(small_n, small_path);        \
    GraphResult result = ALGO(sut, 1);                               \
                                                                     \
    CU_ASSERT_EQUAL(Graph_kSuccess, result);                         \
    NodeHasDistance(sut->V[1], 0);                                   \
    NodeHasDistance(sut->V[2], 1);                                   \
    NodeHasDistance(sut->V[3], 6);                                   \
    NodeHasDistance(sut->V[4], 3);                                   \
                                                                     \
    Graph_Destroy(sut, free);                                        \
  }                                                                  \
                                                                     \
  static void ALGO##_medium() {                                      \
    Graph* sut = Graph_WeightedFromFile(med_n, med_path);            \
                                                                     \
    GraphResult result = ALGO(sut, 1);                               \
                                                                     \
    CU_ASSERT_EQUAL(Graph_kSuccess, result);                         \
    NodeHasDistance(sut->V[1], 0);                                   \
    NodeHasDistance(sut->V[2], 1);                                   \
    NodeHasDistance(sut->V[3], 2);                                   \
    NodeHasDistance(sut->V[4], 3);                                   \
    NodeHasDistance(sut->V[5], 4);                                   \
    NodeHasDistance(sut->V[6], 4);                                   \
    NodeHasDistance(sut->V[7], 3);                                   \
    NodeHasDistance(sut->V[8], 2);                                   \
                                                                     \
    Graph_Destroy(sut, free);                                        \
  }                                                                  \
                                                                     \
  static void ALGO##_large() {                                       \
    Graph* sut = Graph_WeightedFromFile(big_n, big_path);            \
                                                                     \
    GraphResult result = ALGO(sut, 1);                               \
    CU_ASSERT_EQUAL(Graph_kSuccess, result);                         \
    CU_ASSERT_EQUAL(Distance(sut->V[7]), 2599);                      \
    CU_ASSERT_EQUAL(Distance(sut->V[37]), 2610);                     \
    CU_ASSERT_EQUAL(Distance(sut->V[59]), 2947);                     \
    CU_ASSERT_EQUAL(Distance(sut->V[82]), 2052);                     \
    CU_ASSERT_EQUAL(Distance(sut->V[99]), 2367);                     \
    CU_ASSERT_EQUAL(Distance(sut->V[115]), 2399);                    \
    CU_ASSERT_EQUAL(Distance(sut->V[133]), 2029);                    \
    CU_ASSERT_EQUAL(Distance(sut->V[165]), 2442);                    \
    CU_ASSERT_EQUAL(Distance(sut->V[188]), 2505);                    \
    CU_ASSERT_EQUAL(Distance(sut->V[197]), 3068);                    \
                                                                     \
    Graph_Destroy(sut, free);                                        \
  }                                                                  \
                                                                     \
  CU_TestInfo ALGO##_tests[] = {CU_TEST_INFO(ALGO##_null_parameter), \
                                CU_TEST_INFO(ALGO##_invalid_vertex), \
                                CU_TEST_INFO(ALGO##_failed_malloc),  \
                                CU_TEST_INFO(ALGO##_small),          \
                                CU_TEST_INFO(ALGO##_medium),         \
                                CU_TEST_INFO(ALGO##_large),          \
                                CU_TEST_INFO_NULL};
#else
#define SHORTEST_PATH_TESTS(ALGO)                                    \
  static void ALGO##_null_parameter() {                              \
    GraphResult result = ALGO(NULL, 1);                              \
    CU_ASSERT_EQUAL(Graph_kNullParameter, result);                   \
  }                                                                  \
                                                                     \
  static void ALGO##_invalid_vertex() {                              \
    Graph* sut = Graph_Create(5);                                    \
                                                                     \
    GraphResult result = ALGO(sut, 6);                               \
    CU_ASSERT_EQUAL(Graph_InvalidVertexId, result);                  \
                                                                     \
    result = ALGO(sut, 6);                                           \
    CU_ASSERT_EQUAL(Graph_InvalidVertexId, result);                  \
                                                                     \
    Graph_Destroy(sut, NULL);                                        \
  }                                                                  \
                                                                     \
  static void ALGO##_small() {                                       \
    Graph* sut = Graph_WeightedFromFile(small_n, small_path);        \
    GraphResult result = ALGO(sut, 1);                               \
                                                                     \
    CU_ASSERT_EQUAL(Graph_kSuccess, result);                         \
    NodeHasDistance(sut->V[1], 0);                                   \
    NodeHasDistance(sut->V[2], 1);                                   \
    NodeHasDistance(sut->V[3], 6);                                   \
    NodeHasDistance(sut->V[4], 3);                                   \
                                                                     \
    Graph_Destroy(sut, free);                                        \
  }                                                                  \
                                                                     \
  static void ALGO##_medium() {                                      \
    Graph* sut = Graph_WeightedFromFile(med_n, med_path);            \
                                                                     \
    GraphResult result = ALGO(sut, 1);                               \
                                                                     \
    CU_ASSERT_EQUAL(Graph_kSuccess, result);                         \
    NodeHasDistance(sut->V[1], 0);                                   \
    NodeHasDistance(sut->V[2], 1);                                   \
    NodeHasDistance(sut->V[3], 2);                                   \
    NodeHasDistance(sut->V[4], 3);                                   \
    NodeHasDistance(sut->V[5], 4);                                   \
    NodeHasDistance(sut->V[6], 4);                                   \
    NodeHasDistance(sut->V[7], 3);                                   \
    NodeHasDistance(sut->V[8], 2);                                   \
                                                                     \
    Graph_Destroy(sut, free);                                        \
  }                                                                  \
                                                                     \
  static void ALGO##_large() {                                       \
    Graph* sut = Graph_WeightedFromFile(big_n, big_path);            \
                                                                     \
    GraphResult result = ALGO(sut, 1);                               \
    CU_ASSERT_EQUAL(Graph_kSuccess, result);                         \
    CU_ASSERT_EQUAL(Distance(sut->V[7]), 2599);                      \
    CU_ASSERT_EQUAL(Distance(sut->V[37]), 2610);                     \
    CU_ASSERT_EQUAL(Distance(sut->V[59]), 2947);                     \
    CU_ASSERT_EQUAL(Distance(sut->V[82]), 2052);                     \
    CU_ASSERT_EQUAL(Distance(sut->V[99]), 2367);                     \
    CU_ASSERT_EQUAL(Distance(sut->V[115]), 2399);                    \
    CU_ASSERT_EQUAL(Distance(sut->V[133]), 2029);                    \
    CU_ASSERT_EQUAL(Distance(sut->V[165]), 2442);                    \
    CU_ASSERT_EQUAL(Distance(sut->V[188]), 2505);                    \
    CU_ASSERT_EQUAL(Distance(sut->V[197]), 3068);                    \
                                                                     \
    Graph_Destroy(sut, free);                                        \
  }                                                                  \
                                                                     \
  CU_TestInfo ALGO##_tests[] = {CU_TEST_INFO(ALGO##_null_parameter), \
                                CU_TEST_INFO(ALGO##_invalid_vertex), \
                                CU_TEST_INFO(ALGO##_small),          \
                                CU_TEST_INFO(ALGO##_medium),         \
                                CU_TEST_INFO(ALGO##_large),          \
                                CU_TEST_INFO_NULL};
#endif

SHORTEST_PATH_TESTS(Graph_DijkstraShortestPathNaive)
SHORTEST_PATH_TESTS(Graph_DijkstraShortestPath)

int RegisterDijkstraTests() {
  CU_SuiteInfo suites[] = {{.pName = "Dijkstra Shortest Path - Naive",
                            .pInitFunc = noop,
                            .pCleanupFunc = noop,
                            .pTests = Graph_DijkstraShortestPathNaive_tests},
                           {.pName = "Dijkstra Shortest Path",
                            .pInitFunc = noop,
                            .pCleanupFunc = noop,
                            .pTests = Graph_DijkstraShortestPath_tests},
                           CU_SUITE_INFO_NULL};

  return CU_register_suites(suites);
}
