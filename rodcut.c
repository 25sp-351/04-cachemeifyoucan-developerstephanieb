#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cut_list.h"
#include "piece_values.h"
#include "vec.h"

#define BUFFER_SIZE 128

bool validate_arguments(int argc, char *argv[]);
Vec load_piece_values(const char *filename);
void process_user_input(Vec value_list);
int get_rod_length(int interactive_mode);

int main(int argc, char *argv[]) {
  if (!validate_arguments(argc, argv)) {
    return 1;
  }

  Vec value_list = load_piece_values(
      argv[1]); // Now reads piece values first before handling rod lengths
  if (value_list == NULL) {
    return 1;
  }

  process_user_input(
      value_list); // Handles rod lengths only after piece values are loaded

  vec_free(value_list);
  cutlist_close_json(CUTLIST_JSON_FILENAME);

  return 0;
}

bool validate_arguments(int argc, char *argv[]) {
  if (argc != 2 || !strcmp(argv[1], "-h")) {
    fprintf(stderr, "Usage: %s <piece_values_file>\n", argv[0]);
    return false;
  }
  return true; // Now enforces that the filename is provided as a command-line
               // argument
}

Vec load_piece_values(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Error: File not found or cannot be opened: %s\n",
            filename);
    return NULL;
  }
  fclose(fp);

  Vec value_list = read_piece_values(filename);
  if (value_list == NULL) {
    fprintf(stderr, "Error: Failed to read piece values from file: %s\n",
            filename);
  }
  return value_list;
}

void process_user_input(Vec value_list) {
  int interactive_mode = isatty(STDIN_FILENO);

  if (interactive_mode) {
    printf("Enter rod length (Ctrl+D to exit):\n");
  }

  while (1) {
    int rod_length = get_rod_length(interactive_mode);
    if (rod_length == -1)
      break;

    CutList *cl = optimal_cutlist_for(value_list, rod_length);
    cutlist_print(cl, rod_length);
    cutlist_free(cl);
  }

  if (interactive_mode) {
    printf("\nExiting...\n");
  }
}

int get_rod_length(int interactive_mode) {
  char buffer[BUFFER_SIZE];

  while (1) {
    if (interactive_mode) {
      printf("> ");
      fflush(stdout);
    }

    if (!fgets(buffer, sizeof(buffer), stdin)) {
      return -1;
    }

    int rod_length;
    char extra[32];

    if (sscanf(buffer, "%d%31s", &rod_length, extra) == 1 && rod_length > 0) {
      return rod_length;
    }

    fprintf(stderr, "Invalid rod length. Please enter a positive integer.\n");
  }
}
