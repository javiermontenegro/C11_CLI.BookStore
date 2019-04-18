#ifndef BOOK_H
#define BOOK_H

/*! \file book.h
 *  \brief Definitions for book store manipulation.
 *
 *  The book datatype is implemented as a bag using a linked list. It
 *  supports CRUD opertions of entries as well as IO operations for storing
 *  and retrieving from a file.
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "node_string.h"
#include "node_entry.h"

/*! \typedef entry_node_t
 *  \brief Type definition for a nodes of a linked list.
 *
 *  This is an implementation of a node that contains a previous and next
 *  link in order to implement a doubly linked list.
 */
typedef struct entry_node
{
    struct entry_node *n_prev;
    entry_t *n_entry;
    struct entry_node *n_next;
} entry_node_t;

/*! \typedef book_t
 *  \brief Type definition of an book store.
 *
 *  This is a doubly linked list implementation that contains a head and
 *  tail.
 */
typedef struct
{
    entry_node_t *a_head;
    entry_node_t *a_tail;
} book_t;

/*! \fn book_t *book_create( void )
 *  \brief Creates an book store.
 *  \return On success an book store is returned. Otherwise NULL is
 *  returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the book store.
 */
extern book_t *book_create( void );

/*! \fn book_T *book_duplicate( const book_t *book )
 *  \brief Duplicates an book store.
 *  \param book The book store to be duplicated.
 *  \return On success a duplicate book store and its entries are
 *  returned. Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the book store or its
 *  entries.
 */
extern book_t *book_duplicate( const book_t *book );

/*! \fn book_t *book_get( const book_t *book, unsigned index  )
 *  \brief Gets an entry node from the book store given its index.
 *  \param book The book store from which the entry will be retrieve.
 *  \param index A zero-based index of the entry in the book store.
 *  \return On success the correspoding entry node is returned. Otherwise
 *  NULL is returned and errno is set appropriately.
 *  \exception EINVAL An invalid number was provided.
 */
extern entry_node_t *book_get( const book_t *book, unsigned index );

/*! \fn void book_write( FILE *file, book_t *book )
 *  \brief Writes an book store in binary format to a specified stream.
 *  \param file The stream where to write the book store.
 *  \param book The book store to be written.
 */
extern void book_write( FILE *file, book_t *book );

/*! \fn void book_t *book_read( FILE *file )
 *  \brief Reads an book store in binary format from a specified stream.
 *  \param file The stream from where to read the entry.
 *  \return On success a new book store with read values is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memeory to allocate the entry.
 */
extern book_t *book_read( FILE *file );

/*! \fn int book_add( book_t *book, entry_t *entry )
 *  \brief Duplicates and adds an entry to the end of an book store.
 *  \param book The book store for which an entry is to be added.
 *  \param entry The entry to be duplicated and added.
 *  \return On success the entry is added and zero is returned. Otherwise -1
 *  is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate an entry node or
 *  duplicate an entry.
 */
extern int book_add( book_t *book, entry_t *entry );

/*! \fn int book_add_all( book_t *book, book_t *some_book )
 *  \brief Duplicates and adds all entries from another book store.
 *  \param book The book store for which entries are too be added.
 *  \param some_book The book store containing all entries to be added.
 *  \return On success all entries are added and zero is returned. Otherwise
 *  -1 is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate an entry node or
 *  duplicate an entry.
 */
extern int book_add_all( book_t *book, book_t *some_book );

/*! \fn int book_add_many book_add_many( book_t *book, int count, ... )
 *  \brief Duplicates and adds many entries into an book store at once.
 *  \param book The book store for which the entries are to be added.
 *  \param count The number of arguments to follow.
 *  \return On success all entries are added and zero is returned. Otherwise
 *  -1 is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate an entry node or
 *  duplicate an entry.
 */
extern int book_add_many( book_t *book, int count, ... );

/*! \fn book_t *book_find_by_title( const book_t *book, const char *title )
 *  \brief Finds entries in an book store by first name.
 *  \param book The book store from which entries are to be searched.
 *  \param title A null-terminated string containing the value for title.
 *  \return On success an book store containing found entries is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate a new book store or
 *  entry nodes.
 */
extern book_t *book_find_by_title( const book_t *book, const char *title );

/*! \fn book_t *book_find_by_author( const book_t *book, const char *author )
 *  \brief Finds entries in an book store by last name.
 *  \param book The book store from which entries are to be searched.
 *  \param author A null-terminated string containing the value for author.
 *  \return On success an book store containing found entries is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate a new book store or
 *  entry nodes.
 */
extern book_t *book_find_by_author( const book_t *book, const char *author );

/*! \fn book_t *book_find_by_publisher( const book_t *book, const char *publisher )
 *  \brief Finds entries in an book store by pubdate name.
 *  \param book The book store from which entries are to be searched.
 *  \param pubdate A null-terminated string containing the value for publisher.
 *  \return On success an book store containing found entries is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate a new book store or
 *  entry nodes.
 */
extern book_t *book_find_by_publisher( const book_t *book, const char *publisher );

/*! \fn entry_t *book_remove( book_t *book, entry_node_t *entry_node )
 *  \brief Removes an entry from an book store.
 *  \param book The book store for which an entry is to be removed.
 *  \param entry_node The entry node to be removed from the book store.
 *  \return On success the entry is removed from the list and returned.
 *  Otherwise NULL is returned.
 */
extern entry_t *book_remove( book_t *book, entry_node_t *entry_node );

/*! \fn book_t *book_remove_all( book_t *book, book_t *some_book )
 *  \brief Removes all entries from an book store.
 *  \param book The book store from which entries are to be removed.
 *  \param some_book The book store containing all entries to be removed.
 *  \return On success an book store containing all removed entries is
 *  returned. Otherwise NULL is returned.
 */
extern book_t *book_remove_all( book_t *book, book_t *some_book );

/*! \fn book_t *book_destroy( book_t *book, int all )
 *  \brief Destroys an book store and its entries.
 *  \param book The book store to be destroyed.
 *  \param all An integer signifying whether or not entries are to be destroyed.
 */
extern void book_destroy( book_t *book, int all );

#endif /* BOOK_H */
