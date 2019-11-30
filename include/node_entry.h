#ifndef NODE_ENTRY_H
#define NODE_ENTRY_H

/*! \file node_entry.h
 *  \brief Definitions for memory allocated entries.
 *
 *  The entry datatype supports the storage of attributes pertaining to
 *  entries from an book store, such as title, author,
 *  pages and publisher.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "node_string.h"

/*! \typedef entry_t
 *  \brief Type definition for book store entries.
 */
typedef struct
{
    string_t    *e_title; 
    string_t    *e_author; 
    string_t    *e_pages; 
    string_t    *e_edition; 
    string_t    *e_language; 
    string_t    *e_publisher;
    string_t    *e_pubdate;
    string_t    *e_isbn; 
    string_t    *e_description; 
} entry_t;

/*! \fn entry_t *entry_create( void )
 *  \brief Creates an entry.
 *  \return On success a new entry is returned with its members empty.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the entry.
 */
extern entry_t *entry_create( void );

/*! \fn entry_t *entry_duplicate( const entry_t *entry )
 *  \brief Duplicates an entry.
 *  \param entry The entry to be duplicated.
 *  \return On success a duplicate entry is returned. Otherwise NULL is
 *  returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the entry.
 */
extern entry_t *entry_duplicate ( const entry_t *entry );

/*! \fn void entry_print( FILE *file, entry_t *entry )
 *  \brief Prints an entry to a specified stream.
 *  \param file The stream where to print the entry.
 *  \param entry The entry to be printed.
 */
extern void entry_print( FILE *file, entry_t *entry );

/*! \fn entry_t *entry_scan( FILE *file )
 *  \brief Scans an entry from a specified stream.
 *  \param file The stream from where to scan the entry.
 *  \return On success a new entry with the scanned values is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the entry.
 */
extern entry_t *entry_scan( FILE *file );

/*! \fn void entry_write( FILE *file, entry_t *entry )
 *  \brief Writes an entry in binary format to a specifed stream.
 *  \param file The stream where to write the entry.
 *  \param entry The entry to be written.
 */
extern void entry_write( FILE *file, entry_t *entry );

/*! \fn entry_t *entry_read( FILE *file )
 *  \brief Reads an entry in binary format from a specified stream.
 *  \param file The stream from where to read the entry.
 *  \return On success a new entry with read values is returned. Otherwise
 *  NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the entry.
 */
extern entry_t *entry_read( FILE *file );

/*! \fn void entry_set_title( entry_t *entry, string_t *title )
 *  \brief Sets member title of entry structure.
 *  \param entry The entry to be modified.
 *  \param title A string containing a value for title.
 */
extern void entry_set_title( entry_t *entry, string_t *title );

/*! \fn void entry_set_author( entry_t *entry, string_t *author )
 *  \brief Sets member author of entry structure.
 *  \param entry The entry to be modified.
 *  \param author A string containing a value for author.
 */
extern void entry_set_author( entry_t *entry, string_t *author );

/*! \fn void entry_set_pages( entry_t *entry, string_t *pages )
 *  \brief Sets member pages of entry structure.
 *  \param entry The entry to be modified.
 *  \param pages A string containing a value for pages.
 */
extern void entry_set_pages( entry_t *entry, string_t *pages );

/*! \fn void entry_set_edition( entry_t *entry, string_t *edition )
 *  \brief Sets member edition of entry structure.
 *  \param entry The entry to be modified.
 *  \param edition A string containing a value for edition.
 */
extern void entry_set_edition( entry_t *entry, string_t *edition );

/*! \fn void entry_set_language( entry_t *entry, string_t *language )
 *  \brief Sets member language of entry structure.
 *  \param entry The entry to be modified.
 *  \param language A string containing a value for language.
 */
extern void entry_set_language( entry_t *entry, string_t *language );

/*! \fn void entry_set_publisher( entry_t *entry, address_t *publisher )
 *  \brief Sets member publisher of entry structure.
 *  \param entry The entry to be modified.
 *  \param publisher An address containing a value for publisher.
 */
extern void entry_set_publisher( entry_t *entry, string_t *publisher );

/*! \fn void entry_set_pubdate( entry_t *entry, string_t *pubdate )
 *  \brief Sets member pubdate of entry structure.
 *  \param entry The entry to be modified.
 *  \param pubdate A string containing a value for publication date.
 */
extern void entry_set_pubdate( entry_t *entry, string_t *pubdate );

/*! \fn void entry_set_isbn( entry_t *entry, string_t *isbn )
 *  \brief Sets member isbn of entry structure.
 *  \param entry The entry to be modified.
 *  \param isbn A string containing a value for isbn.
 */
extern void entry_set_isbn( entry_t *entry, string_t *isbn );

/*! \fn void entry_set_description( entry_t *entry, string_t *description )
 *  \brief Sets member description of entry structure.
 *  \param entry The entry to be modified.
 *  \param description A string containing a value for entry description.
 */
extern void entry_set_description( entry_t *entry, string_t *description );

/*! \fn string_t *entry_get_title( entry_t *entry )
 *  \brief Gets member title of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for title.
 */
extern string_t *entry_get_title( entry_t *entry );

/*! \fn string_t *entry_get_author( entry_t *entry )
 *  \brief Gets member author of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for author.
 */
extern string_t *entry_get_author( entry_t *entry );

/*! \fn string_t *entry_get_pages( entry_t *entry )
 *  \brief Gets member pages of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for pages.
 */
extern string_t *entry_get_pages( entry_t *entry );

/*! \fn string_t *entry_get_edition( entry_t *entry )
 *  \brief Gets member edition of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for edition.
 */
extern string_t *entry_get_edition( entry_t *entry );

/*! \fn string_t *entry_get_language( entry_t *entry )
 *  \brief Gets member language of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for language.
 */
extern string_t *entry_get_language( entry_t *entry );

/*! \fn address_t *entry_get_publisher( entry_t *entry )
 *  \brief Gets member publisher of entry structure.
 *  \param entry The entry to be accessed.
 *  \return An address containing the value for publisher.
 */
extern string_t *entry_get_publisher( entry_t *entry );

/*! \fn string_t *entry_get_pubdate( entry_t *entry )
 *  \brief Gets member pubdate of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for publication date.
 */
extern string_t *entry_get_pubdate( entry_t *entry );

/*! \fn string_t *entry_get_isbn( entry_t *entry )
 *  \brief Gets member isbn of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for isbn.
 */
extern string_t *entry_get_isbn( entry_t *entry );

/*! \fn string_t *entry_get_description( entry_t *entry )
 *  \brief Gets member description of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for description.
 */
extern string_t *entry_get_description( entry_t *entry );

/*! \fn void entry_destroy( entry_t *entry )
 *  \brief Destroys an entry.
 *  \param entry The entry to be destroyed.
 */
extern void entry_destroy( entry_t *entry );

#endif /* NODE_ENTRY_H */
