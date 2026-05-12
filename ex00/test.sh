#!/bin/bash

# Build the project
echo "Building project..."
make re

echo "======================================"
echo "    Testing ScalarConverter (Ex00)    "
echo "======================================"

# Function to run a test
run_test() {
    echo -e "\n$> ./convert \"$1\""
    ./convert "$1"
}

# 1. Subject Examples
echo "[1] Subject Examples:"
run_test "0"
run_test "nan"
run_test "42.0f"

# 2. Chars
echo -e "\n[2] Char literals:"
run_test "c"
run_test "a"
run_test "*"

# 3. Ints
echo -e "\n[3] Int literals:"
run_test "-42"
run_test "42"
run_test "2147483647" # INT_MAX
run_test "-2147483648" # INT_MIN

# 4. Floats
echo -e "\n[4] Float literals:"
run_test "0.0f"
run_test "-4.2f"
run_test "4.2f"
run_test "-inff"
run_test "+inff"
run_test "nanf"

# 5. Doubles
echo -e "\n[5] Double literals:"
run_test "0.0"
run_test "-4.2"
run_test "4.2"
run_test "-inf"
run_test "+inf"

# 6. Edge Cases / Invalid
echo -e "\n[6] Edge Cases (Invalid inputs / Overflows):"
run_test "30000000000000000" # Should overflow int 
run_test "invalid"
run_test ""

echo -e "\n======================================"
echo "          Tests Completed             "
echo "======================================"
