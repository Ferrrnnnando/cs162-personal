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

word_count provides lists of words and associated count

Functional methods take the head of a list as first arg.
Mutators take a reference to a list as first arg.
*/

#include "word_count.h"

#include "assert.h"

/* Basic utilities */

char *new_string(char *str) {
  char *new_str = (char *) malloc(strlen(str) + 1);
  if (new_str == NULL) {
    return NULL;
  }
  return strcpy(new_str, str);
}

int init_words(WordCount **wclist) {
  /* Initialize word count.
     Returns 0 if no errors are encountered
     in the body of this function; 1 otherwise.
  */
  *wclist = NULL;
  return 0;
}

size_t len_words(WordCount *wchead) {
  /* Return -1 if any errors are
     encountered in the body of
     this function.
  */
  size_t len = 0;
	while (wchead != NULL)
	{
		len++;
		wchead = wchead->next;
	}
  return len;
}

WordCount *find_word(WordCount *wchead, char *word) {
  /* Return count for word, if it exists */
  WordCount* node = wchead;
  while (node != NULL && strcmp(node->word, word) != 0)
  {
    node = node->next;
  }
  return node; 
}

static WordCount* create_word(char* s, WordCount* next) {
    WordCount* word = malloc(sizeof(WordCount));
    word->word = new_string(s);
    word->count = 1;
    word->next = next;
    return word;
}

/*
 * Comparator to sort list by frequency.
 * Useful function: strcmp().
 */
bool wordcount_less(const WordCount *wc1, const WordCount *wc2) {
  assert(wc1 != NULL && wc2 != NULL);

  if (wc1 == wc2) {
    return false;
  }

  if (wc1->count < wc2->count) {
    return true;
  }

  if ((wc1->count == wc2->count) && (strcmp(wc1->word, wc2->word) < 0)) {
    return true;
  }

  return 0;
}

int add_word(WordCount **wclist, char *word) {
  /* If word is present in word_counts list, increment the count.
     Otherwise insert with count 1.
     Returns 0 if no errors are encountered in the body of this function; 1 otherwise.
  */
  WordCount* word_found = find_word(*wclist, word);
  if (word_found != NULL) {
    word_found->count++;
    return 0;
  }

	WordCount* new_word = create_word(word, NULL);
  if (new_word == NULL) {
    return 1;
  }
  // insert new word at head
  new_word->next = *wclist;
  *wclist = new_word;
 	return 0;
}

void fprint_words(WordCount *wchead, FILE *ofile) {
  /* print word counts to a file */
  WordCount *wc;
  for (wc = wchead; wc; wc = wc->next) {
    fprintf(ofile, "%i\t%s\n", wc->count, wc->word);
  }
}

/*
 * Swap and return front node after swap
 */
static WordCount* swap_adjacent(WordCount* node1, WordCount* node2) {
  WordCount* node2_old_next = node2->next;
  node2->next = node1;
  node1->next = node2_old_next;
  return node2;
}

void wordcount_sort(WordCount **wclist, bool less(const WordCount *, const WordCount *)) {
  size_t num_words = len_words(*wclist);
  bool has_swapped;
  WordCount** h;
  for (int i = 0; i < num_words; i++) {
    has_swapped = false;
    h = wclist;
    for (int j = 0; j < num_words - i - 1; j++) {
      WordCount* node1 = *h;
      WordCount* node2 = node1->next;
      if (!less(node1, node2)) {
        has_swapped = true;
        *h = swap_adjacent(node1, node2);
      }
      h = &((*h)->next);
    }
    if (!has_swapped) {
      return;
    }
  }
}
