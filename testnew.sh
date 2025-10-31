#!/bin/bash

# Define the path to your Minishell executable
MINISHELL="./minishell"

# Define a temporary file to store outputs
TMP_MINISHELL_OUTPUT=$(mktemp)
TMP_BASH_OUTPUT=$(mktemp)
DIFF_OUTPUT=$(mktemp)
TMP_VALGRIND_OUTPUT=$(mktemp)

# Arrays to track different types of failures
FAILED_OUTPUT=()
FAILED_EXIT_CODE=()
FAILED_LEAKS=()
PASSED_TESTS=()
PASSED_WITH_LEAKS=()

# Array to track test files created
TEST_FILES=()

# Function to cleanup all temporary files
cleanup_all() {
    # Remove temporary files created by mktemp
    rm -f "$TMP_MINISHELL_OUTPUT" "$TMP_BASH_OUTPUT" "$DIFF_OUTPUT" "$TMP_VALGRIND_OUTPUT"
    
    # Remove valgrind suppression file
    rm -f valgrind.supp
    
    # Remove all test files created during tests
    rm -f file.txt outfile output.txt count.txt out1 out2 out3 outfile1 outfile2 outfile3 outfile4 outfile5 outfile6
    rm -f no_permission no_write input.txt testfile.txt infile infil
    
    # Remove test directories if they exist
    rm -rf t1 '/tmp/My Directory with Spaces' '/tmp/special!dir@#' /tmp/testdir_* /tmp/symlink_*
    rm -f /tmp/testfile_symlink /tmp/symlink_file
    rm -rf '/tmp/dir_with\backslash'
    
    # Clean any additional output files that might have been created
    rm -f out*.txt count*.txt output*.txt file*.txt
}

# Trap to ensure cleanup on script exit or interruption
trap cleanup_all EXIT INT TERM

# Function to clean outputs by removing specific prefixes
clean_output() {
    local file="$1"
    # Remove minishell prefix
    sed -i 's/^minishell: //' "$file"
    # Remove bash prefix with line numbers
    sed -i 's/^bash: line [0-9]*: //' "$file"
    # Remove trailing whitespace and normalize line endings
    sed -i 's/[[:space:]]*$//' "$file"
}

