/*!
    \file
    \brief Source file with all functions from text.h 
    \author SaintAleksei
    \date November 2020
*/

#include "text.h"

//*********************************************************************************************************************

int read_text (Text *text_pointer, const char *file_name)
{
    if (!text_pointer)
        return BAD_POINTER;
    if (!file_name)
        return BAD_POINTER;
    if (strlen (file_name) >= MAX_NAME)
        return BAD_NAME;

    FILE *stream = fopen (file_name, "r");
	size_t i = 0;
	size_t j = 0;

    if (!stream)
        return BAD_FILE;

    setlocale (LC_CTYPE, "C.UTF-8");

    Text text = {0};

    strcpy (text.name, file_name);

	fseek (stream, 0, SEEK_END);       
	text.size = ftell (stream);
	fseek (stream, 0, SEEK_SET);

    char *buf = (char *   ) calloc (text.size + 1, sizeof (char   ) ); 
    
    if (!buf)
        return MEMORY_TROUBLE;

    text.data = (wchar_t *) calloc (text.size + 1, sizeof (wchar_t) ); 
    
    if (!text.data)
    {
        free (buf);
        return MEMORY_TROUBLE;
    }

    if (fread (buf, sizeof (char), text.size, stream) != text.size)
    {
        free (buf);
        free (text.data);
        return READING_TROUBLE;
    }

    fclose (stream);

    buf[text.size] = '\0';

    if ( (text.chars_amt = mbstowcs (text.data, buf, text.size) ) == (size_t) -1)
    {
        free (buf);
        free (text.data);
        return BAD_ENCODING;
    }
 
    free (buf);

    text.data = (wchar_t *) realloc (text.data, (text.chars_amt + 1) * sizeof (wchar_t) ); 

	for (; i < text.chars_amt; i++)
	{
        if (text.data[i] == L'\r')
            text.data[i] = L' ';

		if (text.data[i] == L'\n')
        {
			text.data[i] = L'\0'; 
            j++;
        }
	}

	text.lines_amt = j;

    text.lines = (Line *) calloc (text.lines_amt + 1, sizeof (Line) );

    if (!text.lines)
    {
        free (text.data);
        return MEMORY_TROUBLE;
    }

    text.lines[0].str = text.data;
	for (i = 1, j = 1; i < text.chars_amt; i++)
	{
		if (!text.data[i-1])
		{
			text.lines[j].str = text.data  + i;		 				 
			text.lines[j].len = wcslen (text.lines[j].str);     
	
			j++;
		}
	}

    if (text.lines_amt != j)
    { 
        free (text.data);
        free (text.lines);
        return LOGIC_TROUBLE;
    }

    free_text (text_pointer);
    *text_pointer = text;
    text_pointer->status = INIT;

	return 0;
}

//*********************************************************************************************************************

int write_text (Text text, const char *file_name)
{    
    if (text.status != INIT)
        return LOGIC_TROUBLE;

    if (!file_name)
        return BAD_POINTER;

    FILE *stream = fopen (file_name, "w");

    if (!stream)
        return BAD_FILE;

    setlocale (LC_CTYPE, "C.UTF-8");

	for (size_t i = 0; i < text.lines_amt; i++)
		    fprintf (stream, "%ls\n", text.lines[i].str);

    fclose (stream);

    return 0;
} 

//*********************************************************************************************************************

int format_text (Text *text_pointer, is_letter_t is_letter)
{    
    if (!text_pointer)
        return BAD_POINTER;
    
    if (text_pointer->status != INIT)
        return LOGIC_TROUBLE;

    Text text = *text_pointer;
    size_t k  = 0;
    
    for (size_t i = 0; i < text.lines_amt; i++)
    {
        size_t count = 0;

        for (size_t j = 0; j <= text.lines[i].len; j++)
            if (is_letter (text.lines[i].str[j]) )
                count++;
        
        if (count)
            text.lines[k++] = text.lines[i];
    }

    text.lines_amt = k;
    text.lines = (Line *) realloc (text.lines, text.lines_amt * sizeof (Line) );
    
    if (!text.lines)    
        return MEMORY_TROUBLE;

    for (size_t i = 0; i < text.lines_amt; i++)
    {
        size_t j = 0;
        wchar_t c = 0;
        while ( ( (c = text.lines[i].str[j]) == L' ' || c == L'\t' || c == PIECE_OF_SHIT) && c)
        {
            text.lines[i].str[j] = L'\0';
            j++;
        }

        text.lines[i].str += j;
        text.lines[i].len  = wcslen (text.lines[i].str);
    }

    *text_pointer = text;

    return 0;
}

//*********************************************************************************************************************

