// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2010-2012 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//#define BOOST_GEOMETRY_DEBUG_WITH_MAPPER
//#define BOOST_GEOMETRY_DEBUG_ASSEMBLE
//#define BOOST_GEOMETRY_DEBUG_IDENTIFIER

#include <geometry_test_common.hpp>

#include <boost/geometry/algorithms/buffer.hpp>
#include <boost/geometry/core/coordinate_type.hpp>

#include <boost/geometry/strategies/strategies.hpp>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <test_common/test_point.hpp>

#include <test_buffer.hpp>

static std::string const simplex = "LINESTRING(0 0,4 5)";
static std::string const straight = "LINESTRING(0 0,4 5,8 10)";
static std::string const one_bend = "LINESTRING(0 0,4 5,7 4)";
static std::string const two_bends = "LINESTRING(0 0,4 5,7 4,10 6)";
static std::string const overlapping = "LINESTRING(0 0,4 5,7 4,10 6, 10 2,2 2)";
static std::string const curve = "LINESTRING(2 7,3 5,5 4,7 5,8 7)";

static std::string const for_collinear = "LINESTRING(2 0,0 0,0 4,6 4,6 0,4 0)";
static std::string const for_collinear2 = "LINESTRING(2 1,2 0,0 0,0 4,6 4,6 0,4 0,4 1)";

static std::string const chained2 = "LINESTRING(0 0,1 1,2 2)";
static std::string const chained3 = "LINESTRING(0 0,1 1,2 2,3 3)";
static std::string const chained4 = "LINESTRING(0 0,1 1,2 2,3 3,4 4)";

static std::string const reallife1 = "LINESTRING(76396.40464822574 410095.6795147947,76397.85016212701 410095.211865792,76401.30666443033 410095.0466387949,76405.05892643372 410096.1007777959,76409.45103273794 410098.257640797,76412.96309264141 410101.6522238015)";

