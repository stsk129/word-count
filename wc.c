#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXLEN 100

int getword(char *, int);
struct tnode *addtree(struct tnode *, char *);
struct tnode *sortlist(struct tnode *);
void treeprint(struct tnode *);

/* Tree node for binary tree.
 * Holds the word string, it's count as well as pointers to left, mid and right tree nodes.
 * Mid tree node is used to store words with the same count after resorting 
 */
struct tnode {
    char *word;
    int count;
    struct tnode *left;
    struct tnode *mid;
    struct tnode *right;
};

/* simple word count program
 * lists all words by decreasing order of frequency
 */
int main()
{
    char word[MAXLEN];                          /* array for single words */
    struct tnode *root;                         /* root node of binary tree */
    root = NULL;
   
    while (getword(word, MAXLEN) != EOF)        /* read words one at a time till EOF */
        if (isalpha(word[0]))                   /* define word as starting with a char */
            root = addtree(root, word);         /* add word to binary tree, update count */
    root = sortlist(root);                      /* resort binary tree by frequency of words */
    treeprint(root);                            /* print words from tree recursively */
    return 0;
}

/* read the next word into the provided array, skip whitespace */
int getword(char *word, int lim)
{
    int c, getch(void);
    void ungetch(int);
    char *w = word;
    
    while (isspace(c = getch()))
        ;
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for ( ; --lim > 1; w++)        
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}

struct tnode *talloc(void);
char *strdup_v2(char *);

/* construct binary tree and add words to the tree by lexicographic sort (strcmp) */
struct tnode *addtree(struct tnode *p, char *word)
{
    int cond;

    if (p == NULL) {                                    /* if node does not exist, create new node */
        p = talloc();
        p->word = strdup_v2(word);
        p->count = 1;
        p->left = p->mid = p->right = NULL;
    } else if ((cond = strcmp(word, p->word)) == 0)     /* if word already exists, increment count */
        p->count++;
    else if (cond < 0)                                  /* else continue searching tree */
        p->left = addtree(p->left, word);
    else
        p->right = addtree(p->right, word);
    
    return p;
}

struct tnode *addcount(struct tnode *, struct tnode *);

/* go to leftmost leaf, i.e. smallest word
 * set leftmost leaf as origin for resorted tree
 * go up the old tree and sort node into new tree
 */
struct tnode *sortlist(struct tnode *p)
{
    static struct tnode *origin = NULL;
    
    if (p->left != NULL)
        sortlist(p->left);
    if (p->right !=NULL)
        sortlist(p->right);
    if (origin == NULL)
        origin = p;
    else {
        p->left = p->right = NULL;
        addcount(p, origin);
    }
    return origin;            
}


/* print entire tree with words preceded by count starting from leftmost node */
void treeprint(struct tnode *p)
{
    if (p != NULL) {
        treeprint(p->right);
        printf("%4d %s\n", p->count, p->word);
        treeprint(p->mid);
        treeprint(p->left);
    }
}

struct tnode *talloc(void)
{
    return (struct tnode *) malloc(sizeof(struct tnode));
}

char *strdup_v2(char *word)
{
    char *p;
    
    p = (char *) malloc(strlen(word)+1);
    if (p != NULL)
        strcpy(p, word);
    return p;
}

/* add node to correct position by comparing count to new origin node 
 * the comparison is done recursively starting from the root node of the newly created tree
 * if two words are found to have the same count, the mid-pointer is used to insert node
 * 
 */
struct tnode *addcount(struct tnode *p, struct tnode *origin)
{
    int cond;
    
    if (origin == NULL)
        origin = p;
    else if ((cond = origin->count-p->count) == 0)
        origin->mid = addcount(p, origin->mid);
    else if (cond > 0)
        origin->left = addcount(p, origin->left);
    else
        origin->right = addcount(p, origin->right);
    return origin;
}

/* create stack to allow for one char pushback of getchar() function */
#define BUFSIZE 100

int buf[BUFSIZE];
int bufp = 0;

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
