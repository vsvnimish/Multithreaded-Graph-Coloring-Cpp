# Multithreaded Graph Coloring (C++)

This project implements multithreaded graph coloring using **coarse-grained** and **fine-grained locks** in C++. It explores the effects of thread partitioning, graph connectivity, and locking strategies on performance.

---

## 🔧 Key Features

* Implemented in **C++11** using:

  * `std::thread`
  * Semaphores (`sem_init`, `sem_wait`, `sem_post`)
  * Adjacency list and adjacency matrix representations
* Supports **partitioning of vertices** for multiple threads
* Implements **greedy coloring algorithm** for:

  * Coarse-grained locking
  * Fine-grained locking
* Performance analysis for graphs with **1000–5000 vertices**
* Evaluates:

  * Execution time vs number of threads
  * Number of colors used
  * Impact of graph connectivity

---

## 🧠 Algorithm Overview

### Common Vertex Coloring Algorithm

1. Initialize a local array `available[no_of_vertices]` to track usable colors.
2. For each vertex `v`, mark colors assigned to neighbors as unavailable.
3. Assign the first available color to vertex `v` in the `color[no_of_vertices]` array.
4. Colors range from `1` to `no_of_vertices` in the worst case.

---

### Coarse-Grained Locking

1. Each thread receives a **partition of vertices**.
2. For each vertex `v`:

   * **Internal vertex:** all neighbors in the same partition → color without lock.
   * **External vertex:** neighbors in other partitions → acquire a **global semaphore lock** (`mutex`) before coloring.
3. Ensures thread safety but may reduce parallelism when few internal vertices exist.

---

### Fine-Grained Locking

1. Each thread receives a **partition of vertices**.
2. Global array of semaphores: `lock[no_of_vertices]` initialized to `1`.
3. For each vertex `v`:

   * **Internal vertex:** color directly.
   * **External vertex:** acquire locks for all neighbors and `v` itself before coloring, then release all locks.
4. Better safety for highly concurrent coloring but may suffer high overhead in connected graphs.

---

## 📊 Performance Analysis

### Graph 1: Effect of Internal Vertices

* If few internal vertices exist:

  * **Coarse lock** may perform worse due to global lock contention.
  * Coarse lock performs better than fine-grained lock when graph connectivity is high.

### Graph 2: Number of Colors

* The number of colors remains nearly the same for all methods.
* Greedy algorithm ordering affects color assignment, but thread count does not significantly change color count.

### Graph 3: Execution Time vs Threads

* Using threads > CPU cores introduces overhead:

  * Coarse and fine-grained locks show fluctuations due to thread management and CPU scheduling.

### Graph 4: Thread Count vs Color Assignment

* Increasing threads does **not affect the number of colors**; it only influences execution time.

---

## 🧩 Key Observations

1. Coarse-grained locking performs better than fine-grained locking in highly connected graphs due to reduced semaphore operations.
2. Fine-grained locking introduces higher overhead because of multiple semaphore operations.
3. Proper thread count relative to CPU cores is crucial for performance.
4. Number of colors is consistent across all approaches; performance differences arise primarily from thread management overhead.

---

## 🛠 Technologies Used

* **C++11** (`std::thread`, `semaphore`)
* **Data Structures:** adjacency list & adjacency matrix
* **Linux** (tested on 4-core CPU)
* **Performance Tools:** timing analysis using `chrono`

---

## ⚡ Conclusion

* Coarse-grained locking is preferable to fine-grained locking for highly connected graphs.
* Fine-grained locking introduces significant overhead.
* Proper thread management and understanding of graph structure are key to maximizing performance.
* Number of threads should generally **not exceed the number of available CPU cores**.
