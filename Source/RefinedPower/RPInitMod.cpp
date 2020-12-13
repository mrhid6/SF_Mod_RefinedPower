// ILikeBanas


#include "RPInitMod.h"

#include "Kismet/GameplayStatics.h"
#include "FGCharacterPlayer.h"
#include "FGResourceScanner.h"

void ARPInitMod::loadNodes()
{
    FString DefaultJsonString = FString("{\"WaterTurbineNodes\":{\"Fast\":[");
    DefaultJsonString.Append("[445062.5,-259631.25,-1735.4290771484375],");
    DefaultJsonString.Append("[-76064.7578125,-101216.125,-52.670166015625],");
    DefaultJsonString.Append("[421712.03125,-259238.25,-1748.0863037109375],");
    DefaultJsonString.Append("[422612.03125,-259238.25,-1748.0865478515625],");
    DefaultJsonString.Append("[423912.03125,-259238.25,-1748.087158203125],");
    DefaultJsonString.Append("[425112.03125,-259238.25,-1748.0863037109375],");
    DefaultJsonString.Append("[426112.03125,-259238.25,-1748.0869140625],");
    DefaultJsonString.Append("[427312.03125,-259238.25,-1748.08642578125],");
    DefaultJsonString.Append("[430312.03125,-259238.25,-1748.086669921875],");
    DefaultJsonString.Append("[442962.5,-259631.25,-1735.4293212890625],");
    DefaultJsonString.Append("[428912.03125,-259238.25,-1748.0869140625],");
    DefaultJsonString.Append("[431212.03125,-259238.25,-1748.086669921875],");
    DefaultJsonString.Append("[443762.5,-259631.25,-1735.4290771484375],");
    DefaultJsonString.Append("[432112.03125,-259238.25,-1748.086181640625],");
    DefaultJsonString.Append("[433112.03125,-259238.25,-1748.086669921875],");
    DefaultJsonString.Append("[438562.5,-259631.25,-1735.428955078125],");
    DefaultJsonString.Append("[439662.5,-259631.25,-1735.428955078125],");
    DefaultJsonString.Append("[440562.5,-259631.25,-1735.428955078125],");
    DefaultJsonString.Append("[441762.5,-259631.25,-1735.4290771484375],");
    DefaultJsonString.Append("[446262.5,-259631.25,-1735.4290771484375],");
    DefaultJsonString.Append("[447362.5,-259631.25,-1735.4290771484375],");
    DefaultJsonString.Append("[448362.5,-259631.25,-1735.4290771484375],");
    DefaultJsonString.Append("[449262.5,-259631.25,-1735.4290771484375],");
    DefaultJsonString.Append("[450162.5,-259631.25,-1735.4290771484375],");
    DefaultJsonString.Append("[451662.5,-259531.25,-1735.4290771484375],");
    DefaultJsonString.Append("[209881.3125,-191540.203125,23567.361328125],");
    DefaultJsonString.Append("[210914.96875,-191916.421875,23567.361328125],");
    DefaultJsonString.Append("[205666.0625,-24658.478515625,592.82275390625],");
    DefaultJsonString.Append("[206266.0625,-23619.248046875,592.822998046875],");
    DefaultJsonString.Append("[168168.125,232125.21875,-9389.12109375],");
    DefaultJsonString.Append("[206816.0625,-22666.62109375,592.82275390625],");
    DefaultJsonString.Append("[166625.421875,233963.71875,-9389.12109375],");
    DefaultJsonString.Append("[-101053.3828125,157804,6.993896484375],");
    DefaultJsonString.Append("[209544.5,-22740.787109375,592.8228759765625],");
    DefaultJsonString.Append("[208418.6875,-22090.787109375,592.822998046875],");
    DefaultJsonString.Append("[167026.0625,232863.953125,-9389.12109375],");
    DefaultJsonString.Append("[-100827.6328125,159084.25,6.9940185546875],");
    DefaultJsonString.Append("[209065.71875,-30193.6953125,1887.07861328125],");
    DefaultJsonString.Append("[205233.71875,-188615.859375,22919.912109375],");
    DefaultJsonString.Append("[204857.5,-187582.203125,22919.912109375],");
    DefaultJsonString.Append("[204447.09375,-186454.578125,22919.912109375],");
    DefaultJsonString.Append("[205891.828125,-189546.90625,22919.912109375],");
    DefaultJsonString.Append("[204224.609375,-185258.546875,22919.912109375],");
    DefaultJsonString.Append("[203780,-184036.953125,22919.912109375],");
    DefaultJsonString.Append("[203403.78125,-183003.296875,22919.912109375],");
    DefaultJsonString.Append("[174959.46875,66317.3046875,9510.0947265625],");
    DefaultJsonString.Append("[-105831.0078125,-28455.58984375,15415.4638671875],");
    DefaultJsonString.Append("[67838.3359375,24590.712890625,13156.177734375],");
    DefaultJsonString.Append("[68468.7890625,23683.783203125,13156.177734375],");
    DefaultJsonString.Append("[-92507.8984375,159548.59375,-4334.1259765625],");
    DefaultJsonString.Append("[73474.140625,28825.44921875,11687.2255859375],");
    DefaultJsonString.Append("[175931.921875,60802.390625,9510.0947265625],");
    DefaultJsonString.Append("[73896.8515625,27579.9296875,11687.2255859375],");
    DefaultJsonString.Append("[175202.578125,64938.5703125,9510.0947265625],");
    DefaultJsonString.Append("[175347.21875,63542.48046875,9510.0947265625],");
    DefaultJsonString.Append("[175636.71875,62476.55859375,9510.0947265625],");
    DefaultJsonString.Append("[176157.671875,59522.14453125,9510.0947265625],");
    DefaultJsonString.Append("[176400.78125,58143.41796875,9510.0947265625],");
    DefaultJsonString.Append("[176591.796875,57060.1328125,9510.0947265625],");
    DefaultJsonString.Append("[177977.671875,51235.765625,9496.599609375],");
    DefaultJsonString.Append("[176765.453125,56075.328125,9510.0947265625],");
    DefaultJsonString.Append("[177083.96875,55420.6953125,9510.0947265625],");
    DefaultJsonString.Append("[177541.6875,52141.25390625,9496.599609375],");
    DefaultJsonString.Append("[20197.89453125,-10153.0703125,22327.0859375],");
    DefaultJsonString.Append("[179203.46875,45287.49609375,9504.716796875],");
    DefaultJsonString.Append("[182462.125,29924.13671875,9417.66015625],");
    DefaultJsonString.Append("[182323.203125,30711.982421875,9417.66015625],");
    DefaultJsonString.Append("[182184.28125,31499.828125,9417.66015625],");
    DefaultJsonString.Append("[182045.359375,32287.673828125,9417.66015625],");
    DefaultJsonString.Append("[183232.390625,24906.201171875,9475.1572265625],");
    DefaultJsonString.Append("[182890.390625,25845.892578125,9475.1572265625],");
    DefaultJsonString.Append("[178921.21875,46355.359375,9504.716796875],");
    DefaultJsonString.Append("[178040.625,48482.44921875,9504.716796875],");
    DefaultJsonString.Append("[178552.96875,49413.8515625,9504.716796875],");
    DefaultJsonString.Append("[-117952.4453125,-23617.517578125,10293.380859375],");
    DefaultJsonString.Append("[-92264.78125,158169.875,-4334.1259765625],");
    DefaultJsonString.Append("[-118345.9453125,-21951.63671875,10293.380859375],");
    DefaultJsonString.Append("[-93054.09375,61761.31640625,11487.150390625],");
    DefaultJsonString.Append("[-116310.7421875,-28128.0390625,10293.380859375],");
    DefaultJsonString.Append("[-93054.09375,62461.31640625,11487.150390625],");
    DefaultJsonString.Append("[-46127.42578125,55641.51953125,23410.845703125],");
    DefaultJsonString.Append("[-120133.921875,31408.044921875,19754.16015625],");
    DefaultJsonString.Append("[-120407.5390625,32159.798828125,19754.16015625],");
    DefaultJsonString.Append("[-22466.7265625,48075.03125,20813.6796875],");
    DefaultJsonString.Append("[-22623.01171875,48961.359375,20813.6796875],");
    DefaultJsonString.Append("[-190105.515625,32242.6328125,15271.814453125],");
    DefaultJsonString.Append("[-157035.875,25839.212890625,18476.30859375],");
    DefaultJsonString.Append("[-165256.359375,24983.490234375,17734.798828125],");
    DefaultJsonString.Append("[-157192.15625,26725.5390625,18476.30859375],");
    DefaultJsonString.Append("[-165395.28125,25771.3359375,17734.798828125],");
    DefaultJsonString.Append("[-157331.078125,27513.384765625,18476.30859375],");
    DefaultJsonString.Append("[-159745.984375,17254.89453125,17643.71484375],");
    DefaultJsonString.Append("[-189797.703125,33088.35546875,15271.814453125],");
    DefaultJsonString.Append("[-189489.90625,33934.078125,15271.814453125],");
    DefaultJsonString.Append("[-189147.890625,34873.7734375,15271.814453125],");
    DefaultJsonString.Append("[-173502.984375,69936.8671875,6941.8837890625],");
    DefaultJsonString.Append("[-174976.171875,70757.0078125,-1168.7935791015625],");
    DefaultJsonString.Append("[-179894.671875,70549.90625,12710.486328125],");
    DefaultJsonString.Append("[-107883.0859375,-31238.767578125,15415.212890625],");
    DefaultJsonString.Append("[-107383.0859375,-30372.7421875,15415.212890625],");
    DefaultJsonString.Append("[-24305.2578125,-74396,10865.8837890625],");
    DefaultJsonString.Append("[-79964.7578125,-101416.125,-52.670166015625],");
    DefaultJsonString.Append("[-23668.654296875,-75298.625,10865.8837890625],");
    DefaultJsonString.Append("[-25555.2578125,-72230.9375,10865.8837890625],");
    DefaultJsonString.Append("[-26191.861328125,-71328.3125,10865.8837890625],");
    DefaultJsonString.Append("[295508.875,110445.3984375,-1638.550537109375],");
    DefaultJsonString.Append("[-109069.6875,-33094.0234375,15415.212890625],");
    DefaultJsonString.Append("[-108519.6875,-32141.39453125,15415.212890625],");
    DefaultJsonString.Append("[307084.09375,117575.109375,-1646.019775390625],");
    DefaultJsonString.Append("[306197.78125,117731.3984375,-1646.02001953125],");
    DefaultJsonString.Append("[-16698.58984375,-43753.43359375,16132.8505859375],");
    DefaultJsonString.Append("[-15289.0517578125,-43240.3984375,16132.8505859375],");
    DefaultJsonString.Append("[305114.46875,117922.4140625,-1646.02001953125],");
    DefaultJsonString.Append("[-15628.546875,-50928.328125,15652.9599609375],");
    DefaultJsonString.Append("[-12349.083984375,-41939.08984375,16036.5361328125],");
    DefaultJsonString.Append("[254983.515625,134912.03125,-3844.3095703125],");
    DefaultJsonString.Append("[8733.3916015625,-15788.9970703125,23361.166015625],");
    DefaultJsonString.Append("[-271447.75,-73559.3515625,-392.97235107421875],");
    DefaultJsonString.Append("[-118748.078125,-19969.646484375,10293.380859375],");
    DefaultJsonString.Append("[-268489.9375,-57879.65625,-406.484130859375],");
    DefaultJsonString.Append("[-132645.46875,-30383.044921875,2577.310546875],");
    DefaultJsonString.Append("[-153573.53125,-87507.671875,480.9532470703125],");
    DefaultJsonString.Append("[-133866.796875,-28927.556640625,2577.310546875],");
    DefaultJsonString.Append("[-133224,-29693.6015625,2577.310546875],");
    DefaultJsonString.Append("[-154782.515625,-85911.3046875,480.9532470703125],");
    DefaultJsonString.Append("[-152930.75,-88273.71875,480.9534912109375],");
    DefaultJsonString.Append("[-133395.953125,-41700.28515625,2520.112548828125],");
    DefaultJsonString.Append("[-117003.078125,-27103.015625,10293.380859375],");
    DefaultJsonString.Append("[-115353.078125,-30759.1796875,10293.380859375],");
    DefaultJsonString.Append("[-154011.171875,-86830.5546875,480.9532470703125],");
    DefaultJsonString.Append("[-115729.3046875,-29725.517578125,10293.380859375],");
    DefaultJsonString.Append("[-19561.79296875,-74240.53125,13140.7734375],");
    DefaultJsonString.Append("[53140.59375,-71207.3203125,9323.7939453125],");
    DefaultJsonString.Append("[-20397.4140625,-73244.671875,13140.7734375],");
    DefaultJsonString.Append("[-21309.640625,-72313.09375,13140.7734375],");
    DefaultJsonString.Append("[-22273.818359375,-71164.03125,13140.7734375],");
    DefaultJsonString.Append("[-23354.228515625,-69720.8671875,13140.7734375],");
    DefaultJsonString.Append("[294031.65625,110184.921875,-1638.5504150390625],");
    DefaultJsonString.Append("[-77464.7578125,-101216.125,-52.670166015625],");
    DefaultJsonString.Append("[292357.46875,109889.7109375,-1638.5506591796875],");
    DefaultJsonString.Append("[291077.25,109663.96875,-1638.55029296875],");
    DefaultJsonString.Append("[300831.3125,114676.96875,-1664.7718505859375],");
    DefaultJsonString.Append("[301039.65625,115858.734375,-1664.7723388671875],");
    DefaultJsonString.Append("[169493.328125,230234.734375,-9389.12109375],");
    DefaultJsonString.Append("[-145727.71875,228719.03125,-1669.558837890625],");
    DefaultJsonString.Append("[-142527.71875,228719.03125,-1669.558837890625],");
    DefaultJsonString.Append("[-147627.71875,228719.03125,-1669.55859375],");
    DefaultJsonString.Append("[-148827.71875,228719.03125,-1669.558349609375],");
    DefaultJsonString.Append("[-157413.953125,224795.4375,-1782.420166015625],");
    DefaultJsonString.Append("[-158813.953125,224895.4375,-1782.419921875],");
    DefaultJsonString.Append("[-160113.953125,224895.4375,-1782.420166015625],");
    DefaultJsonString.Append("[-143727.71875,228819.03125,-1669.55859375],");
    DefaultJsonString.Append("[-135527.71875,228819.03125,-1669.55859375],");
    DefaultJsonString.Append("[-137627.71875,228719.03125,-1669.558349609375],");
    DefaultJsonString.Append("[-139527.71875,228819.03125,-1669.55859375],");
    DefaultJsonString.Append("[-141327.71875,228819.03125,-1669.55859375],");
    DefaultJsonString.Append("[-133927.71875,228919.03125,-1669.5584716796875],");
    DefaultJsonString.Append("[-161613.953125,224895.4375,-1782.419921875],");
    DefaultJsonString.Append("[-172033.078125,224242.53125,-1772.6695556640625],");
    DefaultJsonString.Append("[-171048.265625,224068.890625,-1772.669677734375],");
    DefaultJsonString.Append("[-162513.953125,224895.4375,-1782.419677734375],");
    DefaultJsonString.Append("[-163813.953125,224895.4375,-1782.419921875],");
    DefaultJsonString.Append("[-165013.953125,224795.4375,-1782.419677734375],");
    DefaultJsonString.Append("[-169964.984375,223877.875,-1772.669921875],");
    DefaultJsonString.Append("[-168980.1875,223704.21875,-1772.669677734375],");
    DefaultJsonString.Append("[26914.693359375,38423.37109375,23388.404296875],");
    DefaultJsonString.Append("[25787.0625,38012.9453125,23388.404296875],");
    DefaultJsonString.Append("[-15238.8564453125,110953.4609375,20884.638671875],");
    DefaultJsonString.Append("[-14138.8564453125,110853.4609375,20884.638671875],");
    DefaultJsonString.Append("[-175737.125,243971.703125,-1770.31103515625],");
    DefaultJsonString.Append("[-175737.125,242771.703125,-1770.3109130859375],");
    DefaultJsonString.Append("[-175737.125,241071.703125,-1770.310791015625],");
    DefaultJsonString.Append("[-175737.125,239671.703125,-1770.310791015625],");
    DefaultJsonString.Append("[-175737.125,238571.703125,-1770.3109130859375],");
    DefaultJsonString.Append("[-175737.125,236971.703125,-1770.310791015625],");
    DefaultJsonString.Append("[-175737.125,235571.703125,-1770.3111572265625],");
    DefaultJsonString.Append("[-175648.3125,234184.859375,-1721.423828125],");
    DefaultJsonString.Append("[-175439.921875,233003.09375,-1721.4241943359375],");
    DefaultJsonString.Append("[-175231.546875,231821.328125,-1721.424072265625],");
    DefaultJsonString.Append("[-175579.3125,230338.421875,-1721.424072265625],");
    DefaultJsonString.Append("[-175358.828125,258928.734375,-1776.9925537109375],");
    DefaultJsonString.Append("[-175358.828125,257828.734375,-1776.9925537109375],");
    DefaultJsonString.Append("[-175458.828125,256528.734375,-1776.992431640625],");
    DefaultJsonString.Append("[-175558.828125,255328.734375,-1776.992431640625],");
    DefaultJsonString.Append("[-175458.828125,253328.734375,-1776.9923095703125],");
    DefaultJsonString.Append("[-175458.828125,251528.734375,-1776.9923095703125],");
    DefaultJsonString.Append("[-175558.828125,250028.734375,-1776.99267578125],");
    DefaultJsonString.Append("[-128358.0625,225126.765625,-1772.80419921875],");
    DefaultJsonString.Append("[-126961.96875,224982.140625,-1772.804931640625],");
    DefaultJsonString.Append("[-19678.61328125,112174.1328125,20909.396484375],");
    DefaultJsonString.Append("[-125600.609375,224640.546875,-1772.804931640625],");
    DefaultJsonString.Append("[-18438.857421875,112153.4609375,20884.638671875],");
    DefaultJsonString.Append("[-17338.857421875,112253.4609375,20884.638671875]");


    DefaultJsonString.Append("],\"Medium\":[");
    DefaultJsonString.Append("[228774.796875,-191433.8125,1063.96875],");
    DefaultJsonString.Append("[223225.0625,-181182.25,1070.556640625],");
    DefaultJsonString.Append("[219420.234375,-173414.28125,1062.0340576171875],");
    DefaultJsonString.Append("[233551.453125,-186962.109375,1065.302978515625],");
    DefaultJsonString.Append("[232772.046875,-187412.109375,1065.302978515625],");
    DefaultJsonString.Append("[230121.796875,-304263.96875,1069.492919921875],");
    DefaultJsonString.Append("[158140.1875,57517.9375,9486.1748046875],");
    DefaultJsonString.Append("[286408.34375,-302321.09375,1066.45166015625],");
    DefaultJsonString.Append("[285209.21875,-302430.96875,1066.4515380859375],");
    DefaultJsonString.Append("[159047.125,58148.40234375,9486.1748046875],");
    DefaultJsonString.Append("[196515.53125,63982.78125,-1663.096923828125],");
    DefaultJsonString.Append("[195815.53125,65195.21875,-1663.096923828125],");
    DefaultJsonString.Append("[195215.53125,66234.453125,-1663.096923828125],");
    DefaultJsonString.Append("[193712.078125,41809.4296875,-1622.6341552734375],");
    DefaultJsonString.Append("[194312.078125,42848.66015625,-1622.6336669921875],");
    DefaultJsonString.Append("[194862.078125,43801.2890625,-1622.6337890625],");
    DefaultJsonString.Append("[179427.078125,59358.671875,-1641.493896484375],");
    DefaultJsonString.Append("[179183.96875,60737.3984375,-1641.493896484375],");
    DefaultJsonString.Append("[178940.859375,62116.12890625,-1641.494140625],");
    DefaultJsonString.Append("[178663.015625,63691.81640625,-1641.4942626953125],");
    DefaultJsonString.Append("[178350.4375,65464.4765625,-1641.4942626953125],");
    DefaultJsonString.Append("[182757.171875,45690.578125,-1641.19091796875],");
    DefaultJsonString.Append("[182150.1875,47065.9140625,-1641.191162109375],");
    DefaultJsonString.Append("[181662.734375,48697.59375,-1641.1912841796875],");
    DefaultJsonString.Append("[181594,50056.00390625,-1641.1912841796875],");
    DefaultJsonString.Append("[180909.96875,51935.390625,-1641.1912841796875],");
    DefaultJsonString.Append("[180448.09375,53789.20703125,-1641.1910400390625],");
    DefaultJsonString.Append("[179935.0625,55198.74609375,-1641.19140625],");
    DefaultJsonString.Append("[226578.078125,78456.6015625,-1272.6588134765625],");
    DefaultJsonString.Append("[224940.890625,74794.734375,-1304.7513427734375],");
    DefaultJsonString.Append("[211560.109375,77701.9375,-1280.3154296875],");
    DefaultJsonString.Append("[216804.703125,83455.5390625,-1283.1949462890625],");
    DefaultJsonString.Append("[226503.40625,94131.3125,463.03173828125],");
    DefaultJsonString.Append("[256640.84375,105295.0703125,-1721.314697265625],");
    DefaultJsonString.Append("[228179.5,89106.0078125,442.19317626953125],");
    DefaultJsonString.Append("[237080.1875,61953.01171875,-1665.77587890625],");
    DefaultJsonString.Append("[157067.71875,56618.03515625,9486.1748046875],");
    DefaultJsonString.Append("[155918.640625,55653.85546875,9486.1748046875],");
    DefaultJsonString.Append("[129890.25,65427.96484375,9464.599609375],");
    DefaultJsonString.Append("[131111.859375,64983.3359375,9464.599609375],");
    DefaultJsonString.Append("[138184.546875,44480.859375,9482.56640625],");
    DefaultJsonString.Append("[138734.546875,45433.48828125,9482.5673828125],");
    DefaultJsonString.Append("[131792.703125,27601.4609375,9521.4892578125],");
    DefaultJsonString.Append("[133806.453125,42066.8515625,9504.48828125],");
    DefaultJsonString.Append("[129184.1171875,30710.19921875,9494.126953125],");
    DefaultJsonString.Append("[103235.078125,42650.0859375,9517.9833984375],");
    DefaultJsonString.Append("[103443.4609375,41468.3125,9517.9833984375],");
    DefaultJsonString.Append("[88748.078125,40858.28125,9498.42578125],");
    DefaultJsonString.Append("[88326.140625,41819.0546875,9498.42578125],");
    DefaultJsonString.Append("[87646.453125,42766.75,9498.4267578125],");
    DefaultJsonString.Append("[88868.6015625,44508.52734375,9498.4267578125],");
    DefaultJsonString.Append("[75256.3515625,29371.333984375,9535.013671875],");
    DefaultJsonString.Append("[76056.3515625,27985.693359375,9535.013671875],");
    DefaultJsonString.Append("[68629.375,25716.44921875,12280.287109375],");
    DefaultJsonString.Append("[69413.0390625,24938.078125,12280.287109375],");
    DefaultJsonString.Append("[71383.9296875,7001.5078125,13129.6689453125],");
    DefaultJsonString.Append("[72583.9296875,7001.50634765625,13129.6689453125],");
    DefaultJsonString.Append("[59017.20703125,-4135.64990234375,13151.37890625],");
    DefaultJsonString.Append("[73588.484375,-5286.8154296875,13132.0908203125],");
    DefaultJsonString.Append("[61172.78515625,-7684.18603515625,13112.2734375],");
    DefaultJsonString.Append("[61772.78515625,-8723.416015625,13112.2734375],");
    DefaultJsonString.Append("[74637.0390625,-4898.8388671875,13132.0908203125],");
    DefaultJsonString.Append("[11599.224609375,-269.5880126953125,23349.89453125],");
    DefaultJsonString.Append("[11599.22265625,-1569.587890625,23349.89453125],");
    DefaultJsonString.Append("[16383.9296875,1586.4129638671875,23349.80078125],");
    DefaultJsonString.Append("[17289.087890625,15616.4658203125,23344.357421875],");
    DefaultJsonString.Append("[24655.837890625,41124.94140625,21375.609375],");
    DefaultJsonString.Append("[25542.1640625,41281.2265625,21375.609375],");
    DefaultJsonString.Append("[26474.876953125,41750.31640625,21375.609375],");
    DefaultJsonString.Append("[59881.953125,-43482.9765625,13116.380859375],");
    DefaultJsonString.Append("[3414.55126953125,65699.21875,20881.71875],");
    DefaultJsonString.Append("[248904.34375,122760.7421875,-2561.78076171875],");
    DefaultJsonString.Append("[45371.62890625,-33867.140625,13137.5390625],");
    DefaultJsonString.Append("[-78432.671875,73864.234375,16466.341796875],");
    DefaultJsonString.Append("[28436.255859375,9700.5791015625,23345.458984375],");
    DefaultJsonString.Append("[-77930.765625,73110.515625,16466.341796875],");
    DefaultJsonString.Append("[-17709.837890625,54205.828125,20876.001953125],");
    DefaultJsonString.Append("[-16723.515625,51459.58203125,20874.55859375],");
    DefaultJsonString.Append("[2898.665771484375,50332.8046875,20867.451171875],");
    DefaultJsonString.Append("[-53068.21875,37226.90234375,19674.123046875],");
    DefaultJsonString.Append("[35070.13671875,238993.0625,-5119.591796875],");
    DefaultJsonString.Append("[249294.3125,119623.84375,-2048.57666015625],");
    DefaultJsonString.Append("[-35798.515625,31799.814453125,19693.4765625],");
    DefaultJsonString.Append("[-67336.9921875,35596.91796875,19698.861328125],");
    DefaultJsonString.Append("[-134651.9375,16422.595703125,19145.08203125],");
    DefaultJsonString.Append("[-86452.9140625,32837.78515625,19685.56640625],");
    DefaultJsonString.Append("[-98967.4453125,26756.57421875,19971.517578125],");
    DefaultJsonString.Append("[-113720.203125,41028.5,19690.921875],");
    DefaultJsonString.Append("[-138087.234375,28979.931640625,18500.5703125],");
    DefaultJsonString.Append("[-112126.609375,29260.078125,19780.3125],");
    DefaultJsonString.Append("[-137913.578125,27995.125,18500.5703125],");
    DefaultJsonString.Append("[-123960.7421875,12531.23046875,19357.095703125],");
    DefaultJsonString.Append("[-128746.75,25498.333984375,19080.71484375],");
    DefaultJsonString.Append("[-114471.9609375,41302.1171875,19690.921875],");
    DefaultJsonString.Append("[-96740.1328125,44254.43359375,19700.06640625],");
    DefaultJsonString.Append("[-167693.6875,25253.669921875,15546.4453125],");
    DefaultJsonString.Append("[-168001.5,26099.392578125,15546.4453125],");
    DefaultJsonString.Append("[-167317.46875,24220.0078125,15546.4453125],");
    DefaultJsonString.Append("[-203195.859375,40539.859375,-1744.676025390625],");
    DefaultJsonString.Append("[-174371.4375,84802.59375,-1045.595703125],");
    DefaultJsonString.Append("[-156218.828125,69010.9921875,-1122.2718505859375],");
    DefaultJsonString.Append("[-157481.71875,69335.2265625,-1122.2718505859375],");
    DefaultJsonString.Append("[-158941.5625,69694.1796875,-1122.2718505859375],");
    DefaultJsonString.Append("[-197145.015625,75516.28125,-1773.4315185546875],");
    DefaultJsonString.Append("[-146996.5,-67520.828125,513.85498046875],");
    DefaultJsonString.Append("[-193787.296875,32560.392578125,-1749.98681640625],");
    DefaultJsonString.Append("[-193308.484375,33875.9609375,-1749.987060546875],");
    DefaultJsonString.Append("[-192957.828125,35131.765625,-1749.98681640625],");
    DefaultJsonString.Append("[-192325.28125,36284.9609375,-1749.98681640625],");
    DefaultJsonString.Append("[-191974.640625,37540.76171875,-1749.987060546875],");
    DefaultJsonString.Append("[-196498.21875,38066.23046875,-1768.75244140625],");
    DefaultJsonString.Append("[-127376.3984375,-85340.1875,306.142578125],");
    DefaultJsonString.Append("[-133958.125,-67354.6171875,312.4940185546875],");
    DefaultJsonString.Append("[-126665.921875,-83934.6640625,315.361328125],");
    DefaultJsonString.Append("[-109734.3125,-87711.375,-42.60186767578125],");
    DefaultJsonString.Append("[-24226.83203125,-30282.93359375,17377.794921875],");
    DefaultJsonString.Append("[-127670.21875,-32483.06640625,2723.47705078125],");
    DefaultJsonString.Append("[-22556.322265625,-29288.01953125,17409.876953125],");
    DefaultJsonString.Append("[-5612.1103515625,-44695.6640625,13163.8671875],");
    DefaultJsonString.Append("[-6224.94677734375,-45209.89453125,13163.8671875],");
    DefaultJsonString.Append("[-6914.3876953125,-45788.40234375,13163.8671875],");
    DefaultJsonString.Append("[-126200.9765625,-40800.1875,2609.1708984375],");
    DefaultJsonString.Append("[-150347.59375,-49161.83203125,476.2027587890625],");
    DefaultJsonString.Append("[-141247.921875,-40047.13671875,482.81842041015625],");
    DefaultJsonString.Append("[-142750.421875,-36005.4609375,472.5338134765625],");
    DefaultJsonString.Append("[-142060.984375,-36583.97265625,472.53387451171875],");
    DefaultJsonString.Append("[-250938.53125,-81260.109375,-411.28564453125],");
    DefaultJsonString.Append("[-250092.8125,-80952.296875,-411.28564453125],");
    DefaultJsonString.Append("[-260471.21875,-97529.5625,-429.6790771484375],");
    DefaultJsonString.Append("[-224995.078125,-81271.6484375,-379.6890869140625],");
    DefaultJsonString.Append("[-225268.6875,-82023.40625,-379.68896484375],");
    DefaultJsonString.Append("[-199353.984375,-91879.6796875,-384.7099609375],");
    DefaultJsonString.Append("[-199080.375,-91127.921875,-384.709716796875],");
    DefaultJsonString.Append("[-172730.953125,-83828.6328125,-379.68243408203125],");
    DefaultJsonString.Append("[-78432.8515625,-106534.328125,-1808.7122802734375],");
    DefaultJsonString.Append("[-79332.8515625,-106534.328125,-1729.6680908203125],");
    DefaultJsonString.Append("[-77532.8515625,-106534.328125,-1761.9986572265625],");
    DefaultJsonString.Append("[-23884.31640625,-89106.9765625,2508.82861328125],");
    DefaultJsonString.Append("[44602.3046875,-101407.1796875,10452.7890625],");
    DefaultJsonString.Append("[53074.171875,-76494.359375,9316.400390625],");
    DefaultJsonString.Append("[54354.421875,-76268.6171875,9316.400390625],");
    DefaultJsonString.Append("[57198.00390625,-29998.046875,13119.0419921875],");
    DefaultJsonString.Append("[28704.71484375,-40766.4765625,13115.08984375],");
    DefaultJsonString.Append("[-10876.8349609375,81456.8515625,20875.060546875],");
    DefaultJsonString.Append("[36831.47265625,-100862.2421875,11882.3017578125],");
    DefaultJsonString.Append("[210778.8125,-26925.576171875,1394.5113525390625],");
    DefaultJsonString.Append("[207815.171875,-39539.51953125,2132.3740234375],");
    DefaultJsonString.Append("[-113595.78125,163695.078125,256.5909423828125],");
    DefaultJsonString.Append("[208236.328125,-27617.1171875,1377.313232421875],");
    DefaultJsonString.Append("[13485.943359375,51473.4375,21308.337890625],");
    DefaultJsonString.Append("[-27775.091796875,46245.2890625,19705.484375],");
    DefaultJsonString.Append("[14575.1728515625,51986.8359375,21308.337890625],");
    DefaultJsonString.Append("[-114085.9765625,162246.0625,256.5911865234375]");


    DefaultJsonString.Append("],\"Slow\":[");
    DefaultJsonString.Append("[239964.078125,-196948.390625,1065.614990234375],");
    DefaultJsonString.Append("[252104.21875,-219656.75,1069.1748046875],");
    DefaultJsonString.Append("[251573.5625,-239316.328125,1067.9512939453125],");
    DefaultJsonString.Append("[259408.1875,-272830.4375,1068.734130859375],");
    DefaultJsonString.Append("[251293.640625,-263458.03125,1067.9815673828125],");
    DefaultJsonString.Append("[242645.625,-271448.09375,1071.418212890625],");
    DefaultJsonString.Append("[227777.25,-285359.59375,1081.2333984375],");
    DefaultJsonString.Append("[283010.09375,-282449.28125,1065.581298828125],");
    DefaultJsonString.Append("[215329.390625,70476.7109375,-1648.330810546875],");
    DefaultJsonString.Append("[211406.0625,61285.25,-1656.489501953125],");
    DefaultJsonString.Append("[212909.5625,61832.484375,-1656.48974609375],");
    DefaultJsonString.Append("[222857.203125,74237.65625,-1296.0806884765625],");
    DefaultJsonString.Append("[215063.484375,88192.0546875,-808.3131103515625],");
    DefaultJsonString.Append("[215945.265625,87805.171875,-820.2295532226562],");
    DefaultJsonString.Append("[219937.3125,90121.3984375,115.41778564453125],");
    DefaultJsonString.Append("[219437.3125,90987.421875,115.4176025390625],");
    DefaultJsonString.Append("[222484.765625,92562.7421875,260.414306640625],");
    DefaultJsonString.Append("[226221.109375,84069.578125,-1284.615478515625],");
    DefaultJsonString.Append("[223200.265625,64956.796875,-1666.794921875],");
    DefaultJsonString.Append("[248812.671875,56075.64453125,-1645.509033203125],");
    DefaultJsonString.Append("[248812.671875,55175.64453125,-1645.5089111328125],");
    DefaultJsonString.Append("[249391.140625,66232.296875,-1669.7989501953125],");
    DefaultJsonString.Append("[258344.96875,81527.0078125,-1662.78369140625],");
    DefaultJsonString.Append("[291664.28125,52589.2421875,-1668.7724609375],");
    DefaultJsonString.Append("[291664.28125,54089.2421875,-1668.7724609375],");
    DefaultJsonString.Append("[269846.625,93703.0703125,-1649.9305419921875],");
    DefaultJsonString.Append("[283068.03125,59275.69921875,-1635.17041015625],");
    DefaultJsonString.Append("[272744.59375,36054.8203125,-1655.921875],");
    DefaultJsonString.Append("[276804.21875,27752.501953125,-1665.650390625],");
    DefaultJsonString.Append("[247339.171875,15417.265625,-1665.08642578125],");
    DefaultJsonString.Append("[246739.171875,16456.49609375,-1665.08642578125],");
    DefaultJsonString.Append("[246002.5625,17532.326171875,-1665.086181640625],");
    DefaultJsonString.Append("[236821.109375,5517.48388671875,-1665.319091796875],");
    DefaultJsonString.Append("[222278.484375,47374.78515625,-1683.56640625],");
    DefaultJsonString.Append("[221996.578125,11292.0439453125,-1673.9385986328125],");
    DefaultJsonString.Append("[223863.140625,23719.740234375,-1675.045166015625],");
    DefaultJsonString.Append("[222591.34375,46478.109375,-1690.65234375],");
    DefaultJsonString.Append("[210829.859375,35675.65234375,-1659.14599609375],");
    DefaultJsonString.Append("[-93565.5703125,30989.513671875,19821.861328125],");
    DefaultJsonString.Append("[-48078.59765625,90799.8203125,21118.861328125],");
    DefaultJsonString.Append("[-64175.984375,76148.7734375,19095.984375],");
    DefaultJsonString.Append("[-64690.21484375,76761.609375,19095.984375],");
    DefaultJsonString.Append("[-75383.8359375,88558.1015625,20140.77734375],");
    DefaultJsonString.Append("[-66712.9609375,77710,19258.28515625],");
    DefaultJsonString.Append("[-78779.765625,67821.28125,15884.5966796875],");
    DefaultJsonString.Append("[-79098.6328125,58696.19140625,19065.814453125],");
    DefaultJsonString.Append("[-83488.8046875,13330.12890625,21267.33203125],");
    DefaultJsonString.Append("[-72466.6328125,35381.953125,20022.1875],");
    DefaultJsonString.Append("[-108124.2890625,9567.9091796875,21908.740234375],");
    DefaultJsonString.Append("[-107884.875,10225.693359375,21908.740234375],");
    DefaultJsonString.Append("[-89561.96875,21671.90625,20902.2421875],");
    DefaultJsonString.Append("[-116652.171875,15126.3681640625,20090.33203125],");
    DefaultJsonString.Append("[-110578.25,24303.359375,19856.310546875],");
    DefaultJsonString.Append("[-227188.75,-7919.00927734375,-1750.8143310546875],");
    DefaultJsonString.Append("[-253613.015625,-10161.2138671875,-1767.2431640625],");
    DefaultJsonString.Append("[-215942.59375,-30381.96484375,-1739.557373046875],");
    DefaultJsonString.Append("[-195245.953125,-39972.01171875,-1733.4246826171875],");
    DefaultJsonString.Append("[-194603.15625,-39205.96484375,-1733.42431640625],");
    DefaultJsonString.Append("[-154199.34375,-49624.33984375,448.239501953125],");
    DefaultJsonString.Append("[-58173.88671875,-96891.4609375,-50.946075439453125],");
    DefaultJsonString.Append("[-152884.71875,-61814.421875,554.506103515625],");
    DefaultJsonString.Append("[-39023.4921875,-105051.65625,1966.766845703125],");
    DefaultJsonString.Append("[-32846.578125,-92859.328125,59.5941162109375],");
    DefaultJsonString.Append("[-224139.9375,-93631.6328125,-379.68499755859375],");
    DefaultJsonString.Append("[-38054.45703125,54979.1796875,21021.66796875],");
    DefaultJsonString.Append("[74617.015625,-39592.99609375,5818.125],");
    DefaultJsonString.Append("[74309.1953125,-38747.2734375,5818.125]");
    DefaultJsonString.Append("]}}");


    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(DefaultJsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        SML::WriteModConfig("RefinedPower", JsonObject.ToSharedRef());
        TSharedRef<FJsonObject> ModJson = SML::ReadModConfig("RefinedPower", JsonObject.ToSharedRef());

        if (JsonObject->HasField("WaterTurbineNodes"))
        {
            TSharedPtr<FJsonObject> Nodes = ModJson->GetObjectField("WaterTurbineNodes");

            TArray<TSharedPtr<FJsonValue>> FastRows = Nodes->GetArrayField("Fast");
            TArray<TSharedPtr<FJsonValue>> MedRows = Nodes->GetArrayField("Medium");
            TArray<TSharedPtr<FJsonValue>> SlowRows = Nodes->GetArrayField("Slow");

            //SML::Logging::info("[RefinedPower] - Loading WaterTurbineNodes [FAST] ..");
            loadWTNToArray(FastRows, WT_FastLocations);

            //SML::Logging::info("[RefinedPower] - Loading WaterTurbineNodes [MED] ..");
            loadWTNToArray(MedRows, WT_MediumLocations);

            //SML::Logging::info("[RefinedPower] - Loading WaterTurbineNodes [SLOW] ..");
            loadWTNToArray(SlowRows, WT_SlowLocations);
        }
    }
}

