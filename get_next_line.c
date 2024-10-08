#include "get_next_line.h"

static char *read_and_store_buffer(int file_descriptor, char *read_buffer, char *backup_buffer)
{
    int     bytes_read;      
    char    *temporary_holder; 

    bytes_read = 1;

    while (bytes_read != 0)
    {
        bytes_read = read(file_descriptor, read_buffer, BUFFER_SIZE);
        
        if (bytes_read == -1)
            return (NULL);
        
        else if (bytes_read == 0)
            break;
        
        read_buffer[bytes_read] = '\0';
        
        if (!backup_buffer)
            backup_buffer = ft_strdup("");
        
        temporary_holder = backup_buffer;
        
        backup_buffer = ft_strjoin(temporary_holder, read_buffer);
        
        free(temporary_holder);
        temporary_holder = NULL;

        if (ft_strchr(read_buffer, '\n'))
            break;
    }
    return (backup_buffer);
}

static char *extract_remaining_text(char *current_line)
{
    size_t  character_count;
    char    *remaining_backup_buffer;

    character_count = 0;

    while (current_line[character_count] != '\n' && current_line[character_count] != '\0')
        character_count++;
    
    if (current_line[character_count] == '\0' || current_line[1] == '\0')
        return (NULL);
    
    remaining_backup_buffer = ft_substr(current_line, character_count + 1, ft_strlen(current_line) - character_count);
    
    if (*remaining_backup_buffer == '\0')
    {
        free(remaining_backup_buffer);
        remaining_backup_buffer = NULL;
    }
    
    current_line[character_count + 1] = '\0';
    
    return (remaining_backup_buffer);
}

char *get_next_line(int file_descriptor)
{
    char        *final_line;     
    char        *read_buffer;    
    static char *backup_buffer;  

    if (file_descriptor < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    
    read_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
    
    if (!read_buffer)
        return (NULL);
    
    final_line = read_and_store_buffer(file_descriptor, read_buffer, backup_buffer);
    
    free(read_buffer);
    read_buffer = NULL;
    
    if (!final_line)
        return (NULL);
    
    backup_buffer = extract_remaining_text(final_line);
    
    return (final_line);
}
/*int main(void)
{
    // Step 1: Open the file to read
    int fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file.\n");
        return (1);
    }

    // Step 2: Read and print each line using get_next_line
    char *line;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line: %s", line);
        free(line); // Don't forget to free the memory allocated by get_next_line
    }

    // Step 3: Close the file
    close(fd);

    return (0);
}*/
