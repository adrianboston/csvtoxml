<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">


<xsl:template match="/">
<html>
<body>
<h2>SAMPLE CSVTOXML TEMPLATE/h2>

<table border="1">

<xsl:for-each select="data/row">
<tr>
<td>
<xsl:value-of select="a"/>
</td><td>
<xsl:value-of select="b"/>
</td><td>
<xsl:value-of select="c"/>
</td><td>
<xsl:value-of select="d"/>
</td><td>
<xsl:value-of select="e"/>
</td><td>
<xsl:value-of select="f"/>
</td><td>
<xsl:value-of select="g"/>
</td><td>
<xsl:value-of select="e - z"/>  <!-- this is basic math function in xsl -->
</td>
</tr>
</xsl:for-each>


</table>
</body>
</html>
</xsl:template>

</xsl:stylesheet>
