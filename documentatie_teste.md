# Documentație — Design Teste

## Generalități

Testele sunt generate de programul `gen.cpp` (compilat ca `gen.exe`), care folosește `std::mt19937` cu seed fix pentru reproductibilitate. Fiecare test produce două fișiere:

- `<nume>.in` — input pentru program (prima linie: N; următoarele N linii: valorile)
- `<nume>.ok` — output corect (vectorul sortat crescător, același format ca `.in`)

Două categorii de teste, conform cerinței din enunț:

| Categorie | N | Scop |
|---|---|---|
| `small_NN` | 20.000 | Conform PDF, destinate algoritmilor O(n²) (bubble, gnome, insertion). |
| `big_NN` | 100.000 | Pentru algoritmii eficienți (merge, quick, intro, heap, radix, patience). Redus față de 10⁷ din PDF ca să poată finaliza local și O(n²)-ul într-un timp rezonabil pentru comparație. |

Cele 8 tipuri de input sunt identice între small și big — doar dimensiunea diferă, astfel încât putem observa cum scalează fiecare algoritm.

## Cele 8 tipuri de teste

### 01. random — valori uniforme pe int32

Generat cu `uniform_int_distribution<int>(INT_MIN, INT_MAX)`. Acoperă întregul interval int32, ceea ce înseamnă valori negative și pozitive amestecate.

**Stressează:** baseline pentru toți algoritmii. Niciun pattern exploatabil. Este cazul mediu așteptat.

**Predicții:**
- O(n²) (bubble, gnome, insertion) — timp ~n²/2 comparații
- O(n log n) (merge, quick, intro, heap) — performanță tipică, quicksort cu median-of-three e cel mai rapid în medie
- Radix — O(n × k) unde k = numărul de cifre (≈32 pentru int32)

### 02. sorted — crescător 0, 1, 2, ..., n-1

Vector deja sortat.

**Stressează:**
- **Best case** pentru bubble/gnome/insertion cu flag de early-exit — fac O(n) comparații și se opresc
- **Worst case** pentru quicksort cu pivot prost ales (primul/ultimul element). La noi avem median-of-three, deci nu degradează
- Merge/heap — identic cu random (nu sunt adaptive)

**Predicții:** bubble/gnome/insertion mult mai rapide decât pe random; restul similar.

### 03. reverse — descrescător n-1, n-2, ..., 0

Vector sortat invers.

**Stressează:**
- **Worst case** pentru bubble/gnome/insertion — maxim de swap-uri/deplasări
- Quicksort cu median-of-three — pivotul devine mediana adevărată, partiții echilibrate → comportament bun
- Merge — la fel ca random

**Predicții:** cazul cel mai lent pentru O(n²); restul ok.

### 04. equal — toate valorile egale

Toate cele N elemente au aceeași valoare (random per seed, în [-10⁶, 10⁶]).

**Stressează:**
- **Worst case clasic pentru Lomuto quicksort** — toate elementele ≤ pivot, partiția devine 0|n-1 → recursie liniară → O(n²). Median-of-three nu salvează situația, doar 3-way partition o face
- Bubble/gnome cu flag — best case, O(n)
- Insertion sort — O(n) (toate elementele "deja la locul lor")
- Merge/heap — la fel ca random

**Predicții:** quicksort foarte slab (timeout posibil); bubble/gnome/insertion foarte rapide.

### 05. few_distinct — doar 10 valori distincte

Generăm K=10 valori random pe int32, apoi populăm cei N elemente alegând uniform random dintre aceste 10.

**Stressează:**
- Quicksort Lomuto — multe duplicate degradează partiția (variantă mai blândă a `equal`)
- Radix sort — performanță similară cu random
- Sorturi bazate pe comparație — nu profită de numărul mic de chei distincte

**Predicții:** quicksort vizibil mai lent decât pe random; restul nealterat semnificativ.

### 06. nearly_sorted — sortat + ~1% swap-uri random

Pornim de la `0, 1, ..., n-1` și aplicăm `n/100` swap-uri între poziții random.

**Stressează:**
- **Best/near-best** pentru algoritmii adaptive (bubble, gnome, insertion cu flag)
- Patience sort — beneficiază de run-uri lungi crescătoare existente în input
- Merge — la fel ca random (nu profită de ordine parțială)

**Predicții:** adaptive sorts aproape de O(n); restul similar cu random.

### 07. sawtooth — run-uri ciclice de lungime 100

Valorile sunt `i % 100` — adică ciclu `0, 1, 2, ..., 99, 0, 1, ..., 99, ...`. Multe duplicate, multe run-uri ascendente scurte.

**Stressează:**
- **Patience sort** — input ideal: multe run-uri ordonate, deci puține "stive"
- Quicksort — duplicate multe → degradare ușoară
- Adaptive sorts — beneficiu parțial (run-urile scurte oricum cer multe swap-uri între ele)

**Predicții:** patience sort excelent; restul ok.

### 08. organ_pipe — crescător până la mijloc, apoi descrescător

Prima jumătate `0, 1, ..., n/2-1`, a doua jumătate `n/2-1, n/2-2, ..., 0`. Vector "bitonic" (unimodal).

**Stressează:**
- Quicksort cu pivot = mijloc — exact maximul → partiție 0|n-1 → degradare (la noi median-of-three găsește mediana corectă, deci OK)
- Patience sort — un singur "vârf", două run-uri lungi → eficient
- Restul — performanță tipică

**Predicții:** patience și algoritmii eficienți se descurcă bine; bubble/gnome lente ca pe random.

## Convenții de naming

Conform cerinței din PDF:

