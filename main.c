#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <ctype.h>

char *readFile()
{
    FILE *file = fopen("task.json", "r");
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = (char *)malloc(file_size + 1);
    fread(content, 1, file_size, file);
    content[file_size] = '\0';
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

char *desription_json(int index)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *desription_array = cJSON_GetObjectItem(root, "description");
    cJSON *descript = cJSON_GetArrayItem(desription_array, index);
    return descript->valuestring;
}

char *deadline_json(int index)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *desription_array = cJSON_GetObjectItem(root, "deadline");
    cJSON *descript = cJSON_GetArrayItem(desription_array, index);
    return descript->valuestring;
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

char **situationkListSubtask(int index)
{

    cJSON *root = cJSON_Parse(readFile());

    cJSON *tasks_array = cJSON_GetObjectItem(root, "subtasksSit");
    cJSON *subtasks_array = cJSON_GetArrayItem(tasks_array, index);
    int array_size = cJSON_GetArraySize(tasks_array);
    int subtask_size = cJSON_GetArraySize(subtasks_array);

    char **subtask_c_array = malloc(subtask_size * sizeof(char *));
    for (int i = 0; i < subtask_size; i++)
    {
        cJSON *subtask_element = cJSON_GetArrayItem(subtasks_array, i);

        subtask_c_array[i] = strdup(cJSON_GetStringValue(subtask_element));
    }

    return subtask_c_array;
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

int sizeSubtask(int index)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *tasks_array = cJSON_GetObjectItem(root, "subtasks");
    cJSON *subtasks_array = cJSON_GetArrayItem(tasks_array, index);
    int subtask_size = cJSON_GetArraySize(subtasks_array);
    return subtask_size;
}

char **subtaskList(int index)
{

    cJSON *root = cJSON_Parse(readFile());
    cJSON *tasks_array = cJSON_GetObjectItem(root, "subtasks");
    cJSON *subtasks_array = cJSON_GetArrayItem(tasks_array, index);
    int subtask_size = cJSON_GetArraySize(subtasks_array);
    char **subtask_c_array = malloc(subtask_size * sizeof(char *));
    for (int i = 0; i < subtask_size; i++)
    {
        cJSON *subtask_element = cJSON_GetArrayItem(subtasks_array, i);

        subtask_c_array[i] = strdup(cJSON_GetStringValue(subtask_element));
    }

    return subtask_c_array;
}

char **categoriesList(int index)
{

    cJSON *root = cJSON_Parse(readFile());
    cJSON *tasks_array = cJSON_GetObjectItem(root, "categories");
    cJSON *subtasks_array = cJSON_GetArrayItem(tasks_array, index);
    int subtask_size = cJSON_GetArraySize(subtasks_array);
    char **subtask_c_array = malloc(subtask_size * sizeof(char *));
    for (int i = 0; i < subtask_size; i++)
    {
        cJSON *subtask_element = cJSON_GetArrayItem(subtasks_array, i);

        subtask_c_array[i] = strdup(cJSON_GetStringValue(subtask_element));
    }

    return subtask_c_array;
}

int categoriesSize(int index)
{

    cJSON *root = cJSON_Parse(readFile());
    cJSON *tasks_array = cJSON_GetObjectItem(root, "categories");
    cJSON *subtasks_array = cJSON_GetArrayItem(tasks_array, index);
    int subtask_size = cJSON_GetArraySize(subtasks_array);
    return subtask_size;
}

void taskAdder(char *task)
{

    cJSON *root = cJSON_Parse(readFile());
    cJSON *array_task = cJSON_GetObjectItem(root, "tasks");
    cJSON *array_situation = cJSON_GetObjectItem(root, "sit");
    cJSON_AddItemToArray(array_task, cJSON_CreateString(task));
    cJSON_AddItemToArray(array_situation, cJSON_CreateString("o"));
    char *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    free(updated_json_string);
}

void deadlineAddJSON(char task[])
{

    cJSON *root = cJSON_Parse(readFile());
    cJSON *array_task = cJSON_GetObjectItem(root, "deadline");
    cJSON_AddItemToArray(array_task, cJSON_CreateString(task));
    char *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    free(updated_json_string);
}

void descriptionAdd(char *description)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *array_description = cJSON_GetObjectItem(root, "description");
    cJSON_AddItemToArray(array_description, cJSON_CreateString(description));
    char *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    free(updated_json_string);
}

