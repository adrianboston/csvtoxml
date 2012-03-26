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

#ifndef CSVTOXML_H__
#define CSVTOXML_H__

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

//this will allow isspace, isalpha, etc
//#include <cctype>  

/**
HELP string, printed to console if -h in the arguments
*/
#define HELP \
			"Usage: " << argv[0] << " [-i file] [-o file] [-c chars] [-d chars] [-l header lines] [-x xsl]  \n\n" \
			"Where options include:\n" \
			"\t-i set an input filename.\n" \
			"\t-o set an output filename.\n" \
			"\t-c set an array of delimiters ( '\\w' is space). Array treated as individual characters not a string.\n" \
			"\t-d set an array of characters to ignore/delete while parsing. Chars will not be present in output.\n" \
			"\t-l set the number of lines to place into header tag.\n" \
			"\t-x set a xsl stylesheet and sets a standard iso <?xml> tag.\n\n" \
			"\t-h show this help.\n" \
			"Function:\n" \
			"\tThis program converts a single comma-seperated-value (CSV) stream into an xml tagged stream using stdin or file stream as input.\n" \
			"\tThe resulting xml stream is sent to stdout, an output file may be specified.\n" \
			"\tComma is the default seperator between values, given its popularity in corporate data communication.\n" \
			"\tEnd of line, \\n, is always the indicator for a new line of data.\n" \
			"\tThe xml tags are incremented <a>, <b>, ,,, <z>.\n" \
			"\tTags are recycled at z, additional characters are double tags: aa,bb, aaa, etc.\n" \
			"\tThis version has no facility for converting some other token (whitespace, period, ?) as xml attributes\n\n" \
			"Eg:\n\t 1. Convert a csv input file and pipe it into output file\n" \
			"\t" << argv[0] << " -i /var/datafile.csv > resultingfile.xml\n" \
			"\t 2. Pipe the contents into executable and pipe that into output file\n" \
			"\tmore /var/datafile.csv | " << argv[0] << " > resultingfile.xml\n" \
			"\t 3. Pipe the contents into executable and write to output file\n" \
			"\tmore /var/datafile.csv | " << argv[0] << " -o resultingfile.xml\n" \
			"AB 2007\n";

/**
	This is a check for proper usage in Win 32 compilations
	Win cannot 'pipe' from cat, more, pr. and must use file streams requiring at least one (extra) argument
*/
#define WIN_USAGE \
    if (argc < 2) { \
        cerr << "Usage: " << argv[0] << " [-options] -i path of an input csv file. -help for help\n"; \
        return(-1); \
    }

//#define ESC_TAB    0x09
#define ESC_TAB   '\t'

//#define ESC_SPACE  0x20
#define ESC_SPACE  ' '

//#define ESC_CR     0x0d
#define ESC_CR     '\r'

//#define ESC_LF     0x0a
#define ESC_LF     '\n'

#define ESC_COMMA  0x2c

#define ESC_QUOTE  0x22

#define ESC_NULL   0x00

//This will produce an precompile and compile WARNING. "unknown escape sequence \w."
//Needed in console mode
#define ESC_SPACE_CONSOLE '\w'

#define DEFAULT_DELIMITER ','

#define DEFAULT_IGNORE '\0'

/*
Default enclosing tags.
*/

#define OPEN_ROOT_TAG "<data>"

#define CLOSE_ROOT_TAG "</data>"

#define OPEN_HEADER_TAG "<hd>"

#define CLOSE_HEADER_TAG "</hd>"

#define DEFAULT_ROW_TAG "row"


int csv_to_xml_file(const int head, const char * delimiters, const char * ignore, const char * tag, const char * xsl, const char * filename, const char * out_filename);
int csv_to_xml_stream(const int head, const char * delimiters, const char * ignore,  const char * tag, const char * xsl, std::istream&  in, std::ostream&  out);
char * multi_char(const char * key, const int num);
int string_token(char **to, const char *from, const char *delim);

#endif