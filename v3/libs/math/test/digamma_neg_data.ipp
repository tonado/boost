//  (C) Copyright John Maddock 2006-7.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
   static const boost::array<boost::array<T, 2>, 200> digamma_neg_data = { {
      { SC_(-99.7181549072265625), SC_(2.03892909952497242038694056382195623059) }, 
      { SC_(-99.5216522216796875), SC_(4.3913617771941913117971009646096341326) }, 
      { SC_(-98.80979156494140625), SC_(-0.01795976054399933340057713377738267488686) }, 
      { SC_(-98.646087646484375), SC_(3.044214979883807667874683391024711390163) }, 
      { SC_(-98.2226104736328125), SC_(8.327061837969621994688065941368639650164) }, 
      { SC_(-96.81671905517578125), SC_(-0.2613967795671165924570265553692310141858) }, 
      { SC_(-96.555389404296875), SC_(4.023029392905834803729410679283679163172) }, 
      { SC_(-96.4288330078125), SC_(5.288313818176304987255615817663265383342) }, 
      { SC_(-96.35587310791015625), SC_(6.101607804168127653000043332720050192048) }, 
      { SC_(-95.3828582763671875), SC_(5.774458772879798661504785984613127024683) }, 
      { SC_(-94.87835693359375), SC_(-3.258775525008949657912384482006797161857) }, 
      { SC_(-94.60498809814453125), SC_(3.479509682356698077739870471765368709254) }, 
      { SC_(-93.2404632568359375), SC_(7.876249181059212250961342610258680843403) }, 
      { SC_(-92.41457366943359375), SC_(5.395652110469876120841383508575006667739) }, 
      { SC_(-90.2868194580078125), SC_(6.996060449908181672759658412431764721268) }, 
      { SC_(-90.2459564208984375), SC_(7.730510069525265980585453066651639424664) }, 
      { SC_(-89.013824462890625), SC_(76.7844527353150160809893161375899712728) }, 
      { SC_(-88.75354766845703125), SC_(1.279073328662160048752885420029490083884) }, 
      { SC_(-88.1002349853515625), SC_(14.12873575145206447240140060245136812233) }, 
      { SC_(-87.481719970703125), SC_(4.657749706875254767656016829355880725463) }, 
      { SC_(-87.41033935546875), SC_(5.385408972387998240975025273868320766616) }, 
      { SC_(-87.3013153076171875), SC_(6.737807742019520253440825060046509923858) }, 
      { SC_(-87.00937652587890625), SC_(111.0902104512757743601088229496385482381) }, 
      { SC_(-86.452301025390625), SC_(4.939690918031593302255270591348782158549) }, 
      { SC_(-86.1375579833984375), SC_(11.27311653802311845949475246171079728424) }, 
      { SC_(-85.81136322021484375), SC_(-0.2076024359899236820459284405564934682571) }, 
      { SC_(-85.08860015869140625), SC_(15.4432197184156901762673734012738165669) }, 
      { SC_(-85.0706024169921875), SC_(18.38013049788840734996810404641615698096) }, 
      { SC_(-84.55615997314453125), SC_(3.88321705564237834505242432670033691134) }, 
      { SC_(-84.2386932373046875), SC_(7.812678135978754145263274987740845778654) }, 
      { SC_(-84.194244384765625), SC_(8.931719867523162463837746400725488234199) }, 
      { SC_(-83.73882293701171875), SC_(1.505295935399338449868426369185929751229) }, 
      { SC_(-82.8813323974609375), SC_(-3.609402776980612588036128298328365683225) }, 
      { SC_(-82.6134796142578125), SC_(3.250221400281531800616333769504919375611) }, 
      { SC_(-81.31273651123046875), SC_(6.500209496762236219781012282892172792378) }, 
      { SC_(-81.16180419921875), SC_(10.04118609757139096209050165941519368701) }, 
      { SC_(-80.34047698974609375), SC_(6.113463877628090613614429157059502101004) }, 
      { SC_(-79.1931915283203125), SC_(8.902649813785723219406837941195708301933) }, 
      { SC_(-78.979095458984375), SC_(-43.39220275151493342151673584769713674459) }, 
      { SC_(-78.80756378173828125), SC_(-0.1741139524199893908631289936754328553595) }, 
      { SC_(-78.276214599609375), SC_(7.029126641974004636072711755974329319293) }, 
      { SC_(-77.8965911865234375), SC_(-4.965949067939776233732103672063117487233) }, 
      { SC_(-77.618804931640625), SC_(3.128016725588726483845498994960937165101) }, 
      { SC_(-76.9843902587890625), SC_(-59.66111542957708160367279993835116598861) }, 
      { SC_(-75.6475067138671875), SC_(2.762831843674791808646068344156198358292) }, 
      { SC_(-74.8916168212890625), SC_(-4.544467476010996269925717795920967036228) }, 
      { SC_(-74.57178497314453125), SC_(3.59770242115359475843120457380803078855) }, 
      { SC_(-74.49048614501953125), SC_(4.411294618813885078478516140128263239646) }, 
      { SC_(-74.249176025390625), SC_(7.472045154786435852081961669032387751756) }, 
      { SC_(-72.397491455078125), SC_(5.337270392731504876256252154330660996853) }, 
      { SC_(-72.30770111083984375), SC_(6.45618615562028783175344592523791857763) }, 
      { SC_(-72.150177001953125), SC_(10.44291983579332484719137950538141198138) }, 
      { SC_(-71.41609954833984375), SC_(5.123297394484169650507849264219211277233) }, 
      { SC_(-70.29705810546875), SC_(6.586989653308987758464004111780026164342) }, 
      { SC_(-70.0168304443359375), SC_(63.61661733083149880967450730403400964169) }, 
      { SC_(-69.808685302734375), SC_(-0.3289873714853887848889508326638941457588) }, 
      { SC_(-69.18329620361328125), SC_(9.082833286105283040745693219253094753599) }, 
      { SC_(-68.3449554443359375), SC_(5.895816498784197711310887233679346985484) }, 
      { SC_(-68.2900543212890625), SC_(6.667078439728477642775937857513723712022) }, 
      { SC_(-66.7551727294921875), SC_(0.9631089274457797371832913555201378277263) }, 
      { SC_(-66.2877349853515625), SC_(6.674402390903648107264524741943941318692) }, 
      { SC_(-65.96142578125), SC_(-21.60039097244717646250068532377671067773) }, 
      { SC_(-65.00162506103515625), SC_(619.5382407651047414044960295217348989637) }, 
      { SC_(-64.83405303955078125), SC_(-1.290399564904368875515657543138626740639) }, 
      { SC_(-64.72376251220703125), SC_(1.515712957371258935902920146011383158566) }, 
      { SC_(-64.36548614501953125), SC_(5.585036804770642096748234463405359373268) }, 
      { SC_(-63.870601654052734375), SC_(-3.132938413135016068514633000959251655011) }, 
      { SC_(-61.955417633056640625), SC_(-18.14906679867205714676685355624200206247) }, 
      { SC_(-61.84415435791015625), SC_(-1.762828050189299963414925510084105410157) }, 
      { SC_(-61.27040863037109375), SC_(6.885962246437210164247050669769241762116) }, 
      { SC_(-61.1430206298828125), SC_(10.63639337906714605332887925207272339472) }, 
      { SC_(-60.7772979736328125), SC_(0.3828568174285943765552196371752184023788) }, 
      { SC_(-60.767955780029296875), SC_(0.597630996899231770205669545427601282449) }, 
      { SC_(-60.50917816162109375), SC_(4.020425571512186535251807369058720729888) }, 
      { SC_(-60.126148223876953125), SC_(11.61249771821316227897371950731036451444) }, 
      { SC_(-59.5791473388671875), SC_(3.298014524278177804824357796228486139154) }, 
      { SC_(-59.12688446044921875), SC_(11.54738093463764209365528407465938463008) }, 
      { SC_(-58.733348846435546875), SC_(1.252516603233681404470535780337376850768) }, 
      { SC_(-57.823871612548828125), SC_(-1.020030805813282973065541328061417103161) }, 
      { SC_(-57.791233062744140625), SC_(-0.01721196139850082730515140522511875898686) }, 
      { SC_(-57.6834869384765625), SC_(2.021283718214246528807976825874750730944) }, 
      { SC_(-56.1255645751953125), SC_(11.58306242149007962260737588966574096145) }, 
      { SC_(-55.441379547119140625), SC_(4.6095090326877333856682583039277727222) }, 
      { SC_(-54.396717071533203125), SC_(5.062176320861478792112156635998489324447) }, 
      { SC_(-54.201084136962890625), SC_(8.295102791777500360299701370233860660785) }, 
      { SC_(-53.06093597412109375), SC_(20.19053903029668873838826742013474757079) }, 
      { SC_(-52.671115875244140625), SC_(2.10070123867348232297487204513445237558) }, 
      { SC_(-52.52413177490234375), SC_(3.73213352669881119442059765740051858078) }, 
      { SC_(-51.462436676025390625), SC_(4.323002262954298089562762598650242024201) }, 
      { SC_(-51.243106842041015625), SC_(7.226998934762993806818100988804419542686) }, 
      { SC_(-51.0235595703125), SC_(46.31011451882384360024092555028601375848) }, 
      { SC_(-50.94109344482421875), SC_(-12.8413475810657768775226051735008739843) }, 
      { SC_(-50.163593292236328125), SC_(9.490021355163248564818621481640556958139) }, 
      { SC_(-49.63372802734375), SC_(2.511313156813305996562305257857390789172) }, 
      { SC_(-49.404293060302734375), SC_(4.884246649262366888920411419024147958277) }, 
      { SC_(-47.262851715087890625), SC_(6.763893885944739117755918311467424143999) }, 
      { SC_(-46.920246124267578125), SC_(-8.416023355487745311972990967262253971971) }, 
      { SC_(-45.31185150146484375), SC_(5.932978383248106730387626359282358526882) }, 
      { SC_(-45.278446197509765625), SC_(6.448720313065481170040935613035156187478) }, 
      { SC_(-45.277942657470703125), SC_(6.457159638091017467737698925977740795224) }, 
      { SC_(-45.027637481689453125), SC_(39.91011698129930359222267887372752760268) }, 
      { SC_(-43.8442535400390625), SC_(-2.107933118887280485309161602735949216416) }, 
      { SC_(-43.217838287353515625), SC_(7.628256500265451720152832482834432955622) }, 
      { SC_(-43.11763763427734375), SC_(11.88558178274471415613706962118145261485) }, 
      { SC_(-42.62453460693359375), SC_(2.468202270232387819257535057032048703186) }, 
      { SC_(-41.904331207275390625), SC_(-6.388806142931646581376930809159704270731) }, 
      { SC_(-41.473590850830078125), SC_(3.998311840817845119612809226760453214553) }, 
      { SC_(-41.47322845458984375), SC_(4.001904991009103072678533254287758877523) }, 
      { SC_(-40.71761322021484375), SC_(1.159373041348507101344016731863232548419) }, 
      { SC_(-40.549640655517578125), SC_(3.220862320117017004190871786172571326259) }, 
      { SC_(-38.39553070068359375), SC_(4.73066601123284545617603996389975515062) }, 
      { SC_(-36.764072418212890625), SC_(0.185641778964637473051349775352193113594) }, 
      { SC_(-36.0236663818359375), SC_(45.77413334958292444754118070178767678465) }, 
      { SC_(-35.60390472412109375), SC_(2.522888342129267961559021180379112152327) }, 
      { SC_(-35.3686981201171875), SC_(4.954668236416411879662604875880175363163) }, 
      { SC_(-34.49019622802734375), SC_(3.651891900429621653936520779034983699363) }, 
      { SC_(-34.452213287353515625), SC_(4.029227315027716909301715786442880409125) }, 
      { SC_(-34.425933837890625), SC_(4.297752459205291355542460490934969321341) }, 
      { SC_(-33.6394500732421875), SC_(2.05879207452756962707410762095930722907) }, 
      { SC_(-32.12648773193359375), SC_(10.97049318285659411778704721045579811462) }, 
      { SC_(-32.02973175048828125), SC_(37.01840088144976405978561031600232101035) }, 
      { SC_(-32.018024444580078125), SC_(58.90273484616011864735901401561254400453) }, 
      { SC_(-31.864048004150390625), SC_(-3.425649819825478548122204473279221831269) }, 
      { SC_(-30.5171375274658203125), SC_(3.265278944239551980140922031510366995665) }, 
      { SC_(-30.4767131805419921875), SC_(3.663521748406629288062646856398131484428) }, 
      { SC_(-30.0923290252685546875), SC_(13.94615623704883065583726298948748620016) }, 
      { SC_(-29.4225749969482421875), SC_(4.178249593518503446782790798157935603259) }, 
      { SC_(-29.395389556884765625), SC_(4.46906479672600980848725781028158753644) }, 
      { SC_(-29.0635166168212890625), SC_(18.92098221867999526957587349227217750794) }, 
      { SC_(-28.929615020751953125), SC_(-10.59321162183541691346972253310776238772) }, 
      { SC_(-27.416103363037109375), SC_(4.17700594969969944426868830026842791532) }, 
      { SC_(-27.304531097412109375), SC_(5.54012214812515808522852628894283103891) }, 
      { SC_(-26.72013092041015625), SC_(0.7028843790088342764015208907756718567463) }, 
      { SC_(-25.9352741241455078125), SC_(-11.9614838597048426449728575053473182996) }, 
      { SC_(-25.6867523193359375), SC_(1.176826484915453835886205411786243733377) }, 
      { SC_(-24.8732929229736328125), SC_(-4.237136620209855403786484087613189060887) }, 
      { SC_(-24.6270904541015625), SC_(1.898484134239669451822687252748496404653) }, 
      { SC_(-24.53133392333984375), SC_(2.909938662308256016840011167255882888239) }, 
      { SC_(-24.27997589111328125), SC_(5.809456022692943160671997012588234680914) }, 
      { SC_(-24.225986480712890625), SC_(6.863255301509444395634121067991546441736) }, 
      { SC_(-23.8268795013427734375), SC_(-2.003570170516321537241642099057200674105) }, 
      { SC_(-23.6249980926513671875), SC_(1.882051906931976769141489221324624486755) }, 
      { SC_(-23.4483184814453125), SC_(3.690576780901990543590973501647480396476) }, 
      { SC_(-22.6082859039306640625), SC_(2.02829972990549794339388303789633983053) }, 
      { SC_(-22.1102294921875), SC_(11.82489297106174961055859856595077767589) }, 
      { SC_(-22.0832767486572265625), SC_(14.85021720005861723216276067526099288351) }, 
      { SC_(-20.779270172119140625), SC_(-0.7220286362185610999510531120120858367544) }, 
      { SC_(-20.6402416229248046875), SC_(1.570036502538595917265616936047416332984) }, 
      { SC_(-20.60250091552734375), SC_(2.001360446832507792499854867762750216251) }, 
      { SC_(-20.480007171630859375), SC_(3.241245764823419396963096620359809656269) }, 
      { SC_(-20.27201080322265625), SC_(5.768313318300353594949489118483966781535) }, 
      { SC_(-20.2071361541748046875), SC_(7.156819372589727235791276774661186960423) }, 
      { SC_(-20.1894168853759765625), SC_(7.670698039778866912498508209079922928436) }, 
      { SC_(-19.971954345703125), SC_(-32.54467844963252077717911577731405834236) }, 
      { SC_(-19.2469005584716796875), SC_(6.186480084023335387087272289090747328989) }, 
      { SC_(-19.1824493408203125), SC_(7.847002126080603757397510906504995585177) }, 
      { SC_(-19.0265483856201171875), SC_(40.55157678949956103482720787629324186475) }, 
      { SC_(-18.5715198516845703125), SC_(2.230314699128516174606595799951710607353) }, 
      { SC_(-18.5276336669921875), SC_(2.672586759609702793340088983364248246452) }, 
      { SC_(-17.9159221649169921875), SC_(-8.702502484177860216144583065634001249363) }, 
      { SC_(-17.8754024505615234375), SC_(-4.700545068020732462956282791568579773556) }, 
      { SC_(-17.8096714019775390625), SC_(-1.704910040367315348529245955199242558161) }, 
      { SC_(-17.654216766357421875), SC_(1.245525979337004465966586338242325823861) }, 
      { SC_(-16.9171390533447265625), SC_(-8.936978387990812220163335657856867859665) }, 
      { SC_(-16.4991436004638671875), SC_(2.841759414051282631909664692084257003501) }, 
      { SC_(-15.9282741546630859375), SC_(-10.90604642089629536174323573315025319791) }, 
      { SC_(-15.1532230377197265625), SC_(8.7652341843566231062515337784346464505) }, 
      { SC_(-15.0870685577392578125), SC_(13.94393218121241823357181745576517766981) }, 
      { SC_(-12.7571163177490234375), SC_(-0.7005285484888729861556721133426365160483) }, 
      { SC_(-12.32425594329833984375), SC_(4.486877647992933354241229013873681135288) }, 
      { SC_(-12.15693378448486328125), SC_(8.38572290746722802141392801697704201607) }, 
      { SC_(-10.90967655181884765625), SC_(-8.337781110433724243601733849861559188948) }, 
      { SC_(-9.42080593109130859375), SC_(3.093207549574642835823398326728433769791) }, 
      { SC_(-9.26352691650390625), SC_(5.164408378967561819557747661923746174341) }, 
      { SC_(-8.66241455078125), SC_(0.4573363030846697184753722937434504783991) }, 
      { SC_(-8.42644596099853515625), SC_(2.928692490941547323611393715468003596111) }, 
      { SC_(-8.28063488006591796875), SC_(4.761502420123261711232867499795716416469) }, 
      { SC_(-7.91252231597900390625), SC_(-9.011928670506620642435146780806294048492) }, 
      { SC_(-7.073635101318359375), SC_(15.36275862317279916670563199810883722471) }, 
      { SC_(-6.600677967071533203125), SC_(0.9328456790163266794082601288835098544981) }, 
      { SC_(-6.59893131256103515625), SC_(0.9516512759861559819594253210068193102636) }, 
      { SC_(-5.9925975799560546875), SC_(-133.1949549912806814878528595454864731658) }, 
      { SC_(-4.9777927398681640625), SC_(-43.25515309581336581959034619574585012987) }, 
      { SC_(-4.4982433319091796875), SC_(1.628080704179537179987368246302687190569) }, 
      { SC_(-4.28330326080322265625), SC_(4.11144188155776750659420254650051070116) }, 
      { SC_(-4.24931621551513671875), SC_(4.714952991490022604296929488005221371491) }, 
      { SC_(-4.0708599090576171875), SC_(15.40013806764590763994801524236096523633) }, 
      { SC_(-4.05075550079345703125), SC_(21.05232398567253746583677948874966443745) }, 
      { SC_(-4.0256023406982421875), SC_(40.48643215403911174179393965158605983731) }, 
      { SC_(-3.5111486911773681640625), SC_(1.281561880048097406180157051598176902444) }, 
      { SC_(-3.5033643245697021484375), SC_(1.356501581084328750160720447675663936527) }, 
      { SC_(-3.2305061817169189453125), SC_(4.871536212360443209892217244039906620205) }, 
      { SC_(-3.1132221221923828125), SC_(9.744280495465310545518498968361342433867) }, 
      { SC_(-2.9407203197479248046875), SC_(-15.4345615314581643435197891745342605011) }, 
      { SC_(-1.8890321254730224609375), SC_(-7.765572445501014244464453609042267110197) }, 
      { SC_(-1.2540400028228759765625), SC_(3.637637081727820317005755368640064000683) }, 
      { SC_(-1.1478424072265625), SC_(6.784401313345897445876900487443690652634) }, 
      { SC_(-0.7118701934814453125), SC_(-2.248647256037530489173954985789306442878) }, 
      { SC_(-0.593149662017822265625), SC_(-0.8263747473361177168793211424797478169291) }, 
      { SC_(-0.3538668155670166015625), SC_(1.443172070386911802413212733069332368731) }
   } };
#undef SC_


