#include <stdlib.h>
#include <string.h>

#include "include/OverflowChecker.h"

#include "WeightedIndependentSet.h"

const WeightedIndependentSet EMPTY_SET = {
    .n = 0, .weight = 0, .vertices = NULL};

static Result _weightedIndependentSet_Init(const size_t n, const int weight,
                                           WeightedVertex* vertices[],
                                           WeightedIndependentSet** wis) {
  if (wis == NULL || vertices == NULL) return NullParameter;

  (*wis) = calloc(sizeof(WeightedIndependentSet), 1);
  if ((*wis) == NULL) return FailedMemoryAllocation;

  (*wis)->n = n;
  (*wis)->weight = weight;
  (*wis)->vertices = vertices;

  return Success;
}

static Result _weightedIndependentSetFromVertex(
    WeightedVertex* vertex, WeightedIndependentSet** result) {
  WeightedVertex** new_vertices = malloc(sizeof(void*));
  if (new_vertices == NULL) return FailedMemoryAllocation;

  *new_vertices = vertex;
  Result r =
      _weightedIndependentSet_Init(1, vertex->weight, new_vertices, result);
  if (r != Success) free(new_vertices);

  return r;
}

static Result WeightedIndependentSet_Union(
    WeightedIndependentSet* wis, WeightedVertex* wv,
    WeightedIndependentSet** conclusion) {
  WeightedVertex** vertices = calloc(sizeof(WeightedVertex*), wis->n + 1);
  if (vertices == NULL) return FailedMemoryAllocation;

  if (wis != &EMPTY_SET)
    memcpy(vertices, wis->vertices, sizeof(void*) * wis->n);

  vertices[wis->n] = wv;

  // This should never overflow because it's checked by the caller
  int new_weight = wis->weight + wv->weight;
  Result result = _weightedIndependentSet_Init(wis->n + 1, new_weight, vertices,
                                               conclusion);

  if (result != Success) free(vertices);

  return result;
}

void WeightedIndependentSet_Destroy(WeightedIndependentSet* wis) {
  if (wis == NULL || wis == &EMPTY_SET) return;

  WeightedIndependentSet* _wis = wis;
  free(_wis->vertices);
  free(wis);
}

static Result _wisResursive(WeightedIndependentSet** wis,
                            WeightedVertex* vertices[], const size_t n) {
  if (n == 0) {
    (*wis) = (WeightedIndependentSet*)&EMPTY_SET;
    return Success;
  }

  if (n == 1) return _weightedIndependentSetFromVertex(vertices[0], wis);

  WeightedIndependentSet* wis1 = NULL;
  Result result1 = _wisResursive(&wis1, vertices, n - 1);
  if (result1 != Success) {
    WeightedIndependentSet_Destroy(wis1);
    return result1;
  }

  WeightedIndependentSet* wis2 = NULL;
  Result result2 = _wisResursive(&wis2, vertices, n - 2);
  if (result2 != Success) {
    WeightedIndependentSet_Destroy(wis1);
    WeightedIndependentSet_Destroy(wis2);
    return result2;
  }

  if (is_add_overflow_uint(wis2->weight, vertices[n - 1]->weight)) {
    WeightedIndependentSet_Destroy(wis1);
    WeightedIndependentSet_Destroy(wis2);
    return ArithmeticOverflow;
  }

  // overflow is checked in the block above
  if (wis1->weight >= wis2->weight + vertices[n - 1]->weight) {
    WeightedIndependentSet_Destroy(wis2);
    (*wis) = wis1;
    return Success;
  } else {
    WeightedIndependentSet* final = NULL;
    Result result = WeightedIndependentSet_Union(wis2, vertices[n - 1], &final);
    WeightedIndependentSet_Destroy(wis1);
    WeightedIndependentSet_Destroy(wis2);

    (*wis) = final;
    return result;
  }
}

static Result _dupWeightedIndependentSet(WeightedIndependentSet* input,
                                         WeightedIndependentSet** new_set) {
  if (input == &EMPTY_SET) {
    *new_set = (WeightedIndependentSet*)&EMPTY_SET;
    return Success;
  }

  *new_set = malloc(sizeof(WeightedIndependentSet));
  if (*new_set == NULL) return FailedMemoryAllocation;

  WeightedVertex** vertices = calloc(sizeof(WeightedVertex*), input->n + 1);
  if (vertices == NULL) return FailedMemoryAllocation;

  memcpy(vertices, input->vertices, sizeof(void*) * input->n);

  (*new_set)->n = input->n;
  (*new_set)->vertices = vertices;
  (*new_set)->weight = input->weight;

  return Success;
}

static void _solutionsDestroy(WeightedIndependentSet** solutions,
                              const size_t n) {
  for (size_t i = 0; i < n; i++) WeightedIndependentSet_Destroy(solutions[i]);
}

static Result _reconstruct(unsigned int solutions[], PathGraph* graph,
                           WeightedIndependentSet** result) {
  WeightedIndependentSet* winner = (WeightedIndependentSet*)&EMPTY_SET;
  size_t i = graph->n;
  while (i >= 2) {
    WeightedVertex* w = graph->vertices[i - 1];
    if (solutions[i - 1] > solutions[i - 2] + w->weight)
      i = i - 1;
    else {
      WeightedIndependentSet* new;
      Result r = WeightedIndependentSet_Union(winner, w, &new);
      WeightedIndependentSet_Destroy(winner);
      if (r != Success) return r;

      winner = new;
      i = i - 2;
    }

    if (i == 1) {
      WeightedIndependentSet* new;
      Result r = WeightedIndependentSet_Union(winner, graph->vertices[0], &new);
      WeightedIndependentSet_Destroy(winner);
      if (r != Success) return r;

      winner = new;
    }
  }

  (*result) = winner;

  return Success;
}

