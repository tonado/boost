<?xml version='1.0' encoding='iso-8859-1'?>

<!DOCTYPE stylesheet [
  <!ENTITY nbsp "&#160;">
  <!ENTITY space "&#32;">
]>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  
  <xsl:param name="container"/>
  <xsl:param name="xmldir"/>
  <xsl:variable name="doxygen-version">1.4.1</xsl:variable>
  
  <xsl:template match="/">
    <xsl:if test="doxygenindex/@version != $doxygen-version">
      <xsl:message>Warning: The source documentation was generated by the doxygen version <xsl:value-of select="doxygenindex/@version"/> which differs from the reference doxygen version (<xsl:value-of select="$doxygen-version"/>).</xsl:message>
    </xsl:if>
    <html>
      <header><title>Source Code Documentation</title></header>
      <body>
        <xsl:variable name="container-ref" select="//compound[name=concat('boost::', $container) and @kind='class']/@refid"/>
        <xsl:variable name="class-file" select="concat($xmldir, '/', $container-ref, '.xml')"/>
        <xsl:variable name="class" select="document($class-file)/doxygen/compounddef[@id = $container-ref]"/>
        <xsl:apply-templates select="$class" mode="synopsis"/>
        <xsl:apply-templates select="$class" mode="description"/>
      </body>
    </html>
  </xsl:template>
  
  <xsl:template match="linebreak">
    <br />
  </xsl:template>
  
  <xsl:template match="computeroutput">
    <code><xsl:apply-templates/></code>
  </xsl:template>
  
  <xsl:template match="para">
    <xsl:apply-templates/>
  </xsl:template>
  
  <xsl:template match="ref">
    <a href="#{@refid}"><xsl:apply-templates/></a>
  </xsl:template>
  
  <xsl:template match="ulink">
    <a href="{@url}"><xsl:apply-templates/></a>
  </xsl:template>
  
  <xsl:template match="parametername">
    <dd><i><xsl:apply-templates/></i>
    <xsl:apply-templates select="following::parameterdescription[1]"/></dd>
  </xsl:template>
  
  <xsl:template match="parameterlist[@kind='param']">
    <dl><dt><b>Parameters:</b></dt><xsl:apply-templates select="parametername"/></dl>
  </xsl:template>
  
  <xsl:template match="parameterlist[@kind='exception']">
    <dl><dt><b>Throws:</b></dt><xsl:apply-templates select="parametername"/></dl>
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
  
  <xsl:template match="simplesect[@kind='note']">
    <dl><dt><b>Note:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>
  
  <!-- Synopsis mode -->
    
  <xsl:template match="compounddef[@kind = 'class']" mode="synopsis">
    <div id="srcdoc_synopsis">
      <table border="0" cellpadding="5">
        <tr><td></td><td>
<pre>
namespace boost {

template &lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;
class <xsl:value-of select="$container"/>
{
public:
<xsl:apply-templates select="sectiondef[@kind='public-type']/memberdef" mode="synopsis"/><xsl:text disable-output-escaping="yes">
</xsl:text>
<xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type = '']" mode="synopsis">
  <xsl:sort select="name"/>
</xsl:apply-templates><xsl:text disable-output-escaping="yes">
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
  
