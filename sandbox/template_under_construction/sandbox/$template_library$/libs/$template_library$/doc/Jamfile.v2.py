# template file

# Copyright 2007 Stjepan Rajko.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

docs = template.options.get('docs')
doxygen = docs != 'qb'
    

content = """$template_start$
$template_python_copyright$

project boost/$template_library$/doc ;

import boostbook : boostbook ;
using quickbook ;

local loc = [ path.native [ path.pwd ] ] ;
local root = [ path.native [ path.join [ path.pwd ] ../../.. ] ] ;

xml $template_library$_xml : $template_library$.qbk ;"""

if doxygen:
    content += """
doxygen $template_library$_doxygen
   :
      [ glob ../../../boost/$template_library$/*.hpp dox/*.hpp ]
   :
        <doxygen:param>EXAMPLE_PATH=../example
        <doxygen:param>STRIP_FROM_PATH=$(root)
        <doxygen:param>STRIP_FROM_INC_PATH=$(root)
        <doxygen:param>EXTRACT_ALL=NO
        <doxygen:param>HIDE_UNDOC_MEMBERS=NO
        <doxygen:param>EXTRACT_PRIVATE=NO
        <doxygen:param>DETAILS_AT_TOP=YES
        <doxygen:param>ENABLE_PREPROCESSING=YES
        <doxygen:param>MACRO_EXPANSION=YES
        <doxygen:param>SEARCH_INCLUDES=YES
        <doxygen:param>INCLUDE_PATH=../../..
        <doxygen:param>PREDEFINED=DOXYGEN_DOCS_ONLY"""

    if docs=='qb+doxy':
        content += """
        <doxygen:param>GENERATE_HTML=YES
        <doxygen:param>HTML_OUTPUT=$(loc)/html/doxygen
        <doxygen:param>HTML_STYLESHEET=$(loc)/html/boostbook_doxygen.css"""
    content +="""
   ;
"""

content += """
boostbook standalone
   :
      $template_library$_xml"""
if (doxygen):
    content+="""
      $template_library$_doxygen"""

content +="""
    :
        <xsl:param>chunk.first.sections=1
        <xsl:param>chunk.section.depth=3
        <xsl:param>toc.section.depth=3
        <xsl:param>toc.max.depth=3
        <xsl:param>generate.section.toc.level=3
    ;
"""

template.submit_content(content)
