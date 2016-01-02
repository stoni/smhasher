# smhasher

Taken from google-code

## Warning

This is WIP... everything quite messy structure/code etc.. its just ment for testing for now

### Notes

#### Other smhasher github repos

Just noticed rurban's extensions of smhasher on github along with a hash-bench for perl5

* https://github.com/rurban/smhasher
* https://github.com/rurban/perl-hash-stats

Copied over some hash tests from there

#### Interesting input

* http://www.overclock.net/t/1319572/benchmarking-the-fastest-hash-function/20

In the author of the fast FNV hashes explains that his he has optimized the functions for the Intel Sandy-Bridge architecture.
However in 2016 we have Skylake and formerly Hashwell which feature 1 ALU more then Sandy bridge used to have. Therefore the FNV algorithm might be much faster on 
newer HW if the loop was unrolled 4 times and not 3. Referring to FNV32a_YoshimitsuTRIAD
