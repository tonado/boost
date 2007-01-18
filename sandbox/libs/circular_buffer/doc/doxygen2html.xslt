<?xml version='1.0' encoding='iso-8859-1'?>
<!--
Generic XSL transformation from the XML files generated by Doxygen into XHTML
source code documentation.

Copyright (c) 2006 Jan Gaspar

Use, modification, and distribution is subject to the Boost Software
License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

<!DOCTYPE stylesheet [
  <!ENTITY nbsp "&#160;">
  <!ENTITY space "&#32;">
]>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:param name="container"/>
  <xsl:param name="xmldir"/>
  <xsl:variable name="container-ref" select="//compound[name=concat('boost::', $container) and @kind='class']/@refid"/>
  <xsl:variable name="class-file" select="concat($xmldir, '/', $container-ref, '.xml')"/>
  <xsl:variable name="class" select="document($class-file)/doxygen/compounddef[@id = $container-ref]"/>

  <xsl:template match="/">
    <html>
      <header><title>Source Code Documentation</title></header>
      <body>
        <xsl:apply-templates select="$class" mode="synopsis"/>
        <xsl:apply-templates select="$class" mode="description"/>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="nonbreakablespace">
    &nbsp;
  </xsl:template>

  <xsl:template match="bold">
    <b><xsl:apply-templates/></b>
  </xsl:template>

  <xsl:template match="linebreak">
    <br />
  </xsl:template>

  <xsl:template match="preformatted">
    <pre><xsl:apply-templates/></pre>
  </xsl:template>

  <xsl:template match="computeroutput">
    <code><xsl:apply-templates/></code>
  </xsl:template>

  <xsl:template match="para">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="ref">
    <xsl:variable name="link-text" select="substring-after(text(), concat($container, '::'))"/>
    <xsl:choose>
      <xsl:when test="string-length($link-text) &gt; 0">
        <xsl:element name="a">
          <xsl:attribute name="href"><xsl:call-template name="reference"/></xsl:attribute>
          <xsl:value-of select="$link-text"/>
        </xsl:element>
      </xsl:when>
      <xsl:when test="text() = $container">
        <xsl:value-of select="text()"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:element name="a">
          <xsl:choose>
            <xsl:when test="count(//memberdef[@id=current()/@refid]) &gt; 0">
              <xsl:attribute name="href"><xsl:value-of select="concat('#', @refid)"/></xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="href"><xsl:value-of select="concat($link-prefix, '#', @refid)"/></xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:value-of select="text()"/>
        </xsl:element>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="ulink">
    <a href="{@url}"><xsl:apply-templates/></a>
  </xsl:template>

  <xsl:template match="parameteritem">
    <xsl:param name="desc" select="''"/>
    <xsl:param name="style" select="'code'"/>
    <xsl:if test="$desc != ''">
      <dt><b><xsl:value-of select="$desc"/></b></dt>
    </xsl:if>
    <dd>
      <xsl:choose>
        <xsl:when test="$style = 'code'">
          <dl compact="yes">
            <dt><code><xsl:apply-templates select="parameternamelist/parametername"/></code></dt>
            <dd><xsl:apply-templates select="parameterdescription"/></dd>
          </dl>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates select="parameternamelist/parametername"/>
          <xsl:apply-templates select="parameterdescription"/>
        </xsl:otherwise>
      </xsl:choose>
    </dd>
    <xsl:apply-templates select="following-sibling::parameteritem[1]">
        <xsl:with-param name="style" select="$style"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="simplesect[@kind='return']">
    <dl><dt><b>Returns:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>

  <xsl:template match="simplesect[@kind='pre']">
    <dl><dt><b>Precondition:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>

  <xsl:template match="simplesect[@kind='post']">
    <dl><dt><b>Postcondition:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>

  <xsl:template match="simplesect[@kind='par']">
    <dl><dt><b><xsl:value-of select="title"/>:</b></dt><dd><xsl:apply-templates select="para"/></dd></dl>
  </xsl:template>

  <xsl:template match="simplesect[@kind='warning']">
    <dl><dt><b>Warning:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>

  <xsl:template match="simplesect[@kind='note']">
    <dl><dt><b>Note:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>

  <xsl:template match="simplesect[@kind='see']">
    <dl><dt><b>See Also:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>

  <xsl:template match="parameterlist[@kind='param']">
    <dl><xsl:apply-templates select="parameteritem[1]">
      <xsl:with-param name="desc" select="'Parameter(s):'"/>
    </xsl:apply-templates></dl>
  </xsl:template>

  <xsl:template match="parameterlist[@kind='exception']">
    <dl><xsl:apply-templates select="parameteritem[1]">
      <xsl:with-param name="desc" select="'Throws:'"/>
      <xsl:with-param name="style" select="'plain'"/>
    </xsl:apply-templates></dl>
  </xsl:template>

  <xsl:template match="briefdescription">
    <xsl:apply-templates/>
  </xsl:template>

  <!-- Synopsis mode -->

  <xsl:template match="compounddef[@kind = 'class']" mode="synopsis">
    <div id="srcdoc_synopsis">
      <table id="table_synopsis" border="0" cellpadding="10">
        <tr><td>
