<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output encoding="UTF-8" indent="yes" method="html"/>
<xsl:strip-space elements="*"/>

<xsl:variable name="list"
  select="document('index.xml')/list" />

<xsl:template match="/">
    <h1>
        Unittest report
    </h1>

    <p>
        <b>
    Total tests run <xsl:value-of select="sum(document($list/entry/@name)/testsuite/@tests)"/>
    , failures: <xsl:value-of select="sum(document($list/entry/@name)/testsuite/@failures) + sum(document($list/entry/@name)/testsuite/@errors)"/>
        </b>

    <xsl:for-each select="document($list/entry/@name)" >
        <xsl:apply-templates select="testsuite"/>
    </xsl:for-each>
    </p>
</xsl:template>

<xsl:template match="testsuite">
    <h2>
        <xsl:value-of select="@name" />
    </h2>
    <table border="1" cellSpacing="0" cellPadding="10" >
    <tr>
        <th>Tests run</th>
        <th>Tests failed</th>
        <th>Other errors</th>
    </tr>
    <tr>
        <td><xsl:value-of select="@tests"/></td>
        <td><xsl:value-of select="@failures"/></td>
        <td><xsl:value-of select="@errors"/></td>
    </tr>
    </table>
    <br/>
    <table border="1" cellSpacing="0" cellPadding="10" >
    <tr>
        <th>Tests name</th>
        <th>PASS/FAIL</th>
        <th>Failing case</th>
        <th>Reason</th>
    </tr>
    <xsl:apply-templates select="testcase"/>
    </table>
</xsl:template>

<xsl:template match="testcase">
    <xsl:choose>
        <xsl:when test="failure">
            <tr><td><font color="#FF0000"><xsl:value-of select="@name" /></font></td><xsl:apply-templates select="failure"/></tr>
        </xsl:when>
        <xsl:when test="error">
            <tr><td><font color="#FF0000"><xsl:value-of select="@name" /></font></td><xsl:apply-templates select="error"/></tr>
        </xsl:when>
        <xsl:otherwise>
            <tr><td><xsl:value-of select="@name" /></td><td><font color="#00FF00">PASS</font></td><td></td><td></td></tr>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template match="failure">
    <td>
        <b><font color="#FF0000">FAIL</font></b>
    </td>
    <td>
        <font color="#ff0000">
            <xsl:value-of select="@message" />
        </font>
    </td>
    <td>
        <font color="#ff0000">
            <xsl:value-of select="@type" />
        </font>
    </td>
</xsl:template>

<xsl:template match="error">
    <td>
        <b><font color="#FF0000">FAIL</font></b>
    </td>
    <td>
        <font color="#ff0000">
            <xsl:value-of select="@message" />
        </font>
    </td>
    <td>
        <font color="#ff0000">
            <xsl:value-of select="@type" />
        </font>
    </td>
</xsl:template>

</xsl:stylesheet>