/*
 * cache.h - Cache functionalities for proxy server of SNU proxy lab
 * Author: Jiwong Ko
 * Email: jiwong@csap.snu.ac.kr
 */

#include <stdio.h>

#define MAX_CONTENT_SIZE 200000 // MAX content size for proxy server: 200kb
#define MAX_OBJECT_SIZE 1000000 // MAX cache size for proxy server: 2MB
#define MAX_URL_SIZE 1024
#define MAX_RESP_SIZE 1024
#define MAX_CACHE_NUM 200     //MAX cache block number for cache. it prevents huge searching times when linked list length is longer.

typedef struct cache_block{
		/*
		 * Linked list based cache.
		 *
		 * Cache block needs to contain
		 * 1. URL (recommend to use key value)
		 * 2. Response
		 * 3. contents
		 * 4. size of contents
		 * 5. next member of cache block.
		 */
        char *url;
        char *response;
        char *contents;
        int contentsize;
        struct cache_block *next;
}cache_block;

/* Cache function prototypes*/
cache_block* find_cache_block(char* uri);
void cache_replacement_policy();
void add_cache_block(char* uri, char* response, char* content, int contentLength);
void cache_init();
void delete_cache_block();
