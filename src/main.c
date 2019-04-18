#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <termios.h>
#include <signal.h>
#include <errno.h>
#include <book.h>

#define MAXLENGTH   512

char FILENAME[ MAXLENGTH ]; 
typedef enum
{
    ADD=1,
    DISPLAY,
    FIND_BY_TITLE,
    FIND_BY_AUTHOR,
    FIND_BY_PUBLISHER,
    EDIT,
    DELETE
} option_t;

struct termios saved_term;
static int login( void );
static entry_t *entry_prompt( void );
static void restore_terminal( void );
static void sigint_handler( int sig );

int main( int argc, char *argv[ ] )
{
    int logged;

    logged = login( );

    if( logged == 1 )
    {
        book_t *book;
        FILE *file;
        int option;


        printf( "User logged in successful\n" );

        if( ( file = fopen( FILENAME, "r" ) ) == NULL )
        {
            if( errno == ENOENT )
            {
                book = book_create( );
            }
            else
            {
                perror( "fopen" );
                return EXIT_FAILURE;
            }
        }
        else
        {
            book = book_read( file );
            fclose( file );
        }

        do {
            printf( "\
[1] Add new entry\n\
[2] Display a list of all entries\n\
[3] Find entries by title\n\
[4] Find entries by author\n\
[5] Find entries by publisher\n\
[6] Edit an entry\n\
[7] Delete an entry\n\
[0] Exit\n\
--> " );
            scanf( "%d", &option );
            while( getchar( ) != '\n' );

            switch( option )
            {
                case ADD:
                    {
                        entry_t *entry;

                        entry = entry_prompt( );

                        if( book_add( book, entry ) == -1 )
                        {
                            perror( "book_add" );
                        }
                        else
                        {
                            printf( "Book added to book store!\n" );
                            entry_destroy( entry );
                        }
                    } break;
                case DISPLAY:
                    {
                        entry_node_t *it;

                        it = book->a_head;

                        while( it != NULL )
                        {
                            printf( "%s, %s (%s)\n",
                                    it->n_entry->e_author->s_ptr,
                                    it->n_entry->e_title->s_ptr,
                                    it->n_entry->e_publisher->s_ptr );

                            it = it->n_next;
                        }
                    } break;
                case FIND_BY_TITLE:
                    {
                        char title[ MAXLENGTH ];
                        book_t *result;
                        entry_node_t *it;
                        int next_option;
                        unsigned count;

                        printf( "Enter Book title: " );
                        scanf( "%[^\n]", title );
                        while( getchar( ) != '\n' );

                        result = book_find_by_title( book, title );
                        it = result->a_head;
                        count = 0;

                        printf( "List of entries found\n" );

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_author->s_ptr,
                                    it->n_entry->e_title->s_ptr,
                                    it->n_entry->e_publisher->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "No results found for \"%s\"", title );
                            break;
                        }

                        do {
                            printf( "\
[1] Edit found entry\n\
[2] Delete found entry\n\
[3] Display entry information\n\
[0] Exit\n\
--> " );
                            scanf( "%d", &next_option );
                            while( getchar( ) != '\n' );

                            switch( next_option )
                            {
                                case 1:
                                    {
                                        entry_t *entry;
                                        int field_option;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;

                                        do {
                                            printf( "\
What field to wish to edit?\n\
[ 1] Book title\n\
[ 2] Author\n\
[ 3] Pages\n\
[ 4] Edition\n\
[ 5] Language\n\
[ 6] Publisher\n\
[ 7] Publication date\n\
[ 8] ISBN\n\
[ 9] Description\n\
[10] All fields\n\
[ 0] Exit\n\
--> " );
                                            scanf( "%d", &field_option );
                                            while( getchar( ) != '\n' );

                                            switch( field_option )
                                            {
                                                case 1:
                                                    {
                                                        string_t *title;

                                                        printf( "Enter Book title: " );
                                                        title = string_scan( stdin );

                                                        entry_set_title( entry, title );
                                                    } break;
                                                case 2:
                                                    {
                                                        string_t *author;

                                                        printf( "Enter Author: " );
                                                        author = string_scan( stdin );

                                                        entry_set_author( entry, author );
                                                    } break;
                                                case 3:
                                                    {
                                                        string_t *pages;

                                                        printf( "Enter Pages: " );
                                                        pages = string_scan( stdin );

                                                        entry_set_pages( entry, pages );
                                                    } break;
                                                case 4:
                                                    {
                                                        string_t *edition;

                                                        printf( "Enter Edition: " );
                                                        edition = string_scan( stdin );

                                                        entry_set_edition( entry, edition );
                                                    } break;
                                                case 5:
                                                    {
                                                        string_t *language;

                                                        printf( "Enter Language: " );
                                                        language = string_scan( stdin );

                                                        entry_set_language( entry, language );
                                                    } break;
                                                case 6:
                                                    {
                                                        string_t *publisher;

                                                        printf( "Enter Publisher: " );
                                                        publisher = string_scan( stdin );
                                                        
                                                        entry_set_publisher( entry, publisher );
                                                    } break;
                                                case 7:
                                                    {
                                                        string_t *pubdate;

                                                        printf( "Enter Publication date: " );
                                                        pubdate = string_scan( stdin );

                                                        entry_set_pubdate( entry, pubdate );
                                                    } break;
                                                case 8:
                                                    {
                                                        string_t *isbn;

                                                        printf( "Enter ISBN: " );
                                                        isbn = string_scan( stdin );

                                                        entry_set_isbn( entry, isbn );
                                                    } break;
                                                case 9:
                                                    {
                                                        string_t *description;

                                                        printf( "Description: " );
                                                        description = string_scan( stdin );

                                                        entry_set_description( entry, description );
                                                    } break;
                                                case 10:
                                                    {
                                                        entry_destroy( entry );
                                                        entry = entry_prompt( );
                                                    } break;
                                            }
                                        } while( field_option != 0 );

                                    } break;
                                case 2:
                                    {
                                        entry_node_t *it;
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;
                                        it = book->a_head;

                                        while( it != NULL && it->n_entry != entry )
                                        {
                                            it = it->n_next;
                                        }

                                        book_remove( book, it );
                                        printf( "Book removed successfully\n" );
                                    } break;
                                case 3:
                                    {
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;
                                        entry_print( stdout, entry );
                                    } break;
                            }
                        } while( next_option != 0 );

                        book_destroy( result, 0 );
                    } break;
                case FIND_BY_AUTHOR:
                    {
                        char author[ MAXLENGTH ];
                        book_t *result;
                        entry_node_t *it;
                        int next_option;
                        unsigned count;

                        printf( "Enter Author: " );
                        scanf( "%[^\n]", author );
                        while( getchar( ) != '\n' );

                        result = book_find_by_author( book, author );
                        it = result->a_head;
                        count = 0;

                        printf( "List of entry found\n" );

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_author->s_ptr,
                                    it->n_entry->e_title->s_ptr,
                                    it->n_entry->e_publisher->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "No results found for \"%s\"", author );
                            break;
                        }

                        do {
                            printf( "\
[1] Edit found entry\n\
[2] Delete found entry\n\
[3] Display entry information\n\
[0] Exit\n\
--> " );
                            scanf( "%d", &next_option );
                            while( getchar( ) != '\n' );

                            switch( next_option )
                            {
                                case 1:
                                    {
                                        entry_t *entry;
                                        int field_option;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;

                                        do {
                                            printf( "\
What field to wish to edit?\n\
[ 1] Book title\n\
[ 2] Author\n\
[ 3] Pages\n\
[ 4] Edition\n\
[ 5] Language\n\
[ 6] Publisher\n\
[ 7] Publication date\n\
[ 8] ISBN\n\
[ 9] Description\n\
[10] All fields\n\
[ 0] Exit\n\
--> " );
                                            scanf( "%d", &field_option );
                                            while( getchar( ) != '\n' );

                                            switch( field_option )
                                            {
                                                case 1:
                                                    {
                                                        string_t *title;

                                                        printf( "Enter Book title: " );
                                                        title = string_scan( stdin );

                                                        entry_set_title( entry, title );
                                                    } break;
                                                case 2:
                                                    {
                                                        string_t *author;

                                                        printf( "Enter Author: " );
                                                        author = string_scan( stdin );

                                                        entry_set_author( entry, author );
                                                    } break;
                                                case 3:
                                                    {
                                                        string_t *pages;

                                                        printf( "Enter Pages: " );
                                                        pages = string_scan( stdin );

                                                        entry_set_pages( entry, pages );
                                                    } break;
                                                case 4:
                                                    {
                                                        string_t *edition;

                                                        printf( "Enter Edition: " );
                                                        edition = string_scan( stdin );

                                                        entry_set_edition( entry, edition );
                                                    } break;
                                                case 5:
                                                    {
                                                        string_t *language;

                                                        printf( "Enter Language: " );
                                                        language = string_scan( stdin );

                                                        entry_set_language( entry, language );
                                                    } break;
                                                case 6:
                                                    {
                                                        string_t *publisher;
                                                        
                                                        printf( "Enter Publisher: " );
                                                        publisher = string_scan( stdin );
                                                        
                                                        entry_set_publisher( entry, publisher );
                                                    } break;
                                                case 7:
                                                    {
                                                        string_t *pubdate;

                                                        printf( "Enter Publication date: " );
                                                        pubdate = string_scan( stdin );

                                                        entry_set_pubdate( entry, pubdate );
                                                    } break;
                                                case 8:
                                                    {
                                                        string_t *isbn;

                                                        printf( "Enter ISBN: " );
                                                        isbn = string_scan( stdin );

                                                        entry_set_isbn( entry, isbn );
                                                    } break;
                                                case 9:
                                                    {
                                                        string_t *description;

                                                        printf( "Description: " );
                                                        description = string_scan( stdin );

                                                        entry_set_description( entry, description );
                                                    } break;
                                                case 10:
                                                    {
                                                        entry_destroy( entry );
                                                        entry = entry_prompt( );
                                                    } break;
                                            }
                                        } while( field_option != 0 );

                                    } break;
                                case 2:
                                    {
                                        entry_node_t *it;
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;
                                        it = book->a_head;

                                        while( it != NULL && it->n_entry != entry )
                                        {
                                            it = it->n_next;
                                        }

                                        book_remove( book, it );
                                        printf( "Book removed successfully\n" );
                                    } break;
                                case 3:
                                    {
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;
                                        entry_print( stdout, entry );
                                    } break;
                            }
                        } while( next_option != 0 );

                        book_destroy( result, 0 );
                    } break;
                case FIND_BY_PUBLISHER:
                    {
                        char publisher[ MAXLENGTH ];
                        book_t *result;
                        entry_node_t *it;
                        int next_option;
                        unsigned count;

                        printf( "Enter Publisher: " );
                        scanf( "%[^\n]", publisher );
                        while( getchar( ) != '\n' );

                        result = book_find_by_publisher( book, publisher );
                        it = result->a_head;
                        count = 0;

                        printf( "List of entries found\n" );

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_author->s_ptr,
                                    it->n_entry->e_title->s_ptr,
                                    it->n_entry->e_publisher->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "No results found for \"%s\"", publisher );
                            break;
                        }

                        do {
                            printf( "\
[1] Edit found entry\n\
[2] Delete found entry\n\
[3] Display entry information\n\
[0] Exit\n\
--> " );
                            scanf( "%d", &next_option );
                            while( getchar( ) != '\n' );

                            switch( next_option )
                            {
                                case 1:
                                    {
                                        entry_t *entry;
                                        int field_option;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;

                                        do {
                                            printf( "\
What field to wish to edit?\n\
[ 1] Book title\n\
[ 2] Author\n\
[ 3] Pages\n\
[ 4] Edition\n\
[ 5] Language\n\
[ 6] Publisher\n\
[ 7] Publication date\n\
[ 8] ISBN\n\
[ 9] Description\n\
[10] All fields\n\
[ 0] Exit\n\
--> " );
                                            scanf( "%d", &field_option );
                                            while( getchar( ) != '\n' );

                                            switch( field_option )
                                            {
                                                case 1:
                                                    {
                                                        string_t *title;

                                                        printf( "Enter Book title: " );
                                                        title = string_scan( stdin );

                                                        entry_set_title( entry, title );
                                                    } break;
                                                case 2:
                                                    {
                                                        string_t *author;

                                                        printf( "Enter Author: " );
                                                        author = string_scan( stdin );

                                                        entry_set_author( entry, author );
                                                    } break;
                                                case 3:
                                                    {
                                                        string_t *pages;

                                                        printf( "Enter Pages: " );
                                                        pages = string_scan( stdin );

                                                        entry_set_pages( entry, pages );
                                                    } break;
                                                case 4:
                                                    {
                                                        string_t *edition;

                                                        printf( "Enter Edition: " );
                                                        edition = string_scan( stdin );

                                                        entry_set_edition( entry, edition );
                                                    } break;
                                                case 5:
                                                    {
                                                        string_t *language;

                                                        printf( "Enter Language: " );
                                                        language = string_scan( stdin );

                                                        entry_set_language( entry, language );
                                                    } break;
                                                case 6:
                                                    {
                                                        string_t *publisher;
                                                           
                                                        printf( "Enter Publisher: " );
                                                        publisher = string_scan( stdin );
                                                        
                                                        entry_set_publisher( entry, publisher );
                                                    } break;
                                                case 7:
                                                    {
                                                        string_t *pubdate;

                                                        printf( "Enter Publication date: " );
                                                        pubdate = string_scan( stdin );

                                                        entry_set_pubdate( entry, pubdate );
                                                    } break;
                                                case 8:
                                                    {
                                                        string_t *isbn;

                                                        printf( "Enter ISBN: " );
                                                        isbn = string_scan( stdin );

                                                        entry_set_isbn( entry, isbn );
                                                    } break;
                                                case 9:
                                                    {
                                                        string_t *description;

                                                        printf( "Description: " );
                                                        description = string_scan( stdin );

                                                        entry_set_description( entry, description );
                                                    } break;
                                                case 10:
                                                    {
                                                        entry_destroy( entry );
                                                        entry = entry_prompt( );
                                                    } break;
                                            }
                                        } while( field_option != 0 );

                                    } break;
                                case 2:
                                    {
                                        entry_node_t *it;
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;
                                        it = book->a_head;

                                        while( it != NULL && it->n_entry != entry )
                                        {
                                            it = it->n_next;
                                        }

                                        book_remove( book, it );
                                        printf( "Book removed successfully\n" );
                                    } break;
                                case 3:
                                    {
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;
                                        entry_print( stdout, entry );
                                    } break;
                            }
                        } while( next_option != 0 );

                        book_destroy( result, 0 );
                    } break;
                case EDIT:
                    {
                        entry_node_t *it;
                        int next_option;
                        unsigned count;

                        it = book->a_head;
                        count = 0;

                        printf( "List of entries found\n" );

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_author->s_ptr,
                                    it->n_entry->e_title->s_ptr,
                                    it->n_entry->e_publisher->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "There are no entries in your book store\n" );
                            break;
                        }

                        do {
                            printf( "\
[1] Edit entry\n\
[2] Delete entry\n\
[3] Display entry information\n\
[0] Exit\n\
--> " );
                            scanf( "%d", &next_option );
                            while( getchar( ) != '\n' );

                            switch( next_option )
                            {
                                case 1:
                                    {
                                        entry_t *entry;
                                        int field_option;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;

                                        do {
                                            printf( "\
What field to wish to edit?\n\
[ 1] Book title\n\
[ 2] Author\n\
[ 3] Pages\n\
[ 4] Edition\n\
[ 5] Language\n\
[ 6] Publisher\n\
[ 7] Publication date\n\
[ 8] ISBN\n\
[ 9] Description\n\
[10] All fields\n\
[ 0] Exit\n\
--> " );
                                            scanf( "%d", &field_option );
                                            while( getchar( ) != '\n' );

                                            switch( field_option )
                                            {
                                                case 1:
                                                    {
                                                        string_t *title;

                                                        printf( "Enter Book title: " );
                                                        title = string_scan( stdin );

                                                        entry_set_title( entry, title );
                                                    } break;
                                                case 2:
                                                    {
                                                        string_t *author;

                                                        printf( "Enter Author: " );
                                                        author = string_scan( stdin );

                                                        entry_set_author( entry, author );
                                                    } break;
                                                case 3:
                                                    {
                                                        string_t *pages;

                                                        printf( "Enter Pages: " );
                                                        pages = string_scan( stdin );

                                                        entry_set_pages( entry, pages );
                                                    } break;
                                                case 4:
                                                    {
                                                        string_t *edition;

                                                        printf( "Enter Edition: " );
                                                        edition = string_scan( stdin );

                                                        entry_set_edition( entry, edition );
                                                    } break;
                                                case 5:
                                                    {
                                                        string_t *language;

                                                        printf( "Enter Language: " );
                                                        language = string_scan( stdin );

                                                        entry_set_language( entry, language );
                                                    } break;
                                                case 6:
                                                    {
                                                        string_t *publisher;
                                                        
                                                        printf( "Enter Publisher: " );
                                                        publisher = string_scan( stdin );
                                                        
                                                        entry_set_publisher( entry, publisher );
                                                    } break;
                                                case 7:
                                                    {
                                                        string_t *pubdate;

                                                        printf( "Enter Publication date: " );
                                                        pubdate = string_scan( stdin );

                                                        entry_set_pubdate( entry, pubdate );
                                                    } break;
                                                case 8:
                                                    {
                                                        string_t *isbn;

                                                        printf( "Enter ISBN: " );
                                                        isbn = string_scan( stdin );

                                                        entry_set_isbn( entry, isbn );
                                                    } break;
                                                case 9:
                                                    {
                                                        string_t *description;

                                                        printf( "Description: " );
                                                        description = string_scan( stdin );

                                                        entry_set_description( entry, description );
                                                    } break;
                                                case 10:
                                                    {
                                                        entry_destroy( entry );
                                                        entry = entry_prompt( );
                                                    } break;
                                            }
                                        } while( field_option != 0 );

                                    } break;
                                case 2:
                                    {
                                        entry_node_t *it;
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;
                                        it = book->a_head;

                                        while( it != NULL && it->n_entry != entry )
                                        {
                                            it = it->n_next;
                                        }

                                        book_remove( book, it );
                                        printf( "Book removed successfully\n" );
                                    } break;
                                case 3:
                                    {
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = book_get( book, index-1 )->n_entry;
                                        entry_print( stdout, entry );
                                    } break;
                            }
                        } while( next_option != 0 );
                    } break;
                case DELETE:
                    {
                        unsigned count, index;
                        entry_node_t *it;

                        it = book->a_head;
                        count = 0;

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_author->s_ptr,
                                    it->n_entry->e_title->s_ptr,
                                    it->n_entry->e_publisher->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "There are not entries in the book store.. EXITING\n" );
                            break;
                        }

                        printf( "Enter index from list: " );
                        scanf( "%d", &index );
                        while( getchar( ) != '\n' );

                        it = book->a_head;

                        while( index > 1 )
                        {
                            it = it->n_next;
                            index--;
                        }

                        book_remove( book, it );
                        printf( "Entry successfully removed\n" );
                    } break;
            }
        } while( option != 0 );

        if( ( file = fopen( FILENAME, "w+" ) ) == NULL )
        {
            perror( "fopen" );
            return EXIT_FAILURE;
        }
        else
        {
            book_write( file, book );
            fclose( file );
        }

        book_destroy( book, 1 );
    }
    else if( logged == 0 )
    {
        printf( "Fail to login\n" );
    }

    return 0;
}