void modalDeadlineAddReplace(int highlighted)
{
    WINDOW *win = newwin(18, 46, 9, 23);
    box(win, 0, 0);
    curs_set(1);
    char task[50] = "";
    int ch, i = 0;
    mvwprintw(win, 1, 1, "Enter your deadline yy/mm/dd: ");
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
        else if (((ch >= '0' && ch <= '9') || ch == '/') && i < 10)
        {
            task[i] = ch;
            i++;
            task[i] = '\0';
        }
        mvwprintw(win, 2, 1, "%s", task);
        wrefresh(win);
    }
    if (deadlineVaild(task))
    {
        cJSON *root = cJSON_Parse(readFile());
        cJSON *array_task = cJSON_GetObjectItem(root, "deadline");
        cJSON_ReplaceItemInArray(array_task, highlighted, cJSON_CreateString(task));
        char *updated_json_string = cJSON_Print(root);
        writeFile("task.json", updated_json_string);
        free(updated_json_string);
    }
    else
    {
        return modalDeadlineAddReplace(highlighted);
    }
}

int deadlineVaild(char *str)
{
    int i = 0;
    int countSlash = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '/')
        {
            countSlash += 1;
        }
        i++;
    }
    if (countSlash == 2)
    {

        char *token;
        int year, month, day;
        token = strtok(str, "/");
        if (token != NULL)
        {
            year = atoi(token);

            token = strtok(NULL, "/");
            if (token != NULL)
            {
                month = atoi(token);
                token = strtok(NULL, "/");
                if (token != NULL)
                {
                    day = atoi(token);

                    if ((month < 12 && month > 0) && (day < 30 && day > 0) && year > 0)
                    {
                        return 1;
                    }
                }
            }
        }

        return 0;
    }
}
void modalDeadlineAdd()
{
    WINDOW *win = newwin(18, 46, 9, 23);
    box(win, 0, 0);
    curs_set(1);
    char task[100] = "";
    char temp[100];
    int ch, i = 0;
    mvwprintw(win, 1, 1, "Enter your deadline yy/mm/dd: ");
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
        else if (((ch >= '0' && ch <= '9') || ch == '/') && i < 10)
        {
            task[i] = ch;
            i++;
            task[i] = '\0';
        }
        mvwprintw(win, 2, 1, "%s", task);
        wrefresh(win);
    }

    strcpy(temp, task);
    if (deadlineVaild(temp))
    {
        cJSON *root = cJSON_Parse(readFile());
        cJSON *array_task = cJSON_GetObjectItem(root, "deadline");
        cJSON_AddItemToArray(array_task, cJSON_CreateString(task));
        char *updated_json_string = cJSON_Print(root);
        writeFile("task.json", updated_json_string);
        free(updated_json_string);
    }
    else
    {
        return modalDeadlineAdd();
    }
}

void modalDescriptionHandler()
{
    WINDOW *win = newwin(18, 46, 9, 23);
    box(win, 0, 0);
    curs_set(1);
    char task[50] = "";
    int ch, i = 0;
    mvwprintw(win, 1, 1, "Enter your description: ");
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
    descriptionAdd(task);
    // mvwprintw(win, 6, 1, "Press any key to exit...");
    wrefresh(win);
    // wgetch(win);
    wclear(win);
    wrefresh(win);
    delwin(win);
}

void modalHandler()
{
    WINDOW *win = newwin(18, 46, 9, 23);
    box(win, 0, 0);
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
    taskAdder(task);
    wclear(win);
    wrefresh(win);
    delwin(win);
}

void subtaskAdder(char subtask[], int index)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *array_subtask = cJSON_GetObjectItem(root, "subtasks");
    cJSON *array_subsituation = cJSON_GetObjectItem(root, "subtasksSit");
    cJSON *array_task = cJSON_GetArrayItem(array_subtask, index);
    cJSON *array_situation = cJSON_GetArrayItem(array_subsituation, index);
    cJSON_AddItemToArray(array_task, cJSON_CreateString(subtask));
    cJSON_AddItemToArray(array_situation, cJSON_CreateString("o"));
    char *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    free(updated_json_string);
}

void categoryAdder(char category[], int index)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *array_subtask = cJSON_GetObjectItem(root, "categories");
    cJSON *array_task = cJSON_GetArrayItem(array_subtask, index);
    cJSON_AddItemToArray(array_task, cJSON_CreateString(category));
    char *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    free(updated_json_string);
}

