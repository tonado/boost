//  Copyright (c) 2006 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Test data for RC, each row contains in order:
//
// x, y, RC(x, y)
//
#define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
   static const boost::array<boost::array<T, 3>, 201> ellint_rc_data = {
      SC_(0.11698430441812742785454260394458960094979845402414e-30), SC_(0.1429457475085533184e20), SC_(0.41546482167023371731518451342517736095263780557582e-9), 
      SC_(0.60682196926171104626767727515008970841312337712241e-30), SC_(0.20031258624e11), SC_(0.11098537606275153066313383027431717728910787699004e-4), 
      SC_(0.75974287571611502022633594876763844865744160884209e-30), SC_(0.20657736771171195551744e23), SC_(0.10928951720653730370271800759007590309935803177545e-10), 
      SC_(0.10711124272997777587041261201942927950011067906889e-29), SC_(0.47946235e7), SC_(0.71736902777915132997000781571354752397421561836787e-3), 
      SC_(0.22148209406780902835423848339554340607932845033396e-29), SC_(0.19855378956352178823888896e26), SC_(0.35251758550831167800571476411635458611576128740787e-12), 
      SC_(0.41366469991877353664505423235416110752209856263632e-29), SC_(0.1080339869874636584075679427013641468012674864323e-29), SC_(0.73882898278016317196709226502699714371366335165311e15), 
      SC_(0.44767214645416419053919245787147718165594030426536e-29), SC_(0.26553176031232e14), SC_(0.30483276103299498395114390375939883603035900740311e-6), 
      SC_(0.60078474312334545656635060929362382794743178141472e-29), SC_(0.37131016039637643189053051173686981201171875e-8), SC_(0.25778133019186154518263488635604654249614963342631e5), 
      SC_(0.61834249098451553748337548320711315606925672313013e-29), SC_(0.15125532639232e16), SC_(0.40389133730438812928090167163598082343384172921425e-7), 
      SC_(0.15386248307178551019952166983379913769903023636025e-28), SC_(0.11083928e9), SC_(0.14920144492544889019454092007748150269559896140113e-3), 
      SC_(0.19696330564073048658818435820590984305819001943432e-28), SC_(0.32238440243425309843308409068705787565729120602853e-24), SC_(0.27528315706771612956183484092479935495449666128525e13), 
      SC_(0.3000732163818651513042917353020530062720192427143e-28), SC_(0.28816120624542236328125e1), SC_(0.92534167116491481810744772274112199743911312596723e0), 
      SC_(0.47155488899394500240213479534565823420775918905622e-28), SC_(0.4463543296e10), SC_(0.23511481798013935669754503155612804985989597208762e-4), 
      SC_(0.65104581211365760324995297566040105840304400052279e-28), SC_(0.700861590985368820838630199432373046875e-9), SC_(0.59334016337111736885565181305039468621938956693965e5), 
      SC_(0.3949284499458758322165682321378970124179942071882e-27), SC_(0.670079424e9), SC_(0.60681559827956246896673839347875883912858354314319e-4), 
      SC_(0.55263283825603576259688637477087526891102295303199e-27), SC_(0.14570690537170526341649579027404115549870766699314e-16), SC_(0.41150783369497640278522169895393702894524362953078e9), 
      SC_(0.56127809703514701593677302775789618452802567406812e-27), SC_(0.9228809556838246663801328395493328571319580078125e-11), SC_(0.51706725181069699755346169652670461023196225991383e6), 
      SC_(0.12941088853490731622934705547460304161866623869764e-26), SC_(0.51905646303429040628284185654450766378431580960751e-16), SC_(0.21802745983479448951816941696856049925016007329412e9), 
      SC_(0.13705567485615801022446745431989965211687137938007e-26), SC_(0.5078582763671875e0), SC_(0.22041878790098954794667581190100145257683475089988e1), 
      SC_(0.16793731632787596740420928687663819571897812904515e-26), SC_(0.3518738464768e13), SC_(0.8373873246615759174577066128415186701403437615454e-6), 
      SC_(0.38344456302173223195286237326130017242619817813276e-26), SC_(0.1287535325337014953720268054032123824574619620157e-27), SC_(0.39122934559161731153423283680987267387840851196098e14), 
      SC_(0.39915753210320770987303899714279126384431587576396e-26), SC_(0.21695373623974084952946051325821291927687106275468e-28), SC_(0.52362555864098150198972240138118002904488963483036e14), 
      SC_(0.4719122320543576863524228422930565122003386188676e-26), SC_(0.13622372113626680630865559552e29), SC_(0.13458402336127820973876482234542361593374249767564e-13), 
      SC_(0.60865414403362585004788968196643994088311036341765e-26), SC_(0.123742392e9), SC_(0.14120842972598363583851550459968997099008965981731e-3), 
      SC_(0.10912435391066719332460695603753930316446923640128e-25), SC_(0.701712293691392e16), SC_(0.18751686532816272762803237369940986207556087504473e-7), 
      SC_(0.11837831633021164326650292224515978315498715423004e-25), SC_(0.27219574150194603134877979755401611328125e-7), SC_(0.95209265255492104735446619994567552559697239051341e4), 
      SC_(0.27826294978205525521980137859608542144911232096849e-25), SC_(0.87440356429873925658498856170228582591246249688943e-25), SC_(0.39786583496854506510922922833320145505046484128036e13), 
      SC_(0.34919368622379363366113344149995795887854964367758e-25), SC_(0.253342638931968e15), SC_(0.9868831456740266966933448023215284826954206363932e-7), 
      SC_(0.43573388456807634241895548780677530339229715228289e-25), SC_(0.4951682e8), SC_(0.22322534554145528116293051775479117197834119520022e-3), 
      SC_(0.60515264161661452673922225736467336588411333742066e-25), SC_(0.787468255232e13), SC_(0.55976191846459760569727531324370941602465103140201e-6), 
      SC_(0.14044194233643483429544463937103636612564738772591e-24), SC_(0.11511007425024644135191920213401317596435546875e-10), SC_(0.4629810115470076825533605597799208284244799296532e6), 
      SC_(0.14235380836999617679740726134029336127093459674064e-24), SC_(0.13978527726259656235898667313173260930711863658793e-24), SC_(0.26665402305818478969438170708975555299492041941187e13), 
      SC_(0.63171350201374448567926894124993295043246632758382e-24), SC_(0.34594001439795600424927311319582115491182941913878e-27), SC_(0.55978470361004464102520617099230932588604458370896e13), 
      SC_(0.1809900535358029636909945199492450026643175498009e-23), SC_(0.4521496521192602813243865966796875e-5), SC_(0.73871816148130469176849633363812844159103317042416e3), 
      SC_(0.63795086741914598182229839666991088459324643622494e-23), SC_(0.19953517994584643852448181812338140391502663839596e-24), SC_(0.97252851335184218861516452118643565455819826848263e12), 
      SC_(0.12701055793371805794043341177064064973639689881679e-22), SC_(0.2068948375e7), SC_(0.10920563536493094936157694716029214151487193559361e-2), 
      SC_(0.25335998731998849030698736206528910198942927678445e-22), SC_(0.99404931640625e3), SC_(0.49821398201934740455232191323883688893256512444693e-1), 
      SC_(0.71863877935764437100797607417958061393203905709015e-22), SC_(0.174722980453023744e19), SC_(0.11883513458141578715964474656644636676047017752235e-8), 
      SC_(0.33895318467300131244893458519885746926814107382597e-21), SC_(0.108193802361711277626454830169677734375e-6), SC_(0.47754980229555933492229104255715482771563363261078e4), 
      SC_(0.96362472102334232055222076765519517715929964651878e-21), SC_(0.229194643907248973846435546875e-3), SC_(0.1037570568888513512672421917866018283061872751937e3), 
      SC_(0.24141449950396592978032251394348434159198291126813e-20), SC_(0.18448819419086848e17), SC_(0.11564732376740008952525889248768816116526843336083e-7), 
      SC_(0.27589042212186362242123678200494962808875243354123e-20), SC_(0.16342684e8), SC_(0.3885600849080471885427919126603669380019521916121e-3), 
      SC_(0.32517390764125836699536241120728208287005145393778e-20), SC_(0.32148523727519219712e22), SC_(0.27703801044419253206437768255298726473435062914505e-10), 
      SC_(0.13713993780991436762807365477498389161326031171484e-19), SC_(0.805980574786256896e18), SC_(0.17496758182365486418837119046398196242651995908202e-8), 
      SC_(0.48988976909583188869227858043553514022505623870529e-19), SC_(0.20838384443777613341808319091796875e-4), SC_(0.34410251008331861623295558733232180042927229855377e3), 
      SC_(0.59220483766176692878700381014733800100202643079683e-19), SC_(0.170295034922225631232e22), SC_(0.38064390844573575145663807454900781472238386155909e-10), 
      SC_(0.82997499668329372719087076888666576479636205476709e-19), SC_(0.13155479315039595949803718770027355301939497866925e-23), SC_(0.21587999863350804994131547408624282253340872631383e11), 
      SC_(0.94969911521617292598785838378394608128019171999767e-19), SC_(0.16011657277825719347674748860299587249755859375e-10), SC_(0.39253685794445481713501328838333615978147538487421e6), 
      SC_(0.20581069188704517551523841788208013525718342862092e-18), SC_(0.79597584432992412060764095271814255738718202337623e-18), SC_(0.1350343783064271491301645561749747948509003384249e10), 
      SC_(0.22622955483503716568203720238550147314526839181781e-18), SC_(0.85913526136044117637383110697157917456934228539467e-16), SC_(0.16414615605631462414018849697787131696542279589859e9), 
      SC_(0.30420727664434581210352358596193944606511649908498e-18), SC_(0.28656053473241627216339111328125e-4), SC_(0.29343482785228047725924696469302777739683607703434e3), 
      SC_(0.5065993732313771067901500902430633743733778828755e-18), SC_(0.1568245533434264871175400912761688232421875e-11), SC_(0.12538793439957801193747997519423750597372445322011e7), 
      SC_(0.1269279595243605041993096779751049041351507185027e-17), SC_(0.33297255999405977634352134242323728585688513703644e-17), SC_(0.6307182542731725660001150747369468435224945316479e9), 
      SC_(0.12784377253954858878954355810853016350847610738128e-17), SC_(0.687874899968e12), SC_(0.1893935330545222150970846791789958509229989435048e-5), 
      SC_(0.29343589097392396787046010830923847834128537215292e-17), SC_(0.988066171875e5), SC_(0.49972014999347714801287931360926030236802518594537e-2), 
      SC_(0.46871312598868006987253009842930850936681963503361e-17), SC_(0.3093168493023691324522496e26), SC_(0.28243472271946788164939900071622955356799263239731e-12), 
      SC_(0.152616857880340486461855087352290638591512106359e-16), SC_(0.2803833618070566221119488e25), SC_(0.93808838274358563783082952025021048853623300403696e-12), 
      SC_(0.27022364375023134885640124780437076879024971276522e-16), SC_(0.27506543358959030582888269221280574638246675400223e-29), SC_(0.30108032875150741383088202176437518530075493145293e10), 
      SC_(0.55460104261295954577309919031335994077380746603012e-16), SC_(0.154132149174272e16), SC_(0.4001043378763428887183412693378271572857296764857e-7), 
      SC_(0.13209346463532805303342509972708285204134881496429e-15), SC_(0.3125807523929324815981090068817138671875e-7), SC_(0.8884250355437972613883213327503914013438652227259e4), 
      SC_(0.31354745663454523796342954256033408455550670623779e-15), SC_(0.18973853599391077295877039432525634765625e-8), SC_(0.3605203076290095918464079777019357642842666108285e5), 
      SC_(0.40558081566683637682324548023871102486737072467804e-15), SC_(0.208714828125e6), SC_(0.3438295704906441724421284923146054131214779732901e-2), 
      SC_(0.86986000872677090266227306614155168063007295131683e-15), SC_(0.6724360036363488659828113408e28), SC_(0.19155539931271849389875536772683217400458105317664e-13), 
      SC_(0.92657114094649205629505850367877428652718663215637e-15), SC_(0.32666126748015500425026402808725833892822265625e-10), SC_(0.27390641716269741695214339048257696917547990788968e6), 
      SC_(0.1327809695148788465823841420387907419353723526001e-14), SC_(0.54374770505732477601443242143375300656771287322044e-16), SC_(0.63904369494942025897253747392585085351726876366063e8), 
      SC_(0.16682115902222024485246354430501014576293528079987e-14), SC_(0.40149480284466451673885956097365124151110649108887e-13), SC_(0.69610251923240433370059407687180639587649479389139e7), 
      SC_(0.62660384591791979530528422515089914668351411819458e-14), SC_(0.199429788239058325416408479213714599609375e-7), SC_(0.11119107448133471075560965516334728421690408075844e5), 
      SC_(0.63024210653626119155168439078806841280311346054077e-14), SC_(0.83430262439221944230360872296614616061560809612274e-15), SC_(0.22574681584731394754127234813389452607870443810549e8), 
      SC_(0.10676147262794234704674778413391322828829288482666e-13), SC_(0.43214794983836329352138753913566138198376620493946e-21), SC_(0.89081662541541586914322012810403516608773228196705e8), 
      SC_(0.11175146230483004594624674155056709423661231994629e-13), SC_(0.13292268387650598312620966562043814396254504117678e-22), SC_(0.10375337151147634989665344794895368826014425089457e9), 
      SC_(0.14452686009774895481427847698796540498733520507813e-13), SC_(0.669478515625e5), SC_(0.6070878688152193299924869338064352690120583642415e-2), 
      SC_(0.15741248433312655941040958396115456707775592803955e-13), SC_(0.55817784918415767502848e23), SC_(0.66486516347017242385736020715678344161218353890444e-11), 
      SC_(0.20907895301679711719256005153511068783700466156006e-13), SC_(0.893892793119022144310292787849903106689453125e-9), SC_(0.52377290723662340541539667066133146373413622848047e5), 
      SC_(0.39424006729538510485078006695403018966317176818848e-13), SC_(0.57692237126522896384e20), SC_(0.2068050725918743676865501216749142009296095185495e-9), 
      SC_(0.5554841051654164918449652077470091171562671661377e-13), SC_(0.12544762676037794895951386908716784773787367157638e-18), SC_(0.30521794362592775449474387740993552064903872666301e8), 
      SC_(0.98092148011034996368806559985387139022350311279297e-13), SC_(0.8399241342592483328e19), SC_(0.54200069924858829039523381828914780837800840557603e-9), 
      SC_(0.10564504593401150001064081607182743027806282043457e-12), SC_(0.4479792022705078125e2), SC_(0.23468802898098123142592434447378499148543917622314e0), 
      SC_(0.15615465734711286849289990641409531235694885253906e-12), SC_(0.24870398826241113884932236596184834676037667122728e-20), SC_(0.24472819399433022007294333045527261944964286360397e8), 
      SC_(0.29870528793496387010009129880927503108978271484375e-12), SC_(0.2262512535863296e17), SC_(0.10442978405373505212946398659629346285587347485068e-7), 
      SC_(0.86422194270699415064029835775727406144142150878906e-12), SC_(0.20821377315431516209152e23), SC_(0.10885920287879685686355831941644040399805551265099e-10), 
      SC_(0.12200210459945015628591136191971600055694580078125e-11), SC_(0.56225779987930131445265275562229206527363467582603e-24), SC_(0.13486078631154888854229043546871905506012009735232e8), 
      SC_(0.164856496509546701645376742817461490631103515625e-11), SC_(0.12800646827237915914565746788866817951202392578125e-10), SC_(0.36045787009899043915638137502605942636753878032318e6), 
      SC_(0.1943433088069657088681196910329163074493408203125e-10), SC_(0.22031295035060773623229295812958821278778470675874e-24), SC_(0.37991980723075792959264812541886429306041092818746e7), 
      SC_(0.8669365225699721122509799897670745849609375e-10), SC_(0.11262596188998941215686500072479248046875e-7), SC_(0.14027640945609861854903798744913081835239816302217e5), 
      SC_(0.111468466401642984919817536137998104095458984375e-9), SC_(0.68559452379320356268932457055598206352442502975464e-14), SC_(0.52486878758492554321888354871667427243362154354677e6), 
      SC_(0.14367766698608619435617583803832530975341796875e-9), SC_(0.5059831664766534231603145599365234375e-6), SC_(0.21848877860361015086443267739440324861750113257735e4), 
      SC_(0.354709317473833607436972670257091522216796875e-9), SC_(0.34857664908449107348958795506280082952452713251912e-25), SC_(0.10153352525458852419843701240043332863605820898506e7), 
      SC_(0.573736336217933740044827573001384735107421875e-9), SC_(0.141051064161956674070097506046295166015625e-7), SC_(0.11757815303155685731623615095164779613218612398386e5), 
      SC_(0.151380081803154098452068865299224853515625e-7), SC_(0.94440849851249091320281320378524601844016927998382e-25), SC_(0.16662544565755989565448833912578784004220914986167e6), 
      SC_(0.11666782739894188125617802143096923828125e-6), SC_(0.34799534478224813938140869140625e-3), SC_(0.83236457192835769575662961408434019583738106058101e2), 
      SC_(0.68897207938789506442844867706298828125e-6), SC_(0.221162365880631296e20), SC_(0.33401368702824721858284957396270596689739407469446e-9), 
      SC_(0.1116302655645995400846004486083984375e-5), SC_(0.5191992844812585795584e22), SC_(0.21799816321471514057295223267883049361883182330942e-10), 
      SC_(0.1885123992906301282346248626708984375e-5), SC_(0.25172518744793432127678756566548684645107602196601e-26), SC_(0.18008547998944050904488110810216807100106707853798e5), 
      SC_(0.23743823476252146065235137939453125e-5), SC_(0.12017452733891625937800995416182558983564376831055e-12), SC_(0.59008749326752561914554583802676227170763409488603e4), 
      SC_(0.53310077419155277311801910400390625e-5), SC_(0.217494208512e12), SC_(0.3368185696185283123083955915747809814159015048245e-5), 
      SC_(0.63429242800339125096797943115234375e-5), SC_(0.31824542466005171886425636864e29), SC_(0.88051912373598395652506815894053991110568556947188e-14), 
      SC_(0.130162216009921394288539886474609375e-4), SC_(0.759344620746560394763946533203125e-4), SC_(0.14421506983819104132004852227111619430015782508182e3), 
      SC_(0.371529138647019863128662109375e-3), SC_(0.12312668720852486471439889495482766790246387245134e-18), SC_(0.96055276655007628911605449769323501052904206390342e3), 
      SC_(0.40748735773377120494842529296875e-3), SC_(0.57247653448122368e17), SC_(0.65650952593515499663260488879276797594548086476475e-8), 
      SC_(0.210290006361901760101318359375e-2), SC_(0.4932620110638545248192561152e28), SC_(0.22365625181037002220104830902172314909670879382756e-13), 
      SC_(0.47173579223453998565673828125e-2), SC_(0.28601321158264026057365024112938563807684451932578e-26), SC_(0.41603232866055243668483247942751627427269549962905e3), 
      SC_(0.2028485946357250213623046875e-1), SC_(0.43614054590079831541515886783599853515625e-7), SC_(0.50680436057151466590097864087821442993240691166811e2), 
      SC_(0.256623141467571258544921875e-1), SC_(0.33872525312e11), SC_(0.85348491273558027611269000760555604867934140869545e-5), 
      SC_(0.389046929776668548583984375e-1), SC_(0.18785430908203125e2), SC_(0.35227918956056369186636973233446582534230004889078e0), 
      SC_(0.39087764918804168701171875e-1), SC_(0.37496308203098109226999139757157200603110425163322e-21), SC_(0.12007602449751896613428312094975957780678427145037e3), 
      SC_(0.1238969862461090087890625e0), SC_(0.50384063720703125e3), SC_(0.69289771484876454297871646601530180487547603756684e-1), 
      SC_(0.4837694466114044189453125e0), SC_(0.2555249786376953125e2), SC_(0.28615961511581393939657047798947909654100110707466e0), 
      SC_(0.15793964862823486328125e1), SC_(0.21153786174505042274269802208549073334284835448335e-27), SC_(0.26085930256702446153426928501283414996469493899754e2), 
      SC_(0.3046932220458984375e1), SC_(0.18625525552055897944743393684375905872507558290159e-28), SC_(0.19665317857884549061595716355515640138874370283719e2), 
      SC_(0.12346179962158203125e2), SC_(0.304309702915784704e18), SC_(0.2847488476701153720061251397440169274771068622536e-8), 
      SC_(0.130841693878173828125e2), SC_(0.516425012058796509045456896e28), SC_(0.21858292984467318233636908716192880532684611323597e-13), 
      SC_(0.2496727752685546875e2), SC_(0.755542572949963187056710012257099151611328125e-10), SC_(0.27928314162875666520962642485579357074135329858573e1), 
      SC_(0.4151866912841796875e2), SC_(0.84098145009994339684283693509730169685090217512879e-22), SC_(0.4341003402255665800060030872597376661848978347545e1), 
      SC_(0.10465634918212890625e3), SC_(0.60640822994173504412174224853515625e-5), SC_(0.88219974915068377829851076473240001539884786059211e0), 
      SC_(0.204871795654296875e3), SC_(0.351293559140227787146657792e27), SC_(0.83807866206725702601826883835498669002259349569411e-13), 
      SC_(0.2247014312744140625e3), SC_(0.275117858615052709715247104e27), SC_(0.94702291082189117328396733175326455545223390155305e-13), 
      SC_(0.322791595458984375e3), SC_(0.86205957643638764405225050069248027284629642963409e-15), SC_(0.11646880911415179034823341402428681949283112375143e1), 
      SC_(0.4562802734375e3), SC_(0.10197021348467909816516131089894163913918803398168e-29), SC_(0.17922488338074155572310350987171414204572553246592e1), 
      SC_(0.7562174072265625e3), SC_(0.1910448768e10), SC_(0.35923470132422725783693134841062611631507906827816e-4), 
      SC_(0.82290283203125e3), SC_(0.36223742228003175114281475543975830078125e-7), SC_(0.43980399212423650879963940207747478236239652368737e0), 
      SC_(0.10313978271484375e4), SC_(0.10318525717298732830018437311991874594241380691528e-14), SC_(0.6668515395137059692868787992249145942381978211602e0), 
      SC_(0.11206973876953125e4), SC_(0.159823103248039936e18), SC_(0.39291632590134749018316722859590852663653580203117e-8), 
      SC_(0.3524259033203125e4), SC_(0.38758790625e6), SC_(0.23805498764247481510606154825690304471222889948051e-2), 
      SC_(0.3542151123046875e4), SC_(0.23456829198204715014597354638681281358003616333008e-13), SC_(0.34396142776310129846777566611486054989278031943136e0), 
      SC_(0.69560673828125e4), SC_(0.747194576263427734375e1), SC_(0.49317089833424179564861860197880854610400944287808e-1), 
      SC_(0.7298279296875e4), SC_(0.135008262029312e15), SC_(0.13518785304121054341303872941843981481438748034264e-6), 
      SC_(0.897631640625e4), SC_(0.386145068359375e4), SC_(0.13761011816758051951614138230181415517505541771123e-1), 
      SC_(0.147901005859375e5), SC_(0.62520772668534154026944814518561754825703991045316e-23), SC_(0.2648413871016930409485455149414286637028184421264e0), 
      SC_(0.815536015625e5), SC_(0.230844914913177490234375e0), SC_(0.24794349449595078621345239278260580833606450246749e-1), 
      SC_(0.84901359375e5), SC_(0.18469835205078125e4), SC_(0.90274392712796214783133197618962325121969170157594e-2), 
      SC_(0.131195640625e6), SC_(0.399027378608056683915492826862951233901632974721e-21), SC_(0.8619841670060382832194238459033456814758956298182e-1), 
      SC_(0.41534478125e6), SC_(0.1918984889984130859375e1), SC_(0.1060665265394308433482579591641745485349613369435e-1), 
      SC_(0.6869200625e6), SC_(0.12406291034494643099606037139892578125e-5), SC_(0.17148853609122838287334270730377713622015582671517e-1), 
      SC_(0.9115189375e6), SC_(0.179409878467662494683889664e27), SC_(0.11727263974665223656008245016259368445874373950871e-12), 
      SC_(0.39914275e7), SC_(0.5617260831058956682682037353515625e-4), SC_(0.66003352582444822055431410346869855793086865122376e-2), 
      SC_(0.15278292e8), SC_(0.14619622e8), SC_(0.25961118278381005988486780674736203564957112818546e-3), 
      SC_(0.15583587e8), SC_(0.46741176605224609375e2), SC_(0.17863256980859237735262630967043753358312118433161e-2), 
      SC_(0.2359947e8), SC_(0.83892250504513065845701871993405683214864787598623e-28), SC_(0.85438729190979830430450108794113432317121617101695e-2), 
      SC_(0.44006012e8), SC_(0.41172763385266176e17), SC_(0.77411561887565592752972899889991015699435496559546e-8), 
      SC_(0.4426536e8), SC_(0.89961878334825112180062577083017316681434749625623e-18), SC_(0.45500003788487494954911364150613981149754477134657e-2), 
      SC_(0.14248488e9), SC_(0.61731988312258099732254031872e29), SC_(0.63221492521852077836928323850507110790467251568351e-14), 
      SC_(0.156271296e9), SC_(0.13287944602780044078826904296875e-3), SC_(0.11670991383498154005718703499716739301912113499221e-2), 
      SC_(0.339495296e9), SC_(0.27455182077952e14), SC_(0.29911423091309748142593311126423073053561438997539e-6), 
      SC_(0.355727936e9), SC_(0.49794778812580822691949943719295546451682199506905e-24), SC_(0.20422111659287768080880666674695585733215224225899e-2), 
      SC_(0.1626075136e10), SC_(0.482386627197265625e2), SC_(0.23211090322482879766302355319080524745211784432509e-3), 
      SC_(0.2606333696e10), SC_(0.12266837021255184297929829995155159849673509597778e-13), SC_(0.53963700104507610983907942976446519710715041862358e-3), 
      SC_(0.3957605632e10), SC_(0.55799657429356219751070966594852507114410400390625e-13), SC_(0.42920548607924953119431231450313296279672960982957e-3), 
      SC_(0.7740245504e10), SC_(0.589840515072e13), SC_(0.63227008905693272145832965758127025031892199413741e-6), 
      SC_(0.10259405824e11), SC_(0.35736448257653877024582131372021365223190514370799e-17), SC_(0.31894320284009100162338080720486603695808737357409e-3), 
      SC_(0.14041162752e11), SC_(0.16802999673239973138530304e26), SC_(0.38320084631624641337451768839752529254504071282248e-12), 
      SC_(0.82854125568e11), SC_(0.58830391620982291911135882706044579926359106014644e-24), SC_(0.14299276138191576425667241629897528275040512189828e-3), 
      SC_(0.11738591232e12), SC_(0.761073704058645716941100545227527618408203125e-10), SC_(0.7322171577268687983119525908536326486897490571614e-4), 
      SC_(0.191893782528e12), SC_(0.174726296875e6), SC_(0.17458388731589946205436634365599236215476886257278e-4), 
      SC_(0.446171086848e12), SC_(0.49623249953612003082525916397571563720703125e-9), SC_(0.37153579565929854820799756427432381213691623554542e-4), 
      SC_(0.572994879488e12), SC_(0.4479686882443445386870784e25), SC_(0.742157274353212118943917435113707955824870795782e-12), 
      SC_(0.92746285056e12), SC_(0.7325884342193603515625e1), SC_(0.13992334087709732045223614104220216290282058103286e-4), 
      SC_(0.13073324703744e14), SC_(0.48555353730121618377670656e27), SC_(0.71285511551986730987092942275962922466144519563445e-13), 
      SC_(0.1591604150272e14), SC_(0.351564170056957952e18), SC_(0.26379290397755114021116829042194283811143014246219e-8), 
      SC_(0.20741449842688e14), SC_(0.4743499375e6), SC_(0.20837274754438050716836877714486012090201227681745e-5), 
      SC_(0.31191365320704e14), SC_(0.2293932139873504638671875e0), SC_(0.30376223823640976472682511381218417507215286561237e-5), 
      SC_(0.51712110886912e14), SC_(0.156551992965379627797503279104e30), SC_(0.39700005931080030246901295737647079077294776223606e-14), 
      SC_(0.14830236860416e15), SC_(0.57842544071306897015280826246375056598481023684144e-17), SC_(0.30262887085625134071904723108221624416890211410925e-5), 
      SC_(0.3135611338752e15), SC_(0.2348838144e10), SC_(0.37238603812796720490349747070177231425073166090273e-6), 
      SC_(0.323813657018368e15), SC_(0.156708486328125e5), SC_(0.69847661515884606972303830302067390332677255420951e-6), 
      SC_(0.530327327997952e15), SC_(0.10366431646728266850195727608041629252966231433675e-18), SC_(0.17153262952128191564833782071342486326068147437365e-5), 
      SC_(0.727262651482112e15), SC_(0.1535589888e10), SC_(0.26799428982612804774219551247863136845480307410692e-6), 
      SC_(0.95759886188544e15), SC_(0.4659874708323741288040764629840850830078125e-8), SC_(0.8897375003298093960305950194491436152687587334261e-6), 
      SC_(0.1551973144854528e16), SC_(0.10139344880747031860591903938868807433237861914677e-19), SC_(0.10458468387194864418446638976290709697775378499213e-5), 
      SC_(0.183459460939776e16), SC_(0.58535432060362684618705258305616519358052915120161e-26), SC_(0.11315636043901720121388837656247249664177763590949e-5), 
      SC_(0.27152649027584e16), SC_(0.66861986169897136278450489044189453125e-6), SC_(0.49072897074683974426280640967267909946582802584589e-6), 
      SC_(0.3762704347037696e16), SC_(0.10164524532769642241629852708355797252185587220552e-24), SC_(0.77271884197062324056183842244132156703113101877894e-6), 
      SC_(0.6551729407524864e16), SC_(0.25762003497220575809478759765625e-3), SC_(0.28457658270165863549014595037995212294784586710095e-6), 
      SC_(0.9007602981666816e16), SC_(0.25765624e9), SC_(0.98811088491194357965387716654259888241690606624295e-7), 
      SC_(0.31614863423832064e17), SC_(0.64014885e7), SC_(0.66664506125039749834276834929108360333353634282303e-7), 
      SC_(0.45080659638616064e17), SC_(0.66192871423119660955992064e26), SC_(0.19306655679013338902506375891076552233340289260346e-12), 
      SC_(0.59877638017122304e17), SC_(0.53512100536178474827209150532780768116936087608337e-15), SC_(0.15362013975191677793880105907602135335521807551786e-6), 
      SC_(0.128395461743607808e18), SC_(0.206836903384823477636694016e27), SC_(0.10921919594473705616931168879025397857363831649357e-12), 
      SC_(0.140845240494850048e18), SC_(0.15238641357421875e4), SC_(0.44689988476580111818524171778021205499463465100743e-7), 
      SC_(0.245924553149120512e18), SC_(0.12432241406593107741605394588021265875441186132822e-25), SC_(0.10191370089476240496442184751961162333606213558986e-6), 
      SC_(0.10302131894484992e19), SC_(0.86229714468864e14), SC_(0.53078949279350852244495899285397248236587855360507e-8), 
      SC_(0.4173026783555223552e19), SC_(0.25325901731494375978251987705874090841683295149966e-25), SC_(0.25259325443772059313156731529761618841007461413987e-7), 
      SC_(0.38083625921602912256e20), SC_(0.14043506688e11), SC_(0.18721812477205878318873337191745626686057078380972e-8), 
      SC_(0.6036870791327383552e20), SC_(0.7191220191232e13), SC_(0.11151877475097208967247771680064286641451016014729e-8), 
      SC_(0.140771054246300745728e21), SC_(0.4364620208740234375e2), SC_(0.18544022482541756413624180462592298070565733208098e-8), 
      SC_(0.229345916576958775296e21), SC_(0.181080293376e13), SC_(0.66174853669110756842649598160820882183579693595045e-9), 
      SC_(0.486625867239520206848e21), SC_(0.2663514463837359985504355062296832912238642165903e-19), SC_(0.21326856610092306285181605511492710866111336973419e-8), 
      SC_(0.1241846768099137683456e22), SC_(0.16907547929525060340181807204941172504655995389999e-25), SC_(0.15507902235497089042906909015124525282864868106372e-8), 
      SC_(0.2085586588137791946752e23), SC_(0.1494682042368e13), SC_(0.85673852855596798336664141721336700864507390102108e-10), 
      SC_(0.140444835444342210428928e24), SC_(0.468889537015105749778432e24), SC_(0.17303641617108978771183397785345947512931840394585e-11), 
      SC_(0.277204771630374352584704e24), SC_(0.178649388253688812255859375e-1), SC_(0.56400743476075289901756964355291370430816192512748e-10), 
      SC_(0.292186842467552253181952e24), SC_(0.8000304500511383121995759616e28), SC_(0.17494456350937238152051998129209403413657779766045e-13), 
      SC_(0.404188410838900933656576e24), SC_(0.15307389070737408e17), SC_(0.14530172405497225203148631842861632557097281219397e-10), 
      SC_(0.432213410600102056165376e24), SC_(0.1959631583933896656101239768660304818581607833039e-18), SC_(0.75206449301552958256440392567562049430798996389352e-10), 
      SC_(0.48688847917066659823616e24), SC_(0.12212552573408426924125600686592708205013835254249e-21), SC_(0.76232145024480748870634232091696723820904729119412e-10), 
      SC_(0.1611108507323484384264192e25), SC_(0.11971012087915599989354498156046702206367626786232e-15), SC_(0.3694442785175160419076949350029585599009031148763e-10), 
      SC_(0.1626183676572159287754752e26), SC_(0.18501463382170069138510370976291596889495849609375e-11), SC_(0.10719260914536354712818827718038676299273171700718e-10), 
      SC_(0.18820064668587267094216704e26), SC_(0.302210201308383830109960399568080902099609375e-9), SC_(0.93936394693848721355885906623218887308774325852727e-11), 
      SC_(0.24925849334950778558742528e26), SC_(0.31317834636651731435069138009717243018286553235541e-24), SC_(0.11646010681447365669777654446715227593887198721003e-10), 
      SC_(0.9036469625899950631026688e26), SC_(0.690641880035400390625e1), SC_(0.31148497090217722784373888164259764652317411281318e-11), 
      SC_(0.7376334675354954747676196864e28), SC_(0.3832167254355459644063744e25), SC_(0.52109798218409315418287118331564136634721278594953e-13), 
      SC_(0.19445130267235161963321360384e29), SC_(0.27158976561908717618255837411567199524142779409885e-16), SC_(0.37530178368631060137545101809988924309836018928577e-12), 
   };
#undef SC_


