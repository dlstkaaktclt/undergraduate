/*
 * mm.c - dynamic memory allocator based segregated fit.
 *
 * Each block has header.
 *
 * Header form
 *
 *  31 30 29.....       4 3 2 1 0
 *  -----------------------------
 *   | size of block       |  f a
 *  -----------------------------
 *
 *  31~3 bits : size of this block
 *  2 bit  : don't use
 *  1 bit  : forward block allocate.
 *           if forward block is allocated, this has value 1, else 0
 *  0 bit  : allocate.
 *           if this block allocated, this has value 1, else 0
 *
 * Heap architecture
 *
 * start                                        end
 * heap                                        heap
 *
 * -------------------------------------------------
 * pad |  prologue       | blocks         |epilogue
 *     |  block (sizeset)|                |   block
 * -------------------------------------------------
 *
 * prologue block architecture (sizesetnum + 2 words)
 *
 * first word : header, size : (SIZESETNUM + 2) * WORDSIZE, forawardalloc : 1, alloc : 1
 * second word ~ SIZESETNUM + 1 word :
 *        sizeset first pointers.
 *        these pointers point to sizeset's first block.
 *        if sizeset has no elements, then it's value is NULL.
 * last word : end of sizeset. it has value 0x1
 *
 * sizeset : classified by block size.
 *           first : 16~31 byte
 *           second: 32~63 byte
 *                  ...
 *
 * this implementaion has 12 sizeset,
 * last sizeset has blocks size is more than 32768 bytes. (include 32768 byte size)
 *
 * epilogue block architecture (one word)
 *
 * first word : header, size : 0, forwardalloc : it depends. alloc : 1
 *
 *
 *
 * Allocate block architecture
 * ---------------------------
 * Header  |  Payload
 *         |
 * ---------------------------
 *
 * Header  : one word. (4byte)
 * Payload : user use this space.
 *
 *
 * Free block architecture
 * --------------------------------------------------------------
 * Header |  prev free block | next free block | ... |Footer
 *        |  pointer         | pointer         | ... |(same as Header)
 * --------------------------------------------------------------
 *
 * Header : one word. (4byte)
 * Footer : one word. (4byte)
 * prev free block pointer : one word. (4byte) point to prev free block in it's sizeset.
 * next free block pointer : one word. (4byte) point to next free block in it's sizeset.
 *
 * SIZE set architecture
 *
 * ---------------          ---------------------       ---------------------
 * Sizeset first |  ---->   |  sizeset's        |  ---> | sizeset's         |  ->
 *     pointer   |          |  first free block |  <--- | second free block |  <- ...
 * ---------------          ---------------------       ---------------------
 *             ---------------------       ---------------------
 *  ...---->   |  sizeset's        |  ---> | sizeset's         |
 *    <----    |  nth free block   |  <--- | last free block   |
 *             ---------------------       ---------------------
 *  they are connected by free block pointer.
 *  if they has no refernce, then has NULL value in it's free block pointer.
 *
 *
 *  allocator find free block by using sizeset pointers. (placing)
 *  1. find correct sizeset by size.
 *  2. step by step, it sees size of free block.
 *     if free block size is more than size of to allocate, use that.
 *     else, go to next freeblock by using freeblock's next freeblock pointer.
 *  3. if that sizeset has no block to allocate, go to next sizeset.
 *  4. if all of sizeset has no freeblock to allocate, expand heap and allocate.
 *
 *  allocator allocate free block (spliting)
 *  1. if freeblock size is more than allocatesize + Minblocksize, split that.
 *  2. else, just allocate.
 *  spliting by this, if allocated block size is less than SMALLBLOCKSIZE (in this implement, 80 byte, CHUNKSIZE/50)
 *  place allocated block to forward, else place allocated block to rear.
 *  this method reduce external fragmentation
 *  by preventing small size allocate block divide heap.
 *
 *  allocate sum free block (coalescing)
 *  1. if freeblock's prev block is free block, coalesce. (using forward alloc field of header)
 *  2. if freeblock's next block is free block, coalesce. (step to next block's header, and get alloc field of next block header.)
 *                                                        (this is possible because current blockpointer + it's block size is next blockpointer.)
 *
 *  allocator free allocated block
 *  1. free allocated block, and coalesce this block to near free block.
 *  2. add free block to correct sizeset. to sizeset's head position.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your information in the following struct.
 ********************************************************/
team_t team = {
  /* Team name : Your student ID */
  "2014-16757",
  /* Your full name */
  "KimBochang",
  /* Your student ID, again  */
  "2014-16757",
  /* leave blank */
  "",
  /* leave blank */
  ""
};

