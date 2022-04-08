# Concordance program
This repo houses code for a program that produces a 'concordance'
(list of word frequences with sentence numbers) of its plain text
input from either standard input or a file.

Example usage:

```sh
$ concordance --filename example.txt
a {2:1,1}
all {1:1}
alphabetical {1:1}
an {2:1,1}
appeared {1:2}
arbitrary {1:1}
bonus {1:2}
concordance {1:1}
document {1:1}
each {2:2,2}
english {1:1}
frequencies {1:1}
generate {1:1}
given {1:1}
i.e. {1:1}
in {2:1,2}
label {1:2}
labeled {1:1}
list {1:1}
numbers {1:2}
occurrence {1:2}
occurrences {1:1}
of {1:1}
program {1:1}
sentence {1:2}
text {1:1}
that {1:1}
the {1:2}
which {1:2}
will {1:1}
with {2:1,2}
word {3:1,1,2}
write {1:1}
written {1:1}
```

To parse from standard input, don't supply any arguments. 

There is also a `--help` argument.

## Building and running `concordance`

This repo follows a fairly typical `cmake` layout. To build, you can do:

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make -j
```

At that point, you can run the `concordance` program in `src`:

```sh
src/concordance --file ../example.txt
```

## Building testing `concordance`
You can build with unit tests in the following way:

```sh
$ mkdir build
$ cd build
$ cmake -DBUILD_UNIT_TESTS=Yes ..
$ make -j
$ ctest -VV .
```

You need to have `gtest` installed to build with tests.

