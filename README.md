# Concordance program
This repo houses code for a program that produces a 'concordance'
(list of word frequences with sentence numbers) of its plain text
input from either standard input or a file.

## Use
These example usages assume you are in a build directory and have the
relative paths set accordingly.  See the below section about building
and running concordance for more. Here is an example usage of the
`concordance` program from a build directory:

```sh
$ ./src/concordance --filename ../example.txt
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

Each output line follows the following format:

```
<word> {<number-of-occurrences>:<sentence-occurrence1>,<sentence-occurrence2>...<sentence-occurrenceN>}
```

To parse from standard input, don't supply any arguments:

```sh
$echo "hello world!" | ./src/concordance
hello {1:1}
world {1:1}
```


There is also a `--help` argument, which will produce a usage message.

## Special words

Some words are 'special' to the tokenizer (the thing that breaks text
into constituent words). These are words with periods in them, since
these normally signal the end of sentence but do not for special
cases, e.g.: e.g., i.e., and so on. In order to deal with this, there
is a small built-in database of special words. However, in order to
add all the special words you might want, there is an option to add a
special word file containing a list of such words (one word per line),
the `-s` or `--special-words` option. There is an example special
words file and text included in the repo, which you can use as follows
(assuming you are in your build directory, see the section below about
building and running):

```sh
$ ./src/concordance --special-words ../example-special-words.txt --filename ../example-with-special-words.txt
a {1:2}
am {1:1}
and {1:1}
announce {1:1}
bjarne {1:1}
d.d.s. {1:1}
enough {1:2}
esq. {1:1}
file {1:1}
for {1:2}
however {1:2}
i {1:1}
is {3:1,1,2}
it {1:2}
long {2:1,2}
m.d. {1:1}
my {1:1}
name {1:1}
not {1:1}
p.h.d. {1:1}
pleased {1:1}
quite {1:1}
simple {1:2}
stroustrup {1:1}
test {1:2}
text {1:1}
that {1:1}
this {1:1}
to {1:1}
very {1:1}
```

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

## Building and testing `concordance`
You can build with unit tests in the following way:

```sh
$ mkdir build
$ cd build
$ cmake -DBUILD_UNIT_TESTS=Yes ..
$ make -j
$ ctest -VV .
```

You need to have `gtest` installed to build with tests.