/* DON'T MODIFY THIS VALUE AND LEAVE IT AS IT WAS */
static range_t **gl_ranges;

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* my custom macros  */
#define WSIZE 4
#define DSIZE 8
#define HEADERSIZE 4 /* header size of implementation. */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */
#define MINBLOCKSIZE (4*HEADERSIZE) /* minimum allocated or free block size. 4*Headersize = 16bytes. */
#define SIZESETNUM 12       /* number of sizeset */
#define SMALLBLOCKSIZE 96 /*small block size, used by place function. GET by CHUNKSIZE/40 '=. 96~ */

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

#define PACK(size, alloc, forwardalloc) ((size) | (alloc) | ((forwardalloc)<<1)) /* packing block header. currently, don't use */
#define FREEPACK(size, alloc, forwardalloc) ((size) | (alloc) | ((forwardalloc)<<1)) /* packing freeblock header */
#define ALLOCPACK(size, alloc, forwardalloc, internalFrag) ((size) | (alloc) | (((forwardalloc)<<1)) | (((internalFrag)<<2))) /* packing allocated block header. I don't use internalFrag */
#define ISINTERNALFRAG(blocksize, payloadsize) ((((blocksize) - (HEADERSIZE) - (payloadsize)) > (HEADERSIZE)) ? 1 : 0)
/* (** I DON'T USE THIS **) if allcated block has more than headersize internelfragmentation, it should make footer */



/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))

/* this function has no difference with above ones, but it is useful to debugging */
#define PUTPTR(p, val) (*(unsigned int **)(p) = (unsigned int *) (val))
#define GETPTR(p)       (*(unsigned int **)(p))

/* Read the size and allocated fields from address p. these macros should be used in header.*/
#define GET_SIZE(p)  		(GET(p) & ~0x7)
#define GET_ALLOC(p) 		(GET(p) & 0x1)
#define GET_FORWARDALLOC(p)     ((GET(p) & 0x2)>>1)
#define SET_FORWARDALLOC(p,forwardalloc)    (PUT((p), (((GET(p)) & ~0x2) | ((forwardalloc)<<1))))

#define GET_INTERNALFRAG(p)     ((GET(p) & 0x4)>>2) /* This one doesn't used */



/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)        ((char *)(bp) - WSIZE)
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) // FTRP must not be called when block has no header

#define ISBLOCKHASFOOTER(bp) ((GET_ALLOC((HDRP(bp)))) && (!(GET_INTERNALFRAG(HDRP(bp)))) ? 0 : 1) // (** I Don't Use this **)

/* Given block ptr bp, compute address of next and previous block pointer */
#define NEXT_BLKP(bp)     ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)     ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) // PREV_BLKP Must not be called when forward block is allocated
#define CANACCESSPREV(bp) ((GET_FORWARDALLOC(HDRP(bp))) ? 0 : 1) // (**I Don't Use this **) if forward block is allocated, then we can't access forword block.

/* Given free block's ptr bp, move to location where address of its PREV block and FORWARD block is located. */
#define NEXT_FREEBLKP_POSITION(bp) ((char *)(bp) + WSIZE)
#define PREV_FREEBLKP_POSITION(bp) ((char *)(bp))


/* pointer to heap_low (sizeset's start pointer).
 * I can compute that value without using this variable by calling mem_heap_lo() + WSIZE*2,
 * but use of this variable is more convinient, safe, and have better performance */
static char *heap_lowp;

/* functions */
int mm_init(range_t **ranges);
static void *extend_heap(size_t words);
char *find_sizeset_startpoint(size_t blocksize);
void insert_block_to_sizeset(char *bp);
void delete_block_in_sizeset(char *bp);
static void *coalesce(void *bp);
static void *place(void *bp, size_t payloadsize);
static size_t adjustpayload(size_t payloadsize);
static void *find_fit(size_t payloadsize);
void copyblock(void *beforeblock,void *afterblock,size_t beforeblocksize);

void* mm_malloc(size_t size);
void mm_free(void *bp);
void* mm_realloc(void *ptr, size_t t);
void mm_exit(void);
int mm_check(void);

/*
 * remove_range - manipulate range lists
 * DON'T MODIFY THIS FUNCTION AND LEAVE IT AS IT WAS
 */
static void remove_range(range_t **ranges, char *lo)
{
  range_t *p;
  range_t **prevpp = ranges;

  if (!ranges)
    return;

  for (p = *ranges;  p != NULL; p = p->next) {
    if (p->lo == lo) {
      *prevpp = p->next;
      free(p);
      break;
    }
    prevpp = &(p->next);
  }
}
/*
 * mm_init - initialize the malloc package.
 *           this fucntion set heap's prologue and epilogue block.
 */

