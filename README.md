# 🎨 Multithreaded Graph Coloring (C++)

This project implements **multithreaded graph coloring** using **coarse-grained** and **fine-grained locks** in **C++11**.
It explores the effects of thread partitioning, graph connectivity, and locking strategies on performance.

---

## 🔹 Key Features

* ✅ Multithreaded using `std::thread` and **semaphores** (`sem_init`, `sem_wait`, `sem_post`)
* ✅ Supports **adjacency list** & **adjacency matrix** representations
* ✅ Greedy coloring algorithm with:

  * **Coarse-grained locking**
  * **Fine-grained locking**
* ✅ Graph sizes: 1000–5000 vertices
* ✅ Analyzes:

  * Execution time vs number of threads
  * Number of colors used
  * Impact of graph connectivity

---

## 🧩 Algorithm Overview

### Common Vertex Coloring

1. Initialize `available[no_of_vertices]` to track usable colors.
2. For each vertex `v`, mark neighbors’ colors as unavailable.
3. Assign the first available color to vertex `v` in `color[no_of_vertices]`.
4. Worst-case: each vertex gets a unique color (1 to number of vertices).

### Coarse-Grained Locking

1. Each thread gets a **partition of vertices**.
2. For each vertex `v`:

   * **Internal vertex:** all neighbors in the same partition → color without lock
   * **External vertex:** neighbors in other partitions → acquire a **global semaphore** (`mutex`) before coloring
3. Pros: safe and simple
4. Cons: reduced parallelism if few internal vertices exist

### Fine-Grained Locking

1. Each thread gets a **partition of vertices**.
2. Global array of semaphores: `lock[no_of_vertices]` initialized to 1.
3. For each vertex `v`:

   * **Internal vertex:** color directly
   * **External vertex:** acquire locks for all neighbors + `v`, color, then release locks
4. Pros: safe for high concurrency
5. Cons: high overhead in connected graphs

---

## 📊 Performance Analysis

**Graph 1 – Internal Vertices**

* Few internal vertices → coarse lock may perform worse due to global lock contention
* Coarse lock better than fine-grained lock in highly connected graphs

**Graph 2 – Number of Colors**

* Colors used are nearly identical across methods
* Thread count does not significantly affect color assignment

**Graph 3 – Execution Time vs Threads**

* Threads > CPU cores → overhead dominates
* Coarse/fine-grained locks show fluctuations due to CPU scheduling

**Graph 4 – Thread Count vs Colors**

* Increasing threads does **not affect number of colors**, only execution time

---

## 💡 Key Observations

* Coarse-grained locking outperforms fine-grained in highly connected graphs
* Fine-grained locking introduces significant semaphore overhead
* Proper thread count relative to CPU cores is essential
* Performance differences are due to thread management, not coloring

---

## 🛠 Technologies Used

* **C++11** (`std::thread`, `semaphore`)
* **Data Structures:** adjacency list & adjacency matrix
* **Linux** (tested on 4-core CPU)
* **Performance Tools:** `chrono` for timing analysis

---

## ⚡ Conclusion

* Prefer **coarse-grained locking** for highly connected graphs
* Avoid excessive threads; keep ≤ CPU cores
* Fine-grained locking has higher overhead but ensures strict concurrency
* Number of colors consistent across methods; execution time varies with locks and threads

---

## 📌 Notes

* Tested for **1000–5000 vertices** (higher may cause segmentation faults)
* Graph connectivity and partitioning strategy strongly affect performance
