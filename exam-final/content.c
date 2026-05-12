// While the final exam is cumlative, this content file is not
// also look at exam-{1,2,3}/content.c
// lecture 22 to 25
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

// This is a pretty weak example :(

void reserve_memory_space();
void threaded_test();
void contention_is_bad();

int main(int argc, char** argv) {
  reserve_memory_space();
  threaded_test();
  contention_is_bad();
  return 0;
}

void reserve_memory_space() {
  void *addr = (void *)(0x60000000000);
  char* buf = mmap(addr, 4096, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  if (buf != addr) {
    // did not work
    if (buf == MAP_FAILED) {
      // did not work at all
    } else {
      // mapped somewhere else
    }
  }

  buf[0] = 'a';
  buf[4095] = 'b';

  int res = munmap(buf, 4096);

  if (res == -1) {
    printf("Failed to mummap\n");
  }

  printf("mmaped and mummaped successfully\n");
  return;
}


typedef struct {
  int thread_id;
  int thread_count;

  double *array;
  int size;
  
  pthread_mutex_t *mutex;
  double *sum;
} ctx_t;

void *worker(void* v) {
  ctx_t *ctx = (ctx_t *)v;

  int start = ctx->size / ctx->thread_count * ctx->thread_id; 
  int end = ctx->size / ctx->thread_count * (ctx->thread_id + 1);

  if (ctx->thread_id == ctx->thread_count - 1) {
    end = ctx->size;
  }

  double local_sum = 0.0;

  for(int i = start; i < end; i++) {
    local_sum += ctx->array[i];
  }

  pthread_mutex_lock(ctx->mutex);
  *ctx->sum += local_sum;
  pthread_mutex_unlock(ctx->mutex);

  return NULL;
}

double non_threaded_test();

void threaded_test() {
  int thread_count = 8;

  double *array = malloc(sizeof(double) * 32767);

  for(size_t i = 0; i < 32767; i++) {
    array[i] = (i * i * i * 23.2 - i * 12.5853);
  }

  pthread_t threads[thread_count];
  ctx_t ctx[thread_count];

  double sum = 0.0;
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  for(int id = 0; id < thread_count; id++) {
    ctx[id].thread_id = id;
    ctx[id].thread_count = thread_count;

    ctx[id].array = array;
    ctx[id].size = 32767;

    ctx[id].mutex = &mutex;
    ctx[id].sum = &sum;

    pthread_create(&threads[id], NULL, worker, (void *)(&ctx[id]));
  }

  for(int i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Together, all the threads calculated a sum of %lf\n", sum);
  printf("The unthreaded solution is %lf\n", non_threaded_test());
  pthread_mutex_destroy(&mutex);
}

double non_threaded_test() {
  double sum = 0.0;

  for(size_t i = 0; i < 32767; i++) {
    sum += (i * i * i * 23.2 - i * 12.5853);
  }

  return sum;
}

typedef struct {
  pthread_mutex_t *lock;
  int *prod;
  int start;
  int end;
} ct_ctx_t;

void *contention_worker(void *v) {
  ct_ctx_t *ctx = v;

  for(int i = ctx->start; i < ctx->end; i++) {
    pthread_mutex_lock(ctx->lock);
    *ctx->prod *= i;
  }

  return NULL;
}

void *no_contention_worker(void *v) {
  ct_ctx_t *ctx = v;

  int local_prod = 0;
  for(int i = ctx->start; i < ctx->end; i++) {
    local_prod *= i;
  }


  pthread_mutex_lock(ctx->lock);
  ctx->prod += local_prod;
  pthread_mutex_unlock(ctx->lock);

  return NULL;
}


void contention_is_bad() {
  int thread_count = 16;
  clock_t begin, end;

  pthread_t threads[thread_count];
  ct_ctx_t ctx[thread_count];
  pthread_mutex_t lock;
  pthread_mutex_init(&lock, NULL);

  int prod = 1;

  begin = clock();
  for(int i = 0; i < thread_count; i++) {
    ctx[i].start = 27;
    ctx[i].end = 2924;
    ctx[i].prod = &prod;
    ctx[i].lock = &lock;

    pthread_create(&threads[i], NULL, contention_worker, (void *)&ctx[i]);
  }

  for(int i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
  }
  end = clock();

  double contented_time = (double)(end - begin) / CLOCKS_PER_SEC;

  begin = clock();
  for(int i = 0; i < thread_count; i++) {
    ctx[i].start = 27;
    ctx[i].end = 2924;
    ctx[i].prod = &prod;
    ctx[i].lock = &lock;

    pthread_create(&threads[i], NULL, no_contention_worker, (void *)&ctx[i]);
  }

  for(int i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
  }
  end = clock();


  double uncontented_time = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Contented took: %lf\n", contented_time);
  printf("Uncontented took: %lf\n", uncontented_time);
  printf("Uncontented was %lfx faster\n", contented_time / uncontented_time);
}
