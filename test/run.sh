#!/bin/bash
#
# Perform checks on the nushell project.

# Constants
readonly RED='\033[31m'
readonly GREEN='\033[32m'
readonly RESET='\033[37m'

########################################
# Print OK in green, followed by a newline.
# Globals:
#   None
# Arguments:
#   None
# Returns:
#   None
########################################
function print_ok()
{
    echo -e "[${GREEN}OK${RESET}]"
}

#######################################
# Print KO in red, followed by a newline.
# Globals:
#   None
# Arguments:
#   None
# Returns:
#   None
#######################################
function print_ko()
{
    echo -e "[${RED}KO${RESET}]"
}

#######################################
# kill the shell in clean way and remove temporary files.
# Globals:
#   SHELL
#   OUTPUTFILE
#   ERROROUTPUTFILE
# Arguments:
#   None
# Returns:
#   None
#######################################
function stop_shell()
{
    if [[ $(pidof "$SHELL" | wc -l) -ne 0 ]]; then
        killall -9 "$SHELL" 2>&1 > /dev/null
    fi
    rm -f "$OUTPUTFILE" "$ERROROUTPUTFILE" "$LTRACEOUTPUTFILE"
}

# load configuration
source config

# Locate all tests and launch them
for dir in $(ls -d "$TESTDIR"/*/); do
    echo "> $dir"
    for testname in $(ls "$dir" | grep -v "~$"); do
        echo -n "   # $testname: "
        source "$dir$testname"
    done
done

# clean up
rm -f "$OUTPUTFILE" "$ERROROUTPUTFILE" "$LTRACEOUTPUTFILE"
