# 目的
研究动态链接库间接依赖的行为，例如存在如下依赖关系：app -> bar.so -> foo.so，那么bar.so生成时，是否指定foo.so的影响。
参见：http://www.kaizou.org/2015/01/linux-libraries.html


