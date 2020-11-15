/*!
    \file
    \brief Header file with description of functions and structures
    \author SaintAleksei
    \date November 2020

    This file contains:
        Some helpful defines;
        The definition of structures Text (_text_) and Line (_line_);
        The definition of all functions;
*/

#ifndef _TEXT_LIBRARY_
#define _TEXT_LIBRARY_ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wctype.h>
#include <wchar.h>

//*********************************************************************************************************************

#define PIECE_OF_SHIT 160 

#define MAX_NAME 64      
#define INIT 7777777    

//********************************************************************************************************************

//! Error codes

enum ERRORS
{
    BAD_FILE        = 1,
    BAD_ENCODING    = 2,
    BAD_POINTER     = 3,
    BAD_NAME        = 4,
    MEMORY_TROUBLE  = 5,
    READING_TROUBLE = 6,
    LOGIC_TROUBLE   = 7, 
};

//*********************************************************************************************************************

//! Struct that store wide character line and its length

typedef struct _line_ 
{
    wchar_t *str;
    size_t len;
} Line;

//! Struct that store text and its info

typedef struct _text_
{
    wchar_t *data;
    size_t chars_amt;
    Line *lines;
    size_t lines_amt;
    char name[MAX_NAME];
    size_t size;
    int status;
} Text;

//! The type of functions that defines letter

typedef int (*is_letter_t)   (wchar_t);

//! The type of functions that compares letters

typedef int (*letters_cmp_t) (wchar_t, wchar_t);

//! The type of functions that compares lines

typedef int (*lines_cmp_t)   (Line, Line, is_letter_t, letters_cmp_t);

//*********************************************************************************************************************

/*!
    This function is responsible for reading text from a file into memory
    \param[in] file_name Input file name
    \param[in,out] text_pointer A pointer to a variable that will hold the text
    \return Zero on success and error code otherwise
*/
int  read_text (Text *text_pointer, const char *file_name);

/*! 
    This function is responsible for writing text into a file 
    \param[in] file_name Input file name
    \param[in] text The text to be written
    \return Zero on success and error code otherwise
*/
int  write_text (Text text, const char *file_name);

/*!
    This function is responsible for text formatting
    \param[in,out] text_pointer A pointer to a Text variable that will be formatted 
    \param[in] is_letter_t Function that defines a letter in a text
    \return Zero on success and error code otherwise
*/
int  format_text (Text *text_pointer, is_letter_t);

/*!
    This function is responsible for freeing text
    \param[in,out] text_pointer A pointer to a Text variable that will be freed
*/
void free_text (Text *text_pointer);

/*!
    This function is responsible for sorting lines
    \param[in,out] lines A pointer to lines array that will be sorted
    \param[in] lines_amt Amount of lines in array
    \param[in] lines_cmp_t Function that compare two lines
    \param[in] is_letter_t Function that defines a letter in a line
    \param[in] letters_cmp_t Function that compare two letterts
*/
void sort_lines (Line *lines, size_t lines_amt, lines_cmp_t, is_letter_t, letters_cmp_t); 

/*!
    This function is responsible for comparsion lines in lexicographical order ignoring all characters except letters 
    \param[in] str1 First line
    \param[in] str2 Second line
    \param[in] is_letter_t Function that defines a letter in a line
    \param[in] letters_cmp_t Function that compare two letterts
    \return Positive integer if str1 "more" than str2, negative integer otherwise and zero if they are the same
*/
int  lines_cmp (Line str1, Line str2, is_letter_t, letters_cmp_t);

/*!
    This function is responsible for comparsion lines in reverse lexicographical order ignoring all characters except letters
    \param[in] str1 First line
    \param[in] str2 Second line
    \param[in] is_letter_t Function that defines a letter in a line
    \param[in] letters_cmp_t Function that compare two letterts
    \return Positive integer if str1 "more" than str2, negative integer otherwise and zero if they are the same
*/
int  rev_lines_cmp (Line str1, Line str2, is_letter_t, letters_cmp_t);

/*!
    This function is compare two englsih letters
    \param[in] c1 First letter in unicode
    \param[in] c2 Second letter in unicode 
    \return Positive integer if c1 "more" than c2, negative integer otherwise and zero if they are the same
    \warning if c1 or c2 aren't english letters behavior undefined
*/
int  letters_cmp_en (wchar_t c1, wchar_t c2);

/*!
    This function is compare two russian letters
    \param[in] c1 First letter in unicode
    \param[in] c2 Second letter in unicode 
    \return Positive integer if c1 "more" than c2, negative integer otherwise and zero if they are the same
    \warning if c1 or c2 aren't russian letters behavior undefined
*/
int  letters_cmp_ru (wchar_t c1, wchar_t c2);

/*!
    This function defines russian letter
    \param[in] c Any symbol in unicode
    \return 1 if c is russian letter and zero otherwise
*/
int  is_letter_ru (wchar_t c);

/*!
    This function defines english letter
    \param[in] c Any symbol in unicode
    \return 1 if c is english letter and zero otherwise
*/
int  is_letter_en (wchar_t c);

#endif 