int mm_init(range_t **ranges)
{
  /* YOUR IMPLEMENTATION */
  int i;

  if ((heap_lowp = (char *)mem_sbrk((4+SIZESETNUM)*WSIZE)) == (char *)-1) return -1;

  PUT(heap_lowp, 0);                        /* Alignment padding */
  PUT(heap_lowp + (1*WSIZE), ALLOCPACK(WSIZE*(2+SIZESETNUM),1,1,0));  /*Prologue header*/
  heap_lowp+= (2*WSIZE);
  for(i=0;i<SIZESETNUM;i++)			     /*Prologue sizeset pointer*/
  {
  	PUT(heap_lowp + (i*WSIZE), 0);             /*they has NULL */
  }
  PUT(heap_lowp + (i*WSIZE),0x1);		       /* end of sizeset pointer */
  PUT(heap_lowp + ((i+1)*WSIZE),ALLOCPACK(0,1,1,0));   /* Epilogue header */
  /* DON't MODIFY THIS STAGE AND LEAVE IT AS IT WAS */
  gl_ranges = ranges;
  //if(mm_check() == 0) return -1;
  return 0;
}

/* extend_heap : extend heap by word.
 *
 */

static void *extend_heap(size_t words)
{
     char *bp;
     size_t size;
     /*Allocate an even number of words to maintain alignment*/
     size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
     if ((long)(bp=mem_sbrk(size)) == -1) return NULL;


     unsigned int forwardalloc = GET_FORWARDALLOC(HDRP(bp));
     /* Initialize free block header/footer and the epilogue header */
     PUT(HDRP(bp), FREEPACK(size,0,forwardalloc)); /* FREE BLOCK HEADER */
     PUT(FTRP(bp), FREEPACK(size,0,forwardalloc)); /* FREE BLOCK FOOTER */
     insert_block_to_sizeset(bp);    /*link freeblock to sizeset */

     PUT(HDRP((NEXT_BLKP(bp))), ALLOCPACK(0,1,0,0));

     /* Coalesce if the previous block was free */
     return coalesce(bp);
}


/* find_sizeset_startpoint : return pointer at sizeset's first pointer,
 *                           using block size
 */
char *find_sizeset_startpoint(size_t blocksize)
{
	char *set_p = heap_lowp;

	size_t setsize = (blocksize/MINBLOCKSIZE);		// minimum blocksize

	if(setsize == 0) return NULL;       //invalid value
	else if(setsize == 1) return set_p;	 		//minimum set
	else         // get set's first pointer
	{
		while(setsize > 1 && GET(set_p) != 0x1)
		{
			set_p = set_p + HEADERSIZE;
			setsize = setsize >> 1;
		}

		if(GET(set_p) == 0x1)
		{
			return set_p - HEADERSIZE;
		}
		else
		{
			return set_p;
		}
	}

}


/* insert_block_to_sizeset : insert freeblock to it's correct sizeset.
 *                           freeblock must have header and footer.
 */
void insert_block_to_sizeset(char *bp)
{
	char *set_startp = find_sizeset_startpoint(GET_SIZE(HDRP(bp)));
	unsigned int *before_setfirstbp = GETPTR(set_startp);
	if(before_setfirstbp == NULL)   // if set's first freeblock pointer is empty
	{
		PUTPTR(PREV_FREEBLKP_POSITION(bp),NULL);
		PUTPTR(NEXT_FREEBLKP_POSITION(bp),NULL);
		PUTPTR(set_startp,bp);
	}
	else
	{
		PUTPTR(PREV_FREEBLKP_POSITION(bp),NULL);
		PUTPTR(NEXT_FREEBLKP_POSITION(bp),before_setfirstbp);
		PUTPTR(set_startp,bp);
		PUTPTR(PREV_FREEBLKP_POSITION(before_setfirstbp),bp);
	}

}
/* delete_block_in_sizeset : delete freeblock from it's sizeset
 *                           freeblock must have header and footer.
 */
void delete_block_in_sizeset(char *bp)
{
	unsigned int *prev_block_ptr = GETPTR(PREV_FREEBLKP_POSITION(bp));
	unsigned int *next_block_ptr = GETPTR(NEXT_FREEBLKP_POSITION(bp));

    if(prev_block_ptr == NULL)    // if this block is sizeset's first block
    {
        if(next_block_ptr == NULL)
        {
            PUTPTR(find_sizeset_startpoint(GET_SIZE(HDRP(bp))),NULL);
        }
        else
        {
            PUTPTR(find_sizeset_startpoint(GET_SIZE(HDRP(bp))),next_block_ptr);
            PUTPTR(PREV_FREEBLKP_POSITION(next_block_ptr),NULL);
        }
    }
    else
    {
        if(next_block_ptr == NULL)
        {
            PUTPTR(NEXT_FREEBLKP_POSITION(prev_block_ptr),NULL);
        }
        else
        {
            PUTPTR(NEXT_FREEBLKP_POSITION(prev_block_ptr),next_block_ptr);
            PUTPTR(PREV_FREEBLKP_POSITION(next_block_ptr),prev_block_ptr);
        }
    }

}

