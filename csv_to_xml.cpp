/*
 *  cvs_to_xml.cpp
 *  
 *
 *  Created by ptr_to_null on 1/24/07.
 *
 *  Copywrong 2007 __Null__. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
 
#include "csv_to_xml.h"

/**
	Code for closing files. repeated in catch and function
*/
#define FILE_CLOSE \
 	if (file_in != NULL) \
		f.close(); \
	if (file_in != NULL) \
		g.close(); \
		
/*
	Code for generating multikey char xml tags, whether opening, closing or empty.
	According to XML specifications for open, close and empty tags.
	OPEN_TAG will generate <aa>
	CLOSE_TAG will generate </aa>
	EMPTY_TAG will generate <aa/>
*/

#define OPEN_TAG \
	"<" << multi_char(key, key_count) << ">"

#define CLOSE_TAG \
	"</" << multi_char(key, key_count) << ">"

#define EMPTY_TAG \
	"<" << multi_char(key, key_count) << "/>"

#define XSL_SHEET \
	"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n<?xml-stylesheet type=\"text/xsl\" href=\"" << xsl << "\"?>\n"

#define TAG_CONDITION \
	y < tag_sz
//	tags != NULL && (tags[y] != NULL || tags[y] != '\0')


static char commas[]= ",";

using namespace std;

/**
* main called from commandline.
*/
int main (int argc, char * const argv[])
{																//win cannot 'pipe' from cat, more, pr.
																//thus use file streams requiring at least one extra argument. see header
	#ifdef _WIN32												
		WIN_USAGE												
	#endif

	int head = 0;
	char *xsl = NULL;
	char *file_in = NULL;
	char *file_out = NULL;
	char *delimit = NULL;
    char *ignore = NULL;
    char *tag = NULL;
	
	for (int x=1; x< argc;x++) {
	
		if (*argv[x] == '-') {									//look for args beginning with '-'
            switch (* (argv[x]+1) ) {							//examine the char following the '-' token
                case 'i':
                    file_in = argv[x+1]; x++;					//advance 2
                    break;
                case 'o':
                    file_out = argv[x+1]; x++;
                    break;
                case 'l':
                    head = atoi(argv[x+1]); x++;
                    break;
                case 'x':
                    xsl = argv[x+1]; x++;
                    break;
                case 'c':
                    delimit = argv[x+1]; x++;
                    break;
                case 'd':
                    ignore = argv[x+1]; x++;
                    break;
				case 't':
					tag = argv[x+1]; x++;
					break;
                case 'h':
					cerr << HELP;                           //print help return
                    return -1;
			}
	   }
	}
	
    if (delimit == NULL || strlen(delimit) < 1)
        delimit = new char(DEFAULT_DELIMITER);              //possible warning
		
    if (ignore == NULL || strlen(ignore) < 1)
        ignore = new char(DEFAULT_IGNORE);
		
    if (tag == NULL || strlen(tag) < 1)
        tag = new char(DEFAULT_IGNORE);		
	
	try {
		return csv_to_xml_file(head, delimit, ignore, tag, xsl, file_in, file_out);
	} catch (int e) {
		cerr << e;
		return e;
	}
    
}


/**
	csv_to_xml converts a seperated value stream into a basic xml tagged stream.
	head size of header information in number of line
	delimiters is a character array of chosen delimiters
	ignore is a character array of chars to ignore for the output stream
	tags is a character array of chosen strings as xml tags
	attrb is a character array of chosen delimiters as xml attributes
	xsl is a character array representing a the location of an xsl file used in xsl transformation
	file_in is a string containing a full path and filename
	file_out is a string containing a full path and filename
	returns success or failure
	throws int error
*/
int csv_to_xml_file(const int head, const char * delimiters, const char * ignore, const char * tag, const char * xsl, const char * file_in, const char * file_out)
{

	streambuf *bf = NULL;
	streambuf *bg = NULL;
	ifstream f;
	ofstream g;
															//set up a stream for in, whether cin or file
	if (file_in == NULL) {
		bf = cin.rdbuf();
	} else {
		f.open(file_in, ios::in);
		if (!f) {
			cerr << "Error opening input stream " << file_in << endl;
			return -1;
			}
		bf = f.rdbuf();
	}
	istream in(bf);
															//set up a stream for out, whether std::cout or file
	if (file_out == NULL) {
		bg = std::cout.rdbuf();
	} else {
		g.open(file_out, ios::out);
		if (!g) {
			cerr << "Error opening output stream " << file_out << endl;
			return -1;
		}
		bg = g.rdbuf();
	}
	ostream out(bg);
	
	int ret = 0;
	try {
		ret = csv_to_xml_stream(head, delimiters, ignore, tag, xsl, in, out);
	} catch (int e) {
		FILE_CLOSE;
		throw e;
	}
	
	FILE_CLOSE;
	return ret;
}

