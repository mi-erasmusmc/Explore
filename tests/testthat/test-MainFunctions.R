# TODO: check differences variable names with / without ''

test_that("option 1: fully pre-specified settings file works", {
  ### Tests for EXPLORE using iris dataset
  data_path <- system.file("examples", "tests", "iris.arff", package = "Explore")
  settings_path <- system.file("examples", "tests", "iris.project", package = "Explore")
  output_path <- paste0(tempdir(), "/", "Test1")
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  output_path <- paste0(output_path, "/")
  data <- farff::readARFF(data_path)
  model <- Explore::trainExplore(output_path = output_path,
                                 file_name = "iris",
                                 train_data = data,
                                 ClassFeature = "'class'",
                                 PositiveClass = '"Iris-versicolor"')
  expect_equal(class(model), "character")
  # expect_true(is.na(model), info = "Test failed because model is NA")
  expect_equal(model, "'petallength'>2.45AND'petallength'<=4.95AND'petalwidth'<=1.65")
})

test_that("option 2: pre-specified settings file with input data works", {
  ### Tests for EXPLORE using iris dataset
  data_path <- system.file("examples", "tests", "iris.arff", package = "Explore")
  settings_path <- system.file("examples", "tests", "iris.project", package = "Explore")
  output_path <- paste0(tempdir(), "/", "Test2")
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  output_path <- paste0(output_path, "/")
  data <- farff::readARFF(data_path)
  model <- Explore::trainExplore(output_path = output_path,
                                 train_data = data,
                                 settings_path = settings_path,
                                 ClassFeature = "'class'",
                                 PositiveClass = '"Iris-versicolor"')
  expect_equal(class(model), "character")
  expect_false(model == "NA", info = "Test failed because model is NA")
  expect_equal(model, "'petallength'>2.45AND'petallength'<=4.95AND'petalwidth'<=1.65") # OR "'sepallength'>4.85" ?
})

test_that("option 3: only input parameters (no settings file) works", {
  ### Tests for EXPLORE using iris dataset
  data_path <- system.file("examples", "tests", "iris.arff", package = "Explore")
  settings_path <- system.file("examples", "tests", "iris.project", package = "Explore")
  output_path <- paste0(tempdir(), "/", "Test3")
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  output_path <- paste0(output_path, "/")
  data <- farff::readARFF(data_path)
  model <- Explore::trainExplore(output_path = output_path,
                                 file_name = "iris",
                                 train_data = data,
                                 ClassFeature = "'class'",
                                 PositiveClass = '"Iris-versicolor"')
  expect_equal(class(model), "character")
  expect_false(model == "NA", info = "Test failed because model is NA")
  expect_equal(model, "'petallength'>2.45AND'petallength'<=4.95AND'petalwidth'<=1.65")
})

test_that("prediction", {
  ### Tests for EXPLORE using iris dataset
  data_path <- system.file("examples", "tests", "iris.arff", package = "Explore")
  settings_path <- system.file("examples", "tests", "iris.project", package = "Explore")
  output_path <- paste0(tempdir(), "/", "Test4")
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  output_path <- paste0(output_path, "/")
  data <- farff::readARFF(data_path)
  model <- Explore::trainExplore(output_path = output_path,
                                 train_data = data,
                                 settings_path = settings_path,
                                 ClassFeature = "'class'",
                                 PositiveClass = '"Iris-versicolor"')
  prediction <- Explore::predictExplore(model, test_data = data)
  expect_equal(class(prediction), "integer")
  expect_equal(length(prediction), nrow(data))
  expect_equal(sum(prediction %in% c(0,1) == FALSE),0)
})

test_that("compute AUC", {
  data_path <- system.file("examples","tests", "iris.arff", package = "Explore")
  settings_path <- system.file("examples", "tests", "iris.project", package = "Explore")
  output_path <- paste0(tempdir(), "/", "Test5")
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  output_path <- paste0(output_path, "/")
  data <- farff::readARFF(data_path)
  
  modelsCurve <- Explore::modelsCurveExplore(output_path = output_path,
                                             train_data = data,
                                             settings_path = settings_path,
                                             ClassFeature = "'class'",
                                             PositiveClass = '"Iris-versicolor"')
  
  auroc <- Explore::rocCurveExplore(modelsCurve = modelsCurve,
                                    data = data,
                                    labels = ifelse(data["class"] == "Iris-versicolor", 1, 0))
  
  expect_equal(class(auroc), "numeric")
  expect_true(auroc < 100)
  expect_true(auroc > 0)
})

test_that("mandatory features", {
  ### Tests for EXPLORE using iris dataset
  data_path <- system.file("examples", "tests", "iris.arff", package = "Explore")
  settings_path <- system.file("examples", "tests", "iris.project", package = "Explore")
  output_path <- paste0(tempdir(), "/", "Test1")
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  output_path <- paste0(output_path, "/")
  data <- farff::readARFF(data_path)
  model <- Explore::trainExplore(output_path = output_path,
                                 file_name = "iris",
                                 train_data = data,
                                 ClassFeature = "'class'",
                                 PositiveClass = '"Iris-versicolor"',
                                 FeatureInclude = "'sepalwidth';'sepallength'")
  expect_equal(class(model), "character")
  # expect_true(is.na(model), info = "Test failed because model is NA")
  expect_equal(model, "'sepallength'>4.9AND'sepalwidth'<=3.2AND'petalwidth'<=1.7")
})

test_that("balanced accuracy constraint ", {
  data_path <- system.file("examples", "complexity", "mix_4.arff", package = "Explore")
  output_path <- paste0(getwd(), "/", "Test1")
  dir.create(output_path)
  if (.Platform$OS.type == "windows") {
    output_path <- gsub("\\\\", "/", output_path)
  }
  output_path <- paste0(output_path, "/")
  
  data <- farff::readARFF(data_path)
  data <-as.data.frame(apply(data,2,as.numeric))

  model_without <- Explore::trainExplore(output_path = output_path,
                                 file_name = "mix_4",
                                 train_data = data,
                                 StartRulelength = 3,
                                 ClassFeature = "'outcomeCount'",
                                 PositiveClass = '"1"')
  num_without <- Explore::candidatesExplore(paste0(output_path, "mix_4", ".result"))
  
  model_with <- Explore::trainExplore(output_path = output_path,
                                         file_name = "mix_4",
                                         train_data = data,
                                         StartRulelength = 3,
                                         ClassFeature = "'outcomeCount'",
                                         PositiveClass = '"1"',
                                         BalancedAccuracy = 0.6,
                                         OutputMethod = "EVERY",
                                         Parallel = FALSE)
  num_with <- Explore::candidatesExplore(paste0(output_path, "mix_4", ".result"))
  
  expect_equal(num_without, 1940)
  expect_equal(num_with, 36)
})

test_that("Results Explore", {
  
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
                         StartRulelength = 1,
                         EndRulelength = 2,
                         CutoffMethod = "RVAC",
                         ClassFeature = "'outcomeCount'",
                         PositiveClass = "\"1\"",
                         Maximize = "ACCURACY",
                         PrintPerformance = TRUE,
                         Subsumption = TRUE)
  
  
  outputFile <- paste0(output_path, file_name, ".result")
  results_list <- resultsExplore(outputFile = outputFile)
  expect_equal(results_list$total_count_cutoff_sets, "16")
  expect_length(results_list$candidate_model, 32)

})
