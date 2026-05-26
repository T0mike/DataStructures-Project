# DataStructures-Project — Sorting Algorithms Benchmark

University assignment (Data Structures, 1st year): implement and benchmark a set of sorting algorithms on a diverse set of inputs.

## Algorithms implemented

Each algorithm is a standalone `.cpp` file containing a `custom_sort(std::vector<int>&)` function plus the standard `main` template required by the assignment (reads `n` integers, calls `custom_sort`, asserts the result is sorted).

| File | Algorithm | Complexity | Notes |
|---|---|---|---|
| `bubblesort.cpp` | Bubble Sort | O(n²) avg/worst, O(n) best | Early-exit flag |
| `gnomesort.cpp` | Gnome Sort | O(n²) avg/worst, O(n) best | Single loop, walks back on swap |
| `mergesort.cpp` | Merge Sort | O(n log n) | Stable, O(n) extra memory |
| `quicksort.cpp` | Quick Sort | O(n log n) avg, O(n²) worst | Median-of-three pivot, Lomuto partition |
| `introsort.cpp` | Intro Sort | O(n log n) guaranteed | Hybrid: quick + heap + insertion |
| `heap_sort.cpp` | Heap Sort | O(n log n) | In-place max-heap |
| `insertion_sort.cpp` | Insertion Sort | O(n²) avg/worst, O(n) best | |
| `patience_sorting.cpp` | Patience Sort | O(n log n) | Patience-card-game model |
| `radix_sort.cpp` | Radix Sort | O(n × k) | k = digits |

## Test design

Tests live in `tests/`. Two size categories per the assignment:

- **Small** — `n = 20,000` (suitable for O(n²) algorithms)
- **Big** — `n = 100,000` (reduced from the original 10⁷ so that O(n²) algorithms also finish within reasonable time for local benchmarking)

For each size, all 8 test types are present (`small_01..08`, `big_01..08`), giving 16 inputs and 16 expected outputs.

### Seeds (for reproducibility)

| Test | Type | Seed (small) | Seed (big) |
|---|---|---|---|
| 01 | random | 1 | 101 |
| 02 | sorted | 2 | 102 |
| 03 | reverse | 3 | 103 |
| 04 | equal | 4 | 104 |
| 05 | few_distinct | 5 | 105 |
| 06 | nearly_sorted | 6 | 106 |
| 07 | sawtooth | 7 | 107 |
| 08 | organ_pipe | 8 | 108 |

For `sorted`, `reverse`, `sawtooth`, and `organ_pipe` the seed does not affect the output (deterministic input). For `random`, `equal`, `few_distinct`, and `nearly_sorted`, the values depend on the seed.

### Rationale per test type

- **random** — baseline. No exploitable pattern. Expected average-case behavior.
- **sorted** — best case for bubble/gnome/insertion (with early-exit flag). Historically the worst case for naive quicksort (first/last pivot); not a problem here because of median-of-three.
- **reverse** — worst case for bubble/gnome/insertion (maximum number of swaps).
- **equal** — classic worst case for Lomuto-partition quicksort: every element compares ≤ pivot, partition becomes `0 | n-1`, recursion degenerates to O(n²). Best case for bubble/gnome/insertion.
- **few_distinct** — milder version of `equal`. Stresses partition-based sorts in the presence of mass duplicates without becoming pathological.
- **nearly_sorted** — adaptive sorts (bubble, gnome, insertion with early-exit) should shine; non-adaptive sorts (merge, heap) behave identically to random.
- **sawtooth** — many short ascending runs. Designed to favor patience sort, which builds piles aligned with ascending runs.
- **organ_pipe** — bitonic input. Trips up quicksort variants that pick the middle element as pivot (the middle is the global maximum).

## Benchmark

### Results — BIG tests (n = 100,000), milliseconds

