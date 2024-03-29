# Concordance program
This repo houses code for a program that produces a 'concordance'
(list of word frequences with sentence numbers) of its plain text
input from either standard input or a file.

## Use
These example usages assume you are in a build directory and have the
relative paths set accordingly.  See the below section about [building
and running concordance](#building-and-running-concordance) for more. Here is an example usage of the
`concordance` program from a build directory:

```sh
$ ./src/concordance --filename ../example.txt
a. a {2:1,1}
b. all {1:1}
c. alphabetical {1:1}
d. an {2:1,1}
e. appeared {1:2}
f. arbitrary {1:1}
g. bonus {1:2}
h. concordance {1:1}
i. document {1:1}
j. each {2:2,2}
k. english {1:1}
l. frequencies {1:1}
m. generate {1:1}
n. given {1:1}
o. i.e. {1:1}
p. in {2:1,2}
q. label {1:2}
r. labeled {1:1}
s. list {1:1}
t. numbers {1:2}
u. occurrence {1:2}
v. occurrences {1:1}
w. of {1:1}
x. program {1:1}
y. sentence {1:2}
z. text {1:1}
aa. that {1:1}
bb. the {1:2}
cc. which {1:2}
dd. will {1:1}
ee. with {2:1,2}
ff. word {3:1,1,2}
gg. write {1:1}
hh. written {1:1}
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
a. a {2:2,2}
b. am {1:1}
c. and {1:1}
d. announce {1:1}
e. bjarne {1:1}
f. d.d.s. {1:1}
g. e.g. {1:2}
h. enough {1:2}
i. esq. {1:1}
j. file {1:1}
k. for {1:2}
l. however {1:2}
m. i {1:1}
n. is {3:1,1,2}
o. it {1:2}
p. long {2:1,2}
q. m.d. {1:1}
r. mr. {1:1}
s. my {1:1}
t. name {1:1}
u. not {1:1}
v. p.h.d. {1:1}
w. pleased {1:1}
x. quite {1:1}
y. simple {1:2}
z. stroustrup {1:1}
aa. test {2:2,2}
bb. text {1:1}
cc. that {1:1}
dd. this {1:1}
ee. to {1:1}
ff. unit {1:2}
gg. very {1:1}
```

See the file `example-special-words.txt` for more information about
the file format, it details how to deal with prefix special words and
ones that can end sentences.

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

This will link against `gtest`, you need to have
[gtest](https://github.com/google/googletest) installed to build with
tests.

## Some minor notes on special words

In English, if I write 'My name is so-and-so Ph.D. How are you?', this
counts as two sentences, even though 'Ph.D.' doesn't always end a
sentence, see [this
article](https://www.quickanddirtytips.com/education/grammar/ending-sentence-abbreviation)
for more information. The text parser automatically detects if a
'special' word from the database ends with a period and if the first
letter after it is capitalized. If so, it will treat this as a new sentence.

But wait! What about Mr. So-and-so? 'Mr.' does not end the
sentence. For these cases a small list of non-sentence-ending special
words is kept by the program, which can be extended with the special
words file (see the `example-special-words.txt` file for more
information, and the `--help` command of `concordance`, and the
[relevant section in this document](#special-words)), so that
'Mr. English', for example, doesn't count as two sentences.


