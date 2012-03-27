<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">


<xsl:template match="/">
<html>
<body>

<h2>CBOE Quote Data</h2>
<table border="1">
<!-- HEADER -->
<tr>
<th>
<xsl:value-of select="data/hd/row2/calls"/>
</th><th>
<xsl:value-of select="data/hd/row2/last"/>
</th><th>
<xsl:value-of select="data/hd/row2/ch"/>
</th><th>
<xsl:value-of select="data/hd/row2/bid"/>
</th><th>
<xsl:value-of select="data/hd/row2/ask"/>
</th><th>
<xsl:value-of select="data/hd/row2/vol"/>
</th><th>
<xsl:value-of select="data/hd/row2/oi"/>
</th>

<th>
<xsl:value-of select="data/hd/row2/puts"/>
</th><th>
<xsl:value-of select="data/hd/row2/last"/>
</th><th>
<xsl:value-of select="data/hd/row2/ch"/>
</th><th>
<xsl:value-of select="data/hd/row2/bid"/>
</th><th>
<xsl:value-of select="data/hd/row2/ask"/>
</th><th>
<xsl:value-of select="data/hd/row2/vol"/>
</th><th>
<xsl:value-of select="data/hd/row2/oi"/>
</th>

</tr>

<!-- Data -->
<xsl:for-each select="data/row">
<tr>
<td>
<span style="font-weight:bold;color:black">
<xsl:value-of select="calls"/>
</span>
</td><td>
<xsl:value-of select="last"/>
</td><td>
<xsl:value-of select="ch"/>
</td><td>
<xsl:value-of select="bid"/>
</td><td>
<xsl:value-of select="ask"/>
</td><td>
<xsl:value-of select="vol"/>
</td><td>
<xsl:value-of select="oi"/>
</td><td>
<span style="font-weight:bold;color:black">
<xsl:value-of select="puts"/>
</span>
</td><td>
<xsl:value-of select="last"/>
</td><td>
<xsl:value-of select="ch"/>
</td><td>
<xsl:value-of select="bid"/>
</td><td>
<xsl:value-of select="ask"/>
</td><td>
<xsl:value-of select="vol"/>
</td><td>
<xsl:value-of select="oi"/>
</td>
</tr>
</xsl:for-each>

</table>
</body>
</html>
</xsl:template>

</xsl:stylesheet>
