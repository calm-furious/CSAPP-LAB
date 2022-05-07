
#define DECL_LIST(typename) struct typename *prev, *next;

#define INSERT_AFTER(head, pos, elem)                       \
    (elem)->prev = pos;                                     \
    if (pos)                                                \
        (elem)->next = (pos)->next;                         \
    else {                                                  \
        (elem)->next = *(head);                             \
        *(head) = elem;                                     \
    }                                                       \
    if((elem)->next)                                        \
        (elem)->next->prev = elem;                          \
    if((elem)->prev)                                        \
        (elem)->prev->next = elem;
    
#define DELETE_THIS(head, elem)             \
    if((elem)->next)                        \
        (elem)->next->prev = (elem)->prev;  \
    if((elem)->prev)                        \
        (elem)->prev->next = (elem)->next;  \
    if(!(elem)->prev)                       \
        *(head) = (elem)->next;

#define REPLACE_WITH(head, pos, elem)       \
    INSERT_AFTER(head, pos, elem);          \
    DELETE_THIS(head, pos)