// Test first 20 cases of the list sent by Aimes on geometry list at 2012 Jul 19.
// Complete list at: http://boost-geometry.203548.n3.nabble.com/file/n4025063/BufferProblems.txt
template <typename P>
void test_aimes()
{
    namespace buf = bg::strategy::buffer;
    typedef bg::model::linestring<P> linestring;
    typedef bg::model::polygon<P> polygon;
	double const aimes_width = 0.000018;

	static std::string const aimes1 = "LINESTRING(-3.031075 52.22385,-3.030809 52.223579,-3.030424 52.223207,-3.030212 52.222931,-3.030013 52.222764,-3.029721 52.222584,-3.029469 52.222443,-3.029218 52.222319,-3.028886 52.222187,-3.028859 52.222176,-3.028815 52.222158,-3.028563 52.222016,-3.028462 52.221945,-3.028407 52.221881,-3.028351 52.22179,-3.028301 52.221718,-3.028203 52.221601,-3.028151 52.221552,-3.027982 52.221422,-3.027871 52.221353,-3.027755 52.221293,-3.027573 52.22121,-3.027493 52.221179,-3.027463 52.221165,-3.027147 52.221065,-3.027045 52.221039,-3.026812 52.220989,-3.026691 52.220968,-3.026602 52.220957,-3.026111 52.220898,-3.025715 52.220861,-3.025332 52.220842,-3.025011 52.220845,-3.024587 52.220849,-3.024339 52.220869,-3.024213 52.220881,-3.024103 52.220905,-3.023904 52.220966,-3.023687 52.221027,-3.023575 52.221054,-3.023409 52.221077,-3.023317 52.221072,-3.023259 52.221049,-3.023028 52.220905,-3.022859 52.220819,-3.022799 52.220774,-3.022739 52.220688)";
	static std::string const aimes2 = "LINESTRING(-3.056646 52.20421,-3.055298 52.204143,-3.054991 52.204119,-3.053966 52.204125,-3.053793 52.204126)";
	static std::string const aimes3 = "LINESTRING(-3.040117 52.307787,-3.040012 52.307874,-3.03999 52.30792,-3.039986 52.307973,-3.040054 52.308123,-3.040092 52.308238,-3.040125 52.308398,-3.040165 52.308558,-3.040194 52.308587,-3.040539 52.30889,-3.040868 52.309147,-3.041196 52.309396,-3.042401 52.310114,-3.042414 52.310121,-3.042758 52.31032,-3.04277 52.310327,-3.042779 52.310353,-3.042771 52.31038,-3.042746 52.310444,-3.042627 52.310704,-3.042601 52.310799,-3.042588 52.310846,-3.042588 52.310941,-3.042596 52.311007,-3.04261 52.311035,-3.04264 52.311099,-3.042646 52.311111,-3.042662 52.311134,-3.042681 52.311161,-3.042708 52.311199,-3.042792 52.311275,-3.042808 52.31129,-3.042908 52.311364,-3.04294 52.311385,-3.042996 52.311423,-3.043232 52.311542,-3.043424 52.311632,-3.043447 52.311641,-3.043452 52.311643,-3.043537 52.311677,-3.043725 52.311728,-3.043876 52.311759,-3.044048 52.311784,-3.044325 52.311813,-3.044963 52.31187,-3.045654 52.311927,-3.046044 52.31196,-3.04609 52.311963,-3.046235 52.311979,-3.046374 52.312015,-3.046477 52.312058,-3.046587 52.312109,-3.046654 52.312147,-3.046771 52.312228,-3.046962 52.312372,-3.047043 52.312443,-3.047089 52.312498,-3.04711 52.312547,-3.047112 52.312586,-3.047113 52.312607,-3.047105 52.312666,-3.047067 52.312722)";
	static std::string const aimes4 = "LINESTRING(-3.054162 52.34456,-3.054088 52.344534,-3.054014 52.344508,-3.053926 52.34449,-3.053823 52.344491,-3.053632 52.344493,-3.053368 52.344513,-3.053149 52.344551,-3.053076 52.34457,-3.053033 52.344588,-3.053003 52.344598)";
	static std::string const aimes5 = "LINESTRING(-3.032714 52.342934,-3.032658 52.342814,-3.032617 52.342701,-3.032583 52.342606,-3.032554 52.342502,-3.032517 52.342439,-3.032453 52.34233,-3.032374 52.3422,-3.032325 52.342121,-3.032288 52.342056,-3.032252 52.342028)";
	static std::string const aimes6 = "LINESTRING(-3.041066 52.338221,-3.040547 52.33801,-3.040399 52.337939,-3.040398 52.337921,-3.040427 52.337894,-3.040456 52.337867,-3.040499 52.337849)";
	static std::string const aimes7 = "LINESTRING(-3.044091 52.565713,-3.044077 52.565734,-3.044071 52.565742,-3.044028 52.565796,-3.043992 52.565879,-3.043952 52.565959,-3.043915 52.566,-3.043857 52.56604,-3.043741 52.566113,-3.043492 52.566223,-3.042615 52.566563,-3.042425 52.566646,-3.042294 52.566683,-3.041932 52.566745,-3.041648 52.566791,-3.041445 52.566841,-3.041226 52.566924,-3.041066 52.566992,-3.04089 52.567056,-3.040865 52.56708,-3.040858 52.5671,-3.040879 52.567118,-3.040944 52.567135,-3.041034 52.567137,-3.041288 52.567141,-3.041822 52.567144,-3.042107 52.567134,-3.042245 52.567142,-3.04229 52.56715,-3.042558 52.567247,-3.042736 52.56729,-3.042918 52.567326,-3.043082 52.567338,-3.043388 52.567401,-3.043848 52.567532,-3.043908 52.567568,-3.043923 52.567577,-3.043938 52.567603,-3.04394 52.567657,-3.043941 52.567711,-3.043895 52.56798,-3.043869 52.568115,-3.043868 52.568138,-3.043869 52.568158)";
	static std::string const aimes8 = "LINESTRING(-3.039713 52.536795,-3.039808 52.536825,-3.039958 52.536844,-3.040172 52.536885,-3.040322 52.536924,-3.040476 52.536964,-3.04061 52.536989,-3.040678 52.536992,-3.040773 52.536986,-3.040949 52.536958,-3.041148 52.536905,-3.041312 52.536857,-3.041544 52.536777,-3.04173 52.536705,-3.041942 52.536634,-3.042081 52.5366,-3.042204 52.53659,-3.042324 52.536587,-3.042414 52.536591,-3.042491 52.536598,-3.042595 52.536619,-3.042767 52.536683,-3.042916 52.536749,-3.043007 52.536786,-3.043108 52.5368)";
	static std::string const aimes9 = "LINESTRING(-3.049078 52.571837,-3.049229 52.571855,-3.04964 52.571857,-3.049841 52.571858,-3.049967 52.571873,-3.05006 52.571903,-3.050094 52.571922,-3.050135 52.57194,-3.050228 52.571958,-3.05033 52.571969,-3.050391 52.571987,-3.050413 52.572027,-3.050421 52.572072,-3.050386 52.572205)";
	static std::string const aimes10 = "LINESTRING(-2.983733 52.233609,-2.98371 52.233684,-2.983736 52.233793,-2.98379 52.233906,-2.983869 52.233994,-2.984058 52.234121,-2.984225 52.234228,-2.984382 52.234341,-2.984557 52.234481,-2.984695 52.234575,-2.984716 52.234589,-2.984758 52.234617,-2.984943 52.234694,-2.984978 52.234706,-2.985031 52.234722,-2.98506 52.234731,-2.985343 52.234799,-2.985594 52.234867,-2.985755 52.234917,-2.986071 52.23497,-2.9863 52.235006,-2.986458 52.235044,-2.986602 52.235109,-2.987015 52.23534,-2.987215 52.235464,-2.987423 52.235617,-2.987637 52.235785,-2.987968 52.23602,-2.988192 52.236213,-2.988438 52.236386,-2.988512 52.236449,-2.988579 52.236505,-2.98865 52.236583,-2.98869 52.236656,-2.988721 52.236744,-2.988612 52.236848,-2.988548 52.236932,-2.988497 52.237107,-2.988432 52.237336,-2.988372 52.237567,-2.98833 52.237709,-2.988286 52.23788,-2.988233 52.23807,-2.988249 52.238145,-2.988291 52.238321,-2.988323 52.238505,-2.988379 52.238624,-2.988435 52.238727,-2.988438 52.238732,-2.988466 52.238777,-2.988485 52.238809,-2.988525 52.238854,-2.98858 52.238916,-2.988715 52.239098,-2.988806 52.239204,-2.988865 52.239275,-2.988962 52.239393,-2.989045 52.239474,-2.989244 52.239629,-2.989254 52.239636,-2.989273 52.239649,-2.989508 52.239816,-2.989534 52.239835,-2.989747 52.23998,-2.989769 52.239996,-2.98982 52.240033,-2.989843 52.240055,-2.989949 52.240152,-2.989956 52.240159,-2.99002 52.240248,-2.990074 52.240334,-2.990079 52.24034,-2.990089 52.240357,-2.990106 52.240401,-2.99015 52.240512,-2.990268 52.240793,-2.990336 52.24095,-2.990425 52.241123,-2.990472 52.241198,-2.990617 52.241383,-2.990723 52.241527,-2.990784 52.241601,-2.990811 52.241633,-2.990859 52.241753,-2.990892 52.241854,-2.99094 52.241995,-2.990947 52.242025,-2.990958 52.24207,-2.991023 52.242175,-2.991122 52.242333,-2.991202 52.242485,-2.991276 52.242624,-2.991408 52.242809,-2.991639 52.2431,-2.99172 52.243202,-2.991734 52.243222,-2.991897 52.243461,-2.991944 52.243547,-2.992072 52.243781,-2.992267 52.244138,-2.992431 52.244408,-2.992503 52.244532,-2.992657 52.244802,-2.992761 52.245063,-2.9928 52.245305,-2.992813 52.245622,-2.992822 52.245701,-2.99281 52.245784,-2.992793 52.245867,-2.992771 52.245927,-2.992736 52.245978,-2.992646 52.246076,-2.992573 52.246144,-2.992455 52.246276,-2.992316 52.246468,-2.992432 52.246557,-2.99253 52.246568,-2.992901 52.246565,-2.993581 52.246527,-2.993926 52.246509,-2.994191 52.246514)";
	static std::string const aimes11 = "LINESTRING(-3.012501 52.228425,-3.012694 52.22837,-3.012848 52.228308,-3.012946 52.22824,-3.013055 52.228146,-3.013123 52.228048,-3.013182 52.227904,-3.013242 52.227742,-3.013269 52.227632,-3.01327 52.227553,-3.013245 52.227479,-3.013151 52.227332,-3.013072 52.227223,-3.012938 52.227095,-3.012779 52.226936,-3.01271 52.226867,-3.012639 52.226817,-3.012452 52.226743,-3.012278 52.226685,-3.01201 52.226616,-3.011707 52.226519,-3.011541 52.226458,-3.011388 52.22638,-3.011334 52.226352,-3.011111 52.226206,-3.010983 52.226107,-3.010893 52.226012,-3.010832 52.225905,-3.010825 52.225845,-3.010841 52.225687,-3.010858 52.225516,-3.010857 52.225451,-3.010844 52.225405,-3.010747 52.225247,-3.01062 52.225123,-3.010467 52.224893,-3.010372 52.224658)";
	static std::string const aimes12 = "LINESTRING(-2.998218 52.230123,-2.998074 52.230203,-2.998066 52.230206,-2.997957 52.230258,-2.997863 52.230302,-2.997049 52.230561,-2.996351 52.230783,-2.995741 52.230976,-2.995552 52.231032)";
	static std::string const aimes13 = "LINESTRING(-3.005291 52.231131,-3.005218 52.2312,-3.005076 52.23132,-3.004901 52.231443,-3.004725 52.231551,-3.004586 52.231621,-3.004439 52.231667,-3.004381 52.231685,-3.004236 52.231693,-3.004106 52.231684,-3.003877 52.231653,-3.003705 52.231618,-3.003613 52.231587,-3.003529 52.231549,-3.003019 52.231279,-3.002922 52.231244,-3.002789 52.231196,-3.002657 52.231144,-3.002492 52.231112,-3.002372 52.231091,-3.002239 52.231073,-3.001708 52.231062,-3.001596 52.231053,-3.000959 52.230853,-3.000714 52.230758,-3.00005 52.2305,-2.999016 52.230131,-2.999001 52.230122)";
	static std::string const aimes14 = "LINESTRING(-3.031075 52.22385,-3.030809 52.223579,-3.030424 52.223207,-3.030212 52.222931,-3.030013 52.222764,-3.029721 52.222584,-3.029469 52.222443,-3.029218 52.222319,-3.028886 52.222187,-3.028859 52.222176,-3.028815 52.222158,-3.028563 52.222016,-3.028462 52.221945,-3.028407 52.221881,-3.028351 52.22179,-3.028301 52.221718,-3.028203 52.221601,-3.028151 52.221552,-3.027982 52.221422,-3.027871 52.221353,-3.027755 52.221293,-3.027573 52.22121,-3.027493 52.221179,-3.027463 52.221165,-3.027147 52.221065,-3.027045 52.221039,-3.026812 52.220989,-3.026691 52.220968,-3.026602 52.220957,-3.026111 52.220898,-3.025715 52.220861,-3.025332 52.220842,-3.025011 52.220845,-3.024587 52.220849,-3.024339 52.220869,-3.024213 52.220881,-3.024103 52.220905,-3.023904 52.220966,-3.023687 52.221027,-3.023575 52.221054,-3.023409 52.221077,-3.023317 52.221072,-3.023259 52.221049,-3.023028 52.220905,-3.022859 52.220819,-3.022799 52.220774,-3.022739 52.220688)";
	static std::string const aimes15 = "LINESTRING(-3.005281 52.202464,-3.005919 52.202654,-3.006134 52.202718,-3.007138 52.202939,-3.007725 52.20309)";
	static std::string const aimes16 = "LINESTRING(-3.003371 52.210074,-3.003224 52.21001,-3.003195 52.209997,-3.003094 52.209988,-3.003007 52.209981,-3.002994 52.20998,-3.002952 52.209994,-3.002853 52.210026,-3.002667 52.210068,-3.002566 52.21008,-3.002539 52.210084,-3.002326 52.210102,-3.002318 52.210103,-3.00231 52.210104,-3.002164 52.210134,-3.001964 52.21022,-3.001792 52.210359)";
	static std::string const aimes17 = "LINESTRING(-2.968498 52.185726,-2.968594 52.185677,-2.968762 52.185642,-2.968969 52.185608,-2.969272 52.185575,-2.970091 52.185526,-2.970473 52.185443,-2.971035 52.185384,-2.971959 52.185251,-2.972754 52.185111,-2.973037 52.185044,-2.973168 52.185016,-2.973851 52.184803,-2.97401 52.184739,-2.974271 52.184602,-2.974856 52.184364,-2.97488 52.184356,-2.975153 52.184265,-2.975391 52.184212,-2.975741 52.184173,-2.97598 52.184125,-2.976288 52.184004,-2.976831 52.183673,-2.977132 52.183471,-2.977414 52.183259,-2.977612 52.183012,-2.978006 52.182385,-2.978157 52.182251,-2.97863 52.181886)";
	static std::string const aimes18 = "LINESTRING(-2.898438 52.177673,-2.897173 52.178057,-2.896867 52.178131,-2.89675 52.178159,-2.896383 52.178233,-2.896087 52.178291,-2.895818 52.178337,-2.895512 52.178384,-2.895118 52.178441,-2.894593 52.178499,-2.894339 52.178519,-2.894317 52.178514,-2.894308 52.178511,-2.894282 52.178504,-2.894217 52.178425,-2.894159 52.178354,-2.894028 52.178134,-2.894001 52.178031,-2.893855 52.177495,-2.893851 52.177478,-2.893683 52.17713,-2.893553 52.176988,-2.893238 52.176731,-2.893127 52.176628,-2.892918 52.176433,-2.892613 52.176056,-2.892122 52.175233,-2.892067 52.175113,-2.891895 52.174561,-2.891781 52.174335,-2.891634 52.174134,-2.891492 52.17394,-2.891344 52.173699,-2.891083 52.173401,-2.89083 52.173169,-2.890577 52.172964,-2.890468 52.172913,-2.890384 52.172882,-2.890276 52.172865,-2.890107 52.17286,-2.890063 52.172851,-2.889975 52.172834,-2.889808 52.17278,-2.889709 52.172725,-2.889451 52.172532,-2.88907 52.172247,-2.888619 52.171957,-2.888574 52.171916)";
	static std::string const aimes19 = "LINESTRING(-3.001027 52.270697,-3.001233 52.270714,-3.00157 52.270711,-3.001614 52.270715,-3.001658 52.270719,-3.001746 52.270736,-3.002 52.270828)";
	static std::string const aimes20 = "LINESTRING(-3.002514 52.270973,-3.002616 52.270954,-3.00282 52.270916,-3.002878 52.27088,-3.002978 52.270771,-3.003022 52.270752,-3.003066 52.270743,-3.003124 52.270734)";

	test_one<linestring, buf::join_miter, polygon>("aimes1", aimes1, 'm', 3.41515232094025e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes2", aimes2, 'm', 1.02802331980456e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes3", aimes3, 'm', 3.56112451527224e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes4", aimes4, 'm', 4.25305444196056e-008, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes5", aimes5, 'm', 3.69520876120077e-008, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes6", aimes6, 'm', 3.12580681338659e-008, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes7", aimes7, 'm', 2.64535032101776e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes8", aimes8, 'm', 1.26452604831684e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes9", aimes9, 'm', 5.62396706982327e-008, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes10", aimes10, 'm', 7.03945403301987e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes11", aimes11, 'm', 2.05895901217446e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes12", aimes12, 'm', 1.01571146160495e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes13", aimes13, 'm', 2.42559636376427e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes14", aimes14, 'm', 3.41515232094025e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes15", aimes15, 'm', 9.08697614931953e-008, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes16", aimes16, 'm', 6.0466163631645e-008, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes17", aimes17, 'm', 4.04659118657946e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes18", aimes18, 'm', 4.80904560618001e-007, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes19", aimes19, 'm', 3.57066625156222e-008, aimes_width, aimes_width);
	test_one<linestring, buf::join_miter, polygon>("aimes20", aimes20, 'm', 2.44433664420285e-008, aimes_width, aimes_width);

/* PostGIS: 
with viewy as
(
select ST_GeomFromText('LINESTRING(-3.031075 52.22385,-3.030809 52.223579,-3.030424 52.223207,-3.030212 52.222931,-3.030013 52.222764,-3.029721 52.222584,-3.029469 52.222443,-3.029218 52.222319,-3.028886 52.222187,-3.028859 52.222176,-3.028815 52.222158,-3.028563 52.222016,-3.028462 52.221945,-3.028407 52.221881,-3.028351 52.22179,-3.028301 52.221718,-3.028203 52.221601,-3.028151 52.221552,-3.027982 52.221422,-3.027871 52.221353,-3.027755 52.221293,-3.027573 52.22121,-3.027493 52.221179,-3.027463 52.221165,-3.027147 52.221065,-3.027045 52.221039,-3.026812 52.220989,-3.026691 52.220968,-3.026602 52.220957,-3.026111 52.220898,-3.025715 52.220861,-3.025332 52.220842,-3.025011 52.220845,-3.024587 52.220849,-3.024339 52.220869,-3.024213 52.220881,-3.024103 52.220905,-3.023904 52.220966,-3.023687 52.221027,-3.023575 52.221054,-3.023409 52.221077,-3.023317 52.221072,-3.023259 52.221049,-3.023028 52.220905,-3.022859 52.220819,-3.022799 52.220774,-3.022739 52.220688)',0) as p1
	, ST_GeomFromText('LINESTRING(-3.056646 52.20421,-3.055298 52.204143,-3.054991 52.204119,-3.053966 52.204125,-3.053793 52.204126)',0) as p2
	, ST_GeomFromText('LINESTRING(-3.040117 52.307787,-3.040012 52.307874,-3.03999 52.30792,-3.039986 52.307973,-3.040054 52.308123,-3.040092 52.308238,-3.040125 52.308398,-3.040165 52.308558,-3.040194 52.308587,-3.040539 52.30889,-3.040868 52.309147,-3.041196 52.309396,-3.042401 52.310114,-3.042414 52.310121,-3.042758 52.31032,-3.04277 52.310327,-3.042779 52.310353,-3.042771 52.31038,-3.042746 52.310444,-3.042627 52.310704,-3.042601 52.310799,-3.042588 52.310846,-3.042588 52.310941,-3.042596 52.311007,-3.04261 52.311035,-3.04264 52.311099,-3.042646 52.311111,-3.042662 52.311134,-3.042681 52.311161,-3.042708 52.311199,-3.042792 52.311275,-3.042808 52.31129,-3.042908 52.311364,-3.04294 52.311385,-3.042996 52.311423,-3.043232 52.311542,-3.043424 52.311632,-3.043447 52.311641,-3.043452 52.311643,-3.043537 52.311677,-3.043725 52.311728,-3.043876 52.311759,-3.044048 52.311784,-3.044325 52.311813,-3.044963 52.31187,-3.045654 52.311927,-3.046044 52.31196,-3.04609 52.311963,-3.046235 52.311979,-3.046374 52.312015,-3.046477 52.312058,-3.046587 52.312109,-3.046654 52.312147,-3.046771 52.312228,-3.046962 52.312372,-3.047043 52.312443,-3.047089 52.312498,-3.04711 52.312547,-3.047112 52.312586,-3.047113 52.312607,-3.047105 52.312666,-3.047067 52.312722)',0) as p3
	, ST_GeomFromText('LINESTRING(-3.054162 52.34456,-3.054088 52.344534,-3.054014 52.344508,-3.053926 52.34449,-3.053823 52.344491,-3.053632 52.344493,-3.053368 52.344513,-3.053149 52.344551,-3.053076 52.34457,-3.053033 52.344588,-3.053003 52.344598)',0) as p4
	, ST_GeomFromText('LINESTRING(-3.032714 52.342934,-3.032658 52.342814,-3.032617 52.342701,-3.032583 52.342606,-3.032554 52.342502,-3.032517 52.342439,-3.032453 52.34233,-3.032374 52.3422,-3.032325 52.342121,-3.032288 52.342056,-3.032252 52.342028)',0) as p5
	, ST_GeomFromText('LINESTRING(-3.041066 52.338221,-3.040547 52.33801,-3.040399 52.337939,-3.040398 52.337921,-3.040427 52.337894,-3.040456 52.337867,-3.040499 52.337849)',0) as p6
	, ST_GeomFromText('LINESTRING(-3.044091 52.565713,-3.044077 52.565734,-3.044071 52.565742,-3.044028 52.565796,-3.043992 52.565879,-3.043952 52.565959,-3.043915 52.566,-3.043857 52.56604,-3.043741 52.566113,-3.043492 52.566223,-3.042615 52.566563,-3.042425 52.566646,-3.042294 52.566683,-3.041932 52.566745,-3.041648 52.566791,-3.041445 52.566841,-3.041226 52.566924,-3.041066 52.566992,-3.04089 52.567056,-3.040865 52.56708,-3.040858 52.5671,-3.040879 52.567118,-3.040944 52.567135,-3.041034 52.567137,-3.041288 52.567141,-3.041822 52.567144,-3.042107 52.567134,-3.042245 52.567142,-3.04229 52.56715,-3.042558 52.567247,-3.042736 52.56729,-3.042918 52.567326,-3.043082 52.567338,-3.043388 52.567401,-3.043848 52.567532,-3.043908 52.567568,-3.043923 52.567577,-3.043938 52.567603,-3.04394 52.567657,-3.043941 52.567711,-3.043895 52.56798,-3.043869 52.568115,-3.043868 52.568138,-3.043869 52.568158)',0) as p7
	, ST_GeomFromText('LINESTRING(-3.039713 52.536795,-3.039808 52.536825,-3.039958 52.536844,-3.040172 52.536885,-3.040322 52.536924,-3.040476 52.536964,-3.04061 52.536989,-3.040678 52.536992,-3.040773 52.536986,-3.040949 52.536958,-3.041148 52.536905,-3.041312 52.536857,-3.041544 52.536777,-3.04173 52.536705,-3.041942 52.536634,-3.042081 52.5366,-3.042204 52.53659,-3.042324 52.536587,-3.042414 52.536591,-3.042491 52.536598,-3.042595 52.536619,-3.042767 52.536683,-3.042916 52.536749,-3.043007 52.536786,-3.043108 52.5368)',0) as p8
	, ST_GeomFromText('LINESTRING(-3.049078 52.571837,-3.049229 52.571855,-3.04964 52.571857,-3.049841 52.571858,-3.049967 52.571873,-3.05006 52.571903,-3.050094 52.571922,-3.050135 52.57194,-3.050228 52.571958,-3.05033 52.571969,-3.050391 52.571987,-3.050413 52.572027,-3.050421 52.572072,-3.050386 52.572205)',0) as p9
	, ST_GeomFromText('LINESTRING(-2.983733 52.233609,-2.98371 52.233684,-2.983736 52.233793,-2.98379 52.233906,-2.983869 52.233994,-2.984058 52.234121,-2.984225 52.234228,-2.984382 52.234341,-2.984557 52.234481,-2.984695 52.234575,-2.984716 52.234589,-2.984758 52.234617,-2.984943 52.234694,-2.984978 52.234706,-2.985031 52.234722,-2.98506 52.234731,-2.985343 52.234799,-2.985594 52.234867,-2.985755 52.234917,-2.986071 52.23497,-2.9863 52.235006,-2.986458 52.235044,-2.986602 52.235109,-2.987015 52.23534,-2.987215 52.235464,-2.987423 52.235617,-2.987637 52.235785,-2.987968 52.23602,-2.988192 52.236213,-2.988438 52.236386,-2.988512 52.236449,-2.988579 52.236505,-2.98865 52.236583,-2.98869 52.236656,-2.988721 52.236744,-2.988612 52.236848,-2.988548 52.236932,-2.988497 52.237107,-2.988432 52.237336,-2.988372 52.237567,-2.98833 52.237709,-2.988286 52.23788,-2.988233 52.23807,-2.988249 52.238145,-2.988291 52.238321,-2.988323 52.238505,-2.988379 52.238624,-2.988435 52.238727,-2.988438 52.238732,-2.988466 52.238777,-2.988485 52.238809,-2.988525 52.238854,-2.98858 52.238916,-2.988715 52.239098,-2.988806 52.239204,-2.988865 52.239275,-2.988962 52.239393,-2.989045 52.239474,-2.989244 52.239629,-2.989254 52.239636,-2.989273 52.239649,-2.989508 52.239816,-2.989534 52.239835,-2.989747 52.23998,-2.989769 52.239996,-2.98982 52.240033,-2.989843 52.240055,-2.989949 52.240152,-2.989956 52.240159,-2.99002 52.240248,-2.990074 52.240334,-2.990079 52.24034,-2.990089 52.240357,-2.990106 52.240401,-2.99015 52.240512,-2.990268 52.240793,-2.990336 52.24095,-2.990425 52.241123,-2.990472 52.241198,-2.990617 52.241383,-2.990723 52.241527,-2.990784 52.241601,-2.990811 52.241633,-2.990859 52.241753,-2.990892 52.241854,-2.99094 52.241995,-2.990947 52.242025,-2.990958 52.24207,-2.991023 52.242175,-2.991122 52.242333,-2.991202 52.242485,-2.991276 52.242624,-2.991408 52.242809,-2.991639 52.2431,-2.99172 52.243202,-2.991734 52.243222,-2.991897 52.243461,-2.991944 52.243547,-2.992072 52.243781,-2.992267 52.244138,-2.992431 52.244408,-2.992503 52.244532,-2.992657 52.244802,-2.992761 52.245063,-2.9928 52.245305,-2.992813 52.245622,-2.992822 52.245701,-2.99281 52.245784,-2.992793 52.245867,-2.992771 52.245927,-2.992736 52.245978,-2.992646 52.246076,-2.992573 52.246144,-2.992455 52.246276,-2.992316 52.246468,-2.992432 52.246557,-2.99253 52.246568,-2.992901 52.246565,-2.993581 52.246527,-2.993926 52.246509,-2.994191 52.246514)',0) as p10
	, ST_GeomFromText('LINESTRING(-3.012501 52.228425,-3.012694 52.22837,-3.012848 52.228308,-3.012946 52.22824,-3.013055 52.228146,-3.013123 52.228048,-3.013182 52.227904,-3.013242 52.227742,-3.013269 52.227632,-3.01327 52.227553,-3.013245 52.227479,-3.013151 52.227332,-3.013072 52.227223,-3.012938 52.227095,-3.012779 52.226936,-3.01271 52.226867,-3.012639 52.226817,-3.012452 52.226743,-3.012278 52.226685,-3.01201 52.226616,-3.011707 52.226519,-3.011541 52.226458,-3.011388 52.22638,-3.011334 52.226352,-3.011111 52.226206,-3.010983 52.226107,-3.010893 52.226012,-3.010832 52.225905,-3.010825 52.225845,-3.010841 52.225687,-3.010858 52.225516,-3.010857 52.225451,-3.010844 52.225405,-3.010747 52.225247,-3.01062 52.225123,-3.010467 52.224893,-3.010372 52.224658)',0) as p11
	, ST_GeomFromText('LINESTRING(-2.998218 52.230123,-2.998074 52.230203,-2.998066 52.230206,-2.997957 52.230258,-2.997863 52.230302,-2.997049 52.230561,-2.996351 52.230783,-2.995741 52.230976,-2.995552 52.231032)',0) as p12
	, ST_GeomFromText('LINESTRING(-3.005291 52.231131,-3.005218 52.2312,-3.005076 52.23132,-3.004901 52.231443,-3.004725 52.231551,-3.004586 52.231621,-3.004439 52.231667,-3.004381 52.231685,-3.004236 52.231693,-3.004106 52.231684,-3.003877 52.231653,-3.003705 52.231618,-3.003613 52.231587,-3.003529 52.231549,-3.003019 52.231279,-3.002922 52.231244,-3.002789 52.231196,-3.002657 52.231144,-3.002492 52.231112,-3.002372 52.231091,-3.002239 52.231073,-3.001708 52.231062,-3.001596 52.231053,-3.000959 52.230853,-3.000714 52.230758,-3.00005 52.2305,-2.999016 52.230131,-2.999001 52.230122)',0) as p13
	, ST_GeomFromText('LINESTRING(-3.031075 52.22385,-3.030809 52.223579,-3.030424 52.223207,-3.030212 52.222931,-3.030013 52.222764,-3.029721 52.222584,-3.029469 52.222443,-3.029218 52.222319,-3.028886 52.222187,-3.028859 52.222176,-3.028815 52.222158,-3.028563 52.222016,-3.028462 52.221945,-3.028407 52.221881,-3.028351 52.22179,-3.028301 52.221718,-3.028203 52.221601,-3.028151 52.221552,-3.027982 52.221422,-3.027871 52.221353,-3.027755 52.221293,-3.027573 52.22121,-3.027493 52.221179,-3.027463 52.221165,-3.027147 52.221065,-3.027045 52.221039,-3.026812 52.220989,-3.026691 52.220968,-3.026602 52.220957,-3.026111 52.220898,-3.025715 52.220861,-3.025332 52.220842,-3.025011 52.220845,-3.024587 52.220849,-3.024339 52.220869,-3.024213 52.220881,-3.024103 52.220905,-3.023904 52.220966,-3.023687 52.221027,-3.023575 52.221054,-3.023409 52.221077,-3.023317 52.221072,-3.023259 52.221049,-3.023028 52.220905,-3.022859 52.220819,-3.022799 52.220774,-3.022739 52.220688)',0) as p14
	, ST_GeomFromText('LINESTRING(-3.005281 52.202464,-3.005919 52.202654,-3.006134 52.202718,-3.007138 52.202939,-3.007725 52.20309)',0) as p15
	, ST_GeomFromText('LINESTRING(-3.003371 52.210074,-3.003224 52.21001,-3.003195 52.209997,-3.003094 52.209988,-3.003007 52.209981,-3.002994 52.20998,-3.002952 52.209994,-3.002853 52.210026,-3.002667 52.210068,-3.002566 52.21008,-3.002539 52.210084,-3.002326 52.210102,-3.002318 52.210103,-3.00231 52.210104,-3.002164 52.210134,-3.001964 52.21022,-3.001792 52.210359)',0) as p16
	, ST_GeomFromText('LINESTRING(-2.968498 52.185726,-2.968594 52.185677,-2.968762 52.185642,-2.968969 52.185608,-2.969272 52.185575,-2.970091 52.185526,-2.970473 52.185443,-2.971035 52.185384,-2.971959 52.185251,-2.972754 52.185111,-2.973037 52.185044,-2.973168 52.185016,-2.973851 52.184803,-2.97401 52.184739,-2.974271 52.184602,-2.974856 52.184364,-2.97488 52.184356,-2.975153 52.184265,-2.975391 52.184212,-2.975741 52.184173,-2.97598 52.184125,-2.976288 52.184004,-2.976831 52.183673,-2.977132 52.183471,-2.977414 52.183259,-2.977612 52.183012,-2.978006 52.182385,-2.978157 52.182251,-2.97863 52.181886)',0) as p17
	, ST_GeomFromText('LINESTRING(-2.898438 52.177673,-2.897173 52.178057,-2.896867 52.178131,-2.89675 52.178159,-2.896383 52.178233,-2.896087 52.178291,-2.895818 52.178337,-2.895512 52.178384,-2.895118 52.178441,-2.894593 52.178499,-2.894339 52.178519,-2.894317 52.178514,-2.894308 52.178511,-2.894282 52.178504,-2.894217 52.178425,-2.894159 52.178354,-2.894028 52.178134,-2.894001 52.178031,-2.893855 52.177495,-2.893851 52.177478,-2.893683 52.17713,-2.893553 52.176988,-2.893238 52.176731,-2.893127 52.176628,-2.892918 52.176433,-2.892613 52.176056,-2.892122 52.175233,-2.892067 52.175113,-2.891895 52.174561,-2.891781 52.174335,-2.891634 52.174134,-2.891492 52.17394,-2.891344 52.173699,-2.891083 52.173401,-2.89083 52.173169,-2.890577 52.172964,-2.890468 52.172913,-2.890384 52.172882,-2.890276 52.172865,-2.890107 52.17286,-2.890063 52.172851,-2.889975 52.172834,-2.889808 52.17278,-2.889709 52.172725,-2.889451 52.172532,-2.88907 52.172247,-2.888619 52.171957,-2.888574 52.171916)',0) as p18
	, ST_GeomFromText('LINESTRING(-3.001027 52.270697,-3.001233 52.270714,-3.00157 52.270711,-3.001614 52.270715,-3.001658 52.270719,-3.001746 52.270736,-3.002 52.270828)',0) as p19
	, ST_GeomFromText('LINESTRING(-3.002514 52.270973,-3.002616 52.270954,-3.00282 52.270916,-3.002878 52.27088,-3.002978 52.270771,-3.003022 52.270752,-3.003066 52.270743,-3.003124 52.270734)',0) as p20

)

select ST_Area(ST_Buffer(p1, 0.000018, 'endcap=flat join=miter')) as a1
, ST_Area(ST_Buffer(p2, 0.000018, 'endcap=flat join=miter')) as a2
, ST_Area(ST_Buffer(p3, 0.000018, 'endcap=flat join=miter')) as a3
, ST_Area(ST_Buffer(p4, 0.000018, 'endcap=flat join=miter')) as a4
, ST_Area(ST_Buffer(p5, 0.000018, 'endcap=flat join=miter')) as a5
, ST_Area(ST_Buffer(p6, 0.000018, 'endcap=flat join=miter')) as a6
, ST_Area(ST_Buffer(p7, 0.000018, 'endcap=flat join=miter')) as a7
, ST_Area(ST_Buffer(p8, 0.000018, 'endcap=flat join=miter')) as a8
, ST_Area(ST_Buffer(p9, 0.000018, 'endcap=flat join=miter')) as a9
, ST_Area(ST_Buffer(p10, 0.000018, 'endcap=flat join=miter')) as a10
, ST_Area(ST_Buffer(p11, 0.000018, 'endcap=flat join=miter')) as a11
, ST_Area(ST_Buffer(p12, 0.000018, 'endcap=flat join=miter')) as a12
, ST_Area(ST_Buffer(p13, 0.000018, 'endcap=flat join=miter')) as a13
, ST_Area(ST_Buffer(p14, 0.000018, 'endcap=flat join=miter')) as a14
, ST_Area(ST_Buffer(p15, 0.000018, 'endcap=flat join=miter')) as a15
, ST_Area(ST_Buffer(p16, 0.000018, 'endcap=flat join=miter')) as a16
, ST_Area(ST_Buffer(p17, 0.000018, 'endcap=flat join=miter')) as a17
, ST_Area(ST_Buffer(p18, 0.000018, 'endcap=flat join=miter')) as a18
, ST_Area(ST_Buffer(p19, 0.000018, 'endcap=flat join=miter')) as a19
, ST_Area(ST_Buffer(p20, 0.000018, 'endcap=flat join=miter')) as a20
from viewy
*/
}


