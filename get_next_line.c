#include "get_next_line.h"

char	*ft_strchr(char *s, int c) // przeszukuje ciag znakow az do pierwszego wystapienia znaku nowej linii i zwraca wskaznik na to miejsce
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

size_t	ft_strlen(const char *s) // zwraca dlugosc ciagow znakow
{
	size_t	i = 0;
	
	while (s[i])
		i++;
	return (i);
}

void	ft_strcpy(char *dst, const char *src) // kopiuje znaki z src do dest
{
	while (*src)	
		*dst++ = *src++;
	*dst = '\0';
}

char	*ft_strdup(const char *src) // alokuje pamiec dla src i kopiuje ten ciag do tej pamieci
{
	size_t	len = ft_strlen(src) + 1;
	char	*dst = malloc(len);
	
	if (dst == NULL)
		return (NULL);
	ft_strcpy(dst, src);
	return (dst);
}

char	*ft_strjoin(char *s1, char const *s2) // laczy dwa ciagi znakow jeden nowy ciag
{
	size_t	s1_len = ft_strlen(s1);
	size_t	s2_len = ft_strlen(s2);
	char	*join = malloc((s1_len + s2_len + 1));

	if (!s1 || !s2)
		return (NULL);
	if (!join)
		return (NULL);
	ft_strcpy(join, s1);
	ft_strcpy((join + s1_len), s2);
	free(s1);
	return (join);
}

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*line;
	char		*newline;
	int			countread;
	int			to_copy;

	line = ft_strdup(buf); // robie kopie bufora
	while (!(newline = ft_strchr(line, '\n')) && (countread = read(fd, buf, BUFFER_SIZE))) //petla wykonuje sie dopoki nie znajdzie znaku nowej linii w "line" i  dopoki czytanie z pliku zwraca wiecej niz 0 bajtow; countread zwraca liczbe przeczytanych bajtow
	{
		buf[countread] = '\0'; // konczy ciag znakow buforze
		line = ft_strjoin(line, buf); // dolacza zawartosc bufora do line
	}
	if (ft_strlen(line) == 0) // Jeśli line jest pusty, zwalniamy pamięć i zwracamy NULL
		return (free(line), NULL);

	if (newline != NULL) // ...jesli znalezlismy znak nowej linii...
	{
		to_copy = newline - line + 1; // ... to obliczamy długość do skopiowania, włącznie z nową linią
		ft_strcpy(buf, newline + 1); // kopiujemy pozostałą część bufora po nowej linii
	}
	else // ...a jesli nie znalelismy znaku nowej linii...
	{
		to_copy = ft_strlen(line); // ... to kopiujemy cala dlugosc 'line'
		buf[0] = '\0'; // oprozniamy bufor
	}
	line[to_copy] = '\0'; // konczymy ciag 'line' na odpowiedniej pozycji
	return (line); // zwracam wskaznik do linii zawierajacej pzeczytane dane, wlacznie z nowa linia jesli istnieje
}

int main(void)
{
	char	*line;
	int		fd;

	fd = open("test.txt", O_RDONLY);

	while((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}	
	return(0);
}
