#include "list.h"


int ListCtor(LIST *list)
    {
    assert(list);

    list->capacity = capacity_list;

    list->size     = size_list;

    list->status   = LIST_OK;

    list->free     = 1;

    list->front    = ZERO;

    list->back     = ZERO;

    list->data = (Node*)calloc(list->capacity, sizeof(Node));

    if (list->data == nullptr)
        {
        return ALLOC_FAIL;
        }

    SetList(list, 1);

    list->data[ZERO].next = ZERO;

    list->data[ZERO].prev = ZERO;

    return list->status;
    }


int ListVerify(LIST* list)
    {
    if (list->data == nullptr)
        {
        list->status |= NULL_DATA_BASE;
        }

    if (list == NULL)
        {
        list->status |= NULL_LIST;
        }

    if (list->capacity < ZERO)
        {
        list->status |= CAPACITY_LESS_THAN_ZERO;
        }

    if (list->size < ZERO)
        {
        list->status |= SIZE_LESS_ZERO;
        }


    if (list->size > list->capacity)
        {
        list->status |= CAPACITY_LESS_THAN_SIZE;
        }

    return list->status;
    }


void ListDtor(LIST* list)
    {
    assert(list);

    list->capacity = POISON_NUMBER_FOR_VALUE;

    list->size     = POISON_NUMBER_FOR_VALUE;

    list->free     = POISON_NUMBER_FOR_VALUE;

    list->front    = POISON_NUMBER_FOR_VALUE;

    list->back     = POISON_NUMBER_FOR_VALUE;

    free(list->data);
    }


int PushFront(LIST* list, List_type value)
    {
    return ListInsert(list, value, list->front);
    }


int PushBack(LIST* list, List_type value)
    {
    return ListInsert(list, value, list->data[list->back].next);
    }


void UpdateParams(LIST* list) 
    {
    list->front = list->data[ZERO].next;

    list->back = list->data[ZERO].prev;
    }


int ListInsert(LIST* list, List_type value, iterator_t index)
    {
    if (list->size + 1 >= list->capacity)
        {
        ListResize(list, list->capacity * size_extend);
        }

    iterator_t old_free = list->free;

    list->free = list->data[old_free].next;

    list->data[old_free].value = value;

    if (list->size == ZERO) 
        {
        list->data[old_free].next = ZERO;

        list->data[old_free].prev = ZERO;

        list->data[ZERO].next = old_free;

        list->data[ZERO].prev = old_free;

        list->front = old_free;

        list->back = old_free;

        list->size++;

        return list->status;
        } 

    else 
        {
        list->data[old_free].next = index;

        list->data[old_free].prev = list->data[index].prev;

        list->data[list->data[index].prev].next = old_free;

        list->data[index].prev = old_free;
        }

    UpdateParams(list);

    list->size++;

    return list->status;
    }


int PopFront(LIST* list)
    {
    return ListDelete(list, list->front);
    }


int PopBack(LIST* list)
    {
    return ListDelete(list, list->back);
    }


int ListDelete(LIST* list, iterator_t index)
    {
    if (index >= list->capacity)
        {
        return INDEX_BIGGER_THAN_CAPACITY;
        }

    if (list->size == ZERO)
        {
        return LIST_IS_EMPTY;
        }

    list->data[list->data[index].prev].next = list->data[index].next;

    list->data[list->data[index].next].prev = list->data[index].prev;

    iterator_t old_free = list->free;

    list->free = index;

    list->data[list->free].prev = FREE_INDEX;

    list->data[list->free].next = old_free;

    UpdateParams(list);

    list->size--;

    return list->status;
    }


static int ListResize(LIST* list, int new_capacity_list)
    {
    if (new_capacity_list <= ZERO)
        {
        return WRONG_NEW_CAPACITY;
        }

    list->capacity = new_capacity_list;

    Node* new_data = (Node*)calloc(new_capacity_list, sizeof(Node));

    if (new_data == nullptr)
        {
        return ALLOC_FAIL;
        }

    for (size_t i = ZERO; i <= list->size; i++)
        {
        (new_data)[i].next  = list->data[i].next;

        (new_data)[i].prev  = list->data[i].prev;

        (new_data)[i].value = list->data[i].value;
        }
    
    free(list->data);

    list->data = new_data;

    SetList(list, list->size + 1);

    list->free = list->size + 1;

    return list->status;    
    }


iterator_t NextCurIndex(LIST* list, size_t index)
    {
    return list->data[index].next;
    }


iterator_t PrevCurIndex(LIST* list, size_t index)
    {
    return list->data[index].prev;
    }