/* coalesce : coalescing freeblocks.
 *            it should be get complete freeblock.
 *            (freeblock must have it's header, footer and prev/next block pointer)
 */
static void *coalesce(void *bp)
{

    size_t prev_alloc = GET_FORWARDALLOC(HDRP(bp));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    unsigned int size = GET_SIZE(HDRP(bp));

    if(!prev_alloc)
    {
        if(!next_alloc) /* case 1 : both next,prev block has to be coalesced */
        {
            unsigned int forwardalloc = GET_FORWARDALLOC(HDRP(PREV_BLKP(bp)));
            size += GET_SIZE(HDRP(NEXT_BLKP(bp))) + GET_SIZE(HDRP(PREV_BLKP(bp)));
            delete_block_in_sizeset(bp);
            delete_block_in_sizeset(PREV_BLKP(bp));
            delete_block_in_sizeset(NEXT_BLKP(bp));
            PUT(HDRP(PREV_BLKP(bp)),FREEPACK(size,0,forwardalloc));
            PUT(FTRP(NEXT_BLKP(bp)),FREEPACK(size,0,forwardalloc));
            bp = PREV_BLKP(bp);
            insert_block_to_sizeset(bp);

        }
        else        /* case 2 : prev block has to be coalesced */
        {
            unsigned int forwardalloc = GET_FORWARDALLOC(HDRP(PREV_BLKP(bp)));
            size += GET_SIZE(HDRP(PREV_BLKP(bp)));
            delete_block_in_sizeset(bp);
            delete_block_in_sizeset(PREV_BLKP(bp));
            PUT(FTRP(bp), FREEPACK(size,0,forwardalloc));
            PUT(HDRP(PREV_BLKP(bp)),FREEPACK(size,0,forwardalloc));
            bp = PREV_BLKP(bp);
            insert_block_to_sizeset(bp);
        }
    }
    else if(!next_alloc)       /* case 3 : next block has to be coalesced */
    {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        delete_block_in_sizeset(bp);
        delete_block_in_sizeset(NEXT_BLKP(bp));
        PUT(HDRP(bp), FREEPACK(size,0,1));
        PUT(FTRP(bp), FREEPACK(size,0,1));
        insert_block_to_sizeset(bp);
    }
    SET_FORWARDALLOC(HDRP(NEXT_BLKP(bp)),0);
    return bp;
}

/* adjustpayload : modify payloadsize to available block size
 */

static size_t adjustpayload(size_t payloadsize)
{
	if(payloadsize <= MINBLOCKSIZE - HEADERSIZE) return (size_t) MINBLOCKSIZE;
	else
	{
		size_t blocksize = payloadsize + HEADERSIZE;
		return (size_t) ALIGN(blocksize);
	}
}


/* find_fit : find freeblock which can store payload.
 *            if it can't, return NULL
 */

static void *find_fit(size_t blocksize)
{
	char *set_start = find_sizeset_startpoint(blocksize);
    if(set_start==NULL) return NULL;
	while(GET(set_start) != 0x1)  // to end of sizeset
	{
		unsigned int *blockptr = GETPTR(set_start);
		while(blockptr != NULL)
		{
			if(GET_SIZE(HDRP(blockptr)) >= blocksize) return blockptr;
			blockptr = GETPTR(NEXT_FREEBLKP_POSITION(blockptr));
		}
		set_start = set_start + HEADERSIZE;  //go to next sizeset
	}
	return NULL;

}


/* place : allocate block at bp, and splice block properly.
 *         in splice, if allocated blocksize is less than SMALLBLOCKSIZE
 *         possess allocated block head of block,
 *         and free block tail of block.
 *         else possess allocated block tail of block.
 *         this effect is that :
 *         smaller allocate block go to heap's forward side,
 *         and bigger allocate block goes to heap's back side.
 *         this prevent external fragment, smaller allocated block dividing entire heap.
 */
