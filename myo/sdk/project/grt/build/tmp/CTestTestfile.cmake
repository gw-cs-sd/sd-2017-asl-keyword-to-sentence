# CMake generated Testfile for 
# Source directory: /Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/myo/sdk/project/grt/build
# Build directory: /Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/myo/sdk/project/grt/build/tmp
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ANBCTest "ANBCTest")
add_test(AdaBoostTest "AdaBoostTest")
add_test(BAGTest "BAGTest")
add_test(DTWTest "DTWTest")
add_test(DecisionTreeTest "DecisionTreeTest")
add_test(FiniteStateMachineTest "FiniteStateMachineTest")
add_test(GMMTest "GMMTest")
add_test(HMMTest "HMMTest")
add_test(KNNTest "KNNTest")
add_test(LDATest "LDATest")
add_test(MinDistTest "MinDistTest")
add_test(RandomForestsTest "RandomForestsTest")
add_test(SVMTest "SVMTest")
add_test(SoftmaxTest "SoftmaxTest")
add_test(SwipeDetectorTest "SwipeDetectorTest")
add_test(ClassifierTest "ClassifierTest")
add_test(MatrixTest "MatrixTest")
add_test(VectorFloatTest "VectorFloatTest")
add_test(VectorTest "VectorTest")
add_test(MLPRegressionTest "MLPRegressionTest")
add_test(CommandLineParserTest "CommandLineParserTest")
add_test(DictTest "DictTest")
add_test(DynamicTypeTest "DynamicTypeTest")
add_test(TypedefsTest "TypedefsTest")
subdirs("third_party/gtest")
