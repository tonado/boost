<?xml version='1.0' encoding='iso-8859-1'?>
<!--
XSL transformation from the XML files generated by doxygen into HTML source documentation.
Author: Jan Gaspar (jano_gaspar[at]yahoo.com)
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  
  <xsl:import href="doxygen2html.xslt"/>
  
  <xsl:output method="xml" version="1.0" encoding="iso-8859-1" indent="yes" media-type="text/xml"/>

  <xsl:variable name="link-prefix" select="''"/>
  <xsl:variable name="standalone-functions" select="document(concat($xmldir, '/namespaceboost.xml'))/doxygen/compounddef/sectiondef[@kind='func']"/>

  <xsl:template name="reference">
    <xsl:value-of select="concat('#', @refid)"/>
  </xsl:template>
  
  <xsl:template name="template-parameters">
    <xsl:apply-templates select="templateparamlist/param" mode="synopsis"/>
  </xsl:template>
  
  <xsl:template name="public-types">
    <xsl:apply-templates select="sectiondef[@kind='public-type']/memberdef" mode="synopsis">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>
  
  <xsl:template name="member-functions">
    <xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type != '']" mode="synopsis">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template name="standalone-functions">
    <xsl:apply-templates select="$standalone-functions/memberdef[contains(argsstring, 'circular_buffer&lt;')]" mode="synopsis">
      <xsl:sort select="name"/>
      <xsl:with-param name="indent" select="''"/>
    </xsl:apply-templates>
  </xsl:template>
  
  <xsl:template name="template-parameters-details">
    <xsl:apply-templates select="detaileddescription//parameterlist[@kind='param']/parameteritem" mode="description"/>
  </xsl:template>
  
  <xsl:template name="public-types-details">
    <xsl:apply-templates select="sectiondef[@kind='public-type']/memberdef" mode="description">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>
  
  <xsl:template name="constructors-details">
    <xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type = '']" mode="description">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>
  
  <xsl:template name="member-functions-details">
    <xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type != '']" mode="description">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>
  
  <xsl:template name="standalone-functions-details">
    <xsl:apply-templates select="$standalone-functions/memberdef[contains(argsstring, 'circular_buffer&lt;')]" mode="description">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>

</xsl:stylesheet>
