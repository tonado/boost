#!/usr/bin/perl -w

# Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

use strict;

system("doxygen reference.doxygen");
chdir("xml");
system("xsltproc combine.xslt index.xml > all.xml");
chdir("..");
system("xsltproc reference.xsl xml/all.xml > reference.qbk");
#system("rm -rf xml");

#system("rm -rf xml reference.tags");
