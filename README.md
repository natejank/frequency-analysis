# Frequency Analysis

This is an experiment in doing frequency analysis over large amounts of text.
This was originally accomplished in C (found in `c/`), but I decided to 
re-implement it in Julia because it allowed me to maintain most of the speed
from C while making processing substantially easier, as it included tools
for working with Unicode text, as well as other string processing utilities.

The program is `freq.jl`, which relies on library functions found at
`Freqency.jl`.  The program aggregates data from inputs, and outputs a
CSV of words to stdout.
