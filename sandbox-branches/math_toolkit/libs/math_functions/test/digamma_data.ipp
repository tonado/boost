#define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
   static const boost::array<boost::array<T, 2>, 500> digamma_data = {
      SC_(2.818432331085205078125), SC_(0.8484115700906551606307984398000472347785), 
      SC_(4.6342258453369140625), SC_(1.421713669467331557347601964119226105014), 
      SC_(4.783483982086181640625), SC_(1.457016504476551729585831562031964212238), 
      SC_(8.0939121246337890625), SC_(2.02806725001046109435006913084666450899), 
      SC_(11.90207004547119140625), SC_(2.434114987242530269375534072241650635459), 
      SC_(13.53912639617919921875), SC_(2.568199379535107033473901142572267131293), 
      SC_(15.40344142913818359375), SC_(2.70177959979566700235213147968567613712), 
      SC_(16.5205841064453125), SC_(2.774036634025874647373138331993451055497), 
      SC_(17.7738971710205078125), SC_(2.849336085709903318587899668469823726457), 
      SC_(18.45084381103515625), SC_(2.887766358793563776387986802412157943366), 
      SC_(29.2202816009521484375), SC_(3.357654052423628189208902434898861034575), 
      SC_(31.832843780517578125), SC_(3.444709303236843682984386511584019805155), 
      SC_(33.268566131591796875), SC_(3.48950850367834697800598135337384460165), 
      SC_(34.31731414794921875), SC_(3.521009352966155255811365379184456009466), 
      SC_(34.446079254150390625), SC_(3.524809514386367110160928575889334171204), 
      SC_(35.711681365966796875), SC_(3.561411485674988633898237821294494311983), 
      SC_(36.441249847412109375), SC_(3.581917907724542515429931900299321464116), 
      SC_(40.471111297607421875), SC_(3.688183052096727944017539059038660866267), 
      SC_(42.0541229248046875), SC_(3.727020872145296937504698477067321294092), 
      SC_(43.023799896240234375), SC_(3.750086956755859174992028774044963741414), 
      SC_(45.059543609619140625), SC_(3.796847336482220986142117794493357261772), 
      SC_(46.171390533447265625), SC_(3.821492048093989120306052355659348850595), 
      SC_(47.944286346435546875), SC_(3.859574612789685102190529392545009249772), 
      SC_(49.654422760009765625), SC_(3.894984071254501776336224194667579417761), 
      SC_(51.216426849365234375), SC_(3.92626605711337968996352527100620814691), 
      SC_(53.152568817138671875), SC_(3.963730056566564459495465774334833292077), 
      SC_(53.9501190185546875), SC_(3.978763447797611600078998595828792990884), 
      SC_(59.779544830322265625), SC_(4.082276159182205975531619692613299061202), 
      SC_(67.59537506103515625), SC_(4.206124370756245048917505923080906736667), 
      SC_(75.8542938232421875), SC_(4.322208246185371494736862321857785356042), 
      SC_(75.96668243408203125), SC_(4.323698582444848954937627922600414958993), 
      SC_(77.55702972412109375), SC_(4.344552810478025331117690547449165135348), 
      SC_(78.17552947998046875), SC_(4.352547177563450363247154506704116472633), 
      SC_(81.125762939453125), SC_(4.389824647463436852929138346906265829317), 
      SC_(83.821380615234375), SC_(4.422711187859794187691096231764756635029), 
      SC_(84.43585205078125), SC_(4.430058755062111563873245625234033772093), 
      SC_(85.5157928466796875), SC_(4.442842802158912826398856512764451371231), 
      SC_(89.82315826416015625), SC_(4.492266007498633968205232190450336649676), 
      SC_(95.3551788330078125), SC_(4.552355925956436228488375948364274467585), 
      SC_(96.45452880859375), SC_(4.563878946335176406579712835902366153383), 
      SC_(97.13177490234375), SC_(4.570912081982896265651973317486608671594), 
      SC_(97.54039764404296875), SC_(4.575131786976403596191579466163638961361), 
      SC_(100.73848724365234375), SC_(4.607556365921065105146592555963264085078), 
      SC_(106.65277099609375), SC_(4.664882989935773341843363089846541745757), 
      SC_(108.061920166015625), SC_(4.678070284634890633120327580057526752699), 
      SC_(109.86175537109375), SC_(4.694664727381252692793513795934630009291), 
      SC_(111.1192169189453125), SC_(4.706097230556212718518769127144178200579), 
      SC_(111.20275115966796875), SC_(4.706852091693387791616938432500996073933), 
      SC_(112.46451568603515625), SC_(4.718185320564378713782753437437734301757), 
      SC_(113.192535400390625), SC_(4.724666465501466074724041170795702750496), 
      SC_(117.41765594482421875), SC_(4.761472939730569483093207984677455277661), 
      SC_(118.9976806640625), SC_(4.774896355243431821176509841645237231067), 
      SC_(119.54718017578125), SC_(4.779522826588010949258957921967185987839), 
      SC_(123.31893157958984375), SC_(4.81071393197534640354115364663320564994), 
      SC_(125.18276214599609375), SC_(4.825775288759329337505915366578532955896), 
      SC_(125.89662933349609375), SC_(4.83148439831388328994923062624365450503), 
      SC_(126.98681640625), SC_(4.840140688918909476658690945857238201803), 
      SC_(129.846527099609375), SC_(4.86249754984079778426805097304440742781), 
      SC_(129.9062042236328125), SC_(4.862958815139702327034828252875472153454), 
      SC_(131.973297119140625), SC_(4.878806177776566374942264024665615055025), 
      SC_(132.99603271484375), SC_(4.886555076645559019076170710371114254694), 
      SC_(133.73638916015625), SC_(4.892127259828019964975240621714384800823), 
      SC_(135.4770050048828125), SC_(4.905106717626831988577402303939056656494), 
      SC_(136.0685577392578125), SC_(4.90947974061898410128546715429801925019), 
      SC_(138.0013275146484375), SC_(4.923635775505805776701767785882153544687), 
      SC_(138.62445068359375), SC_(4.928157279267557841707446444163947285747), 
      SC_(140.1438446044921875), SC_(4.939097350804489336186984804448028468727), 
      SC_(141.8863372802734375), SC_(4.951498208532141218325182302525386281704), 
      SC_(144.954803466796875), SC_(4.972968676588775902921394002712748457896), 
      SC_(145.5389862060546875), SC_(4.977004557354005944430878448659040833139), 
      SC_(147.829010009765625), SC_(4.992670168744977024725477253611297229446), 
      SC_(149.113983154296875), SC_(5.001354113685057718576710684744348466536), 
      SC_(149.29400634765625), SC_(5.002564723643670955839282486237921850327), 
      SC_(152.378021240234375), SC_(5.023079513160560927910374282855545874691), 
      SC_(154.43841552734375), SC_(5.036554381553671577576060432491657040309), 
      SC_(157.6130828857421875), SC_(5.056967507189124773825675779780432867978), 
      SC_(158.057586669921875), SC_(5.059792699534529307362125473412020388939), 
      SC_(162.1822967529296875), SC_(5.085634872441444055790538285561513352114), 
      SC_(162.3285064697265625), SC_(5.086538763499375385825459901390036092276), 
      SC_(162.6117401123046875), SC_(5.088287436707226991346072251084741331228), 
      SC_(162.8721160888671875), SC_(5.089892294410720846921196992521724082324), 
      SC_(165.6487274169921875), SC_(5.106847973878888830832110337446518371119), 
      SC_(167.5558013916015625), SC_(5.118329389684532657993072571814825296866), 
      SC_(168.9900360107421875), SC_(5.126878082277710058624325179569901288279), 
      SC_(170.708038330078125), SC_(5.137022881977642343120933693408155267379), 
      SC_(171.1866912841796875), SC_(5.139831091399062175194966875853774050215), 
      SC_(173.86517333984375), SC_(5.155401584700129074352914825821571994537), 
      SC_(181.572967529296875), SC_(5.198901356355351487232884915474078625454), 
      SC_(181.847015380859375), SC_(5.200413674777345067384965017459287722255), 
      SC_(183.5111541748046875), SC_(5.209548346790303183756319086272109508414), 
      SC_(183.9077911376953125), SC_(5.211713279243515056757039824210382332605), 
      SC_(184.816314697265625), SC_(5.216654609736436280673189918782642034751), 
      SC_(186.8726043701171875), SC_(5.227749118747922979749925127240621358077), 
      SC_(188.3819732666015625), SC_(5.235815144271654507738040431638317824219), 
      SC_(188.9550018310546875), SC_(5.238860434511886935491539718311895522563), 
      SC_(194.4953765869140625), SC_(5.267835433896789878679452996831857175441), 
      SC_(194.7642822265625), SC_(5.269220615601523996859748369572800146799), 
      SC_(196.595245361328125), SC_(5.278601570477420849009352984342881774012), 
      SC_(199.55120849609375), SC_(5.293563172363173736409123622092103582665), 
      SC_(199.87286376953125), SC_(5.29517780707170337549563059440671599137), 
      SC_(207.7422943115234375), SC_(5.333889582810755539805705568562512261187), 
      SC_(208.06805419921875), SC_(5.335460225032043059439507730667027716784), 
      SC_(210.209075927734375), SC_(5.345722164946006440059254445596129365136), 
      SC_(210.3018798828125), SC_(5.346164602882524533186213817875265383748), 
      SC_(211.9243316650390625), SC_(5.353868096697531240993528975318640869535), 
      SC_(213.7727508544921875), SC_(5.362572783030003908578204737735026180568), 
      SC_(217.2378387451171875), SC_(5.378689393785450240762515278142610186647), 
      SC_(221.0340576171875), SC_(5.396052995987518692513315434729480482768), 
      SC_(221.746734619140625), SC_(5.399279376199210442291570588468305439493), 
      SC_(223.419952392578125), SC_(5.406813587612010012477220821227640963526), 
      SC_(223.8119354248046875), SC_(5.40857044312848898111490039577861789539), 
      SC_(225.921783447265625), SC_(5.417974060411836810226948440207258474062), 
      SC_(227.6642913818359375), SC_(5.425674313715258008117354357369952302663), 
      SC_(228.976959228515625), SC_(5.431436168546949625044655797954749386032), 
      SC_(230.15606689453125), SC_(5.436583618434440970405411512638103654567), 
      SC_(230.4881591796875), SC_(5.438028613498667787626776999061156299023), 
      SC_(234.7799072265625), SC_(5.45651734435963939743321146310693692221), 
      SC_(237.2835693359375), SC_(5.467147257515587656692603812143613587167), 
      SC_(239.5024566650390625), SC_(5.47647456024034998344432612758666650177), 
      SC_(239.916168212890625), SC_(5.47820405414781872212296108547269195884), 
      SC_(239.9525146484375), SC_(5.47835585519092106650208297916994251088), 
      SC_(240.0759429931640625), SC_(5.478871182270947292729700821285916853014), 
      SC_(241.6912841796875), SC_(5.48559104520771247073410657218711428497), 
      SC_(243.52496337890625), SC_(5.493164873500147032216227026263947788629), 
      SC_(244.138275146484375), SC_(5.495685348360518575357280836637213941973), 
      SC_(247.84759521484375), SC_(5.510795296469788210349267225408057089008), 
      SC_(251.0838470458984375), SC_(5.523794246827039306466724212993790091993), 
      SC_(252.9559783935546875), SC_(5.53123754427692590099511415861194353476), 
      SC_(254.2821807861328125), SC_(5.53647699008893800801173473697704786508), 
      SC_(255.095123291015625), SC_(5.539675174361895155905420362931642677874), 
      SC_(257.50823974609375), SC_(5.54910877645277178885293455204990306081), 
      SC_(258.064697265625), SC_(5.551271568142368038329308299350794617341), 
      SC_(258.2958984375), SC_(5.552168807470126931792259144648002540331), 
      SC_(259.87042236328125), SC_(5.558257861474442709235556716423323490943), 
      SC_(262.21173095703125), SC_(5.567244242836903141874863560065650986603), 
      SC_(262.48223876953125), SC_(5.568277317584441062906205258279469903793), 
      SC_(262.971282958984375), SC_(5.570142282612872731717692549388720316202), 
      SC_(263.80291748046875), SC_(5.573305747885775172479587641948129399505), 
      SC_(265.920654296875), SC_(5.581316533594154573012665212950951963344), 
      SC_(272.753021240234375), SC_(5.606732421189819995671142457643674993514), 
      SC_(276.02508544921875), SC_(5.618679227574827562968855008817945055844), 
      SC_(276.9229736328125), SC_(5.62193275108461049287671704761080177701), 
      SC_(278.49822998046875), SC_(5.627615285836316944304326354848356653533), 
      SC_(281.6273193359375), SC_(5.638808187647616258573018310333124752979), 
      SC_(282.673309326171875), SC_(5.642521977550928843609142257361465463159), 
      SC_(285.839019775390625), SC_(5.653678528153416638005857155381996231704), 
      SC_(287.80499267578125), SC_(5.660544848531287229964224746346326537309), 
      SC_(296.32080078125), SC_(5.689754344363163686181089294681536867498), 
      SC_(296.675872802734375), SC_(5.690953917746555781396415233069043398008), 
      SC_(297.029449462890625), SC_(5.692147011020137765888724960240975571168), 
      SC_(299.83172607421875), SC_(5.701552875188694305814208034977343378525), 
      SC_(301.246337890625), SC_(5.706267638858004649656056828257867279121), 
      SC_(301.827301025390625), SC_(5.708197511695138802806710871639099088268), 
      SC_(301.913116455078125), SC_(5.708482262307644688021338490250719094243), 
      SC_(303.61395263671875), SC_(5.714109269616552222989399906960963535438), 
      SC_(306.3494873046875), SC_(5.723093554813365658046336277832734920358), 
      SC_(308.16705322265625), SC_(5.729018642421226976646437744710250114317), 
      SC_(311.102294921875), SC_(5.738513731255894331377758163498728167044), 
      SC_(311.215057373046875), SC_(5.738876709545896308969873915166375996455), 
      SC_(312.50799560546875), SC_(5.743029242434299652839488723689056915437), 
      SC_(316.550445556640625), SC_(5.755902255817228423385699779528788254728), 
      SC_(317.0994873046875), SC_(5.757637943754943613314524976138239539917), 
      SC_(318.7783203125), SC_(5.762926632420383028466536214352273605745), 
      SC_(322.5107421875), SC_(5.774585307653271847190205931939723481736), 
      SC_(324.963531494140625), SC_(5.782173541925585116003437602057941223569), 
      SC_(332.448272705078125), SC_(5.804979529807743662919055583913702847441), 
      SC_(337.122650146484375), SC_(5.818962938187135636742305501932989080651), 
      SC_(337.71942138671875), SC_(5.820734186921434094429909639202086179781), 
      SC_(340.219696044921875), SC_(5.828121215221842999606169529284319625934), 
      SC_(340.3857421875), SC_(5.828609869422364475855293647958724101162), 
      SC_(343.83953857421875), SC_(5.838720219369554242298305075518187973959), 
      SC_(343.92974853515625), SC_(5.838982927383424836208079234708676764532), 
      SC_(344.32794189453125), SC_(5.840141715792665213495451845063858259471), 
      SC_(346.2333984375), SC_(5.845668301886241439514342457852325235755), 
      SC_(349.9837646484375), SC_(5.856457448655756722565805616029229423181), 
      SC_(350.72711181640625), SC_(5.858582174643380806977967882520602080122), 
      SC_(350.952392578125), SC_(5.859225209340739029885042670228181902477), 
      SC_(351.659515380859375), SC_(5.861240917897877761839636580241630893874), 
      SC_(352.76239013671875), SC_(5.864376660211097551985117273619920161247), 
      SC_(353.1585693359375), SC_(5.865500698218050522379186042690723838268), 
      SC_(354.63812255859375), SC_(5.869687345986841682980604946721230334574), 
      SC_(356.34515380859375), SC_(5.874496004070293633977629973243397192974), 
      SC_(360.8607177734375), SC_(5.887105844462842568231690256873044524566), 
      SC_(361.29400634765625), SC_(5.888307496020129242675686611572690424192), 
      SC_(363.18646240234375), SC_(5.893539037844593630113143804047702478072), 
      SC_(368.484588623046875), SC_(5.908041366251291466893815884478290769093), 
      SC_(369.246795654296875), SC_(5.910110524295502063829120796381053011945), 
      SC_(378.50042724609375), SC_(5.934895617510447498576865442817972697249), 
      SC_(378.609375), SC_(5.935183797060069242663195486702002966313), 
      SC_(379.418853759765625), SC_(5.937322366182591168893192048144610446318), 
      SC_(380.445831298828125), SC_(5.940028981977972573511311112384361555998), 
      SC_(381.558441162109375), SC_(5.942953039189986310250537695096760920284), 
      SC_(382.33294677734375), SC_(5.944983486642878955588861619779712183753), 
      SC_(385.71051025390625), SC_(5.953790245535102839028892159533990782538), 
      SC_(387.2958984375), SC_(5.957897438189758732168023760232019857062), 
      SC_(388.569793701171875), SC_(5.961185480793951888080130571435866339859), 
      SC_(388.61517333984375), SC_(5.961302410689641047121043666111858152527), 
      SC_(389.73883056640625), SC_(5.964193390561584996249624057879172484094), 
      SC_(392.227020263671875), SC_(5.970565491861186670927570593593922746651), 
      SC_(392.320465087890625), SC_(5.970804009055547779176675482333698236361), 
      SC_(393.58984375), SC_(5.974038465445461053564629207897789530972), 
      SC_(394.908233642578125), SC_(5.977386766739159526527423143050993473122), 
      SC_(398.738525390625), SC_(5.987051398521802594134573229887245743555), 
      SC_(399.78265380859375), SC_(5.98966983297736662713511729544362351217), 
      SC_(401.808013916015625), SC_(5.994729505682939003379681261442316095543), 
      SC_(403.912139892578125), SC_(5.999958975030022983362581727689153017309), 
      SC_(404.20849609375), SC_(6.000693328854312027033338977366250530793), 
      SC_(404.98138427734375), SC_(6.002605968553351055079821088079186595986), 
      SC_(408.7198486328125), SC_(6.011806124002334414126565011018849669266), 
      SC_(408.731170654296875), SC_(6.011833858711596841091740153684611585383), 
      SC_(412.666534423828125), SC_(6.021427722714450543272375426891465502276), 
      SC_(417.267059326171875), SC_(6.032527694873243105931087101820210964016), 
      SC_(420.18994140625), SC_(6.039516440455429955766164077664900139336), 
      SC_(421.761260986328125), SC_(6.043253448414102502433018626928644727058), 
      SC_(422.087677001953125), SC_(6.044028002107108186220134524106733043847), 
      SC_(423.16510009765625), SC_(6.046580373164618907726121091198392578715), 
      SC_(424.166778564453125), SC_(6.048947478773228346602304770587362502163), 
      SC_(430.2073974609375), SC_(6.063104731694863158498203027307993195815), 
      SC_(431.413818359375), SC_(6.065908337098829708316996099109173916052), 
      SC_(435.245697021484375), SC_(6.074761476046398186404025268023433471772), 
      SC_(435.698699951171875), SC_(6.075802928287907510845374398686273404793), 
      SC_(435.85858154296875), SC_(6.07617023673887939600287017602598678356), 
      SC_(436.1175537109375), SC_(6.076764907594542344403398466659178736447), 
      SC_(438.66680908203125), SC_(6.08259989755215167271203767330669598323), 
      SC_(438.744354248046875), SC_(6.082776858174768286116463240157641988703), 
      SC_(438.8699951171875), SC_(6.083063508320507989449416901329106432625), 
      SC_(441.223480224609375), SC_(6.088417864327000147587478272648801388231), 
      SC_(442.67828369140625), SC_(6.091713370624069002395664415060658196535), 
      SC_(445.586212158203125), SC_(6.09826820891842885583333776024666913377), 
      SC_(446.78375244140625), SC_(6.100955175158840019640884425000576615711), 
      SC_(447.584381103515625), SC_(6.102747557295619121106102670653678229036), 
      SC_(449.55609130859375), SC_(6.107148011530301966053822903337711218286), 
      SC_(450.541595458984375), SC_(6.10934021856897802099580290683898433424), 
      SC_(450.9237060546875), SC_(6.110188914150944431871753996420181358557), 
      SC_(456.032806396484375), SC_(6.121467937724061922128554049058015478328), 
      SC_(457.989166259765625), SC_(6.125753403208273372726190971914060421286), 
      SC_(458.49688720703125), SC_(6.126862586238352284366184295285682829153), 
      SC_(458.848846435546875), SC_(6.127630765961755033027033241022489856889), 
      SC_(462.47418212890625), SC_(6.135509201915263462051894175844019358072), 
      SC_(469.390625), SC_(6.150369721809370729825444412044817108946), 
      SC_(470.92333984375), SC_(6.153633200651127889846979557070586407071), 
      SC_(471.956787109375), SC_(6.155827635510642589118081435451236816658), 
      SC_(473.01507568359375), SC_(6.158069839252994031314486039965613313878), 
      SC_(473.288848876953125), SC_(6.15864906693157877173538018092295228381), 
      SC_(474.758697509765625), SC_(6.161753133400770857335496852979821817608), 
      SC_(476.638092041015625), SC_(6.165708105278002487733159955708518039802), 
      SC_(485.375640869140625), SC_(6.183892624705687372974280692449022738796), 
      SC_(486.7916259765625), SC_(6.18680867358564366225953846900945967356), 
      SC_(487.56890869140625), SC_(6.188405784992797235754687039867456958423), 
      SC_(488.608978271484375), SC_(6.190538871937823182207008881895013566252), 
      SC_(488.617706298828125), SC_(6.190556753080449523844892653864095258225), 
      SC_(489.25262451171875), SC_(6.191856655555710657680907555429146440994), 
      SC_(489.764404296875), SC_(6.192903221491570223625167765358468005398), 
      SC_(490.301483154296875), SC_(6.194000346274314039340661485248412605601), 
      SC_(490.58905029296875), SC_(6.194587283360205433159890246616353800577), 
      SC_(490.864105224609375), SC_(6.195148360331463990452548262484128395487), 
      SC_(492.441986083984375), SC_(6.198360967247667289203047166777662852555), 
      SC_(493.32684326171875), SC_(6.200158053193846223394413941905224342212), 
      SC_(498.36407470703125), SC_(6.210327265513424866339085669932575635988), 
      SC_(498.544189453125), SC_(6.21068897490776727949765832965196264653), 
      SC_(503.190032958984375), SC_(6.219973908373394206679161074530674466899), 
      SC_(503.6627197265625), SC_(6.220913780825655911052191262801777393669), 
      SC_(504.77099609375), SC_(6.223113978085038338735916934803931854624), 
      SC_(505.49853515625), SC_(6.224555691973970405577369173344297221254), 
      SC_(505.957061767578125), SC_(6.225463255838234486684327725118604039811), 
      SC_(507.8582763671875), SC_(6.229217575393520147953064296764206590646), 
      SC_(508.5086669921875), SC_(6.230498669894225271351080293124801058189), 
      SC_(510.771575927734375), SC_(6.234943245832893913399836097415067319716), 
      SC_(513.24951171875), SC_(6.239787603153936308761478021300341867958), 
      SC_(527.3714599609375), SC_(6.26695675989766014952180473516016639132), 
      SC_(528.53314208984375), SC_(6.269159200285662272626421793314655554156), 
      SC_(529.84197998046875), SC_(6.271634836682912113235700829440296732259), 
      SC_(530.79754638671875), SC_(6.273438405458562785658780556867743148032), 
      SC_(532.8255615234375), SC_(6.277255406962121270372044267186720492822), 
      SC_(533.93310546875), SC_(6.27933382124723736043226562438644919799), 
      SC_(538.3424072265625), SC_(6.28756573791909313051189664771803422567), 
      SC_(540.13800048828125), SC_(6.290898688375581006405495741840322979188), 
      SC_(543.80548095703125), SC_(6.297671882932801362742512328060055029049), 
      SC_(544.05609130859375), SC_(6.298133046141955500936690446904311697961), 
      SC_(546.5919189453125), SC_(6.302787451011865112672341728223109009577), 
      SC_(546.8814697265625), SC_(6.303317533868186538707062673105932672072), 
      SC_(547.00885009765625), SC_(6.303550641132128690074401949014653406049), 
      SC_(547.21551513671875), SC_(6.303928724516392966155084897475347327756), 
      SC_(547.2205810546875), SC_(6.303937990566003104329651374824144391912), 
      SC_(549.7236328125), SC_(6.308505842439691623988056771871693216424), 
      SC_(549.86016845703125), SC_(6.308754409016708944632117615324933227263), 
      SC_(550.1563720703125), SC_(6.309293442830642860089093034772365129842), 
      SC_(552.88116455078125), SC_(6.314238460027511407047381188737584243717), 
      SC_(558.268798828125), SC_(6.323944671432167039241381145581814873829), 
      SC_(561.5574951171875), SC_(6.329823519476373092212405522576108829899), 
      SC_(567.82159423828125), SC_(6.340926458002524075449031889956629217335), 
      SC_(568.8236083984375), SC_(6.342691118582913267534099618695745472789), 
      SC_(573.754638671875), SC_(6.351330140380622771107787271382996222648), 
      SC_(575.2086181640625), SC_(6.353863287488337643657368466542593119861), 
      SC_(578.52508544921875), SC_(6.359617392870022484646570771392481186969), 
      SC_(579.70458984375), SC_(6.361655889694939183701010004548872090829), 
      SC_(580.95672607421875), SC_(6.363815376071726439662811719409699982055), 
      SC_(584.38226318359375), SC_(6.369699480584486422871125906468857080696), 
      SC_(585.26409912109375), SC_(6.371208638389763868709447504290801733396), 
      SC_(585.2677001953125), SC_(6.37121479653500333470827397736457127071), 
      SC_(587.04473876953125), SC_(6.374249067232996439686972718899923431032), 
      SC_(592.8238525390625), SC_(6.384053652275533641719864391983551192749), 
      SC_(594.50360107421875), SC_(6.386885499848034458902157664992427248316), 
      SC_(594.89605712890625), SC_(6.387545977951704153497341261837707303324), 
      SC_(601.98193359375), SC_(6.399396614546605823173454188612392794784), 
      SC_(602.84307861328125), SC_(6.400827295849304940475815798766156086542), 
      SC_(604.23150634765625), SC_(6.403129687503785434500679408789199298648), 
      SC_(616.044677734375), SC_(6.422507640667089449069553404969421958134), 
      SC_(620.36004638671875), SC_(6.429493829366529691782302916589330838881), 
      SC_(622.05511474609375), SC_(6.432224695074060781574961177412352412717), 
      SC_(622.47509765625), SC_(6.432900163721955173795252325722094013871), 
      SC_(623.435546875), SC_(6.434443164750907605937303777412492801794), 
      SC_(625.61859130859375), SC_(6.437941484456789575138548752117584086992), 
      SC_(627.38433837890625), SC_(6.440762161347675760635876431243167810861), 
      SC_(632.3592529296875), SC_(6.448666772647988605535323096388911661556), 
      SC_(633.06365966796875), SC_(6.449780967364441602752385978976936185726), 
      SC_(639.76336669921875), SC_(6.460316625883878714928422495164000965323), 
      SC_(643.9609375), SC_(6.466861422641841163115547439790852566178), 
      SC_(644.318115234375), SC_(6.467416356975518058351605023347089876719), 
      SC_(644.550537109375), SC_(6.467777297247478327338444774262678613444), 
      SC_(646.31298828125), SC_(6.470510069949258359745036258720962577061), 
      SC_(647.7459716796875), SC_(6.472726494325499127620694571869476943941), 
      SC_(649.115478515625), SC_(6.474840157130650254368072192966429504885), 
      SC_(651.9686279296875), SC_(6.479229339980760566248271955481499493423), 
      SC_(654.0791015625), SC_(6.482463666223159470601395935144865056701), 
      SC_(655.0980224609375), SC_(6.484021438312744798558143642548832482531), 
      SC_(655.4779052734375), SC_(6.484601599785286175366253598788561328841), 
      SC_(655.74066162109375), SC_(6.485002687522549887824574950594472055316), 
      SC_(655.7799072265625), SC_(6.485062580665467941142729902484080793074), 
      SC_(660.11944580078125), SC_(6.491663167216662693403076432415948835692), 
      SC_(663.60552978515625), SC_(6.49693424239261660697884227280722447065), 
      SC_(669.28509521484375), SC_(6.505462868816168906848474745781783475634), 
      SC_(678.735107421875), SC_(6.519494084518996469650508956074345352049), 
      SC_(679.51971435546875), SC_(6.520650251903972495307924790017120133482), 
      SC_(679.70269775390625), SC_(6.52091969726851616942271814508002088275), 
      SC_(679.81976318359375), SC_(6.521092039518570890159009605013714327936), 
      SC_(680.28704833984375), SC_(6.521779675083894126919680539525971204723), 
      SC_(680.4066162109375), SC_(6.521955549772378968850975278275518538879), 
      SC_(681.35955810546875), SC_(6.523356145843551429282561007449588269899), 
      SC_(686.77545166015625), SC_(6.531279168634480064303406688971587947728), 
      SC_(689.21453857421875), SC_(6.53482696034163300895210941857399062802), 
      SC_(694.82861328125), SC_(6.542945441035880360749863408250769056176), 
      SC_(695.23284912109375), SC_(6.5435274682425404780673540153675890027), 
      SC_(699.07672119140625), SC_(6.549045095113352556887019319492674591721), 
      SC_(705.77423095703125), SC_(6.558586791026949965247721665028964915463), 
      SC_(706.04608154296875), SC_(6.558972170428375943894892162331319111229), 
      SC_(707.321533203125), SC_(6.560778289147646212053005392960932901218), 
      SC_(709.36480712890625), SC_(6.563664910211286354361274659607364432189), 
      SC_(710.703857421875), SC_(6.565552134560890098719864436298697541221), 
      SC_(711.2158203125), SC_(6.566272742257831657216451568845017111234), 
      SC_(716.1007080078125), SC_(6.573122422391305665202378372481901866655), 
      SC_(718.469970703125), SC_(6.576427825470989904051723420894245752996), 
      SC_(720.493408203125), SC_(6.579242137071712732628854802208909850768), 
      SC_(725.8389892578125), SC_(6.586639196163926236455690072217971768091), 
      SC_(726.6544189453125), SC_(6.587762769498345108593561389467357233635), 
      SC_(726.9547119140625), SC_(6.588176222669208952468481654231519862001), 
      SC_(730.33087158203125), SC_(6.592812902760243297414563417571806817033), 
      SC_(731.722412109375), SC_(6.594717748731769186160610648305398172825), 
      SC_(732.7987060546875), SC_(6.596188576803175388558793950872542497826), 
      SC_(740.64727783203125), SC_(6.606849266552187262338997827259823953612), 
      SC_(743.1324462890625), SC_(6.610201309160827227217063092842877635818), 
      SC_(744.69281005859375), SC_(6.612300229797847496307719944337046025631), 
      SC_(748.151611328125), SC_(6.616937183758078139241196510560664648452), 
      SC_(749.94097900390625), SC_(6.619327641466965773804177461936924399101), 
      SC_(751.26708984375), SC_(6.621095544221748738916949620438022847397), 
      SC_(753.7291259765625), SC_(6.624369539120572790247890002187576586953), 
      SC_(754.6866455078125), SC_(6.625639951090094344607777868204027432855), 
      SC_(755.78985595703125), SC_(6.62710166378741985559108127492656943644), 
      SC_(757.20025634765625), SC_(6.628967285239014652929108380360053789214), 
      SC_(757.50360107421875), SC_(6.629368083169212155191930540373997326396), 
      SC_(757.7401123046875), SC_(6.629680465113967501897080616727473070964), 
      SC_(761.731201171875), SC_(6.634937195939679832108830040398849824872), 
      SC_(762.42132568359375), SC_(6.635843374996103932953744864855947073907), 
      SC_(762.931396484375), SC_(6.636512604288287695667831495157907988255), 
      SC_(763.75), SC_(6.637585703139515450729086875916629124126), 
      SC_(765.516845703125), SC_(6.639897925305579070472267454056802592828), 
      SC_(773.9171142578125), SC_(6.650818577182925150685349765007717566496), 
      SC_(774.91046142578125), SC_(6.652102114421492829419722285286772404085), 
      SC_(775.712646484375), SC_(6.653137443611928596722616479632884363769), 
      SC_(778.897705078125), SC_(6.6572376513800787667154306478413125791), 
      SC_(779.167236328125), SC_(6.657583855585783056176609207961461550674), 
      SC_(779.68939208984375), SC_(6.658254207016113619993993137077534358157), 
      SC_(780.2274169921875), SC_(6.658944461745262492487573187543498710589), 
      SC_(780.25201416015625), SC_(6.658976007098292587450609334547284886854), 
      SC_(781.9091796875), SC_(6.661098998535236012443285119544067308945), 
      SC_(792.207275390625), SC_(6.674191788190495720635040843079615121198), 
      SC_(793.59759521484375), SC_(6.675946351193374567715402021389527667759), 
      SC_(793.9749755859375), SC_(6.676422068894767804835061682908706231587), 
      SC_(794.2845458984375), SC_(6.676812137779252911754812198193513578098), 
      SC_(794.8314208984375), SC_(6.677500846866967477229981288001168637074), 
      SC_(795.199951171875), SC_(6.677964689479356370863371111643685192501), 
      SC_(797.2799072265625), SC_(6.680578554749739014414223828144905732992), 
      SC_(797.3641357421875), SC_(6.680684260292981427494930116387169656838), 
      SC_(797.92864990234375), SC_(6.681392428974166964964677408576394246851), 
      SC_(798.1058349609375), SC_(6.68161459976456580623320912581051620254), 
      SC_(799.6531982421875), SC_(6.683552730103765717785258709879113031128), 
      SC_(800.0684814453125), SC_(6.684072249121468373962678765344998551064), 
      SC_(800.28045654296875), SC_(6.684337325823917944748077105114139749528), 
      SC_(801.01458740234375), SC_(6.685254820154850118553681554697581754228), 
      SC_(802.1114501953125), SC_(6.686624079247030582774578430359074551284), 
      SC_(807.531005859375), SC_(6.693362152645817588123872344533895119059), 
      SC_(808.17547607421875), SC_(6.694160403193372004065984871882451973199), 
      SC_(809.13720703125), SC_(6.695350434005938715450618220546308991728), 
      SC_(809.7344970703125), SC_(6.696088799092145379287817855954413316766), 
      SC_(811.58050537109375), SC_(6.698367379321691365123987787716196541633), 
      SC_(811.87396240234375), SC_(6.69872912383054362547878895705642827061), 
      SC_(814.2847900390625), SC_(6.701696008262299263121462513848022021511), 
      SC_(814.72369384765625), SC_(6.702235199265751419311368399785889082955), 
      SC_(815.76910400390625), SC_(6.70351831039065858731291819842905206617), 
      SC_(817.30322265625), SC_(6.705398274761274025517280475385018089932), 
      SC_(817.62774658203125), SC_(6.705795505605953960123274832847788002709), 
      SC_(820.84075927734375), SC_(6.709719875947035021518634958560843902315), 
      SC_(821.19403076171875), SC_(6.710150423121597468638525790922918705368), 
      SC_(821.2459716796875), SC_(6.710213710127498226829732029114238959828), 
      SC_(821.90325927734375), SC_(6.711014231224021145756877729370797694754), 
      SC_(822.60498046875), SC_(6.711868162017285030823308685434146199673), 
      SC_(823.45538330078125), SC_(6.712902048459101920134138581529056322254), 
      SC_(823.45782470703125), SC_(6.712905015086914031224699618961034238211), 
      SC_(825.8170166015625), SC_(6.715767636243291150268286133191516840221), 
      SC_(830.82861328125), SC_(6.721821602486133073771415419589804218135), 
      SC_(831.21429443359375), SC_(6.722285986772520722985120580144878454781), 
      SC_(835.008544921875), SC_(6.726843042451428339229067173099443198509), 
      SC_(840.71728515625), SC_(6.73366059021322264537674210235946387634), 
      SC_(844.30877685546875), SC_(6.737925959738321890628442129723129258846), 
      SC_(848.4677734375), SC_(6.742842689994820594916976128991288216001), 
      SC_(849.12933349609375), SC_(6.743622556956527511542416448168619758566), 
      SC_(853.0311279296875), SC_(6.748209779519757322824915399863143662548), 
      SC_(854.45098876953125), SC_(6.749873859079778299591453176485066806485), 
      SC_(855.8751220703125), SC_(6.751540168929954768274473011138988372597), 
      SC_(862.67816162109375), SC_(6.759461989375137122955523168205353583437), 
      SC_(868.6947021484375), SC_(6.766416056015530146135252193038369240874), 
      SC_(869.29217529296875), SC_(6.767103998002709424523574394478208979411), 
      SC_(870.186767578125), SC_(6.768133164641880669262131602717669262048), 
      SC_(872.4288330078125), SC_(6.770707861861071404604235546152482005058), 
      SC_(875.94281005859375), SC_(6.774729881353175605276607003284786011147), 
      SC_(876.7574462890625), SC_(6.775659990483147584810245626024403172256), 
      SC_(877.3638916015625), SC_(6.776351836800695164519969516994906905958), 
      SC_(878.4306640625), SC_(6.777567674259764690685426386600703554342), 
      SC_(885.99810791015625), SC_(6.786150373612182701581356639019071248638), 
      SC_(887.72613525390625), SC_(6.788099946665576471808465592376033272437), 
      SC_(890.90325927734375), SC_(6.791674512973996076620205962146827011757), 
      SC_(900.0538330078125), SC_(6.80189895079299198904108771904549258351), 
      SC_(900.1832275390625), SC_(6.802042783444049392389068654739673857143), 
      SC_(902.71612548828125), SC_(6.804854149520866258225931342229077189181), 
      SC_(904.88092041015625), SC_(6.807250694718158106286277547784604092869), 
      SC_(905.79193115234375), SC_(6.808257518383144815884886145813559834871), 
      SC_(907.36474609375), SC_(6.809993367381673942808480783672644328598), 
      SC_(908.43359375), SC_(6.811171291835528522342757827992129791562), 
      SC_(910.56494140625), SC_(6.81351601145607889196433654356525525846), 
      SC_(910.6475830078125), SC_(6.813606815790404023323189518843380307986), 
      SC_(912.5775146484375), SC_(6.815725030681610574873878555395906108156), 
      SC_(913.33734130859375), SC_(6.816557756247443703552647371667225468139), 
      SC_(913.3758544921875), SC_(6.816599945984404149476890515171707321892), 
      SC_(915.73553466796875), SC_(6.819181496829614596845401444040303420405), 
      SC_(917.117919921875), SC_(6.820690771903488486844573292669501047527), 
      SC_(917.19366455078125), SC_(6.82077340337986035180672708919674786967), 
      SC_(920.874755859375), SC_(6.824780979614736502636113976154470262398), 
      SC_(923.379638671875), SC_(6.827498872615433477170026117448771541295), 
      SC_(927.49285888671875), SC_(6.8319459105580415512096734861697582082), 
      SC_(928.85418701171875), SC_(6.83341337535443392277100858245459000213), 
      SC_(929.263671875), SC_(6.833854364953862271803203220770951435661), 
      SC_(929.385986328125), SC_(6.833986052262011870912515832740972478803), 
      SC_(933.99322509765625), SC_(6.838933753242727661252945811698284684698), 
      SC_(934.01068115234375), SC_(6.838952452778342207434589177668657889371), 
      SC_(939.0015869140625), SC_(6.844284594236099395567215793156428746645), 
      SC_(940.07403564453125), SC_(6.845426666187627323729754250792575000864), 
      SC_(942.05059814453125), SC_(6.847528135708265169237587238138487198853), 
      SC_(944.7872314453125), SC_(6.850430437191786907351651758111614319783), 
      SC_(948.925048828125), SC_(6.854802811834284555381489323758582235861), 
      SC_(950.2220458984375), SC_(6.856169404714020378408540666432044534508), 
      SC_(954.943359375), SC_(6.861128346520892642201796166599822038471), 
      SC_(955.017578125), SC_(6.861206104783820446126170121986330983269), 
      SC_(956.13458251953125), SC_(6.862375649747642715205235987065776445518), 
      SC_(957.1669921875), SC_(6.863455405877975518619709756325785766734), 
      SC_(957.5068359375), SC_(6.863810580012297609207700982417845904956), 
      SC_(959.2913818359375), SC_(6.865673559355261880433729035303716037083), 
      SC_(959.492431640625), SC_(6.865883228230126473126797544809231414745), 
      SC_(959.74395751953125), SC_(6.866145475218019847811591867037562659094), 
      SC_(961.8980712890625), SC_(6.868388594389408661650623078145486428762), 
      SC_(964.88848876953125), SC_(6.871494254878780851989128297020995382715), 
      SC_(964.96636962890625), SC_(6.871575008339416373031500751728687321642), 
      SC_(967.6949462890625), SC_(6.874400118733980051857714900416741852838), 
      SC_(968.640380859375), SC_(6.87537714276502884037846310503316214533), 
      SC_(968.8677978515625), SC_(6.875612016002868563135875018985921894413), 
      SC_(970.5927734375), SC_(6.877391753918391265533592997745089246348), 
      SC_(977.00201416015625), SC_(6.883976856642686182085022703036491098977), 
      SC_(979.74835205078125), SC_(6.886785333127100045440434410093146016624), 
      SC_(979.9256591796875), SC_(6.886966381237166628662813047575594521666), 
      SC_(981.10968017578125), SC_(6.888174544185669495233192817179284924608), 
      SC_(981.72314453125), SC_(6.888799943368115034020659880636949694688), 
      SC_(982.36053466796875), SC_(6.8894493197658087146525978621453836978), 
      SC_(987.4595947265625), SC_(6.894629143910089536380903205901047293626), 
      SC_(988.37939453125), SC_(6.895560662679555267312975090816879245672), 
      SC_(988.5216064453125), SC_(6.895704609063605825635576817887832481836), 
      SC_(990.1099853515625), SC_(6.897310953864133148550105914216515716675), 
      SC_(992.88128662109375), SC_(6.900107437258989936275310085429403733899), 
      SC_(993.53472900390625), SC_(6.900765679507473199060169339559463847836), 
      SC_(994.0684814453125), SC_(6.901303031311155785795352189437299613696), 
      SC_(996.13470458984375), SC_(6.903380469883631241011306751657568074638), 
      SC_(996.4613037109375), SC_(6.903708447139272864766001846716835845992), 
   };
#undef SC_

