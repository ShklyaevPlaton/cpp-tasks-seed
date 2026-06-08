#!/bin/bash

set -e

echo "Integration tests..."

./gauss AB.csv > result.csv

if diff -w result.csv expected.csv; then
    echo "Integration test passed"
else
    echo "Integration test failed"
    exit 1
fi
