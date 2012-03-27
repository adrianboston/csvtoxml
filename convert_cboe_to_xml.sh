#!/bin/sh

#  convert_cboe_to_xml.sh
#  csvtoxml
#

./csvtoxml -i RUT_QuoteData.dat -l 3 -x cboexmltohtml.xsl -t calls,last,ch,bid,ask,vol,oi,puts,last,ch,bid,ask,vol,oi