static void *place(void *bp, size_t allocblocksize)
{
	size_t freeblocksize = GET_SIZE(HDRP(bp));
    size_t remainsize = freeblocksize-allocblocksize;
	if(remainsize>=MINBLOCKSIZE)	 /* if remainsize is bigger then minblocksize, split. */
	{
	    if(allocblocksize<=SMALLBLOCKSIZE) // if block size is lesser than SMALLBLOCKSIZE
            {
            	delete_block_in_sizeset(bp);
            	PUT(HDRP(bp),ALLOCPACK(allocblocksize,1,GET_FORWARDALLOC(HDRP(bp)),0));
            	PUT(HDRP(NEXT_BLKP(bp)),FREEPACK(remainsize,0,1));
            	PUT(FTRP(NEXT_BLKP(bp)),FREEPACK(remainsize,0,1));
            	insert_block_to_sizeset(NEXT_BLKP(bp));
            }
            else
            {
           	delete_block_in_sizeset(bp);
            	PUT(HDRP(bp),FREEPACK(remainsize,0,GET_FORWARDALLOC(HDRP(bp))));
            	PUT(FTRP(bp),FREEPACK(remainsize,0,GET_FORWARDALLOC(HDRP(bp))));
            	PUT(HDRP(NEXT_BLKP(bp)),ALLOCPACK(allocblocksize,1,0,0));
            	insert_block_to_sizeset(bp);
            	bp = NEXT_BLKP(bp);
            	SET_FORWARDALLOC((HDRP(NEXT_BLKP(bp))),1);
            }
	}
	else			/* else, make whole block to allocated block */
	{
		delete_block_in_sizeset(bp);
		PUT(HDRP(bp),ALLOCPACK(freeblocksize,1,GET_FORWARDALLOC(HDRP(bp)),0));
		SET_FORWARDALLOC((HDRP(NEXT_BLKP(bp))),1);
	}

	return bp;
}


/*
 * mm_malloc - Allocate a block.
 *     Always allocate a block whose size is a multiple of the alignment.
 *     find free block from sizeset, and place block that position.
 */


void* mm_malloc(size_t size)
{
  size_t blocksize;
  size_t extendsize;
  char *bp;
  /* ignore spurious requests */

  if (size == 0) return NULL;

  blocksize = adjustpayload(size); // get blocksize.

  if ((bp = find_fit(blocksize)) != NULL) {
  	return place(bp,blocksize);
  }

  /* NO fit found. Get more memory and place the block */
  extendsize = MAX(blocksize,CHUNKSIZE);
  if ((extend_heap(extendsize/WSIZE)) == NULL) return NULL;
  bp = find_fit(blocksize);
  return place(bp,blocksize);

  //if(mm_check()==0) return NULL;
}

/*
 * mm_free - Freeing a block.
 *       Free block, and coalesce these free block.
 */
void mm_free(void *ptr)
{
  size_t size = GET_SIZE(HDRP(ptr));

  if(GET_ALLOC(HDRP(ptr)) == 0)	// if user try to free already freed block , print errormessage
  {
  	fprintf(stderr,"ERROR! Double Free Occured! ptr : %p\n",ptr);
  }
  else
  {
  	PUT(HDRP(ptr),FREEPACK(size,0,GET_FORWARDALLOC(HDRP(ptr))));
	PUT(FTRP(ptr),FREEPACK(size,0,GET_FORWARDALLOC(HDRP(ptr))));
	insert_block_to_sizeset(ptr);
	coalesce(ptr);
  }

  //if(mm_check()==0) printf("something wrong occured!");
  /* DON't MODIFY THIS STAGE AND LEAVE IT AS IT WAS */
  if (gl_ranges)
    remove_range(gl_ranges, ptr);
}
/*
 * copyblock : copy blockcontent from beforeblock to afterblock by beforeblocksize
 */

void copyblock(void *beforeblock,void *afterblock,size_t beforeblocksize)
{
    int i;
    unsigned int *beforep = (unsigned int *)beforeblock;
    unsigned int *afterp = (unsigned int *)afterblock;
    for(i=0;i<((beforeblocksize-HEADERSIZE)/WSIZE);i++)
    {
        afterp[i] = beforep[i];
    }
}



/*
 * mm_realloc : realloc block by it's size.
 */
