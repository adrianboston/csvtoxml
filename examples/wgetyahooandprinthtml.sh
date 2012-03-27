#!/bin/sh

#  wgetyahooandprinthtml.sh
#  csvtoxml
#
#  Created by Adrian Boston on 12-03-26.

# Get all daily S&P data from yahoo, convert ot xml and make xml file. Add html style sheet to look at data

wget http://ichart.finance.yahoo.com/table.csv?s=%5EGSPC&d=1&e=5&f=2012&g=d&a=0&b=3&c=1950&ignore=.csv | csvtoxml -x abcdeftohml.xsl > SPX_data.xml