void ARPInitMod::loadWTNToArray(TArray<TSharedPtr<FJsonValue>> Rows, TArray<FVector>& arr)
{
    arr.Empty();

    for (int RowNum = 0; RowNum != Rows.Num(); RowNum++)
    {
        TArray<TSharedPtr<FJsonValue>> tempRow = Rows[RowNum]->AsArray();
        if (tempRow.Num() == 3)
        {
            float x = tempRow[0]->AsNumber();
            float y = tempRow[1]->AsNumber();
            float z = tempRow[2]->AsNumber();

            FVector Location = FVector(x, y, z);
            //SML::Logging::info(TCHAR_TO_UTF8(*Location.ToString()));

            arr.Add(Location);
        }
    }
}

void ARPInitMod::FinishedSpawningNodes()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);

    if(PC)
    {
        AFGCharacterPlayer* Player = Cast<AFGCharacterPlayer>(PC->GetPawn());
        if(Player)
        {
            AFGResourceScanner* scanner = Player->GetResourceScanner();

            scanner->GenerateNodeClustersAccessor();
        
            //SML::Logging::info("[RefinedPower] - Did IT!");
        }else
        {
            //SML::Logging::info("[RefinedPower] - Cant get Player!");
        }
        
    }else
    {
        //SML::Logging::info("[RefinedPower] - Cant get PC!");
    }
}

