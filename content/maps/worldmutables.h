#pragma once

#include <cstdint>

const uint16_t mutable_indices[] = { 1109, 1110, 1327, 1341, 1342, 1343, 1543, 1553, 1554, 1555, 1556, 1683, 1759, 1769, 1770, 1771, 1772, 1879, 1886, 1899, 1975, 1985, 1986, 1987, 1988, 2110, 2115, 2191, 2214, 2215, 2216, 2331, 2339, 2430, 2431, 2432, 2547, 2562, 2625, 2626, 2627, 2646, 2647, 2648, 2753, 2761, 2762, 2763, 2841, 2842, 2843, 2977, 3000, 3057, 3058, 3059, 3193, 3198, 3259, 3283, 3403, 3409, 3425, 3475, 3499, 3625, 3691, 3715, 3841, 3849, 4057, 4082, 4120, 4121, 4172, 4173, 4174, 4210, 4211, 4212, 4388, 4389, 4390, 4604, 4605, 4606, 4894, 5325, 5326, 5327, 5541, 5542, 5543, 5757, 5758, 5759, 5794, 6010, 6190, 6226, 6245, 6246, 6442, 6461, 6462, 6658, 6677, 6678, 6690, 6712, 6713, 6893, 6894, 8528, 8529, 8530, 8744, 8745, 8746, 8879, 8960, 8961, 8962, 9176, 9177, 9178, 9360, 9392, 9393, 9394, 9608, 9609, 9610, 9758, 9824, 9825, 9826, 10040, 10041, 10042, 11817, 11818, 11819, 11820, 11821, 11964, 11965, 11966, 11967, 11968, 12033, 12034, 12035, 12036, 12037, 12180, 12181, 12182, 12183, 12184, 12249, 12250, 12251, 12252, 12253, 12798, 12799, 12800, 12801, 12802, 12969, 13008, 13065, 13076, 13287, 13493, 13686, 13687, 13688, 13701, 13702, 13703, 13735, 13902, 13903, 13904, 13917, 13918, 13919, 14118, 14119, 14120, 14133, 14134, 14135, 14962, 15475, 15594, 16079, 16080, 16081, 16082, 16083, 16237, 16248, 16295, 16296, 16297, 16298, 16299, 16446, 16511, 16512, 16513, 16514, 16515, 16655, 16727, 16728, 16729, 16730, 16731, 16943, 16944, 16945, 16946, 16947, 17019, 17020, 17111, 17235, 17236, 17338, 17405, 17406, 17451, 17452, 17525, 17554, 17621, 17622, 17633, 17653, 17667, 17668, 17731, 17770, 17837, 17838, 17883, 17884, 17986, 18034, 18035, 18036, 18053, 18054, 18099, 18100, 18250, 18251, 18252, 18260, 18269, 18270, 18315, 18316, 18466, 18467, 18468, 18476, 18485, 18486, 18507, 18531, 18532, 18692, 18701, 18702, 18747, 18748, 19876, 19877, 19878, 19879, 19880, 19889, 19909, 20326, 20327, 20328, 20329, 20333, 20334, 20335, 20336, 21187, 21203, 21403, 21419, 21580, 21581, 21582, 21583, 21619, 21635, 21826, 21835, 21851, 22042, 22258, 22702, 22703, 22704, 22705, 22709, 22710, 22711, 22712, 23129, 23149, 24218, 24219, 24220, 24434, 24435, 24436, 24650, 24651, 24652, 25906, 25907, 25908, 26122, 26123, 26124, 26338, 26339, 26340, 26522, 26523, 26524, 26722, 26738, 26739, 26740, 26796, 26797, 26798, 26841, 26842, 26843, 26954, 26955, 26956, 27012, 27013, 27014, 27057, 27058, 27059, 27228, 27229, 27230, 27273, 27274, 27275, 27373, 27374, 27375, 27376, 27377, 27378, 27379, 27671, 27804, 27811, 28096, 28239, 28674, 29184, 29839, 29955, 30171, 30387, 30915, 30925, 31047, 31048, 31049, 31263, 31264, 31265, 31479, 31480, 31481, 31959, 33090, 33091, 33092, 33112, 33306, 33307, 33308, 33315, 33316, 33317, 33318, 33522, 33523, 33524, 33531, 33532, 33533, 33534, 33747, 33748, 33749, 33750, 33963, 33964, 33965, 33966, 34179, 34180, 34181, 34182, 34401, 34402, 34403, 34617, 34618, 34619, 34708, 34833, 34834, 34835, 35597, 37870, 38977, 38978, 38979, 38980, 38981, 38982, 38983, 39193, 39194, 39195, 39196, 39197, 39198, 39199, 39409, 39410, 39411, 39412, 39413, 39414, 39415, 41596, 41597, 41598, 41599, 41600, 43585, };