/**
	csv_to_xml converts a seperated value stream into a basic xml tagged stream.
	head size of header information in number of line
	delimiters is a character array of chosen delimiters
	ignore is a character array of chars to delete while parsing the stream
	xsl is a character array representing a the location of an xsl file used in xsl transformation
	in is an input stream
	out is an output stream
	returns success or failure
	throws int error
*/
int csv_to_xml_stream(const int head, const char * delimiters, const char * ignore, const char * tag, const char * xsl, istream& in, ostream& out)
{
 	
    char **tags = new char *;                               //string token will convert a char array into a 2d char array
    int tag_sz = string_token(tags, tag, commas);            //empty entries will be ignored
	
	const char *alphakey = "abcdefghijklmnopqrstuvwxyz";

    int sz = strlen(delimiters);							//memcpy delimiters into new array, converting symbolic white space ( \w ) into blank char, ' '
    char * chars = new char[sz];							//which is used later in string char compare
    for (int x = 0; x < sz; x++) {
        if (delimiters[x] == ESC_SPACE_CONSOLE)
            chars[x] =  ESC_SPACE;
		else
            chars[x] = delimiters[x];
		chars[x+1] = ESC_NULL;								//safety?
	}
    
	try {
													
		if (xsl != NULL)									//insert xsl instructions. the inclusion of xsl is an impure (yet convenient) function of this program
			out << XSL_SHEET;								//unix users can use cat in order to add the following strings into their data but the lesser one cannot easily do this

		out << OPEN_ROOT_TAG << ESC_LF;	

		if (head > 0)
			out << OPEN_HEADER_TAG << ESC_LF;

		char c;												//vars used for input, tags, and printout
		int x = 0, y = 0;                                   //x= numb lines, y = num of unique tags per line
		char last = ESC_LF;									//set last to LF, \n as signal
		char * key = (char *)alphakey;						//pointer to alpha array, set to 'a'
		int key_count = 1;
															//main read loop. in.get() is a blocking read
		while ( (c = (char)in.get()) != EOF) {            
            
			if (last == ESC_LF) {
				if (x < head)
					out << "<" << DEFAULT_ROW_TAG << x << ">";				//print unique tag for header
				else
					out << "<" << DEFAULT_ROW_TAG << ">";
				if (TAG_CONDITION)
					out << "<" << tags[y] << ">";
				else
					out << "<a>";
			}

			switch (c) {
				case ESC_LF:
					if (strchr(chars, last) != NULL) {		//if last char matches a delimiter
						if (TAG_CONDITION )
							out << "<" << tags[y] << " />";
						else
							out << EMPTY_TAG;
					} else {
						if (TAG_CONDITION)
							out << "</" << tags[y] << ">";
						else
							out << CLOSE_TAG;					//create closing tag
					}
					if (x < head)
						out << "</" << DEFAULT_ROW_TAG << x << ">\n";		//close an enclosing line for a header line
					else
						out << "</" << DEFAULT_ROW_TAG << ">\n";					//print enclosing line tag with endl

					x++;										//increment number of lines
					if (x == head)
						out << CLOSE_HEADER_TAG << ESC_LF;		//this is the end of header info, if there was one
					key = (char *)alphakey;						//reset to 'a'
					key_count = 1;								//reset to 1
					y = 0;										//reset to 0
					break;
				case ESC_CR:
					#ifndef _WIN32
						c = last;								//remove carriage return if unix, retaining the previous char
					#endif
					break;
				case ESC_TAB:									//remove tab in all cases, retain the previous 'active' char
					c = last;
					break;
				default:		//put more conditions in here
					
					if (ignore != NULL) {						//omit the characters in the ignore set
						if (strchr(ignore, c) != NULL) {
							c = last;
							break;
						}
					}                    

					
					if (strchr(chars, c) != NULL) {				//check for match between delimiter characters and current char
						if (strchr(chars, last) != NULL ) {		//check for an empty value, aka two commas together ,,
							if (TAG_CONDITION)
								out << "<" << tags[y] << "/>";
							else
								out << EMPTY_TAG;				//print empty tag
						} else {
							if (TAG_CONDITION)
								out << "</" << tags[y] << ">";
							else
								out << CLOSE_TAG;
						}										//print closing tag
						if (*key+1 == 'z')	{					//increment to next alpha char if 'z' is reached
							key = (char *)alphakey;				//reset to 'a'
							key_count++;						//and increment the count to produce 'aa'
						} else
                            key++;                              //or simply increment to next alpha key, including during descriptive tags
						y++;                                    //increment number of tags per line
						break;
					}
																//check for match between delimiter characters and last char in the stream
																//place the opening xml tag for the next value
					if (strchr(chars, last) != NULL) {
						if (TAG_CONDITION)
							out << "<" << tags[y] << ">";
						else
							out << OPEN_TAG;					//put in an opening  tag, "</a><b>"
					}
					
					out << c;                                   //print char to outstream, unless break; has been issued
			}													//end of switch

			last = c;
			#ifdef DEBUG
				out.flush();									//constant flushing after each printed character will impede performance
			#endif
		}
	
	} catch (int e)
	{
		out << CLOSE_ROOT_TAG << ESC_LF;
		throw e;
	}
	out << CLOSE_ROOT_TAG << ESC_LF;	
	return 0;
}

/**
	multi_char returns a char arry filled with a repeated single character upto num times: a, aa, ... aaaaaaaaa
	uses the efficient memset function
*/
char * multi_char(const char * key, const int num)
{
	char *result = new char[num+1];
	memset(result, *key, num);									//array will have ending '\0' already from new char +1
	return result;
}

/**
	break down a char array (char *) by a token(s) into a char array array (char **)
	essentially an extended strtok
 */
int string_token(char **to, const char *from, const char *delim)
{   
    char *temp = new char[strlen(from)];
    strcpy(temp,from);
    
    char *result=NULL;
    int x=0;
    result=strtok(temp,delim); //for example, strtok('open,close,bid,' ',')
    
    while (result!=NULL) {
        to[x] = new char[strlen(result)];
        strcpy(to[x++],result);
        result=strtok(NULL, delim);
    }
//    to[x] = '\0';	//THINK!										//add terminating character, needed?
    return x;   //returns len of number in strings. alt would be to use \0 as null terminator
}