void free_text (Text *text_pointer)
{   
    if (!text_pointer)
        return;

    if (text_pointer->status == INIT)
    {
        free (text_pointer->data  );
        free (text_pointer->lines );
    }

    text_pointer->data      = NULL;
    text_pointer->chars_amt = 0;
    text_pointer->lines     = NULL;
    text_pointer->lines_amt = 0;
    text_pointer->name[0]   = '\0';
    text_pointer->size      = 0;
    text_pointer->status    = 0;
}

//*********************************************************************************************************************

void sort_lines (Line *lines, size_t lines_amt, lines_cmp_t lines_cmp, is_letter_t is_letter, letters_cmp_t letters_cmp)
{	
    if (!lines || !lines_cmp || !letters_cmp || !is_letter)
        return;    

	size_t wall = 0;                               
	for (size_t i = 0; i < lines_amt - 1; i++)     
	{
		if (lines_cmp (lines[i], lines[lines_amt - 1], is_letter, letters_cmp) < 0) 		
		{
			Line buff = lines[i];
			lines[i] = lines[wall];
			lines[wall] = buff;
			wall++;
		}
    }
	
    if (1)
    {
	    Line buff = lines[lines_amt - 1];         
        lines[lines_amt - 1] = lines[wall];
		lines[wall] = buff;
	}
	

	if (wall >= 2)                          
		sort_lines (lines, wall, lines_cmp, is_letter, letters_cmp);
	if (lines_amt - wall >= 3)                 
		sort_lines (lines + wall + 1, lines_amt - wall - 1, lines_cmp, is_letter, letters_cmp);
}

//*********************************************************************************************************************

int lines_cmp (Line str1, Line str2, is_letter_t is_letter, letters_cmp_t letters_cmp)
{
	const wchar_t *s1 = str1.str;
	const wchar_t *s2 = str2.str;

    if (!s1 || !s2 || !is_letter || !letters_cmp)
        return BAD_POINTER;
    
    int i = 0;
    int j = 0;
    while (1)
	{
		while (!is_letter (s1[i]) )
			if (s1[i] == '\0')
				break;
			else
				i++;
		
		while (!is_letter (s2[j]) )
			if (s2[j] == '\0')
				break;
			else
				j++;

        int res = letters_cmp (s1[i], s2[j]);
        if (res)
            return res;             
        else if (s1[i] == '\0' || s2[i] == '\0')
            return 0;
        else
        {
            i++;
            j++;
        }
	}	
}

//*********************************************************************************************************************

int rev_lines_cmp (Line str1, Line str2, is_letter_t is_letter, letters_cmp_t letters_cmp)
{
	const wchar_t *s1 = str1.str;
	const wchar_t *s2 = str2.str;

    if (!s1 || !s2 || !is_letter || !letters_cmp)
        return BAD_POINTER;

    int i = str1.len;
    int j = str2.len;
    while (1)
	{
		while (!is_letter (s1[i]) )
			if (!i)
				break;
			else
				i--;				

		while (!is_letter (s2[j]) )
			if (!j)
				break;
			else
				j--;
        
        int res = letters_cmp (s1[i], s2[j]);
        if (res)
            return res;
        else if (!i || !j)
            return 0;
        else
        {
            i--;
            j--;
        }
	}	
}

//*********************************************************************************************************************

int is_letter_en (wchar_t c)
{
    return iswalpha (c);
}

int letters_cmp_en (wchar_t c1, wchar_t c2)
{
    c1 = towlower (c1);
    c2 = towlower (c2);

    return c2 - c1;
}

//*********************************************************************************************************************

int is_letter_ru (wchar_t c)
{
    return ( (c >= L'А' && c <= L'я') || (c == L'ё') || (c == L'Ё') ) ? 1 : 0;
}

int letters_cmp_ru (wchar_t c1, wchar_t c2)
{
    if (c1 != L'Ё' && c1 != L'ё' && c1 < L'а' && c1 >= L'А' )
        c1 += L'а' - L'А';
    else if (c1 == L'Ё')
        c1 = L'ё';    
    
    if (c2 != L'Ё' && c2 != L'ё' && c2 < L'а' && c1 >= L'А')
        c2 += L'а' - L'А';
    else if (c2 == L'Ё')
        c2 = L'ё';    
        
    if (c1 != L'ё' && c2 != L'ё')
        return c2 - c1;
    else if (c1 == L'ё' && c2 != L'ё')
    {
        if (c2 <= L'е')
            return -1;
        if (c2 >= L'ж')
            return 1; 
    } 
    else if (c1 != L'ё' && c2 == L'ё')
    {
        if (c1 <= L'е')
            return 1;
        if (c1 >= L'ж')
            return -1; 
    } 

    return 0;
}

//*********************************************************************************************************************

