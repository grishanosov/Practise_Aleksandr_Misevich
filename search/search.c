#include "search.h"

void changeToParentDir(char* string) { //decreases our string-path
    int len = 0;
    if(string == NULL) return;
    len = strlen(string);
    do {
        string [len] = '\0';
        len--;
    } while(len != 0 && string [len] != '/');
    return;
}

void append(char* string, char* extra_dir) { //increases our string-path
    strcat(string, "/");
    strcat(string, extra_dir);
    return;
}

enum SearchingResult {
  Found,
  NotFound
};

/*
мне кажется, что если заменить название string на path, то будет чуть яснее
*/
SearchingResult search(char *string, int deep, const char *file, char* answer) {
    DIR* dir;
    struct dirent *enter;
    int result;
    if(deep < 0) {
        return NotFound;
    }
    if((dir = opendir(string)) == NULL) {
	    perror("Can't open directory");
	    /*
	    это не повод обивать процесс. представьте, что эта ф-я - часть большой программы.
	    у вас нет доступа к какой-то директории ... и вы из-за этого "убиваете" всю программу, что
	    немного странно.
	    */
	    exit(1);
    }
    while((enter = readdir(dir)) != NULL) {
	    if(!strcmp(enter->d_name, file)) {
            strcpy(answer, string);
            return Found;
        }
    }
	/*
	вы дважды бежите по директории. можно в одном цикле и проверять имя файла, и рекурсивно запускаться, если
	встретили директорию.
	*/
    rewinddir(dir);
    while((enter = readdir(dir)) != NULL) {
        if(enter->d_type == 4 && strcmp(".", enter -> d_name) && strcmp("..", enter -> d_name)) {
            append(string, enter->d_name);
            result = search(string, deep - 1, file, answer);
            if(result == Found) {
                return Found;
            } else {
                changeToParentDir(string);
            }
        }
    }
    return NotFound;
}

//give directory, deep and needed file

int main(int argc, char** argv) {
    if(argc - 1 == 3) { 
        int deep = atoi(argv[2]);
        char* directory;
        char* file;
        char* answer;
        int res = 0; 
        directory = (char*)calloc(MAX_SIZE, sizeof(char));
        file = (char*)calloc(MAX_SIZE, sizeof(char));
        answer = (char*)calloc(MAX_SIZE, sizeof(char));
        strcpy(directory, argv[1]);
        strcpy(file, argv[3]);
	/*
	if (search(...) == Found)
	   print (answer)
	и не надо проверять длину строки answer и т.п.
	*/
        search(directory, deep, file, answer);
        if(strlen(answer) > 1) printf("File %s was founded in %s directory\n", file, answer);
        else printf("File %s was not founded\n", file);
        free(directory);
        free(answer);
        free(file);
    } else {
        printf("Incorect number of arguments\n");
    }
}
