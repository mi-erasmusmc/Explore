test_that("binary_3 trainExplore resultsExplore", {
    
    dataset <- "binary_3"
    config <- getDataSetPath(dataset = dataset) 
    train_data <- farff::readARFF(config$data_path)
    output_path <- paste0(tempdir(), "/", glue::glue("{getRandomId()}"), "/")
    file_name <- paste0(dataset, "_train_data")
    dir.create(output_path)
    if (.Platform$OS.type == "windows") {
      output_path <- gsub("\\\\", "/", output_path)
    }
    
    result <- trainExplore(train_data = train_data,
                           settings_path = NULL,
                           output_path = output_path,
                           file_name = file_name,
                           OutputFile = NULL,
                           StartRulelength = 3,
                           EndRulelength = 3,
                           OperatorMethod = "EXHAUSTIVE", #"MEDIAN",
                           CutoffMethod = "ALL", #"RVAC",
                           ClassFeature = config$class_feature,
                           PositiveClass = config$positive_class,
                           FeatureInclude = "",
                           Maximize = "BALANCEDACCURACY",#"ACCURACY",
                           Accuracy = 0,
                           BalancedAccuracy = 0,
                           Specificity = 0,
                           PrintSettings = TRUE,
                           PrintPerformance = TRUE,
                           Subsumption = TRUE,
                           BranchBound = TRUE,
                           Parallel = FALSE,
                           PrintCutoffSets = TRUE,
                           Sorted = "none",
                           OutputMethod = "BEST", #"EVERY",
                           BinaryReduction = TRUE)
    
    outputFile <- paste0(output_path, file_name, ".result")
    results_list <- resultsExplore(outputFile = outputFile)
    # Expected result is 6
    expect_length(results_list$candidateModels, 40)
    unlink(output_path, recursive = TRUE)

})

test_that("Test binary_3", {
  
  # Binary reduction FALSE
  
  cand_models_binary_3_1_F <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 1, 
                                          EndRulelength = 1,
                                          BinaryReduction = FALSE)
  result_1_F <- length(cand_models_binary_3_1_F$candidate_models)
  # Expected result is 6
  expect_equal(result_1_F, 6)
  
  cand_models_binary_3_2_F <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 2, 
                                          EndRulelength = 2,
                                          BinaryReduction = FALSE)
  result_2_F <- length(cand_models_binary_3_2_F$candidate_models)
  # Expected result is 24
  expect_equal(result_2_F, 24)
  
  cand_models_binary_3_3_F <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 3, 
                                          EndRulelength = 3,
                                          BinaryReduction = FALSE)
  result_3_F <- length(cand_models_binary_3_3_F$candidate_models)
  # Expected result is 64
  expect_equal(result_3_F, 64)
  
  # Binary reduction TRUE
  
  cand_models_binary_3_1_T <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 1, 
                                          EndRulelength = 1,
                                          BinaryReduction = TRUE)
  result_1_T <- length(cand_models_binary_3_1_T$candidate_models)
  # Expected result is 6
  expect_equal(result_1_T, 6)
  
  cand_models_binary_3_2_T <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 2, 
                                          EndRulelength = 2,
                                          BinaryReduction = TRUE)
  result_2_T <- length(cand_models_binary_3_2_T$candidate_models)
  # Expected result is 24
  expect_equal(result_2_T, 24)
  
  cand_models_binary_3_3_T <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 3, 
                                          EndRulelength = 3,
                                          BinaryReduction = TRUE)
  result_3_T <- length(cand_models_binary_3_3_T$candidate_models)
  # FAIL: Current result 64
  expect_equal(result_3_T, 40)
  
})

test_that("Test binary_10 FALSE", {
  
  cand_models_binary_10_1 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 1, 
                                         EndRulelength = 1,
                                         BinaryReduction = FALSE)
  result_1 <- length(cand_models_binary_10_1$candidate_models)
  # Expected result is 20
  expect_equal(result_1, 20)
  
  cand_models_binary_10_2 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 2, 
                                         EndRulelength = 2,
                                         BinaryReduction = FALSE)
  result_2 <- length(cand_models_binary_10_2$candidate_models)
  # Expected result is 360
  expect_equal(result_2, 360)
  
  cand_models_binary_10_3 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 3, 
                                         EndRulelength = 3,
                                         BinaryReduction = FALSE)
  result_3 <- length(cand_models_binary_10_3$candidate_models)
  # Expected result is 5160
  expect_equal(result_3, 5160)
  
  cand_models_binary_10_4 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 4, 
                                         EndRulelength = 4,
                                         BinaryReduction = FALSE)
  result_4 <- length(cand_models_binary_10_4$candidate_models)
  # Expected result is 65700
  expect_equal(result_4, 65700)
  
  cand_models_binary_10_5 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 5, 
                                         EndRulelength = 5,
                                         BinaryReduction = FALSE)
  result_5 <- length(cand_models_binary_10_5$candidate_models)
  # Expected result is 758928
  expect_equal(result_5, 758928)
  
})

