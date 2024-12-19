#!/bin/bash

set -e
set -u
set -x

rm -rf /tmp/snmp-*

cd build && make -s

export SNMP_SAVE_TMPDIR=yes DYNAMIC_ANALYZER="/opt/local/bin/valgrind --trace-children=yes --trace-children-skip=/usr/bin/env,/bin/sed,/bin/ls,/bin/sh --track-origins=no --leak-check=full --suppressions=${PWD}/net-snmp-memcheck.supp --gen-suppressions=all"

make -s test
cd testing || exit $?

./RUNFULLTESTS -g unit-tests
echo
echo 'Output files with Valgrind complaints:'
if grep -rEl 'Invalid | uninitialised |Invalid free|Mismatched free|Source and destination overlap' /tmp/snmp-*; then
  exit 1
else
  exit 0
fi

