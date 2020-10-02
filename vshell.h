void vsh_print_welcome_message();
void vsh_execute();
void vsh_exit();
char* vsh_read_line();
char** vsh_extract_words();
void vsh_execute_command(char **words);
void vsh_cd(char **words);
void vsh_exit(char **words);
void vsh_help(char **words);
#define BUFFER_SIZE 128
//Initial allocation of buffer to read a line
#define TOTAL_WORDS 16
//Initial allocation for number of words to be parsed from the line

