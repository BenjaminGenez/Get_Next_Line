/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igenez-y <igenez-y@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:41:34 by igenez-y          #+#    #+#             */
/*   Updated: 2024/10/15 16:27:24 by igenez-y         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*s_buffer(int fd, char *read_buffer, char *backup_buffer)
{
	int		bytes_read;
	char	*temporary_holder;

	bytes_read = 1;
	while (bytes_read != 0)
	{
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (NULL);
		else if (bytes_read == 0)
			break ;
		read_buffer[bytes_read] = '\0';
		if (!backup_buffer)
			backup_buffer = ft_strdup("");
		temporary_holder = backup_buffer;
		backup_buffer = ft_strjoin(temporary_holder, read_buffer);
		free(temporary_holder);
		temporary_holder = NULL;
		if (ft_strchr(read_buffer, '\n'))
			break ;
	}
	return (backup_buffer);
}

static char	*extract(char *line)
{
	size_t	chr_count;
	char	*remaing_bf;

	chr_count = 0;
	while (line[chr_count] != '\n' && line[chr_count] != '\0')
		chr_count++;
	if (line[chr_count] == '\0' || line[1] == '\0')
		return (NULL);
	remaing_bf = ft_substr(line, chr_count + 1, ft_strlen(line) - chr_count);
	if (*remaing_bf == '\0')
	{
		free(remaing_bf);
		remaing_bf = NULL;
	}
	line[chr_count + 1] = '\0';
	return (remaing_bf);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*final_line;
	char		*read_buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	read_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!read_buffer)
		return (NULL);
	final_line = s_buffer(fd, read_buffer, buffer);
	free(read_buffer);
	read_buffer = NULL;
	if (!final_line)
		return (NULL);
	buffer = extract(final_line);
	return (final_line);
}

//this is a comment.
int	main(void)
{
	int fd = open("test.txt", O_RDONLY);
	
	if (fd == -1)
	{
		printf("error");
		return (1);
	}
	char *line;

	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return 0;

}