void modalHandlerSubtask(int index)
{

    WINDOW *win = newwin(18, 46, 9, 23);
    box(win, 0, 0);
    curs_set(1);
    char task[50] = "";
    int ch, i = 0;
    mvwprintw(win, 1, 1, "Enter your subtask: ");
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
    subtaskAdder(task, index);
    mvwprintw(win, 6, 1, "Press any key to exit...");
    wrefresh(win);
    wgetch(win);
    wclear(win);
    wrefresh(win);
    delwin(win);
}

void modalHandlerCategory(int index)
{
    WINDOW *win = newwin(18, 46, 9, 23);
    box(win, 0, 0);
    curs_set(1);
    char task[50] = "";
    int ch, i = 0;
    mvwprintw(win, 1, 1, "Enter your category: ");
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
    categoryAdder(task, index);
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
    cJSON *desctiption_array = cJSON_GetObjectItem(root, "description");
    cJSON *deadline_array = cJSON_GetObjectItem(root, "deadline");
    cJSON *subtasks_array = cJSON_GetObjectItem(root, "subtasks");
    cJSON *subtasksSit_array = cJSON_GetObjectItem(root, "subtasksSit");
    cJSON *categories_array = cJSON_GetObjectItem(root, "categories");
    cJSON_DeleteItemFromArray(tasks_array, index_of_task);
    cJSON_DeleteItemFromArray(situation_array, index_of_task);
    cJSON_DeleteItemFromArray(desctiption_array, index_of_task);
    cJSON_DeleteItemFromArray(deadline_array, index_of_task);
    cJSON_DeleteItemFromArray(subtasksSit_array, index_of_task);
    cJSON_DeleteItemFromArray(subtasks_array, index_of_task);
    cJSON_DeleteItemFromArray(categories_array, index_of_task);
    cJSON *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    werase(win);
    box(win, 0, 0);
}

void editJSONDescription(int index, char *new_text)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *task_array = cJSON_GetObjectItem(root, "description");
    cJSON *edited_item = cJSON_CreateString(new_text);
    cJSON_ReplaceItemInArray(task_array, index, edited_item);
    cJSON *updated_json = cJSON_Print(root);
    writeFile("task.json", updated_json);
    free(updated_json);
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

void editHandlerDescription(int index, char *task_text)
{
    keypad(stdscr, TRUE);
    WINDOW *win = newwin(18, 46, 9, 23);
    box(win, 0, 0);
    wrefresh(win);
    curs_set(1);

    int cursor_pos = strlen(task_text);
    wprintw(win, "Edit your description");
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
    editJSONDescription(index, task_text);
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

void change_situation(int index, char *current_situation)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *sit_array = cJSON_GetObjectItem(root, "sit");

    if (strcmp(current_situation, "x") == 0)
    {

        cJSON_ReplaceItemInArray(sit_array, index, cJSON_CreateString("o"));
    }
    else
    {
        cJSON_ReplaceItemInArray(sit_array, index, cJSON_CreateString("x"));
    }

    cJSON *updated_json = cJSON_Print(root);
    writeFile("task.json", updated_json);
    free(updated_json);
}

void changeSituationSubtask(int index, char *current_situation, int task_index)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *subtask_sit_array = cJSON_GetObjectItem(root, "subtasksSit");
    cJSON *subtask_array = cJSON_GetArrayItem(subtask_sit_array, task_index);

    if (strcmp(current_situation, "x") == 0)
    {

        cJSON_ReplaceItemInArray(subtask_array, index, cJSON_CreateString("o"));
    }
    else
    {
        cJSON_ReplaceItemInArray(subtask_array, index, cJSON_CreateString("x"));
    }

    cJSON *updated_json = cJSON_Print(root);
    writeFile("task.json", updated_json);
    free(updated_json);
}

void delete_subtask(int index, int highlighted, WINDOW *win)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *subtasks_array = cJSON_GetObjectItem(root, "subtasks");
    cJSON *subtasksSit_array = cJSON_GetObjectItem(root, "subtasksSit");
    cJSON *subtasks_item_array = cJSON_GetArrayItem(subtasks_array, index);
    cJSON *subtasksSit_item_array = cJSON_GetArrayItem(subtasksSit_array, index);
    cJSON_DeleteItemFromArray(subtasks_item_array, highlighted);
    cJSON_DeleteItemFromArray(subtasksSit_item_array, highlighted);
    cJSON *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    werase(win);
    box(win, 0, 0);
}