| Test | bubble | gnome | merge | quick | intro | heap | insertion | patience | radix |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 01 random | 11767 | 5003 | 56 | 51 | 69 | 65 | 600 | 140 | **FAIL** |
| 02 sorted | 44 | 50 | 52 | 41 | 41 | 45 | 45 | 2349 | 50 |
| 03 reverse | 1601 | 10505 | 50 | 42 | 41 | 45 | 1152 | 52 | 46 |
| 04 equal | 45 | 47 | 47 | **1790** | 45 | 45 | 48 | 47 | 44 |
| 05 few_distinct | 11440 | 4542 | 55 | 221 | 44 | 49 | 552 | 53 | **FAIL** |
| 06 nearly_sorted | 1565 | 178 | 46 | 47 | 44 | 50 | 62 | 2316 | 50 |
| 07 sawtooth | 3999 | 4918 | 44 | 64 | 46 | 49 | 600 | 46 | 46 |
| 08 organ_pipe | 1392 | 5002 | 46 | 135 | 49 | 49 | 608 | 801 | 48 |

### Results — SMALL tests (n = 20,000), milliseconds

| Test | bubble | gnome | merge | quick | intro | heap | insertion | patience | radix |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 01 random | 396 | 243 | 39 | 41 | 41 | 40 | 64 | 44 | **FAIL** |
| 02 sorted | 35 | 39 | 35 | 38 | 36 | 46 | 37 | 108 | 41 |
| 03 reverse | 95 | 450 | 39 | 35 | 37 | 36 | 80 | 37 | 35 |
| 04 equal | 35 | 38 | 36 | 103 | 37 | 35 | 38 | 39 | 37 |
| 05 few_distinct | 378 | 215 | 35 | 47 | 36 | 36 | 57 | 37 | **FAIL** |
| 06 nearly_sorted | 100 | 44 | 35 | 38 | 36 | 39 | 36 | 97 | 45 |
| 07 sawtooth | 203 | 232 | 36 | 37 | 41 | 36 | 64 | 39 | 41 |
| 08 organ_pipe | 96 | 235 | 38 | 35 | 36 | 41 | 57 | 72 | 42 |

### Observations

1. **Theory confirmed**
   - Bubble/gnome/insertion are O(n²) — visibly slower on adversarial inputs (random, reverse, sawtooth).
   - Bubble's best case is `sorted` (35 ms on big), thanks to the early-exit flag.
   - Gnome's worst case is `reverse` (10.5 s on big) — each element has to walk all the way to its final position.
   - Quicksort with Lomuto + median-of-three **degrades on `equal` and `few_distinct`** (1.8 s vs ~50 ms baseline), confirming that median-of-three doesn't help with mass duplicates — only a 3-way partition would.
   - Introsort is the most robust algorithm — under 70 ms on every input.
   - Merge sort and heap sort are insensitive to input pattern — predictable performance.

2. **Anomalies**
   - **Patience sort is slow on `sorted` and `nearly_sorted`** (~2.3 s on big), where it should theoretically be optimal (a single pile). The implementation likely uses a binary-search + priority-queue pattern without a short-circuit for already-ordered input.
   - **Radix sort crashes** with exit code `-1073741819` (Windows access violation, `0xC0000005`) on `random` and `few_distinct` — the test types that contain negative values. The implementation does not handle the sign bit of int32. On inputs restricted to non-negative values it works correctly and is very fast.
   - Quicksort on `equal` at small size (103 ms) is ~3× slower than baseline (35 ms) — the same pattern as on big, scaled down.

3. **Practical takeaways**
   - For general-purpose data: introsort (or `std::sort`).
   - For nearly-sorted data: adaptive sorts beat O(n log n) algorithms (gnome on `nearly_sorted` big: 178 ms vs 2316 ms for patience).
   - For non-negative integers with a bounded range: radix sort (once the negative-number bug is fixed) is among the fastest.
   - Avoid Lomuto quicksort without a 3-way partition when the input may contain many duplicates.