test_that("Test binary_10 Binary Reduction TRUE", {
  
  cand_models_binary_10_1 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 1, 
                                         EndRulelength = 1,
                                         BinaryReduction = TRUE)
  result_1 <- length(cand_models_binary_10_1$candidate_models)
  # Expected result is 20
  expect_equal(result_1, 20)
  
  cand_models_binary_10_2 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 2, 
                                         EndRulelength = 2,
                                         BinaryReduction = TRUE)
  result_2 <- length(cand_models_binary_10_2$candidate_models)
  # Expected result is 360
  expect_equal(result_2, 360)
  
  cand_models_binary_10_3 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 3, 
                                         EndRulelength = 3,
                                         BinaryReduction = TRUE)
  result_3 <- length(cand_models_binary_10_3$candidate_models)
  # FAIL: `actual`: 5160
  expect_equal(result_3, 4800)
  
  cand_models_binary_10_4 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 4, 
                                         EndRulelength = 4,
                                         BinaryReduction = TRUE)
  result_4 <- length(cand_models_binary_10_4$candidate_models)
  # Expected result is 56880
  expect_equal(result_4, 56880)
  
  cand_models_binary_10_5 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 5, 
                                         EndRulelength = 5,
                                         BinaryReduction = TRUE)
  result_5 <- length(cand_models_binary_10_5$candidate_models)
  # Expected result is 603648
  expect_equal(result_5, 603648)
  
})

test_that("Test continuous_4_small", {
  
  cand_models_continuous_4_small_1 <- testExplore(dataset = "continuous_4_small", 
                                                  StartRulelength = 1, 
                                                  EndRulelength = 1,
                                                  BinaryReduction = FALSE)
  result_1 <- length(cand_models_continuous_4_small_1$candidate_models)
  # Expected result is 24
  expect_equal(result_1, 24)
  
  cand_models_continuous_4_small_2 <- testExplore(dataset = "continuous_4_small", 
                                                  StartRulelength = 2, 
                                                  EndRulelength = 2,
                                                  BinaryReduction = FALSE)
  result_2 <- length(cand_models_continuous_4_small_2$candidate_models)
  # Expected result is 456
  expect_equal(result_2, 456)
  
  cand_models_continuous_4_small_3 <- testExplore(dataset = "continuous_4_small", 
                                                  StartRulelength = 3, 
                                                  EndRulelength = 3,
                                                  BinaryReduction = FALSE)
  result_3 <- length(cand_models_continuous_4_small_3$candidate_models)
  # Expected result is 5840
  expect_equal(result_3, 5840)
  
})

test_that("Test continuous_4", {
  
  # cand_models_continuous_4_1 <- testExplore(dataset = "continuous_4",
  #                                           StartRulelength = 1,
  #                                           EndRulelength = 1,
  #                                           BinaryReduction = FALSE)
  # result_1 <- length(cand_models_continuous_4_1$candidate_models)
  # # FAIL: `actual`: 64 `expected`: 80
  # expect_equal(result_1, 80)

  
  # TEST CRASHES R
  # cand_models_continuous_4_2 <- testExplore(dataset = "continuous_4", 
  #                                           StartRulelength = 2, 
  #                                           EndRulelength = 2,
  #                                           BinaryReduction = FALSE)
  # result_2 <- length(cand_models_continuous_4_2$candidate_models)
  # # Expected result is 5840
  # expect_equal(result_2, 254)

  
  # TEST CRASHES R  
  # cand_models_continuous_4_3 <- testExplore(dataset = "continuous_4", 
  #                                           StartRulelength = 3, 
  #                                           EndRulelength = 3,
  #                                           BinaryReduction = FALSE)
  # result_3 <- length(cand_models_continuous_4_3$candidate_models)
  # # Expected result is 5840
  # expect_equal(result_3, 1134)
  
})

# Error in parseHeader(path) : 
#   Invalid column specification line found in ARFF header

