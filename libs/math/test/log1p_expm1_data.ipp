#define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
   static const boost::array<boost::array<T, 3>, 80> log1p_expm1_data = { {
      { SC_(-0.69330310821533203125e0), SC_(-0.1181895342296499380302723361817935835636e1), SC_(-0.5000779577496508480606742934033661111325e0) }, 
      { SC_(-0.650003612041473388671875e0), SC_(-0.1049832444670425873798449427248829256278e1), SC_(-0.477956108886575099597621504254337139212e0) }, 
      { SC_(-0.5634434223175048828125e0), SC_(-0.8288372954181591063099417140530721209296e0), SC_(-0.4307544676154126107123951950891833745657e0) }, 
      { SC_(-0.546193540096282958984375e0), SC_(-0.7900844716039173375586798387434829539395e0), SC_(-0.4208498682390169422105137923243733478616e0) }, 
      { SC_(-0.542549669742584228515625e0), SC_(-0.7820869679038185958797390511513816016257e0), SC_(-0.4187356706519199578716316143188015118402e0) }, 
      { SC_(-0.5222184658050537109375e0), SC_(-0.7386016923991178813343233368825674656574e0), SC_(-0.4067969136237976293436891521638796489665e0) }, 
      { SC_(-0.510332167148590087890625e0), SC_(-0.7140280098902130849529032452448458253718e0), SC_(-0.3997038529677125427667940816216463479375e0) }, 
      { SC_(-0.50135910511016845703125e0), SC_(-0.6958690918220109127943821843553816517932e0), SC_(-0.3942931192785328340465460021958309251731e0) }, 
      { SC_(-0.479341685771942138671875e0), SC_(-0.6526612791947966365578043302467499708984e0), SC_(-0.3808091201700817035685503987715593769588e0) }, 
      { SC_(-0.4362652301788330078125e0), SC_(-0.5731714043684679946812364099230074705554e0), SC_(-0.3535537538578334381043477595638178633635e0) }, 
      { SC_(-0.4033059179782867431640625e0), SC_(-0.5163507223485766856253687547437478575359e0), SC_(-0.3318923180936536087108170796894639170118e0) }, 
      { SC_(-0.3905523121356964111328125e0), SC_(-0.4952021616945213559322173628246423264147e0), SC_(-0.3233169689815286035820337522007644642142e0) }, 
      { SC_(-0.3101024627685546875e0), SC_(-0.3712121891835896680955152331639073308925e0), SC_(-0.2666281909314749260551669118328805822824e0) }, 
      { SC_(-0.2841587960720062255859375e0), SC_(-0.3342969188422780419049667231102322921458e0), SC_(-0.247352882189668478949423010818860745509e0) }, 
      { SC_(-0.268566131591796875e0), SC_(-0.3127484680657795755742977783527708289938e0), SC_(-0.2355251348015818742687440901815837983695e0) }, 
      { SC_(-0.19418840110301971435546875e0), SC_(-0.215905312065666655084617543044814851896e0), SC_(-0.1764972591721576227824907304661137322776e0) }, 
      { SC_(-0.14176605641841888427734375e0), SC_(-0.1528785551425763265780363302506293842933e0), SC_(-0.1321757453457543023426366655287893957346e0) }, 
      { SC_(-0.109534211456775665283203125e0), SC_(-0.1160105952462048156067755365883457898243e0), SC_(-0.1037484982319069072752933517880363955799e0) }, 
      { SC_(-0.2047410048544406890869140625e-1), SC_(-0.2068660038044094868521052319477265955827e-1), SC_(-0.2026592921724753704129022027337835687888e-1) }, 
      { SC_(0.1690093176520690576580818742513656616211e-8), SC_(0.1690093175092483105529122131518271037775e-8), SC_(0.16900931779488980500463190560092746436e-8) }, 
      { SC_(0.2114990849122477811761200428009033203125e-8), SC_(0.2114990846885884668978873262661703032735e-8), SC_(0.2114990851359070959273901626052243209537e-8) }, 
      { SC_(0.7099628440698779741069301962852478027344e-8), SC_(0.7099628415496417862364745346932718974223e-8), SC_(0.7099628465901141798701264063185361883662e-8) }, 
      { SC_(0.136718796284185373224318027496337890625e-7), SC_(0.1367187953495839188729947299064627330362e-7), SC_(0.1367187972187868403533999531964021065056e-7) }, 
      { SC_(0.1679341288252089725574478507041931152344e-7), SC_(0.1679341274151154071302093036120830402912e-7), SC_(0.1679341302353025616649699444201095417115e-7) }, 
      { SC_(0.586768322818898013792932033538818359375e-7), SC_(0.5867683056040454540164807679535005383825e-7), SC_(0.5867683400337515836824145241465956859382e-7) }, 
      { SC_(0.1140460881288163363933563232421875e-6), SC_(0.1140460816255617220976462903776019835232e-6), SC_(0.1140460946320716923598363683695123099251e-6) }, 
      { SC_(0.1455586016163579188287258148193359375e-6), SC_(0.1455585910227056945720521239406972268757e-6), SC_(0.1455586122100116850826593912609916845078e-6) }, 
      { SC_(0.38918477685001562349498271942138671875e-6), SC_(0.3891847011176400068545868550436683472421e-6), SC_(0.3891848525824207140259509650302544482884e-6) }, 
      { SC_(0.623782625552848912775516510009765625e-6), SC_(0.6237824310005478475318960793412787601785e-6), SC_(0.623782820105271336383228077398355607783e-6) }, 
      { SC_(0.104669607026153244078159332275390625e-5), SC_(0.1046695522475582933881286527214809459507e-5), SC_(0.1046696618048055313232628353357778906342e-5) }, 
      { SC_(0.2951089072666945867240428924560546875e-5), SC_(0.2951084718212155380639451770777340673208e-5), SC_(0.2951093427134586747271699561094046027716e-5) }, 
      { SC_(0.4877083483734168112277984619140625e-5), SC_(0.4877071590801182991613986686611799404767e-5), SC_(0.487709537672515613069814424540824650525e-5) }, 
      { SC_(0.9066634447663091123104095458984375e-5), SC_(0.9066593345981423108474522054350755440144e-5), SC_(0.9066675549717413905283549214899456610946e-5) }, 
      { SC_(0.2360353755648247897624969482421875e-4), SC_(0.2360325899737320048013584393963240356542e-4), SC_(0.236038161221667766684232806678163246188e-4) }, 
      { SC_(0.60817910707555711269378662109375e-4), SC_(0.6081606137340567462263175491115417378507e-4), SC_(0.6081976015418009724388783415958425809014e-4) }, 
      { SC_(0.119476739200763404369354248046875e-3), SC_(0.1194696024236052968763590872811035649465e-3), SC_(0.1194838768306258449523761123420162837989e-3) }, 
      { SC_(0.2437086659483611583709716796875e-3), SC_(0.2436789738154874267053728955508067082563e-3), SC_(0.2437383653179059006664060856490910639369e-3) }, 
      { SC_(0.47970912419259548187255859375e-3), SC_(0.4795941005544676642733520039561612799338e-3), SC_(0.4798242030152303995117671825532525519771e-3) }, 
      { SC_(0.960788573138415813446044921875e-3), SC_(0.9603273112237537137213831663159454349065e-3), SC_(0.9612502783347382477096533911074491600195e-3) }, 
      { SC_(0.113048148341476917266845703125e-2), SC_(0.112984297039538753162021050860817616211e-2), SC_(0.1131120718465376073328378699867756887911e-2) }, 
      { SC_(0.33707791008055210113525390625e-2), SC_(0.3365110759179022039875145678841719780479e-2), SC_(0.3376466565278741394700008076507908493073e-2) }, 
      { SC_(0.512778759002685546875e-2), SC_(0.5114685258802700303545874906083848033916e-2), SC_(0.5140957193498527263160102812012697907234e-2) }, 
      { SC_(0.7697627879679203033447265625e-2), SC_(0.7668152306886386648889951166330157951323e-2), SC_(0.7727330782215801339522867700900079915854e-2) }, 
      { SC_(0.154774188995361328125e-1), SC_(0.1535886535535876489145011045382406354476e-1), SC_(0.1559781448309931312470733745915557934013e-1) }, 
      { SC_(0.305807329714298248291015625e-1), SC_(0.3012246177452263928099228685195389251529e-1), SC_(0.3105312667138388983748287762435717112768e-1) }, 
      { SC_(0.346831791102886199951171875e-1), SC_(0.3409527271716101078966670703672108048896e-1), SC_(0.3529165481200088712490838501531684667149e-1) }, 
      { SC_(0.65634094178676605224609375e-1), SC_(0.6357001557994644965537090025497140391848e-1), SC_(0.6783591829384049159063415288088650400045e-1) }, 
      { SC_(0.6610882282257080078125e-1), SC_(0.6401540573272318085520396443445644947172e-1), SC_(0.6834297093791793596420077701374399081879e-1) }, 
      { SC_(0.9283597767353057861328125e-1), SC_(0.8877613176091369848730538080276748892544e-1), SC_(0.9728174180292914206798492307510621372797e-1) }, 
      { SC_(0.1853029727935791015625e0), SC_(0.1699984151517873986675151014366108622057e0), SC_(0.2035830378085867638230987836336350617284e0) }, 
      { SC_(0.195668697357177734375e0), SC_(0.1787056082557073958655865192859050644124e0), SC_(0.2161239335120153310995245962871550390859e0) }, 
      { SC_(0.218036949634552001953125e0), SC_(0.1972405051449346869157040783806052110702e0), SC_(0.2436330185556740271801313404522599276529e0) }, 
      { SC_(0.22476322948932647705078125e0), SC_(0.2027475432657783430097849833923015479242e0), SC_(0.2520262382028310304455135247279894706452e0) }, 
      { SC_(0.250229179859161376953125e0), SC_(0.2233268783961024315029665215194302402664e0), SC_(0.2843197231751691135236590716143900016849e0) }, 
      { SC_(0.253903329372406005859375e0), SC_(0.2262613494244882135168356599690803021814e0), SC_(0.2890471852440059179173797372107595021134e0) }, 
      { SC_(0.3161745369434356689453125e0), SC_(0.2747294509656754848973082105230889771257e0), SC_(0.3718696766716845552650417052219688730639e0) }, 
      { SC_(0.409090220928192138671875e0), SC_(0.3429442627531700751753321547062422000484e0), SC_(0.5054475372328648428694815984023805796901e0) }, 
      { SC_(0.41710007190704345703125e0), SC_(0.3486125805910782371810230966107318794672e0), SC_(0.5175543698964877990407235081110698982288e0) }, 
      { SC_(0.4173481762409210205078125e0), SC_(0.3487876441750920349995178382655357863143e0), SC_(0.5179309284235235985742545616245146385983e0) }, 
      { SC_(0.42039263248443603515625e0), SC_(0.3509333351432084595068912946293452186787e0), SC_(0.522559244493788135782100429719474184743e0) }, 
      { SC_(0.4406131207942962646484375e0), SC_(0.3650688012994099484177768785497336743499e0), SC_(0.5536595074987176176278695373720601072416e0) }, 
      { SC_(0.4500701129436492919921875e0), SC_(0.3716119090177188338876380950054248074284e0), SC_(0.5684221483289186932443765450666370231476e0) }, 
      { SC_(0.4690119922161102294921875e0), SC_(0.3845900606819792767555511238898325590012e0), SC_(0.5984141671678306664208145374987554621041e0) }, 
      { SC_(0.488781034946441650390625e0), SC_(0.3979576877817231446668587804772904526162e0), SC_(0.6303276957438828775174515106733850816524e0) }, 
      { SC_(0.52980291843414306640625e0), SC_(0.4251389156264805389990504406429844143619e0), SC_(0.6985975133709526468615664928080778563331e0) }, 
      { SC_(0.56810867786407470703125e0), SC_(0.4498702293886911216071551868503271338352e0), SC_(0.7649258494577443195036874224587751059599e0) }, 
      { SC_(0.57872617244720458984375e0), SC_(0.4566183018986474768197353974513078612477e0), SC_(0.7837647742172442075416378527651943088102e0) }, 
      { SC_(0.582029759883880615234375e0), SC_(0.4587086807259736626531803258754840111707e0), SC_(0.7896673415707786528734865994546559029663e0) }, 
      { SC_(0.607590496540069580078125e0), SC_(0.474736471992995596813856979079879927429e0), SC_(0.836002211172822800500580737750918478977e0) }, 
      { SC_(0.640033721923828125e0), SC_(0.4947168037733846594212322562172572984505e0), SC_(0.8965448333670257147748892521303229061693e0) }, 
      { SC_(0.640509545803070068359375e0), SC_(0.4950068922396821400142385044888446891867e0), SC_(0.8974474694176578931586020775970152155848e0) }, 
      { SC_(0.643289387226104736328125e0), SC_(0.4966999569758336894986723845165627033027e0), SC_(0.9027294105692233274199303070362746252986e0) }, 
      { SC_(0.64851474761962890625e0), SC_(0.4998747295737266888041967257469236722589e0), SC_(0.9126978792095101927158614842576974964632e0) }, 
      { SC_(0.650843918323516845703125e0), SC_(0.5012866228091318430397939496252397876475e0), SC_(0.9171580713331758702589264747154474769605e0) }, 
      { SC_(0.65477287769317626953125e0), SC_(0.5036637653893261974582119443232408393674e0), SC_(0.9247053242168904876365979871823094489023e0) }, 
      { SC_(0.65641486644744873046875e0), SC_(0.504655547804425337585747233639878779038e0), SC_(0.927868264760303541518755094801625468889e0) }, 
      { SC_(0.6588299274444580078125e0), SC_(0.5061124908504770714998094003973873746865e0), SC_(0.932529810907327764547265337859703076731e0) }, 
      { SC_(0.673553526401519775390625e0), SC_(0.5149492258613715166147831918156453824087e0), SC_(0.9611941077924732903931571711562687857294e0) }, 
      { SC_(0.69003379344940185546875e0), SC_(0.5247485248589687068613254590642295922543e0), SC_(0.9937829089064982966773652105234409046975e0) }, 
      { SC_(0.69504582881927490234375e0), SC_(0.5277097781183924897416903820415806504912e0), SC_(0.1003800903666412193968978978979482061619e1) }
   } };
#undef SC_

