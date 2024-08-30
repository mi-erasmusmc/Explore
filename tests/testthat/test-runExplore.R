test_that("Test EndRulelength 4", {
  
  ### Tests for EXPLORE using iris dataset
  data_path <- system.file("examples", "tests", "iris.arff", package = "Explore")
  settings_path <- system.file("examples", "tests", "iris.project", package = "Explore")
  settings <- paste0(paste(readLines(settings_path), collapse="\n"),"\n")
  output_path <- paste0(tempdir(), "/", "TestRules4", "/")
  file_name <- "train_data"
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  train_data <- farff::readARFF(data_path)
  
  saveData(output_path, train_data, file_name)
  
  # Create selected settings 
  settings_path <- settingsExplore(settings, 
                                   output_path = output_path,
                                   file_name = file_name,
                                   train_data = train_data,
                                   OutputFile = paste0(output_path, file_name, ".result"),
                                   StartRulelength = 2,
                                   EndRulelength = 2,
                                   OperatorMethod = "EXHAUSTIVE",
                                   CutoffMethod = "RVAC",
                                   ClassFeature = "'class'",
                                   PositiveClass = '"Iris-versicolor"',
                                   FeatureInclude = "",
                                   Maximize = "ACCURACY",
                                   Accuracy = "",
                                   BalancedAccuracy = "",
                                   Specificity = "",
                                   PrintSettings = "yes",
                                   PrintPerformance = "yes",
                                   Subsumption = "yes",
                                   BranchBound = "yes",
                                   Parallel = "no")
  
  # Train EXPLORE model
  runExplore(settings_path)
  
  # Load results file
  settings <- paste(readLines(settings_path), collapse="\n")
  results <- paste(readLines(getSetting(settings, "OutputFile", type = "value")), collapse="\n")
  
  cand_models_lines <- strsplit(results, "\n")
  cand_models <- grep("Candidate model:", unlist(cand_models_lines), value = TRUE)
  cat(cand_models, sep = "\n")
  unlink(output_path, recursive = TRUE)
  
})

test_that("Test EndRulelength 5", {
  
  ### Tests for EXPLORE using iris dataset
  data_path <- system.file("examples", "tests", "iris.arff", package = "Explore")
  settings_path <- system.file("examples", "tests", "iris.project", package = "Explore")
  settings <- paste0(paste(readLines(settings_path), collapse="\n"),"\n")
  output_path <- paste0(tempdir(), "/", "TestRules5", "/")
  file_name <- "train_data"
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  train_data <- farff::readARFF(data_path)
  
  saveData(output_path, train_data, file_name)
  
  # Create selected settings 
  settings_path <- settingsExplore(settings, 
                                   output_path = output_path,
                                   file_name = file_name,
                                   train_data = train_data,
                                   OutputFile = paste0(output_path, file_name, ".result"),
                                   StartRulelength = 1,
                                   EndRulelength = 5,
                                   OperatorMethod = "EXHAUSTIVE",
                                   CutoffMethod = "RVAC",
                                   ClassFeature = "'class'",
                                   PositiveClass = '"Iris-versicolor"',
                                   FeatureInclude = "",
                                   Maximize = "ACCURACY",
                                   Accuracy = "",
                                   BalancedAccuracy = "",
                                   Specificity = "",
                                   PrintSettings = "yes",
                                   PrintPerformance = "yes",
                                   Subsumption = "yes",
                                   BranchBound = "yes",
                                   Parallel = "no")
  
  # Train EXPLORE model
  runExplore(settings_path)
  
  # Load results file
  settings <- paste(readLines(settings_path), collapse="\n")
  results <- paste(readLines(getSetting(settings, "OutputFile", type = "value")), collapse="\n")
  
  cand_models_lines <- strsplit(results, "\n")
  cand_models <- grep("Candidate model:", unlist(cand_models_lines), value = TRUE)
  cat(cand_models, sep = "\n")
  unlink(output_path, recursive = TRUE)
  
})

test_that("getDataSetPath list cases", {
  
  dataset <- "iris"
  data_path <- getDataSetPath(dataset = dataset) 
  expect_equal(data_path$ClassFeature, "'class'")
  
  
  dataset <- "binary_3"
  data_path <- getDataSetPath(dataset = dataset)
  expect_equal(data_path$ClassFeature, "'outcomeCount'")
  
  dataset <- "mix_4_ordered"
  expect_error(getDataSetPath(dataset = dataset))

})