void delete_category(int index, int highlighted, WINDOW *win)
{
    cJSON *root = cJSON_Parse(readFile());
    cJSON *subtasks_array = cJSON_GetObjectItem(root, "categories");
    cJSON *subtasks_item_array = cJSON_GetArrayItem(subtasks_array, index);
    cJSON_DeleteItemFromArray(subtasks_item_array, highlighted);
    cJSON *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    werase(win);
    box(win, 0, 0);
}

void subtasksHandler(WINDOW *win, int index)
{
    int highlighted = 0;

    int choice;
    while (1)

    {
        char **subtasks_list = subtaskList(index);
        char **sit_list_subtasks = situationkListSubtask(index);
        int size_subtask = sizeSubtask(index);
        werase(win);

        for (int i = 0; i < size_subtask; i++)
        {
            if (i == highlighted)
            {
                wattron(win, A_REVERSE);
            }

            mvwprintw(win, i + 1, 1, "%d", i + 1);
            mvwprintw(win, i + 1, 2, ".[");
            mvwprintw(win, i + 1, 4, sit_list_subtasks[i]);
            mvwprintw(win, i + 1, 5, "]");
            mvwprintw(win, i + 1, 6, subtasks_list[i]);

            wattroff(win, A_REVERSE);
        }

        box(win, 0, 0);
        mvwprintw(win, 0, 0, "subtasks");
        wrefresh(win);

        choice = wgetch(win);
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
            if (highlighted > size_subtask - 1)
            {
                highlighted = size_subtask - 1;
            }

            break;
        case 'd':
            delete_subtask(index, highlighted, win);
            werase(win);
            box(win, 0, 0);
            wrefresh(win);

            highlighted--;
            if (highlighted == -1)
            {
                highlighted = 0;
            }

            wrefresh(win);
            break;
        case 'a':
            modalHandlerSubtask(index);
            curs_set(0);
            box(win, 0, 0);
            break;

        case ' ':
            changeSituationSubtask(highlighted, sit_list_subtasks[highlighted], index);
            break;
        default:
            break;
        }

        if (choice == 'h')
        {
            break;
        }
    }
}

void categoriesHandler(WINDOW *win, int index)
{
    int highlighted = 0;

    int choice;
    while (1)

    {
        char **categories = categoriesList(index);
        int categories_size = categoriesSize(index);
        werase(win);

        for (int i = 0; i < categories_size; i++)
        {
            if (i == highlighted)
            {
                wattron(win, A_REVERSE);
            }

            mvwprintw(win, i + 1, 1, categories[i]);

            wattroff(win, A_REVERSE);
        }

        box(win, 0, 0);
        mvwprintw(win, 0, 0, "categories");
        wrefresh(win);

        choice = wgetch(win);
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
            if (highlighted > categories_size - 1)
            {
                highlighted = categories_size - 1;
            }

            break;
        case 'd':
            delete_category(index, highlighted, win);
            werase(win);
            box(win, 0, 0);
            wrefresh(win);

            highlighted--;
            if (highlighted == -1)
            {
                highlighted = 0;
            }

            wrefresh(win);
            break;
        case 'a':
            modalHandlerCategory(index);
            curs_set(0);
            box(win, 0, 0);
            break;

        default:
            break;
        }

        if (choice == 'c')
        {
            break;
        }
    }
}

void initialTask(int highlighted)
{

    cJSON *root = cJSON_Parse(readFile());
    cJSON *array_subtask = cJSON_GetObjectItem(root, "subtasks");
    cJSON *array_categories = cJSON_GetObjectItem(root, "subtasksSit");
    cJSON *array_situation = cJSON_GetObjectItem(root, "categories");
    cJSON *empty_array = cJSON_CreateArray();
    cJSON_AddItemToArray(array_subtask, empty_array);
    cJSON_AddItemToArray(array_categories, empty_array);
    cJSON_AddItemToArray(array_situation, empty_array);
    char *updated_json_string = cJSON_Print(root);
    writeFile("task.json", updated_json_string);
    free(updated_json_string);
}

