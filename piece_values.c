#include "piece_values.h"

bool _list_contains_length(Vec ptr, PieceLength len);

int compare_piece_values(PieceLengthValue *a, PieceLengthValue *b) {
  if (a->value != b->value)
    return b->value - a->value;
  return b->length - a->length;
}

Vec read_piece_values(const char *filename) {
  Vec val_list = new_vec(sizeof(PieceLengthValue));
  char buffer[128];

  FILE *file = fopen(filename, "r");

  while (fgets(buffer, sizeof(buffer), file)) {
    if (buffer[0] == '#' || buffer[0] == '\n') {
      continue;
    }

    PieceLengthValue item;
    if (sscanf(buffer, "%d, %d", &item.length, &item.value) != 2 ||
        item.length < 1 || item.value < 1 ||
        _list_contains_length(val_list, item.length)) {
      vec_free(val_list);
      fclose(file);
      return NULL;
    }

    vec_add(val_list, &item);
  }

  fclose(file);

  if (val_list->length == 0) {
    vec_free(val_list);
    return NULL;
  }

  qsort(vec_items(val_list), val_list->length, sizeof(PieceLengthValue),
        (int (*)(const void *, const void *))compare_piece_values);

  return val_list;
}

bool _list_contains_length(Vec v, PieceLength len) {
  PieceLengthValue *items = vec_items(v);
  for (int ix = 0; ix < v->length; ix++)
    if (items[ix].length == len)
      return true;

  return false;
}

void print_piece_values(Vec v) {
  PieceLengthValue *items = vec_items(v);
  for (int ix = 0; ix < v->length; ix++)
    printf("length %4d, value %4d\n", items[ix].length, items[ix].value);
}

bool pvs_equal(PieceLengthValue *pv1, PieceLengthValue *pv2) {
  return pv1->length == pv2->length && pv1->value == pv2->value;
}