int login( void )
{
    struct termios tmp_term;
    struct sigaction sa_sigint;
    char username[ MAXLENGTH ],
         password[ MAXLENGTH ],
         scanned_line[ MAXLENGTH ],
         scanned_username[ MAXLENGTH ],
         scanned_password[ MAXLENGTH ];
    FILE *file;

    printf( "Enter your username: " );
    scanf( "%s", username );

    if( tcgetattr( fileno( stdin ), &saved_term ) == -1 ) 
    {
        perror( "tcgetattr" );
        exit( EXIT_FAILURE );
    }

    tmp_term = saved_term;

    memset( &sa_sigint, 0, sizeof( struct sigaction ) );
    sa_sigint.sa_handler = sigint_handler;
    sa_sigint.sa_flags = 0;

    if( sigaction( SIGINT, &sa_sigint, NULL ) < 0 )
    {
        perror( "sigaction" );
        exit( EXIT_FAILURE );
    }

    tmp_term.c_lflag &= ~ECHO;
    if( tcsetattr( fileno( stdin ), TCSANOW, &tmp_term ) == -1 ) 
    {
        perror( "tcgetattr" );
        exit( EXIT_FAILURE );
    }

    printf( "Enter password: " );
    scanf( "%s", password );

    restore_terminal( );

    if( ( file = fopen( "credentials.txt", "r" ) ) == NULL )
    {
        perror( "fopen" );
        return -1;
    }

    while( !feof( file ) )
    {
        int items_scanned;
        char *ptr1, *ptr2;

        items_scanned = fscanf( file, "%[^\n]\n", scanned_line );

        if( scanned_line[ 0 ] == '#' )
        {
            continue;
        }

        /* parse and copy scanned line from credentials file */
        ptr1 = strchr( scanned_line, ':' );
        strncpy( scanned_username, scanned_line, ptr1-scanned_line );
        scanned_username[ ptr1-scanned_line ] = '\0';

        ptr2 = strchr( ptr1 + 1, ':' );
        strncpy( scanned_password, ptr1 + 1, ptr2-ptr1-1 );
        scanned_password[ ptr2-ptr1-1 ] = '\0';
        strcpy( FILENAME, ptr2 + 1 );

        if( items_scanned == 1
                && strcmp( username, scanned_username ) == 0
                && strcmp( password, scanned_password ) == 0 )
        {
            fclose( file );
            return 1;
        }
    }

    fclose( file );
    return 0;
}

