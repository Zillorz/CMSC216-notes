# Lecture 20 - 4/16/26

### Core Idea
Study the following code
```c
int *data = NULL;

int sum_simple(int len, int stride) {
  int sum = 0;
  for(int i = 0; i < len; i += stride) {
    sum += data[i];
  }
  return sum;
}
```

We would expect the throughput (additions / s) to stay the same as stride increases right?
It doesn't. 

As it increases, throughput actually decreases, due to cache misses!

```
Throughout with stride: 1 is 931,721,395.176098 additions/s
Throughout with stride: 8 is 870,210,119.557042 additions/s
Throughout with stride: 16 is 727,388,510.730544 additions/s
Throughout with stride: 24 is 405,114,636.531566 additions/s
Throughout with stride: 32 is 158,212,934.497652 additions/s
Throughout with stride: 64 is 135,012,682.675594 additions/s
Throughout with stride: 96 is 96,563,939.634631 additions/s
```

This is with data being a 1GB array of random ints, run on a m1 macbook pro
Run without optimizations, they kinda mess this up (too fast).

Also, this is a device with SOC ram, use a laptop with separate ram and the difference will be more obvious.

### CPU vs Memory Speed
- Early Computing Systems had CPU Chip and Memory Chips, little if any data storage in the CPU
- CPU and Memory Chips ran at similar speeds / clock freqs., CPU would fetch from mem, do op, and write to mem
- CPU chip speed was a lot easier to speed up compared to memory
- Registers and Cache were developed in response to the growing speed difference between CPU and Memory
- Registers, LN cache


### Cache Favors Temporal and Spatial Locality
- Hardware people realized that programmers often access memory sequentially e.g.

```c
int sum = 0;
for(int i = 0; i < n; i++) {
    sum = data[i];
}
```

**Temporal Locality**
Memory used recently is likely to be used again (i, sum)

**Spatial Locality**
nearby addresses to recently used memory likely to be used (data\[i\], data\[i+1\])

- Using cache well speeds up code A LOT
- This is the reason why vectors/arrays are so much faster than linked lists

### The Memory Pyramid?

CPU - Extremely small (one thing)
Registers - small (16, 8 byte registers) 256b
CPU Cache (L1 4-16kb per core, L2 64-128 per core, L3 4-64mb per cpu)
RAM (4gb-1024gb)
SSD (128gb to 16tb)
HDD (128gb to 16tb)
Network (infinite)

As you go up, less memory but more speed

### Numbers Everyone Should Know
- "Main Memory" is composed of many different physical devices that work together and have differing sizes/speeds
- Accessing memory at #4096 may involve some or all of
    - CPU Cache
    - Ram
    - SSD/HDD

### Why isn't everything cache?
Cache is big, cache is REALLY expensive.
But this is kinda happening, look at AMD X3D chips and INTEL bLLC
