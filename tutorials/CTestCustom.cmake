#---Custom CTest settings---------------------------------------------------

set(CTEST_CUSTOM_TESTS_IGNORE ${CTEST_CUSTOM_TESTS_IGNORE} tutorial-pyroot-zdemo)

if("$ENV{COMPILER}" STREQUAL "classic") #  TTreeProcessorM{T,P} are not available
  list(APPEND CTEST_CUSTOM_TESTS_IGNORE tutorial-multicore-mp102_readNtuplesFillHistosAndFit)
endif()

if (CTEST_BUILD_NAME MATCHES arm64 AND CTEST_BUILD_NAME MATCHES dbg)
  # these tutorials are disabled as they timeout
  list(APPEND CTEST_CUSTOM_TESTS_IGNORE tutorial-roostats-StandardBayesianNumericalDemo tutorial-roostats-OneSidedFrequentistUpperLimitWithBands tutorial-tmva-TMVAClassification tutorial-tmva-TMVARegression tutorial-tmva-TMVAMulticlass  tutorial-roostats-TwoSidedFrequentistUpperLimitWithBands)
endif()
