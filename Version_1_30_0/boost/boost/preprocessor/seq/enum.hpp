# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.  Permission to copy, use,        *
#  *     modify, sell, and distribute this software is granted provided       *
#  *     this copyright notice appears in all copies.  This software is       *
#  *     provided "as is" without express or implied warranty, and with       *
#  *     no claim at to its suitability for any purpose.                      *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_SEQ_ENUM_HPP
# define BOOST_PREPROCESSOR_SEQ_ENUM_HPP
#
# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/seq/size.hpp>
#
# /* BOOST_PP_SEQ_ENUM */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_SEQ_ENUM(seq) BOOST_PP_CAT(BOOST_PP_SEQ_ENUM_, BOOST_PP_SEQ_SIZE(seq)) seq
# else
#    define BOOST_PP_SEQ_ENUM(seq) BOOST_PP_SEQ_ENUM_I(seq)
#    define BOOST_PP_SEQ_ENUM_I(seq) BOOST_PP_CAT(BOOST_PP_SEQ_ENUM_, BOOST_PP_SEQ_SIZE(seq)) seq
# endif
#
# define BOOST_PP_SEQ_ENUM_1(x) x
# define BOOST_PP_SEQ_ENUM_2(x) x, BOOST_PP_SEQ_ENUM_1
# define BOOST_PP_SEQ_ENUM_3(x) x, BOOST_PP_SEQ_ENUM_2
# define BOOST_PP_SEQ_ENUM_4(x) x, BOOST_PP_SEQ_ENUM_3
# define BOOST_PP_SEQ_ENUM_5(x) x, BOOST_PP_SEQ_ENUM_4
# define BOOST_PP_SEQ_ENUM_6(x) x, BOOST_PP_SEQ_ENUM_5
# define BOOST_PP_SEQ_ENUM_7(x) x, BOOST_PP_SEQ_ENUM_6
# define BOOST_PP_SEQ_ENUM_8(x) x, BOOST_PP_SEQ_ENUM_7
# define BOOST_PP_SEQ_ENUM_9(x) x, BOOST_PP_SEQ_ENUM_8
# define BOOST_PP_SEQ_ENUM_10(x) x, BOOST_PP_SEQ_ENUM_9
# define BOOST_PP_SEQ_ENUM_11(x) x, BOOST_PP_SEQ_ENUM_10
# define BOOST_PP_SEQ_ENUM_12(x) x, BOOST_PP_SEQ_ENUM_11
# define BOOST_PP_SEQ_ENUM_13(x) x, BOOST_PP_SEQ_ENUM_12
# define BOOST_PP_SEQ_ENUM_14(x) x, BOOST_PP_SEQ_ENUM_13
# define BOOST_PP_SEQ_ENUM_15(x) x, BOOST_PP_SEQ_ENUM_14
# define BOOST_PP_SEQ_ENUM_16(x) x, BOOST_PP_SEQ_ENUM_15
# define BOOST_PP_SEQ_ENUM_17(x) x, BOOST_PP_SEQ_ENUM_16
# define BOOST_PP_SEQ_ENUM_18(x) x, BOOST_PP_SEQ_ENUM_17
# define BOOST_PP_SEQ_ENUM_19(x) x, BOOST_PP_SEQ_ENUM_18
# define BOOST_PP_SEQ_ENUM_20(x) x, BOOST_PP_SEQ_ENUM_19
# define BOOST_PP_SEQ_ENUM_21(x) x, BOOST_PP_SEQ_ENUM_20
# define BOOST_PP_SEQ_ENUM_22(x) x, BOOST_PP_SEQ_ENUM_21
# define BOOST_PP_SEQ_ENUM_23(x) x, BOOST_PP_SEQ_ENUM_22
# define BOOST_PP_SEQ_ENUM_24(x) x, BOOST_PP_SEQ_ENUM_23
# define BOOST_PP_SEQ_ENUM_25(x) x, BOOST_PP_SEQ_ENUM_24
# define BOOST_PP_SEQ_ENUM_26(x) x, BOOST_PP_SEQ_ENUM_25
# define BOOST_PP_SEQ_ENUM_27(x) x, BOOST_PP_SEQ_ENUM_26
# define BOOST_PP_SEQ_ENUM_28(x) x, BOOST_PP_SEQ_ENUM_27
# define BOOST_PP_SEQ_ENUM_29(x) x, BOOST_PP_SEQ_ENUM_28
# define BOOST_PP_SEQ_ENUM_30(x) x, BOOST_PP_SEQ_ENUM_29
# define BOOST_PP_SEQ_ENUM_31(x) x, BOOST_PP_SEQ_ENUM_30
# define BOOST_PP_SEQ_ENUM_32(x) x, BOOST_PP_SEQ_ENUM_31
# define BOOST_PP_SEQ_ENUM_33(x) x, BOOST_PP_SEQ_ENUM_32
# define BOOST_PP_SEQ_ENUM_34(x) x, BOOST_PP_SEQ_ENUM_33
# define BOOST_PP_SEQ_ENUM_35(x) x, BOOST_PP_SEQ_ENUM_34
# define BOOST_PP_SEQ_ENUM_36(x) x, BOOST_PP_SEQ_ENUM_35
# define BOOST_PP_SEQ_ENUM_37(x) x, BOOST_PP_SEQ_ENUM_36
# define BOOST_PP_SEQ_ENUM_38(x) x, BOOST_PP_SEQ_ENUM_37
# define BOOST_PP_SEQ_ENUM_39(x) x, BOOST_PP_SEQ_ENUM_38
# define BOOST_PP_SEQ_ENUM_40(x) x, BOOST_PP_SEQ_ENUM_39
# define BOOST_PP_SEQ_ENUM_41(x) x, BOOST_PP_SEQ_ENUM_40
# define BOOST_PP_SEQ_ENUM_42(x) x, BOOST_PP_SEQ_ENUM_41
# define BOOST_PP_SEQ_ENUM_43(x) x, BOOST_PP_SEQ_ENUM_42
# define BOOST_PP_SEQ_ENUM_44(x) x, BOOST_PP_SEQ_ENUM_43
# define BOOST_PP_SEQ_ENUM_45(x) x, BOOST_PP_SEQ_ENUM_44
# define BOOST_PP_SEQ_ENUM_46(x) x, BOOST_PP_SEQ_ENUM_45
# define BOOST_PP_SEQ_ENUM_47(x) x, BOOST_PP_SEQ_ENUM_46
# define BOOST_PP_SEQ_ENUM_48(x) x, BOOST_PP_SEQ_ENUM_47
# define BOOST_PP_SEQ_ENUM_49(x) x, BOOST_PP_SEQ_ENUM_48
# define BOOST_PP_SEQ_ENUM_50(x) x, BOOST_PP_SEQ_ENUM_49
# define BOOST_PP_SEQ_ENUM_51(x) x, BOOST_PP_SEQ_ENUM_50
# define BOOST_PP_SEQ_ENUM_52(x) x, BOOST_PP_SEQ_ENUM_51
# define BOOST_PP_SEQ_ENUM_53(x) x, BOOST_PP_SEQ_ENUM_52
# define BOOST_PP_SEQ_ENUM_54(x) x, BOOST_PP_SEQ_ENUM_53
# define BOOST_PP_SEQ_ENUM_55(x) x, BOOST_PP_SEQ_ENUM_54
# define BOOST_PP_SEQ_ENUM_56(x) x, BOOST_PP_SEQ_ENUM_55
# define BOOST_PP_SEQ_ENUM_57(x) x, BOOST_PP_SEQ_ENUM_56
# define BOOST_PP_SEQ_ENUM_58(x) x, BOOST_PP_SEQ_ENUM_57
# define BOOST_PP_SEQ_ENUM_59(x) x, BOOST_PP_SEQ_ENUM_58
# define BOOST_PP_SEQ_ENUM_60(x) x, BOOST_PP_SEQ_ENUM_59
# define BOOST_PP_SEQ_ENUM_61(x) x, BOOST_PP_SEQ_ENUM_60
# define BOOST_PP_SEQ_ENUM_62(x) x, BOOST_PP_SEQ_ENUM_61
# define BOOST_PP_SEQ_ENUM_63(x) x, BOOST_PP_SEQ_ENUM_62
# define BOOST_PP_SEQ_ENUM_64(x) x, BOOST_PP_SEQ_ENUM_63
# define BOOST_PP_SEQ_ENUM_65(x) x, BOOST_PP_SEQ_ENUM_64
# define BOOST_PP_SEQ_ENUM_66(x) x, BOOST_PP_SEQ_ENUM_65
# define BOOST_PP_SEQ_ENUM_67(x) x, BOOST_PP_SEQ_ENUM_66
# define BOOST_PP_SEQ_ENUM_68(x) x, BOOST_PP_SEQ_ENUM_67
# define BOOST_PP_SEQ_ENUM_69(x) x, BOOST_PP_SEQ_ENUM_68
# define BOOST_PP_SEQ_ENUM_70(x) x, BOOST_PP_SEQ_ENUM_69
# define BOOST_PP_SEQ_ENUM_71(x) x, BOOST_PP_SEQ_ENUM_70
# define BOOST_PP_SEQ_ENUM_72(x) x, BOOST_PP_SEQ_ENUM_71
# define BOOST_PP_SEQ_ENUM_73(x) x, BOOST_PP_SEQ_ENUM_72
# define BOOST_PP_SEQ_ENUM_74(x) x, BOOST_PP_SEQ_ENUM_73
# define BOOST_PP_SEQ_ENUM_75(x) x, BOOST_PP_SEQ_ENUM_74
# define BOOST_PP_SEQ_ENUM_76(x) x, BOOST_PP_SEQ_ENUM_75
# define BOOST_PP_SEQ_ENUM_77(x) x, BOOST_PP_SEQ_ENUM_76
# define BOOST_PP_SEQ_ENUM_78(x) x, BOOST_PP_SEQ_ENUM_77
# define BOOST_PP_SEQ_ENUM_79(x) x, BOOST_PP_SEQ_ENUM_78
# define BOOST_PP_SEQ_ENUM_80(x) x, BOOST_PP_SEQ_ENUM_79
# define BOOST_PP_SEQ_ENUM_81(x) x, BOOST_PP_SEQ_ENUM_80
# define BOOST_PP_SEQ_ENUM_82(x) x, BOOST_PP_SEQ_ENUM_81
# define BOOST_PP_SEQ_ENUM_83(x) x, BOOST_PP_SEQ_ENUM_82
# define BOOST_PP_SEQ_ENUM_84(x) x, BOOST_PP_SEQ_ENUM_83
# define BOOST_PP_SEQ_ENUM_85(x) x, BOOST_PP_SEQ_ENUM_84
# define BOOST_PP_SEQ_ENUM_86(x) x, BOOST_PP_SEQ_ENUM_85
# define BOOST_PP_SEQ_ENUM_87(x) x, BOOST_PP_SEQ_ENUM_86
# define BOOST_PP_SEQ_ENUM_88(x) x, BOOST_PP_SEQ_ENUM_87
# define BOOST_PP_SEQ_ENUM_89(x) x, BOOST_PP_SEQ_ENUM_88
# define BOOST_PP_SEQ_ENUM_90(x) x, BOOST_PP_SEQ_ENUM_89
# define BOOST_PP_SEQ_ENUM_91(x) x, BOOST_PP_SEQ_ENUM_90
# define BOOST_PP_SEQ_ENUM_92(x) x, BOOST_PP_SEQ_ENUM_91
# define BOOST_PP_SEQ_ENUM_93(x) x, BOOST_PP_SEQ_ENUM_92
# define BOOST_PP_SEQ_ENUM_94(x) x, BOOST_PP_SEQ_ENUM_93
# define BOOST_PP_SEQ_ENUM_95(x) x, BOOST_PP_SEQ_ENUM_94
# define BOOST_PP_SEQ_ENUM_96(x) x, BOOST_PP_SEQ_ENUM_95
# define BOOST_PP_SEQ_ENUM_97(x) x, BOOST_PP_SEQ_ENUM_96
# define BOOST_PP_SEQ_ENUM_98(x) x, BOOST_PP_SEQ_ENUM_97
# define BOOST_PP_SEQ_ENUM_99(x) x, BOOST_PP_SEQ_ENUM_98
# define BOOST_PP_SEQ_ENUM_100(x) x, BOOST_PP_SEQ_ENUM_99
# define BOOST_PP_SEQ_ENUM_101(x) x, BOOST_PP_SEQ_ENUM_100
# define BOOST_PP_SEQ_ENUM_102(x) x, BOOST_PP_SEQ_ENUM_101
# define BOOST_PP_SEQ_ENUM_103(x) x, BOOST_PP_SEQ_ENUM_102
# define BOOST_PP_SEQ_ENUM_104(x) x, BOOST_PP_SEQ_ENUM_103
# define BOOST_PP_SEQ_ENUM_105(x) x, BOOST_PP_SEQ_ENUM_104
# define BOOST_PP_SEQ_ENUM_106(x) x, BOOST_PP_SEQ_ENUM_105
# define BOOST_PP_SEQ_ENUM_107(x) x, BOOST_PP_SEQ_ENUM_106
# define BOOST_PP_SEQ_ENUM_108(x) x, BOOST_PP_SEQ_ENUM_107
# define BOOST_PP_SEQ_ENUM_109(x) x, BOOST_PP_SEQ_ENUM_108
# define BOOST_PP_SEQ_ENUM_110(x) x, BOOST_PP_SEQ_ENUM_109
# define BOOST_PP_SEQ_ENUM_111(x) x, BOOST_PP_SEQ_ENUM_110
# define BOOST_PP_SEQ_ENUM_112(x) x, BOOST_PP_SEQ_ENUM_111
# define BOOST_PP_SEQ_ENUM_113(x) x, BOOST_PP_SEQ_ENUM_112
# define BOOST_PP_SEQ_ENUM_114(x) x, BOOST_PP_SEQ_ENUM_113
# define BOOST_PP_SEQ_ENUM_115(x) x, BOOST_PP_SEQ_ENUM_114
# define BOOST_PP_SEQ_ENUM_116(x) x, BOOST_PP_SEQ_ENUM_115
# define BOOST_PP_SEQ_ENUM_117(x) x, BOOST_PP_SEQ_ENUM_116
# define BOOST_PP_SEQ_ENUM_118(x) x, BOOST_PP_SEQ_ENUM_117
# define BOOST_PP_SEQ_ENUM_119(x) x, BOOST_PP_SEQ_ENUM_118
# define BOOST_PP_SEQ_ENUM_120(x) x, BOOST_PP_SEQ_ENUM_119
# define BOOST_PP_SEQ_ENUM_121(x) x, BOOST_PP_SEQ_ENUM_120
# define BOOST_PP_SEQ_ENUM_122(x) x, BOOST_PP_SEQ_ENUM_121
# define BOOST_PP_SEQ_ENUM_123(x) x, BOOST_PP_SEQ_ENUM_122
# define BOOST_PP_SEQ_ENUM_124(x) x, BOOST_PP_SEQ_ENUM_123
# define BOOST_PP_SEQ_ENUM_125(x) x, BOOST_PP_SEQ_ENUM_124
# define BOOST_PP_SEQ_ENUM_126(x) x, BOOST_PP_SEQ_ENUM_125
# define BOOST_PP_SEQ_ENUM_127(x) x, BOOST_PP_SEQ_ENUM_126
# define BOOST_PP_SEQ_ENUM_128(x) x, BOOST_PP_SEQ_ENUM_127
# define BOOST_PP_SEQ_ENUM_129(x) x, BOOST_PP_SEQ_ENUM_128
# define BOOST_PP_SEQ_ENUM_130(x) x, BOOST_PP_SEQ_ENUM_129
# define BOOST_PP_SEQ_ENUM_131(x) x, BOOST_PP_SEQ_ENUM_130
# define BOOST_PP_SEQ_ENUM_132(x) x, BOOST_PP_SEQ_ENUM_131
# define BOOST_PP_SEQ_ENUM_133(x) x, BOOST_PP_SEQ_ENUM_132
# define BOOST_PP_SEQ_ENUM_134(x) x, BOOST_PP_SEQ_ENUM_133
# define BOOST_PP_SEQ_ENUM_135(x) x, BOOST_PP_SEQ_ENUM_134
# define BOOST_PP_SEQ_ENUM_136(x) x, BOOST_PP_SEQ_ENUM_135
# define BOOST_PP_SEQ_ENUM_137(x) x, BOOST_PP_SEQ_ENUM_136
# define BOOST_PP_SEQ_ENUM_138(x) x, BOOST_PP_SEQ_ENUM_137
# define BOOST_PP_SEQ_ENUM_139(x) x, BOOST_PP_SEQ_ENUM_138
# define BOOST_PP_SEQ_ENUM_140(x) x, BOOST_PP_SEQ_ENUM_139
# define BOOST_PP_SEQ_ENUM_141(x) x, BOOST_PP_SEQ_ENUM_140
# define BOOST_PP_SEQ_ENUM_142(x) x, BOOST_PP_SEQ_ENUM_141
# define BOOST_PP_SEQ_ENUM_143(x) x, BOOST_PP_SEQ_ENUM_142
# define BOOST_PP_SEQ_ENUM_144(x) x, BOOST_PP_SEQ_ENUM_143
# define BOOST_PP_SEQ_ENUM_145(x) x, BOOST_PP_SEQ_ENUM_144
# define BOOST_PP_SEQ_ENUM_146(x) x, BOOST_PP_SEQ_ENUM_145
# define BOOST_PP_SEQ_ENUM_147(x) x, BOOST_PP_SEQ_ENUM_146
# define BOOST_PP_SEQ_ENUM_148(x) x, BOOST_PP_SEQ_ENUM_147
# define BOOST_PP_SEQ_ENUM_149(x) x, BOOST_PP_SEQ_ENUM_148
# define BOOST_PP_SEQ_ENUM_150(x) x, BOOST_PP_SEQ_ENUM_149
# define BOOST_PP_SEQ_ENUM_151(x) x, BOOST_PP_SEQ_ENUM_150
# define BOOST_PP_SEQ_ENUM_152(x) x, BOOST_PP_SEQ_ENUM_151
# define BOOST_PP_SEQ_ENUM_153(x) x, BOOST_PP_SEQ_ENUM_152
# define BOOST_PP_SEQ_ENUM_154(x) x, BOOST_PP_SEQ_ENUM_153
# define BOOST_PP_SEQ_ENUM_155(x) x, BOOST_PP_SEQ_ENUM_154
# define BOOST_PP_SEQ_ENUM_156(x) x, BOOST_PP_SEQ_ENUM_155
# define BOOST_PP_SEQ_ENUM_157(x) x, BOOST_PP_SEQ_ENUM_156
# define BOOST_PP_SEQ_ENUM_158(x) x, BOOST_PP_SEQ_ENUM_157
# define BOOST_PP_SEQ_ENUM_159(x) x, BOOST_PP_SEQ_ENUM_158
# define BOOST_PP_SEQ_ENUM_160(x) x, BOOST_PP_SEQ_ENUM_159
# define BOOST_PP_SEQ_ENUM_161(x) x, BOOST_PP_SEQ_ENUM_160
# define BOOST_PP_SEQ_ENUM_162(x) x, BOOST_PP_SEQ_ENUM_161
# define BOOST_PP_SEQ_ENUM_163(x) x, BOOST_PP_SEQ_ENUM_162
# define BOOST_PP_SEQ_ENUM_164(x) x, BOOST_PP_SEQ_ENUM_163
# define BOOST_PP_SEQ_ENUM_165(x) x, BOOST_PP_SEQ_ENUM_164
# define BOOST_PP_SEQ_ENUM_166(x) x, BOOST_PP_SEQ_ENUM_165
# define BOOST_PP_SEQ_ENUM_167(x) x, BOOST_PP_SEQ_ENUM_166
# define BOOST_PP_SEQ_ENUM_168(x) x, BOOST_PP_SEQ_ENUM_167
# define BOOST_PP_SEQ_ENUM_169(x) x, BOOST_PP_SEQ_ENUM_168
# define BOOST_PP_SEQ_ENUM_170(x) x, BOOST_PP_SEQ_ENUM_169
# define BOOST_PP_SEQ_ENUM_171(x) x, BOOST_PP_SEQ_ENUM_170
# define BOOST_PP_SEQ_ENUM_172(x) x, BOOST_PP_SEQ_ENUM_171
# define BOOST_PP_SEQ_ENUM_173(x) x, BOOST_PP_SEQ_ENUM_172
# define BOOST_PP_SEQ_ENUM_174(x) x, BOOST_PP_SEQ_ENUM_173
# define BOOST_PP_SEQ_ENUM_175(x) x, BOOST_PP_SEQ_ENUM_174
# define BOOST_PP_SEQ_ENUM_176(x) x, BOOST_PP_SEQ_ENUM_175
# define BOOST_PP_SEQ_ENUM_177(x) x, BOOST_PP_SEQ_ENUM_176
# define BOOST_PP_SEQ_ENUM_178(x) x, BOOST_PP_SEQ_ENUM_177
# define BOOST_PP_SEQ_ENUM_179(x) x, BOOST_PP_SEQ_ENUM_178
# define BOOST_PP_SEQ_ENUM_180(x) x, BOOST_PP_SEQ_ENUM_179
# define BOOST_PP_SEQ_ENUM_181(x) x, BOOST_PP_SEQ_ENUM_180
# define BOOST_PP_SEQ_ENUM_182(x) x, BOOST_PP_SEQ_ENUM_181
# define BOOST_PP_SEQ_ENUM_183(x) x, BOOST_PP_SEQ_ENUM_182
# define BOOST_PP_SEQ_ENUM_184(x) x, BOOST_PP_SEQ_ENUM_183
# define BOOST_PP_SEQ_ENUM_185(x) x, BOOST_PP_SEQ_ENUM_184
# define BOOST_PP_SEQ_ENUM_186(x) x, BOOST_PP_SEQ_ENUM_185
# define BOOST_PP_SEQ_ENUM_187(x) x, BOOST_PP_SEQ_ENUM_186
# define BOOST_PP_SEQ_ENUM_188(x) x, BOOST_PP_SEQ_ENUM_187
# define BOOST_PP_SEQ_ENUM_189(x) x, BOOST_PP_SEQ_ENUM_188
# define BOOST_PP_SEQ_ENUM_190(x) x, BOOST_PP_SEQ_ENUM_189
# define BOOST_PP_SEQ_ENUM_191(x) x, BOOST_PP_SEQ_ENUM_190
# define BOOST_PP_SEQ_ENUM_192(x) x, BOOST_PP_SEQ_ENUM_191
# define BOOST_PP_SEQ_ENUM_193(x) x, BOOST_PP_SEQ_ENUM_192
# define BOOST_PP_SEQ_ENUM_194(x) x, BOOST_PP_SEQ_ENUM_193
# define BOOST_PP_SEQ_ENUM_195(x) x, BOOST_PP_SEQ_ENUM_194
# define BOOST_PP_SEQ_ENUM_196(x) x, BOOST_PP_SEQ_ENUM_195
# define BOOST_PP_SEQ_ENUM_197(x) x, BOOST_PP_SEQ_ENUM_196
# define BOOST_PP_SEQ_ENUM_198(x) x, BOOST_PP_SEQ_ENUM_197
# define BOOST_PP_SEQ_ENUM_199(x) x, BOOST_PP_SEQ_ENUM_198
# define BOOST_PP_SEQ_ENUM_200(x) x, BOOST_PP_SEQ_ENUM_199
# define BOOST_PP_SEQ_ENUM_201(x) x, BOOST_PP_SEQ_ENUM_200
# define BOOST_PP_SEQ_ENUM_202(x) x, BOOST_PP_SEQ_ENUM_201
# define BOOST_PP_SEQ_ENUM_203(x) x, BOOST_PP_SEQ_ENUM_202
# define BOOST_PP_SEQ_ENUM_204(x) x, BOOST_PP_SEQ_ENUM_203
# define BOOST_PP_SEQ_ENUM_205(x) x, BOOST_PP_SEQ_ENUM_204
# define BOOST_PP_SEQ_ENUM_206(x) x, BOOST_PP_SEQ_ENUM_205
# define BOOST_PP_SEQ_ENUM_207(x) x, BOOST_PP_SEQ_ENUM_206
# define BOOST_PP_SEQ_ENUM_208(x) x, BOOST_PP_SEQ_ENUM_207
# define BOOST_PP_SEQ_ENUM_209(x) x, BOOST_PP_SEQ_ENUM_208
# define BOOST_PP_SEQ_ENUM_210(x) x, BOOST_PP_SEQ_ENUM_209
# define BOOST_PP_SEQ_ENUM_211(x) x, BOOST_PP_SEQ_ENUM_210
# define BOOST_PP_SEQ_ENUM_212(x) x, BOOST_PP_SEQ_ENUM_211
# define BOOST_PP_SEQ_ENUM_213(x) x, BOOST_PP_SEQ_ENUM_212
# define BOOST_PP_SEQ_ENUM_214(x) x, BOOST_PP_SEQ_ENUM_213
# define BOOST_PP_SEQ_ENUM_215(x) x, BOOST_PP_SEQ_ENUM_214
# define BOOST_PP_SEQ_ENUM_216(x) x, BOOST_PP_SEQ_ENUM_215
# define BOOST_PP_SEQ_ENUM_217(x) x, BOOST_PP_SEQ_ENUM_216
# define BOOST_PP_SEQ_ENUM_218(x) x, BOOST_PP_SEQ_ENUM_217
# define BOOST_PP_SEQ_ENUM_219(x) x, BOOST_PP_SEQ_ENUM_218
# define BOOST_PP_SEQ_ENUM_220(x) x, BOOST_PP_SEQ_ENUM_219
# define BOOST_PP_SEQ_ENUM_221(x) x, BOOST_PP_SEQ_ENUM_220
# define BOOST_PP_SEQ_ENUM_222(x) x, BOOST_PP_SEQ_ENUM_221
# define BOOST_PP_SEQ_ENUM_223(x) x, BOOST_PP_SEQ_ENUM_222
# define BOOST_PP_SEQ_ENUM_224(x) x, BOOST_PP_SEQ_ENUM_223
# define BOOST_PP_SEQ_ENUM_225(x) x, BOOST_PP_SEQ_ENUM_224
# define BOOST_PP_SEQ_ENUM_226(x) x, BOOST_PP_SEQ_ENUM_225
# define BOOST_PP_SEQ_ENUM_227(x) x, BOOST_PP_SEQ_ENUM_226
# define BOOST_PP_SEQ_ENUM_228(x) x, BOOST_PP_SEQ_ENUM_227
# define BOOST_PP_SEQ_ENUM_229(x) x, BOOST_PP_SEQ_ENUM_228
# define BOOST_PP_SEQ_ENUM_230(x) x, BOOST_PP_SEQ_ENUM_229
# define BOOST_PP_SEQ_ENUM_231(x) x, BOOST_PP_SEQ_ENUM_230
# define BOOST_PP_SEQ_ENUM_232(x) x, BOOST_PP_SEQ_ENUM_231
# define BOOST_PP_SEQ_ENUM_233(x) x, BOOST_PP_SEQ_ENUM_232
# define BOOST_PP_SEQ_ENUM_234(x) x, BOOST_PP_SEQ_ENUM_233
# define BOOST_PP_SEQ_ENUM_235(x) x, BOOST_PP_SEQ_ENUM_234
# define BOOST_PP_SEQ_ENUM_236(x) x, BOOST_PP_SEQ_ENUM_235
# define BOOST_PP_SEQ_ENUM_237(x) x, BOOST_PP_SEQ_ENUM_236
# define BOOST_PP_SEQ_ENUM_238(x) x, BOOST_PP_SEQ_ENUM_237
# define BOOST_PP_SEQ_ENUM_239(x) x, BOOST_PP_SEQ_ENUM_238
# define BOOST_PP_SEQ_ENUM_240(x) x, BOOST_PP_SEQ_ENUM_239
# define BOOST_PP_SEQ_ENUM_241(x) x, BOOST_PP_SEQ_ENUM_240
# define BOOST_PP_SEQ_ENUM_242(x) x, BOOST_PP_SEQ_ENUM_241
# define BOOST_PP_SEQ_ENUM_243(x) x, BOOST_PP_SEQ_ENUM_242
# define BOOST_PP_SEQ_ENUM_244(x) x, BOOST_PP_SEQ_ENUM_243
# define BOOST_PP_SEQ_ENUM_245(x) x, BOOST_PP_SEQ_ENUM_244
# define BOOST_PP_SEQ_ENUM_246(x) x, BOOST_PP_SEQ_ENUM_245
# define BOOST_PP_SEQ_ENUM_247(x) x, BOOST_PP_SEQ_ENUM_246
# define BOOST_PP_SEQ_ENUM_248(x) x, BOOST_PP_SEQ_ENUM_247
# define BOOST_PP_SEQ_ENUM_249(x) x, BOOST_PP_SEQ_ENUM_248
# define BOOST_PP_SEQ_ENUM_250(x) x, BOOST_PP_SEQ_ENUM_249
# define BOOST_PP_SEQ_ENUM_251(x) x, BOOST_PP_SEQ_ENUM_250
# define BOOST_PP_SEQ_ENUM_252(x) x, BOOST_PP_SEQ_ENUM_251
# define BOOST_PP_SEQ_ENUM_253(x) x, BOOST_PP_SEQ_ENUM_252
# define BOOST_PP_SEQ_ENUM_254(x) x, BOOST_PP_SEQ_ENUM_253
# define BOOST_PP_SEQ_ENUM_255(x) x, BOOST_PP_SEQ_ENUM_254
# define BOOST_PP_SEQ_ENUM_256(x) x, BOOST_PP_SEQ_ENUM_255
#
# endif
