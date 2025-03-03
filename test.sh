#!/bin/bash

EXEC="./rodcut"
TEST_FILE="test_piece_values.txt"
ROD_LENGTHS="test_rod_lengths.txt"

if [[ ! -f "$EXEC" ]]; then
  echo "Error: Executable '$EXEC' not found. Compile the program first."
  exit 1
fi

cat > "$TEST_FILE" <<EOL
3, 2
5, 4
7, 6
EOL

echo "Running invalid input tests..."

cat > "$ROD_LENGTHS" <<EOL
abc
-5
0
5 extra
 
   
20
EOL

echo "Testing invalid rod lengths followed by a valid one..."
$EXEC "$TEST_FILE" < "$ROD_LENGTHS"

rm -f "$TEST_FILE" "$ROD_LENGTHS"

echo "✅ All tests completed successfully."