- `small_NN.in` / `small_NN.ok` cu NN ∈ {01..08}
- `big_NN.in` / `big_NN.ok` cu NN ∈ {01..08}

(Echivalentul `<tip>_team_<id>_<nr>.in` din PDF — am omis sufixul `_team_<id>` pentru claritate locală; la submission Polygon se redenumesc corespunzător.)

## Seeds folosite (reproductibilitate)

| Test | Seed (small) | Seed (big) |
|---|---|---|
| 01 random | 1 | 101 |
| 02 sorted | 2 | 102 |
| 03 reverse | 3 | 103 |
| 04 equal | 4 | 104 |
| 05 few_distinct | 5 | 105 |
| 06 nearly_sorted | 6 | 106 |
| 07 sawtooth | 7 | 107 |
| 08 organ_pipe | 8 | 108 |

Pentru sorted/reverse/sawtooth/organ_pipe seed-ul nu afectează rezultatul (input determinist). Pentru random/equal/few_distinct/nearly_sorted, valorile diferă în funcție de seed.

## Rezultate benchmark

Rulat cu `benchmark.ps1`, timeout 150s per rulare. Datele complete în `benchmark.csv`.

### Timpi pe testele BIG (n=100.000), milisecunde

| Test \ Algoritm | bubble | gnome | merge | quick | intro | heap | insertion | patience | radix |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 01 random | 11767 | 5003 | 56 | 51 | 69 | 65 | 600 | 140 | **FAIL** |
| 02 sorted | 44 | 50 | 52 | 41 | 41 | 45 | 45 | 2349 | 50 |
| 03 reverse | 1601 | 10505 | 50 | 42 | 41 | 45 | 1152 | 52 | 46 |
| 04 equal | 45 | 47 | 47 | **1790** | 45 | 45 | 48 | 47 | 44 |
| 05 few_distinct | 11440 | 4542 | 55 | 221 | 44 | 49 | 552 | 53 | **FAIL** |
| 06 nearly_sorted | 1565 | 178 | 46 | 47 | 44 | 50 | 62 | 2316 | 50 |
| 07 sawtooth | 3999 | 4918 | 44 | 64 | 46 | 49 | 600 | 46 | 46 |
| 08 organ_pipe | 1392 | 5002 | 46 | 135 | 49 | 49 | 608 | 801 | 48 |

### Timpi pe testele SMALL (n=20.000), milisecunde

| Test \ Algoritm | bubble | gnome | merge | quick | intro | heap | insertion | patience | radix |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 01 random | 396 | 243 | 39 | 41 | 41 | 40 | 64 | 44 | **FAIL** |
| 02 sorted | 35 | 39 | 35 | 38 | 36 | 46 | 37 | 108 | 41 |
| 03 reverse | 95 | 450 | 39 | 35 | 37 | 36 | 80 | 37 | 35 |
| 04 equal | 35 | 38 | 36 | 103 | 37 | 35 | 38 | 39 | 37 |
| 05 few_distinct | 378 | 215 | 35 | 47 | 36 | 36 | 57 | 37 | **FAIL** |
| 06 nearly_sorted | 100 | 44 | 35 | 38 | 36 | 39 | 36 | 97 | 45 |
| 07 sawtooth | 203 | 232 | 36 | 37 | 41 | 36 | 64 | 39 | 41 |
| 08 organ_pipe | 96 | 235 | 38 | 35 | 36 | 41 | 57 | 72 | 42 |

### Observații cheie

1. **Confirmate teoretic:**
   - Bubble/gnome/insertion sunt O(n²) — clar mai lenți pe inputuri "rele" (random, reverse, sawtooth).
   - Bubble best case pe sorted (35ms) datorită flag-ului de early-exit.
   - Gnome worst case pe **reverse** (10.5s la big) — fiecare element trebuie mutat pas cu pas până la poziția finală.
   - Quicksort cu Lomuto + median-of-three **degradează vizibil pe `equal` și `few_distinct`** (1.8s vs 50ms baseline) — confirmă necesitatea 3-way partition pentru duplicate masive.
   - Introsort este cel mai robust — sub 70ms pe toate testele.
   - Mergesort și heapsort sunt insensibili la pattern-ul de input — performanță predictibilă.

2. **Anomalii / observații neașteptate:**
   - **Patience sort este LENT pe `sorted` și `nearly_sorted`** (2.3s pe big) când teoretic ar trebui să fie cazul ideal (un singur pile). Probabil implementarea folosește mereu binary search + priority queue fără short-circuit pentru intrare ordonată.
   - **Radix sort eșuează cu access violation** (exit code -1073741819 = 0xC0000005) pe testele `random` și `few_distinct`, care conțin valori negative. Implementarea nu gestionează semnul int32. Pe testele cu doar valori non-negative (sorted, reverse, equal cu seed favorabil, nearly_sorted, sawtooth, organ_pipe) merge corect și foarte rapid.
   - Quicksort pe `equal` la small (103ms) e ~3x mai lent decât baseline (35ms) — același pattern ca pe big, doar la scară mai mică.

3. **Concluzii practice:**
   - Pentru date general-purpose: introsort sau std::sort.
   - Pentru date aproape sortate: gnome/insertion cu early-exit pot bate algoritmii O(n log n) (vezi gnome pe `nearly_sorted` big: 178ms vs 2316ms patience).
   - Pentru date numeric pozitive cu rang limitat: radix sort (când va fi reparat) — pe testele unde merge, e printre cei mai rapizi.
   - **De evitat:** Lomuto quicksort fără 3-way partition pe date cu multe duplicate.
