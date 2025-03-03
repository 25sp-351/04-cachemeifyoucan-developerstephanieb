#ifndef CUT_LIST_H
#define CUT_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "piece_values.h"
#include "vec.h"

#define CUTLIST_JSON_FILENAME "cuts.json"

typedef struct pg {
  PieceLengthValue pv;
  int count;
} PieceGroup;

typedef struct cl {
  Vec groups;
  PieceLength remainder;
  PieceValue total_value;
} CutList;

CutList *new_cutlist(PieceLength length);
void cutlist_free(CutList *cl);
void cutlist_close_json(const char *filename);
void cutlist_print(CutList *cl, PieceLength input_length);
CutList *optimal_cutlist_for(Vec pv, PieceLength total_length);

#endif