# 4/21/26 - Lecture 21

### Direct Mapped Cache
- CPU Caches are normally LRU
- Regions of memory map to one cache lane

## 2D Arrays - Matrices
- Several way to construct matrices in C
- All must **embed** a 2D construct into 1-dimensional memory

### Methods

Repeat mallocs
```c
int **matr = malloc(rows * sizeof(int*));

for(int i = 0; i < rows; i++) {
    matr[i] = malloc(cols * sizeof(int));
}

process(matr[i][j]);

for(int i = 0; i < rows; i++) {
    free(matr[i]);
}

free(matr);
```

2 mallocs
```c
int **matr = malloc(rows * sizeof(int*));
int *data = malloc(rows * cols * sizeof(int));

for(int i = 0; i < rows; i++) {
    matr[i] = data + i * cols;
}

process(matr[i][j]);

free(data);
free(matr);
```

2 mallocs strategy is more cache-efficient

### Single Mallo Matrices
- Use a 1D array as a 2D matrix
- Usually need to do math to get index \[i\]\[j\] -> \[i * cols + j\]
- Can use macros for this

### Row Major vs Column Major
- Many languages use **Row-Major** order for 2D arrays/lists
     - C, Java, Python, Ocaml
     - mat\[i\] is a row, mat\[i\]\[j\] is the ith row, jth col
- Numerically-oriented langauges use **Column-Major** order
     - Fortran, Matlab, R, Ocaml
     - mat\[j\] is a col
- Beware of convention to save performance
    - A sum on a appreciably large matrix is 6x faster with the correct order, due to cache

## Flavors of Permanent Storage
- Many types of fast memory, but they're **small** and *ephermeral* - all bits become 0 when powered off
- At the bottom of the pyramid are slow and **large** memory
- These are persistent

### HDD - Hard Disk Drive
- Store bits "permanently" as magnetized areas on special platters
- Magnetic disks: moving parts -> slow
- Cheap
- Infinitely readable/writeable

### SSD - Solid State Drive
- Faster, Lighter, Denser
- No moving parts, store electrons instead
- Writes degrade drive, only rewritable a few thousand times
- There is a write-balancing chip on the drive, won't write the same cells everytime
- More expensive
- Seq faster than random, but only a little
- Faster reads then writes
- Better SSDS will have a DRAM cache

> [!NOTE]
> This is a very good video about SSDs: [https://youtu.be/JwYttFnXRps]

### LTO - Tape Drives
- Slow: store bits as magnetic field on a tape/cassette
- Extremely cheap for GB, can store a LOT of data
- Only writable once

## The I/O System Connects CPU and Periphals
- Motherboard connects everything together
- Connects CPU to ram, storage, and everything else (sata, pci(e), usb)