Result PathGraph_Create(unsigned int ids[], int weights[], size_t n,
                        PathGraph** graph) {
  if (ids == NULL || weights == NULL || graph == NULL) return NullParameter;

  PathGraph* _graph = malloc(sizeof(PathGraph));
  if (_graph == NULL) return FailedMemoryAllocation;

  _graph->vertices = calloc(sizeof(void*), n);
  if (_graph->vertices == NULL) {
    PathGraph_Destroy(_graph);
    return FailedMemoryAllocation;
  }

  _graph->n = n;

  for (size_t i = 0; i < n; i++) {
    WeightedVertex* wv = malloc(sizeof(WeightedVertex));
    if (wv == NULL) {
      PathGraph_Destroy(_graph);
      return FailedMemoryAllocation;
    }

    wv->id = ids[i];
    wv->weight = weights[i];
    _graph->vertices[i] = wv;
  }

  *graph = _graph;
  return Success;
}

void PathGraph_Destroy(void* graph) {
  if (graph == NULL) return;
  PathGraph* _graph = graph;

  if (_graph->vertices != NULL) {
    for (size_t i = 0; i < _graph->n; i++) free(_graph->vertices[i]);
  }

  free(_graph->vertices);
  free(_graph);
}

Result WeightedVertex_Init(unsigned int id, const int weight,
                           WeightedVertex** vw) {
  (*vw) = malloc(sizeof(WeightedVertex));
  if (vw == NULL) return FailedMemoryAllocation;

  (*vw)->weight = weight;
  (*vw)->id = id;
  return Success;
}

void WeightedVertex_Destroy(WeightedVertex* vw) { free(vw); }

Result WeightedIndependentSet_Recursive(PathGraph* graph,
                                        WeightedIndependentSet** conclusion) {
  if (graph == NULL || conclusion == NULL) return NullParameter;

  return _wisResursive(conclusion, graph->vertices, graph->n);
}

Result WeightedIndependentSet_Dynamic_Reconstruction(
    PathGraph* graph, WeightedIndependentSet** conclusion) {
  if (graph == NULL || conclusion == NULL) return NullParameter;

  // Pass in an empty set and you deserve to get one back
  if (graph->n == 0) {
    (*conclusion) = (WeightedIndependentSet*)&EMPTY_SET;
    return Success;
  }

  // If there is only 1 vertex, return it
  if (graph->n == 1)
    return _weightedIndependentSetFromVertex(graph->vertices[0], conclusion);

  // Create array to hold solutions
  size_t solutions_n = graph->n + 1;
  unsigned int solutions[solutions_n];
  memset(solutions, 0, sizeof(unsigned int) * solutions_n);

  // Solution 0 and 1 are already calculated
  solutions[0] = 0;
  solutions[1] = graph->vertices[0]->weight;

  // Calculate the remaining solutions with the assumption that solution[i] =
  // max(solution[i-1], solutions[i-2] + vertex[i])
  for (size_t i = 2; i < graph->n + 1; i++) {
    unsigned int iminus1 = solutions[i - 1];
    unsigned int iminus2 = solutions[i - 2];
    unsigned int w = graph->vertices[i - 1]->weight;

    if (is_add_overflow_uint(iminus1, w)) return ArithmeticOverflow;

    // overflow checked above
    if (iminus1 >= iminus2 + w)
      solutions[i] = iminus1;
    else
      solutions[i] = iminus2 + w;
  }

  return _reconstruct(solutions, graph, conclusion);
}

Result WeightedIndependentSet_Dynamic(PathGraph* graph,
                                      WeightedIndependentSet** conclusion) {
  return WeightedIndependentSet_Dynamic_Reconstruction(graph, conclusion);
  if (graph == NULL || conclusion == NULL) return NullParameter;

  // Pass in an empty set and you deserve to get one back
  if (graph->n == 0) {
    (*conclusion) = (WeightedIndependentSet*)&EMPTY_SET;
    return Success;
  }

  // Set 1 will always be the first vertex by itself
  WeightedIndependentSet* set1 = NULL;
  Result result = _weightedIndependentSetFromVertex(graph->vertices[0], &set1);

  if (result != Success) return result;

  // If there is only 1 vertex, return it
  if (graph->n == 1) {
    *conclusion = set1;
    return Success;
  }

  // Create array to hold solutions
  size_t solutions_n = graph->n + 1;
  WeightedIndependentSet* solutions[solutions_n];
  memset(solutions, 0, sizeof(void*) * solutions_n);

  // Solution 0 and 1 are already calculated
  solutions[0] = (WeightedIndependentSet*)&EMPTY_SET;
  solutions[1] = set1;

  // Calculate the remaining solutions with the assumption that solution[i] =
  // max(solution[i-1], solutions[i-2] + vertex[i])
  for (size_t i = 2; i < graph->n + 1; i++) {
    WeightedIndependentSet* iminus1 = solutions[i - 1];
    WeightedIndependentSet* iminus2 = solutions[i - 2];
    WeightedVertex* w = graph->vertices[i - 1];

    if (is_add_overflow_uint(iminus1->weight, w->weight)) {
      _solutionsDestroy(solutions, solutions_n);
      return ArithmeticOverflow;
    }

    Result r;
    // overflow checked above
    if (iminus1->weight >= iminus2->weight + w->weight)
      r = _dupWeightedIndependentSet(iminus1, &solutions[i]);
    else
      r = WeightedIndependentSet_Union(iminus2, w, &solutions[i]);

    if (r != Success) {
      _solutionsDestroy(solutions, solutions_n);
      return r;
    }
  }

  _solutionsDestroy(solutions, solutions_n - 1);
  (*conclusion) = solutions[graph->n];

  return Success;
}