FString ARPInitMod::DumpPPSettings(FPostProcessSettings PPSettings)
{
    UScriptStruct* Struct = PPSettings.StaticStruct();
    FString Output = TEXT("");
    Struct->ExportText(Output, &PPSettings, nullptr, this, (PPF_ExportsNotFullyQualified | PPF_Copy | PPF_Delimited | PPF_IncludeTransient || PPF_ParsingDefaultProperties), nullptr);

    
    return Output;
}

void ARPInitMod::DumpUObject(UObject* object)
{
    UClass *cl = object->GetClass();
    
    	if (cl)
    	{

    		for (TFieldIterator<UBoolProperty> Property(cl); Property; ++Property)
    		{
    			// "Converting" bool to string
    			FName const PropertyName = Property->GetFName();
    			FString value = Property->GetPropertyValue_InContainer(object) ? "true" : "false";
    			SML::Logging::info(TCHAR_TO_UTF8(*PropertyName.ToString()));
    			SML::Logging::info(TCHAR_TO_UTF8(*value));
    		}

    		for (TFieldIterator<UFloatProperty> Property(cl); Property; ++Property)
    		{
    			// "Converting" bool to string
    			FName const PropertyName = Property->GetFName();
    			float value = Property->GetPropertyValue_InContainer(object);
    			
    			SML::Logging::info(TCHAR_TO_UTF8(*PropertyName.ToString()));
    			SML::Logging::info(value);
    		}

    		for (TFieldIterator<UUInt32Property> Property(cl); Property; ++Property)
    		{
    			// "Converting" bool to string
    			FName const PropertyName = Property->GetFName();
    			uint32 value = Property->GetPropertyValue_InContainer(object);
    			
    			SML::Logging::info(TCHAR_TO_UTF8(*PropertyName.ToString()));
    			SML::Logging::info(value);
    		}

    		for (TFieldIterator<UIntProperty> Property(cl); Property; ++Property)
    		{
    			// "Converting" bool to string
    			FName const PropertyName = Property->GetFName();
    			int value = Property->GetPropertyValue_InContainer(object);
    			
    			SML::Logging::info(TCHAR_TO_UTF8(*PropertyName.ToString()));
    			SML::Logging::info(value);
    		}
    		
    	} 
}