<pre>
namespace boost {

template &lt;<xsl:call-template name="template-parameters"/>&gt;
class <xsl:value-of select="$container"/>
{
public:
<xsl:call-template name="public-types"/><xsl:text disable-output-escaping="yes">
</xsl:text>
<xsl:call-template name="constructors"/><xsl:text disable-output-escaping="yes">
</xsl:text>
<xsl:call-template name="member-functions"/><xsl:text disable-output-escaping="yes">};

</xsl:text>
<xsl:call-template name="standalone-functions"/>
} // namespace boost
</pre>
        </td></tr>
      </table>
    </div>
  </xsl:template>

  <xsl:template match="param" mode="synopsis">
    <xsl:param name="link-prefix" select="''"/>
    <xsl:value-of select="type"/>&nbsp;<a href="{$link-prefix}#templateparam_{declname}"><xsl:value-of select="declname"/></a><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
  </xsl:template>

  <xsl:template match="memberdef[@kind='typedef']" mode="synopsis">
    <xsl:param name="link-prefix" select="''"/>
    <xsl:if test="normalize-space(briefdescription) != ''">&nbsp;&nbsp;&nbsp;typedef&nbsp;<xsl:value-of select="substring('typename ', 1 div (contains(type, '::') and not(contains(type, '&gt;'))))"/>
      <xsl:choose>
        <xsl:when test="contains(type, 'cb_details::')"><i>implementation-defined</i>&nbsp;</xsl:when>
        <xsl:otherwise><xsl:apply-templates select="type" mode="synopsis"/>&nbsp;</xsl:otherwise>
      </xsl:choose>
      <a href="{$link-prefix}#{@id}"><xsl:value-of select="name"/></a>;<xsl:text disable-output-escaping="yes">
</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="memberdef[@kind='function']" mode="synopsis">
    <xsl:param name="indent" select="'&nbsp;&nbsp;&nbsp;'"/>
    <xsl:param name="link-prefix" select="''"/>
    <xsl:param name="link" select="''"/>
    <xsl:value-of select="$indent"/>
    <xsl:value-of select="substring('explicit ', 1 div (@explicit = 'yes'))"/>
    <xsl:if test="count(templateparamlist) &gt; 0">template&nbsp;&lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;<xsl:text disable-output-escaping="yes">
&nbsp;&nbsp;&nbsp;</xsl:text><xsl:value-of select="$indent"/>
    </xsl:if>
    <xsl:if test="type != ''"><xsl:apply-templates select="type" mode="synopsis"/>&nbsp;</xsl:if>
    <a href="{$link-prefix}#{concat(substring(@id, 1 div (string-length($link) = 0)), substring($link, 1 div (string-length($link) &gt; 0)))}">
      <xsl:value-of select="name"/>
    </a>(<xsl:for-each select="param">
    <xsl:apply-templates select="type" mode="synopsis"/>&nbsp;<xsl:value-of select="declname"/>
    <xsl:value-of select="substring(concat(' = ', defval), 1 div (normalize-space(defval) != ''))"/>
    <xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
    </xsl:for-each>)<xsl:value-of select="substring(' const', 1 div (@const = 'yes'))"/>;<xsl:text disable-output-escaping="yes">
