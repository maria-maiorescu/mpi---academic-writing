# Sorting Algorithm Performance Study

An experimental comparison of six sorting algorithms implemented in C, measuring average elapsed time across varying array sizes, structures, and element types. Accompanies the academic paper *"Experimental Study of Sorting Algorithm Performance"*.

## Algorithms

| Algorithm | Average | Worst | Best |
|---|---|---|---|
| Bubble sort | O(n²) | O(n²) | O(n) |
| Insertion sort | O(n²) | O(n²) | O(n) |
| Selection sort | O(n²) | O(n²) | O(n²) |
| Merge sort | O(n log n) | O(n log n) | O(n log n) |
| Quick sort (median-of-three) | O(n log n) | O(n²) | O(n log n) |
| Radix sort (base-10) | O(nk) | O(nk) | O(nk) |

## Input Variants

- **Reversed** — strictly descending sequence
- **Random** — uniformly distributed integers in [0, n]
- **Partially sorted** — sorted sequence with a fraction of random swaps (10 %, 30 %, 50 %, 70 %, 90 % sorted)

## Array Sizes & Runs

| Array size (n) | Repetitions |
|---|---|
| 10 | 1,000,000 |
| 50 | 1,000,000 |
| 100 | 1,000,000 |
| 1,000 | 100 |
| 10,000 | 10 |
| 100,000 | 1 |
| 1,000,000 | 1 |

The reported value for each configuration is the **average elapsed time in milliseconds** across all repetitions.

## Files

| File | Description |
|---|---|
| `algorithms.c` | C99 source: data generators, six sorting algorithms, timing harness, CSV output |
| `results.csv` | Raw benchmark results (`Elements, Runs, Algorithm, Variant, Elapsed time(ms)`) |
| `sorting_paper.tex` | Main LaTeX paper (compile with `pdflatex → bibtex → pdflatex × 2`) |
| `sorting_paper.bib` | BibTeX bibliography (Knuth, CLRS, and four supplementary sources) |
| `sorting_paper_backup.tex` | Backup of original paper before the major rewrite |
| `notes for the paper.txt` | Professor feedback notes used to guide revisions |

## Compiling the Paper

```bash
pdflatex sorting_paper.tex
bibtex sorting_paper
pdflatex sorting_paper.tex
pdflatex sorting_paper.tex
```

Requires a TeX Live installation with `pgfplots`, `algorithmicx`, `listings`, and `booktabs`.

## About

Written for the *Academic Writing* course, Year 1 Computer Science, West University of Timișoara.  
Author: Maria Maiorescu — maria.maiorescu06@e-uvt.ro
