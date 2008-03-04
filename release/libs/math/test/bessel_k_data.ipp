//  Copyright (c) 2007 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
   static const boost::array<boost::array<T, 3>, 263> bessel_k_data = {
      SC_(-0.8049192047119140625e2), SC_(0.24750102996826171875e2), SC_(0.6579017807810652710369517871806355927214e29), 
      SC_(-0.8049192047119140625e2), SC_(0.637722015380859375e2), SC_(0.2395518238062557960566710371847643552469e-8), 
      SC_(-0.8049192047119140625e2), SC_(0.1252804412841796875e3), SC_(0.3069043255911758700865294859650240330974e-44), 
      SC_(-0.8049192047119140625e2), SC_(0.25554705810546875e3), SC_(0.2303430936664631154413247069375132759954e-106), 
      SC_(-0.8049192047119140625e2), SC_(0.503011474609375e3), SC_(0.1203148508747254149682895744594491240807e-216), 
      SC_(-0.8049192047119140625e2), SC_(0.10074598388671875e4), SC_(0.2865368119939400701179862849573503322931e-437), 
      SC_(-0.8049192047119140625e2), SC_(0.1185395751953125e4), SC_(0.8632633219300624004437758135158135952472e-515), 
      SC_(-0.8049192047119140625e2), SC_(0.353451806640625e4), SC_(0.5013665804582944405266048580134316878986e-1536), 
      SC_(-0.8049192047119140625e2), SC_(0.80715478515625e4), SC_(0.7765547631230743133384730763696548377855e-3507), 
      SC_(-0.8049192047119140625e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.639546878366615050472401588575857541732e-7050)), 
      SC_(-0.8049192047119140625e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5074028894875745794984647078151040612894e-13928)), 
      SC_(-0.8049192047119140625e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2862328185162412476566225413964872968853e-15796)), 
      SC_(-0.7460263824462890625e2), SC_(0.24750102996826171875e2), SC_(0.1194046640827563151857444163209777353211e25), 
      SC_(-0.7460263824462890625e2), SC_(0.637722015380859375e2), SC_(0.5818966684329205041972653154218173748165e-11), 
      SC_(-0.7460263824462890625e2), SC_(0.1252804412841796875e3), SC_(0.9892143938422535628101195141323126645363e-46), 
      SC_(-0.7460263824462890625e2), SC_(0.25554705810546875e3), SC_(0.3972603961730133195379956336197334288476e-107), 
      SC_(-0.7460263824462890625e2), SC_(0.503011474609375e3), SC_(0.4874624060193139320406839502988832481089e-217), 
      SC_(-0.7460263824462890625e2), SC_(0.10074598388671875e4), SC_(0.1822212069789909176095875838528811873338e-437), 
      SC_(-0.7460263824462890625e2), SC_(0.1185395751953125e4), SC_(0.5875055967970574458131259176159286617499e-515), 
      SC_(-0.7460263824462890625e2), SC_(0.353451806640625e4), SC_(0.4406079158432466047722722836894011978239e-1536), 
      SC_(-0.7460263824462890625e2), SC_(0.80715478515625e4), SC_(0.7338395057162425548486505792810413989371e-3507), 
      SC_(-0.7460263824462890625e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.6218012346099611746045494400088987852165e-7050)), 
      SC_(-0.7460263824462890625e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5002276251033884106325883264873018499132e-13928)), 
      SC_(-0.7460263824462890625e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2826609186886966995318007844162294906315e-15796)), 
      SC_(-0.7290460205078125e2), SC_(0.24750102996826171875e2), SC_(0.5561803915497248563365929946842781443078e23), 
      SC_(-0.7290460205078125e2), SC_(0.637722015380859375e2), SC_(0.1094524924593545154904194423989731358977e-11), 
      SC_(-0.7290460205078125e2), SC_(0.1252804412841796875e3), SC_(0.3839300658689373815830761148374331937154e-46), 
      SC_(-0.7290460205078125e2), SC_(0.25554705810546875e3), SC_(0.2451728941031062427272665484306743376086e-107), 
      SC_(-0.7290460205078125e2), SC_(0.503011474609375e3), SC_(0.3804541047790449891831659262615119819112e-217), 
      SC_(-0.7290460205078125e2), SC_(0.10074598388671875e4), SC_(0.1609485041764832205733383613676089003386e-437), 
      SC_(-0.7290460205078125e2), SC_(0.1185395751953125e4), SC_(0.5286617461619307606407976695028744909355e-515), 
      SC_(-0.7290460205078125e2), SC_(0.353451806640625e4), SC_(0.4252727041870810007272294050962600690759e-1536), 
      SC_(-0.7290460205078125e2), SC_(0.80715478515625e4), SC_(0.7225421446583687935214716001980501582795e-3507), 
      SC_(-0.7290460205078125e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.617021607511078284203431245617539588877e-7050)), 
      SC_(-0.7290460205078125e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.4982778020141303245214047263053056397026e-13928)), 
      SC_(-0.7290460205078125e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.281689238316161921546343371679087454574e-15796)), 
      SC_(-0.62323604583740234375e2), SC_(0.24750102996826171875e2), SC_(0.6745183967776568226882524708487938056875e15), 
      SC_(-0.62323604583740234375e2), SC_(0.637722015380859375e2), SC_(0.6545311734942178902723924532558287624952e-16), 
      SC_(-0.62323604583740234375e2), SC_(0.1252804412841796875e3), SC_(0.1656532226161521639805764466363495194113e-48), 
      SC_(-0.62323604583740234375e2), SC_(0.25554705810546875e3), SC_(0.1547673376370412380297419400250693508513e-108), 
      SC_(-0.62323604583740234375e2), SC_(0.503011474609375e3), SC_(0.9227214789189674273358185346965399203582e-218), 
      SC_(-0.62323604583740234375e2), SC_(0.10074598388671875e4), SC_(0.7918944121135532385395798829085145544592e-438), 
      SC_(-0.62323604583740234375e2), SC_(0.1185395751953125e4), SC_(0.2892810675468518815348991889357281331268e-515), 
      SC_(-0.62323604583740234375e2), SC_(0.353451806640625e4), SC_(0.3473597010045323910900283230401929551928e-1536), 
      SC_(-0.62323604583740234375e2), SC_(0.80715478515625e4), SC_(0.6612598699249681198531835080793951126635e-3507), 
      SC_(-0.62323604583740234375e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5904134813790360951037473338098849774904e-7050)), 
      SC_(-0.62323604583740234375e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.4872840764596854116092324404256445242957e-13928)), 
      SC_(-0.62323604583740234375e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2762021177708928645685587252784589095343e-15796)), 
      SC_(-0.5579319000244140625e2), SC_(0.24750102996826171875e2), SC_(0.2000280553692923364816391845858003081304e11), 
      SC_(-0.5579319000244140625e2), SC_(0.637722015380859375e2), SC_(0.3011072877774196098095590001850230113398e-18), 
      SC_(-0.5579319000244140625e2), SC_(0.1252804412841796875e3), SC_(0.8546927999408637677019436633377190577324e-50), 
      SC_(-0.5579319000244140625e2), SC_(0.25554705810546875e3), SC_(0.3476662826409067664561159567913234387987e-109), 
      SC_(-0.5579319000244140625e2), SC_(0.503011474609375e3), SC_(0.4297054501709256270968786489029298481091e-218), 
      SC_(-0.5579319000244140625e2), SC_(0.10074598388671875e4), SC_(0.5402417605668363705190262971682628012869e-438), 
      SC_(-0.5579319000244140625e2), SC_(0.1185395751953125e4), SC_(0.2089958153703015756026346516755670166549e-515), 
      SC_(-0.5579319000244140625e2), SC_(0.353451806640625e4), SC_(0.3114579751849795632507912984614279046035e-1536), 
      SC_(-0.5579319000244140625e2), SC_(0.80715478515625e4), SC_(0.6304085854717380067005364169553964460527e-3507), 
      SC_(-0.5579319000244140625e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5765486071511390988466247681614919123209e-7050)), 
      SC_(-0.5579319000244140625e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.4814584754866145181310357527683878612407e-13928)), 
      SC_(-0.5579319000244140625e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2732885595625837213661882188978422888301e-15796)), 
      SC_(-0.4430035400390625e2), SC_(0.95070552825927734375e1), SC_(0.5693602607646284460254541471864922205948e23), 
      SC_(-0.4430035400390625e2), SC_(0.24750102996826171875e2), SC_(0.1242729664484783369574386233140179346878e4), 
      SC_(-0.4430035400390625e2), SC_(0.637722015380859375e2), SC_(0.7993412663367930219134100562570886747324e-22), 
      SC_(-0.4430035400390625e2), SC_(0.1252804412841796875e3), SC_(0.9881485422320279470670535583393602847552e-52), 
      SC_(-0.4430035400390625e2), SC_(0.25554705810546875e3), SC_(0.3730073474257981229404066713919938526343e-110), 
      SC_(-0.4430035400390625e2), SC_(0.503011474609375e3), SC_(0.1373667058825755035108658858704836181807e-218), 
      SC_(-0.4430035400390625e2), SC_(0.10074598388671875e4), SC_(0.3053981542547827391176412965629715685693e-438), 
      SC_(-0.4430035400390625e2), SC_(0.1185395751953125e4), SC_(0.1286946967513954764039399072385367798007e-515), 
      SC_(-0.4430035400390625e2), SC_(0.353451806640625e4), SC_(0.2646904138441718084112297837143320157831e-1536), 
      SC_(-0.4430035400390625e2), SC_(0.80715478515625e4), SC_(0.5870517224916429591472211129863301372511e-3507), 
      SC_(-0.4430035400390625e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.556473689966344568142116011084377141557e-7050)), 
      SC_(-0.4430035400390625e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.4728995700926201307193816855831797459788e-13928)), 
      SC_(-0.4430035400390625e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2690004084585417033992798959421396256472e-15796)), 
      SC_(-0.383665924072265625e2), SC_(0.51139926910400390625e1), SC_(0.4971541960447850485036217351908812317262e28), 
      SC_(-0.383665924072265625e2), SC_(0.95070552825927734375e1), SC_(0.1514361321039985388396503732421720671137e18), 
      SC_(-0.383665924072265625e2), SC_(0.24750102996826171875e2), SC_(0.6394950974987836910026204697203193967018e0), 
      SC_(-0.383665924072265625e2), SC_(0.637722015380859375e2), SC_(0.2193338639829460198350749734782436371427e-23), 
      SC_(-0.383665924072265625e2), SC_(0.1252804412841796875e3), SC_(0.1453511701396794997715829292555274995485e-52), 
      SC_(-0.383665924072265625e2), SC_(0.25554705810546875e3), SC_(0.143713375904389446480817449936644081692e-110), 
      SC_(-0.383665924072265625e2), SC_(0.503011474609375e3), SC_(0.8444454506176474868328821803019802033188e-219), 
      SC_(-0.383665924072265625e2), SC_(0.10074598388671875e4), SC_(0.2394533298546757062831171384270888559283e-438), 
      SC_(-0.383665924072265625e2), SC_(0.1185395751953125e4), SC_(0.1046548965046943506923715763697928376188e-515), 
      SC_(-0.383665924072265625e2), SC_(0.353451806640625e4), SC_(0.2469489195756011361369232839149459989988e-1536), 
      SC_(-0.383665924072265625e2), SC_(0.80715478515625e4), SC_(0.5694829422897774445672362912138934152265e-3507), 
      SC_(-0.383665924072265625e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5481275253120409890933948754020554435422e-7050)), 
      SC_(-0.383665924072265625e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.4692963862579519130682539614682894698339e-13928)), 
      SC_(-0.383665924072265625e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2671924165871110012794803086242495185922e-15796)), 
      SC_(0.93762989044189453125e1), SC_(0.7444499991834163665771484375e-2), SC_(0.2721057737406919258362851434733030661109e28), 
      SC_(0.93762989044189453125e1), SC_(0.1433600485324859619140625e-1), SC_(0.5838623563730079614571930542897504652141e25), 
      SC_(0.93762989044189453125e1), SC_(0.1760916970670223236083984375e-1), SC_(0.8489946048751590475608166482859224225092e24), 
      SC_(0.93762989044189453125e1), SC_(0.6152711808681488037109375e-1), SC_(0.6830571091090551343461028048390799816461e19), 
      SC_(0.93762989044189453125e1), SC_(0.11958599090576171875e0), SC_(0.1343359009110053824259990283998636373658e17), 
      SC_(0.93762989044189453125e1), SC_(0.15262925624847412109375e0), SC_(0.1363284485271480815886779689298289259733e16), 
      SC_(0.93762989044189453125e1), SC_(0.408089816570281982421875e0), SC_(0.134259416566970599290007837647461336503e12), 
      SC_(0.93762989044189453125e1), SC_(0.6540834903717041015625e0), SC_(0.1597975002045160572672179531598793911004e10), 
      SC_(0.93762989044189453125e1), SC_(0.1097540378570556640625e1), SC_(0.1218600558373790598620257006463817375794e8), 
      SC_(0.93762989044189453125e1), SC_(0.30944411754608154296875e1), SC_(0.5737745231574650911405694300067785689295e3), 
      SC_(0.93762989044189453125e1), SC_(0.51139926910400390625e1), SC_(0.3246966656549353778987348015527776300809e1), 
      SC_(0.93762989044189453125e1), SC_(0.95070552825927734375e1), SC_(0.19259552888384324744815873080876780038e-2), 
      SC_(0.93762989044189453125e1), SC_(0.24750102996826171875e2), SC_(0.2504648183075237600300430416689758405566e-10), 
      SC_(0.93762989044189453125e1), SC_(0.637722015380859375e2), SC_(0.6244803648310629601249155899182438184287e-28), 
      SC_(0.93762989044189453125e1), SC_(0.1252804412841796875e3), SC_(0.6191446466751584500204805922395171337903e-55), 
      SC_(0.93762989044189453125e1), SC_(0.25554705810546875e3), SC_(0.9682463471616540956204871299762767140143e-112), 
      SC_(0.93762989044189453125e1), SC_(0.503011474609375e3), SC_(0.2137809921466214788678904652498781109907e-219), 
      SC_(0.93762989044189453125e1), SC_(0.10074598388671875e4), SC_(0.1205266421458943045615585394077333566019e-438), 
      SC_(0.93762989044189453125e1), SC_(0.1185395751953125e4), SC_(0.5839106645002418157495951205762502056769e-516), 
      SC_(0.93762989044189453125e1), SC_(0.353451806640625e4), SC_(0.2030427297652053795158460484630149203489e-1536), 
      SC_(0.93762989044189453125e1), SC_(0.80715478515625e4), SC_(0.5226939353263804188411440085354658326468e-3507), 
      SC_(0.93762989044189453125e1), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5252465465326302525711470608778783375618e-7050)), 
      SC_(0.93762989044189453125e1), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.4592768866982000517622013128091807640282e-13928)), 
      SC_(0.93762989044189453125e1), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2621561909115651489224577641835193518283e-15796)), 
      SC_(0.944411754608154296875e1), SC_(0.7444499991834163665771484375e-2), SC_(0.4612303621288630783177800069821634006522e28), 
      SC_(0.944411754608154296875e1), SC_(0.1433600485324859619140625e-1), SC_(0.9466510890632728074237180634479345051875e25), 
      SC_(0.944411754608154296875e1), SC_(0.1760916970670223236083984375e-1), SC_(0.1357461344862089559034023564527191961687e25), 
      SC_(0.944411754608154296875e1), SC_(0.6152711808681488037109375e-1), SC_(0.1003303142080314424800411457172597807663e20), 
      SC_(0.944411754608154296875e1), SC_(0.11958599090576171875e0), SC_(0.1886231320842499600375267715822147672472e17), 
      SC_(0.944411754608154296875e1), SC_(0.15262925624847412109375e0), SC_(0.188280089386461382126468384112433440471e16), 
      SC_(0.944411754608154296875e1), SC_(0.408089816570281982421875e0), SC_(0.1734646534584651712744606645408188786047e12), 
      SC_(0.944411754608154296875e1), SC_(0.6540834903717041015625e0), SC_(0.199971824633020007858878828667253874034e10), 
      SC_(0.944411754608154296875e1), SC_(0.1097540378570556640625e1), SC_(0.1472636215284163281676380655271707289138e8), 
      SC_(0.944411754608154296875e1), SC_(0.30944411754608154296875e1), SC_(0.647535838284923144345128491044954961457e3), 
      SC_(0.944411754608154296875e1), SC_(0.51139926910400390625e1), SC_(0.3553277729257552005521728768120656012587e1), 
      SC_(0.944411754608154296875e1), SC_(0.95070552825927734375e1), SC_(0.2040071094456773692563150515108671359582e-2), 
      SC_(0.944411754608154296875e1), SC_(0.24750102996826171875e2), SC_(0.2567440825871508188288207680932556974655e-10), 
      SC_(0.944411754608154296875e1), SC_(0.637722015380859375e2), SC_(0.6306907593710430642478969958415511608599e-28), 
      SC_(0.944411754608154296875e1), SC_(0.1252804412841796875e3), SC_(0.6222912130499996900925885235524244670356e-55), 
      SC_(0.944411754608154296875e1), SC_(0.25554705810546875e3), SC_(0.9706621439129537561340645836040221175143e-112), 
      SC_(0.944411754608154296875e1), SC_(0.503011474609375e3), SC_(0.2140521102843204219804606886329697019218e-219), 
      SC_(0.944411754608154296875e1), SC_(0.10074598388671875e4), SC_(0.1206029763999953146275755740805060498224e-438), 
      SC_(0.944411754608154296875e1), SC_(0.1185395751953125e4), SC_(0.5842249762482709543785194262081511657188e-516), 
      SC_(0.944411754608154296875e1), SC_(0.353451806640625e4), SC_(0.2030793889519787363418582358670409448464e-1536), 
      SC_(0.944411754608154296875e1), SC_(0.80715478515625e4), SC_(0.5227352618739306325665664939206185443367e-3507), 
      SC_(0.944411754608154296875e1), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5252672007246779268718011077113001418501e-7050)), 
      SC_(0.944411754608154296875e1), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.459286027254970380991295408755149756266e-13928)), 
      SC_(0.944411754608154296875e1), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2621607912254129949405484094002130635557e-15796)), 
      SC_(0.264718532562255859375e2), SC_(0.30944411754608154296875e1), SC_(0.3138027209961995689508888198752860190762e21), 
      SC_(0.264718532562255859375e2), SC_(0.51139926910400390625e1), SC_(0.4476324433882455541710673350842360857005e15), 
      SC_(0.264718532562255859375e2), SC_(0.95070552825927734375e1), SC_(0.1797701745980331951453531906793951728351e8), 
      SC_(0.264718532562255859375e2), SC_(0.24750102996826171875e2), SC_(0.1853961961302356637784495895011492126184e-5), 
      SC_(0.264718532562255859375e2), SC_(0.637722015380859375e2), SC_(0.6846626929866392127688041786664525681184e-26), 
      SC_(0.264718532562255859375e2), SC_(0.1252804412841796875e3), SC_(0.7007081484985308886329921570722300664339e-54), 
      SC_(0.264718532562255859375e2), SC_(0.25554705810546875e3), SC_(0.3200441114843926372066668842735553862582e-111), 
      SC_(0.264718532562255859375e2), SC_(0.503011474609375e3), SC_(0.3928255814072251732422205624746319957713e-219), 
      SC_(0.264718532562255859375e2), SC_(0.10074598388671875e4), SC_(0.1633431966531690433140682468944903891714e-438), 
      SC_(0.264718532562255859375e2), SC_(0.1185395751953125e4), SC_(0.7560629628467314221548665586213495324071e-516), 
      SC_(0.264718532562255859375e2), SC_(0.353451806640625e4), SC_(0.2214280876600389182130706966921584688308e-1536), 
      SC_(0.264718532562255859375e2), SC_(0.80715478515625e4), SC_(0.5429172542806217828545213979048536079656e-3507), 
      SC_(0.264718532562255859375e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5352575470018113610063179994677083114284e-7050)), 
      SC_(0.264718532562255859375e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.4636866635489855424809660555450541152793e-13928)), 
      SC_(0.264718532562255859375e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2643743165267396396035797763057882346377e-15796)), 
      SC_(0.62944732666015625e2), SC_(0.24750102996826171875e2), SC_(0.1881615957650121835951151459881112433642e16), 
      SC_(0.62944732666015625e2), SC_(0.637722015380859375e2), SC_(0.1119984019082018571198103341652740655437e-15), 
      SC_(0.62944732666015625e2), SC_(0.1252804412841796875e3), SC_(0.2231352378794206033018221030703217338878e-48), 
      SC_(0.62944732666015625e2), SC_(0.25554705810546875e3), SC_(0.1798990817823353758503389738709281233441e-108), 
      SC_(0.62944732666015625e2), SC_(0.503011474609375e3), SC_(0.996645616647744895755158621634872080052e-218), 
      SC_(0.62944732666015625e2), SC_(0.10074598388671875e4), SC_(0.8230359935387643787461157397598595672318e-438), 
      SC_(0.62944732666015625e2), SC_(0.1185395751953125e4), SC_(0.2989238946471756510957437745632191274437e-515), 
      SC_(0.62944732666015625e2), SC_(0.353451806640625e4), SC_(0.3512034003208095136837320409364102048508e-1536), 
      SC_(0.62944732666015625e2), SC_(0.80715478515625e4), SC_(0.6644545217571422485174738283493212729231e-3507), 
      SC_(0.62944732666015625e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5918304376897943519364581212285760132986e-7050)), 
      SC_(0.62944732666015625e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.4878756153781530982084716360839760780792e-13928)), 
      SC_(0.62944732666015625e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2764977329940568590368972098475304997089e-15796)), 
      SC_(0.67001708984375e2), SC_(0.24750102996826171875e2), SC_(0.1757647753992712633411465211149383976276e19), 
      SC_(0.67001708984375e2), SC_(0.637722015380859375e2), SC_(0.4154340476824965842042958242696691352995e-14), 
      SC_(0.67001708984375e2), SC_(0.1252804412841796875e3), SC_(0.1670237943823919787558085338623229345138e-47), 
      SC_(0.67001708984375e2), SC_(0.25554705810546875e3), SC_(0.498292972015574678613637170399151819008e-108), 
      SC_(0.67001708984375e2), SC_(0.503011474609375e3), SC_(0.16798665586593853126389607640525271824e-217), 
      SC_(0.67001708984375e2), SC_(0.10074598388671875e4), SC_(0.1068843553462207539802876852120126159371e-437), 
      SC_(0.67001708984375e2), SC_(0.1185395751953125e4), SC_(0.3732889925210086644082342106707283321478e-515), 
      SC_(0.67001708984375e2), SC_(0.353451806640625e4), SC_(0.3783910172802151361859098458404374020052e-1536), 
      SC_(0.67001708984375e2), SC_(0.80715478515625e4), SC_(0.6865103783696347817927096681584785305085e-3507), 
      SC_(0.67001708984375e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.6015210836405717718928078795712448330673e-7050)), 
      SC_(0.67001708984375e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.4919025754419690182073132779957366712633e-13928)), 
      SC_(0.67001708984375e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2785090394166165242891428442540272849689e-15796)), 
      SC_(0.8115838623046875e2), SC_(0.24750102996826171875e2), SC_(0.2324073088339678621147409396324732999963e30), 
      SC_(0.8115838623046875e2), SC_(0.637722015380859375e2), SC_(0.4837827210750428522126700812080085686173e-8), 
      SC_(0.8115838623046875e2), SC_(0.1252804412841796875e3), SC_(0.4594264686055687077550409988148757821966e-44), 
      SC_(0.8115838623046875e2), SC_(0.25554705810546875e3), SC_(0.2833330148811881793898662192076717416668e-106), 
      SC_(0.8115838623046875e2), SC_(0.503011474609375e3), SC_(0.1338396381867003597313054226031862608662e-216), 
      SC_(0.8115838623046875e2), SC_(0.10074598388671875e4), SC_(0.3022491925648536051406776884375450576521e-437), 
      SC_(0.8115838623046875e2), SC_(0.1185395751953125e4), SC_(0.9033480738287476948124079020084554369235e-515), 
      SC_(0.8115838623046875e2), SC_(0.353451806640625e4), SC_(0.509064318997321343171662382751489914207e-1536), 
      SC_(0.8115838623046875e2), SC_(0.80715478515625e4), SC_(0.7817541885759932491771910816986777412753e-3507), 
      SC_(0.8115838623046875e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.641673078077478782785017874716473833835e-7050)), 
      SC_(0.8115838623046875e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5082559642711766692197944021073471381169e-13928)), 
      SC_(0.8115838623046875e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2866570865715865919392615432142045181677e-15796)), 
      SC_(0.826751708984375e2), SC_(0.637722015380859375e2), SC_(0.2433991751428157267576270898319088542731e-7), 
      SC_(0.826751708984375e2), SC_(0.1252804412841796875e3), SC_(0.1163543281808651892214225743779071494397e-43), 
      SC_(0.826751708984375e2), SC_(0.25554705810546875e3), SC_(0.4566929184599502585539208159658295553088e-106), 
      SC_(0.826751708984375e2), SC_(0.503011474609375e3), SC_(0.1711152047141484938876244759615327721674e-216), 
      SC_(0.826751708984375e2), SC_(0.10074598388671875e4), SC_(0.3418546552853844882451811321181419188818e-437), 
      SC_(0.826751708984375e2), SC_(0.1185395751953125e4), SC_(0.1003047492936821319442367382054853866129e-514), 
      SC_(0.826751708984375e2), SC_(0.353451806640625e4), SC_(0.5272736092096344411527859246178125271135e-1536), 
      SC_(0.826751708984375e2), SC_(0.80715478515625e4), SC_(0.7938803272826403099037784228659554176949e-3507), 
      SC_(0.826751708984375e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.6466043665396298360019348958169487245916e-7050)), 
      SC_(0.826751708984375e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5102291405695378889093263053607260918263e-13928)), 
      SC_(0.826751708984375e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2876381032836693648306723630952201558894e-15796)), 
      SC_(0.9150136566162109375e2), SC_(0.637722015380859375e2), SC_(0.4529077292464217597597948585815203283964e-3), 
      SC_(0.9150136566162109375e2), SC_(0.1252804412841796875e3), SC_(0.350642591754902750250314160055299520222e-41), 
      SC_(0.9150136566162109375e2), SC_(0.25554705810546875e3), SC_(0.8700616972854344814927244483121769950875e-105), 
      SC_(0.9150136566162109375e2), SC_(0.503011474609375e3), SC_(0.7816646797077293163816105964438544637339e-216), 
      SC_(0.9150136566162109375e2), SC_(0.10074598388671875e4), SC_(0.7321805895040821980280295179421030138686e-437), 
      SC_(0.9150136566162109375e2), SC_(0.1185395751953125e4), SC_(0.1916743519112958612938166592106183128909e-514), 
      SC_(0.9150136566162109375e2), SC_(0.353451806640625e4), SC_(0.6553297924375392696631080233904592409428e-1536), 
      SC_(0.9150136566162109375e2), SC_(0.80715478515625e4), SC_(0.8731920655533864492797505716863887075778e-3507), 
      SC_(0.9150136566162109375e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.6779648490610206356859880361410662474219e-7050)), 
      SC_(0.9150136566162109375e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5226073790697475866503341992543667357366e-13928)), 
      SC_(0.9150136566162109375e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2937821135047504313538653046960400698576e-15796)), 
      SC_(0.9297769927978515625e2), SC_(0.637722015380859375e2), SC_(0.2514159824708407029519182952938261461039e-2), 
      SC_(0.9297769927978515625e2), SC_(0.1252804412841796875e3), SC_(0.9571910315928452948496436653664936091399e-41), 
      SC_(0.9297769927978515625e2), SC_(0.25554705810546875e3), SC_(0.1464962376552961548773813405449012101125e-104), 
      SC_(0.9297769927978515625e2), SC_(0.503011474609375e3), SC_(0.102289622788229900557470609045948505245e-215), 
      SC_(0.9297769927978515625e2), SC_(0.10074598388671875e4), SC_(0.8379350403930188404501970121630099171596e-437), 
      SC_(0.9297769927978515625e2), SC_(0.1185395751953125e4), SC_(0.2149730062284109979164130574050367445585e-514), 
      SC_(0.9297769927978515625e2), SC_(0.353451806640625e4), SC_(0.6810640416069074949832134642840700061677e-1536), 
      SC_(0.9297769927978515625e2), SC_(0.80715478515625e4), SC_(0.8880475416546418613925474382495752636196e-3507), 
      SC_(0.9297769927978515625e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.6836772391670514269657537156841197891453e-7050)), 
      SC_(0.9297769927978515625e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5248314285242353145777820274280636896797e-13928)), 
      SC_(0.9297769927978515625e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2948841983584072578394887812793739973464e-15796)), 
      SC_(0.935389862060546875e2), SC_(0.637722015380859375e2), SC_(0.4848538332206214872114685461457208526682e-2), 
      SC_(0.935389862060546875e2), SC_(0.1252804412841796875e3), SC_(0.1407357631531569552447817697356939114967e-40), 
      SC_(0.935389862060546875e2), SC_(0.25554705810546875e3), SC_(0.1789644517979726575647466756527040582949e-104), 
      SC_(0.935389862060546875e2), SC_(0.503011474609375e3), SC_(0.1134297740334902791076613679075967354327e-215), 
      SC_(0.935389862060546875e2), SC_(0.10074598388671875e4), SC_(0.8825347981316688657305249314794392766025e-437), 
      SC_(0.935389862060546875e2), SC_(0.1185395751953125e4), SC_(0.2246641303550294973758291904185796545436e-514), 
      SC_(0.935389862060546875e2), SC_(0.353451806640625e4), SC_(0.6912227290322635031334137954308461354241e-1536), 
      SC_(0.935389862060546875e2), SC_(0.80715478515625e4), SC_(0.893824833574208599253272536219631346374e-3507), 
      SC_(0.935389862060546875e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.6858858035128739755837136827347333728619e-7050)), 
      SC_(0.935389862060546875e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5256888449089264657997112108384303458607e-13928)), 
      SC_(0.935389862060546875e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2953089268910496653492909777451085521704e-15796)), 
      SC_(0.937735595703125e2), SC_(0.637722015380859375e2), SC_(0.6385107666034147877046020721409920794724e-2), 
      SC_(0.937735595703125e2), SC_(0.1252804412841796875e3), SC_(0.1654344599410448916584423239120073465608e-40), 
      SC_(0.937735595703125e2), SC_(0.25554705810546875e3), SC_(0.1946478889917254423807361997687268337342e-104), 
      SC_(0.937735595703125e2), SC_(0.503011474609375e3), SC_(0.1184592506913226623646749833739806823329e-215), 
      SC_(0.937735595703125e2), SC_(0.10074598388671875e4), SC_(0.901953355688788108549058088701948794206e-437), 
      SC_(0.937735595703125e2), SC_(0.1185395751953125e4), SC_(0.2288605393501964892215062020117435573162e-514), 
      SC_(0.937735595703125e2), SC_(0.353451806640625e4), SC_(0.6955313792142442148137724010444647419301e-1536), 
      SC_(0.937735595703125e2), SC_(0.80715478515625e4), SC_(0.8962607682372740535877231935882633914515e-3507), 
      SC_(0.937735595703125e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.6868148706801060740839033435132304040165e-7050)), 
      SC_(0.937735595703125e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.526049123050695967902464958981106803334e-13928)), 
      SC_(0.937735595703125e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.295487369292694824273684043549849890813e-15796)), 
      SC_(0.98576263427734375e2), SC_(0.637722015380859375e2), SC_(0.1990227834888151454373307286008775674612e1), 
      SC_(0.98576263427734375e2), SC_(0.1252804412841796875e3), SC_(0.4894424029224796393682491693146694616972e-39), 
      SC_(0.98576263427734375e2), SC_(0.25554705810546875e3), SC_(0.1136170375675789312265821779116321329958e-103), 
      SC_(0.98576263427734375e2), SC_(0.503011474609375e3), SC_(0.2948452830082071500214646231076147090724e-215), 
      SC_(0.98576263427734375e2), SC_(0.10074598388671875e4), SC_(0.1425279255280814495079497699158457698546e-436), 
      SC_(0.98576263427734375e2), SC_(0.1185395751953125e4), SC_(0.3377080491094056336148164895290399079231e-514), 
      SC_(0.98576263427734375e2), SC_(0.353451806640625e4), SC_(0.7926042309665472483589245698365986058321e-1536), 
      SC_(0.98576263427734375e2), SC_(0.80715478515625e4), SC_(0.9490411542227230445519501560068255751126e-3507), 
      SC_(0.98576263427734375e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.706642352671901713112325219552778166628e-7050)), 
      SC_(0.98576263427734375e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5336813326314223870941968492332354768707e-13928)), 
      SC_(0.98576263427734375e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.2992641523816122158711374925214774085881e-15796)), 
      SC_(0.99292266845703125e2), SC_(0.637722015380859375e2), SC_(0.4765079470941391660151554471287672030993e1), 
      SC_(0.99292266845703125e2), SC_(0.1252804412841796875e3), SC_(0.8211475782164329588746473557436288828796e-39), 
      SC_(0.99292266845703125e2), SC_(0.25554705810546875e3), SC_(0.1488689678947819908654123225405437300755e-103), 
      SC_(0.99292266845703125e2), SC_(0.503011474609375e3), SC_(0.3390847523037554808868465852319867472935e-215), 
      SC_(0.99292266845703125e2), SC_(0.10074598388671875e4), SC_(0.1528876974454391292968555182897209684705e-436), 
      SC_(0.99292266845703125e2), SC_(0.1185395751953125e4), SC_(0.3584703400837929375694495921804618033184e-514), 
      SC_(0.99292266845703125e2), SC_(0.353451806640625e4), SC_(0.8086451033245101399967163404205099861285e-1536), 
      SC_(0.99292266845703125e2), SC_(0.80715478515625e4), SC_(0.9574060247932829286311891259694070669118e-3507), 
      SC_(0.99292266845703125e2), SC_(0.1622925e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.7097333247638353953239893327006028051669e-7050)), 
      SC_(0.99292266845703125e2), SC_(0.3206622265625e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.5348615672629262632704311334792898428533e-13928)), 
      SC_(0.99292266845703125e2), SC_(0.3636794921875e5), SC_(BOOST_MATH_SMALL_CONSTANT(0.299847616599843593158654409241939580736e-15796))
   };
#undef SC_