# Function to run a test case
run_test() {
    local test_case="$1"
    local description="$2"
    local test_passed=1  # 0 = passed, 1 = failed

    echo -e "${BW}Testing:${NC} $test_case"

    # Run the test case in Minishell
    echo -n "$test_case" | $MINISHELL > "$TMP_MINISHELL_OUTPUT" 2>&1
    local minishell_exit_code=$?

    # Run the test case in Bash
    echo -n "$test_case" | LANG=C bash > "$TMP_BASH_OUTPUT" 2>&1
    local bash_exit_code=$?

    # Clean both outputs by removing prefixes
    clean_output "$TMP_MINISHELL_OUTPUT"
    clean_output "$TMP_BASH_OUTPUT"

    # Compare the outputs
    diff "$TMP_MINISHELL_OUTPUT" "$TMP_BASH_OUTPUT" > "$DIFF_OUTPUT"
    local diff_exit_code=$?

    # Run with valgrind to check for leaks
    echo -n "$test_case" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --suppressions=valgrind.supp --quiet $MINISHELL > /dev/null 2> "$TMP_VALGRIND_OUTPUT"

    # Extract leak information from valgrind output
    local definitely_lost=$(grep -o "definitely lost: [0-9,]* bytes" "$TMP_VALGRIND_OUTPUT" | head -1)
    local indirectly_lost=$(grep -o "indirectly lost: [0-9,]* bytes" "$TMP_VALGRIND_OUTPUT" | head -1)
    local possibly_lost=$(grep -o "possibly lost: [0-9,]* bytes" "$TMP_VALGRIND_OUTPUT" | head -1)
    local still_reachable=$(grep -o "still reachable: [0-9,]* bytes" "$TMP_VALGRIND_OUTPUT" | head -1)

    # Default values if not found
    definitely_lost=${definitely_lost:-"definitely lost: 0 bytes"}
    indirectly_lost=${indirectly_lost:-"indirectly lost: 0 bytes"}
    possibly_lost=${possibly_lost:-"possibly lost: 0 bytes"}
    still_reachable=${still_reachable:-"still reachable: 0 bytes"}

    local has_leaks=0
    if [[ ! "$definitely_lost" =~ "definitely lost: 0 bytes" ]] || 
       [[ ! "$indirectly_lost" =~ "indirectly lost: 0 bytes" ]] || 
       [[ ! "$possibly_lost" =~ "possibly lost: 0 bytes" ]]; then
        has_leaks=1
    fi

    # Determine test result
    local output_ok=0
    local exit_code_ok=0
    local leaks_ok=0

    if [ $diff_exit_code -eq 0 ]; then
        output_ok=1
    fi

    if [ $minishell_exit_code -eq $bash_exit_code ]; then
        exit_code_ok=1
    fi

    if [ $has_leaks -eq 0 ]; then
        leaks_ok=1
    fi

    # Print result with detailed information
    if [ $output_ok -eq 1 ] && [ $exit_code_ok -eq 1 ] && [ $leaks_ok -eq 1 ]; then
        echo -e "  ${BG}✓ PASS${NC}"
        PASSED_TESTS+=("$test_case")
        test_passed=0
    else
        echo -e "  ${BR}✗ FAIL${NC}"
        test_passed=1
        
        # Show detailed failure reasons
        if [ $output_ok -eq 0 ]; then
            echo -e "    ${BY}Output difference:${NC}"
            echo -e "    ${BW}Minishell:${NC} $(cat "$TMP_MINISHELL_OUTPUT" | tr '\n' ' ')"
            echo -e "    ${BW}Bash:${NC} $(cat "$TMP_BASH_OUTPUT" | tr '\n' ' ')"
            FAILED_OUTPUT+=("$test_case")
        fi
        
        if [ $exit_code_ok -eq 0 ]; then
            echo -e "    ${BY}Exit code difference:${NC}"
            echo -e "    ${BW}Minishell:${NC} $minishell_exit_code ${BW}Bash:${NC} $bash_exit_code"
            FAILED_EXIT_CODE+=("$test_case")
        fi
        
        if [ $leaks_ok -eq 0 ]; then
            echo -e "    ${BY}Memory leaks:${NC}"
            echo -e "    ${BR}$definitely_lost${NC}"
            echo -e "    ${BR}$indirectly_lost${NC}"
            echo -e "    ${BR}$possibly_lost${NC}"
            FAILED_LEAKS+=("$test_case")
        fi
        
        # Show full diff if output differs
        if [ $output_ok -eq 0 ] && [ -s "$DIFF_OUTPUT" ]; then
            echo -e "    ${BY}Detailed diff:${NC}"
            sed 's/^/      /' "$DIFF_OUTPUT"
        fi
    fi
    
    echo  # Empty line for readability
}

