/* 
  Try to reproduce xpc's list in c-macro
*/

#define DECL_LIST(typename) struct typename * prev, *next;

#define INSERT_AFTER(head, pos, elem)       \
        if(!pos){ \
          (elem)->next = *(head);                \
          (elem)->prev = NULL; \
          if( *(head) ) (*(head))->prev = (elem); \
          *(head) = (elem);                     \
        }else{ \
          (elem)->prev = (pos);                   \
          (elem)->next = (pos)->next;             \
          if( (pos)->next ) {\
            (pos)->next->prev = elem;             \
          }\
          (pos)->next = elem;                   \
        }

#define DELETE_THIS(head, elem)             \
          if((elem)->prev) (elem)->prev->next = (elem)->next;\
          if((elem)->next) (elem)->next->prev = (elem)->prev;\
          if( *(head) == elem ) \
            *(head) = (*(head))->next;\
          

#define REPLACE_WITH(head, pos, elem)       \
        INSERT_AFTER(head,pos,elem) \
        DELETE_THIS(head,pos)
