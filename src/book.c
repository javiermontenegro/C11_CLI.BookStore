#include <book.h>

book_t *book_create( void )
{
    book_t *book;

    if( ( book = malloc( sizeof( book_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    memset( book, 0, sizeof( book_t ) );

    return book;
}

book_t *book_duplicate( const book_t *book )
{
    book_t *duplicate;

    if( ( duplicate = malloc( sizeof( book_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    if( book_add_all( duplicate, ( book_t* )book ) == -1 )
    {
        book_destroy( duplicate, 1 );
        errno = ENOMEM;

        return NULL;
    }

    return duplicate;
}

entry_node_t *book_get( const book_t *book, unsigned index )
{
    entry_node_t *it;
    unsigned i;

    it = book->a_head;
    i = 0;

    while( it != NULL && i < index )
    {
        it = it->n_next;
        i++;
    }

    if( it == NULL )
    {
        errno = EINVAL;
        return NULL;
    }

    return it;
}

void book_write( FILE *file, book_t *book )
{
    entry_node_t *it;
    unsigned count;

    it = book->a_head;
    count = 0;

    while( it != NULL )
    {
        it = it->n_next;
        count++;
    }

    fwrite( &count, sizeof( count ), 1, file );

    it = book->a_head;

    while( it != NULL )
    {
        entry_write( file, it->n_entry );
        it = it->n_next;
    }
}

book_t *book_read( FILE *file )
{
    book_t *book;
    entry_t *entry;
    unsigned count;

    fread( &count, sizeof( count ), 1, file );

    book = book_create( );

    while( count > 0 )
    {
        entry = entry_read( file );

        if( book_add( book, entry ) == -1 )
        {
            book_destroy( book, 1 );
            errno = ENOMEM;

            return NULL;
        }

        count--;
    }

    return book;
}

int book_add( book_t *book, entry_t *entry )
{
    entry_node_t *node;

    if( ( node = malloc( sizeof( entry_node_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return -1;
    }

    if( ( node->n_entry = entry_duplicate( entry ) ) == NULL )
    {
        errno = ENOMEM;
        free( node );
    
        return -1;
    }
    

    node->n_prev = node->n_next = NULL;
    /* node->n_entry = entry; */

    if( book->a_head == NULL )
    {
        book->a_head = node;
    }
    else if( book->a_tail == NULL )
    {
        book->a_tail = node;
        book->a_head->n_next = book->a_tail;
        book->a_tail->n_prev = book->a_head;
    }
    else
    {
        node->n_prev = book->a_tail;
        book->a_tail->n_next = node;
        book->a_tail = node;
    }

    return 0;
}

int book_add_all( book_t *book, book_t *some_book )
{
    entry_node_t *it;

    it = some_book->a_head;

    while( it != NULL )
    {
        if( book_add( book, it->n_entry ) == -1 )
        {
            errno = ENOMEM;
            return -1;
        }

        it = it->n_next;
    }

    return 0;
}

int book_add_many( book_t *book, int count, ... )
{
    va_list ap;

    va_start( ap, count );

    while( count > 0 )
    {
        if( book_add( book, va_arg( ap, entry_t* ) ) == -1 )
        {
            book_destroy( book, 1 );
            errno = ENOMEM;

            return -1;
        }

        count--;
    }

    va_end( ap );

    return 0;
}

book_t *book_find_by_title( const book_t *book, const char *title )
{
    entry_node_t *it, *tmp;
    book_t *retval;

    if( ( retval = book_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    it = book->a_head;

    while( it != NULL )
    {
        if( strcmp( it->n_entry->e_title->s_ptr, title ) == 0 )
        {
            if( ( tmp = malloc( sizeof( entry_node_t ) ) ) == NULL )
            {
                book_destroy( retval, 0 );
                errno = ENOMEM;

                return NULL;
            }

            tmp->n_prev = tmp->n_next = NULL;
            tmp->n_entry = it->n_entry;

            if( retval->a_head == NULL )
            {
                retval->a_head = tmp;
            }
            else if( retval->a_tail == NULL )
            {
                retval->a_tail = tmp;
                retval->a_head->n_next = retval->a_tail;
                retval->a_tail->n_prev = retval->a_head;
            }
            else
            {
                tmp->n_prev = retval->a_tail;
                retval->a_tail->n_next = tmp;
                retval->a_tail = tmp;
            }
        }

        it = it->n_next;
    }

    return retval;
}

book_t *book_find_by_author( const book_t *book, const char *author )
{
    entry_node_t *it, *tmp;
    book_t *retval;

    if( ( retval = book_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    it = book->a_head;

    while( it != NULL )
    {
        if( strcmp( it->n_entry->e_author->s_ptr, author ) == 0 )
        {
            if( ( tmp = malloc( sizeof( entry_node_t ) ) ) == NULL )
            {
                book_destroy( retval, 0 );
                errno = ENOMEM;

                return NULL;
            }

            tmp->n_prev = tmp->n_next = NULL;
            tmp->n_entry = it->n_entry;

            if( retval->a_head == NULL )
            {
                retval->a_head = tmp;
            }
            else if( retval->a_tail == NULL )
            {
                retval->a_tail = tmp;
                retval->a_head->n_next = retval->a_tail;
                retval->a_tail->n_prev = retval->a_head;
            }
            else
            {
                tmp->n_prev = retval->a_tail;
                retval->a_tail->n_next = tmp;
                retval->a_tail = tmp;
            }
        }

        it = it->n_next;
    }

    return retval;
}

book_t *book_find_by_publisher( const book_t *book, const char *publisher )
{
    entry_node_t *it, *tmp;
    book_t *retval;

    if( ( retval = book_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    it = book->a_head;

    while( it != NULL )
    {
        if( strcmp( it->n_entry->e_publisher->s_ptr, publisher ) == 0 )
        {
            if( ( tmp = malloc( sizeof( entry_node_t ) ) ) == NULL )
            {
                book_destroy( retval, 0 );
                errno = ENOMEM;

                return NULL;
            }

            tmp->n_prev = tmp->n_next = NULL;
            tmp->n_entry = it->n_entry;

            if( retval->a_head == NULL )
            {
                retval->a_head = tmp;
            }
            else if( retval->a_tail == NULL )
            {
                retval->a_tail = tmp;
                retval->a_head->n_next = retval->a_tail;
                retval->a_tail->n_prev = retval->a_head;
            }
            else
            {
                tmp->n_prev = retval->a_tail;
                retval->a_tail->n_next = tmp;
                retval->a_tail = tmp;
            }
        }

        it = it->n_next;
    }

    return retval;
}

entry_t *book_remove( book_t *book, entry_node_t *entry_node )
{
    entry_t *retval;

    if( book->a_head == entry_node )
    {
        retval = book->a_head->n_entry;
        book->a_head = book->a_head->n_next;

        if( book->a_head != NULL )
        {
            book->a_head->n_prev = NULL;
        }
    }
    else if( book->a_tail == entry_node )
    {
        retval = book->a_tail->n_entry;
        book->a_tail = book->a_tail->n_prev;

        if( book->a_tail != NULL )
        {
            book->a_tail->n_next = NULL;
        }
    }
    else
    {
        retval = entry_node->n_entry;
        entry_node->n_prev->n_next = entry_node->n_next;
        entry_node->n_next->n_prev = entry_node->n_prev;
    }

    free( entry_node );

    return retval;
}

book_t *book_remove_all( book_t *book, book_t *some_book )
{
    entry_node_t *it1, *it2;

    it1 = some_book->a_head;

    while( it1 != NULL )
    {
        it2 = book->a_head;

        while( it2 != NULL )
        {
            if( it1->n_entry == it2->n_entry )
            {
                book_remove( book, it2 );
            }

            it2 = it2->n_next;
        }

        it1 = it1->n_next;
    }

    return some_book;
}

void book_destroy( book_t *book, int all )
{
    entry_node_t *it, *next;

    it = book->a_head;

    while( it != NULL )
    {
        next = it->n_next;

        if( all )
        {
            entry_destroy( it->n_entry );
        }

        free( it );
        it = next;
    }

    free( book );
}
