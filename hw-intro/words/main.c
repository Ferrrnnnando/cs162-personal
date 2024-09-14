/*

  Word Count using dedicated lists

*/

/*
Copyright © 2019 University of California, Berkeley

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <assert.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "word_count.h"

/* Global data structure tracking the words encountered */
WordCount *word_counts = NULL;

/* The maximum length of each word in a file */
#define MAX_WORD_LEN 64
#define SPACE ' '
#define NEW_LINE '\n'
#define STRING_TERMINATOR '\0' 

bool is_word_character(char c) {
  return (c != SPACE) && (c != NEW_LINE);
}

bool is_word_terminator(char c) {
  return (c == SPACE) || (c == NEW_LINE);
}

/*
 * 3.1.1 Total Word Count
 *
 * Returns the total amount of words found in infile.
 * Useful functions: fgetc(), isalpha().
 */
int num_words(FILE* infile) {
  int num_words = 0;
  char prev_c = SPACE;
  int c;  // store current character read from stream
  
  while ((c = fgetc(infile)) != EOF) {
    if (is_word_terminator(c) && is_word_character(prev_c)) {
      num_words++;
    }
    prev_c = c;
  }

  if (is_word_character(prev_c)) {
    num_words++;
  }

  return num_words;
}

static char* allocate_buffer(size_t new_size) {
  return malloc(sizeof(char) * new_size);
}

/*
 * 3.1.2 Word Frequency Count
 *
 * Given infile, extracts and adds each word in the FILE to `wclist`.
 * Useful functions: fgetc(), isalpha(), tolower(), add_word().
 * 
 * As mentioned in the spec, your code should not panic or
 * segfault on errors. Thus, this function should return
 * 1 in the event of any errors (e.g. wclist or infile is NULL)
 * and 0 otherwise.
 */
int count_words(WordCount **wclist, FILE *infile) {
  if (infile == NULL) {
    return 1;
  }

  size_t buffer_size = 16;
  char prev_c = SPACE;
  int c;  // store current character read from stream
  char* buffer = allocate_buffer(buffer_size);
  int buffer_write_idx = 0;

  while ((c = fgetc(infile)) != EOF) {
    // reallocate a larger buffer if remaining buffer volume is not sufficient
    if (buffer_write_idx >= buffer_size) {
      const size_t old_buffer_size = buffer_size;
      buffer_size *= 2;
      char* new_buffer = allocate_buffer(buffer_size);
      memcpy(new_buffer, buffer, old_buffer_size * sizeof(char));
      free(buffer);
      buffer = new_buffer;
    }

    if (is_word_character(c)) {
      buffer[buffer_write_idx++] = c;
    }
    if (is_word_terminator(c) && is_word_character(prev_c)) {
      buffer[buffer_write_idx] = STRING_TERMINATOR;  // add string terminator
      add_word(wclist, buffer); // a complete word has been made to buffer
      buffer_write_idx = 0;
    }
    prev_c = c;
  }

  if (is_word_character(prev_c)) {
    buffer[buffer_write_idx] = STRING_TERMINATOR;  // add string terminator
    add_word(wclist, buffer);
    buffer_write_idx = 0;
  }

  return 0;
}

// In trying times, displays a helpful message.
static int display_help(void) {
	printf("Flags:\n"
	    "--count (-c): Count the total amount of words in the file, or STDIN if a file is not specified. This is default behavior if no flag is specified.\n"
	    "--frequency (-f): Count the frequency of each word in the file, or STDIN if a file is not specified.\n"
	    "--help (-h): Displays this help message.\n");
	return 0;
}

/*
 * Handle command line flags and arguments.
 */
int main (int argc, char *argv[]) {

  // Count Mode (default): outputs the total amount of words counted
  bool count_mode = true;
  int total_words = 0;

  // Freq Mode: outputs the frequency of each word
  bool freq_mode = false;

  FILE *infile = NULL;

  // Variables for command line argument parsing
  int i;
  static struct option long_options[] =
  {
      {"count", no_argument, 0, 'c'},
      {"frequency", no_argument, 0, 'f'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0}
  };

  // Sets flags
  while ((i = getopt_long(argc, argv, "cfh", long_options, NULL)) != -1) {
      switch (i) {
          case 'c':
              count_mode = true;
              freq_mode = false;
              break;
          case 'f':
              count_mode = false;
              freq_mode = true;
              break;
          case 'h':
              return display_help();
      }
  }

  if (!count_mode && !freq_mode) {
    printf("Please specify a mode.\n");
    return display_help();
  }

  /* Create the empty data structure */
  init_words(&word_counts);

  if ((argc - optind) < 1) {
    // No input file specified, instead, read from STDIN instead.
    infile = stdin;
    total_words = num_words(infile);
    fclose(infile);
  } else {
    // At least one file specified. Useful functions: fopen(), fclose().
    // The first file can be found at argv[optind]. The last file can be
    // found at argv[argc-1].
    for(int i = 2; i < argc; i++) {
      FILE* f = fopen(argv[i], "r");
      if (f == NULL) {
        printf("file: '%s' cannot be opened!\n", argv[i]);
        continue;
      }

      if (count_mode) {
        total_words += num_words(f);
      } else {
        int ret = count_words(&word_counts, f);
        if (ret == 1) {
          printf("count_words: failed\n");
        }
      }
      fclose(f);
    }
  }

  if (count_mode) {
    printf("The total number of words is: %i\n", total_words);
  } else {
    wordcount_sort(&word_counts, wordcount_less);
    printf("The frequencies of each word are: \n");
    fprint_words(word_counts, stdout);
}
  return 0;
}
