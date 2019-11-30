#include <node_entry.h>


entry_t *entry_create( void )
{
    entry_t *entry;

    if( ( entry = malloc( sizeof( entry_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    memset( entry, 0, sizeof( entry_t ) );

    return entry;
}

entry_t *entry_duplicate( const entry_t *entry )
{
    entry_t *duplicate;

    if( ( duplicate = entry_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    duplicate->e_title  = string_duplicate ( entry->e_title );
    duplicate->e_author = string_duplicate ( entry->e_author );
    duplicate->e_pages  = string_duplicate ( entry->e_pages );
    duplicate->e_edition = string_duplicate ( entry->e_edition );
    duplicate->e_language = string_duplicate ( entry->e_language );
    duplicate->e_publisher = string_duplicate ( entry->e_publisher );
    duplicate->e_pubdate = string_duplicate ( entry->e_pubdate );
    duplicate->e_isbn = string_duplicate ( entry->e_isbn );
    duplicate->e_description    = string_duplicate  ( entry->e_description );

    return duplicate;
}

void entry_print( FILE *file, entry_t *entry )
{
    const char *fmt = "\
Book title:       %s\n\
Author:           %s\n\
Pages:            %s\n\
Edition:          %s\n\
Language:         %s\n\
Publisher:        %s\n\
Publication date: %s\n\
ISBN:             %s\n\
Description:\n\n\
%s\n";
    fprintf( file, fmt,
        entry_get_title( entry )->s_ptr,
        entry_get_author( entry )->s_ptr,
        entry_get_pages( entry )->s_ptr,
        entry_get_edition( entry )->s_ptr,
        entry_get_language( entry )->s_ptr,
        entry_get_publisher( entry )->s_ptr,
        entry_get_pubdate( entry )->s_ptr,
        entry_get_isbn( entry )->s_ptr,
        entry_get_description( entry )->s_ptr );
}

entry_t *entry_scan( FILE *file )
{
    entry_t *entry;

    entry = entry_create( );
    entry_set_title( entry, string_scan( file ) );
    entry_set_author( entry, string_scan( file ) );
    entry_set_pages( entry, string_scan( file ) );
    entry_set_edition( entry, string_scan( file ) );
    entry_set_language( entry, string_scan( file ) );
    entry_set_publisher( entry, string_scan( file ) );
    entry_set_pubdate( entry, string_scan( file ) );
    entry_set_isbn( entry, string_scan( file ) );
    entry_set_description( entry, string_scan( file ) );

    return entry;
}

void entry_write( FILE *file, entry_t *entry )
{
    string_write( file, entry->e_title );
    string_write( file, entry->e_author );
    string_write( file, entry->e_pages );
    string_write( file, entry->e_edition );
    string_write( file, entry->e_language );
    string_write( file, entry->e_publisher );
    string_write( file, entry->e_pubdate );
    string_write( file, entry->e_isbn );
    string_write( file, entry->e_description );
}

entry_t *entry_read( FILE *file )
{
    entry_t *entry;

    entry = entry_create( );
    entry_set_title( entry, string_read( file ) );
    entry_set_author( entry, string_read( file ) );
    entry_set_pages( entry, string_read( file ) );
    entry_set_edition( entry, string_read( file ) );
    entry_set_language( entry, string_read( file ) );
    entry_set_publisher( entry, string_read( file ) );
    entry_set_pubdate( entry, string_read( file ) );
    entry_set_isbn( entry, string_read( file ) );
    entry_set_description( entry, string_read( file ) );

    return entry;
}

void entry_set_title( entry_t *entry, string_t *title )
{
    if( entry->e_title != NULL )
        string_destroy( entry->e_title );

    entry->e_title = title;
}

void entry_set_author( entry_t *entry, string_t *author )
{
    if( entry->e_author != NULL )
        string_destroy( entry->e_author );

    entry->e_author = author;
}

void entry_set_pages( entry_t *entry, string_t *pages )
{
    if( entry->e_pages != NULL )
        string_destroy( entry->e_pages );

    entry->e_pages = pages;
}

void entry_set_edition( entry_t *entry, string_t *edition )
{
    if( entry->e_edition != NULL )
        string_destroy( entry->e_edition );

    entry->e_edition = edition;
}

void entry_set_language( entry_t *entry, string_t *language )
{
    if( entry->e_language != NULL )
        string_destroy( entry->e_language );

    entry->e_language = language;
}

void entry_set_publisher( entry_t *entry, string_t *publisher )
{
    if( entry->e_publisher != NULL )
        string_destroy( entry->e_publisher );

    entry->e_publisher = publisher;
}

void entry_set_pubdate( entry_t *entry, string_t *pubdate )
{
    if( entry->e_pubdate != NULL )
        string_destroy( entry->e_pubdate );

    entry->e_pubdate = pubdate;
}

void entry_set_isbn( entry_t *entry, string_t *isbn )
{
    if( entry->e_isbn != NULL )
        string_destroy( entry->e_isbn );

    entry->e_isbn = isbn;
}

void entry_set_description( entry_t *entry, string_t *description )
{
    if( entry->e_description != NULL )
        string_destroy( entry->e_description );

    entry->e_description = description;
}

string_t *entry_get_title( entry_t *entry )
{
    return entry->e_title;
}

string_t *entry_get_author( entry_t *entry )
{
    return entry->e_author;
}

string_t *entry_get_pages( entry_t *entry )
{
    return entry->e_pages;
}

string_t *entry_get_edition( entry_t *entry )
{
    return entry->e_edition;
}

string_t *entry_get_language( entry_t *entry )
{
    return entry->e_language;
}

string_t *entry_get_publisher( entry_t *entry )
{
    return entry->e_publisher;
}

string_t *entry_get_pubdate( entry_t *entry )
{
    return entry->e_pubdate;
}

string_t *entry_get_isbn( entry_t *entry )
{
    return entry->e_isbn;
}

string_t *entry_get_description( entry_t *entry )
{
    return entry->e_description;
}

void entry_destroy( entry_t *entry )
{
    string_destroy  ( entry->e_title );
    string_destroy  ( entry->e_author );
    string_destroy  ( entry->e_pages );
    string_destroy  ( entry->e_edition );
    string_destroy  ( entry->e_language );
    string_destroy  ( entry->e_publisher );
    string_destroy  ( entry->e_pubdate );
    string_destroy  ( entry->e_isbn );
    string_destroy  ( entry->e_description );
    free            ( entry );
}
