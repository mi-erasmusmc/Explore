test_that("Test binary_3", {
  
  # Binary reduction FALSE
  
  cand_models_binary_3_1_F <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 1, 
                                          EndRulelength = 1,
                                          BinaryReduction = FALSE)
  result_1_F <- length(cand_models_binary_3_1_F$candidate_models)
  expect_equal(result_1_F, 6)
  
  cand_models_binary_3_2_F <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 2, 
                                          EndRulelength = 2,
                                          BinaryReduction = FALSE)
  result_2_F <- length(cand_models_binary_3_2_F$candidate_models)
  expect_equal(result_2_F, 24)
  
  cand_models_binary_3_3_F <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 3, 
                                          EndRulelength = 3,
                                          BinaryReduction = FALSE)
  result_3_F <- length(cand_models_binary_3_3_F$candidate_models)
  expect_equal(result_3_F, 64)
  
  # Binary reduction TRUE
  
  cand_models_binary_3_1_T <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 1, 
                                          EndRulelength = 1,
                                          BinaryReduction = TRUE)
  result_1_T <- length(cand_models_binary_3_1_T$candidate_models)
  expect_equal(result_1_T, 6)
  
  cand_models_binary_3_2_T <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 2, 
                                          EndRulelength = 2,
                                          BinaryReduction = TRUE)
  result_2_T <- length(cand_models_binary_3_2_T$candidate_models)
  expect_equal(result_2_T, 24)
  
  cand_models_binary_3_3_T <- testExplore(dataset = "binary_3", 
                                          StartRulelength = 3, 
                                          EndRulelength = 3,
                                          BinaryReduction = TRUE)
  result_3_T <- length(cand_models_binary_3_3_T$candidate_models)
  expect_equal(result_3_T, 40)
  
})

test_that("Test binary_10 FALSE", {
  
  cand_models_binary_10_1 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 1, 
                                         EndRulelength = 1,
                                         BinaryReduction = FALSE)
  result_1 <- length(cand_models_binary_10_1$candidate_models)
  expect_equal(result_1, 20)
  
  cand_models_binary_10_2 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 2, 
                                         EndRulelength = 2,
                                         BinaryReduction = FALSE)
  result_2 <- length(cand_models_binary_10_2$candidate_models)
  expect_equal(result_2, 360)
  
  cand_models_binary_10_3 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 3, 
                                         EndRulelength = 3,
                                         BinaryReduction = FALSE)
  result_3 <- length(cand_models_binary_10_3$candidate_models)
  expect_equal(result_3, 5160)
  
  cand_models_binary_10_4 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 4, 
                                         EndRulelength = 4,
                                         BinaryReduction = FALSE)
  result_4 <- length(cand_models_binary_10_4$candidate_models)
  expect_equal(result_4, 65700)
  
  cand_models_binary_10_5 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 5, 
                                         EndRulelength = 5,
                                         BinaryReduction = FALSE)
  result_5 <- length(cand_models_binary_10_5$candidate_models)
  expect_equal(result_5, 758928)
  
})

test_that("Test binary_10 TRUE", {
  
  cand_models_binary_10_1 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 1, 
                                         EndRulelength = 1,
                                         BinaryReduction = TRUE)
  result_1 <- length(cand_models_binary_10_1$candidate_models)
  expect_equal(result_1, 20)
  
  cand_models_binary_10_2 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 2, 
                                         EndRulelength = 2,
                                         BinaryReduction = TRUE)
  result_2 <- length(cand_models_binary_10_2$candidate_models)
  expect_equal(result_2, 360)
  
  cand_models_binary_10_3 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 3, 
                                         EndRulelength = 3,
                                         BinaryReduction = TRUE)
  result_3 <- length(cand_models_binary_10_3$candidate_models)
  expect_equal(result_3, 4800)
  
  cand_models_binary_10_4 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 4, 
                                         EndRulelength = 4,
                                         BinaryReduction = TRUE)
  result_4 <- length(cand_models_binary_10_4$candidate_models)
  expect_equal(result_4, 56880)
  
  cand_models_binary_10_5 <- testExplore(dataset = "binary_10", 
                                         StartRulelength = 5, 
                                         EndRulelength = 5,
                                         BinaryReduction = TRUE)
  result_5 <- length(cand_models_binary_10_5$candidate_models)
  expect_equal(result_5, 603648)
  
})

test_that("Test continuous_4_small", {
  
  cand_models_continuous_4_small_1 <- testExplore(dataset = "continuous_4_small", 
                                                  StartRulelength = 1, 
                                                  EndRulelength = 1,
                                                  BinaryReduction = FALSE)
  result_1 <- length(cand_models_continuous_4_small_1$candidate_models)
  expect_equal(result_1, 24)
  
  cand_models_continuous_4_small_2 <- testExplore(dataset = "continuous_4_small", 
                                                  StartRulelength = 2, 
                                                  EndRulelength = 2,
                                                  BinaryReduction = FALSE)
  result_2 <- length(cand_models_continuous_4_small_2$candidate_models)
  expect_equal(result_2, 456)
  
  cand_models_continuous_4_small_3 <- testExplore(dataset = "continuous_4_small", 
                                                  StartRulelength = 3, 
                                                  EndRulelength = 3,
                                                  BinaryReduction = FALSE)
  result_3 <- length(cand_models_continuous_4_small_3$candidate_models)
  expect_equal(result_3, 5840)
  
})