void* mm_realloc(void *ptr, size_t t)
{
  if(ptr==NULL) return mm_malloc(t);        // get NULL pointer, just malloc
  if(t==0)   // get zerosize, just free.
  {
      mm_free(ptr);
      return NULL;
  }
  size_t newblocksize = adjustpayload(t);
  size_t beforeblocksize = GET_SIZE(HDRP(ptr));
  if(newblocksize == beforeblocksize) return ptr;
  else if(newblocksize < beforeblocksize)  // shrink current block
  {
      size_t remainsize = beforeblocksize - newblocksize;
      if(remainsize < MINBLOCKSIZE) // we can't shrink current block, just make new block
      {
          void *new_ptr = mm_malloc(t);
          copyblock(ptr,new_ptr,newblocksize);
          mm_free(ptr);
          return new_ptr;
      }
      else  //shrink current block, and make freeblock
      {
          char *bp = (char *) ptr;
          SET_FORWARDALLOC(HDRP(NEXT_BLKP(bp)),0);
          PUT(HDRP(bp),ALLOCPACK(newblocksize,1,GET_FORWARDALLOC(HDRP(bp)),0));
          PUT(HDRP(NEXT_BLKP(bp)),FREEPACK(remainsize,0,1));
          PUT(FTRP(NEXT_BLKP(bp)),FREEPACK(remainsize,0,1));
          insert_block_to_sizeset(NEXT_BLKP(bp));
          coalesce(NEXT_BLKP(bp));
          return ptr;
      }
  }
  else
  {
     if(GET_ALLOC(HDRP(NEXT_BLKP(ptr)))) // if next block is allocated, alloc new block and copy
     {
         void *new_ptr = mm_malloc(t);
         copyblock(ptr,new_ptr,beforeblocksize);
         mm_free(ptr);
         return new_ptr;
     }
     else                   //next block is free block
     {
        size_t expandsize = newblocksize - beforeblocksize;
        size_t nextblocksize = GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        size_t remainsize = nextblocksize-expandsize;
        if(remainsize<MINBLOCKSIZE) // we can't expand current block
        {
            void *new_ptr = mm_malloc(t);
            copyblock(ptr,new_ptr,beforeblocksize);
            mm_free(ptr);
            return new_ptr;
        }
        else                        //expand allocated block
        {
            char *bp = (char *)ptr;
            delete_block_in_sizeset(NEXT_BLKP(bp));
            PUT(HDRP(bp),ALLOCPACK(newblocksize,1,GET_FORWARDALLOC(HDRP(bp)),0));
            PUT(HDRP(NEXT_BLKP(bp)),FREEPACK(remainsize,0,1));
            PUT(FTRP(NEXT_BLKP(bp)),FREEPACK(remainsize,0,1));
            insert_block_to_sizeset(NEXT_BLKP(bp));
            return ptr;
        }
     }
  }
}

/*
 * mm_exit - finalize the malloc package.
 */
void mm_exit(void)
{
    char *tempp = heap_lowp;
    tempp = NEXT_BLKP(tempp);
	while(GET_SIZE(HDRP(tempp)) != 0)
    {
        if(GET_ALLOC(HDRP(tempp)))
        {
            mm_free(tempp);
        }
        tempp = NEXT_BLKP(tempp);
    }

}

/*  mm_check_sizeset : checking sizesets of heap..
 *                     it checks connection of free blocks, validation of free blocks,
 *                     and checking sizeset correctness.
 *                     to see what is done, see errorMessage. it explain what is done by codes.
 */

int mm_check_sizeset(void)
{
    int i;			//check sizeset
	char *tempp=heap_lowp;  //get sizeset's first sizeset.
	for(i=0;i<SIZESETNUM;i++)
	{
		unsigned int *blockp = GETPTR(tempp);
		unsigned int *prevbp = NULL;
		unsigned int *nextbp;
		while(blockp!=NULL)
		{
			if(prevbp != GETPTR(PREV_FREEBLKP_POSITION(blockp))) //if prev blockpointer sign wrong block. raise error
			{
				fprintf(stderr,"ERROR! prev block pointer is not correct! mm_check_sizeset check\n");
				fprintf(stderr,"prev pointer : %x, real prev address : %x\n",
				        (unsigned int)GETPTR(PREV_FREEBLKP_POSITION(blockp)), (unsigned int)prevbp);
				return 0;
			}
			nextbp = GETPTR(NEXT_FREEBLKP_POSITION(blockp));
			if(GET_ALLOC(HDRP(blockp))==1)
			{
				fprintf(stderr,"ERROR! allocated block is in free list!! mm_check_sizeset check\n");
				fprintf(stderr,"blockp : %x\n, blockhdr : %x\n",(unsigned int)blockp,(unsigned int)GET(HDRP(blockp)));
				return 0;
			}
			if(GET_SIZE(HDRP(blockp))<MINBLOCKSIZE)
			{
				fprintf(stderr,"ERROR! free block size is lower then Minimum block size!! mm_check_sizeset check\n");
				fprintf(stderr,"blockp : %x, blockhdr : %x\n",(unsigned int)blockp,(unsigned int)GET(HDRP(blockp)));
				return 0;
			}
			if(GET_SIZE(HDRP(blockp))<(MINBLOCKSIZE<<i))
			{
				fprintf(stderr,"ERROR! free block size is lower then set size!! mm_check_sizeset check\n");
				fprintf(stderr,"blockp : %p, blockhdr : %x, setsize : %x\n",blockp,(unsigned int)GET(HDRP(blockp)),(MINBLOCKSIZE<<i));
				return 0;
			}
			if((GET_SIZE(HDRP(blockp))>=(MINBLOCKSIZE<<(i+1))) && (i != (SIZESETNUM -1)))
			{
				fprintf(stderr,"ERROR! free block size is bigger then set size!! mm_check_sizeset check\n");
				fprintf(stderr,"blockp : %p, blockhdr : %x, setsize : %x\n",blockp,(unsigned int)GET(HDRP(blockp)),(MINBLOCKSIZE<<i));

				return 0;
			}
			prevbp = blockp;
			blockp = nextbp;
		}
		tempp = tempp + WSIZE;
	}
	if(GET(tempp)!=0x1)
	{
		fprintf(stderr,"ERROR! sizeset endpoint is corrupted!! mm_check_sizeset check\n");
		fprintf(stderr,"sizeset endpoint : %x\n",GET(tempp));
		return 0;
	}
	fprintf(stderr,"mm_check_sizeset correct\n");
	return 1;
}

