# TODO: check differences variable names with / without ''

test_that("option 1: fully pre-specified settings file works", {
  
  ### Tests for EXPLORE using iris dataset
  root_folder <- system.file(package = "Explore")    
  data_path <- file.path(root_folder, "examples/iris.arff")
  settings_path <- file.path(root_folder, "examples/iris.project")
  output_path <- file.path(root_folder, "examples/output//")
  data <- farff::readARFF(data_path)
  
  # Required dependencies
  withr::local_package("farff")
  
  model <- Explore::trainExplore(output_path = output_path, 
                                 file_name = "iris", 
                                 settings_path = settings_path)
  # model <- Explore::trainExplore(output_path = output_path, 
                                   # file_name = "iris", 
                                   # train_data = data, 
                                   # ClassFeature = "'class'", 
                                   # PositiveClass = '"Iris-versicolor"')
                                   # expect_equal(class(model), "character")
  expect_true(is.na(model), info = "Test failed because model is NA")
  # expect_equal(model, "'petallength'>2.45AND'petallength'<=4.95AND'petalwidth'<=1.65")
})

test_that("option 2: pre-specified settings file with input data works", {
  ### Tests for EXPLORE using iris dataset
  root_folder <- system.file(package = "Explore")    
  data_path <- file.path(root_folder, "examples/iris.arff")
  settings_path <- file.path(root_folder, "examples/iris.project")
  output_path <- file.path(root_folder, "examples/output//")
  data <- farff::readARFF(data_path)
  
  # Required dependencies
  withr::local_package("farff")
  
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
  root_folder <- system.file(package = "Explore")    
  data_path <- file.path(root_folder, "examples/iris.arff")
  settings_path <- file.path(root_folder, "examples/iris.project")
  output_path <- file.path(root_folder, "examples/output//")
  data <- farff::readARFF(data_path)
  
  # Required dependencies
  withr::local_package("farff")
  
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
  root_folder <- system.file(package = "Explore")    
  data_path <- file.path(root_folder, "examples/iris.arff")
  settings_path <- file.path(root_folder, "examples/iris.project")
  output_path <- file.path(root_folder, "examples/output//")
  data <- farff::readARFF(data_path)
  
  # Required dependencies
  withr::local_package("farff")
  
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
  ### Tests for EXPLORE using iris dataset
  root_folder <- system.file(package = "Explore")    
  data_path <- file.path(root_folder, "examples/iris.arff")
  settings_path <- file.path(root_folder, "examples/iris.project")
  output_path <- file.path(root_folder, "examples/output//")
  data <- farff::readARFF(data_path)
  
  # Required dependencies
  withr::local_package("farff")
  
  auroc <- Explore::aurocEXPLORE(output_path = output_path, 
                                 train_data = data, 
                                 settings_path = settings_path, 
                                 ClassFeature = "'class'", 
                                 PositiveClass = '"Iris-versicolor"')
  expect_equal(class(auroc), "numeric")
  expect_true(auroc < 100)
  expect_true(auroc > 0)
})