test_that("Test continuous_4", {
  
  cand_models_continuous_4_1 <- testExplore(dataset = "continuous_4", 
                                            StartRulelength = 1, 
                                            EndRulelength = 1,
                                            BinaryReduction = FALSE)
  result_1 <- length(cand_models_continuous_4_1$candidate_models)
  expect_equal(result_1, 80)
  
  cand_models_continuous_4_2 <- testExplore(dataset = "continuous_4", 
                                            StartRulelength = 2, 
                                            EndRulelength = 2,
                                            BinaryReduction = FALSE)
  result_2 <- length(cand_models_continuous_4_2$candidate_models)
  expect_equal(result_2, 5160)
  
  cand_models_continuous_4_3 <- testExplore(dataset = "continuous_4", 
                                            StartRulelength = 3, 
                                            EndRulelength = 3,
                                            BinaryReduction = FALSE)
  result_3 <- length(cand_models_continuous_4_3$candidate_models)
  expect_equal(result_3, 236560)
  
})

test_that("Test categorical_4", {
  
  cand_models_categorical_4_1 <- testExplore(dataset = "categorical_4", 
                                             StartRulelength = 1, 
                                             EndRulelength = 1,
                                             BinaryReduction = FALSE)
  result_1 <- length(cand_models_categorical_4_1$candidate_models)
  expect_equal(result_1, 12)
  
  cand_models_categorical_4_2 <- testExplore(dataset = "categorical_4", 
                                             StartRulelength = 2, 
                                             EndRulelength = 2,
                                             BinaryReduction = FALSE)
  result_2 <- length(cand_models_categorical_4_2$candidate_models)
  expect_equal(result_2, 120)
  
  cand_models_categorical_4_3 <- testExplore(dataset = "categorical_4", 
                                             StartRulelength = 3, 
                                             EndRulelength = 3,
                                             BinaryReduction = FALSE)
  result_3 <- length(cand_models_categorical_4_3$candidate_models)
  expect_equal(result_3, 864)
  
})

test_that("Test categorical_4_large", {
  
  cand_models_categorical_4_large_1 <- testExplore(dataset = "categorical_4_large", 
                                                   StartRulelength = 1, 
                                                   EndRulelength = 1,
                                                   BinaryReduction = FALSE)
  result_1 <- length(cand_models_categorical_4_large_1$candidate_models)
  expect_equal(result_1, 12)
  
  cand_models_categorical_4_large_2 <- testExplore(dataset = "categorical_4_large", 
                                                   StartRulelength = 2, 
                                                   EndRulelength = 2,
                                                   BinaryReduction = FALSE)
  result_2 <- length(cand_models_categorical_4_large_2$candidate_models)
  expect_equal(result_2, 120)
  
  cand_models_categorical_4_large_3 <- testExplore(dataset = "categorical_4_large", 
                                                   StartRulelength = 3, 
                                                   EndRulelength = 3,
                                                   BinaryReduction = FALSE)
  result_3 <- length(cand_models_categorical_4_large_3$candidate_models)
  expect_equal(result_3, 864)
  
})

test_that("Test mix_4", {
  
  cand_models_mix_4_1 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 1, 
                                     EndRulelength = 1,
                                     BinaryReduction = FALSE)
  result_1 <- length(cand_models_mix_4_1$candidate_models)
  expect_equal(result_1, 20)
  
  cand_models_mix_4_2 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 2, 
                                     EndRulelength = 2,
                                     BinaryReduction = FALSE)
  result_2 <- length(cand_models_mix_4_2$candidate_models)
  expect_equal(result_2, 234)
  
  cand_models_mix_4_3 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 3, 
                                     EndRulelength = 3,
                                     BinaryReduction = FALSE)
  result_3 <- length(cand_models_mix_4_3$candidate_models)
  expect_equal(result_3, 1940)
  
  cand_models_mix_4_4 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 4, 
                                     EndRulelength = 4,
                                     BinaryReduction = FALSE)
  result_2 <- length(cand_models_mix_4_4$candidate_models)
  expect_equal(result_2, 16574)
  
  cand_models_mix_4_5 <- testExplore(dataset = "mix_4", 
                                     StartRulelength = 5, 
                                     EndRulelength = 5,
                                     BinaryReduction = FALSE)
  result_3 <- length(cand_models_mix_4_5$candidate_models)
  expect_equal(result_3, 119548)
  
})

test_that("Test mix_4_ordered", {
  
  cand_models_mix_4_ordered_1 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 1, 
                                             EndRulelength = 1,
                                             BinaryReduction = TRUE)
  result_1 <- length(cand_models_mix_4_ordered_1$candidate_models)
  expect_equal(result_1, 20)
  
  cand_models_mix_4_ordered_2 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 2, 
                                             EndRulelength = 2,
                                             BinaryReduction = TRUE)
  result_2 <- length(cand_models_mix_4_ordered_2$candidate_models)
  expect_equal(result_2, 234)
  
  cand_models_mix_4_ordered_3 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 3, 
                                             EndRulelength = 3,
                                             BinaryReduction = TRUE)
  result_3 <- length(cand_models_mix_4_ordered_3$candidate_models)
  expect_equal(result_3, 1940)
  
  cand_models_mix_4_ordered_4 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 4, 
                                             EndRulelength = 4,
                                             BinaryReduction = TRUE)
  result_2 <- length(cand_models_mix_4_ordered_4$candidate_models)
  expect_equal(result_2, 16574)
  
  cand_models_mix_4_ordered_5 <- testExplore(dataset = "mix_4_ordered", 
                                             StartRulelength = 5, 
                                             EndRulelength = 5,
                                             BinaryReduction = TRUE)
  result_3 <- length(cand_models_mix_4_ordered_5$candidate_models)
  expect_equal(result_3, 119548)
  
})
