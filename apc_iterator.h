/*
  +----------------------------------------------------------------------+
  | APC                                                                  |
  +----------------------------------------------------------------------+
  | Copyright (c) 2006-2011 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Brian Shire <shire@.php.net>                                |
  +----------------------------------------------------------------------+

 */

/* $Id: apc_iterator.h 307048 2011-01-03 23:53:17Z kalle $ */

#ifndef APC_ITERATOR_H
#define APC_ITERATOR_H

#include "apc.h"
#include "apc_stack.h"

#if HAVE_PCRE || HAVE_BUNDLED_PCRE
#   include "ext/pcre/php_pcre.h"
#   include "zend_smart_str.h"
#   define ITERATOR_PCRE 1
#endif


#define APC_ITERATOR_NAME "APCuIterator"

#define APC_DEFAULT_CHUNK_SIZE 100

#define APC_LIST_ACTIVE   0x1
#define APC_LIST_DELETED  0x2

#define APC_ITER_TYPE		(1 << 0)
#define APC_ITER_KEY        (1 << 1)
#define APC_ITER_VALUE      (1 << 2)
#define APC_ITER_NUM_HITS   (1 << 3) 
#define APC_ITER_MTIME      (1 << 4) 
#define APC_ITER_CTIME      (1 << 5) 
#define APC_ITER_DTIME      (1 << 6) 
#define APC_ITER_ATIME      (1 << 7) 
#define APC_ITER_REFCOUNT   (1 << 8) 
#define APC_ITER_MEM_SIZE   (1 << 9) 
#define APC_ITER_TTL        (1 << 10)

#define APC_ITER_NONE       0
#define APC_ITER_ALL        (0xffffffffL)

typedef void* (*apc_iterator_item_cb_t)(apc_cache_slot_t **slot);

/* {{{ apc_iterator_t */
typedef struct _apc_iterator_t {
    short int initialized;   /* sanity check in case __construct failed */
    zend_long format;             /* format bitmask of the return values ie: key, value, info */
    int (*fetch)(struct _apc_iterator_t *iterator);
                             /* fetch callback to fetch items from cache slots or lists */
    zend_long slot_idx;           /* index to the slot array or linked list */
    zend_long chunk_size;         /* number of entries to pull down per fetch */
    apc_stack_t *stack;      /* stack of entries pulled from cache */
    int stack_idx;           /* index into the current stack */
#ifdef ITERATOR_PCRE
    pcre *re;                /* regex filter on entry identifiers */
#endif
    zend_string *regex;
    HashTable *search_hash;  /* hash of keys to iterate over */
    zend_long key_idx;            /* incrementing index for numerical keys */
    short int totals_flag;   /* flag if totals have been calculated */
    zend_long hits;               /* hit total */
    size_t size;             /* size total */
    zend_long count;              /* count total */
    zend_object obj;
} apc_iterator_t;
/* }}} */

#define apc_iterator_fetch_from(o) ((apc_iterator_t*)((char*)o - XtOffsetOf(apc_iterator_t, obj)))
#define apc_iterator_fetch(z) apc_iterator_fetch_from(Z_OBJ_P(z))

/* {{{ apc_iterator_item */
typedef struct _apc_iterator_item_t {
    zend_string *key;
    zval value;
} apc_iterator_item_t;
/* }}} */

extern void apc_iterator_obj_init(
	apc_iterator_t *iterator, 
	zval *search, 
	zend_long format, 
	zend_long chunk_size, 
	zend_long list);
extern zend_class_entry* apc_iterator_ce;

int apc_iterator_init(int module_number);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim>600: expandtab sw=4 ts=4 sts=4 fdm=marker
 * vim<600: expandtab sw=4 ts=4 sts=4
 */