int main()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    WINDOW *tasks = newwin(36, 92, 0, 0);
    WINDOW *description = newwin(27, 92, 0, 92);
    WINDOW *category = newwin(9, 92, 27, 92);
    WINDOW *subtasks = newwin(9, 92, 36, 0);
    WINDOW *deadline = newwin(9, 92, 36, 92);
    box(tasks, 0, 0);
    box(description, 0, 0);
    box(category, 0, 0);
    box(deadline, 0, 0);
    box(subtasks, 0, 0);
    mvwprintw(tasks, 0, 0, "tasks");
    mvwprintw(description, 0, 0, "description");
    mvwprintw(category, 0, 0, "categories");
    mvwprintw(subtasks, 0, 0, "subtasks");
    mvwprintw(deadline, 0, 0, "deadline");
    wrefresh(tasks);
    wrefresh(description);
    wrefresh(category);
    wrefresh(deadline);
    wrefresh(subtasks);

    char **task_list;
    char **sitList;
    char **subtasks_list;
    char **sit_list_subtasks;
    char **categories;
    char *desript;
    char *deadlinestr;
    int size_task;
    int size_subtask;
    int size_categories;
    int choice;
    int highlighted = 0;
    keypad(tasks, true);
    keypad(subtasks, true);
    keypad(category, true);
    if (sizeTask() == 0)
    {
        mvwprintw(tasks, 2, 2, "there is no task please enter a task first");
        wrefresh(tasks);
        modalHandler();
        modalDescriptionHandler();
        modalDeadlineAdd();
        initialTask(highlighted);
        werase(tasks);
        curs_set(0);
        box(tasks, 0, 0);
    }

    while (1)
    {

        task_list = taskList(tasks);
        sitList = situationkList(tasks);
        size_task = sizeTask();
        subtasks_list = subtaskList(highlighted);
        sit_list_subtasks = situationkListSubtask(highlighted);
        size_subtask = sizeSubtask(highlighted);
        categories = categoriesList(highlighted);
        size_categories = categoriesSize(highlighted);
        wrefresh(tasks);
        curs_set(0);

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

        werase(subtasks);
        for (int i = 0; i < size_subtask; i++)
        {

            mvwprintw(subtasks, i + 1, 1, "%d", i + 1);
            mvwprintw(subtasks, i + 1, 2, ".[");
            mvwprintw(subtasks, i + 1, 4, sit_list_subtasks[i]);
            mvwprintw(subtasks, i + 1, 5, "]");
            mvwprintw(subtasks, i + 1, 6, subtasks_list[i]);
        }
        werase(category);
        for (int i = 0; i < size_categories; i++)
        {

            mvwprintw(category, i + 1, 1, categories[i]);
        }

        werase(description);
        werase(deadline);
        desript = desription_json(highlighted);
        deadlinestr = deadline_json(highlighted);
        mvwprintw(description, 1, 1, desript);
        mvwprintw(deadline, 1, 1, deadlinestr);

        box(tasks, 0, 0);
        box(description, 0, 0);
        box(category, 0, 0);
        box(deadline, 0, 0);
        box(subtasks, 0, 0);
        mvwprintw(tasks, 0, 0, "tasks");
        mvwprintw(description, 0, 0, "description");
        mvwprintw(category, 0, 0, "categories");
        mvwprintw(subtasks, 0, 0, "subtasks");
        mvwprintw(deadline, 0, 0, "deadline");
        wrefresh(tasks);
        wrefresh(description);
        wrefresh(category);
        wrefresh(deadline);
        wrefresh(subtasks);

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
            modalHandler();
            modalDescriptionHandler();
            modalDeadlineAdd();
            initialTask(highlighted);
            curs_set(0);
            box(tasks, 0, 0);

            break;
        case 'n':
            modalDeadlineAddReplace(highlighted);
            break;

        case 'd':
            delete_task(highlighted, tasks);
            wrefresh(tasks);
            highlighted--;

            if (highlighted == -1)
            {
                highlighted = 0;
            }

            if (sizeTask() == 0)
            {

                mvwprintw(tasks, 2, 2, "there is no task please enter a task first");
                wrefresh(tasks);
                modalHandler();
                modalDescriptionHandler();
                modalDeadlineAdd();
                initialTask(highlighted);
                werase(tasks);
                curs_set(0);
                box(tasks, 0, 0);
            }
            wrefresh(tasks);

            break;

        case 'e':
            editHandler(highlighted, task_list[highlighted]);
            werase(tasks);
            box(tasks, 0, 0);
            curs_set(0);

            break;

        case ' ':
            change_situation(highlighted, sitList[highlighted]);
            werase(tasks);
            box(tasks, 0, 0);

            break;

        case 'r':
            char *temp = desription_json(highlighted);
            editHandlerDescription(highlighted, temp);
            curs_set(0);
            werase(description);
            box(description, 0, 0);
            werase(tasks);
            box(tasks, 0, 0);

            break;

        case 'h':
            subtasksHandler(subtasks, highlighted);

            break;
        case 'c':
            categoriesHandler(category, highlighted);
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