template <typename P>
void test_all()
{
    namespace buf = bg::strategy::buffer;
    typedef bg::model::linestring<P> linestring;
    typedef bg::model::polygon<P> polygon;

    test_one<linestring, buf::join_round, polygon>("simplex", simplex, 'r', 19.209, 1.5, 1.5);
    test_one<linestring, buf::join_miter, polygon>("simplex", simplex, 'm', 19.209, 1.5, 1.5);

    test_one<linestring, buf::join_miter, polygon>("simplex_asym_neg", simplex, 'm', 3.202, +1.5, -1.0);
    test_one<linestring, buf::join_miter, polygon>("simplex_asym_pos", simplex, 'm', 3.202, -1.0, +1.5);

	// Generates a reverse polygon, which will be empty TODO decide about this
    // test_one<linestring, buf::join_miter, polygon>("simplex_asym_pos", simplex, 'm', 0, -1.5, +1.0); 

    //test_one<linestring, buf::join_round, polygon>("straight", straight, 'r', 19.2093727122985, 1.5, 1.5);
    //test_one<linestring, buf::join_miter, polygon>("straight", straight, 'm', 19.2093727122985, 1.5, 1.5);

    test_one<linestring, buf::join_round, polygon>("one_bend", one_bend, 'r', 28.488, 1.5, 1.5);
    test_one<linestring, buf::join_miter, polygon>("one_bend", one_bend, 'm', 28.696, 1.5, 1.5);

    test_one<linestring, buf::join_round, polygon>("two_bends", two_bends, 'r', 39.222, 1.5, 1.5);
    test_one<linestring, buf::join_miter, polygon>("two_bends", two_bends, 'm', 39.513, 1.5, 1.5);
    test_one<linestring, buf::join_round, polygon>("two_bends_left", two_bends, 'r', 20.028, 1.5, 0.0);
    test_one<linestring, buf::join_miter, polygon>("two_bends_left", two_bends, 'm', 20.225, 1.5, 0.0);
    test_one<linestring, buf::join_round, polygon>("two_bends_right", two_bends, 'r', 19.211, 0.0, 1.5);
    test_one<linestring, buf::join_miter, polygon>("two_bends_right", two_bends, 'm', 19.288, 0.0, 1.5);


	// Next (and all similar cases) which a offsetted-one-sided buffer has to be fixed. TODO
    //test_one<linestring, buf::join_miter, polygon>("two_bends_neg", two_bends, 'm', 99, +1.5, -1.0);
    //test_one<linestring, buf::join_miter, polygon>("two_bends_pos", two_bends, 'm', 99, -1.5, +1.0);
    //test_one<linestring, buf::join_round, polygon>("two_bends_neg", two_bends, 'r', 99, +1.5, -1.0);
    //test_one<linestring, buf::join_round, polygon>("two_bends_pos", two_bends, 'r', 99, -1.5, +1.0);

    test_one<linestring, buf::join_round, polygon>("overlapping150", overlapping, 'r', 65.646, 1.5, 1.5);
    test_one<linestring, buf::join_miter, polygon>("overlapping150", overlapping, 'm', 68.140, 1.5, 1.5);
	// Different cases with intersection points on flat and (left/right from line itself)
    test_one<linestring, buf::join_round, polygon>("overlapping_asym_150_010", overlapping, 'r', 48.308, 1.5, 0.25);
    test_one<linestring, buf::join_miter, polygon>("overlapping_asym_150_010", overlapping, 'm', 50.770, 1.5, 0.25);
    test_one<linestring, buf::join_round, polygon>("overlapping_asym_150_075", overlapping, 'r', 58.506, 1.5, 0.75);
    test_one<linestring, buf::join_miter, polygon>("overlapping_asym_150_075", overlapping, 'm', 60.985, 1.5, 0.75);
    test_one<linestring, buf::join_round, polygon>("overlapping_asym_150_100", overlapping, 'r', 62.514, 1.5, 1.0);
    test_one<linestring, buf::join_miter, polygon>("overlapping_asym_150_100", overlapping, 'm', 64.984, 1.5, 1.0);

    test_one<linestring, buf::join_round, polygon>("for_collinear", for_collinear, 'r', 68.561, 2.0, 2.0);
    test_one<linestring, buf::join_miter, polygon>("for_collinear", for_collinear, 'm', 72, 2.0, 2.0);
    test_one<linestring, buf::join_round, polygon>("for_collinear2", for_collinear2, 'r', 74.387, 2.0, 2.0);
    test_one<linestring, buf::join_miter, polygon>("for_collinear2", for_collinear2, 'm', 78.0, 2.0, 2.0);

    //test_one<linestring, buf::join_round, polygon>("curve", curve, 'r', 99, 5.0, 3.0);
    //test_one<linestring, buf::join_miter, polygon>("curve", curve, 'm', 99, 5.0, 3.0);

    test_one<linestring, buf::join_round, polygon>("chained2", chained2, 'r', 11.3137, 2.5, 1.5);
    test_one<linestring, buf::join_round, polygon>("chained3", chained3, 'r', 16.9706, 2.5, 1.5);
    test_one<linestring, buf::join_round, polygon>("chained4", chained4, 'r', 22.6274, 2.5, 1.5);

    //test_one<linestring, buf::join_round, polygon>("reallife1", reallife1, 'r', 99, 16.5, 6.5);
}


//#define HAVE_TTMATH
#ifdef HAVE_TTMATH
#include <ttmath_stub.hpp>
#endif


int test_main(int, char* [])
{
    test_all<bg::model::point<double, 2, bg::cs::cartesian> >();
    //test_all<bg::model::point<tt, 2, bg::cs::cartesian> >();

	// test_aimes<bg::model::point<double, 2, bg::cs::cartesian> >();

    return 0;
}