entry_t *entry_prompt( void )
{
    entry_t *entry;

    if( ( entry = entry_create( ) ) == NULL )
    {
        perror( "entry_create" );
        return NULL;
    }

    printf( "Book title:        " );
    entry_set_title( entry, string_scan( stdin ) );
    printf( "Author:         " );
    entry_set_author( entry, string_scan( stdin ) );
    printf( "Pages:         " );
    entry_set_pages( entry, string_scan( stdin ) );
    printf( "Edition:         " );
    entry_set_edition( entry, string_scan( stdin ) );
    printf( "Language:   " );
    entry_set_language( entry, string_scan( stdin ) );
    printf( "Publisher:   " );
    entry_set_publisher( entry, string_scan( stdin ) );
    printf( "Publication date:       " );
    entry_set_pubdate( entry, string_scan( stdin ) );
    printf( "ISBN:     " );
    entry_set_isbn( entry, string_scan( stdin ) );
    printf( "Description:   " );
    entry_set_description( entry, string_scan( stdin ) );

    return entry;
}

void restore_terminal( void )
{
    if( tcsetattr( fileno( stdin ), TCSANOW, &saved_term ) == -1 )
    {
        perror( "tcsetattr" );
        exit( EXIT_FAILURE );
    }
}

void sigint_handler( int sig )
{
    if( SIGINT == sig )
   {
        restore_terminal( );
    }

    exit( EXIT_SUCCESS );
}
