//  Copyright (c) 2007 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
   static const boost::array<boost::array<T, 3>, 300> bessel_yn_data = {
      SC_(0.3e1), SC_(0.48770198822021484375e1), SC_(0.110763167474753768460862899966350902213e0), 
      SC_(0.3e1), SC_(0.6349340915679931640625e1), SC_(0.3354120577583840086404698897976135110485e0), 
      SC_(0.3e1), SC_(0.677385044097900390625e1), SC_(0.3025179709577162581120705977708622170952e0), 
      SC_(0.3e1), SC_(0.941909885406494140625e1), SC_(-0.2526681038602083570260072993947985762425e0), 
      SC_(0.3e1), SC_(0.110517024993896484375e2), SC_(-0.7984312482740648942854070462466960846308e-1), 
      SC_(0.3e1), SC_(0.139249114990234375e2), SC_(0.1365190825919215541308802073699101624338e0), 
      SC_(0.3e1), SC_(0.15408351898193359375e2), SC_(-0.1428229643813645478620540882069209010424e0), 
      SC_(0.3e1), SC_(0.27344074249267578125e2), SC_(-0.2323694273239485669670827936469439230487e-2), 
      SC_(0.3e1), SC_(0.273610286712646484375e2), SC_(-0.4900800934361467422069755250398652540081e-2), 
      SC_(0.3e1), SC_(0.316179637908935546875e2), SC_(0.1283616028850886874076400480565553924498e0), 
      SC_(0.3e1), SC_(0.4073618316650390625e2), SC_(-0.8876885924474539097667043483672377859036e-1), 
      SC_(0.3e1), SC_(0.4175042724609375e2), SC_(-0.1204076921163476847306821484924527347889e0), 
      SC_(0.3e1), SC_(0.452895965576171875e2), SC_(0.9669959686067408105414856540894770457144e-1), 
      SC_(0.3e1), SC_(0.45668792724609375e2), SC_(0.6416654049008983918193469604019080196524e-1), 
      SC_(0.3e1), SC_(0.48443389892578125e2), SC_(-0.9309152059642734958018227116834056340071e-1), 
      SC_(0.6e1), SC_(0.48770198822021484375e1), SC_(-0.7682201721816641589208505594820750775031e0), 
      SC_(0.6e1), SC_(0.6349340915679931640625e1), SC_(-0.3479423535919170112541557568046971845486e0), 
      SC_(0.6e1), SC_(0.677385044097900390625e1), SC_(-0.2518297913144822384948024722074769783391e0), 
      SC_(0.6e1), SC_(0.941909885406494140625e1), SC_(0.2696495425502131298219276073956952657041e0), 
      SC_(0.6e1), SC_(0.110517024993896484375e2), SC_(0.1579058061166539049950139801492495632319e0), 
      SC_(0.6e1), SC_(0.139249114990234375e2), SC_(-0.214638507088782708332760891144120271447e0), 
      SC_(0.6e1), SC_(0.15408351898193359375e2), SC_(0.1846008639272405573148078141757591445936e-1), 
      SC_(0.6e1), SC_(0.27344074249267578125e2), SC_(-0.1347270014641035443110406257337437495382e0), 
      SC_(0.6e1), SC_(0.273610286712646484375e2), SC_(-0.1334153397469278773891149524770013367273e0), 
      SC_(0.6e1), SC_(0.316179637908935546875e2), SC_(0.2378484491732698166359206052246509293721e-2), 
      SC_(0.6e1), SC_(0.4073618316650390625e2), SC_(-0.5472472826251697058558978710700928656814e-1), 
      SC_(0.6e1), SC_(0.4175042724609375e2), SC_(0.6520327308993981465264074827650561383002e-1), 
      SC_(0.6e1), SC_(0.452895965576171875e2), SC_(-0.9452992234494168858802163379037878510028e-1), 
      SC_(0.6e1), SC_(0.45668792724609375e2), SC_(-0.1140314635876510494905774064172128035215e0), 
      SC_(0.6e1), SC_(0.48443389892578125e2), SC_(0.9039125064888635209873025550900765793581e-1), 
      SC_(0.9e1), SC_(0.48770198822021484375e1), SC_(-0.9297117648579269026592450591820736286048e1), 
      SC_(0.9e1), SC_(0.6349340915679931640625e1), SC_(-0.1642785653883350207327509679573278148917e1), 
      SC_(0.9e1), SC_(0.677385044097900390625e1), SC_(-0.1163556464540991454316123708406235280022e1), 
      SC_(0.9e1), SC_(0.941909885406494140625e1), SC_(-0.3010601044539710017331500982549858067088e0), 
      SC_(0.9e1), SC_(0.110517024993896484375e2), SC_(-0.2324009087284838473547366749630392221952e-2), 
      SC_(0.9e1), SC_(0.139249114990234375e2), SC_(0.2214037020748939190117162404706773042894e0), 
      SC_(0.9e1), SC_(0.15408351898193359375e2), SC_(-0.8624940937284109551983786643262863800438e-2), 
      SC_(0.9e1), SC_(0.27344074249267578125e2), SC_(0.1529842094320696329428609651300091107366e0), 
      SC_(0.9e1), SC_(0.273610286712646484375e2), SC_(0.1534759546343323314550340680130494112099e0), 
      SC_(0.9e1), SC_(0.316179637908935546875e2), SC_(-0.1106752036561379100843137124587521895002e0), 
      SC_(0.9e1), SC_(0.4073618316650390625e2), SC_(0.1258819156633191815287890960535453440772e0), 
      SC_(0.9e1), SC_(0.4175042724609375e2), SC_(0.5724965886188775547845432977443031893556e-1), 
      SC_(0.9e1), SC_(0.452895965576171875e2), SC_(-0.1845007406807998875290607621548958463304e-1), 
      SC_(0.9e1), SC_(0.45668792724609375e2), SC_(0.2567744007868617975728678932906613386029e-1), 
      SC_(0.9e1), SC_(0.48443389892578125e2), SC_(0.230902957959941886085023895904914286182e-1), 
      SC_(0.12e2), SC_(0.48770198822021484375e1), SC_(-0.5014302199056606705943012073033758819751e3), 
      SC_(0.12e2), SC_(0.6349340915679931640625e1), SC_(-0.3184181647223721856874057494272981331161e2), 
      SC_(0.12e2), SC_(0.677385044097900390625e1), SC_(-0.1689950411904265377863222612787794563465e2), 
      SC_(0.12e2), SC_(0.941909885406494140625e1), SC_(-0.1113045406792556884051547016253472888446e1), 
      SC_(0.12e2), SC_(0.110517024993896484375e2), SC_(-0.4887919143673445189746680356694993315599e0), 
      SC_(0.12e2), SC_(0.139249114990234375e2), SC_(-0.5164270636697790648727679099312164508566e-1), 
      SC_(0.12e2), SC_(0.15408351898193359375e2), SC_(0.1641171463260105309258962095572986351808e0), 
      SC_(0.12e2), SC_(0.27344074249267578125e2), SC_(-0.1313908322913451851763761649507472357274e0), 
      SC_(0.12e2), SC_(0.273610286712646484375e2), SC_(-0.1327409819989311784031204560420443662466e0), 
      SC_(0.12e2), SC_(0.316179637908935546875e2), SC_(0.1459273101591621646487444827617739570529e0), 
      SC_(0.12e2), SC_(0.4073618316650390625e2), SC_(-0.9912101628665068614729233884578138310206e-1), 
      SC_(0.12e2), SC_(0.4175042724609375e2), SC_(-0.1210187021716223197235412947902227468745e0), 
      SC_(0.12e2), SC_(0.452895965576171875e2), SC_(0.1031061645767935052995708616490540524693e0), 
      SC_(0.12e2), SC_(0.45668792724609375e2), SC_(0.7348747656449140685461449731713104253185e-1), 
      SC_(0.12e2), SC_(0.48443389892578125e2), SC_(-0.104644764861527467660104682568902187136e0), 
      SC_(0.15e2), SC_(0.48770198822021484375e1), SC_(-0.666835354744947947091675381536305560756e5), 
      SC_(0.15e2), SC_(0.6349340915679931640625e1), SC_(-0.173729680482248942219130294732963316738e4), 
      SC_(0.15e2), SC_(0.677385044097900390625e1), SC_(-0.7318674423901733450279878419863714169138e3), 
      SC_(0.15e2), SC_(0.941909885406494140625e1), SC_(-0.1227797563147048810430867118311815267114e2), 
      SC_(0.15e2), SC_(0.110517024993896484375e2), SC_(-0.2336439631002320795333551729067096369295e1), 
      SC_(0.15e2), SC_(0.139249114990234375e2), SC_(-0.4608554067953761333240509889177356652562e0), 
      SC_(0.15e2), SC_(0.15408351898193359375e2), SC_(-0.2650007170730137552006350126116876981542e0), 
      SC_(0.15e2), SC_(0.27344074249267578125e2), SC_(0.1339393015080547697867316027047186886737e0), 
      SC_(0.15e2), SC_(0.273610286712646484375e2), SC_(0.1352764957654073478496974616598867736456e0), 
      SC_(0.15e2), SC_(0.316179637908935546875e2), SC_(-0.1504167696767632660980135636348239887166e0), 
      SC_(0.15e2), SC_(0.4073618316650390625e2), SC_(0.4192895741965062819366359688077289580721e-1), 
      SC_(0.15e2), SC_(0.4175042724609375e2), SC_(0.1222369456557319980687974057240153639664e0), 
      SC_(0.15e2), SC_(0.452895965576171875e2), SC_(-0.1212314948571698710841160352735037017439e0), 
      SC_(0.15e2), SC_(0.45668792724609375e2), SC_(-0.117929273832001751151211995799064238057e0), 
      SC_(0.15e2), SC_(0.48443389892578125e2), SC_(0.1133197238546128580729495578555993628308e0), 
      SC_(0.18e2), SC_(0.48770198822021484375e1), SC_(-0.1735006375409931866095877212397092601486e8), 
      SC_(0.18e2), SC_(0.6349340915679931640625e1), SC_(-0.1931345446044047628299354348757244520564e6), 
      SC_(0.18e2), SC_(0.677385044097900390625e1), SC_(-0.6562338168138688104175526311745778766709e5), 
      SC_(0.18e2), SC_(0.941909885406494140625e1), SC_(-0.3414245342500632489013066595244217862939e3), 
      SC_(0.18e2), SC_(0.110517024993896484375e2), SC_(-0.3342370864484154776921067730649931623749e2), 
      SC_(0.18e2), SC_(0.139249114990234375e2), SC_(-0.1925986338761310029583252021688787248734e1), 
      SC_(0.18e2), SC_(0.15408351898193359375e2), SC_(-0.7729891320320143670131205933282193832589e0), 
      SC_(0.18e2), SC_(0.27344074249267578125e2), SC_(-0.1693713632672311004500375437482770352724e0), 
      SC_(0.18e2), SC_(0.273610286712646484375e2), SC_(-0.169861300144376471128043717084906663622e0), 
      SC_(0.18e2), SC_(0.316179637908935546875e2), SC_(0.1563695685362752509428656687488096674676e0), 
      SC_(0.18e2), SC_(0.4073618316650390625e2), SC_(-0.123598598293883990094119772796947997619e-2), 
      SC_(0.18e2), SC_(0.4175042724609375e2), SC_(-0.1035816216055041714196396538346417208839e0), 
      SC_(0.18e2), SC_(0.452895965576171875e2), SC_(0.1043274265755935855106359131946658832855e0), 
      SC_(0.18e2), SC_(0.45668792724609375e2), SC_(0.1201881752927240427353419474405213705808e0), 
      SC_(0.18e2), SC_(0.48443389892578125e2), SC_(-0.8309624755011994592399503020642724269056e-1), 
      SC_(0.21e2), SC_(0.48770198822021484375e1), SC_(-0.7753821734416440873812393125485540325052e10), 
      SC_(0.21e2), SC_(0.6349340915679931640625e1), SC_(-0.3759868228594510202039725582719047882076e8), 
      SC_(0.21e2), SC_(0.677385044097900390625e1), SC_(-0.1037725906923191533052745601541569584266e8), 
      SC_(0.21e2), SC_(0.941909885406494140625e1), SC_(-0.1792312356947587111357560321883440343677e5), 
      SC_(0.21e2), SC_(0.110517024993896484375e2), SC_(-0.9816264308635109952262612473147845726117e3), 
      SC_(0.21e2), SC_(0.139249114990234375e2), SC_(-0.2127234712177029865639640613243979901087e2), 
      SC_(0.21e2), SC_(0.15408351898193359375e2), SC_(-0.4929530434109934804775310785854955847031e1), 
      SC_(0.21e2), SC_(0.27344074249267578125e2), SC_(0.1633000115931048781050122700079249067998e0), 
      SC_(0.21e2), SC_(0.273610286712646484375e2), SC_(0.1621093772190383209869937458074600220423e0), 
      SC_(0.21e2), SC_(0.316179637908935546875e2), SC_(-0.1513673625278760171223693576020059281231e0), 
      SC_(0.21e2), SC_(0.4073618316650390625e2), SC_(-0.8688767252744713843809375160645039487404e-2), 
      SC_(0.21e2), SC_(0.4175042724609375e2), SC_(0.9607878571353470463741773008046900188416e-1), 
      SC_(0.21e2), SC_(0.452895965576171875e2), SC_(-0.8741240150691469640654788323619280887104e-1), 
      SC_(0.21e2), SC_(0.45668792724609375e2), SC_(-0.1118492488032765870829767925254453767725e0), 
      SC_(0.21e2), SC_(0.48443389892578125e2), SC_(0.5199149092587105686782906210189564424145e-1), 
      SC_(0.24e2), SC_(0.48770198822021484375e1), SC_(-0.5461443876290786061809351447005645823262e13), 
      SC_(0.24e2), SC_(0.6349340915679931640625e1), SC_(-0.1166363089113358135443687855049318526504e11), 
      SC_(0.24e2), SC_(0.677385044097900390625e1), SC_(-0.2625133599502869464265375659393314730809e10), 
      SC_(0.24e2), SC_(0.941909885406494140625e1), SC_(-0.1557563982037263340582977212038997882649e7), 
      SC_(0.24e2), SC_(0.110517024993896484375e2), SC_(-0.4936461096398447103587648423487124265789e5), 
      SC_(0.24e2), SC_(0.139249114990234375e2), SC_(-0.4525227622995130407205035914097509628284e3), 
      SC_(0.24e2), SC_(0.15408351898193359375e2), SC_(-0.6838579391319029823184512700439766716529e2), 
      SC_(0.24e2), SC_(0.27344074249267578125e2), SC_(0.5830535937469980667664835132960579367079e-1), 
      SC_(0.24e2), SC_(0.273610286712646484375e2), SC_(0.5998908727772662300265268904568615954016e-1), 
      SC_(0.24e2), SC_(0.316179637908935546875e2), SC_(0.6450139374267427248002925668160089583638e-1), 
      SC_(0.24e2), SC_(0.4073618316650390625e2), SC_(-0.1657940235883360981964182195730278115219e-1), 
      SC_(0.24e2), SC_(0.4175042724609375e2), SC_(-0.1105961304327233662670573989206262285811e0), 
      SC_(0.24e2), SC_(0.452895965576171875e2), SC_(0.8827277758648920696119545891866298767649e-1), 
      SC_(0.24e2), SC_(0.45668792724609375e2), SC_(0.1127450351424643915561539984192859049149e0), 
      SC_(0.24e2), SC_(0.48443389892578125e2), SC_(-0.3904998353413187566910152336494460572733e-1), 
      SC_(0.27e2), SC_(0.48770198822021484375e1), SC_(-0.5700229879022840457171635396240656193837e16), 
      SC_(0.27e2), SC_(0.6349340915679931640625e1), SC_(-0.5398683696415433161973740320342388130399e13), 
      SC_(0.27e2), SC_(0.677385044097900390625e1), SC_(-0.9932607825446184225983016261896537934062e12), 
      SC_(0.27e2), SC_(0.941909885406494140625e1), SC_(-0.2066111827233993913234737443579213592631e9), 
      SC_(0.27e2), SC_(0.110517024993896484375e2), SC_(-0.3859963517479935508952474073070085480419e7), 
      SC_(0.27e2), SC_(0.139249114990234375e2), SC_(-0.1575833003576965837138680244394865501962e5), 
      SC_(0.27e2), SC_(0.15408351898193359375e2), SC_(-0.1624401453813190226441561523350858287342e4), 
      SC_(0.27e2), SC_(0.27344074249267578125e2), SC_(-0.2304841560742319228562155146496514571912e0), 
      SC_(0.27e2), SC_(0.273610286712646484375e2), SC_(-0.2291104911920783871735474862332852318047e0), 
      SC_(0.27e2), SC_(0.316179637908935546875e2), SC_(0.146919681225764025208625464704925761097e0), 
      SC_(0.27e2), SC_(0.4073618316650390625e2), SC_(0.7870921593406705963080756994997274488282e-1), 
      SC_(0.27e2), SC_(0.4175042724609375e2), SC_(0.1377040623482925264077068216102224276994e0), 
      SC_(0.27e2), SC_(0.452895965576171875e2), SC_(-0.1097401858162375653284138113096804910789e0), 
      SC_(0.27e2), SC_(0.45668792724609375e2), SC_(-0.1260546859890806748764811329954061199216e0), 
      SC_(0.27e2), SC_(0.48443389892578125e2), SC_(0.5070577955058294270091859102131542430449e-1), 
      SC_(0.3e2), SC_(0.48770198822021484375e1), SC_(-0.8414585813815509264321238704154870420876e19), 
      SC_(0.3e2), SC_(0.6349340915679931640625e1), SC_(-0.355071160398021375797759541062133043762e16), 
      SC_(0.3e2), SC_(0.677385044097900390625e1), SC_(-0.5348702219490385774762362570840178448138e15), 
      SC_(0.3e2), SC_(0.941909885406494140625e1), SC_(-0.3952612900726184411209755573928998613679e11), 
      SC_(0.3e2), SC_(0.110517024993896484375e2), SC_(-0.4403298489517262739803601147531589885686e9), 
      SC_(0.3e2), SC_(0.139249114990234375e2), SC_(-0.8244863139099880895869870989454459989284e6), 
      SC_(0.3e2), SC_(0.15408351898193359375e2), SC_(-0.5927005358620390584889377499137602702052e5), 
      SC_(0.3e2), SC_(0.27344074249267578125e2), SC_(-0.5430085500226865192070534427655894031679e0), 
      SC_(0.3e2), SC_(0.273610286712646484375e2), SC_(-0.5397316653133181707330098871066578788555e0), 
      SC_(0.3e2), SC_(0.316179637908935546875e2), SC_(-0.1222206355136707773132399071091600011167e0), 
      SC_(0.3e2), SC_(0.4073618316650390625e2), SC_(-0.1473498529357165481092716822399322989769e0), 
      SC_(0.3e2), SC_(0.4175042724609375e2), SC_(-0.1329241150408283257931718001988734097587e0), 
      SC_(0.3e2), SC_(0.452895965576171875e2), SC_(0.1358912030639459939366319242804738135725e0), 
      SC_(0.3e2), SC_(0.45668792724609375e2), SC_(0.1341162547276849864669792140789503354908e0), 
      SC_(0.3e2), SC_(0.48443389892578125e2), SC_(-0.8628212563334357261211700506810224122887e-1), 
      SC_(0.33e2), SC_(0.48770198822021484375e1), SC_(-0.1693784284782744786165845355775974452761e23), 
      SC_(0.33e2), SC_(0.6349340915679931640625e1), SC_(-0.3194876101799053930195847096244427709109e19), 
      SC_(0.33e2), SC_(0.677385044097900390625e1), SC_(-0.3944901861540354328204900072177867423959e18), 
      SC_(0.33e2), SC_(0.941909885406494140625e1), SC_(-0.1045189818626325264118989008475901683727e14), 
      SC_(0.33e2), SC_(0.110517024993896484375e2), SC_(-0.6997251523653067084112138177574188638729e11), 
      SC_(0.33e2), SC_(0.139249114990234375e2), SC_(-0.6124024653828636752373676028415269079948e8), 
      SC_(0.33e2), SC_(0.15408351898193359375e2), SC_(-0.311143445577558554120437367987717666667e7), 
      SC_(0.33e2), SC_(0.27344074249267578125e2), SC_(-0.2131645568741100052609901708893666980648e1), 
      SC_(0.33e2), SC_(0.273610286712646484375e2), SC_(-0.2109339341477468335326929799639456480986e1), 
      SC_(0.33e2), SC_(0.316179637908935546875e2), SC_(-0.3494376433920678648399134899158871555997e0), 
      SC_(0.33e2), SC_(0.4073618316650390625e2), SC_(0.1095596495191871690624590338016882218436e0), 
      SC_(0.33e2), SC_(0.4175042724609375e2), SC_(0.1994018773601119665794680584215570776157e-1), 
      SC_(0.33e2), SC_(0.452895965576171875e2), SC_(-0.117141110298028054083962112086783615652e0), 
      SC_(0.33e2), SC_(0.45668792724609375e2), SC_(-0.9110882944580968804355001551429252507732e-1), 
      SC_(0.33e2), SC_(0.48443389892578125e2), SC_(0.1283726343387552512019823161796983521583e0), 
      SC_(0.36e2), SC_(0.48770198822021484375e1), SC_(-0.4514086098138169973226030174034617738601e26), 
      SC_(0.36e2), SC_(0.6349340915679931640625e1), SC_(-0.381527462358309928073555380802117838104e22), 
      SC_(0.36e2), SC_(0.677385044097900390625e1), SC_(-0.3864705277024927518681134460914641631118e21), 
      SC_(0.36e2), SC_(0.941909885406494140625e1), SC_(-0.3695521231167434412849111458028999950705e16), 
      SC_(0.36e2), SC_(0.110517024993896484375e2), SC_(-0.1495013134817261901719756533597851889758e14), 
      SC_(0.36e2), SC_(0.139249114990234375e2), SC_(-0.6196420279439685191659884689826287903961e10), 
      SC_(0.36e2), SC_(0.15408351898193359375e2), SC_(-0.2244917516984348627389323607107640171731e9), 
      SC_(0.36e2), SC_(0.27344074249267578125e2), SC_(-0.1545427238791694297400129988367380768401e2), 
      SC_(0.36e2), SC_(0.273610286712646484375e2), SC_(-0.1523915770558155873570691291184852714414e2), 
      SC_(0.36e2), SC_(0.316179637908935546875e2), SC_(-0.9471823176252754612212600210916028343601e0), 
      SC_(0.36e2), SC_(0.4073618316650390625e2), SC_(0.1173425902348136586318833789495612852355e0), 
      SC_(0.36e2), SC_(0.4175042724609375e2), SC_(0.161213752895956137670111737354907566266e0), 
      SC_(0.36e2), SC_(0.452895965576171875e2), SC_(-0.9809890796436754979940541980047075943712e-2), 
      SC_(0.36e2), SC_(0.45668792724609375e2), SC_(-0.4400083682264260689426444674935174217496e-1), 
      SC_(0.36e2), SC_(0.48443389892578125e2), SC_(-0.1217719085278386136058589800678093840029e0), 
      SC_(0.39e2), SC_(0.48770198822021484375e1), SC_(-0.1554597256004980110812710730381202390693e30), 
      SC_(0.39e2), SC_(0.6349340915679931640625e1), SC_(-0.5898284655302538808806988321276025813845e25), 
      SC_(0.39e2), SC_(0.677385044097900390625e1), SC_(-0.4904509402336609269919728103147054195654e24), 
      SC_(0.39e2), SC_(0.941909885406494140625e1), SC_(-0.1701017598526523993821571611323638754576e19), 
      SC_(0.39e2), SC_(0.110517024993896484375e2), SC_(-0.417530693969750949899048280358754304865e16), 
      SC_(0.39e2), SC_(0.139249114990234375e2), SC_(-0.82735284556625303260320668828414334725e12), 
      SC_(0.39e2), SC_(0.15408351898193359375e2), SC_(-0.2150905699012014456845831243381060537536e11), 
      SC_(0.39e2), SC_(0.27344074249267578125e2), SC_(-0.173136202360576514562595059683668653722e3), 
      SC_(0.39e2), SC_(0.273610286712646484375e2), SC_(-0.170234226215207199774450660598470420203e3), 
      SC_(0.39e2), SC_(0.316179637908935546875e2), SC_(-0.476564200718666827425515092297538626754e1), 
      SC_(0.39e2), SC_(0.4073618316650390625e2), SC_(-0.1139728443291009808277788472915789866764e0), 
      SC_(0.39e2), SC_(0.4175042724609375e2), SC_(-0.3789392025172586855842365357167594570805e-1), 
      SC_(0.39e2), SC_(0.452895965576171875e2), SC_(0.1632271611911285678989399827341146194315e0), 
      SC_(0.39e2), SC_(0.45668792724609375e2), SC_(0.1627608022568010137669993716629446289949e0), 
      SC_(0.39e2), SC_(0.48443389892578125e2), SC_(-0.3909325122429817340070823484524675983864e-2), 
      SC_(0.42e2), SC_(0.48770198822021484375e1), SC_(-0.6778761113696976108831204681937591870923e33), 
      SC_(0.42e2), SC_(0.6349340915679931640625e1), SC_(-0.1156175851690544099284032996632355510762e29), 
      SC_(0.42e2), SC_(0.677385044097900390625e1), SC_(-0.7895552037472749766365607521724398751657e27), 
      SC_(0.42e2), SC_(0.941909885406494140625e1), SC_(-0.9970250213779731769482155702312393531646e21), 
      SC_(0.42e2), SC_(0.110517024993896484375e2), SC_(-0.1489524208745775017256038034837378483054e19), 
      SC_(0.42e2), SC_(0.139249114990234375e2), SC_(-0.1421193372601400943383360047532395451619e15), 
      SC_(0.42e2), SC_(0.15408351898193359375e2), SC_(-0.266364645856547563022563347741616424841e13), 
      SC_(0.42e2), SC_(0.27344074249267578125e2), SC_(-0.2743088520517812194850488183554456816175e4), 
      SC_(0.42e2), SC_(0.273610286712646484375e2), SC_(-0.2690083634003523361552911979208904087728e4), 
      SC_(0.42e2), SC_(0.316179637908935546875e2), SC_(-0.384805041736847573781666237608273412272e2), 
      SC_(0.42e2), SC_(0.4073618316650390625e2), SC_(-0.3041722094268398285198996796280539333307e0), 
      SC_(0.42e2), SC_(0.4175042724609375e2), SC_(-0.237942599990390419264117350357852632925e0), 
      SC_(0.42e2), SC_(0.452895965576171875e2), SC_(-0.1769165908357241638186400615615275561375e-2), 
      SC_(0.42e2), SC_(0.45668792724609375e2), SC_(0.2627264307139144752250072910959109882562e-1), 
      SC_(0.42e2), SC_(0.48443389892578125e2), SC_(0.1595445273747734203624873579243265164658e0), 
      SC_(0.45e2), SC_(0.48770198822021484375e1), SC_(-0.3678158809277406631158967826248579712809e37), 
      SC_(0.45e2), SC_(0.6349340915679931640625e1), SC_(-0.2823290277495845250473189621249822637552e32), 
      SC_(0.45e2), SC_(0.677385044097900390625e1), SC_(-0.1584048114824233231915994262683772557843e31), 
      SC_(0.45e2), SC_(0.941909885406494140625e1), SC_(-0.7304744289889455012949611900475401935963e24), 
      SC_(0.45e2), SC_(0.110517024993896484375e2), SC_(-0.6658312289631602826704537808573997071741e21), 
      SC_(0.45e2), SC_(0.139249114990234375e2), SC_(-0.3075861182582574432943275662208173510923e17), 
      SC_(0.45e2), SC_(0.15408351898193359375e2), SC_(-0.4170869187140386729800751855187493963783e15), 
      SC_(0.45e2), SC_(0.27344074249267578125e2), SC_(-0.5841747054602714111411632231058985786773e5), 
      SC_(0.45e2), SC_(0.273610286712646484375e2), SC_(-0.5714873510732163760028674196101817926392e5), 
      SC_(0.45e2), SC_(0.316179637908935546875e2), SC_(-0.4439504087675916287818144789832290059488e3), 
      SC_(0.45e2), SC_(0.4073618316650390625e2), SC_(-0.7185408908955997967519881533646995655741e0), 
      SC_(0.45e2), SC_(0.4175042724609375e2), SC_(-0.5010756124409158401140594717316266404372e0), 
      SC_(0.45e2), SC_(0.452895965576171875e2), SC_(-0.2012825578255357825420510083651093929494e0), 
      SC_(0.45e2), SC_(0.45668792724609375e2), SC_(-0.1794186322536855399767456087221221709347e0), 
      SC_(0.45e2), SC_(0.48443389892578125e2), SC_(0.4019015729778697779336545231834378773906e-2), 
      SC_(0.48e2), SC_(0.48770198822021484375e1), SC_(-0.2446594481694463075060840320557933670105e41), 
      SC_(0.48e2), SC_(0.6349340915679931640625e1), SC_(-0.8459207003255131733462304451406482680612e35), 
      SC_(0.48e2), SC_(0.677385044097900390625e1), SC_(-0.3900585059507033418338520667611796016876e34), 
      SC_(0.48e2), SC_(0.941909885406494140625e1), SC_(-0.6584509350557438141050929296978384665502e27), 
      SC_(0.48e2), SC_(0.110517024993896484375e2), SC_(-0.3668948448136800346846820718406706247001e24), 
      SC_(0.48e2), SC_(0.139249114990234375e2), SC_(-0.8242020060037745753714812787166372607277e19), 
      SC_(0.48e2), SC_(0.15408351898193359375e2), SC_(-0.8108471636196350279814807195501067088811e17), 
      SC_(0.48e2), SC_(0.27344074249267578125e2), SC_(-0.161478450826312060240470478671833452801e7), 
      SC_(0.48e2), SC_(0.273610286712646484375e2), SC_(-0.1576027872751121787147362803310570448531e7), 
      SC_(0.48e2), SC_(0.316179637908935546875e2), SC_(-0.6897332324403501516059328674303313990342e4), 
      SC_(0.48e2), SC_(0.4073618316650390625e2), SC_(-0.2873562437345553411051362683296702801187e1), 
      SC_(0.48e2), SC_(0.4175042724609375e2), SC_(-0.1645050914310854634110662323482130128629e1), 
      SC_(0.48e2), SC_(0.452895965576171875e2), SC_(-0.4095205157936721724910226616684093867583e0), 
      SC_(0.48e2), SC_(0.45668792724609375e2), SC_(-0.3702549409277610055202576821426084296534e0), 
      SC_(0.48e2), SC_(0.48443389892578125e2), SC_(-0.1889064785963676989110283778976852771445e0), 
      SC_(0.51e2), SC_(0.48770198822021484375e1), SC_(-0.1969182678091137840672922747889170648771e45), 
      SC_(0.51e2), SC_(0.6349340915679931640625e1), SC_(-0.3069134439111231226532450141215242288989e39), 
      SC_(0.51e2), SC_(0.677385044097900390625e1), SC_(-0.1163353956243705422475270788219762868015e38), 
      SC_(0.51e2), SC_(0.941909885406494140625e1), SC_(-0.7202983789334716962996909315059777818759e30), 
      SC_(0.51e2), SC_(0.110517024993896484375e2), SC_(-0.245737676689172511133503788575172294214e27), 
      SC_(0.51e2), SC_(0.139249114990234375e2), SC_(-0.2693876853731198001028217985845998477702e22), 
      SC_(0.51e2), SC_(0.15408351898193359375e2), SC_(-0.1927060716887504354505080102215582890868e20), 
      SC_(0.51e2), SC_(0.27344074249267578125e2), SC_(-0.5643892233771978378997816826534156972962e8), 
      SC_(0.51e2), SC_(0.273610286712646484375e2), SC_(-0.5496011251862539066107336526262783112098e8), 
      SC_(0.51e2), SC_(0.316179637908935546875e2), SC_(-0.1389344839181512847834347831384757681674e6), 
      SC_(0.51e2), SC_(0.4073618316650390625e2), SC_(-0.1776984353347768195784367735005036727279e2), 
      SC_(0.51e2), SC_(0.4175042724609375e2), SC_(-0.872915584752688114103030525708943114106e1), 
      SC_(0.51e2), SC_(0.452895965576171875e2), SC_(-0.1161540076556705952629084092382499590939e1), 
      SC_(0.51e2), SC_(0.45668792724609375e2), SC_(-0.9803416521580276513923752432914532429705e0), 
      SC_(0.51e2), SC_(0.48443389892578125e2), SC_(-0.3793943440135667994381484637963819220083e0), 
      SC_(0.54e2), SC_(0.48770198822021484375e1), SC_(-0.1895955328653395662512658715131596618242e49), 
      SC_(0.54e2), SC_(0.6349340915679931640625e1), SC_(-0.1332861075842053738784438239373281251896e43), 
      SC_(0.54e2), SC_(0.677385044097900390625e1), SC_(-0.415400028628792112351061646920744162805e41), 
      SC_(0.54e2), SC_(0.941909885406494140625e1), SC_(-0.9448773063492740921951882034107555889314e33), 
      SC_(0.54e2), SC_(0.110517024993896484375e2), SC_(-0.1976226953367884232990383015948873832395e30), 
      SC_(0.54e2), SC_(0.139249114990234375e2), SC_(-0.1060239929445304895052402195312333832751e25), 
      SC_(0.54e2), SC_(0.15408351898193359375e2), SC_(-0.5524852310470588596167631811840899911653e22), 
      SC_(0.54e2), SC_(0.27344074249267578125e2), SC_(-0.2443271932118273203257306735627770090503e10), 
      SC_(0.54e2), SC_(0.273610286712646484375e2), SC_(-0.2374028629035348856642105835047755802793e10), 
      SC_(0.54e2), SC_(0.316179637908935546875e2), SC_(-0.3530147161153490224199678627355098967203e7), 
      SC_(0.54e2), SC_(0.4073618316650390625e2), SC_(-0.1523687692433405507809303644341462788826e3), 
      SC_(0.54e2), SC_(0.4175042724609375e2), SC_(-0.6588811991238001972711661692129876612553e2), 
      SC_(0.54e2), SC_(0.452895965576171875e2), SC_(-0.5340581063654305505018449302519149157836e1), 
      SC_(0.54e2), SC_(0.45668792724609375e2), SC_(-0.4241843123154779988294497423588841413993e1), 
      SC_(0.54e2), SC_(0.48443389892578125e2), SC_(-0.101007717644958422192422706197265435244e1), 
      SC_(0.57e2), SC_(0.48770198822021484375e1), SC_(-0.2161626205739229156929028218866774547264e53), 
      SC_(0.57e2), SC_(0.6349340915679931640625e1), SC_(-0.6857816478363427954638793152049033022474e46), 
      SC_(0.57e2), SC_(0.677385044097900390625e1), SC_(-0.1757633522876923346308719323318346692964e45), 
      SC_(0.57e2), SC_(0.941909885406494140625e1), SC_(-0.1470724735456049669352892370483504273101e37), 
      SC_(0.57e2), SC_(0.110517024993896484375e2), SC_(-0.1887826470413712223495666019943375481003e33), 
      SC_(0.57e2), SC_(0.139249114990234375e2), SC_(-0.4968500213339560352225854720671631559582e27), 
      SC_(0.57e2), SC_(0.15408351898193359375e2), SC_(-0.1888817184319266555840067091337856309212e25), 
      SC_(0.57e2), SC_(0.27344074249267578125e2), SC_(-0.1288212844049340614131327714502525981742e12), 
      SC_(0.57e2), SC_(0.273610286712646484375e2), SC_(-0.1249013008694464348198848991016934457599e12), 
      SC_(0.57e2), SC_(0.316179637908935546875e2), SC_(-0.1107695891983348156343774480706314333398e9), 
      SC_(0.57e2), SC_(0.4073618316650390625e2), SC_(-0.1713142312580323786184747142248961806243e4), 
      SC_(0.57e2), SC_(0.4175042724609375e2), SC_(-0.6604590530228345805227426149899214659609e3), 
      SC_(0.57e2), SC_(0.452895965576171875e2), SC_(-0.3518710444917558080090981427376821716428e2), 
      SC_(0.57e2), SC_(0.45668792724609375e2), SC_(-0.2663864616630346108683365093711526997299e2), 
      SC_(0.57e2), SC_(0.48443389892578125e2), SC_(-0.4306298772312023516324511417973550342892e1), 
      SC_(0.6e2), SC_(0.48770198822021484375e1), SC_(-0.2892081353968073031088937298478587490074e57), 
      SC_(0.6e2), SC_(0.6349340915679931640625e1), SC_(-0.4142396646874563143741981167064851524547e50), 
      SC_(0.6e2), SC_(0.677385044097900390625e1), SC_(-0.8732096096999191430049139445386679139699e48), 
      SC_(0.6e2), SC_(0.941909885406494140625e1), SC_(-0.2690978379323628746253084206502905589883e40), 
      SC_(0.6e2), SC_(0.110517024993896484375e2), SC_(-0.2121795509181260451828069876653538790412e36), 
      SC_(0.6e2), SC_(0.139249114990234375e2), SC_(-0.2744926792364829807723799177062944239004e30), 
      SC_(0.6e2), SC_(0.15408351898193359375e2), SC_(-0.76223015502590825948030726266442383048e27), 
      SC_(0.6e2), SC_(0.27344074249267578125e2), SC_(-0.8156824115957781369979544049155027772995e13), 
      SC_(0.6e2), SC_(0.273610286712646484375e2), SC_(-0.7891899080153060843746196671232001179968e13), 
      SC_(0.6e2), SC_(0.316179637908935546875e2), SC_(-0.4219761955150242610522454447404911542549e10), 
      SC_(0.6e2), SC_(0.4073618316650390625e2), SC_(-0.2439731391973693955896903470108472114229e5), 
      SC_(0.6e2), SC_(0.4175042724609375e2), SC_(-0.8451812456206044962273070236632882957476e4), 
      SC_(0.6e2), SC_(0.452895965576171875e2), SC_(-0.3079126889383895719027479423955437627905e3), 
      SC_(0.6e2), SC_(0.45668792724609375e2), SC_(-0.2235394837400507231196792387743951216596e3), 
      SC_(0.6e2), SC_(0.48443389892578125e2), SC_(-0.2621645109951344510244589118146666833865e2)
   };
#undef SC_


