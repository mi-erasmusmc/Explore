test_that("Convert logical to 0/1", {
  train_data <- data.frame(check.names = FALSE,
                           outcomeCount = c(FALSE,FALSE,FALSE,
                                            FALSE,FALSE,TRUE),
                           `198124209` = c(FALSE,FALSE,FALSE,
                                           FALSE,FALSE,TRUE),
                           `316139209` = c(FALSE,FALSE,FALSE,
                                           FALSE,FALSE,FALSE),
                           `316139210` = c(FALSE,FALSE,FALSE,
                                           FALSE,FALSE,FALSE))
  train_data <- convert_logical(train_data)
  expect_true(all(sapply(train_data, function(col) all(col %in% c(0, 1)))))
})

test_that("getDataSetPath list cases", {
  
  dataset <- "iris"
  data_path <- getDataSetPath(dataset = dataset) 
  expect_equal(data_path$class_feature, "'class'")
  
  dataset <- "binary_3"
  data_path <- getDataSetPath(dataset = dataset)
  expect_equal(data_path$class_feature, "'outcomeCount'")
  
  dataset <- "mix_4_ordered"
  expect_error(getDataSetPath(dataset = dataset))
  
})