/*  mm_check_sizeset : checking all blocks of heap..
 *                     it checks prologue blocks, epilogue block,
 *                     and all of allocated block and free block's validation,
 *                     by checking forward allocate field, size field, freeblock pointer... etc.
 *                     to see what is done, see errorMessage. it explain what is done by codes.
 */
int mm_check_block(void)
{
	char *tempp;
	char *heap_low = (char *)mem_heap_lo();
	char *heap_hi = (char *)mem_heap_hi();
	unsigned int heap_lowmem = (unsigned int) heap_low;
	unsigned int heap_highmem = (unsigned int) (heap_hi + 1);
	tempp=(char *)heap_lowmem;
	//fprintf(stderr,"mm_check_block start\n");
			// check all blocks
	if(GET(tempp) != 0)
	{
		fprintf(stderr,"ERROR! heap init block is not correct! mm_check_block check\n");
		fprintf(stderr,"heap initblock : %x\n",GET(tempp));
		return 0;
	}
	tempp+=WSIZE;
	if(GET_SIZE(tempp) != WSIZE * (SIZESETNUM + 2))
	{
		fprintf(stderr,"ERROR! heap prologue block size is not correct! mm_check_block check\n");
		fprintf(stderr,"heap prologue block : %x\n",GET(tempp));
		return 0;
	}
	if(GET_ALLOC(tempp) != 1)
	{
		fprintf(stderr,"ERROR! heap prologue block allocated is not correct! mm_check_block check\n");
		fprintf(stderr,"heap prologue block : %x\n",GET(tempp));
		return 0;
	}
	if(GET_FORWARDALLOC(tempp) != 1)
	{
		fprintf(stderr,"ERROR! heap prologue block forward alloc is not correct! mm_check_bloch check\n");
		fprintf(stderr,"heap prologue block : %x\n",GET(tempp));
		return 0;
	}
	tempp+=WSIZE;
	//fprintf(stderr,"mm_check_block Prolog finished\n");
	unsigned int prevsize = GET_SIZE(HDRP(tempp));
	unsigned int prevalloc = GET_ALLOC(HDRP(tempp));
	unsigned int size;
	unsigned int alloc;
	unsigned int forwardalloc;
	char *prevtempp = NULL;
	while(GET_SIZE(HDRP(tempp))!=0)
	{
		size = GET_SIZE(HDRP(tempp));
		alloc = GET_ALLOC(HDRP(tempp));
		forwardalloc = GET_FORWARDALLOC(HDRP(tempp));
		if(forwardalloc != prevalloc)
		{
			fprintf(stderr,"ERROR! block forwardallocation is not correct! mm_check_block check\n");
			if(prevtempp!=NULL)
			{
			fprintf(stderr,"now block hdr : %x, forward block hdr : %x\n",GET(HDRP(tempp)),GET(HDRP(prevtempp)));
			return 0;
			}
			else
			{
			fprintf(stderr,"ERROR! first block is failed. block hdr : %x\n",GET(HDRP(tempp)));
			return 0;
			}
		}
		if(alloc)
		{
			if(prevtempp!=NULL)
			{
				if(prevtempp+prevsize!=tempp)
				{
					fprintf(stderr,"ERROR! allocated blocks overrap occured! mm_check_block check\n");
					fprintf(stderr,"now block hdr : %x,forward block hdr : %x\n",GET(HDRP(tempp)),GET(HDRP(prevtempp)));
					return 0;
				}
			}

		}
		else
		{
			if(prevtempp!=NULL)
			{
				if(prevtempp+prevsize!=tempp)
				{
					fprintf(stderr,"ERROR! free block overrap occured! mm_check_block check\n");
					fprintf(stderr,"now block hdr : %x,forward block hdr : %x\n",GET(HDRP(tempp)),GET(HDRP(prevtempp)));
					return 0;
				}
			}
			if(prevalloc==0)
			{
				fprintf(stderr,"ERROR! somehow escape coalescing freeblock occured! mm_check_block check\n");
				fprintf(stderr,"now block hdr : %x,forward block hdr : %x\n",GET(HDRP(tempp)),GET(HDRP(prevtempp)));
				return 0;
			}
			unsigned int *prevblckptr = GETPTR(PREV_FREEBLKP_POSITION(tempp));
			unsigned int *nextblckptr = GETPTR(NEXT_FREEBLKP_POSITION(tempp));

			if(prevblckptr != NULL)
			{
				if((unsigned int)prevblckptr < heap_lowmem || (unsigned int)prevblckptr >= heap_highmem)
				{
					fprintf(stderr,"ERROR! prev free block ptr doesn't point to valid heap address! mm_check_block check\n");
					fprintf(stderr,"now block pointer : %x, prev free block ptr : %x",(unsigned int)tempp,(unsigned int)prevblckptr);
					return 0;
				}

				if(GET_ALLOC(HDRP(prevblckptr)) != 0)
				{
					fprintf(stderr,"ERROR! prev free block is not free block! mm_check_block_check\n");
					fprintf(stderr,"prev free block hdr : %x,prev free block ptr : %x",GET(HDRP(prevblckptr)),(unsigned int)prevblckptr);
					return 0;
				}
				if(GETPTR(NEXT_FREEBLKP_POSITION(prevblckptr)) != (unsigned int *)tempp)
				{
					fprintf(stderr,"ERROR! prev free block is not chase now block! mm_check_block_check\n");
					fprintf(stderr,"prev free block's nextptr : %x,now block ptr : %x",(unsigned int)GETPTR(NEXT_FREEBLKP_POSITION(prevblckptr)),(unsigned int)tempp);
					return 0;
				}
			}

			if(nextblckptr != NULL)
			{
				if((unsigned int)nextblckptr < heap_lowmem || (unsigned int)nextblckptr >= heap_highmem)
				{
					fprintf(stderr,"ERROR! next free block ptr doesn't point to valid heap address! mm_check_block check\n");
					fprintf(stderr,"now block pointer : %x, next free block ptr : %x",(unsigned int)tempp,(unsigned int)nextblckptr);
					return 0;
				}

				if(GET_ALLOC(HDRP(nextblckptr)) != 0)
				{
					fprintf(stderr,"ERROR! next free block is not free block! mm_check_block_check\n");
					fprintf(stderr,"next free block hdr : %x,next free block ptr : %x",GET(HDRP(nextblckptr)),(unsigned int)nextblckptr);
					return 0;
				}
				if(GETPTR(PREV_FREEBLKP_POSITION(nextblckptr)) != (unsigned int *)tempp)
				{
					fprintf(stderr,"ERROR! next free block is not chase now block! mm_check_block_check\n");
					fprintf(stderr,"next free block's prevptr : %x,now block ptr : %x",(unsigned int)GETPTR(PREV_FREEBLKP_POSITION(nextblckptr)),(unsigned int)tempp);
					return 0;
				}
			}

		}
		prevtempp=tempp;
		prevsize = size;
		prevalloc = alloc;
		tempp = NEXT_BLKP(tempp);
		if(tempp == ((char *)heap_hi + 1)) break;
		if((unsigned int)tempp < heap_lowmem || (unsigned int)tempp >= heap_highmem)
		{
			fprintf(stderr,"ERROR! pointers in heap block doesn't point to valid heap address!\n");
			fprintf(stderr,"now block pointer : %x, prev block hdr : %x\n",(unsigned int)tempp,GET(HDRP(prevtempp)));
			return 0;
		}
	}
	if(GET_ALLOC(HDRP(tempp))!=1)
	{
		fprintf(stderr,"ERROR! Epilogue block is not correct!");
		fprintf(stderr,"Epilogue block : %x",GET(HDRP(tempp)));
		return 0;
	}
	if(prevalloc != GET_FORWARDALLOC(HDRP(tempp)))
	{
		fprintf(stderr,"ERROR! Epilogue block's forward alloc is not correct!");
		fprintf(stderr,"Epilogue block : %x, forwardblockhdr : %x",GET(HDRP(tempp)),GET(HDRP(prevtempp)));
		return 0;
	}
	//fprintf(stderr,"mm_check_block correct");
	return 1;
}

/*  mm_check : checking sizesets, and all blocks of heap
 *             by calling mm_check_sizeset, mm_check_block
 */

int mm_check(void)
{

	return mm_check_sizeset() && mm_check_block();

}
