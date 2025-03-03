# Tests
1. arg count
* length not present
* -h works
* no extra params allowed
2. invalid length
* not just an int (anything other than 0-9 chars)
* `< 1`
* `> MAX_INT`
3. len/cost input tests
* at least one
* no duplicate lengths
* both integers `>= 1`
* nothing but "num, num" on line
* (space after comma optional -- multiples?)
* can input lots (1000?)
4. unusual situations
* length < any piece: len 10, 100/1, 50/2 -> 0, remainder 10
5. no remainder
* no dupes: len 8, 5/4 + 3/2 -> 1@5, 3@2
* single len: len 20, 20/5, 10/2 -> 1@20
* composed instead of single, based on value: len 20, 20/2, 10/3, 6/4, 4/4 -> 1@10,
1@6, 1@4
* duplicates: len 20, 10/4, 5/3 -> 4@5
6. has remainder
* one piece: len 10, 8/4 -> 1 @8, rem 2
* mult pieces: len 20, 7@3, 6@4 -> 3@6, rem 2
7. Not value-based
* len 8: 6/5. 4/4 - 2@4, rem 0
8. Same tests with lengths specified in different order
