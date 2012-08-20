#include <string.h>
void split(const char* full, char* path, char* file, char spliter);
char* make_name(char* new_name, char* path, char* prefix, char* file);

void split(const char* full, char* path, char* file, char spliter){
	char* mark = strrchr(full, spliter);
	if(!mark){
		printf("USAGE: `path-to-file/file-name` !\n");
		exit(1);
	}
	int pos = (int)(mark - full);
	strncpy(path, full, ++pos);
	path[pos] = '\0';
	strcpy(file,++mark);
}

char* make_name(char* new_name, char* path, char* prefix, char* file){
	new_name[0] = '\0';
	strcat(new_name, path);
	strcat(new_name, prefix);
	strcat(new_name, file);
}