  <xsl:template match="memberdef[@kind='typedef']" mode="synopsis">
    <xsl:if test="normalize-space(briefdescription) != ''">&nbsp;&nbsp;&nbsp;typedef&nbsp;<xsl:value-of select="substring('typename ', 1 div (contains(type, '::') and not(contains(type, '&gt;'))))"/>
      <xsl:choose>
        <xsl:when test="contains(type, '&gt;')"><i>implementation-defined</i>&nbsp;</xsl:when>
        <xsl:otherwise><xsl:value-of select="type"/>&nbsp;</xsl:otherwise>
      </xsl:choose>
      <a href="#{@id}"><xsl:value-of select="name"/></a>;<xsl:text disable-output-escaping="yes">
</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="memberdef[@kind='function']" mode="synopsis">
    <xsl:param name="indent" select="'&nbsp;&nbsp;&nbsp;'"/>
    <xsl:param name="link-prefix" select="''"/>
    <xsl:variable name="exclude-method">
      <xsl:call-template name="exclude-method">
        <xsl:with-param name="name" select="name"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="too-long-args" select="string-length(argsstring) &gt; 80"/>
    <xsl:if test="not(boolean(string($exclude-method)))">
      <xsl:value-of select="$indent"/>
      <xsl:value-of select="substring('explicit ', 1 div (@explicit = 'yes'))"/>
      <xsl:if test="count(templateparamlist) &gt; 0">template&nbsp;&lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;<xsl:text disable-output-escaping="yes">
&nbsp;&nbsp;&nbsp;</xsl:text><xsl:value-of select="$indent"/>
      </xsl:if>
      <xsl:if test="type != ''"><xsl:apply-templates select="type" mode="synopsis"/>&nbsp;</xsl:if>
      <a href="{$link-prefix}#{@id}">
        <xsl:value-of select="name"/>
      </a>(<xsl:for-each select="param">
      <xsl:if test="$too-long-args and not((count(preceding-sibling::param) + 1) mod 2)">
        <xsl:text disable-output-escaping="yes">
</xsl:text><xsl:value-of select="$indent"/>&nbsp;&nbsp;&nbsp;<xsl:text/>
        <xsl:if test="count(../templateparamlist) &gt; 0">&nbsp;&nbsp;&nbsp;</xsl:if>
      </xsl:if>
      <xsl:apply-templates select="type" mode="synopsis"/>&nbsp;<xsl:value-of select="declname"/>
      <xsl:value-of select="substring(concat(' = ', defval), 1 div (normalize-space(defval) != ''))"/>
      <xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
      </xsl:for-each>)<xsl:value-of select="substring(' const', 1 div (@const = 'yes'))"/>;<xsl:text disable-output-escaping="yes">
</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="type" mode="synopsis">
    <xsl:for-each select="text() | ref">
      <xsl:variable name="item" select="translate(., '&space;', '')"/>
      <xsl:choose>
        <xsl:when test="$item = 'return_value_type' or $item = 'param_value_type'">value_type</xsl:when>
        <xsl:otherwise><xsl:value-of select="$item"/></xsl:otherwise>
      </xsl:choose>
      <xsl:if test="name(.) != 'ref' and position() != last()">&nbsp;</xsl:if>
    </xsl:for-each>
  </xsl:template>
  
  <!-- Description mode -->
  
  <xsl:template match="compounddef[@kind = 'class']" mode="description">
    <div id="srcdoc_types">
      <!--
      <xsl:apply-templates select="sectiondef[@kind='public-type']/memberdef" mode="description">
        <xsl:sort select="name"/>
      </xsl:apply-templates>
      -->
    </div>
    <div id="srcdoc_constructors">
      <xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type = '']" mode="description">
          <xsl:sort select="name"/>
      </xsl:apply-templates>
    </div>
    <div id="srcdoc_methods">
      <xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type != '']" mode="description">
        <xsl:sort select="name"/>
      </xsl:apply-templates>
    </div>
    <div id="srcdoc_functions">
    </div>
  </xsl:template>
  
  <xsl:template match="memberdef[@kind='function' or @kind='typedef']" mode="description">
    <a name="{@id}" />
    <b><pre>&nbsp;<xsl:value-of select="substring('explicit ', 1 div (@explicit = 'yes'))"/>
      <xsl:if test="count(templateparamlist) &gt; 0">
          <xsl:text disable-output-escaping="yes">template </xsl:text>&lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;
      </xsl:if>
      <xsl:apply-templates select="type" mode="description"/>
      <xsl:value-of select="substring(' ', 1 div (normalize-space(type) != ''))"/>
          <xsl:value-of select="name"/>(<xsl:for-each select="param">
          <xsl:apply-templates select="type" mode="description"/>&nbsp;<xsl:value-of select="declname"/>
          <xsl:value-of select="substring(concat(' = ', defval), 1 div (normalize-space(defval) != ''))"/>
          <xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
      </xsl:for-each>)<xsl:value-of select="substring(' const', 1 div (@const = 'yes'))"/>;</pre></b>
    <dl>
      <dd>
        <xsl:apply-templates select="briefdescription"/>
        <xsl:apply-templates select="detaileddescription"/>
      </dd>
    </dl>
    <hr align="left" size="1" />
  </xsl:template>

  <xsl:template match="type" mode="description">
    <xsl:for-each select="text() | ref">
        <xsl:choose>
        <xsl:when test="name(.) = 'ref'">
            <a href="#{@refid}"><xsl:value-of select="."/></a>
        </xsl:when>
        <xsl:otherwise>
            <xsl:choose>
                <xsl:when test=". = 'return_value_type' or . = 'param_value_type'">
                    <a href="#{../../../../sectiondef[@kind='public-type']/memberdef[name = 'value_type']/@id}">value_type</a>
                </xsl:when>
                <xsl:when test=". = ' &amp;'">
                    <xsl:text>&amp;</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="."/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:otherwise>
    </xsl:choose>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