# Function to print summary at the end
print_summary() {
    local total_tests=$((${#PASSED_TESTS[@]} + ${#FAILED_OUTPUT[@]} + ${#FAILED_EXIT_CODE[@]} + ${#FAILED_LEAKS[@]}))
    
    echo -e "\n${BB}================================================================================${NC}"
    echo -e "${BB}TEST SUMMARY${NC}"
    echo -e "${BB}================================================================================${NC}"
    
    echo -e "\n${BG}✓ PASSED: ${#PASSED_TESTS[@]}/$total_tests${NC}"
    
    if [ ${#FAILED_OUTPUT[@]} -gt 0 ]; then
        echo -e "\n${BR}✗ FAILED - OUTPUT DIFFERENCES (${#FAILED_OUTPUT[@]} tests):${NC}"
        for test in "${FAILED_OUTPUT[@]}"; do
            echo -e "  ${BR}•${NC} $test"
        done
    fi
    
    if [ ${#FAILED_EXIT_CODE[@]} -gt 0 ]; then
        echo -e "\n${BR}✗ FAILED - EXIT CODE DIFFERENCES (${#FAILED_EXIT_CODE[@]} tests):${NC}"
        for test in "${FAILED_EXIT_CODE[@]}"; do
            echo -e "  ${BR}•${NC} $test"
        done
    fi
    
    if [ ${#FAILED_LEAKS[@]} -gt 0 ]; then
        echo -e "\n${BY}⚠ FAILED - MEMORY LEAKS (${#FAILED_LEAKS[@]} tests):${NC}"
        for test in "${FAILED_LEAKS[@]}"; do
            echo -e "  ${BY}•${NC} $test"
        done
    fi
    
    # Calculate success rate
    local success_rate=0
    if [ $total_tests -gt 0 ]; then
        success_rate=$(((${#PASSED_TESTS[@]} * 100) / total_tests))
    fi
    
    echo -e "\n${BB}Success rate: $success_rate%${NC}"
}

# COLOURS: BOLD RGBYW
BR="\033[1;31m"
BG="\033[1;32m"
BB="\033[1;34m"
BY="\033[1;33m"
BW="\033[1;37m"

# NO COLOR and CLEAR LINE
NC="\033[0;39m"
CL="\033[2K"

# Create valgrind suppression file if it doesn't exist
create_valgrind_suppressions() {
    cat > valgrind.supp << 'EOF'
{
   fd_leak_dev_ptmx
   Memcheck:Leak
   match-leak-kinds: reachable
   ...
   obj:/dev/ptmx
}

{
    readline_all_1
    Memcheck:Leak
    match-leak-kinds: all
    ...
    obj:/usr/lib/x86_64-linux-gnu/libreadline.so.8.2
}

{
    readline_all_2
    Memcheck:Leak
    match-leak-kinds: all
    ...
    fun:readline
}

{
    readline_history
    Memcheck:Leak
    match-leak-kinds: all
    ...
    fun:add_history
}

{
    readline_internal_alloc
    Memcheck:Leak
    match-leak-kinds: all
    ...
    fun:xmalloc
    obj:/usr/lib/x86_64-linux-gnu/libreadline.so.8.2
}

{
    readline_history_entry
    Memcheck:Leak
    match-leak-kinds: all
    ...
    fun:alloc_history_entry
}

{
    ls_malloc_leak
    Memcheck:Leak
    match-leak-kinds: all
    ...
    fun:malloc
    obj:/usr/bin/ls
}

{
    grep_malloc_leak
    Memcheck:Leak
    match-leak-kinds: all
    ...
    fun:malloc
    obj:/usr/bin/grep
}

{
    cat_malloc_leak
    Memcheck:Leak
    match-leak-kinds: all
    ...
    fun:malloc
    obj:/usr/bin/cat
}

{
    clear_malloc_leak
    Memcheck:Leak
    match-leak-kinds: all
    ...
    fun:malloc
    obj:/usr/bin/clear
}
EOF
}

# Create suppression file
create_valgrind_suppressions

echo -e "\n${BB}================================================================================${NC}"
echo -e "${BB}STARTING MINISHELL TESTS${NC}"
echo -e "${BB}================================================================================${NC}"

# ... (tus tests existentes, pero cambiando run_test por la nueva versión)

# ================================================================================
# BASIC COMMANDS
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Basic Commands${NC}"
run_test "echo \"Hello\"" "Basic echo command"
run_test "ls" "Basic ls command"
run_test "pwd" "Basic pwd command"
run_test "cd .." "Change to parent directory"
run_test "whoami" "Whoami command"
run_test "date" "Date command"

# ================================================================================
# REDIRECTIONS
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Redirections${NC}"
run_test "cat < testfile.txt" "Input redirection"
run_test "echo \"Hello\" > file.txt" "Output redirection"
run_test "echo \"Hello\" >> file.txt" "Append redirection"
run_test "grep \"pattern\" < input.txt" "Input redirection with grep"
run_test "echo \"Hello\" > file.txt 2>&1" "Combined redirection"
run_test "echo \"Hello\" >> file.txt 2>&1" "Combined append redirection"
run_test "ls > outfile" "Simple output redirection"
run_test "pwd > outfile" "pwd to file"

# Advanced redirections
run_test "< input.txt cat" "Input redirection before command"
run_test "cat < input.txt > outfile" "Input and output redirection"
run_test "grep \"line\" < input.txt > outfile" "Grep with both redirections"

# ================================================================================
# PIPES - BASIC
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Pipes - Basic${NC}"
run_test "ls | wc -l" "Basic pipe usage"
run_test "echo \"Hello\" | cat" "Pipe with cat"
run_test "echo \"Hello\" | cat | wc -l" "Multiple pipes"
run_test "ls | grep \"txt\" > output.txt" "Pipe with redirection"
run_test "ls | grep \"txt\" >> output.txt" "Pipe with append redirection"
run_test "ls | grep \"txt\" | wc -l > count.txt" "Multiple pipes with redirection"
run_test "ls | grep \"txt\" | wc -l >> count.txt" "Multiple pipes with append redirection"
run_test "echo \"Hello\" | grep \"H\"" "Pipe with grep"
run_test "echo \"Hello\" | grep \"H\" | wc -l" "Multiple pipes with grep and wc"
run_test "echo \"Hello\" | grep \"H\" > output.txt" "Pipe with redirection to file"
run_test "echo \"Hello\" | grep \"H\" >> output.txt" "Pipe with append redirection to file"
run_test "echo \"Hello\" | grep \"H\" | wc -l > count.txt" "Multiple pipes with redirection to file"
run_test "echo \"Hello\" | grep \"H\" | wc -l >> count.txt" "Multiple pipes with append redirection to file"
run_test "echo \"Hello\" | grep \"H\" | tee file.txt | wc -l" "Pipe with grep, tee, and wc"
run_test "echo \"Hello\" | tee file.txt | cat -n" "Pipe with tee and cat"

# ================================================================================
# PIPES - ADVANCED AND ERROR CASES
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Pipes - Advanced and Error Cases${NC}"
run_test "echo hola mundo | wc -w" "Word count with pipe"
run_test "ls | grep mini | wc -l" "Multiple grep and wc"
run_test "< infile ls -l | wc -l > outfile" "Input and output with pipe"
run_test "< infile grep a1 | wc -w > outfile" "Grep and word count"
run_test "< infil ls -l | wc -l > outfile" "Non-existent input file with pipe"
run_test "< infile lx -l | wx -l > outfile" "Non-existent commands with pipe"
run_test "< infile grep ee | wc -w > outfile" "Valid command in PATH"
run_test "sleep 2 | sleep 2 | sleep 2" "Multiple sleep commands"
run_test "pwd >out1 | sleep 2 | exit 42 | ls" "Mixed commands with exit"

# ================================================================================
# QUOTES AND VARIABLES
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Quotes and Variables${NC}"
run_test "echo \"Hello 'World'\"" "Mixed quotes"
run_test "echo 'Hello \"World\"'" "Mixed quotes"
run_test "echo \"Hello \$USER\"" "Variable expansion in double quotes"
run_test "echo 'Hello \$USER'" "No variable expansion in single quotes"
run_test "echo \"Hello\tWorld\"" "Tab character"
run_test "echo \"Hello\nWorld\"" "Newline character"
run_test "echo \"Hello\\World\"" "Backslash character"
run_test "echo hola" "Sin comillas"
run_test "echo \"hola mundo\"" "Comillas dobles"
run_test "echo 'hola mundo'" "Comillas simples"
run_test "echo \"Texto con ; punto y coma\"" "Semicolon inside quotes"

# ================================================================================
# NON-EXISTENT FILES AND DIRECTORIES
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Non-existent Files and Directories${NC}"
run_test "ls /nonexistent" "Non-existent directory"
run_test "cat /nonexistent/file.txt" "Non-existent file"
run_test "echo \"Hello\" > /nonexistent/file.txt" "Non-existent file for output redirection"
run_test "cat < /nonexistent/file.txt" "Non-existent file for input redirection"
run_test "./no_existe" "Non-existent executable"
run_test "cd /nonexistent_directory" "Non-existent directory for cd"

# ================================================================================
# ENVIRONMENT VARIABLES
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Environment Variables${NC}"
run_test "echo \$HOME" "Environment variable"
run_test "echo \$USER" "Environment variable"
run_test "echo \$PATH" "Environment variable"
run_test "echo \$?" "Exit status of last command"
run_test "echo \"\$HOME\"" "Variable in double quotes"
run_test "echo '\$HOME'" "Variable in single quotes"
run_test "echo \"Hello \$USER\"" "Variable expansion in double quotes"
run_test "echo 'Hello \$USER'" "No variable expansion in single quotes"
run_test "echo \$SHELL" "SHELL variable"

# ================================================================================
# BUILTIN COMMANDS - ECHO
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Builtin Commands - Echo${NC}"
run_test "echo -n \"Hello\"" "Echo with -n option"
run_test "echo -n hola mundo" "echo -n without quotes"
run_test "echo" "echo without arguments"
run_test "echo -n" "echo -n without arguments"
run_test "echo -nnnn hola" "Multiple -n flags"
run_test "echo -n -n -n hola" "Multiple separate -n flags"
run_test "echo -- hola" "echo with --"
run_test "echo hola > out1 > out2 > out3" "Multiple output redirections"

# ================================================================================
# BUILTIN COMMANDS - EXPORT
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Builtin Commands - Export${NC}"
run_test "export TEST_VAR=test" "Set environment variable"
run_test "export VAR=valor" "Create variable"
run_test "export VAR" "Export without value"
run_test "export VAR=" "Export with empty value"
run_test "export _VAR=ok" "Valid variable name with underscore"
run_test "export VAR=123" "Export numeric value"
run_test "export VAR=valor extra" "Export with spaces in value"
run_test "export" "List exported variables"

# ================================================================================
# BUILTIN COMMANDS - UNSET
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Builtin Commands - Unset${NC}"
run_test "unset TEST_VAR" "Unset environment variable"
run_test "unset _VAR" "Unset variable with underscore"
run_test "unset" "Unset without arguments"
run_test "unset VAR1 VAR2 VAR3" "Unset multiple variables"

# ================================================================================
# BUILTIN COMMANDS - ENV
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Builtin Commands - Env${NC}"
run_test "env | grep PATH" "Print environment variables and filter PATH"
run_test "env" "Print all environment variables"

# ================================================================================
# BUILTIN COMMANDS - EXIT
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Builtin Commands - Exit${NC}"
run_test "exit" "Exit without code"
run_test "exit 0" "Exit with code 0"
run_test "ls | exit" "Exit in pipe"

# ================================================================================
# CD BUILTIN - BASIC
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}CD Builtin - Basic${NC}"
run_test "cd /" "Change to root directory"
run_test "cd -" "Change to previous directory"
run_test "cd /tmp | pwd" "Doesn't cd and print working directory"
run_test "cd .." "Change to relative path"
run_test "cd src/" "Change to absolute path"
run_test "cd README.md" "Change to file instead of a directory"
run_test "cd /nonexistent" "Change to non-existent directory"
run_test "cd /tmp && cd / && cd -" "cd to previous directory sequence"
run_test "cd .." "Change to parent directory"
run_test "cd ../otro" "Change to relative path"
run_test "cd" "cd without arguments"
run_test "cd ruta/inexistente" "cd to non-existent path"
run_test "cd \"\"" "cd to empty string"
run_test "cd ." "cd to current directory"
run_test "cd /" "cd to root"

# ================================================================================
# CD BUILTIN - ADVANCED
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}CD Builtin - Advanced${NC}"

# Test: Change to a non-existent directory
mkdir /tmp/testdir_nonexistent 2>/dev/null || true
run_test "cd /tmp/testdir_nonexistent" "Change to non-existent directory"
rm -rf /tmp/testdir_nonexistent

# Test: Permission denied error
mkdir /tmp/testdir_noaccess
chmod 000 /tmp/testdir_noaccess
run_test "cd /tmp/testdir_noaccess" "Change to a directory with no permissions"
chmod 755 /tmp/testdir_noaccess
rm -rf /tmp/testdir_noaccess

# Test: Directory with spaces
mkdir '/tmp/My Directory with Spaces'
run_test "cd '/tmp/My Directory with Spaces'" "Change to directory with spaces"
rm -rf '/tmp/My Directory with Spaces'

# Test: Directory with special characters
mkdir '/tmp/special!dir@#'
run_test "cd '/tmp/special!dir@#'" "Change to directory with special characters"
rm -rf '/tmp/special!dir@#'

# Test: Home directory
export HOME=/tmp/test_home
mkdir $HOME
run_test "cd" "Change to home directory without arguments"
rm -rf /tmp/test_home

# Test: Relative directory
mkdir /tmp/testdir_relative
run_test "cd /tmp/testdir_relative" "Change to relative directory"
rm -rf /tmp/testdir_relative

# Test: Non-readable directory
mkdir /tmp/testdir_nonreadable
chmod 000 /tmp/testdir_nonreadable
run_test "cd /tmp/testdir_nonreadable" "Change to non-readable directory"
chmod 755 /tmp/testdir_nonreadable
rm -rf /tmp/testdir_nonreadable

# Test: Symbolic link to directory
mkdir /tmp/symlink_dir
ln -s /tmp/symlink_dir /tmp/symlink_test
run_test "cd /tmp/symlink_test" "Change to symbolic link directory"
rm -rf /tmp/symlink_dir /tmp/symlink_test

# Test: Symbolic link to file
touch /tmp/testfile_symlink
ln -s /tmp/testfile_symlink /tmp/symlink_file
run_test "cd /tmp/symlink_file" "Change to symbolic link to file"
rm -f /tmp/testfile_symlink /tmp/symlink_file

# Test: Path with backslash
mkdir '/tmp/dir_with\backslash'
run_test "cd '/tmp/dir_with\backslash'" "Change to path with backslash character"
rm -rf '/tmp/dir_with\backslash'

# Test: Directory with no execute permission
mkdir /tmp/testdir_noexec
chmod 000 /tmp/testdir_noexec
run_test "cd /tmp/testdir_noexec" "Change to directory with no execute permission"
chmod 755 /tmp/testdir_noexec
rm -rf /tmp/testdir_noexec

run_test "cd '  '" "Change to directory with spaces as path"

# ================================================================================
# HEREDOC
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Heredoc${NC}"
run_test "$(cat <<EOF
cat << STOP
line1
line2
STOP
EOF
)" "Basic heredoc"

run_test "$(cat <<EOF
wc -l << STOP
line1
line2
line3
STOP
EOF
)" "Heredoc with wc"

run_test "$(cat <<EOF
ls -l << stop | wc -l > outfile
stop
EOF
)" "Heredoc in pipe"

# ================================================================================
# MULTIPLE REDIRECTIONS
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Multiple Redirections${NC}"
run_test "ls >outfile1 >outfile2 >outfile3" "Multiple output redirections"
run_test "ls >outfile1 >>outfile2 >outfile3" "Mixed output redirections"
run_test "ls >outfile1 >>outfile2 >>outfile3" "Multiple append redirections"
run_test "< input.txt > outfile cat" "Input and output redirection"
run_test "ls >outfile1 >>outfile2 >outfile3 | cat >outfile4 >>outfile5 >outfile6" "Multiple redirections with pipe"

# ================================================================================
# RETOKENIZATION
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Retokenization${NC}"
run_test "$(cat <<EOF
export VAR='ls    -l'
echo \$VAR
EOF
)" "Check retokenization of exported variable"

run_test "$(cat <<EOF
export VAR='ls    -l'
"\$VAR"
EOF
)" "Check retokenization of exported variable"

run_test "$(cat <<EOF
export VAR='ls    -l'
'\$VAR'
EOF
)" "Check retokenization of exported variable"

run_test "$(cat <<EOF
export ALGO='s  -l'
l\$ALGO
EOF
)" "Check retokenization of exported variable"

run_test "$(cat <<EOF
export ALGO='s  -l'
l"\$ALGO"
EOF
)" "Check retokenization of exported variable"

run_test "$(cat <<EOF
export VAR="ls    -l    /tmp"
echo \$VAR
EOF
)" "Check retokenization with multiple spaces"

run_test "$(cat <<EOF
export VAR="ls -l /tmp/special!dir@#"
echo \$VAR
EOF
)" "Check retokenization with special characters"

run_test "$(cat <<EOF
export VAR="echo \$VAR"
echo \$VAR
EOF
)" "Check retokenization with escaped characters"

