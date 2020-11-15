#include "text/text.h"

int main (int argc, char *argv[])
{
    char file_name[MAX_NAME] = "";

    Text text   = {0};
    int retcode =  0;

    if (argc >= 2)
        retcode = read_text (&text, argv[1]);
    else
    {
        fprintf (stderr, "Usage: %s <inputfile>\n", argv[0]);    
        return 1;
    }

    if (retcode == BAD_FILE)
    {
        perror (argv[0]);
        return 1;
    }
    else if (retcode)
    {
        fprintf (stderr, "%s: Can't read text from file ((read_text returned %d)\n", argv[0], retcode);
        return 2;
    }


#ifdef RU 
    retcode = format_text (&text, is_letter_ru);

    if (retcode)
    {
        fprintf (stderr, "%s: Can't format text %s (format_text returned %d)\n", argv[0], text.name, retcode);
        return 3;
    }

    sort_lines (text.lines, text.lines_amt, lines_cmp, is_letter_ru, letters_cmp_ru);
    
    strcpy (file_name, "results/");
    strcat (file_name, text.name);
    strcat (file_name, "_sorted_by_alphabet");
    write_text (text, file_name);    

    sort_lines (text.lines, text.lines_amt, rev_lines_cmp, is_letter_ru, letters_cmp_ru);

    strcpy (file_name, "results/");
    strcat (file_name, text.name);
    strcat (file_name, "_sorted_by_rhyme");
    write_text (text, file_name);    
#endif 

#ifdef EN
    retcode = format_text (&text, is_letter_en);

    if (retcode)
    {
        fprintf (stderr, "%s: Can't format text %s (format_text returned %d)\n", argv[0], text.name, retcode);
        return 3;
    }

    sort_lines (text.lines, text.lines_amt, lines_cmp, is_letter_en, letters_cmp_en);
    
    strcpy (file_name, "results/");
    strcat (file_name, text.name);
    strcat (file_name, "_sorted_by_alphabet");
    write_text (text, file_name);    

    sort_lines (text.lines, text.lines_amt, rev_lines_cmp, is_letter_en, letters_cmp_en);

    strcpy (file_name, "results/");
    strcat (file_name, text.name);
    strcat (file_name, "_sorted_by_rhyme");
    write_text (text, file_name);    
#endif

    free_text (&text);

    return 0;
}