# test_that("Test categorical_4", {
# 
#   cand_models_categorical_4_1 <- testExplore(dataset = "categorical_4",
#                                              StartRulelength = 1,
#                                              EndRulelength = 1,
#                                              BinaryReduction = FALSE)
#   result_1 <- length(cand_models_categorical_4_1$candidate_models)
#   # Expected result is 5840
#   expect_equal(result_1, 12)
# 
#   cand_models_categorical_4_2 <- testExplore(dataset = "categorical_4",
#                                              StartRulelength = 2,
#                                              EndRulelength = 2,
#                                              BinaryReduction = FALSE)
#   result_2 <- length(cand_models_categorical_4_2$candidate_models)
#   # Expected result is 5840
#   expect_equal(result_2, 120)
# 
#   cand_models_categorical_4_3 <- testExplore(dataset = "categorical_4",
#                                              StartRulelength = 3,
#                                              EndRulelength = 3,
#                                              BinaryReduction = FALSE)
#   result_3 <- length(cand_models_categorical_4_3$candidate_models)
#   # Expected result is 5840
#   expect_equal(result_3, 864)
# 
# })

# Error in parseHeader(path) : 
#   Invalid column specification line found in ARFF header

# test_that("Test categorical_4_large", {
# 
#   cand_models_categorical_4_large_1 <- testExplore(dataset = "categorical_4_large",
#                                                    StartRulelength = 1,
#                                                    EndRulelength = 1,
#                                                    BinaryReduction = FALSE)
#   result_1 <- length(cand_models_categorical_4_large_1$candidate_models)
#   expect_equal(result_1, 12)
# 
#   cand_models_categorical_4_large_2 <- testExplore(dataset = "categorical_4_large",
#                                                    StartRulelength = 2,
#                                                    EndRulelength = 2,
#                                                    BinaryReduction = FALSE)
#   result_2 <- length(cand_models_categorical_4_large_2$candidate_models)
#   expect_equal(result_2, 120)
# 
#   cand_models_categorical_4_large_3 <- testExplore(dataset = "categorical_4_large",
#                                                    StartRulelength = 3,
#                                                    EndRulelength = 3,
#                                                    BinaryReduction = FALSE)
#   result_3 <- length(cand_models_categorical_4_large_3$candidate_models)
#   expect_equal(result_3, 864)
# 
# })

test_that("Test mix_4", {
  
  cand_models_mix_4_1 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 1, 
                                     EndRulelength = 1,
                                     BinaryReduction = FALSE)
  result_1 <- length(cand_models_mix_4_1$candidate_models)
  # Expected result is 20
  expect_equal(result_1, 20)
  
  cand_models_mix_4_2 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 2, 
                                     EndRulelength = 2,
                                     BinaryReduction = FALSE)
  result_2 <- length(cand_models_mix_4_2$candidate_models)
  # Expected result is 234
  expect_equal(result_2, 234)
  
  cand_models_mix_4_3 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 3, 
                                     EndRulelength = 3,
                                     BinaryReduction = FALSE)
  result_3 <- length(cand_models_mix_4_3$candidate_models)
  # Expected result is 1940
  expect_equal(result_3, 1940)
  
  cand_models_mix_4_4 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 4, 
                                     EndRulelength = 4,
                                     BinaryReduction = FALSE)
  result_2 <- length(cand_models_mix_4_4$candidate_models)
  # Expected result is 16574
  expect_equal(result_2, 16574)
  
  cand_models_mix_4_5 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 5, 
                                     EndRulelength = 5,
                                     BinaryReduction = FALSE)
  result_3 <- length(cand_models_mix_4_5$candidate_models)
  # Expected result is 119548
  expect_equal(result_3, 119548)
  
})

test_that("Test mix_4_ordered", {
  
  cand_models_mix_4_ordered_1 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 1, 
                                             EndRulelength = 1,
                                             BinaryReduction = TRUE)
  result_1 <- length(cand_models_mix_4_ordered_1$candidate_models)
  # Expected result is 20
  expect_equal(result_1, 20)
  
  cand_models_mix_4_ordered_2 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 2, 
                                             EndRulelength = 2,
                                             BinaryReduction = TRUE)
  result_2 <- length(cand_models_mix_4_ordered_2$candidate_models)
  # Expected result is 234
  expect_equal(result_2, 234)
  
  cand_models_mix_4_ordered_3 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 3, 
                                             EndRulelength = 3,
                                             BinaryReduction = TRUE)
  result_3 <- length(cand_models_mix_4_ordered_3$candidate_models)
  # Expected result is 1832
  expect_equal(result_3, 1832)
  
  cand_models_mix_4_ordered_4 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 4, 
                                             EndRulelength = 4,
                                             BinaryReduction = TRUE)
  result_2 <- length(cand_models_mix_4_ordered_4$candidate_models)
  # Expected result is 14852
  expect_equal(result_2, 14852)
  
  cand_models_mix_4_ordered_5 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 5, 
                                             EndRulelength = 5,
                                             BinaryReduction = TRUE)
  result_3 <- length(cand_models_mix_4_ordered_5$candidate_models)
  # Expected result is 100390
  expect_equal(result_3, 100390)
  
})