</xsl:text>
  </xsl:template>

  <xsl:template match="type" mode="synopsis">
    <xsl:for-each select="text() | ref">
      <xsl:variable name="item" select="translate(., '&space;', '')"/>
      <xsl:choose>
        <xsl:when test="contains($item, 'return_value_type') or contains($item, 'param_value_type')">const_reference</xsl:when>
        <xsl:otherwise>
        <xsl:choose>
            <xsl:when test="contains($item, ',')"><xsl:value-of select="concat(substring-before($item, ','), ',&nbsp;', substring-after($item, ','))"/></xsl:when>
            <xsl:otherwise><xsl:value-of select="$item"/></xsl:otherwise>
        </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:if test="not(contains($item, '&lt;')) and not(contains($item, ',')) and name(.) != 'ref' and position() != last()">&nbsp;</xsl:if>
    </xsl:for-each>
  </xsl:template>

  <!-- Description mode -->

  <xsl:template match="compounddef[@kind = 'class']" mode="description">
    <div id="srcdoc_params">
      <table id="table_template_params" border="1" cellpadding="3">
        <tr><th>Parameter</th><th>Description</th><th>Default</th></tr>
        <xsl:call-template name="template-parameters-details"/>
      </table>
    </div>
    <div id="srcdoc_types">
      <table id="table_public_types" border="1" cellpadding="3">
        <tr><th>Type</th><th>Description</th></tr>
        <xsl:call-template name="public-types-details"/>
      </table>
    </div>
    <div id="srcdoc_constructors">
      <table id="table_constructors" border="1" cellpadding="3">
        <xsl:call-template name="constructors-details"/>
      </table>
    </div>
    <div id="srcdoc_methods">
      <table id="table_methods" border="1" cellpadding="3">
        <xsl:call-template name="member-functions-details"/>
      </table>
    </div>
    <div id="srcdoc_functions">
      <table id="table_functions" border="1" cellpadding="3">
        <xsl:call-template name="standalone-functions-details"/>
      </table>
    </div>
  </xsl:template>

  <xsl:template match="parameteritem" mode="description">
    <tr><td><a id="templateparam_{parameternamelist/parametername}"><code><xsl:value-of select="parameternamelist/parametername"/></code></a></td>
    <td>
        <xsl:apply-templates select="parameterdescription"/>
        <xsl:variable name="type-rqmts" select="concat('Type Requirements ', parameternamelist/parametername)"/>
        <xsl:if test="../../simplesect[@kind='par']/title = $type-rqmts">
            <dl><dt><b>Type Requirements:</b></dt>
            <dd><xsl:apply-templates select="../../simplesect[@kind='par'][title=$type-rqmts]/para"/></dd></dl>
        </xsl:if>
    </td>
    <xsl:variable name="default-value" select="concat('Default ', parameternamelist/parametername)"/>
    <xsl:choose>
     <xsl:when test="../../simplesect[@kind='par']/title = $default-value">
       <td><code><xsl:value-of select="../../simplesect[@kind='par'][title=$default-value]/para"/></code></td>
     </xsl:when>
     <xsl:otherwise><td><br/></td></xsl:otherwise>
    </xsl:choose>
    </tr>
  </xsl:template>

  <xsl:template match="memberdef[@kind='typedef']" mode="description">
    <xsl:if test="normalize-space(briefdescription) != ''">
      <tr><td><a id="{@id}"><code><xsl:value-of select="name"/></code></a></td>
      <td>
        <xsl:apply-templates select="briefdescription/child::*"/>
        <xsl:if test="boolean(detaileddescription/para)">
          <xsl:apply-templates select="detaileddescription/para/text()|detaileddescription/para/child::*"/>
        </xsl:if>
      </td></tr>
    </xsl:if>
  </xsl:template>

  <xsl:template match="memberdef[@kind='function']" mode="description">
    <xsl:param name="link-prefix" select="''"/>
    <tr><td><a id="{@id}" /><code><b><xsl:value-of select="substring('explicit ', 1 div (@explicit = 'yes'))"/>
    <xsl:if test="count(templateparamlist) &gt; 0">
      template&nbsp;&lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;<br />&nbsp;&nbsp;&nbsp;
    </xsl:if>
    <xsl:if test="type != ''"><xsl:apply-templates select="type" mode="description"/>&nbsp;</xsl:if>
    <xsl:value-of select="name"/>(<xsl:for-each select="param">
    <xsl:apply-templates select="type" mode="description"/>&nbsp;<xsl:value-of select="declname"/>
    <xsl:value-of select="substring(concat(' = ', defval), 1 div (normalize-space(defval) != ''))"/>
    <xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
    </xsl:for-each>)<xsl:value-of select="substring(' const', 1 div (@const = 'yes'))"/>;</b></code><br /><br />
    <xsl:apply-templates select="briefdescription"/>
    <xsl:if test="normalize-space(detaileddescription) = ''">
      <br /><br />
    </xsl:if>
    <p><xsl:apply-templates select="detaileddescription"/></p>
    </td></tr>
  </xsl:template>

  <xsl:template match="type" mode="description">
    <xsl:for-each select="text() | ref">
      <xsl:variable name="item" select="translate(., '&space;', '')"/>
      <xsl:choose>
        <xsl:when test="$item = 'return_value_type' or $item = 'param_value_type'">
          <xsl:variable name="const-reference" select="$class[@kind = 'class']/sectiondef[@kind='public-type']/memberdef[@kind='typedef' and name='const_reference']"/>
          <xsl:choose>
            <xsl:when test="boolean($const-reference/reimplements)">
              <a href="{$link-prefix}#{$const-reference/reimplements/@refid}">const_reference</a>
            </xsl:when>
            <xsl:otherwise>
              <a href="{$link-prefix}#{$const-reference/@id}">const_reference</a>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:when test="contains($item, 'circular_buffer')"><xsl:value-of select="$item"/></xsl:when>
        <xsl:when test="@refid">
          <xsl:element name="a">
            <xsl:attribute name="href"><xsl:call-template name="reference"/></xsl:attribute>
            <xsl:value-of select="$item"/>
          </xsl:element>
        </xsl:when>
        <xsl:otherwise><xsl:value-of select="$item"/></xsl:otherwise>
      </xsl:choose>
      <xsl:if test="name(.) != 'ref' and position() != last()">&nbsp;</xsl:if>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
