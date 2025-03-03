#include "cut_list.h"

// Global flag to track if the file needs a JSON opening bracket
static bool is_first_json_entry = true;

CutList *new_cutlist(PieceLength length) {
  CutList *cl = malloc(sizeof(CutList));
  cl->groups = new_vec(sizeof(PieceGroup));
  cl->remainder = length;
  cl->total_value = 0;
  return cl;
}

CutList *cutlist_copy(CutList *cl) {
  CutList *ncl = malloc(sizeof(CutList));
  ncl->groups = vec_copy(cl->groups);
  ncl->remainder = cl->remainder;
  ncl->total_value = cl->total_value;
  return ncl;
}

void cutlist_free(CutList *cl) {
  vec_free(cl->groups);
  free(cl);
}

bool cutlist_can_add_piece(CutList *cl, PieceLengthValue pv) {
  return pv.length <= cl->remainder;
}

CutList *cutlist_add_piece(CutList *cl, PieceLengthValue pv) {
  PieceGroup *groups = vec_items(cl->groups);

  cl->remainder -= pv.length;
  cl->total_value += pv.value;

  for (int group_ix = 0; group_ix < cl->groups->length; group_ix++) {
    if (pvs_equal(&groups[group_ix].pv, &pv)) {
      groups[group_ix].count++;
      return cl;
    }
  }
  PieceGroup g = (PieceGroup){.pv = pv, .count = 1};
  vec_add(cl->groups, &g);
  return cl;
}

void cutlist_reset_json(const char *filename) {
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }
  fclose(fp);
  is_first_json_entry = true;
}

void cutlist_print_json(CutList *cl, PieceLength input_length,
                        const char *filename) {
  PieceGroup *groups = vec_items(cl->groups);

  if (is_first_json_entry) {
    cutlist_reset_json(filename);
  }

  FILE *fp = fopen(filename, "a");
  if (fp == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  if (is_first_json_entry) {
    fprintf(fp, "[\n");
    is_first_json_entry = false;
  } else {
    fprintf(fp, ",\n");
  }

  fprintf(fp, "  {\n");
  fprintf(fp, "    \"input_length\": %d,\n", input_length);
  fprintf(fp, "    \"value\": %d,\n", cl->total_value);
  fprintf(fp, "    \"remainder\": %d,\n", cl->remainder);
  fprintf(fp, "    \"cuts\": [\n");

  for (int ix = 0; ix < cl->groups->length; ix++) {
    int total_value_for_length = groups[ix].count * groups[ix].pv.value;
    fprintf(fp,
            "      { \"length\": %d, \"count\": %d, \"piece_value\": %d, "
            "\"value\": %d }%s\n",
            groups[ix].pv.length, groups[ix].count, groups[ix].pv.value,
            total_value_for_length, ix == cl->groups->length - 1 ? "" : ",");
  }

  fprintf(fp, "    ]\n");
  fprintf(fp, "  }");

  is_first_json_entry = false;

  fclose(fp);
}

void cutlist_close_json(const char *filename) {
  if (!is_first_json_entry) {
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
      printf("Error opening file!\n");
      exit(1);
    }
    fprintf(fp, "\n]\n");
    fclose(fp);
  }
}

void cutlist_print(CutList *cl, PieceLength input_length) {
  PieceGroup *groups = vec_items(cl->groups);
  for (int ix = 0; ix < cl->groups->length; ix++) {
    printf("%2d @ %4d = %4d \n", groups[ix].count, groups[ix].pv.length,
           groups[ix].pv.value);
  }
  printf("Remainder: %5d\n", cl->remainder);
  printf("Value:     %5d\n", cl->total_value);

  cutlist_print_json(cl, input_length, CUTLIST_JSON_FILENAME);
}

CutList *choose_best_cuts(CutList *starting_cutlist, Vec pv) {
  CutList *best_option_so_far = cutlist_copy(starting_cutlist);

  for (PieceLengthValue *length_to_try = vec_items(pv);
       length_to_try->length != 0; ++length_to_try) {
    if (!cutlist_can_add_piece(starting_cutlist, *length_to_try))
      continue;

    CutList *maybe_better_option = choose_best_cuts(
        cutlist_add_piece(cutlist_copy(starting_cutlist), *length_to_try), pv);

    if (maybe_better_option->total_value > best_option_so_far->total_value) {
      if (best_option_so_far)
        cutlist_free(best_option_so_far);
      best_option_so_far = maybe_better_option;
      continue;
    }
    cutlist_free(maybe_better_option);
  }
  cutlist_free(starting_cutlist);
  return best_option_so_far;
}

CutList *optimal_cutlist_for(Vec pv, PieceLength total_length) {
  return choose_best_cuts(new_cutlist(total_length), pv);
}