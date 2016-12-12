#ifndef HELPERS_H
#define HELPERS_H

#include "storage.h" // todo unneccessary coupling


/** Capitalizes letters of string
 * String is manipulated directly.
 *  \param string The string
 */
void stringtoupper (char* string);

/** Checks if string consists of digits only.
 * \param string The string.
 * \returns 'true' if string only digits, otherwise 'false'.
 */
bool isnum (char* n);

/*** Reads a string from stdin and allocates memory for it.
 * \param out A pointer to the string to allocate and write to.
 * \param maxBuffer Max string length to accept.
 * \warning Free out after usage.
 */
void readString (char** out, int maxBuffer);


// prompts

/** Prompt for character
 * Keeps prompting user for a character and until an accepted character is read.
 * \param valid A string representing valid characters.
 * \param menu A menu to show the user, has all valid characters and their meaning.
 * \param showMenu The character the user has to press to see the menu.
 * \returns the char that was read.
 */
char askForChar (char* valid, char* menu, char showMenu);

/** Prompt for string
 * Prompts user for a string and writes it to out.
 * \param question The prompt message.
 * \param out A pointer to the string to write the string to.
 * \param maxBuffer Max string length to accept.
 * \warning Free out after usage.
 */
void askForString (char* question, char** out, int maxBuffer);

/** Promps for integer
 * Keeps prompting until user inputs a correct int.
 * \param question The prompt message.
 * \returns The users answer.
 */
int askForInt (char* question);

// todo remove, use askValidInt instead
/** 
 * Prompts user for an int until a valid value is input and then sets edits the goods.
 * \param g The data. E.g. a goods to edit.
 * \param attributeName The readable name of what is being edited in the goods.
 * \param validFun The validation function.
 * \param setFun The set function. Works on the data.
 */
void askSetInt(GoodsT* g, char* attributeName, bool validFun(int), void setFun(GoodsT*, int));

/* Ask user for valid int
 * Prompts user for an int until a valid value is input and returns it.
 * \param question Question to ask user.
 * \param validFun The validation function.
 */
int askValidInt(char* question, bool validFun(int));

/** * Lists a range of goods of a storage.
 * \param s The storage.
 * \param start The start of the range.
 * \param end The end of the range.
 * \returns the last listed goods if start index is valid,
 * otherwise start index is returned.
 */
int uiListGoods(StoreT* s, int start, int end);

/** Print goods' name
 */
void goods_print_fun1 (GoodsT* g);

/** Print goods' name + count
 */
void goods_print_fun2 (GoodsT* g);

/** Ask user for an index while browsing a catalogue
 * \param s The store to browse
 * \returns -1 if browser was exited, otherwise a valid chosen index starting from 0.
 * \note If the index is supposed to be used make sure to check that -1 wasn't returned!
 */
int store_browse_goods(StoreT* s, void goods_printf_function(GoodsT*));

/** Print information about a goods
 * \param goods The goods.
 */
void goods_print(GoodsT* goods);

#endif // HELPERS_H