run_test "$(cat <<EOF
export VAR="ls -l; pwd"
echo \$VAR
EOF
)" "Check retokenization with multiple commands"

run_test "$(cat <<EOF
export VAR="World"
echo 'Hello \$VAR'
EOF
)" "Check variable substitution inside quotes"

# ================================================================================
# COMMAND EXECUTION
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Command Execution${NC}"
run_test "./minishell -c \"ls -l\"" "Command execution"
run_test "./a.out" "Execute compiled program"

# ================================================================================
# ERROR CASES
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Error Cases${NC}"
run_test "$(cat <<EOF
export VAR="'hello' | ls"
echo \$VAR
EOF
)" "Complex variable expansion"

run_test "$(cat <<EOF
export VAR="echo 'Hello' > output.txt"
echo \$VAR
EOF
)" "Variable with redirection"

run_test "$(cat <<EOF
mkdir -p t1/t2/t3
cd t1/t2/t3
rm -rf ../../../t1
pwd
cd ..
cd ..
cd ..
pwd
EOF
)" "Directory manipulation"

run_test "$(cat <<EOF
mkdir -p t1/t2/t3
cd t1/t2/t3
rm -rf ../../../t1
pwd
cd ..
cd -
cd -
cd ..
cd ..
pwd
EOF
)" "Complex cd operations"

