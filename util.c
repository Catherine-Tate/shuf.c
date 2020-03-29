
#include <bsd/stdlib.h>

// swap two elements in an array
void swap(char **buf, unsigned int idx_new, unsigned int idx_old) {
  char *tmp = buf[idx_old];
  buf[idx_old] = buf[idx_new];
  buf[idx_new] = tmp;
}

// randomly shuffle an array of lines
// note: this will shuffle the array in place instead of returning a new copy
void shuffle(char **lines, unsigned int num_lines) {
  int random_index;
  for (int index = 0; index < num_lines; ++index) {
    random_index = arc4random_uniform(num_lines);
    swap(lines, random_index, index);
  }
}