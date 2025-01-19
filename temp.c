#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <ctype.h>

char *readFile()

{
    FILE *file = fopen("task.json", "r");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", "task.json");
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file content
    char *content = (char *)malloc(file_size + 1);
    if (content == NULL)
    {
        printf("Memory allocation failed!\n");
        fclose(file);
        return NULL;
    }

    // Read the file content
    fread(content, 1, file_size, file);
    content[file_size] = '\0'; // Null-terminate the string

    fclose(file);
    return content;
}

void writeFile(const char *filename, const char *content)
{
    FILE *file = fopen("task.json", "w");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);
}

char **situationkList(WINDOW *win)
{

    cJSON *root = cJSON_Parse(readFile());

    cJSON *tasks_array = cJSON_GetObjectItem(root, "sit");

    int array_size = cJSON_GetArraySize(tasks_array);
    char **tasks = (char **)malloc(array_size * sizeof(char *));

    for (int i = 0; i < array_size; i++)
    {
        cJSON *task = cJSON_GetArrayItem(tasks_array, i);
        if (cJSON_IsString(task))
        {
            tasks[i] = (char *)malloc(strlen(task->valuestring) + 1);
            if (tasks[i] != NULL)
            {
                strcpy(tasks[i], task->valuestring);
            }
            else
            {
                tasks[i] = NULL;
            }
        }
        else
        {
            tasks[i] = NULL;
        }
    }

    return tasks;
}

int sizeTask()
{

    cJSON *root = cJSON_Parse(readFile());

    cJSON *tasks_array = cJSON_GetObjectItem(root, "tasks");

    int array_size = cJSON_GetArraySize(tasks_array);

    return array_size;
}

char **taskList(WINDOW *win)
{

    cJSON *root = cJSON_Parse(readFile());

    cJSON *tasks_array = cJSON_GetObjectItem(root, "tasks");

    int array_size = cJSON_GetArraySize(tasks_array);
    char **tasks = (char **)malloc(array_size * sizeof(char *));

    for (int i = 0; i < array_size; i++)
    {
        cJSON *task = cJSON_GetArrayItem(tasks_array, i);
        if (cJSON_IsString(task))
        {
            tasks[i] = (char *)malloc(strlen(task->valuestring) + 1);
            if (tasks[i] != NULL)
            {
                strcpy(tasks[i], task->valuestring);
            }
            else
            {
                tasks[i] = NULL;
            }
        }
        else
        {
            tasks[i] = NULL;
        }
    }

    return tasks;
}

void taskAdder(char *task, WINDOW *task_win)
{

    cJSON *root = cJSON_Parse(readFile());
    cJSON *array_task = cJSON_GetObjectItem(root, "tasks");
    cJSON *array_situation = cJSON_GetObjectItem(root, "sit");
    cJSON_AddItemToArray(array_task, cJSON_CreateString(task));
    cJSON_AddItemToArray(array_situation, cJSON_CreateString("o"));
    char *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    free(updated_json_string);
    wrefresh(task_win);
    refresh();
}

void modalHandler(WINDOW *win, WINDOW *task_win)
{
    curs_set(1);
    char task[50] = "";
    int ch, i = 0;
    mvwprintw(win, 1, 1, "Enter your task: ");
    mvwprintw(win, 2, 1, "");

    wrefresh(win);
    while ((ch = wgetch(win)) != '\n' && i < sizeof(task) - 1)
    {
        if (ch == KEY_BACKSPACE || ch == 127)
        {
            if (i > 0)
            {
                i--;
                task[i] = '\0';
                mvwprintw(win, 2, 1 + i, " ");
            }
        }
        else
        {
            task[i] = ch;
            i++;
            task[i] = '\0';
        }
        mvwprintw(win, 2, 1, "%s", task);
        wrefresh(win);
    }
    taskAdder(task, task_win);
    wrefresh(task_win);
    mvwprintw(win, 6, 1, "Press any key to exit...");
    wrefresh(win);
    wgetch(win);
    wclear(win);
    wrefresh(win);
    delwin(win);
}