run_test "$(cat <<EOF
mkdir -p t1/t2/t3
cd t1/t2/t3
rm -rf ../../../t1
env | grep PWD
cd ..
env | grep PWD
cd ..
env | grep PWD
cd ..
env | grep PWD
EOF
)" "PWD environment variable"

# ================================================================================
# EDGE CASES AND STRESS TESTS
# ================================================================================
echo -e "\n${BB}--------------------------------------------------------------------------------${NC}"
echo -e "${BB}Edge Cases and Stress Tests${NC}"

# Empty commands and edge cases
run_test "" "Empty command"
run_test "   " "Only spaces"
run_test ";;" "Double semicolon"
run_test "||" "Double pipe"
run_test "&&" "Double ampersand"

# Complex pipe sequences
run_test "ls | grep mini | wc -l | cat -n | head -5" "Long pipe sequence"
run_test "echo test | cat | cat | cat | cat | wc -l" "Multiple cat pipes"

# Mixed operators
run_test "ls > outfile && cat outfile" "Redirection and logical AND"
run_test "false || echo \"should print\"" "Logical OR with false"

# Permission tests
run_test "touch no_permission && chmod 000 no_permission && cat no_permission" "No read permission"
run_test "chmod 000 no_write && echo test > no_write" "No write permission"

# Clean up temporary files
rm -f "$TMP_MINISHELL_OUTPUT" "$TMP_BASH_OUTPUT" "$DIFF_OUTPUT" "$TMP_VALGRIND_OUTPUT"
cleanup_test_environment

# Print final summary
print_summary

echo -e "\n${BB}================================================================================${NC}"
echo -e "${BB}ALL TESTS COMPLETED${NC}"
echo -e "${BB}================================================================================${NC}"