iterator_t Begin(LIST* list)
    {
    return list->front;
    }


iterator_t End(LIST* list)
    {
    return list->back;
    }


iterator_t FindByIndex(LIST* list, size_t index)
    {
    iterator_t cur_ind = Begin(list);

    for (size_t i = ZERO; i < index; i++)
        {
        cur_ind = NextCurIndex(list, index);
        }

    return cur_ind;
    }


static void SetList(LIST* list, size_t size)
    {
    assert(list);

    for (size_t i = size; i < list->capacity; i++)
        {
        list->data[i].next  = i + 1;

        list->data[i].prev  = FREE_INDEX;
        }

    list->data[list->capacity - 1].next  = ZERO;
    }


/*void ListGraphDumpFunction(LIST* list, const char* path, const char* signature, unsigned line)
    {
    assert(list);

    graph_file_list = fopen("dotfile_list.dot", "wb");

    static int dump_number = 1;

    PRINT_TO_GRAPH_FILE("digraph G {bgcolor=RosyBrown rankdir = TB\n" "splines = ortho; edge[minlen = 3, penwidth = 2; color = blue];\n\n");

    PRINT_TO_GRAPH_FILE("label = \"list_dump from function %s, List_Index/%s:%d\";\n", signature, path, line);

    PRINT_TO_GRAPH_FILE("Info[shape=record, fillcolor=\"grey\", width=0.2, style=\"filled\","
            "label=\" {Capacity: %zu | Size: %zu | Free: %d | Front: %d | Back: %d}\"] \n\n",
            list->capacity, list->size, list->free, list->front, list->back);

    PRINT_TO_GRAPH_FILE("{rank = max; node0[shape=record, width=0.2, style=\"filled\", fillcolor=\"purple\","
                "label=\" {id: 0 | value: NILL | next: %d | prev: %d}\"]} \n \n",
                list->data[ZERO].next, list->data[ZERO].prev);

    PRINT_TO_GRAPH_FILE("{rank = same;\n");

    for (size_t i = 1; i < list->capacity; i++)
        {
        if (list->data[i].prev == FREE_INDEX)
            {
            PRINT_TO_GRAPH_FILE("node%zu[shape=record, width=0.2, style=\"filled\", fillcolor=\"red\","
                    "label=\" {id: %zu | value: %s | next: %d | prev: %d}\"] \n \n",
                    i, i, "POIZON", list->data[i].next, list->data[i].prev);
            }
        else
            {
            PRINT_TO_GRAPH_FILE("node%zu[shape=record, width=0.2, style=\"filled\", fillcolor=\"green\","
                    "label=\" {id: %zu | value: %s | next: %d | prev: %d}\"] \n \n",
                    i, i, list->data[i].value.func_name, list->data[i].next, list->data[i].prev);
            }   
        }
    
    PRINT_TO_GRAPH_FILE("}\n");

    PRINT_TO_GRAPH_FILE("Free[color=orange, style=filled]");

    PRINT_TO_GRAPH_FILE("Front[color=orange, style=filled]");

    PRINT_TO_GRAPH_FILE("Back[color=orange, style=filled]");

    PRINT_TO_GRAPH_FILE("Free->node%d\n", list->free);

    PRINT_TO_GRAPH_FILE("Front->node%d\n", list->front);

    PRINT_TO_GRAPH_FILE("Back->node%d\n\n", list->back);

    PRINT_TO_GRAPH_FILE("edge[color=darkgreen, constraint = true]\n");

    PRINT_TO_GRAPH_FILE("node0");

    for (size_t i = 1; i < list->capacity; i++)
        {
        PRINT_TO_GRAPH_FILE(" -> node%zu", i);

        if (i == list->capacity - 1)
            {
            PRINT_TO_GRAPH_FILE("\n[style=invis, weight = 10000];");
            }
        }

    PRINT_TO_GRAPH_FILE("\n\n");

    PRINT_TO_GRAPH_FILE("edge[style=solid, constraint = false]\n");

    for (size_t i = 1; i < list->capacity; i++)
        {
        PRINT_TO_GRAPH_FILE("node%zu -> node%d;\n", i, list->data[i].next);
        }

    PRINT_TO_GRAPH_FILE("\n}");

    fclose(graph_file_list);

    char shell_command[MAX_COMMAND_LENGTH] = "";
    sprintf(shell_command, "dot -v -Tpng dotfile_list.dot -o graph_list%d.png", dump_number);
    system(shell_command);

    dump_number++;
    }*/