void delete_task(int index_of_task, WINDOW *win)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *tasks_array = cJSON_GetObjectItem(root, "tasks");
    cJSON *situation_array = cJSON_GetObjectItem(root, "sit");
    cJSON_DeleteItemFromArray(tasks_array, index_of_task);
    cJSON_DeleteItemFromArray(situation_array, index_of_task);
    cJSON *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    werase(win);
    box(win, 0, 0);
}
void editJSON(int index, char *new_text)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *task_array = cJSON_GetObjectItem(root, "tasks");
    cJSON *edited_item = cJSON_CreateString(new_text);
    cJSON_ReplaceItemInArray(task_array, index, edited_item);
    cJSON *updated_json = cJSON_Print(root);
    writeFile("task.json", updated_json);

    free(updated_json);
}
void editHandler(int index, char *task_text)
{
    keypad(stdscr, TRUE);
    WINDOW *win = newwin(18, 46, 9, 23);
    box(win, 0, 0);
    wrefresh(win);
    curs_set(1);

    int cursor_pos = strlen(task_text);
    wprintw(win, "Edit your task");
    mvwprintw(win, 1, 1, "%s", task_text);
    wmove(win, 1, cursor_pos + 1);
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != '\n')
    {
        switch (ch)
        {
        case KEY_BACKSPACE:
        case 127:
        case 8:
            if (cursor_pos > 0)
            {
                memmove(&task_text[cursor_pos - 1], &task_text[cursor_pos], strlen(task_text) - cursor_pos + 1);
                cursor_pos--;
            }
            break;
        default:
            if (isprint(ch))
            {
                memmove(&task_text[cursor_pos + 1], &task_text[cursor_pos], strlen(task_text) - cursor_pos + 1);
                task_text[cursor_pos] = ch;
                cursor_pos++;
            }
            break;
        }

        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 1, "%s", task_text);
        wrefresh(win);

        wmove(win, 1, cursor_pos + 1);
        wrefresh(win);
    }

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Final text: %s", task_text);
    mvwprintw(win, 2, 1, "Press any key to exit...");
    wrefresh(win);
    wgetch(win);
    editJSON(index, task_text);
}

int main()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    WINDOW *tasks = newwin(36, 92, 0, 0);
    // WINDOW *description = newwin(36, 92, 0, 0);
    //   WINDOW * category = newwin(9, 92, 27, 92);
    //   WINDOW * subtasks = newwin(9, 92, 36, 0);
    //   WINDOW * deadline = newwin(9, 92, 36, 92);
    box(tasks, 0, 0);
    // box(description, 0, 0);
    //   box(category, 0, 0);
    //   box(subtasks, 0, 0);
    //   box(deadline, 0, 0);
    refresh();
    wprintw(tasks, "tasks");
    // wprintw(description, "description");
    //   wprintw(category, "category");
    //   wprintw(subtasks, "subtasks");
    //   wprintw(deadline, "deadline");

    wrefresh(tasks);
    //   wrefresh(description);
    //   wrefresh(category);
    //   wrefresh(subtasks);
    //   wrefresh(deadline);
    char **task_list;
    char **sitList;
    int size_task;

    wrefresh(tasks);

    keypad(tasks, true);
    int choice;
    int highlighted = 0;
    while (1)
    {
        task_list = taskList(tasks);
        sitList = situationkList(tasks);
        size_task = sizeTask();
        wrefresh(tasks);

        for (int i = 0; i < size_task; i++)
        {
            if (highlighted == i)
            {
                wattron(tasks, A_REVERSE);
            }

            mvwprintw(tasks, i + 1, 1, "%d", i + 1);
            mvwprintw(tasks, i + 1, 2, ".[");
            mvwprintw(tasks, i + 1, 4, sitList[i]);
            mvwprintw(tasks, i + 1, 5, "]");
            mvwprintw(tasks, i + 1, 6, task_list[i]);

            wattroff(tasks, A_REVERSE);
        }
        wrefresh(tasks);
        choice = wgetch(tasks);
        switch (choice)
        {
        case 'j':
            highlighted--;
            if (highlighted < 0)
            {
                highlighted = 0;
            }

            break;
        case 'k':
            highlighted++;
            if (highlighted > size_task - 1)
            {
                highlighted = size_task - 1;
            }
            break;
        case 'a':
            WINDOW *modal = newwin(18, 46, 9, 23);
            box(modal, 0, 0);
            modalHandler(modal, tasks);
            curs_set(0);
            break;
        case 'd':

            delete_task(highlighted, tasks);
            wrefresh(tasks);
            highlighted--;
            if (highlighted == -1)
            {
                highlighted = 0;
            }

            wrefresh(tasks);

            break;
        case 'e':
            editHandler(highlighted, task_list[highlighted]);
            werase(tasks);
            box(tasks, 0, 0);
            curs_set(0);
            break;
        default:
            break;
        }
        if (choice == 'q')
        {
            break;
        }
    }

    refresh();
    endwin();

    return 0;
}
