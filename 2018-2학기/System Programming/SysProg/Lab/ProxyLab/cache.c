#include "cache.h"
#include "csapp.h"

// Data structures like linkedlist, tree, hash table, you can use anything.

/*
 * cache.c - A simple, iterative HTTP proxy server
 *
 * use linked list (has one dummy member) to deal with cache.
 * it has FIFO replacement policy.
 *
 */




static int currentCacheSize;
static int currentCacheNum;
static cache_block *head; //linked list header
static cache_block *tail; //linked list tail

void cache_init()
{
    currentCacheSize = 0;
    currentCacheNum = 0;
    cache_block *dummy = Malloc(sizeof(cache_block));
    head=dummy;
    tail=dummy;
    dummy->next = dummy;
}


cache_block* find_cache_block(char* uri){ // find cache block by uri.
    cache_block *nblock = head->next;
    while(nblock!=tail)
    {
        if(strcmp(uri,nblock->url)==0)
        {
            return nblock;  //if cache find block, return this block.
        }
        nblock = nblock->next;
    }
    return NULL;  //if cache couldn't find block, just return null.
	/*  Find the cache block according to the URI*/
}
void cache_replacement_policy(int newContentsize){
	/*
	 * Make cache replacement policy in FIFO order
	 * If you want, you can implement other cache replacement policies such as LRU.
	 */
	 while(currentCacheSize+newContentsize>MAX_OBJECT_SIZE) // if current cachesize + new contentsize is bigger than cachesize, refresh cache.
     {
          delete_cache_block(); //delete first member of linked list.
     }
     while(currentCacheNum+1>MAX_CACHE_NUM) //if current cachenumber + 1 is bigger than max cached number, refresh.
     {
         delete_cache_block(); //delete firste member of linked list.
     }
     //I used linked list, many member's in linked list = performance down
     //so I restrict number of cache block.
}


// add_cache_block get uri,response as malloc'd member,
// and get content as mmap'd member. so it should handling them.
// use free and munmap function.
void add_cache_block(char* uri,char* response,char* content,int contentLength){

    cache_block *cblock;
	/* 1.Check the size of the content is over MAX_CONTENT_SIZE */
    if(contentLength>MAX_CONTENT_SIZE)
    {
        Munmap(content,contentLength);
        Free(uri);
        Free(response);
        return;
    }
	/* 2.Find first whether the following uri is already cached */
    cblock = find_cache_block(uri);
    if(cblock!=NULL) //if cache block sholud be replaced, replace that block.
    {
        char *old_url = cblock->url;
        char *old_response = cblock->response;
        char *old_contents = cblock->contents;
        int old_contentsize = cblock->contentsize;
        cblock->url = uri;
        cblock->response = old_response;
        cblock->contents = content;
        cblock->contentsize = contentLength;
        Free(old_url);
        Free(old_response);
        Munmap(old_contents,old_contentsize);
        currentCacheSize = currentCacheSize - old_contentsize + contentLength;
        return;
    }
	/* 3.Check the proxy cache is full, then you have to execute cache replacement policy.*/
    cache_replacement_policy(contentLength);

	/* 4.Add new cache block into proxy cache */
    cache_block *nblock = Malloc(sizeof(cache_block));
    nblock->url = uri;
    nblock->response = response;
    nblock->contents = content;
    nblock->contentsize = contentLength;
    currentCacheNum += 1;
    currentCacheSize += contentLength;
	if(head==tail)
    {
        head->next = nblock;
        tail = nblock;
        nblock->next = nblock;
    }
    else
    {
        cache_block *nextcache = head->next;
        head->next = nblock;
        nblock->next = nextcache;
    }
    return;
}

//delete first member of linked list.
void delete_cache_block()
{
    if(head==tail)
    {
        printf("cache delete error occured!\n");
        exit(0);
    }
    else
    {
        cache_block *dblock = head->next;
        if(tail==dblock)
        {
            tail=head;
            head->next = head;
        }
        else
        {
            head->next = dblock->next;
        }
        char *old_url = dblock->url;
        char *old_response = dblock->response;
        char *old_contents = dblock->contents;
        int old_contentsize = dblock->contentsize;
        Free(old_url);
        Free(old_response);
        Munmap(old_contents,old_contentsize);
        currentCacheSize = currentCacheSize - old_contentsize;
        currentCacheNum -= 1;
        Free(dblock);
        return;
    }

}
