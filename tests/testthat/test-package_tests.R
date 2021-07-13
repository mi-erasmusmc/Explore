
### Tests for EXPLORE using  iris dataset
data <- farff::readARFF(paste0(getwd(),"/examples/iris.arff"))
output_path <- paste0(getwd(),"/examples/")

test_that("option 1: fully pre-specified settings file works", {
  model <- Explore::trainExplore(output_path = output_path, file_name = "iris", settings_path = paste0(output_path, "iris.project"))
  expect_equal(class(model), "character")
  expect_false(model == "NA", info = "Test failed because model is NA")
  expect_equal(model, "petallength>2.45ANDpetallength<=4.95ANDpetalwidth<=1.65")
})

test_that("option 2: pre-specified settings file with input data works", {
  model <- Explore::trainExplore(output_path = output_path, train_data = data, settings_path = paste0(output_path, "train_data.project"), ClassFeature = "'class'", PositiveClass = '"Iris-versicolor"',  PrintSettings = "no", PrintPerformance = "no")
  expect_equal(class(model), "character")
  expect_false(model == "NA", info = "Test failed because model is NA")
  expect_equal(model, "'sepallength'>4.85")
})

test_that("option 3: only input parameters (no settings file) works", {
  model <- Explore::trainExplore(output_path = output_path, file_name = "iris", settings_path = paste0(output_path, "iris.project"))
  expect_equal(class(model), "character")
  expect_false(model == "NA", info = "Test failed because model is NA")
  expect_equal(model, "petallength>2.45ANDpetallength<=4.95ANDpetalwidth<=1.65")
})

test_that("prediction (with model option 2)", {
  model <- Explore::trainExplore(output_path = output_path, train_data = data, settings_path = paste0(output_path, "train_data.project"), ClassFeature = "'class'", PositiveClass = '"Iris-versicolor"',  PrintSettings = "no", PrintPerformance = "no")
  prediction <- Explore::predictExplore(model, test_data = data)
  
  expect_equal(class(prediction), "integer")
  expect_equal(length(prediction), nrow(data))
  expect_equal(sum(prediction %in% c(0,1) == FALSE),0)
})

test_that("compute AUC", {
  aucroc <- Explore::aucrocExplore(output_path = output_path, train_data = data, OutputFile = paste0(output_path, "train_data.result"), settings_path = paste0(output_path, "train_data.project"), ClassFeature = "'class'", PositiveClass = '"Iris-versicolor"')

  expect_equal(class(aucroc), "numeric")
  expect_true(aucroc < 100)
  expect_true(aucroc > 0)
})
