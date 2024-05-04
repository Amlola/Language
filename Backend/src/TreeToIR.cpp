#include "../inc/ir.h"




































/*----------------------------------------------DUMP_FUNC--------------------------------------------*/


void ListGraphDumpFunction(LIST* list, const char* path, const char* signature, unsigned line)
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
                    i, i, list->data[i].value, list->data[i].next, list->data[i].prev);
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
    }