static uint8_t current_tiles[] = { 236, 236, 220, 237, 237, 237, 220, 108, 109, 109, 110, 220, 220, 126, 127, 127, 128, 63, 63, 220, 220, 144, 145, 147, 146, 63, 220, 220, 108, 109, 110, 220, 63, 126, 127, 128, 220, 63, 108, 109, 110, 144, 145, 146, 63, 220, 220, 220, 126, 127, 128, 220, 63, 144, 145, 146, 220, 63, 218, 218, 63, 220, 63, 218, 218, 220, 218, 218, 220, 63, 220, 63, 220, 220, 108, 109, 110, 236, 236, 236, 126, 127, 128, 144, 145, 146, 184, 162, 163, 164, 180, 181, 182, 198, 199, 200, 218, 218, 184, 218, 219, 219, 218, 219, 219, 218, 219, 219, 184, 238, 238, 219, 219, 19, 19, 19, 19, 19, 19, 63, 19, 19, 19, 19, 19, 19, 184, 19, 19, 19, 19, 19, 19, 184, 19, 19, 19, 37, 37, 37, 18, 19, 19, 19, 20, 237, 237, 237, 237, 237, 18, 19, 19, 19, 20, 237, 237, 237, 237, 237, 18, 19, 19, 19, 20, 236, 236, 236, 236, 236, 184, 184, 63, 63, 63, 63, 108, 109, 110, 108, 109, 110, 184, 126, 127, 128, 126, 127, 128, 144, 145, 146, 144, 145, 146, 184, 184, 63, 108, 109, 109, 109, 110, 63, 63, 126, 127, 127, 127, 128, 63, 126, 127, 127, 127, 128, 63, 126, 127, 127, 127, 128, 144, 145, 147, 145, 146, 218, 218, 63, 218, 218, 219, 219, 219, 218, 218, 63, 219, 219, 219, 184, 184, 218, 218, 63, 219, 219, 219, 218, 218, 219, 108, 109, 110, 219, 219, 218, 218, 126, 127, 128, 218, 219, 219, 218, 218, 144, 145, 146, 218, 219, 219, 184, 218, 218, 218, 219, 219, 218, 218, 237, 237, 237, 237, 237, 184, 184, 237, 237, 237, 237, 237, 237, 237, 237, 219, 219, 219, 219, 238, 238, 238, 238, 219, 219, 219, 219, 219, 219, 219, 237, 237, 237, 237, 237, 237, 237, 237, 184, 184, 162, 163, 164, 180, 181, 182, 198, 199, 200, 108, 109, 110, 126, 127, 128, 144, 145, 146, 108, 109, 110, 184, 126, 127, 128, 162, 163, 164, 162, 163, 164, 144, 145, 146, 180, 181, 182, 180, 181, 182, 198, 199, 200, 198, 199, 200, 237, 237, 237, 237, 237, 237, 237, 63, 63, 63, 63, 63, 63, 63, 63, 219, 219, 219, 63, 63, 162, 163, 164, 180, 181, 182, 198, 199, 200, 184, 162, 163, 164, 184, 180, 181, 182, 108, 109, 109, 110, 198, 199, 200, 126, 127, 127, 128, 126, 127, 127, 128, 126, 127, 127, 128, 144, 147, 147, 146, 162, 163, 164, 180, 181, 182, 184, 198, 199, 200, 184, 184, 111, 18, 19, 19, 19, 25, 38, 111, 18, 19, 19, 19, 20, 111, 0, 44, 19, 19, 19, 20, 111, 236, 236, 236, 236, 236, 184, };

const uint16_t barracks_indices[] = { 1826, 3093, 2604, 4913, 16143, 16158, 21271, 18965, 30547, 31280, 39321, };

static uint8_t barracks_data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

const uint16_t blueprints_data[] = { 462, };

