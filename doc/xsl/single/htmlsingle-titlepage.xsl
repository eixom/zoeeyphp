<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:exsl="http://exslt.org/common" version="1.0" exclude-result-prefixes="exsl">

<!-- This stylesheet was created by template/titlepage.xsl-->

<xsl:template name="book.titlepage.recto">
  <xsl:choose>
    <xsl:when test="bookinfo/title">
      <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/title"/>
    </xsl:when>
    <xsl:when test="info/title">
      <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/title"/>
    </xsl:when>
    <xsl:when test="title">
      <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="title"/>
    </xsl:when>
  </xsl:choose>

  <xsl:choose>
    <xsl:when test="bookinfo/subtitle">
      <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/subtitle"/>
    </xsl:when>
    <xsl:when test="info/subtitle">
      <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/subtitle"/>
    </xsl:when>
    <xsl:when test="subtitle">
      <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="subtitle"/>
    </xsl:when>
  </xsl:choose>

  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/corpauthor"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/corpauthor"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/authorgroup"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/authorgroup"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/author"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/author"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/mediaobject"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/mediaobject"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/othercredit"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/othercredit"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/releaseinfo"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/releaseinfo"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/copyright"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/copyright"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/legalnotice"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/legalnotice"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/pubdate"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/pubdate"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/revision"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/revision"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/revhistory"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/revhistory"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="bookinfo/abstract"/>
  <xsl:apply-templates mode="book.titlepage.recto.auto.mode" select="info/abstract"/>
</xsl:template>

<xsl:template name="book.titlepage.verso">
</xsl:template>

<xsl:template name="book.titlepage.separator"><hr/>
</xsl:template>

<xsl:template name="book.titlepage.before.recto">
</xsl:template>

<xsl:template name="book.titlepage.before.verso">
</xsl:template>

<xsl:template name="book.titlepage">
  <div class="titlepage">
    <xsl:variable name="recto.content">
      <xsl:call-template name="book.titlepage.before.recto"/>
      <xsl:call-template name="book.titlepage.recto"/>
    </xsl:variable>
    <xsl:variable name="recto.elements.count">
      <xsl:choose>
        <xsl:when test="function-available('exsl:node-set')"><xsl:value-of select="count(exsl:node-set($recto.content)/*)"/></xsl:when>
        <xsl:when test="contains(system-property('xsl:vendor'), 'Apache Software Foundation')">
          <!--Xalan quirk--><xsl:value-of select="count(exsl:node-set($recto.content)/*)"/></xsl:when>
        <xsl:otherwise>1</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:if test="(normalize-space($recto.content) != '') or ($recto.elements.count &gt; 0)">
      <div><xsl:copy-of select="$recto.content"/></div>
    </xsl:if>
    <xsl:variable name="verso.content">
      <xsl:call-template name="book.titlepage.before.verso"/>
      <xsl:call-template name="book.titlepage.verso"/>
    </xsl:variable>
    <xsl:variable name="verso.elements.count">
      <xsl:choose>
        <xsl:when test="function-available('exsl:node-set')"><xsl:value-of select="count(exsl:node-set($verso.content)/*)"/></xsl:when>
        <xsl:when test="contains(system-property('xsl:vendor'), 'Apache Software Foundation')">
          <!--Xalan quirk--><xsl:value-of select="count(exsl:node-set($verso.content)/*)"/></xsl:when>
        <xsl:otherwise>1</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:if test="(normalize-space($verso.content) != '') or ($verso.elements.count &gt; 0)">
      <div><xsl:copy-of select="$verso.content"/></div>
    </xsl:if>
    <xsl:call-template name="book.titlepage.separator"/>
  </div>
</xsl:template>

<xsl:template match="*" mode="book.titlepage.recto.mode">
  <!-- if an element isn't found in this mode, -->
  <!-- try the generic titlepage.mode -->
  <xsl:apply-templates select="." mode="titlepage.mode"/>
</xsl:template>

<xsl:template match="*" mode="book.titlepage.verso.mode">
  <!-- if an element isn't found in this mode, -->
  <!-- try the generic titlepage.mode -->
  <xsl:apply-templates select="." mode="titlepage.mode"/>
</xsl:template>

<xsl:template match="title" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="subtitle" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="corpauthor" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="authorgroup" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="author" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="mediaobject" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="othercredit" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="releaseinfo" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="copyright" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="legalnotice" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="pubdate" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="revision" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="revhistory" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

<xsl:template match="abstract" mode="book.titlepage.recto.auto.mode">
<div xsl:use-attribute-sets="book.titlepage.recto.style">
<xsl:apply-templates select="." mode="book.titlepage.recto.mode"/>
</div>
</xsl:template>

</xsl:stylesheet>
